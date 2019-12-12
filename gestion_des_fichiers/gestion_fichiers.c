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
    A=malloc(sizeof(Bin_file));
    A->mode=mode;
    if(mode=='r')
    {
        A->file=fopen(p,"r");
    }
    else
    {
        A->file=fopen(p,"w");
    }
    A->record_length=0;
    A->i_record=0;
    A->i_octet=0;
    A->nb_octets=0;
    return A;
}

void Ec_Bit(Bin_file *output,char bit)
{
    unsigned char octet,b;
    int i;
    
    output->octet[output->i_octet]=bit;
    output->i_octet++;
    if (output->i_octet==8)
    {
        octet=0;
        b=0x80;
        for(i=0;i<8;i++)
        {
            if(output->octet[i]=='1')
            {
                octet=octet|b;           
            }
            b=b>>1;
        }
        output->i_octet=0;
        output->record[output->i_record]=octet;
        printf("%c",output->record);
        output->i_record++;
        output->nb_octets++;
        if(output->i_record==BLOCK_SIZE)
        {
            fwrite(output->record,BLOCK_SIZE,1,output->file);
            output->i_record=0;
        }
    }

}

int main()
{
    Bin_file *p;
    int i;
    printf("test1\n");
    p=Ouv_Bit("test.txt",'w');
    printf("test2\n");
    for(i=0;i<8;i++)
    {
        Ec_Bit(p,'1');
    }
    printf("test3\n");
    fclose(p);
    return 0;
}