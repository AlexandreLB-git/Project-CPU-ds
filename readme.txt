# Projet LU2IN006

Ce projet a consisté à coder un simulateur de processeur (CPU) en langage C, capable d’interpréter un langage assembleur simplifié. Le programme gère différents segments mémoire , un ensemble de registres, plusieurs modes d’adressage, ainsi que l’exécution d’instructions classiques telles que MOV, ADD, CMP,  JMP...
L’objectif était de reproduire une version simplifiée du fonctionnement d’un CPU, tout en respectant les contraintes de gestion mémoire et de traitement séquentiel de ce dernier.

Notre projet est composé de 14 fichiers de code, un Makefile et de 3 fichiers textes :

## Fichiers du projet

### 1. Fichiers répondant aux questions de l'Exercice 1– Implémentation d’une table de hachage générique

'hach.h' et 'hach.h':
Ces fichiers contiennent les déclarations des structures de HashEntry et Hashmap,
les signatures des fonctions qui manipulent une Hashmap (création, insertion, suppression, destruction, affichage d'une Hashmap) et leurs définitions dans le .c.


### 2. Fichiers répondant aux questions de l'Exercice 2– Gestion dynamique de la mémoire

'memo.h' et 'memo.c':
Ces fichier contiennent les déclarations des structures de Segment et MemoryHandler, les signatures des fonctions qui manipulent ces structures et leurs définitions (dans le .c).

### 3. Fichiers répondant aux questions de l' Exercice 3– Conception d’un parser pour un langage pseudo-assembleur

'parser.h' et 'parser.c':
Ces fichiers contiennent les déclarations des structures d' Instruction et ParserResult, les signatures des fonctions qui manipulent ces structures et leurs définitions (dans le .c).

### 4. Fichiers répondant aux questions de l'Exercice 4– Allocation d’un segment de données

'cpu.h' et 'cpu.c':

Ces fichiers contiennent la déclaration de structure de CPU et les signatures et défintions des fonctions qui manipulent des CPU.


### 5. Fichiers répondant aux questions de l'Exercice 5– Expressions régulières et résolution d’adressage

'regexadr.h' et 'regexadr.c'

Ces fichiers contiennent les signatures et déclarations des fonctions qui identifient les différents modes d’adressage en pseudo-assembleur afin de déterminer comment une opérande doit être interprétée.


### 6. Fichiers répondant aux questions de l'Exercice 6– Allocation d’un segment de codes et exécution

'segcode.h' et 'segcode.c'

Ces fichiers contiennent les signatures et déclarations des fonctions qui permettent à notre CPU simulé d'exécuter du code.

### 7. 'main.c'

Ce fichier contient tous les jeux de tests en lien avec notre projet, 4 jeux de test ont été effectués permettant de tester chacun une partie du code. 
Ils sont introduits comme suit :'//TEST DE FICHIER.C'

### 8. 'Makefile'
Ce fichier est utilisé pour automatiser la compilation du projet. Il contient les règles nécessaires pour compiler les fichiers source et générer l'exécutable.

### 9. 'test_parse.txt' et 'test_parse1.txt"
Ce fichier (fichier assembleur) est utilisé dans le main pour créer un parser

### 10. 'readme.txt'
Ce fichier (le fichier actuel) contient une description du projet et des fichiers qui le composent.

### 11. 'exo7.c'
Ce fichier contient les réponses à l'exercice 7 qui sont normalement présentes dans d'autres fichiers, mais par manque de temps nous n'avons pas pu faire en sorte que cela fonctionne correctement. 

## Instructions pour compiler et exécuter le projet

1. Supprimez les annoations /* */ associées au jeu de test que vous voulez essayer.
2. Exécutez la commande 'make all' pour compiler tous les fichiers du projet.
3. Lancez l'exécutable généré avec './main'.

Des annotations sont présentes sur tous les fichiers afin de faciliter la compréhension.
Dans le main notamment, chaque étape des jeux de test est expliquée.

(Les annotations du dernier jeu de test (pour segcode.c) ne sont pas présentes)

Nous avons fourni 2 fichiers test en langage assembleur, chacun peut-être essayé. Nous avons laissé le test sur le fichier texte le plus complet.

