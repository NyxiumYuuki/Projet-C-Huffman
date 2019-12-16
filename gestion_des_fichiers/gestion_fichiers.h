/*
    Prototypes de gestion_fichiers.c + variables define + structure(s)
*/
#ifndef __GESTION_FICHIERS__
#define __GESTION_FICHIERS__
#define BLOCK_SIZE 4096                    //Une petite valeur est utile pour les tests sur de petits fichiers.

typedef struct
{
    FILE *file;                         //Identificateur fichier
    char mode;                          //Mode de lecture r ou w
    unsigned char record[BLOCK_SIZE];   //Tampon pour lire ou écrire
    int record_length;                  //nombre d'élèments du tampon
    int i_record;                       //indice dans le tampon
    char octet[9];                      //Octet coupé en 9 caractère, 8 pour les bit et 1 pour le caractère d'échappement
    int i_octet;                        //indice dansl'octet
    int nb_octets;                      //Nb octet lis/écrit
    
}Bin_file;

Bin_file *Ouv_bit(char *p,char mode);
void Ec_Bit(Bin_file *output,char bit);
char Lec_Bit(Bin_file*input);
int Ferm_Bit(Bin_file *fichier);

#endif