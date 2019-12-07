#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "arbre_binaire.h"

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

arbre creer_feuille(Elt e)
{
  return creer_arbre(e, creer_arbre_vide(), creer_arbre_vide()) ;
}

bool est_feuille(arbre a)
{
  if (est_arbre_vide(a))
    return 0 ;
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