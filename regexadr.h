#include "cpu.h"
#include <regex.h>
#include <stdio.h>

int matches(const char *pattern,const char *string) ; // Vérifie si le motif correspond à une chaîne donnée

void *immediate_addressing(CPU *cpu, const char *operand) ; // Vérifie si l'opérande est une valeur immédiate
// Si oui, renvoie la valeur correspondante

void *register_addressing(CPU *cpu, const char *operand) ; // Vérifie si l'opérande est un registre valide

void *memory_direct_addressing(CPU *cpu, const char *operand) ; // Vérifie si l'opérande est au format adressage direct

void *register_indirect_addressing(CPU *cpu, const char *operand) ; // Vérifie si l'opérande est au format adressage indirect par registre

void handle_MOV(CPU* cpu, void* src, void* dest) ; // Gère l'instruction MOV entre deux opérandes

CPU *setup_test_environment() ; // Initialise l'environnement de test pour le CPU

void *resolve_addressing(CPU *cpu, const char *operand) ; // identifie automatiquement le mode d’adressage d’un opérande
