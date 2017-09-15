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

//Funzione che prende in input il primo stato di un trail e lo propaga in avanti per N Round calcolandone il peso
//backwardExtended indica se il primo stato rappresenta a0, cioè il trail prima di λ0 o b0, cioè il trail prima di X0, poichè a0 può essere recuperato applicando λ inversa
void ForwardPropagateNRoundTrail(uint64_t trailState[5][5],int rounds, int backwardExtended, int *totalWeight){

    int weight, revWeight,i;
    //Apllico gli n round, tranne Chi che viene considerata identità e ne calcolo il peso e Iota che non fa nulla per i differenziali
    if(backwardExtended){
        Keccak_f_Theta(trailState);
        Keccak_f_Rho(trailState);
        Keccak_f_Pi(trailState);
    }
    getStateWeights(trailState,&weight,&revWeight);
    *totalWeight = weight;
    for(i=1;i<rounds;i++){
        Keccak_f_Theta(trailState);
        Keccak_f_Rho(trailState);
        Keccak_f_Pi(trailState);
        getStateWeights(trailState,&weight,&revWeight);
        *totalWeight += weight;
    }
}

void VerboseForwardPropagateNRoundTrail(uint64_t trailState[5][5],int rounds, int backwardExtended, int *totalWeight){

    int weight, revWeight,i;
    //Apllico gli n round, tranne Chi che viene considerata identità e ne calcolo il peso e Iota che non fa nulla per i differenziali
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

    for(i=1;i<rounds;i++){
        printf("a%d(before λ%d):\n",i,i);
        PrintTrailState(trailState);
        Keccak_f_Theta(trailState);
        Keccak_f_Rho(trailState);
        Keccak_f_Pi(trailState);
        printf("b%d(before χ%d):\n",i,i);
        PrintTrailState(trailState);
        getStateWeights(trailState,&weight,&revWeight);
        *totalWeight += weight;
        printf("peso: %d\n\n", weight);
    }
}

int mod(int n, int m){
    if(m <= 1)
        return 0;
    while(n < 0)
        n += m;
    return (n%m);
}

uint64_t inversePositions64[5] = {
    0xDE26BC4D789AF134ULL,
    0x09AF135E26BC4D78ULL,
    0xEBC4D789AF135E26ULL,
    0x7135E26BC4D789AFULL,
    0xCD789AF135E26BC4ULL };

void Keccak_f_InverseTheta(uint64_t state[5][5]){
    int x,y,z,xOff;
    uint64_t C[5];
    uint64_t inversePositions[5];
    for(x=0; x<5; x++) {
        C[x] = state[0][x];
        for(y=1; y<5; y++){
            C[x] ^= state[y][x];
        }
    }

    for(x=0; x<5; x++)
        inversePositions[x] = inversePositions64[x];

    for(z=0; z<64; z++){
        for(xOff=0; xOff<5; xOff++)
           for(x=0; x<5; x++)
               for(y=0; y<5; y++){
                    if((inversePositions[xOff] & (0x01UL << z)) != 0x0UL)
                        state[y][x] ^= C[mod(x-xOff,5)];
                }
        for(xOff=0; xOff<5; xOff++) {
            C[xOff]=rotLeft(C[xOff], 1);
        }
    }
}

void Keccak_f_InverseRho(uint64_t state[5][5]){
    int y,x;
    for(y=0;y<5;y++)
        for(x=0;x<5;x++)
            state[y][x] = rotLeft(state[y][x], mod(-rotOffset[y][x],64));
}

void Keccak_f_InversePi(uint64_t state[5][5]){
    uint64_t tmpState[5][5];
    int x,y,X,Y;
    for(X=0; X<5; X++)
        for(Y=0; Y<5; Y++) {
            x = (1*X + 3*Y)%5;
            y = (1*X + 0*Y)%5;
            tmpState[y][x] = state[Y][X];
        }
    memcpy(state,tmpState,200);
}

void InverseLambda(uint64_t state[5][5]){
    Keccak_f_InversePi(state);
    Keccak_f_InverseRho(state);
    Keccak_f_InverseTheta(state);
}
