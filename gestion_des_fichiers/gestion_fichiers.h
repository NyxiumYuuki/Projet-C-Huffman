#ifndef __GESTION_FICHIERS__
#define __GESTION_FICHIERS__
#include <stdio.h>
#define BLOCK_SIZE 4096

typedef struct
{
    FILE*file;                          // Identificateur fichier
    char mode;                          //Mode de lecture r ou w
    unsigned char record[BLOCK_SIZE];   //Tampon pour lire ou écrire
    int record_length;                  //nombre d'élèments du tampon
    int i_record;                       //indice dans le tampon
    char octet[8];                      //On découpe l'octet en 8 caractère
    int i_octet;                        //indice dansl'octet
    int nb_octets;                      //Nb octet lis/écrit
}Bin_file;

#endif