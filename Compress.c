#include <stdio.h>
#include <string.h>
#include <assert.h>
#include<stdlib.h>
#include "gestion_des_fichiers/gestion_fichiers.h"
#include "main_compress.h"
#include "arbre_de_codage/arbre_binaire.h"

#define ASCII_EXT 256

// main_compress.c [nom_du_fichier_a_compresser]
/*#########################################################################################################*/

/*                                  Partie Gestion de fichiers                                             */

/*#########################################################################################################*/


Bin_file *Ouv_Bit(char *p,char mode)
{
  /*Ouverture d'un fichier et initialisation de la structure Bin_file*/

    Bin_file *A;
    A=malloc(sizeof(Bin_file));                                 //Libération de l'espace
    A->mode=mode;                                               //Récupération du mode (ouverture ou écriture)
    if(mode=='r')                                               
    {                                                           
        A->file=fopen(p,"rb");                                  //Ouverture en mode rb/wb. Le b est nécessaire sur windows là où les tests ont été effectués
    }
    else
    {
        A->file=fopen(p,"wb");
    }
    A->record_length=0;                                         //Initialisation du struct
    A->i_record=0;
    A->i_octet=0;
    A->nb_octets=0;
    return A;
}


void Ec_Bit(Bin_file *output,char bit)                              
{
  /*Ecriture bit à bit dans un fichier*/

    unsigned char octet,b;                                          
    int i;
    
    output->octet[output->i_octet]=bit;                             //Stockage de bit dans le premier élément libre du tableau octet
    output->i_octet++;                                              //Passage à l'élément suivant
            
    if (output->i_octet==8)                                         //Ecriture dans record                                         
    {                                                               
        octet=0;                                                    //octet et b vont permettre de passer les bits du tableau octet sous forme de chaine de caractères dans octet
        b=0x80;                                                     
        for(i=0;i<8;i++)                                        
        {
            if(output->octet[i]=='1')                               
            {
                octet=octet|b;                                       
            }
            b=b>>1;                                                 //Décalage de b à droite pour garder le 1 au bon endroit de la comparaison
        }
        output->i_octet=0;                      
        output->record[output->i_record]=octet;                     //Stockage de octet dans record 
        output->i_record++;                                         //Incrémentation de i_record (passage à l'élément suivant) et du nombre d'octet
        output->nb_octets++;                                        
        if(output->i_record==BLOCK_SIZE)                            //Vérification de l'état du buffer (plein ou non).
        {
            fwrite(output->record,1,BLOCK_SIZE,output->file);       //Ecriture de record dans le fichier et réinitialisation de i_record(retour au début du buffer)
            output->i_record=0;                                     
        }
    }

}


char Lec_Bit(Bin_file*input)
{
  /*Lecture d'un fichier bit par bit*/

    char bit;
    unsigned char x, b=0x80;                                                    
    int i;

    if (input->record_length==0){                                               //Vérification de l'état du buffer                               
    
        input->record_length = fread(input->record,1,BLOCK_SIZE,input->file);   //Lecture du fichier et récupération du nombre d'élément              
        input->i_record=0;                                                                                                
        
        x = input->record[input->i_record] ;                                    //Copie de l'élément à lire dans x 
        for(i=0;i<8;i++) {
            if(x&b) input->octet[i]='1';                                        //Comparaison des bits de x à 1 et récupération de la valeur du bit
            else input->octet[i]='0';                                           
            b=b>>1;                                                             //Décalage b à droite (1 est conservé comme élément de comparaison)
        }
        input->octet[8]=0;                                                      //Mise à 0 du dernier élément du tableau octet (fin de chaine de caractère) 
        input->i_record++;                                                      //Passage à l'élément suivant et réinitialisation de i_octet
        input->i_octet=0;
        input->nb_octets=input->nb_octets+input->record_length;                 //Mise à jour du nombre d'éléments
    }

    bit=input->octet[input->i_octet];                                           //Récupération de l'élément i_octet dans bit
    input->i_octet++;                                                           //Incrémentation de i_octet
    if(input->i_octet==8){                                                      
        x = input->record[input->i_record] ;                                    //Même fonctionnement qu'au dessus
        for(i=0;i<8;i++) {
            if(x&b) input->octet[i]='1';
            else input->octet[i]='0';
            b=b>>1;
        }
        input->octet[8]=0;                                                      //Actualisation des éléments du struct
        input->i_record++;
        input->i_octet=0;;
        if(input->i_record==BLOCK_SIZE){                                        //Vérification de l'état du buffer et réinitialisation
            input->record_length=0;        
        } 
    }
    return bit;                                                                 //Récupération du bit lu 
}


