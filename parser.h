#ifndef __PARSER_H__
#define  __PARSER_H__

#include "memo.h"

typedef struct {
    char * mnemonic ; // Instruction mnemonic (ou nom de variable pour .DATA)
    char * operand1 ; // Premier operande (ou type pour .DATA)
    char * operand2 ; // Second operande (ou initialisation pour .DATA)
} Instruction ;

typedef struct {
    Instruction ** data_instructions ; // Tableau d’instructions .DATA
    int data_count ; // Nombre d’instructions .DATA
    Instruction ** code_instructions ; // Tableau d’instructions .CODE
    int code_count ; // Nombre d’instructions .CODE
    HashMap * labels ; // labels -> indices dans code_instructions
    HashMap * memory_locations ; // noms de variables -> adresse memoire
} ParserResult ;

char* createStringWithoutLastChar(const char* original); 

Instruction *parse_data_instruction(const char *line, HashMap *memory_locations,int* cmp); // analyse et stocke une ligne de la section .DATA 

Instruction *parse_code_instruction(const char *line, HashMap *labels, int code_count); //  analyse et stocke une ligne de la section .CODE 

ParserResult *parse(const char *filename); //  analyse le fichier d’entrée et renvoie un ParserResult asssocié 

void parser_destroy(ParserResult *parser); //  libère la mémoire allouée pour le ParserResult

void afficher_tab_ins(Instruction ** tab, int taille); //fonction affichage tableau d'instructions

#endif
