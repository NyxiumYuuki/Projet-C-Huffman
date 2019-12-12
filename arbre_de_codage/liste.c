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

Freq inserer(int nb, int lettre, Freq l){
    printf("Inserer :\n");
    if(est_liste_vide(l)){
        printf("Liste vide :\n");
        return ajouter(nb,lettre,creer_liste_vide());
    }
    else{
        printf("Liste non vide :\n");
        Occ * tmp;
        tmp = NULL;
        Freq new,csl,elem;
        new = l;
        elem->nb=nb;
        elem->lettre=lettre;
        printf("Boucle : freq : \n",tete_freq(new));
        while( new && tete_freq(new)<nb){
            printf("nb: %d / freq : %d \n",nb,tete_freq(new));
            tmp=new;
            new = queue(new);
        }
        printf("Fin boucle :\n");
        elem->suiv=csl;
        tmp->suiv=elem;
        return tmp;
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

int rechercher(int lettre,Freq l){
    if(est_liste_vide(l)){
        return 0;
    }
    else if(lettre==tete_lettre(l)){
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