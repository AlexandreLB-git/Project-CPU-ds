#include "memo.h"

MemoryHandler *memory_init(int size){
    MemoryHandler * memo = (MemoryHandler *)malloc(sizeof(MemoryHandler)) ;
    memo->total_size = size ;
    memo->free_list =  (Segment *)malloc(sizeof(Segment)); 
    memo->free_list->start = 0;
    memo->free_list->size = size ;
    memo->free_list->next = NULL ;
    memo->allocated  = hashmap_create() ; 
    memo->memory = calloc(size, sizeof(void*)) ;
    return memo ;
}

/*
Segment * find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev){
    Segment* precedent = NULL ;
    Segment* tmp=handler->free_list ;
    while(tmp!=NULL){
        if (tmp->start <= start && (tmp->size + tmp->start)>=start+size){
            if(precedent!=NULL){
                precedent->next=*prev;
                *prev=precedent;
            }
            return tmp;

        }
        precedent=tmp ;
        tmp=tmp->next ;
    }

    return NULL;
}
*/

Segment * find_free_segment(MemoryHandler *handler, int start, int size, Segment **prev) {
    if (!handler) return NULL;

    Segment *prec = NULL;
    Segment *curr = handler->free_list;

    while (curr) {
        if (curr->start <= start && (curr->start + curr->size) >= start + size) {
            if (prev) *prev = prec;
            return curr;
        }
        prec = curr;
        curr = curr->next;
    }

    return NULL;
}



int create_segment(MemoryHandler *handler, const char *name, int start, int size) {
    if (!handler || !name || size <= 0) return -1;

    Segment *prev = NULL;
    Segment *free_seg = find_free_segment(handler, start, size, &prev);
    if (!free_seg) return -1;

    // Crée et insère le nouveau segment dans la table "allocated"
    Segment *newseg = malloc(sizeof(Segment));
    newseg->start = start;
    newseg->size = size;
    newseg->next = NULL;
    hashmap_insert(handler->allocated, strdup(name), newseg);

    // Préparer les segments restants après découpage
    int before_size = start - free_seg->start;
    int after_size = (free_seg->start + free_seg->size) - (start + size);

    Segment *before = NULL, *after = NULL;

    if (before_size > 0) {
        before = malloc(sizeof(Segment));
        before->start = free_seg->start;
        before->size = before_size;
        before->next = NULL;
    }

    if (after_size > 0) {
        after = malloc(sizeof(Segment));
        after->start = start + size;
        after->size = after_size;
        after->next = NULL;
    }

    // Rechaînement dans la free list
    if (prev) {
        prev->next = before ? before : after;
    } else {
        handler->free_list = before ? before : after;
    }

    if (before) {
        before->next = after;
    }

    free(free_seg);
    return 0;
}





void fus_seg(Segment* segA, Segment* segB) {

    segA->size += segB->size;
    segA->next = segB->next;
    free(segB);
}

int remove_segment(MemoryHandler *handler, const char *name) {

    Segment *tofree = (Segment *) hashmap_get(handler->allocated, name);
    if (tofree == NULL) return -1;

    Segment *freeSeg = (Segment *) malloc(sizeof(Segment));
    freeSeg->start = tofree->start;
    freeSeg->size = tofree->size;
    freeSeg->next = NULL;
    hashmap_remove(handler->allocated, name);

    Segment *fusPrev = NULL, *fusNext = NULL;
    Segment *seg = handler->free_list;
    while (seg != NULL) {
        if (seg->start + seg->size <= freeSeg->start) fusPrev = seg;
        if (seg->start >= freeSeg->start + freeSeg->size) {
        fusNext = seg; 
        break;
            }
        seg = seg->next;
    }

    if (fusNext != NULL) {
        if (fusNext->start == freeSeg->start + freeSeg->size) fus_seg(freeSeg, fusNext);
        else freeSeg->next = fusNext;
    } else {
        freeSeg->next = NULL;
    }

    if (fusPrev != NULL) {
        if (fusPrev->start + fusPrev->size == freeSeg->start) fus_seg(fusPrev, freeSeg);
        else fusPrev->next = freeSeg;
    } else {
        handler->free_list = freeSeg;
    }

    return 0;
}
