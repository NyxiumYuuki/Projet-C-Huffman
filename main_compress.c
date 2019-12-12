#include <stdio.h>
#include "arbre_de_codage/arbre_binaire.c"
#include "arbre_de_codage/liste.c"

Freq freq_apparition(FILE *file);
arbre huffman(arbre H, Freq L);


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

  // Récupération des fréquences d'apparition des caractères dans le fichier
  Freq tmp,freq;
  freq = freq_apparition(file);
  tmp=freq;
  // Affichage pour vérification
  while(!est_liste_vide(tmp)){
    printf("%d (%d)\n",tete_lettre(tmp),tete_freq(tmp));
    tmp=queue(tmp);
  }

  // freq à ordonner dans l'ordre croissant
  printf("Test Huffman\n");
  arbre huff;
  huff = huffman(creer_arbre_vide(),freq);
  printf("FIN Test Huffman\n");

  return 0;
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
      text=ajouter(1,c,text);
    }
  }
  return text;
}

arbre huffman(arbre H, Freq L){
  /*
    Création de l'arbre de codage de Huffman en considérant une liste avec les fréquences d'apparition des caractères ordonnée croissante
  */
  if(est_liste_vide(L)){
    printf("Liste vide\n");
    return H;
  }
  else{
    printf("Liste NON vide\n");
    // récupérer les deux plus petits poids (cf : deux premieres occurences)
    arbre A,B,AB;
    Freq l;
    l=L;
    int al,bl,ap,bp;
    al = tete_lettre(l);
    ap = tete_freq(l);
    l = queue(l);
    bl = tete_lettre(l);
    bp = tete_freq(l);
    l = queue(l);

    printf("CREATION ARBRE\n");
    A = creer_arbre_huffman(al,ap,creer_arbre_vide(),creer_arbre_vide());
    B = creer_arbre_huffman(bl,bp,creer_arbre_vide(),creer_arbre_vide());
    AB = creer_arbre_huffman(al*100+bl,ap+bp,A,B);
    
    l = inserer(ap+bp,al*100+bl,l);
    printf("test\n");
    while(!est_liste_vide(l)){
      printf("%d (%d)\n",tete_lettre(l),tete_freq(l));
      l=queue(l);
    }

  }
  return H;
}

Freq partition (Freq L; Freq deb; Freq fin)
{
  int permu;
  int pivot;
  Freq compt = deb;
  Freq tmp;
  if (!est_liste_vide(deb))
  {
    pivot = deb -> nb;
    compt = deb;
    tmp = deb -> suiv;
    while(tmp != (fin -> suiv))
    {
      if(tmp -> nb)
      {
        permu = tmp->nb;
        tmp = tmp -> suiv;
        compt -> nb = permu;
      }
      tmp = tmp -> suiv;
    }
    permu = deb -> nb;
    deb -> nb = compt -> nb;
    compt -> valeur = nb;
  }
  return compt;
}

void quicksort(Freq L; Freq deb; Freq fin)
{
  if ((deb -> nb) < (fin -> valeur))
  {
    Freq pivot = partion(L,deb,fin);
    if(!est_liste_vide(pivot))
    {
      quicksort(L,deb,precedent(L,pivot -> nb));
      if
    }
  }
}

/*
Freq fusion(Freq L, int i, int m, int j)
{
  int tmp,u,k,v;
  tmp=j-i+1;
  k=0;
  u=-i;
  v=m;
  while ((u<m) && (v<j))
  {
    if (L[u] < L[v])
    {
      tmp[k]=L[u]
      u=u+1;
      k=k+1;
    }
    else
    {
      tmp[k]=L[v];
      v=v+1;
      k=k+1;
    }
  }

  if (u<m)
  {
    for(l=m;l<u;l++)
    {
      off = m-u;
      L[j-off]=L[l];
    }
  }

  for(l=0;l<k;l++)
  {
    L[l]=tmp[k];
  }
}

Freq tri_fusion(L ,i,j)
{
  if (i<j-1)
  {
    m=int((i+j)/2);
    tri_fusion(i,m-1);
    tri_fusion(m,j);
    fusion(T,i,m,j);
  }
}*/ 