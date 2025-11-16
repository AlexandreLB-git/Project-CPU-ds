#include "segcode.h"

char *trim(char *str) {
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') str++;

    char *end = str + strlen(str)- 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        *end = '\0';
        end--;
    }
    return str;
}

int search_and_replace(char **str, HashMap *values) {
    if (!str || !*str || !values) return 0;
  
    int replaced = 0;
    char *input = *str;

    // Iterate through all keys in the hashmap
    for (int i = 0; i < values->size; i++) {
        if (values->table[i].key && values->table[i].key != (void *)-1) {
            char *key = values->table[i].key;
            
            int value = *(int *)(values->table[i].value);
            
    
            // Find potential substring match
            char *substr = strstr(input, key);
            if (substr) {
                // Construct replacement buffer
                char replacement[64];
                snprintf(replacement, sizeof(replacement), "%d", value);
    
                // Calculate lengths
                int key_len = strlen(key);
                int repl_len = strlen(replacement);
                int remain_len = strlen(substr + key_len);

                // Create new string
                char *new_str = (char *)malloc(strlen(input)- key_len + repl_len + 1);
                strncpy(new_str, input, substr- input);
                new_str[substr- input] = '\0';
                strcat(new_str, replacement);
                strcat(new_str, substr + key_len);
    
                // Free and update original string
                free(input);
                *str = new_str;
                input = new_str;
    
                replaced = 1;
            }
        }
    }
    
    // Trim the final string
    if (replaced) {
        char *trimmed = trim(input);
        if (trimmed != input) {
            memmove(input, trimmed, strlen(trimmed) + 1);
        }
    }
    
    return replaced;
}



int resolve_constants(ParserResult *result){
    if (result==NULL) return 0;

    // On itère à travers toutes les instructions de la section .CODE et on applique search_and_replace
    for (int i = 0; i < result->code_count; i++) {
        Instruction *ins = result->code_instructions[i];
        if(ins->operand2 == NULL){
            char *operand = strdup(ins->operand1);
            if (search_and_replace(&operand, result->labels)) {
                free(ins->operand1);
                ins->operand1 = operand;
            } else {
                free(operand);
            }
        }else{
        	
            if(matches("^[a-zA-Z]+$",ins->operand2)){
            	int len = strlen(ins->operand2);
    			char* operand = malloc(len + 3);  // +1 for '[', +1 for ']', +1 for '\0'
    			operand[0] = '[';
    			strcpy(operand + 1, ins->operand2);
    			operand[len + 1] = ']';
    			operand[len + 2] = '\0';
    			if (search_and_replace(&operand, result->memory_locations)) {
                	free(ins->operand2);
                	ins->operand2 = strdup(operand);
            	} else {
                	free(operand);
            	}
            	}
            
        }

    }

    return 1;
}


void allocate_code_segment(CPU *cpu, Instruction **code_instructions, int code_count) {
    if (!cpu || !code_instructions || code_count <= 0) {
        printf("Paramètres invalides pour allocate_code_segment\n");
        return;
    }

    // Créer le segment CS

    if (!create_segment(cpu->memory_handler, "CS", cpu->memory_handler->total_size - code_count , code_count)) {
        printf("Erreur : création du segment CS\n");
        return;
    }

    //  stocker chaque Instruction dans la mémoire CPU
    for (int i = 0; i < code_count; i++) {
        if(store(cpu->memory_handler, "CS", i, (void *)code_instructions[i]) == NULL) {
            printf("Erreur : stockage de l'instruction dans le segment CS\n");
            return;
        }
    }

    // initialiser IP à 0 
   int *ip = hashmap_get(cpu->context, "IP");
    if (ip) *ip = 0;
}



