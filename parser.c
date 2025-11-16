#include "parser.h"

Instruction *parse_data_instruction(const char *line, HashMap *memory_locations,int* cmp){
    // cmp: compteur pour suivre le nombre total d'opérandes

    Instruction * ins= (Instruction*)malloc(sizeof(Instruction)); // on alloue un espace mémoire pour une instruction

    char  mnemonic[256] ; 
    char operand1[256] ; 
    char operand2[256] ; 

    // On utilise sscanf pour extraire les parties de la ligne
    sscanf(line,"%s %s %s", mnemonic,operand1,operand2 );

    // On remplit la structure Instruction
    ins->mnemonic = strdup(mnemonic);
    ins->operand1 = strdup(operand1);
    ins->operand2 = strdup(operand2);

    //On met à jour memory_locations:

    char *op = strdup(ins->operand2);
    char *token = strtok(op, ",");
    int compteur = 0;

    //on compte le nombre de valeurs séparées d'une virgule dans operand2
    while (token != NULL) {
        compteur++; // on incrémente le compteur d'opérandes
        token = strtok(NULL, ","); // on passe à la valeur suivante
    }
    
    char *tmp = strdup(ins->mnemonic);

    int *address = malloc(sizeof(int));
    *address = *cmp;
    
    hashmap_insert(memory_locations, strdup(ins->mnemonic), address); // on insère l'opérande dans memory_locations

    *cmp+=compteur; //on ajoute 1 au compteur d'opérandes

    // On libère la mémoire allouée pour op
    free(op);

    return ins;
}

Instruction *parse_code_instruction(const char *line, HashMap*labels, int code_count){
    Instruction* ins=(Instruction*)(malloc(sizeof(Instruction)));

    char buffer[256] ;
    char s1[256] ; 
    char s2[256] ; 
    char s3[256] ;

    // On utilise sscanf pour extraire les parties de la ligne
   sscanf(line,"%s %s %s", s1,s2,s3 );


    if(s1[strlen(s1)-1] == ':'){ //il y a une étiquette
            ins->mnemonic = strdup(s2);
            if(s3[2] == ','){  // on vérifie le format de s3: avec ou sans virgule
                strncpy(buffer,s3,2);
                buffer[2] = '\0';
                ins->operand1 = strdup(buffer);
                ins->operand2 = strdup(&s3[3]); 
            }
            else{
                ins->operand1 = strdup(s3);
            }

            // on remplit labels

            char *tmp = strtok(s1, ":"); // on enlève le ':' de l'étiquette
            char *l = strdup(tmp); // fait le malloc + copie + \0
            int *code_count_ptr = malloc(sizeof(int)); // on alloue un espace mémoire pour le code_count
            *code_count_ptr = code_count;

            hashmap_insert(labels, l, code_count_ptr); // ajout de l'étiquette dans le label
            
        }

    else{ // pas d'étiquette
        ins->mnemonic = strdup(s1);
        if(s2[2] == ','){  // on vérifie le format de s2: avec ou sans virgule
                ins->operand1 = strdup(strcat(strncpy(buffer,s3,2),"\0"));
                ins->operand2 = strdup(&s2[3]);   
        }
        else{
            ins->operand1 = strdup(s2);
        } 

    }

    return ins;
}



/*
Instruction *parse_code_instruction(const char *line, HashMap*labels, int code_count){
    Instruction* ins=(Instruction*)(malloc(sizeof(Instruction)));

    char buffer[256] ;
    char s1[256] ; 
    char s2[256] ; 
    char s3[256] ;

    memset(buffer, 0, sizeof(buffer)); // on vide le buffer

    // On utilise sscanf pour extraire les parties de la ligne
   sscanf(line,"%s %s %s", s1,s2,s3 );


    if(s1[strlen(s1)-1] == ':'){ //il y a une étiquette
            ins->mnemonic = strdup(s2);
            if(s3[2] == ','){  // on vérifie le format de s3: avec ou sans virgule
                strncpy(buffer,s3,2);
                buffer[2] = '\0';
                ins->operand1 = strdup(buffer);
                ins->operand2 = strdup(&s3[3]); 
            }
            else{
                ins->operand1 = strdup(s3);
            }

            // on remplit labels

            char *tmp = strtok(s1, ":"); // on enlève le ':' de l'étiquette
            char *l = strdup(tmp); // fait le malloc + copie + \0
            int *code_count_ptr = malloc(sizeof(int)); // on alloue un espace mémoire pour le code_count
            *code_count_ptr = code_count;

            hashmap_insert(labels, l, code_count_ptr); // ajout de l'étiquette dans le label
            
        }

        else { // pas d'étiquette
            ins->mnemonic = strdup(s1);
        
            if (strchr(s2, ',')) {
                memset(buffer, 0, sizeof(buffer));
                strncpy(buffer, s2, strchr(s2, ',') - s2);
                buffer[strchr(s2, ',') - s2] = '\0';
        
                ins->operand1 = strdup(buffer);
                ins->operand2 = strdup(strchr(s2, ',') + 1);  // tout ce qui suit la virgule
            } else {
                
                ins->operand1 = strdup(s2);
                if(s3==NULL){
                    ins->operand2 = NULL; // sécurité si s3 est vide
                }
                else{
                    memset(buffer, 0, sizeof(buffer));
                    strncpy(buffer,s3,2);
                    buffer[2] = '\0';
                    ins->operand1 = strdup(buffer);
                }
            }
        }

    return ins;
} */


