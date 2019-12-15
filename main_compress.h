#ifndef __MAIN_COMPRESS__
#define __MAIN_COMPRESS__
#include <stdio.h>
#include "arbre_de_codage/arbre_binaire.h"
#include "gestion_des_fichiers/gestion_fichiers.h"

struct lexique{
    char lettre;
    char code[256];
};
typedef struct lexique lex;
typedef struct lexique* plex;

arbre huffman(arbre T[]);
void frequence(arbre T[], FILE *file);
void tri_tab(arbre T[],int n);
void afficher_tab(arbre T[], int n);
void init_tab(arbre T[], int n);
void get_lexique(FILE *file, plex Code[], arbre huff);
void init_codage(plex Code[], int n);

#endif