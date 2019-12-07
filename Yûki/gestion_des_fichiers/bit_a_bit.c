#include <stdio.h>
#include <assert.h>
#define BIN_MAX 8

void binaire(unsigned int n, char s[]);

int main(int argc, char **argv){
    FILE *file;
    char buffer;
    int cursor,c;
    file=fopen("text.txt","rb");
    // EOF : End Of File
    while((c=fgetc(file))!=EOF){
        char sb[BIN_MAX+1];
        binaire(32,sb);
        printf("%s\n",sb);
    }
    fclose(file);
    return 0;
}

void binaire(unsigned int n, char s[]){
    /*
        Décomposition binaire d'un entier (<BIN_MAX) et place celle-ci dans une chaîne de caractères passée en argument (s)

    */
    assert(n>=BIN_MAX);
    s[BIN_MAX]= '\0';
    int i,r;
    i=0;
    while(n!=0){
        r = n%2;
        n = n/2;
        if(0<=(BIN_MAX-i-1) && (BIN_MAX-i-1)<=BIN_MAX){
            if(r==0) s[BIN_MAX-i-1]= '0';
            else s[BIN_MAX-i-1]= '1';
            i++;
        }
    }
}
