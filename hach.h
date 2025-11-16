# define TOMBSTONE (( void *) -1)
#define TABLE_SIZE 128

#include <stdlib.h>
#include <string.h> 
#include <stdio.h>

typedef struct hashentry {
    char * key ; // clé de l'élément
    void * value ; 
} HashEntry ;

typedef struct hashmap {
    int size ;  // nombre d'éléments présents dans la table
    HashEntry * table ; 
} HashMap ;

unsigned long simple_hash(const char *str); //convertit une chaîne de caractères en un indice dans la table de hachage

HashMap *hashmap_create(); //alloue dynamiquement une table de hachage et initialise ses cases à NULL.

int hashmap_insert(HashMap *map, const char *key, void *value); // insére un élément dans la table de hachage.

void *hashmap_get(HashMap *map, const char *key); //retourne l'élément associé à une clé donnée dans la table de hachage.

int hashmap_remove(HashMap *map, const char *key); // supprime un élément de la table de hachage. 

void hashmap_destroy(HashMap *map); // libère la mémoire allouée pour la table de hachage.

void affiche_hashmap(HashMap *map); // affiche le contenu de la table de hachage.