int Ferm_Bit(Bin_file *fichier)
{
  /*Fermeture du fichier et écriture de la fin du buffer*/

    unsigned char octet,b;                                                      
    int nb_octets=fichier->nb_octets;                                   
    
    if(fichier->mode=='w')                                                      //Vérification du mode d'ouverture   
    {                                                                            
        if(fichier->i_octet!=0)                                                 //Ecriture du buffer (même principe que Ec_Bit)
        {
            octet=0;
            b=0x80;
            int i;
            for(i=0;i<fichier->i_octet;i++)
            {
                if(fichier->octet[i]=='1')
                {
                    octet=octet|b;
                }
                b=b>>1;
            }
            fichier->record[fichier->i_record]=octet;
            fichier->i_record++;
            nb_octets++;
        }
        if(fichier->i_record!=0)
        {
            fwrite(fichier->record,1,BLOCK_SIZE,fichier->file);
        }
    }
    fclose(fichier->file);                                                      //Fermeture du fichier et libération du sruct
    free(fichier);
    return nb_octets;
}

/*#########################################################################################################*/

/*                                     Partie Arbre binaire                                                */

/*#########################################################################################################*/


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

void arbre_rechercher(arbre a, Elt c, char s[], int s_len, int found[]){
  /*recherche d'un élément du texte dans l'arbre et récupération de son codage*/

  if(!est_arbre_vide(a)){
    if((a->fils_gauche==NULL)&&(a->fils_droit==NULL)&&(racine(a)==c)){
      found[0]=1;                                                         //Indication de la fin de recherche
      s[s_len]='\0';                                                      //Ajout du caractère d'échappement en fin de chaîne
      return ;
    }
    if(!est_arbre_vide(fils_gauche(a))&&(found[0]==0)){                   //Passage au fils gauche et ajout de '0' dans la chaîne de caractère
      s[s_len]='0';
      arbre_rechercher(fils_gauche(a),c,s,s_len+1,found);                 //Appel récursif avec passage a l'élément suivant de la chaîne.
    }
    if(!est_arbre_vide(fils_droit(a))&&(found[0]==0)){                    //Même principe mais en ajoutant '1'
      s[s_len]='1';
      arbre_rechercher(fils_droit(a),c,s,s_len+1,found);
    }
    
  }
}


void serialisation(arbre a,FILE*file){
  
  /*Création de la chaine de caractère qui permettra de décompresser le fichier*/

  if(est_feuille(a)){
    char s1[1],s2[64];                                                   //Création des chaînes de caractère qui récupéreront l'élément de la racine et la valeur du poids en char
    s1[0]=a->elt;
    fwrite(s1,1,1,file);                                                 //écriture de l'élément
    sprintf(s2, "%d", a->poids);                                         //récupération du poids sous forme de char
    int count = 0;                                                       //Recherche du nombre de char dans s2
    while (a->poids != 0) {
        a->poids /= 10;
        ++count;
    }
    fwrite(s2,count,1,file);                                             //Ecriture de s2 et de '|'
    fwrite("|",1,1,file);
  }
  if(!est_arbre_vide(a->fils_gauche)){                                   //Appels récursif sur fils droit et gauche
    serialisation(a->fils_gauche,file);
  }
  if(!est_arbre_vide(a->fils_droit)){
    serialisation(a->fils_droit,file);
  } 
}

void deserialisation(arbre T[], FILE*file){

  /*Récupération de la chaîne de la caractère en début de fichier compréssé pour recreer le tableau de frequence*/

  char c,b;
  rewind(file);                                               //retour au début du fichier
  while((c=fgetc(file))!='|'&&(b=fgetc(file))!='|'){          //Recherche des caractères des ||| qui servent de carctère d'échappement
    char num[64]="";
    int i;
    i=0;
    while(b!='|'){                                            //Incrementation de i tant que le troisième caractère n'est pas trouvé
      num[i]=b;
      b=fgetc(file);
      i++;
    }
    int poids;
    char *end;
    poids = strtol(num, &end, 10);                           //Récupération de la cahine de caractère et contruction du tableau
    printf("    %c%d%c\n",c,poids,b);
    T[c]->elt=c;
    T[c]->poids=poids;
  }
  b=fgetc(file);
}

/*#########################################################################################################*/

/*                               Partie Fonction pour compression                                          */

/*#########################################################################################################*/

void init_tab(arbre T[], int n){

  /*Initialisaion d'un tableau composé d'arbre*/

  int i;
  for(i=0;i<n;i++){
    T[i]=creer_feuille(-1,-1);                                        //Initialisation des poids à -1. Les poids des caractères présents étant forcément positifs                  
  }
}

