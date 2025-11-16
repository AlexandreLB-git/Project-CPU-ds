#include "hach.h"
typedef struct segment {
    int start ; // Position de debut (adresse) du segment dans la memoire
    int size ; // Taille du segment en unites de memoire
    struct segment * next ; // Pointeur vers le segment suivant dans la liste chainee
} Segment ;


typedef struct memoryHandler {
    void ** memory ; // Tableau de pointeurs vers la memoire allouee
    int total_size ; // Taille totale de la memoire geree
    Segment * free_list ; // Liste chainee des segments de memoire libres
    HashMap * allocated ; // Table de hachage (nom, segment)
} MemoryHandler ;

MemoryHandler *memory_init(int size); // Initialise le gestionnaire de memoire

Segment * find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev) ; // retourne le segment libre (s'il existe) qui peut contenir le segment à allouer 


int create_segment(MemoryHandler *handler, const char *name,int start, int size); // alloue dynamiquement un segment de memoire de taille size à l'adresse mémoire start 

void fus_seg(Segment* segA, Segment* segB);  // Fusionne deux segments adjacents

int remove_segment(MemoryHandler *handler, const char *name); // supprime le segment de memoire alloué et l'ajoute à la liste des segments libres
