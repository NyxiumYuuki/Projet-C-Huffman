#ifndef __LISTE_OCCURENCE__
#define __LISTE_OCCURENCE__
struct zoccurence{
    int nb;
    int lettre;
    struct zoccurence *suiv;
};
typedef struct zoccurence Occ;
typedef struct zoccurence * Freq;

Freq creer_liste_vide();
int est_liste_vide(Freq l);
Freq ajouter(int nb, int lettre, Freq l);
int tete(Freq l);
Freq queue(Freq l);
void liberer_liste(Freq l);
Freq liberer_maillon(Freq l);
int rechercher(int lettre, Freq l);
Freq incrementer(int lettre, Freq l);

#endif