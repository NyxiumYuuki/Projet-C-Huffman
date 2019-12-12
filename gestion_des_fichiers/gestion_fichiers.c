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
        A->file=fopen(p,"r");
    }
    else
    {
        A->file=fopen(p,"w");
    }
    A->record_length=0;                                         //On va ensuite initialiser tous les élèments du struct.
    A->i_record=0;
    A->i_octet=0;
    A->nb_octets=0;
    return A;
}

void Ec_Bit(Bin_file *output,char bit)                          // On veux une fonction capable d'écrire un bit dans un fichier
{
    unsigned char octet,b;                                      // On utilise deux variables octet et b qui vont permettre de récupérer les élèmets a écrire.
    int i;
    
    output->octet[output->i_octet]=bit;                         // On va mettre le premier élèment du tableau octet de la struct Bin_file a la valeur de bit
    output->i_octet++;                                          //On incremente aussi i_octet pour ne pas réecrire surcette valeur 
    if (output->i_octet==8)                                     //Si le tableau octet est plein on va aller écire dans le tableau record
    {               
        octet=0;                                                //on passe octet a 0 et b a 0x80 soit 1000 0000
        b=0x80;                                                 // cela permet de toujoursgarder 1 en bit de poids fort dans b apre les décalages a droite
        for(i=0;i<8;i++)                        
        {
            if(output->octet[i]=='1')                           //si on a un '1' dans le tableau octet on va mettre dans octet le résultats de octet ou b
            {
                octet=octet|b;           
            }
            b=b>>1;
        }
        output->i_octet=0;                      
        output->record[output->i_record]=octet;                 // On va apres le for remmtre i_octet a 0 puis on passe dans record la valeur du char octet pour conserver
        output->i_record++;                                     //ce que l'on veut écrire
        output->nb_octets++;                                    //On incrémente aussi i record pour passer a l'élèment suivant et nb_octets car on en a traiter un autre
        if(output->i_record==BLOCK_SIZE)                        //On vérifie ensuite que record qui sert de buffer soit plein avant d'écrire sont contenue.
        {
            fwrite(output->record,1,BLOCK_SIZE,output->file);
            output->i_record=0;                                 //si on éceit on reinitialise i record pour passer auxélèments suivant
        }
    }

}


char Lec_Bit(Bin_file*input)
{
    char bit;                                                         //On veut lire un fichier bit a bit. Pourcela on va vérifier si la record length est nul ou non


    if (input->record_length==0)                                                            
    {
        fread(input->record,BLOCK_SIZE,1,input->file);                // si il est nul on va aller le fichier
        input->record_length=(BLOCK_SIZE/8);                          //on va rentré la longueur lue (4096 bit) en octet
        input->i_record=0;                                            //on reinitialise i_record pour etre sur de reprendre le record du début
    
        for (input->i_octet=0;input->i_octet<8;input->i_octet++)      //On va ensuite aller écrire dans le tableau octet.
        {
            if((input->record[input->i_record]>>1)&1)                 //pour cela on predre le premier élèment de record le décallé de 1 bit a droite et a chaque décallage on va comparer avec 1
            {
                input->octet[7-input->i_octet]='1';                   //Si le bit vaut 1 on ajoute '1' a octet, 0 sinon
            }                                                         // cette méthode lisant en premeir les bits de poids faible, on les ajoutes en partant de la fin du tableau.
            else
            {
                input->octet[7-input->i_octet]='0';
            }
        }
            
        input->i_record++;                                            //on incrémente i record  pour aller a l'élèment suivant on réinitialise i_octet et 
        input->i_octet=0;
        input->nb_octets=input->nb_octets+input->record_length;
    }
    

    bit=input->octet[input->i_octet];                               //On passe alors bit a la valeur de octet[0]
    input->i_octet++;                                               //on incrémente ensuite le compteur
    if(input->i_octet=8)
    {
       for (input->i_octet=0;input->i_octet<8;input->i_octet++)     //Comme précédemment on récupère les 1 et les 0 de record pour les passé dans octet
         {
            if((input->record[input->i_record]>>1)&1)
            {
                input->octet[7-input->i_octet]='1';
            }
            else
            {
                input->octet[7-input->i_octet]='0';
            }
        }
        input->i_record++;
        input->i_octet=0;
        if(input->i_record=BLOCK_SIZE)                              //si on a parcouru tous le buffer on repasse a 0 la longueur
        {
            input->record_length=0;        
        } 
    }
    return bit;                                                     // on renvoie le bit 
}


int Ferm_Bit(Bin_file *fichier)
{
    unsigned char octet,b;
    int nb_octets=fichier->nb_octets;                                   
   
    if(fichier->mode="w")
    {
        if(fichier->i_octet!=0)
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
    fclose(fichier->file);
    free(fichier);
    return nb_octets;
}


int main()
{
    Bin_file *p;
    int i;
    p=Ouv_Bit("test.txt",'r');
    printf("%d",p->record_length);
    /*
    for(i=0;i<32762;i++)
    {
        Ec_Bit(p,'1');
    }
    */
    for(i=0;i<32762;i++)
    {
        printf("%c",Lec_Bit(p));
    }
    int n=Ferm_Bit(p);
    printf("\n");
    printf("%i\n",n);
    return 0;
}
