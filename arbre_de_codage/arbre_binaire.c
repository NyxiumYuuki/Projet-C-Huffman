#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "arbre_binaire.h"
#define DEBUG 1

arbre creer_arbre_vide (void)
{
  return NULL ;
}

arbre creer_arbre_huffman(Elt e, int p, arbre fg, arbre fd)
{
  noeud * tmp = malloc(sizeof(noeud));
  tmp->elt=e;
  tmp->poids=p;
  tmp->fils_gauche=fg;
  tmp->fils_droit=fd;
  return tmp;
}

arbre fils_gauche(arbre a)
{
  assert(!est_arbre_vide(a));
  return a->fils_gauche;
}

arbre fils_droit(arbre b)
{
  assert(!est_arbre_vide(b));
  return b->fils_droit;
}

Elt racine(arbre a)
{
  assert(!est_arbre_vide(a));
  return a->elt;
}

bool est_arbre_vide(arbre a)
{
  return (a == NULL) ;
}

arbre creer_feuille(Elt e, int p)
{
  return creer_arbre_huffman(e, p, creer_arbre_vide(), creer_arbre_vide()) ;
}

bool est_feuille(arbre a)
{
  if(est_arbre_vide(a)){
    return 0;
  }
  return (est_arbre_vide(a->fils_gauche) && est_arbre_vide(a->fils_droit)) ;
}

void free_noeud(arbre a)
{
  assert(!est_arbre_vide(a));
  free(a);
}

void free_arbre(arbre a)
{
  if(!est_arbre_vide(a)){
    free_arbre(fils_gauche(a));
    free_arbre(fils_droit(a));
    free(a);
  }
}

void arbre_rechercher(arbre a, Elt c, char s[], int s_len, int found[]){
  /*
    De ce que j'ai testé, ça marche de temps en temps même en ne changeant pas le caractère recherché, je pense qu'il y a un problème avec VSCode encore une fois LOL (Yûki)
  */
  /*
  #ifdef DEBUG
    printf("\nDEBUG ARBRE RECHERCHER : \n");
    printf("  est_arbre_vide(a) : %d\n",est_arbre_vide(a));
    if(!est_arbre_vide(a)){
      printf("  est_feuille(a) : %d\n",(est_feuille(a)));
      printf("  racine(a) : %c\n",racine(a));
      printf("  c : %c\n",c);
      printf("  est_arbre_vide(fils_gauche(a)) : %d\n",est_arbre_vide(fils_gauche(a)));
      printf("  est_arbre_vide(fils_droit(a)) : %d\n",est_arbre_vide(fils_droit(a)));
      printf("  S : %s\n",s);
      printf("--FIN DEBUG ARBRE RECHERCHER--\n");
    }
    else{
      printf("ARBRE VIDE\n");
    }
  #endif
  */
  if(!est_arbre_vide(a)){
    printf("Racine : %c",racine(a));
    if(racine(a)==c){
      found[0]=1;
      s[s_len]='\0';
      return ;
    }
    if(!est_arbre_vide(fils_gauche(a))&&(found[0]==0)){
      s[s_len]='0';
      arbre_rechercher(fils_gauche(a),c,s,s_len+1,found);
    }
    if(!est_arbre_vide(fils_droit(a))&&(found[0]==0)){
      s[s_len]='1';
      arbre_rechercher(fils_droit(a),c,s,s_len+1,found);
    }
    
  }
}