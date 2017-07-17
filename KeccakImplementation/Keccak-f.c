/*
Nella seguente implementazione di Keccak-f vengono usati i prametri standard degli algoritmi SHA3 del NIST:
    l = 6 --> b = 1600 --> #round = 24
    egue che r + c deve essere 1600 e per comodità assumiamo che r sia multiplo di 8bit
Quindi al momento lo stato è rappresentato da una matrice 5x5 di uint64_t, che in alcune situazioni (tipo l'input e l'output) verrà utilizzato come 200 byte consecutivi di caratteri ASCII.
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned int uint;

//Lookup table per le rotazioni di Rho
uint64_t rotOffset[5][5] = {
    {0,1,62,28,27},
    {36,44,6,55,20},
    {3,10,43,25,39},
    {41,45,15,21,8},
    {18,2,61,56,14}
};

uint64_t roundConstant[24] = {
    0x0000000000000001UL, 0x0000000000008082UL,
    0x800000000000808AUL, 0x8000000080008000UL,
    0x000000000000808BUL, 0x0000000080000001UL,
    0x8000000080008081UL, 0x8000000000008009UL,
    0x000000000000008AUL, 0x0000000000000088UL,
    0x0000000080008009UL, 0x000000008000000AUL,
    0x000000008000808BUL, 0x800000000000008BUL,
    0x8000000000008089UL, 0x8000000000008003UL,
    0x8000000000008002UL, 0x8000000000000080UL,
    0x000000000000800AUL, 0x800000008000000AUL,
    0x8000000080008081UL, 0x8000000000008080UL,
    0x0000000080000001UL, 0x8000000080008008UL
};

uint64_t rotLeft(uint64_t x, int n) {
  return ((x << n) | (x >> (64 - n)));
}

void Keccak_f(uint64_t state[5][5], int round){

    int i,j;

    //Theta
    uint64_t par[5],rot[5];
    //Precomputo la parità di tutte le colonne in par
    for(i=0;i<5;i++){
        par[i] = 0;
        for(j=0;j<5;j++)
            par[i] ^= state[j][i];
    }

    //Precomputo la rotazione di tutte le colonne in rot
    for(i=0;i<5;i++){
        rot[i] = rotLeft(par[i],1);
    }
    //Sommo rot[x+1] + par[x-1], ma devo muovermi per colonne qui!
    for(i=0;i<5;i++){
        for(j=0;j<5;j++){
            state[j][i] ^= rot[(i+1)%5] ^ par[(i-1+5)%5];
        }
    }

    //Rho
    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            state[i][j] = rotLeft(state[i][j], rotOffset[i][j]);

    //Pi
    uint64_t temp_state[5][5];

    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            temp_state[(i*2 + 3*j)%5][j] = state[j][i];

    //Chi
    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            state[i][j] = temp_state[i][j] ^ ((~temp_state[i][(j+1)%5]) & temp_state[i][(j+2)%5]);

    //Iota
    state[0][0] ^= roundConstant[round];
}

void Keccak(uint r, char *input, uint inputLen, char *output, uint requiredOutputLen, uint SHA3PAD){

    uint64_t state[5][5];
    int i,j;

    if(r >= 1600 || r % 8 != 0){
        printf("Il bitrate deve essere minore di 1600 e multiplo di 8 bit!\n");
        return;
    }

    //Inizializzazione stato a 0
    memset(state, 0, sizeof(state));

    //PADDING
    int padSize, paddedInputLen;
    unsigned char *paddedInput;
    unsigned char *padding;

    padSize = ((r - inputLen*8 % r)%r)/8;

    //Se il padSize è 0 significa che:
    //  -il blocco in input è vuoto
    //  -il blocco in input è di esattamente r bit
    //In ogni caso quello che devo fare è aggiungere un intero blocco di r bit di padding
    if(padSize == 0)
        padSize = r/8;
    paddedInputLen = inputLen + padSize;
    paddedInput = calloc(paddedInputLen,1);
    padding = calloc(padSize,1);

    if(padSize == 1)
        padding[0] = SHA3PAD?0x86:0x81;
    else{
        padding[0] = SHA3PAD?0x06:0x80;
        for(i=1; i<padSize-1; i++){
            padding[i] = 0x00;
        }
        padding[padSize-1] = SHA3PAD?0x80:0x01;
    }

    memcpy(paddedInput, input, inputLen);
    memcpy((paddedInput+inputLen), padding, padSize);

    //FASE DI ASSORBIMENTO
    for(i=0; i<paddedInputLen/(r/8); i++){
        for(j=0;j<r/8;j++){
            ((char *)state)[j] ^= (paddedInput+(r/8)*i)[j];
        }
        for(j=0;j<24;j++){
            Keccak_f(state, j);
        }
    }
    //FASE DI SQUEEZING
    while(requiredOutputLen > 0){
        if(requiredOutputLen > r/8){
            memcpy(output, state, r/8);
            output += r/8;
            requiredOutputLen -= r/8;
            for(j=0;j<24;j++){
                Keccak_f(state, j);
            }
        }
        else{
            memcpy(output, state, requiredOutputLen);
            requiredOutputLen = 0;
        }
    }
}

void SHA3_224(char *input, uint inputLen, char *output){
    Keccak(1152, input, inputLen, output, 224/8, 1);
}

void SHA3_256(char *input, uint inputLen, char *output){
    Keccak(1088, input, inputLen, output, 256/8, 1);
}

void SHA3_384(char *input, uint inputLen, char *output){
    Keccak(832, input, inputLen, output, 384/8, 1);
}

void SHA3_512(char *input, uint inputLen, char *output){
    Keccak(576, input, inputLen, output, 512/8, 1);
}

int main(){

    int i;
    char input[1024]; //per il momento funziona con al massimo 1024 caratteri in input

    printf("Inserisci messaggio in input: ");
    gets(input);

    char *output = malloc(64);

    printf("SHA3_224:\n");
    SHA3_224(input, strlen(input), output);
    for(i=0;i<224/8;i++)
        printf("%02hhx",output[i]);
    printf("\n\n");

    printf("SHA3_256:\n");
    SHA3_256(input, strlen(input), output);
    for(i=0;i<256/8;i++)
        printf("%02hhx",output[i]);
    printf("\n\n");

    printf("SHA3_384:\n");
    SHA3_384(input, strlen(input), output);
    for(i=0;i<384/8;i++)
        printf("%02hhx",output[i]);
    printf("\n\n");

    printf("SHA3_512:\n");
    SHA3_512(input, strlen(input), output);
    for(i=0;i<512/8;i++)
        printf("%02hhx",output[i]);
    printf("\n\n");

    return 0;
}
