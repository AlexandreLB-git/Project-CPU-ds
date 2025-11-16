#include "hach.h"

# define TOMBSTONE (( void *) -1)
#define TABLE_SIZE 128
#include <stdint.h>

unsigned long simple_hash(const char *str){
    unsigned long res=0 ;
    while(*str!='\0'){        
        res+=*str ;
        str++;
    }
    return res % TABLE_SIZE ;

}

HashMap *hashmap_create(){
    HashMap *res=(HashMap*)malloc(sizeof(HashMap));

    res->size=0;  //nombre d'elts présents dans table
    res->table=(HashEntry *)calloc(TABLE_SIZE,sizeof(HashEntry));
    return res;
}



int hashmap_insert(HashMap *map, const char *key, void *value) {
    if (!map || !key) return 0;

    unsigned long idx = simple_hash(key);
    for (int i = 0; i < TABLE_SIZE; i++) {
        unsigned long pos = (idx + i) % TABLE_SIZE;
        char *curr_key = map->table[pos].key;

        if (!curr_key || curr_key == TOMBSTONE || strcmp(curr_key, key) == 0) {
            if (curr_key && curr_key != TOMBSTONE) free(map->table[pos].key);  // remplace ancienne clé

            map->table[pos].key = strdup(key);
            map->table[pos].value = value;
            map->size++;
            return 1;
        }
    }

    return 0;  // Table pleine
}

void *hashmap_get(HashMap *map, const char *key) {
    if (!map || !key) return NULL;

    unsigned long idx = simple_hash(key);
    for (int i = 0; i < TABLE_SIZE; i++) {
        unsigned long pos = (idx + i) % TABLE_SIZE;
        char *curr_key = map->table[pos].key;

        if (!curr_key) return NULL;  // case vide, clé absente
        if (curr_key != TOMBSTONE && strcmp(curr_key, key) == 0) {
            return map->table[pos].value;
        }
    }

    return NULL;
}

int hashmap_remove(HashMap *map, const char *key) {
    if (!map || !key) return 0;

    unsigned long idx = simple_hash(key);
    for (int i = 0; i < TABLE_SIZE; i++) {
        unsigned long pos = (idx + i) % TABLE_SIZE;
        char *curr_key = map->table[pos].key;

        if (!curr_key) return 0;
        if (curr_key != TOMBSTONE && strcmp(curr_key, key) == 0) {
            free(map->table[pos].key);
            map->table[pos].key = TOMBSTONE;
            map->table[pos].value = NULL;
            map->size--;
            return 1;
        }
    }

    return 0;
}

void hashmap_destroy(HashMap *map) {
    if (!map) return;

    for (int i = 0; i < TABLE_SIZE; i++) {
        if (map->table[i].key && map->table[i].key != TOMBSTONE) {
            free(map->table[i].key);
            // Optionnel : free(map->table[i].value); → seulement si alloué dynamiquement
        }
    }

    free(map->table);
    free(map);
}

void affiche_hashmap(HashMap *map) {
    if (!map) return;

    for (int i = 0; i < TABLE_SIZE; i++) {
        if (map->table[i].key && map->table[i].key != TOMBSTONE) {
            printf("%s → %d\n", map->table[i].key, (int)(intptr_t)(map->table[i].value));
        }
    }
}
