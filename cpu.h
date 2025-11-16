#include "parser.h"
#include <string.h>

typedef struct {
    MemoryHandler * memory_handler ; // Gestionnaire de memoire
    HashMap * context ; // Registres (AX, BX, CX, DX) + (IP, ZF, SF)
    HashMap * constant_pool ; // Table de hachage pour stocker les valeurs immédiates 
} CPU ;

CPU *cpu_init(int memory_size); // Initialise le CPU avec un gestionnaire de mémoire de taille memory_size

void cpu_destroy(CPU *cpu); // Libère la mémoire allouée pour le CPU

void* store(MemoryHandler *handler, const char *segment_name,int pos, void *data); // Stocke une valeur dans un segment de mémoire à une position donnée

void* load(MemoryHandler *handler, const char *segment_name,int pos); // récupère une valeur d'un segment de mémoire à une position donnée

void allocate_variables(CPU *cpu, Instruction** data_instructions,int data_count) ; // Alloue de la mémoire pour les variables déclarées dans les instructions .DATA

void print_data_segment(CPU *cpu) ; // Affiche le contenu du segment de données
