#include <stdio.h>
#include <stdint.h>

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

//Per quanto riguarda la coordinata z ricorda che il bit più significativo dei 64 è quello con z=0
void getRow(uint64_t state[5][5], int y, int z, uint64_t *row){

    int i;
    *row = 0;
    uint64_t tmp;

    if(y<0 || y>4 || z<0 || z>63)
        return;

    for(i=0;i<5;i++){
        tmp = ((state[y][i] & (0x1UL << (63-z))) >> (63-z));
        *row |= tmp << (4-i);
    }
}

int weightTable[32] = {0,2,2,3,2,3,2,4,2,3,3,3,3,3,4,4,2,3,3,4,3,3,3,4,3,4,3,4,4,4,4,4};
int revWeightTable[32] = {0,2,2,2,2,2,2,2,2,2,2,3,2,3,2,3,2,2,2,2,2,3,3,3,2,2,3,3,2,3,3,3};

int getRowWeight(uint64_t row){
    return weightTable[row];
}

int getRevRowWeight(uint64_t row){
    return revWeightTable[row];
}

//Funzione che calcola il restriction weight e il rev weight di uno stato sommando i pesi delle righe.
//Sfrutta le lookup table costruite da me sopra (si spera giuste)
void getStateWeights(uint64_t state[5][5], int *weight, int *revWeight){
    int y,z;
    uint64_t tmpRow;
    *weight = 0;
    *revWeight = 0;
    for(y=0;y<5;y++)
        for(z=0;z<64;z++){
            getRow(state,y,z,&tmpRow);
            *weight += getRowWeight(tmpRow);
            *revWeight += getRevRowWeight(tmpRow);
        }
}
