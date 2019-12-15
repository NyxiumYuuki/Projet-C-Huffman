#include <stdio.h>
#include <string.h>
#include <assert.h>
#include<stdlib.h>
#include "gestion_des_fichiers/gestion_fichiers.h"
#include "main_compress.h"
#include "arbre_de_codage/arbre_binaire.h"
#ifdef DEBUG
#define DEBUG 1
#endif

#define ASCII_EXT 256

// main_compress.c [nom_du_fichier_a_compresser]
/*#########################################################################################################*/

/*                               Partie Fonction pour compression                                          */

/*#########################################################################################################*/

void init_tab(arbre T[], int n){
  int i;
  for(i=0;i<n;i++){
    T[i]=creer_feuille(-1,-1);
  }
}

void frequence(arbre T[], FILE *file){
  int c;
  while((c=fgetc(file))!=EOF){
    if(T[c]->poids!=-1){
      T[c]->poids++;
    }
    else{
      T[c]->elt=c;
      T[c]->poids=1;
    }
  }
}

void tri_tab(arbre T[],int n){
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

int compteur_tab(arbre T[], int n){
  int i,compteur;
  compteur=0;
  for(i=0;i<n;i++){
    if(T[i]->poids != -1){
      compteur++;
    }
  }
  return compteur;
}

void afficher_tab(arbre T[], int n){
  printf("\n");
  int i;
  for (i=0;i<n;i++){
    printf("T[%d] = %d (%c)\n",i,T[i]->poids,T[i]->elt);
  }
}

arbre huffman(arbre T[]){
  // Création de l'arbre de codage de Huffman en considérant une liste avec les fréquences d'apparition des caractères ordonnée croissante
  
  // récupérer les deux plus petits poids (cf : deux premieres occurences)
  arbre H = malloc(sizeof(arbre));
  H=creer_arbre_vide();
  int i;
  i=0;
  while(T[i]->poids==-1){
    i++;
  }
  int Index;
  Index=i;
  while(Index<ASCII_EXT-1){
    arbre tmp=malloc(sizeof(noeud*));
    tmp->elt=NULL;
    tmp->fils_gauche=T[Index];
    tmp->fils_droit=T[Index+1];
    tmp->poids=T[Index]->poids +T[Index+1]->poids;
    T[Index+1]=tmp;
    printf("%d (%c | %c)\n",tmp->poids,tmp->fils_gauche->elt,tmp->fils_droit->elt);
    Index++;
    tri_tab(T,ASCII_EXT);
  }
  return T[Index];
}

void get_lexique(FILE *file, plex Code[], arbre huff){
  int c_int;
  char c_char;
  rewind(file);
  while((c_int=fgetc(file))!=EOF){
    c_char = c_int;
    if(Code[c_int]->lettre!=c_char){
      Code[c_int]->lettre=c_char;
      char s[9]="";
      int f[1]={0};
      arbre_rechercher(huff,c_char,s,0,f);
      printf("Code %c : %s\n",c_int,s);
      Code[c_int]->code=s;
    }
  }
}

void init_codage(plex Code[], int n){
  int i;
  for(i=0;i<n;i++){
    lex * tmp = malloc(sizeof(lex));
    tmp->code=NULL;
    tmp->lettre=NULL;
    Code[i]=tmp;
  }
}

void compression(plex *Code,int n){
  Bin_file *cmp;
  cmp=Ouv_Bit("cmp.txt",'w');
  printf("%s",(Code[0]->code)[0]);
  int i;
  for(i=0;i<n;i++){
    char *s;
    s=Code[i]->code;
    while(s!='\0'){
      Ec_Bit(cmp,s);
      s++;
    } 
  }
}

/*#########################################################################################################*/

/*                                  Partie Gestion de fichiers                                             */

/*#########################################################################################################*/


Bin_file *Ouv_Bit(char *p,char mode)
{
    Bin_file *A;
    A=malloc(sizeof(Bin_file));                                 //On libère l'espace suffisant pour un élèment de type Bin_files
    A->mode=mode;                                               //On récupére le mode d'ouverture du fichier et on le rentre dansle struct
    if(mode=='r')                                               //suivant le mode (écriturer w ou lecture r) on va ouvrir le fichier a l'aide de fopen et on 
    {                                                           //enregistre cela dans lestruct Bin_files
        A->file=fopen(p,"rb");
    }
    else
    {
        A->file=fopen(p,"wb");
    }
    A->record_length=0;                                         //On va ensuite initialiser tous les élèments du struct.
    A->i_record=0;
    A->i_octet=0;
    A->nb_octets=0;
    return A;
}

void Ec_Bit(Bin_file *output,char bit)                              // On veux une fonction capable d'écrire un bit dans un fichier
{
    unsigned char octet,b;                                          // On utilise deux variables octet et b qui vont permettre de récupérer les élèmets a écrire.
    int i;
    
    output->octet[output->i_octet]=bit;                             // On va mettre le premier élèment du tableau octet de la struct Bin_file a la valeur de bit
    output->i_octet++;                                              //On incremente aussi i_octet pour ne pas réecrire surcette valeur 
    if (output->i_octet==8)                                         //Si le tableau octet est plein on va aller écire dans le tableau record
    {                                                          
        octet=0;                                                    //on passe octet a 0 et b a 0x80 soit 1000 0000
        b=0x80;                                                     // cela permet de toujoursgarder 1 en bit de poids fort dans b apre les décalages a droite
        for(i=0;i<8;i++)                                        
        {
            if(output->octet[i]=='1')                               //si on a un '1' dans le tableau octet on va mettre dans octet le résultats de octet ou b
            {
                octet=octet|b;                                      // On va alors creer une "copie" du tableau octet dans octet 
            }
            b=b>>1;
        }
        output->i_octet=0;                      
        output->record[output->i_record]=octet;                     // On va apres le for remetre i_octet a 0 puis on passe dans record la valeur du char octet pour conserver
        output->i_record++;                                         //ce que l'on veut écrire
        output->nb_octets++;                                        //On incrémente aussi i record pour passer a l'élèment suivant et nb_octets car on en a traiter un autre
        if(output->i_record==BLOCK_SIZE)                            //On vérifie ensuite que record qui sert de buffer soit plein avant d'écrire sont contenue.
        {
            fwrite(output->record,1,BLOCK_SIZE,output->file);
            output->i_record=0;                                     //si on éceit on reinitialise i record pour passer auxélèments suivant
        }
    }

}


char Lec_Bit(Bin_file*input)
{
    char bit;                                                                   //On veut lire un fichier bit a bit. Pourcela on va vérifier si la record length est nul ou non
    unsigned char x, b=0x80;                                                    // Ici on déclare de char, x va permettre de récupérer un octet de record
                                                                                // b vas servir de masque. Il nous permettra de savoir quelle valeur possède les bits des élèment de record
    int i;                                                                      //i est un compteur utiliser pour les boucles for


    if (input->record_length==0){                                               // Si le buffer (record),est vide on va aller chercher lesélèments du fichier                               
    
        input->record_length = fread(input->record,1,BLOCK_SIZE,input->file);   //Pour cela on récupère dans record_length le nombre d'élèment de input-file et on les écrit dans record                
        input->i_record=0;                                                      //On réinitialise l'indice de record pour recommencerdepuis le début.                                           
        
        x = input->record[input->i_record] ;                                    //On réupère la valeurde record[i_record] dans x 
        for(i=0;i<8;i++) {
            if(x&b) input->octet[i]='1';                                        //On va alors tester les bits de x grace a x&b. Cela permet de renvoyer 1 si lesdeux bits sont a 1 
            else input->octet[i]='0';                                           // si le test est vrai on ajoute le char 1 dans octet sinon 0
            b=b>>1;                                                             //On décale ensuite b de 1 bit a droite pourconserver le 1 a l'endroit que l'on veut tester
        }
        input->octet[8]=0;                                                      //On passe le derneir élèment de octet a 0 pour marquer la fin de chaine de caractère 
        input->i_record++;                                                      //on incrémente i record  pour aller a l'élèment suivant on réinitialise i_octet et 
        input->i_octet=0;
        input->nb_octets=input->nb_octets+input->record_length;                 //On ajoute la taille du record dans nb_octets pour conserver le bon nombred'élèment traité
    }

    bit=input->octet[input->i_octet];                                           //On passe alors bit a la valeur de octet[0]
    input->i_octet++;                                                           //on incrémente ensuite le compteur
    if(input->i_octet==8){                                                      // Si on a écrit huit bit dans octet on va aller tester l'élèment suivant de record
        x = input->record[input->i_record] ;
        for(i=0;i<8;i++) {
            if(x&b) input->octet[i]='1';
            else input->octet[i]='0';
            b=b>>1;
        }
        input->octet[8]=0;
        input->i_record++;
        input->i_octet=0;;
        if(input->i_record==BLOCK_SIZE){                                         //si on a parcouru tous le buffer on repasse a 0 la longueur
            input->record_length=0;        
        } 
    }
    return bit;                                                                 // on renvoie le bit 
}


int Ferm_Bit(Bin_file *fichier)
{
    unsigned char octet,b;                                                      // On déclare deux unsigned char octet et b qui auront la même utilité que dans Ec_Bit
    int nb_octets=fichier->nb_octets;                                   
    
    if(fichier->mode="w")                                                       // Si le fichier a été ouvert en mode écriture on va aller écrire la fin du buffer  
    {                                                                           // dans le fichier. 
        if(fichier->i_octet!=0)                                                 //Pour cela on répète le même principe que dans Ec_Bit
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
    fclose(fichier->file);                                                      //On ferme ensuite le fichier et on libère l'espace occupé par le srtuct Bin_File.
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
  /*
    De ce que j'ai testé, ça marche de temps en temps même en ne changeant pas le caractère recherché, je pense qu'il y a un problème avec VSCode encore une fois LOL (Yûki)
  */
  #ifdef DEBUG
    printf("\nDEBUG ARBRE RECHERCHER : \n");
    printf("  est_arbre_vide(a) : %d\n",est_arbre_vide(a));
    printf("  est_feuille(a) : %d\n",est_feuille(a));
    printf("  racine(a) : %c\n",racine(a));
    printf("  c : %c\n",c);
    printf("  est_arbre_vide(fils_gauche(a)) : %d\n",est_arbre_vide(fils_gauche(a)));
    printf("  est_arbre_vide(fils_droit(a)) : %d\n",est_arbre_vide(fils_droit(a)));
    printf("  S : %s\n",s);
    printf("--FIN DEBUG ARBRE RECHERCHER--\n");
  #endif
  
  if(!est_arbre_vide(a)){
    if((a->fils_gauche==NULL)&&(a->fils_droit==NULL)&&(racine(a)==c)){
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

/*#########################################################################################################*/

/*                                                  Main                                                   */

/*#########################################################################################################*/


int main(int argc, char **argv){
  Bin_file *p;
  char *filename = argv[1];
  char mode= 'r';

  // Vérification de l'existance du second argument (Nom du fichier à compresser)
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
  printf("Tableau : \n");
  arbre N[ASCII_EXT];          // Initialisation de l'arbre

  printf("Tableau initialisé -1: \n");
  init_tab(N,ASCII_EXT);   // On initialise l'arbre avec des poids de -1

  printf("Tableau get freq : \n");
  
  frequence(N,p->file);          // On récupère la fréquence d'apparition des lettres du fichiers

  printf("Tableau tri : \n");
  tri_tab(N,ASCII_EXT);       // On fait un tri à bulle sur ce tableau

  printf("Tableau affichage : \n");
  afficher_tab(N,ASCII_EXT);
  
  arbre huff;
  printf("Huffman : \n");
  huff = huffman(N);
  
  printf("Arbre :\n");
  int found[1]={0};
  plex Codage[ASCII_EXT];
  init_codage(Codage,ASCII_EXT);
  get_lexique(p->file, Codage,huff);
  Ferm_Bit(p);
  
  return 0;
}