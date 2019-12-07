#ifndef __LISTE_OCCURENCE__
#define __LISTE_OCCURENCE__
struct zoccurence{
    int nb;
    char lettre;
    struct zoccurence *suiv;
};
typedef struct zoccurence Occ;
typedef struct zoccurence * Freq;

Freq creer_liste_vide();
int est_liste_vide(Freq l);
Freq ajouter(int nb, char lettre, Freq l);
int tete(Freq l);
Freq queue(Freq l);
void liberer_liste(Freq l);
Freq liberer_maillon(Freq l);
int rechercher(char lettre, Freq l);

#endif