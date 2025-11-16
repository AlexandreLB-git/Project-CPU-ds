#include "regexadr.h"

char *trim(char *str); 

int search_and_replace(char **str, HashMap *values); // pour chaque clé k de la table de hachage values remplace chacune de ses occurrences par sa valeur (values[k])

int resolve_constants(ParserResult *result); //remplace les variables par leur adresse dans le segment de données et les étiquettes par leur adresse dans le code

void allocate_code_segment(CPU *cpu, Instruction **code_instructions,int code_count) ; //alloue et initialise le segment de codes (CS). Cette fonction doit allouer un segment de taille code_count, y stocker les instructions (préalablement prétraitées), et initialiser le registre IP à 0

int handle_instruction(CPU *cpu, Instruction *instr, void *src, void *dest); //permet d'éxécuter une instruction dans le CPU en fonction de son mnemonic 

int execute_instruction(CPU *cpu, Instruction *instr); //résout les adresses des opérandes en fonction du type d’adressage, puis délègue l’exécution proprement dite à la fonction handle_instruction.

Instruction* fetch_next_instruction(CPU *cpu); //récupère l’instruction courante à partir de l’adresse IP, puis incrémente l’IP pour pointer vers la prochaine instruction à exécuter.

int run_program(CPU *cpu); // exécute un programme préalablement chargé dans le CPU en mode pas à pas