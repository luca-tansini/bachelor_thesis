#include <stdio.h>

void PrintState(uint64_t state[5][5]){
    int i,j;
    for(i=0;i<86;i++)
        printf("_");
    printf("\n");
    for(i=0;i<5;i++){
        printf("|");
        for (j=0;j<5;j++){
            printf("%016lX|", state[i][j]);
        }
        printf("\n");
    }
    for(i=0;i<86;i++)
        printf("¯");
    printf("\n");
}

//Funzione che sostituisce nella stringa s tutte le occorrenze del carattere a con il carattere b
void str_replace(char *s, char a, char b){
    for(;*s;s++)
        if(*s==a)
            *s=b;
}

void PrintTrailState(uint64_t state[5][5]){
    int i,j;
    char buf[17];
    for(i=0;i<86;i++)
        printf("_");
    printf("\n");
    for(i=0;i<5;i++){
        printf("|");
        for (j=0;j<5;j++){
            sprintf(buf,"%016lX|", state[i][j]);
            str_replace(buf,'0','-');
            printf("%s",buf);
        }
        printf("\n");
    }
    for(i=0;i<86;i++)
        printf("‾");
    printf("\n");
}