int handle_instruction(CPU *cpu, Instruction *instr, void *src, void *dest){

   
    int *ip = (int *)hashmap_get(cpu->context, "IP"); // On récupère la valeur de l'IP


    // On vérifie à quelle type d'instruction on a affaire
    
    if (strncmp(instr->mnemonic, "MOV",3) == 0) {
        handle_MOV(cpu, src, dest);
        *ip += 1; // Incrémente l'IP après l'exécution de MOV
        return 1;
    }

    if (strncmp(instr->mnemonic, "ADD",3) == 0) { //additionne les valeurs de la source et de la destination, puis stocke le résultat dans la destination.

        if (src == NULL || dest == NULL) {
            printf("Erreur : source ou destination NULL\n");
            return 0;
        }

        int *src_val = (int *)src; //seg fault vient d'ici
        int *dest_val = (int *)dest;
        *dest_val += *src_val;

        *ip += 1; // Incrémente l'IP après l'exécution de ADD
        return 1;
    }

    if (strncmp(instr->mnemonic, "CMP",3) == 0){

        int *src_val = (int *)src;
        int *dest_val = (int *)dest;
        if (*src_val == *dest_val) {
            *(int *)hashmap_get(cpu->context, "ZF") = 1; 
        }
        if (*dest_val < *src_val) {
            *(int *)hashmap_get(cpu->context, "SF") = 1; 
        }

        *ip += 1; // Incrémente l'IP après l'exécution de CMP
        return 1;
    }


    if (strncmp(instr->mnemonic, "JMP",3) == 0){

        int* adress = (int*)dest;     
        *ip = *adress; // On met à jour l'IP avec la nouvelle adresse
        
        return 1;    
    }

    if (strncmp(instr->mnemonic, "JZ",2) == 0){
        if(*(int *)hashmap_get(cpu->context, "ZF") == 1) {
           
            int* adress = (int*)dest; 
            *ip = *adress; // On met à jour l'IP avec la nouvelle adresse
            return 1;   
        }
        return 0;
    }

    if(strncmp(instr->mnemonic, "JNZ",3) == 0){
        if(*(int *)hashmap_get(cpu->context, "ZF") == 0){
            
            int* adress = (int*)dest; 
            *ip = *adress; // On met à jour l'IP avec la nouvelle adresse
            return 1;    
        }
        return 0;
    }

    if(strncmp(instr->mnemonic, "HALT",4) == 0){ // termine l’exécution en positionnant le pointeur d’instruction IP à la fin du segment de codes
        
        Segment *cs = hashmap_get(cpu->memory_handler->allocated, "CS");
        *ip = cs->start + cs->size; // On met à jour l'IP à la fin du segment de code
        return 1;
    }

    return 0; // Si aucune instruction ne correspond
}


int execute_instruction(CPU *cpu, Instruction *instr){
    if (cpu == NULL || instr == NULL) return 0;

    // On résout les adresses des opérandes en fonction du type d’adressage
    void *src = NULL;
    void *dest = NULL;
    if(instr->operand1 != NULL) { // On vérifie si l'opérande 1 est présent
        dest = resolve_addressing(cpu, instr->operand1); 
    }
    if (instr->operand2 != NULL) { // On vérifie si l'opérande 2 est présent
        src = resolve_addressing(cpu, instr->operand2);     
    }

    // On gère l'instruction
    if(handle_instruction(cpu, instr, src, dest) == 0) {
        printf("Erreur lors de l'exécution de l'instruction : %s %s %s\n", instr->mnemonic, instr->operand1, instr->operand2);
        return 0; 

    }
    return 1;
}


Instruction* fetch_next_instruction(CPU *cpu){
    if(cpu == NULL) return NULL;

    // On vérifie que l'IP est valide
    int *ip = (int *)hashmap_get(cpu->context, "IP");
    if (ip == NULL) { 
        printf("Registre IP introuvable\n");
        return NULL;
    }

    // On vérifie qu'on est toujours dans le segment de code CS
    Segment *cs = hashmap_get(cpu->memory_handler->allocated, "CS");
    if (cs == NULL) {
        printf("Segment CS introuvable\n");
        return NULL;
    }
    if (cs->start + *ip < cs->start || *ip + cs->start >= cs->start + cs->size) {
        printf("Adresse IP hors du segment CS [%d - %d]\n", cs->start, cs->start + cs->size);
        return NULL;
    }

    // On récupère l'instruction à l'adresse IP
    int *index = (int *)load(cpu->memory_handler, "CS", *ip);
    
    Instruction *instr = (Instruction *)load(cpu->memory_handler, "CS", *ip);;

    return instr;
}


