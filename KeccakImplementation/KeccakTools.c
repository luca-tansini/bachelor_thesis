#include <stdio.h>
#include <stdint.h>
#include "Keccak-f.c"

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

    int x;
    *row = 0x0UL;
    uint64_t tmp;

    if(y<0 || y>4 || z<0 || z>63)
        return;

    for(x=0;x<5;x++){
        tmp = ((state[y][x] & (0x1UL << (63-z))) >> (63-z));
        *row ^= tmp << (4-x);
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

//Funzione che prende in input il primo stato di un trail e lo propaga per 3 Round calcolandone il peso
//backwardExtended indica se il primo stato rappresenta a0, cioè il trail prima di λ0 o b0, cioè il trail prima di X0, poichè a0 può essere recuperato applicando λ inversa
void Propagate3RoundTrail(uint64_t trailState[5][5], int backwardExtended, int *totalWeight){

    int weight, revWeight;

    //Apllico i 3 round, tranne Chi che viene considerata identità e ne calcolo il peso e Iota che non fa nulla per i differenziali
    if(backwardExtended){
        //a0(before λ0)
        Keccak_f_Theta(trailState);
        Keccak_f_Rho(trailState);
        Keccak_f_Pi(trailState);
    }
    //b0(before χ0)
    getStateWeights(trailState,&weight,&revWeight);
    *totalWeight = weight;
    //a1(before λ1)
    Keccak_f_Theta(trailState);
    Keccak_f_Rho(trailState);
    Keccak_f_Pi(trailState);
    //b1(before χ1)
    getStateWeights(trailState,&weight,&revWeight);
    *totalWeight += weight;
    //a2(before λ2)
    Keccak_f_Theta(trailState);
    Keccak_f_Rho(trailState);
    Keccak_f_Pi(trailState);
    //b2(before χ2)
    getStateWeights(trailState,&weight,&revWeight);
    *totalWeight += weight;
}

//Stessa funzionalità della funzione di prima ma produce output su stdout
void VerbosePropagate3RoundTrail(uint64_t trailState[5][5], int backwardExtended, int *totalWeight){

    int weight, revWeight;

    //Apllico i 3 round, tranne Chi che viene considerata identità e ne calcolo il peso e Iota che non fa nulla per i differenziali
    if(backwardExtended){
        printf("a0(before λ0):\n");
        PrintTrailState(trailState);
        Keccak_f_Theta(trailState);
        Keccak_f_Rho(trailState);
        Keccak_f_Pi(trailState);
    }
    printf("b0(before χ0):\n");
    PrintTrailState(trailState);
    getStateWeights(trailState,&weight,&revWeight);
    *totalWeight = weight;
    printf("peso: %d\n\n", weight);
    printf("a1(before λ1):\n");
    PrintTrailState(trailState);
    Keccak_f_Theta(trailState);
    Keccak_f_Rho(trailState);
    Keccak_f_Pi(trailState);
    printf("b1(before χ1):\n");
    PrintTrailState(trailState);
    getStateWeights(trailState,&weight,&revWeight);
    *totalWeight += weight;
    printf("peso: %d\n\n", weight);
    printf("a2(before λ2):\n");
    PrintTrailState(trailState);
    Keccak_f_Theta(trailState);
    Keccak_f_Rho(trailState);
    Keccak_f_Pi(trailState);
    printf("b2(before χ2):\n");
    PrintTrailState(trailState);
    getStateWeights(trailState,&weight,&revWeight);
    printf("peso: %d\n\n", weight);
    *totalWeight += weight;
}
