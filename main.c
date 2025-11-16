#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "segcode.h"

int main(){

    
    //TEST DE PARSER.C
    //Création du parser à partir du fichier test_parse.txt
    ParserResult *test = parse("test_parse.txt");

    //Affichage des instructions de la section .DATA et .CODE
    printf("Instructions .DATA :\n");
    afficher_tab_ins(test->data_instructions, test->data_count);
    printf("\n");

    printf("Instructions .CODE :\n");
    afficher_tab_ins(test->code_instructions, test->code_count);
    printf("\n");

    //Affichage des labels et des emplacements mémoire
    printf("Emplacements mémoire:\n");
    affiche_hashmap(test->memory_locations);
    printf("\n");

    
    printf("Labels :\n");
    affiche_hashmap(test->labels);
    printf("\n");
    
  

    //TEST DE CPU.C
    CPU *cpu = cpu_init(100); // Initialise le CPU avec 100 unités de mémoire

    allocate_variables(cpu, test->data_instructions,test->data_count); // Alloue la mémoire pour les variables déclarées dans les instructions .DATA
   
    print_data_segment(cpu) ;

    printf("Emplacements mémoire :\n");
    affiche_hashmap(test->memory_locations);
    printf("\n");


    printf("Labels :\n");
    affiche_hashmap(test->labels);
    printf("\n");
    
    cpu_destroy(cpu); // Libére la mémoire allouée pour le CPU
    

    parser_destroy(test); // Libére la mémoire allouée pour le ParserResult : NE FONCTIONNE PAS surement on essaie de free un truc NULL mais je vois pas où
    


/*
    //TEST DE REGEXADR.C
    CPU *cpu = setup_test_environment();
    if (!cpu) return 1;
	
    // 1. Adresse immédiate -> registre
    void *src1 = immediate_addressing(cpu, "42");
    void *dest1 = hashmap_get(cpu->context, "AX");
    handle_MOV(cpu, src1, dest1);
    printf("AX après MOV immédiat: %d\n", *(int *)dest1);
	
    // 2. Registre -> registre
    void *src2 = register_addressing(cpu, "BX");
    void *dest2 = hashmap_get(cpu->context, "CX");
    handle_MOV(cpu, src2, dest2);
    printf("CX après MOV registre -> registre (depuis BX): %d\n", *(int *)dest2);

    // 3. Mémoire directe -> registre
    void *src3 = memory_direct_addressing(cpu, "[2]"); // Récupère la valeur à DS[2] (devrait être 25)
    void *dest3 = hashmap_get(cpu->context, "DX");
    handle_MOV(cpu, src3, dest3);
    printf("DX après MOV mémoire directe -> registre: %d\n", *(int *)dest3);

    // 4. Registre indirect -> registre
    // BX contient 6, donc on accède à DS[6]
    void *src4 = register_indirect_addressing(cpu, "[BX]");
    void *dest4 = hashmap_get(cpu->context, "AX");
    handle_MOV(cpu, src4, dest4);
    printf("AX après MOV registre indirect [BX] -> AX : %d\n", *(int *)dest4);
	
	cpu_destroy(cpu) ;

*/
/*
    //TEST DE SEGCODE.C

    // Création du parser à partir du fichier test_parse.txt
    ParserResult *result = parse("test_parse1.txt");
    
    CPU *cpu= cpu_init(100); // Initialisation du CPU avec 100 unités de mémoire
  
    // Allocation de la mémoire pour les variables déclarées dans les instructions .DATA
	allocate_variables(cpu, result->data_instructions,result->data_count); 

    // Résolution des constantes dans les instructions de code
    resolve_constants(result) ; 
 
    //Allocation des instructions de code dans le segment CS après avoir résolu les constantes
    allocate_code_segment(cpu, result->code_instructions, result->code_count);
	
    // On lance le programme
    run_program(cpu);

    // On libère la mémoire allouée pour le parser et le CPU
    parser_destroy(result); 
    cpu_destroy(cpu);

    

    /* Tenter de l'integrer a votre projet. 
Le faire compiler et l'executer sans erreur ni fuite 
de memoire ;) */
/*
    MemoryHandler* MH = memory_init(17); //Q2.1
    create_segment(MH, "MS", 0, 4); //Q2.3
    create_segment(MH, "AS", 5, 2); //Q2.3
    create_segment(MH, "DS", 8, 6); //Q2.3
    create_segment(MH, "CS", 14, 2); //Q2.3

    //Affichage de la liste chainee des segments libres
 */
  
    return 0;

}
