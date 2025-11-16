#include "cpu.h"


CPU *cpu_init(int memory_size){

    //On initialise le CPU
    CPU * cpu = (CPU*)malloc(sizeof(CPU)) ;

    // On initialise le gestionnaire de mémoire
    cpu->memory_handler = memory_init(memory_size);

    // On initialise les hashmaps pour le contexte et le pool de constantes
    cpu->context = hashmap_create() ;
	cpu->constant_pool = hashmap_create() ;

    // Création et initialisation des registres généraux
    const char *reg_names[] = {"AX", "BX", "CX", "DX", "IP", "ZF", "SF"};
    for (int i = 0; i < 7; i++) {
        int *reg = malloc(sizeof(int));
        
        *reg = 0;
        
        hashmap_insert(cpu->context, strdup(reg_names[i]), reg);

    }

    return cpu ;

}


void cpu_destroy(CPU *cpu){

    //On destroy memory handler
    MemoryHandler * memo=cpu->memory_handler;
    hashmap_destroy(memo->allocated);
    
    while(memo->free_list){
        Segment*tmp= memo->free_list->next;
        free(memo->free_list);
        memo->free_list=tmp;
    }
    
    //On libère la mémoire allouée par memory
    for (int i=0;i<memo->total_size;i++){
    	free(memo->memory[i]);
    	}
    free(memo->memory);
    
    free(cpu->memory_handler);

    //on destroy context
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
   
	free(cpu);

} 


void* store(MemoryHandler *handler, const char *segment_name,int pos, void *data){
    Segment* segment =(Segment*)hashmap_get(handler->allocated, segment_name);

    if(segment==NULL){
        return NULL;
    }
    
    if (pos < 0 || pos >= segment->size){
    	return NULL ;
    	}
    
    int memory_position = segment->start + pos;

    handler->memory[memory_position] = data ;

    return handler->memory[memory_position] ;
    
}


void* load(MemoryHandler *handler, const char *segment_name,int pos) {
	
    Segment *segment = (Segment *)hashmap_get(handler->allocated, segment_name);// On récupère le segment alloué depuis la hashmap
	
    if (segment == NULL) {
        return NULL;
    }

    if (pos < 0 || pos >= segment->size) { // Vérifie que la position est dans les limites
        return NULL;
    }

    int memory_position = segment->start + pos;
	
    return handler->memory[memory_position];
}


void allocate_variables(CPU *cpu, Instruction** data_instructions, int data_count) {

    int total_size = 0;

     // Étape 1 : calcul de la taille totale nécessaire
     for (int i = 0; i < data_count; i++) {
        Instruction *inst = data_instructions[i];

        int count = 1;
        for (char *p = inst->operand2; *p; p++) {
            if (*p == ',') count++;
        }

        total_size += count;
    }


    // Étape 2 : trouver un segment libre à partir de l'adresse 0
    Segment *prev = NULL;
    Segment *free_seg = find_free_segment(cpu->memory_handler, 0, total_size, &prev);

    if (!free_seg) {
        printf("Pas de segment libre \n");
        return;
    }

    int start_address = free_seg->start;

    // Étape 3 : créer le segment "DS"
    if (!create_segment(cpu->memory_handler, "DS", start_address, total_size)) {
        return;
    }
    
    // Étape 4 : Remplissage du segment de données avec les valeurs déclarées

    int offset = 0; // position relative dans le segment "DS" où les données seront stockées

    for (int i = 0; i < data_count; i++) { // on parcourt chaque instruction dans data_instructions
        Instruction *inst = data_instructions[i];
        char *operand_copy = strdup(inst->operand2);
		char *token = strtok(operand_copy, ",");
		
		while (token != NULL) {
    		int val = atoi(token);
    		int *stored_val = (int*)malloc(sizeof(int));
    		*stored_val = val;
			
    		store(cpu->memory_handler, "DS", offset, stored_val);
    		offset++;

    		token = strtok(NULL, ","); //
		}
	}
}


void print_data_segment(CPU *cpu) {
    Segment *ds = hashmap_get(cpu->memory_handler->allocated, "DS");

    if (ds == NULL) {
        printf("Le segment 'DS' n'existe pas.\n");
        return;
    }

    printf("Contenu du segment 'DS' [%d à %d]:\n", ds->start, ds->start + ds->size - 1);

    for (int i = 0; i < ds->size; i++) {
        int index = ds->start + i;
        void *ptr = cpu->memory_handler->memory[index];

        if (ptr != NULL) {
            int val = *(int *)ptr;
            printf("  [DS+%d] = %d\n", i, val);  // Affichage relatif (offset dans DS)
        } else {
            printf("  [DS+%d] = NULL\n", i);
        }
    }
}











