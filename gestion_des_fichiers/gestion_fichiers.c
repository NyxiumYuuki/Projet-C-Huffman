/*
    Fonctions liés à la gestion de fichiers

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion_fichiers.h"




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


    if (input->record_length==0){                                               // Si le buffer (record),est videon va aller chercher lesélèments du fichier                               
    
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

/*
void main()
{ 
    Bin_file *p;
    char s[16];
    int i;
    p=Ouv_Bit("test.txt",'r');
    printf("%i\n",p->record_length);
    for (i=0;i<16;i++){
        s[i]=Lec_Bit(p);
    }
    for(i=0;i<16;i++){
        printf("%c",s[i]);
    }
    Ferm_Bit(p);
    p=Ouv_Bit("test.txt","w");
    Ec_Bit(p,'0');
    Ec_Bit(p,'1');
    Ec_Bit(p,'1');
    Ec_Bit(p,'0');
    Ec_Bit(p,'0');
    Ec_Bit(p,'1');
    Ec_Bit(p,'1');
    Ec_Bit(p,'0');
    Ferm_Bit(p);
}
*/