void frequence(arbre T[], FILE *file){

  /*Recherche des caractères et de leurs fréquences dans le texte*/

  int c;
  while((c=fgetc(file))!=EOF){                                        //Boucle jusqu'à la fin du fichier 
    if(T[c]->poids!=-1){                                              //Vérification du poids, le caractère a t-il été trouvé ?
      T[c]->poids++;                                                  //Incrémentation du poids
    }
    else{
      T[c]->elt=c;                                                    //Initialisation du poids à 1
      T[c]->poids=1;
    }
  }
}

void tri_tab(arbre T[],int n){

/*Tri à bulle effectué sur les poids des éléments du tableau*/

  int i,j;
  arbre tmp;
  for(i=0;i<n-1;i++){
    for(j=i+1;j<n;j++){
      if(T[i]->poids>T[j]->poids){
        tmp=T[i];
        T[i]=T[j];
        T[j]=tmp;
      }
    }
  }
}

void afficher_tab(arbre T[], int n){

  /*Fonction de test. Affichage des éléments du tableau*/

  printf("\n");
  int i;
  for (i=0;i<n;i++){
    printf("T[%d] = %d (%c)\n",i,T[i]->poids,T[i]->elt);
  }
}

arbre huffman(arbre T[]){

  /*Création de l'arbre de codage de Huffman en considérant une liste avec les fréquences d'apparition des caractères ordonnée croissante*/
  
  arbre H = malloc(sizeof(arbre));                                                        //Libération de l'espace mémoire pour un arbre
  H=creer_arbre_vide();                                                                   //Initialisation de l'arbre
  int i;
  i=0;
  while(T[i]->poids==-1){                                                                 //Recherche du nombre de caractères effectivement présents dans le tableau
    i++;
  }
  int Index;  
  Index=i;                                                                                //Initialisation de Index, début effectif du tableau
  while(Index<ASCII_EXT-1){
    arbre tmp=malloc(sizeof(noeud*));                                                     //Initialisation d'un arbre tmp qui aura pour fils les plus petits éléments du tableau
    tmp->elt='0';
    tmp->fils_gauche=T[Index];
    tmp->fils_droit=T[Index+1];
    tmp->poids=T[Index]->poids +T[Index+1]->poids;                                        //Actualisation du poids de tmp.
    T[Index+1]=tmp;                                                                       //Mise à jour de la valeur de T[Index+1] vers tmp
    printf("%d (%c | %c)\n",tmp->poids,tmp->fils_gauche->elt,tmp->fils_droit->elt);       //Ligne de test
    Index++;                                                                              // Incrémentation de Index car un élément doit être supprimé du tableau                                                                          
    tri_tab(T,ASCII_EXT);                                                                 //Nouveaux tri une fois les deux plus petites valeurs enlevées. tmp est mis a la bonne place
  }
  return T[Index];                                                                        //Renvoi du dernier élément du tableau correspondant à l'arbre final
}

void init_codage(plex Code[], int n){

  /*Initialisation de la table récupérant les codes de chaque lettre*/

  int i;
  int j;
  for(i=0;i<n;i++){
    lex * tmp = malloc(sizeof(lex));
    for(j=0;j<ASCII_EXT;j++){
        tmp->code[j]='0';
    }
    tmp->lettre='0';
    Code[i]=tmp;
  }
}

void get_lexique(FILE *file, plex Code[], arbre huff){

  /*Création du tableau des correspondances lettre/code */

  int i;
  char c_char;
  rewind(file);                                           //Reprise de la lecture du fichier au début
  i=0;
  char s[ASCII_EXT]="";
  while((c_char=fgetc(file))!=EOF){                       //Parcours du fichier jusqu'à la fin de ce dernier
    Code[i]->lettre=c_char;                               //Attribution de la lettre à une case. A chaque apparition, on stocke une lettre dans une nouvelle case 
    int f[1]={0};
    arbre_rechercher(huff,c_char,s,0,f);                  //Attribution du code associé à la lettre
    strcpy(Code[i]->code,s);
    printf("Code %c : %s\n",c_char,Code[i]->code);        //Ligne de test
    i++;
  }
}


void compression(plex Code[],int n,arbre a){

  /*Ecriture dans un nouveau fichier des lettres avec leurs nouveaux codes*/

  Bin_file* cmp;
  cmp=Ouv_Bit("cmp.txt",'w');                             //Ouverture d'un fichier différent en écriture (évite d'écraser le contenu)
  int i;
  int j;
  char tmp;
  serialisation(a,cmp->file);                             //Lancement de la sérialisation : écriture de l'arbre en début de fichier
  fwrite("||",2,1,cmp->file);                             // Mise en place de ||| qui serviront de délimitaion entre arbre et contenu
  for(i=0;i<n;i++){
    j=0;
    while(Code[i]->code[j]!='\0'){                        //écriture des bits composant le code de chaque lettre 
      tmp=Code[i]->code[j];
      if (tmp=='0'){
        Ec_Bit(cmp,'0');
      }else if(tmp=='1'){
        Ec_Bit(cmp,'1');
      }
      j++;
    }
  }
  Ferm_Bit(cmp);                                        // femeture du fichier compressé
}