ParserResult *parse(const char *filename){
    FILE* f=fopen(filename,"r");

    char buffer[256] ;
    
    ParserResult *res=(ParserResult *)malloc(sizeof(ParserResult));
    int data_c=0; // Nombre d’instructions .DATA
    int code_c=0 ; // Nombre d’instructions .CODE

    //lire tout le fichier une première fois pour connaitre la taille des tableaux d'instructions
    fgets(buffer, 256, f);
   
    if(strncmp ( buffer , ".DATA", 5 )==0){ // On regarde si on est dans la section .DATA
        
        while(fgets(buffer, 256, f) != NULL){ 
            if (strncmp(buffer,".CODE",5)==0){
                break;
            }
            data_c++;
            
        }
    }
    if(strncmp(buffer,".CODE",5)==0){ // On regarde si on est dans la section .CODE
        while(fgets(buffer, 256, f) != NULL){
            code_c++;
        }
    }
    
    
    //On revient au début du fichier en le fermant et en le réouvrant
    fclose(f);
    f=fopen(filename,"r");

    Instruction ** data_i = (Instruction**)malloc(sizeof(Instruction*)*data_c); // Tableau d’instructions .DATA
    
    Instruction ** code_i = (Instruction**)malloc(sizeof(Instruction*)*code_c); // Tableau d’instructions .CODE
    
    HashMap * labels = hashmap_create(); // labels -> indices dans code_instructions
    HashMap * memory_locations = hashmap_create();
   
    // on remet les compteurs à 0 pour l'ajout des instructions
    data_c=0;
    code_c=0;
    int cmp=0; //nb total d'opérandes

    fgets(buffer, 256, f);
    if(strncmp ( buffer , ".DATA", 5 )==0) { // On regarde si on est dans la section .DATA
        while(fgets(buffer, 256, f) != NULL ){
            if(strncmp(buffer,".CODE",5)==0){
                break;
            }
            data_i[data_c]= parse_data_instruction(buffer, memory_locations,&cmp);
            data_c++;
        }
    }
    if(strncmp(buffer,".CODE",5)==0){ // On regarde si on est dans la section .CODE
        while(fgets(buffer, 256, f) != NULL){
            code_i[code_c] = parse_code_instruction(buffer, labels, code_c);
            code_c++;

        }
    }

    res->data_instructions = data_i;
    res-> data_count = data_c;
    res-> code_instructions = code_i ;
    res-> code_count = code_c;
    res-> labels = labels;
    res-> memory_locations = memory_locations;

    fclose(f);
    
    return res;
}


void parser_destroy(ParserResult *parser){

    if (parser == NULL) return;

    // Libérer les instructions de la section .DATA
    for (int i = 0; i < parser->data_count; i++) {
        free(parser->data_instructions[i]->mnemonic);
        free(parser->data_instructions[i]->operand1);
        free(parser->data_instructions[i]->operand2);
        free(parser->data_instructions[i]);
    }
    free(parser->data_instructions);

    // Libérer les instructions de la section .CODE
    for (int i = 0; i < parser->code_count; i++) {
        free(parser->code_instructions[i]->mnemonic);
        free(parser->code_instructions[i]->operand1);
        free(parser->code_instructions[i]->operand2);
        free(parser->code_instructions[i]);
    }
    free(parser->code_instructions);

    // Libérer les labels et memory_locations
    hashmap_destroy(parser->labels);
    hashmap_destroy(parser->memory_locations);
    

    // Libérer la structure ParserResult
    free(parser);
}



void afficher_tab_ins(Instruction ** tab, int taille){
    for(int i=0; i < taille; i++){
        printf("%s %s %s \n",(tab[i])->mnemonic,(tab[i])->operand1, (tab[i])->operand2);
    }
}



  
