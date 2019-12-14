#include <stdio.h>
#include "arbre_de_codage/arbre_binaire.c"
#define ASCII_EXT 256 

arbre huffman(arbre T[]);
void frequence(arbre T[], FILE *file);
void tri_tab(arbre T[],int n);
void afficher_tab(arbre T[], int n);
void init_tab(arbre T[], int n);

// main_compress.c [nom_du_fichier_a_compresser]
int main(int argc, char **argv){
  FILE *file;
  const char *filename = argv[1];
  const char *mode= "rb";
  // Vérification de l'existance du second argument (Nom du fichier à compresser)
  printf("Argc : %d\n",argc);
  if(argc != 2){
    printf("\nErreur : Veuillez mettre en argument un nom de fichier à compresser (Ex: %s text.txt)\n",argv[0]);
    return -1;
  }
  // Vérification de l'o(uverture du fichier en mode lecture binaire !! FONCTION A MODIFIER
  else if(!(file=fopen(filename,mode))){
    printf("\nErreur : Fichier %s inexistant\n",filename);
    return -2;
  }
  printf("Tableau : \n");
  arbre N[ASCII_EXT];          // Initialisation de l'arbre
  printf("Tableau initialisé -1: \n");
  init_tab(N,ASCII_EXT);   // On initialise l'arbre avec des poids de -1
  printf("Tableau get freq : \n");
  frequence(N,file);          // On récupère la fréquence d'apparition des lettres du fichiers
  printf("Tableau tri : \n");
  tri_tab(N,ASCII_EXT);       // On fait un tri à bulle sur ce tableau
  printf("Tableau affichage : \n");
  afficher_tab(N,ASCII_EXT);
  
  arbre huff;
  printf("Huffman : \n");
  huff = huffman(N);
  printf("Arbre :\n");
  return 0;
}

void init_tab(arbre T[], int n){
  int i;
  for(i=0;i<n;i++){
    T[i]=creer_feuille(-1,-1);
  }
}

void frequence(arbre T[], FILE *file){
  int c;
  while((c=fgetc(file))!=EOF){
    if(T[c]->poids!=-1){
      T[c]->poids++;
    }
    else{
      T[c]->elt=c;
      T[c]->poids=1;
    }
  }
}

void tri_tab(arbre T[],int n){
  int i,j;
  arbre tmp;
  for(i=0;i<n-1;i++){
    for(j=i+1;j<n;j++){
      if(T[i]->poids>T[j]->poids){
        tmp=T[i];
        T[i]=T[j];
        T[j]=tmp;
      }
    }
  }
}

int compteur_tab(arbre T[], int n){
  int i,compteur;
  compteur=0;
  for(i=0;i<n;i++){
    if(T[i]->poids != -1){
      compteur++;
    }
  }
  return compteur;
}

void afficher_tab(arbre T[], int n){
  printf("\n");
  int i;
  for (i=0;i<n;i++){
    printf("T[%d] = %d (%d)\n",i,T[i]->poids,T[i]->elt);
  }
}

arbre huffman(arbre T[]){
  // Création de l'arbre de codage de Huffman en considérant une liste avec les fréquences d'apparition des caractères ordonnée croissante
  
  // récupérer les deux plus petits poids (cf : deux premieres occurences)
  arbre H = malloc(sizeof(arbre));
  H=creer_arbre_vide();
  int i;
  i=0;
  while(T[i]->poids==-1){
    i++;
  }
  int Index;
  Index=i;
  while(Index<(ASCII_EXT-1)){
    arbre tmp=malloc(sizeof(noeud*));
    tmp->fils_gauche=T[Index];
    tmp->fils_droit=T[Index+1];
    tmp->poids=T[Index]->poids +T[Index+1]->poids;
    T[Index+1]=tmp;
    printf("%d\n",tmp->poids);
    Index++;
    tri_tab(T,ASCII_EXT);
  }
  return T[Index];
}