void decompression(arbre a,Bin_file*input){
  char bit='0';                                       //Initialisation de bit a une valur ne posant pas de conflit
  Bin_file*dest;                                      //OUverture d'un fichier où écrire la version décompressé
  dest=Ouv_Bit("ucmp.txt",'w');
  int i=0;  
  char s[1];                                          //chaine de caractère contenant la lettre a écrire
  for(i=0;i<a->poids;i++){                            
    arbre tmp=malloc(sizeof(arbre));                  //Création d'un arbre temporaire, copie de a
    tmp=a;                                                                        
    while(!est_feuille(tmp)){                         //Parcours l'arbre tant tmp n'est pas une feuille
      bit=Lec_Bit(input);
      if(bit=='0'){                                   
        tmp=tmp->fils_gauche;
      }else if(bit=='1'){
        tmp=tmp->fils_droit;
      }
    }
    s[0]=tmp->elt;                                    //on écrit la valeur trouvé
    fwrite(s,1,1,dest->file);                    
  }
  Ferm_Bit(dest);
}

/*#########################################################################################################*/

/*                                                  Main                                                   */

/*#########################################################################################################*/


int main(int argc, char **argv){
  Bin_file *p;
  char *filename = argv[1];
  char mode= 'r';

  // Vérification de l'existence du second argument (Nom du fichier à compresser)
  printf("Argc : %d\n",argc);
  if(argc != 2){
    printf("\nErreur : Veuillez mettre en argument un nom de fichier à compresser (Ex: %s text.txt)\n",argv[0]);
    return -1;
  }

  // Vérification de l'ouverture du fichier en mode lecture binaire !! FONCTION A MODIFIER
  else if(!(p=Ouv_Bit(filename,mode))){
    printf("\nErreur : Fichier %s inexistant\n",filename);
    return -2;
  }
  printf("Entrez C pour une compression ou D pour une décompression:");
  char Mode;
  Mode='G';
  while((Mode!='C')&&(Mode!='D')){
    scanf("%c",&Mode);
    printf("Rentrez C ou D\n");
  }
  if(Mode=='C'){
    printf("\n");
    printf("Tableau : \n");
    arbre N[ASCII_EXT];                               // Initialisation de l'arbre

    printf("Tableau initialisé -1: \n");
    init_tab(N,ASCII_EXT);                            // On initialise l'arbre avec des poids de -1

    printf("Tableau get freq : \n");
  
    frequence(N,p->file);                             // On récupère la fréquence d'apparition des lettres du fichier

    printf("Tableau tri : \n");
    tri_tab(N,ASCII_EXT);                             // On fait un tri à bulle sur ce tableau

    printf("Tableau affichage : \n");
    afficher_tab(N,ASCII_EXT);                        //Ligne de test
  
    arbre huff;                                                                   
    printf("Huffman : \n");
    huff = huffman(N);                                //Création de l'arbre de Huffman
  
    printf("Arbre :\n");
    int found[1]={0};
    int taille=huff->poids;                           // Récupération du nombre d'éléments
    plex Codage[taille];                              //Création de la table de codage 
    init_codage(Codage,taille);
    printf("Lexique:\n");
    get_lexique(p->file, Codage,huff);
    printf("Compression %s : \n",Codage[0]->code);
    compression(Codage,taille,huff);                 //Lancement de la compression
    printf("Fermeture:\n");
    Ferm_Bit(p);                                     //fermeture du fichier
  }
  else if(Mode=='D'){
    printf("\n");
    printf("Tableau : \n");
    arbre D[ASCII_EXT];
    init_tab(D,ASCII_EXT);                            // On initialise l'arbre avec des poids de -1
    printf("  Tableau get freq FROM FILE : \n");
    deserialisation(D, p->file);
    printf("  Tableau tri : \n");
    tri_tab(D,ASCII_EXT);                             // On fait un tri à bulle sur ce tableau

    printf("  Tableau affichage : \n");
    afficher_tab(D,ASCII_EXT);

    arbre huff;
    printf("Huffman : \n");
    huff = huffman(D);                                //Reconstitution de l'abre 
    printf("FIN\n");
    decompression(huff,p);                            //Décompression et femeture du fichier
    Ferm_Bit(p);
  }
  return 0;
}