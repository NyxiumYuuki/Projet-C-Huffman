#include <stdio.h>
#include "arbre_de_codage/arbre_binaire.c"
#include "arbre_de_codage/liste.c"

Freq freq_apparition(FILE *file);
arbre huffman(arbre H, Freq L[]);


// main_compress.c [nom_du_fichier_a_compresser]
int main(int argc, char **argv){
  FILE *file;
  // Vérification de l'existance du second argument (Nom du fichier à compresser)
  if(argc != 2){
    printf("\nErreur : Veuillez mettre en argument un nom de fichier à compresser (Ex: %s text.txt)\n",argv[0]);
    return -1;
  }
  // Vérification de l'ouverture du fichier en mode lecture binaire !! FONCTION A MODIFIER
  else if(file=fopen(argv[1],'rb')){
    printf("\nErreur : Fichier %s inexistant\n",argv[1]);
    return -2;
  }
  


}


Freq freq_apparition(FILE *file){
  Freq text;
  text = creer_liste_vide();
  int c;
  while((c=fgetc(file))!=EOF){
    if(rechercher(c,text)==1){
      text=incrementer(c,text);
    }
    else{
      
    }
    
  }

}

arbre huffman(arbre H, Freq L[]){
  /*
    Création de l'arbre de codage de Huffman en considérant une liste avec les fréquences d'apparition des caractères ordonnée croissante
  */
  int i;
  
  printf(L[]->nb);
  printf(L[]->lettre);
}