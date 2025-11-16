#include "regexadr.h"


int matches(const char * pattern,const char * string) {
	regex_t regex ;
	int result = regcomp (&regex , pattern , REG_EXTENDED ) ;
	if (result) {
		fprintf (stderr , "Regex compilation failed for pattern: %s \n" , pattern) ;
 		return 0;
 	}
 	result = regexec (& regex , string , 0 , NULL , 0) ;
 	regfree (& regex ) ;
 	return result == 0;
 }
 
 void *immediate_addressing(CPU *cpu, const char *operand) {
    if (!matches("^-?[0-9]+$", operand)) { // On vérifie si c'est une valeur immédiate
        return NULL;
    }

    // Chercher si déjà présente dans le pool
    void *existing = hashmap_get(cpu->constant_pool, operand);
    if (existing != NULL) {
        return existing;
    }
    
    int *value = malloc(sizeof(int));
    *value = atoi(operand);  // Convertir chaîne → entier
    
	char *new_operand = strdup(operand) ;
    hashmap_insert(cpu->constant_pool, new_operand, value);
	free(new_operand);
    return value;
}

void *register_addressing(CPU *cpu, const char *operand) {
    if (!matches("^(AX|BX|CX|DX)$", operand)) { // On vérifie si l'opérande correspond à un registre valide
        return NULL;
    }

    void *value = hashmap_get(cpu->context, operand);
    return value;  // Peut être NULL si le registre n’a pas été initialisé
}

void *memory_direct_addressing(CPU *cpu, const char *operand) {
    if (!matches("^\\[[0-9]+\\]$", operand)) { // On vérifie si l'opérande correspond au format "[nombre]"
        return NULL;
    }

    int pos = atoi(operand + 1); // operand[0] = '[', on commence à 1
    

    // Récupération du segment "DS"
    Segment *ds = hashmap_get(cpu->memory_handler->allocated, "DS");
    if (ds == NULL) {
        return NULL;
    }

    // Vérifie si la position est valide
    if (pos < 0 || pos >= ds->size) {
        return NULL;
    }
	//printf("vallll : %d \n",*(int*)cpu->memory_handler->memory[ds->start + pos]);
    return cpu->memory_handler->memory[ds->start + pos];
}


void *register_indirect_addressing(CPU *cpu, const char *operand) {
    // Vérifie le format : doit être de la forme [AX], [BX], [CX], [DX]
    if (!matches("^\\[(AX|BX|CX|DX)\\]$", operand)) {
        return NULL;
    }

    // Extrait le nom du registre (sans les crochets)
    char reg_name[3]; // 2 lettres + '\0'
    strncpy(reg_name, operand + 1, 2);
    reg_name[2] = '\0'; // Null-terminate

    // Récupère l'adresse contenue dans le registre
    int *reg_value = (int*)hashmap_get(cpu->context, reg_name);
    if (reg_value == NULL) {
        fprintf(stderr, "Registre %s non trouvé\n", reg_name);
        return NULL;
    }

    int address = *reg_value;

    // Récupère le segment de données "DS"
    Segment *ds = hashmap_get(cpu->memory_handler->allocated, "DS");
    if (ds == NULL) {
        fprintf(stderr, "Segment DS introuvable\n");
        return NULL;
    }

    // Vérifie que l'adresse est bien dans le segment
    if (address < ds->start || address >= ds->start + ds->size) {
        fprintf(stderr, "Adresse %d hors du segment DS [%d - %d)\n",
                address, ds->start, ds->start + ds->size);
        return NULL;
    }

    // Accède à la mémoire
    return cpu->memory_handler->memory[address];
}


void handle_MOV(CPU* cpu, void* src, void* dest) {
    if (src == NULL || dest == NULL) {
        printf("MOV erreur : source ou destination NULL\n");
        return;
    }

    // On suppose que toutes les données sont des entiers (int)
    int value = *((int*)src);
    
    *((int*)dest) = value;
}

CPU *setup_test_environment() {
    // Initialiser le CPU
    CPU *cpu = cpu_init(1024);
    if (!cpu) {
        printf("Error: CPU initialization failed\n");
        return NULL;
    }

    // Initialiser les registres avec des valeurs spécifiques
    int *ax = (int *) hashmap_get(cpu->context, "AX");
    int *bx = (int *) hashmap_get(cpu->context, "BX");
    int *cx = (int *) hashmap_get(cpu->context, "CX");
    int *dx = (int *) hashmap_get(cpu->context, "DX");
    
    *ax = 3;
    *bx = 6;
    *cx = 100;
    *dx = 0;
	
    // Créer et initialiser le segment de données 
    if (!hashmap_get(cpu->memory_handler->allocated, "DS")) {
        create_segment(cpu->memory_handler, "DS", 0, 20);
        // Initialiser le segment de données avec des valeurs de test
        for (int i = 0; i < 10; i++) {
            int *value = (int *)malloc(sizeof(int));
            *value = i * 10 + 5; // Valeurs : 5, 15, 25, 35...
            store(cpu->memory_handler, "DS", i, value);
        }
    }
    printf("Test environment initialized.\n");
    return cpu;
}

void *resolve_addressing(CPU *cpu, const char *operand) {
    void *result = immediate_addressing(cpu, operand);
    if (result != NULL) return result;

    result = register_addressing(cpu, operand);
    if (result != NULL) return result;

    result = memory_direct_addressing(cpu, operand);
    if (result != NULL) return result;

    result = register_indirect_addressing(cpu, operand);
    return result;
}



