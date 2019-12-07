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

int tete(Freq l){
    assert(!est_liste_vide(l));
    return(l->lettre);
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

int rechercher(int lettre,Freq l){
    if(est_liste_vide(l)){
        return 0;
    }
    else if(lettre==tete(l)){
            return 1;
    }
    else{
        return rechercher(lettre,queue(l));
    }
}

Freq incrementer(int lettre, Freq l){
    /*
        Incrémente de 1, l'occurence de la lettre associée 
        (Attention ! Ici, la liste est parcourue complétement même si on a déjà incrémenté ... Autres solutions ?)
    */
    if(!est_liste_vide(l)){
        return l;
    }
    else if(lettre == tete(l)){
        int occ;
        occ=l->nb;
        occ++;
        l->nb=occ;
        return l;
    }
    else{
        return incrementer(lettre,queue(l));
    }
}