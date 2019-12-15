/*
    Prototypes de gestion_fichiers.c + variables define + structure(s)
*/
#ifndef __GESTION_FICHIERS__
#define __GESTION_FICHIERS__
#include <stdio.h>
#define BLOCK_SIZE 128                  //il faut changer sa valeur pour les tests avec blocsize = nb d'octet qu'on veut écrire

struct zBin_file{
    FILE *file;                          // Identificateur fichier
    char mode;                          //Mode de lecture r ou w
    unsigned char record[BLOCK_SIZE];   //Tampon pour lire ou écrire
    int record_length;                  //nombre d'élèments du tampon
    int i_record;                       //indice dans le tampon
    char octet[9];                      //On découpe l'octet en 8 caractère
    int i_octet;                        //indice dansl'octet
    int nb_octets;                      //Nb octet lis/écrit
    
};
typedef struct zBin_file Bin_file;
typedef struct zBin_file pBin_file;

Bin_file *Ouv_bit(char *p,char mode);

void Ec_Bit(Bin_file *output,char bit);

char Lec_Bit(Bin_file*input);

int Ferm_Bit(Bin_file *fichier);

#endif