int run_program(CPU *cpu){

    if (cpu == NULL) return 0;

    //On affiche l'état initial  afficher l’état initial du CPU (segment de données, registres):
    printf("État initial du CPU :\n");
    printf("Registres :\n");
    printf("AX: %d, BX: %d, CX: %d, DX: %d, IP: %d, ZF: %d, SF: %d\n",
        *(int *)hashmap_get(cpu->context, "AX"),
        *(int *)hashmap_get(cpu->context, "BX"),
        *(int *)hashmap_get(cpu->context, "CX"),
        *(int *)hashmap_get(cpu->context, "DX"),
        *(int *)hashmap_get(cpu->context, "IP"),
        *(int *)hashmap_get(cpu->context, "ZF"),
        *(int *)hashmap_get(cpu->context, "SF"));
        
    printf("Segment de données :\n");
    print_data_segment(cpu); // Affiche le contenu du segment de données
    
    // On exécute les instructions jusqu'à ce qu'il n'y en ait plus ou que l'utilisateur choisisse de quitter
    
    int *ip = (int *)hashmap_get(cpu->context, "IP"); 

    Instruction *instr=  load(cpu->memory_handler, "CS",*ip); // récupère une valeur d'un segment de mémoire à une position donnée
    
    while (instr != NULL) {

        // On exécute l'instruction

        printf("Exécution de l'Instruction : %s %s %s\n", instr->mnemonic, instr->operand1, instr->operand2);

        execute_instruction(cpu, instr);

        printf("\n"); // Ajout d'une ligne vide pour la lisibilité

        printf("État actuel du CPU :\n");
        printf("AX: %d, BX: %d, CX: %d, DX: %d, IP: %d, ZF: %d, SF: %d\n",
            *(int *)hashmap_get(cpu->context, "AX"),
            *(int *)hashmap_get(cpu->context, "BX"),
            *(int *)hashmap_get(cpu->context, "CX"),
            *(int *)hashmap_get(cpu->context, "DX"),
            *(int *)hashmap_get(cpu->context, "IP"),
            *(int *)hashmap_get(cpu->context, "ZF"),
            *(int *)hashmap_get(cpu->context, "SF"));

        printf("\n"); // Ajout d'une ligne vide pour la lisibilité 
        
        //on permet à l’utilisateur de contrôler le déroulement
        printf("Appuyez sur Entrée pour continuer ou 'q' pour quitter l'éxécution\n");
        char c[2];
        fgets(c, sizeof(c), stdin); // On lit l'entrée de l'utilisateur
        if (c[0] == 'q') {
            break; // Quitte l'exécution si l'utilisateur appuie sur 'q'
        }
        instr = fetch_next_instruction(cpu);
    }

    printf("\n"); // Ajout d'une ligne vide pour la lisibilité

    // On affiche l'état final du CPU
    printf("État final du CPU :\n");
    printf("AX: %d, BX: %d, CX: %d, DX: %d, IP: %d, ZF: %d, SF: %d\n",
        *(int *)hashmap_get(cpu->context, "AX"),
        *(int *)hashmap_get(cpu->context, "BX"),
        *(int *)hashmap_get(cpu->context, "CX"),
        *(int *)hashmap_get(cpu->context, "DX"),
        *(int *)hashmap_get(cpu->context, "IP"),
        *(int *)hashmap_get(cpu->context, "ZF"),
        *(int *)hashmap_get(cpu->context, "SF")); 


    return 1; // Indique que le programme a été exécuté avec succès

}
