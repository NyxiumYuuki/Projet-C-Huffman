#include <assert.h>
#include <stdlib.h>
#include "liste.h"

Freq creer_liste_vide(){
    return NULL;
}

int est_liste_vide(Freq l){
    return(l==NULL);
}

Freq ajouter(int nb, int lettre, Freq l){
    Occ * tmp;
    tmp=malloc(sizeof(Occ));
    tmp->nb=nb;
    tmp->lettre=lettre;
    tmp->suiv=l;
    return tmp;
}

Freq inserer(Occ *place, Freq l){
    printf("    Inserer :\n");
    if(est_liste_vide(l)){
        printf("    Liste vide :\n");
        return ajouter(place->nb,place->lettre,creer_liste_vide());
    }
    else{
        printf("    Liste non vide :\n");
        Occ * tmp;
        tmp = l;
        if(tmp->suiv == NULL){
            if(tmp->nb < place->nb){
                tmp->suiv=place;
                place->suiv=NULL;
            }else{
                place->suiv=tmp;
                l=place;
            }
            return l;
        }
        while(tmp->suiv != NULL){
            if(tmp->suiv->nb >= place->nb){
                place->suiv=tmp->suiv;
                tmp->suiv=place;
                return l;
            }
            else{
                tmp=tmp->suiv;
            }
        }
        // Ajoute à la fin
        while(l->suiv != NULL){
            l = l->suiv;
        }
        l->suiv=place;
        return l;
    }
}

int tete_lettre(Freq l){
    assert(!est_liste_vide(l));
    return(l->lettre);
}

int tete_freq(Freq l){
    assert(!est_liste_vide(l));
    return(l->nb);
}

Freq queue(Freq l){
    assert(!est_liste_vide(l));
    return(l->suiv);
}

void liberer_liste(Freq l){
    if(!est_liste_vide(l)){
        liberer_liste(queue(l));
        free(l);
    }
}

Freq liberer_maillon(Freq l){
    assert(!est_liste_vide(l));
    Occ * tmp = l->suiv;
    free(l);
    return tmp;
}

int liste_rechercher(int lettre,Freq l){
    if(est_liste_vide(l)){
        return 0;
    }
    else if(lettre==tete_lettre(l)){
            return 1;
    }
    else{
        return liste_rechercher(lettre,queue(l));
    }
}

Freq incrementer(int lettre, Freq l){
    /*
        Incrémente de 1, l'occurence de la lettre associée 
        (Attention ! Ici, la liste est parcourue complétement même si on a déjà incrémenté ... Autres solutions ?)
    */
    if(est_liste_vide(l)){
        return l;
    }
    else{
        while(lettre != tete_lettre(l)){
            l=queue(l);
        }
        l->nb++;
        return l;
    }
}

void afficher(Freq L){
    printf("Affichage ...\n");
    while(queue(L) != NULL){
        printf("    P : %d | Char : %d\n",tete_freq(L),tete_lettre(L));
        L=queue(L);
    }
    printf("Fin Affichage.\n");
}