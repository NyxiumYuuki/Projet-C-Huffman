#ifndef __ARBRE_BINAIRE__
#define __ARBRE_BINAIRE__

typedef int bool;
struct znoeud {
  int elt ;
  int poids;
  struct znoeud *fils_gauche;
  struct znoeud *fils_droit; 
};
typedef struct znoeud noeud ;
typedef struct znoeud * arbre;

arbre creer_arbre_vide(void);
arbre creer_arbre_huffman(int e, int p, arbre fg, arbre fd);
arbre fils_gauche(arbre a);
arbre fils_droit(arbre a);
int racine(arbre a);
bool est_arbre_vide(arbre a);

arbre creer_feuille(int e, int p) ;
bool est_feuille(arbre a) ;
char *arbre_rechercher(arbre a, char c, char*s);
#endif
