#include <stdio.h>
#include "arbre_de_codage/arbre_binaire.c"
#include "arbre_de_codage/liste.c"

Freq freq_apparition(FILE *file, int *nb_char);
arbre huffman(arbre H, Freq L[]);


// main_compress.c [nom_du_fichier_a_compresser]
int main(int argc, char **argv){
  FILE *file;
  const char *filename = argv[1];
  const char *mode= "rb";
  // Vérification de l'existance du second argument (Nom du fichier à compresser)
  printf("Argc : %d",argc);
  if(argc != 2){
    printf("\nErreur : Veuillez mettre en argument un nom de fichier à compresser (Ex: %s text.txt)\n",argv[0]);
    return -1;
  }
  // Vérification de l'ouverture du fichier en mode lecture binaire !! FONCTION A MODIFIER
  else if(!(file=fopen(filename,mode))){
    printf("\nErreur : Fichier %s inexistant\n",filename);
    return -2;
  }
  int *nb_char;
  *nb_char=0;
  Freq freq;
  freq = freq_apparition(file, nb_char);
  printf("hey");
  return 0;
}


Freq freq_apparition(FILE *file, int *nb_char){
  Freq text;
  text = creer_liste_vide();
  int c;
  while((c=fgetc(file))!=EOF){
    if(rechercher(c,text)==1){
      text=incrementer(c,text);
    }
    else{
      text=ajouter(1,c,text);
    }
    *nb_char++;
  }
  return text;
}

arbre huffman(arbre H, Freq L[]){
  /*
    Création de l'arbre de codage de Huffman en considérant une liste avec les fréquences d'apparition des caractères ordonnée croissante
  */
  int i;
  return H;
}