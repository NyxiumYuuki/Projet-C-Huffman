#ifndef __ARBRE_BINAIRE__
#define __ARBRE_BINAIRE__

typedef int Elt;
typedef int bool;
struct znoeud {
  Elt elt ;
  int poids;
  struct znoeud *fils_gauche;
  struct znoeud *fils_droit; 
};
typedef struct znoeud noeud ;
typedef struct znoeud * arbre;

arbre creer_arbre_vide(void);
arbre creer_arbre_huffman(Elt e, int p, arbre fg, arbre fd);
arbre fils_gauche(arbre a);
arbre fils_droit(arbre a);
Elt racine(arbre a);
bool est_arbre_vide(arbre a);

arbre creer_feuille(Elt e, int p) ;
bool est_feuille(arbre a) ;

#endif
