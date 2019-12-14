#include <stdio.h>
#include "arbre_de_codage/arbre_binaire.c"
#define ASCII_EXT 256 

arbre huffman(arbre H, int Tp[], int Tl[]);
void frequence(int Tp[], int Tl[], FILE *file);
void tri_tab(int T[],int T2[],int n);
void afficher_tab(int Tp[], int Tl[], int n);
void init_tab(int T[], int Elt, int n);

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
  // Vérification de l'ouverture du fichier en mode lecture binaire !! FONCTION A MODIFIER
  else if(!(file=fopen(filename,mode))){
    printf("\nErreur : Fichier %s inexistant\n",filename);
    return -2;
  }
  // Récupérer fréquence
  int Tp[ASCII_EXT],Tl[ASCII_EXT];
  init_tab(Tp,-1,ASCII_EXT);
  init_tab(Tl,-1,ASCII_EXT);
  int compteur;
  compteur=0;
  frequence(Tp,Tl,file);
  // Tri
  tri_tab(Tp,Tl,ASCII_EXT);
  afficher_tab(Tp,Tl,ASCII_EXT);
  arbre huff;
  huff = huffman(creer_arbre_vide(),Tp,Tl);
  return 0;
}

void init_tab(int T[], int Elt, int n){
  int i;
  for(i=0;i<n;i++){
    T[i]=Elt;
  }
}

void frequence(int Tp[], int Tl[], FILE *file){
  int c;
  while((c=fgetc(file))!=EOF){
    if(Tl[c]!=-1){
      Tp[c]++;
    }
    else{
      Tl[c]=c;
      Tp[c]=1;
    }
  }
}

void tri_tab(int T[],int T2[],int n){
    int i,j,tmp,tmp2;
    for(i=0;i<n-1;i++){
        for(j=i+1;j<n;j++){
            if(T[i]>T[j]){
            tmp=T[i];
            tmp2=T2[i];
            T[i]=T[j];
            T2[i]=T2[j];
            T[j]=tmp;
            T2[j]=tmp2;
             }
        }
    }
}

void afficher_tab(int Tp[], int Tl[], int n){
  printf("\n");
  int i;
  for (i=0;i<n;i++){
    printf("T[%d] = %d (%d)\n",i,Tp[i],Tl[i]);
  }
}

int compteur_tab(int T[],int n){
  int compteur,i;
  compteur=0;
  for(i=0;i<n;i++){
    if(T[i]!=-1){
      compteur++;
    }
  }
  return compteur;
}

int i_index_min_tab(int T[],int i,int n){
  int index_min;
  index_min=0;
  int k;
  for(k=1;k<n;k++){
    if(T[k]<T[index_min]){
      index_min=k;
      i--;
    }
    if(i==0){
      return index_min;
    }
  }
  return index_min;
}

arbre huffman(arbre H, int Tp[], int Tl[]){
  /*
    Création de l'arbre de codage de Huffman en considérant une liste avec les fréquences d'apparition des caractères ordonnée croissante
  */
  // récupérer les deux plus petits poids (cf : deux premieres occurences)
  int i,compteur;
  i=0;
  compteur=compteur_tab(Tl,ASCII_EXT);
  while(compteur != 1){
    printf("\n Test n°%d \n",i++);
    int l1,l2,p1,p2;
    l1=Tl[i_index_min_tab(Tl,1,ASCII_EXT)];
    p1=Tp[i_index_min_tab(Tl,1,ASCII_EXT)];
    l2=Tl[i_index_min_tab(Tl,2,ASCII_EXT)];
    p2=Tp[i_index_min_tab(Tl,2,ASCII_EXT)];
    arbre fg,fd;
    fg=creer_feuille(l1,p1);
    fd=creer_feuille(l2,p2);
    H=creer_arbre_huffman(0,p1+p2,fg,fd);
    // ajouter le nouveau poid
    compteur--;
  }
  return H;
}
