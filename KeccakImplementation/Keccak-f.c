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

    int i,j,k;

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

    printf("Finita Theta:\n");
    for(k=0;k<5;k++){
        for (j=0;j<5;j++)
            printf("%lu ", state[k][j]);
        printf("\n");
    }
    printf("\n");

    //Rho
    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            state[i][j] = rotLeft(state[i][j], rotOffset[i][j]);

    printf("Finita Rho:\n");
    for(k=0;k<5;k++){
        for (j=0;j<5;j++)
            printf("%lu ", state[k][j]);
        printf("\n");
    }
    printf("\n");

    //Pi
    uint64_t temp_state[5][5];

    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            temp_state[j][(i*2 + 3*j)%5] = state[i][j];

    memcpy(state, temp_state, sizeof(temp_state));

    printf("Finita Pi:\n");
    for(k=0;k<5;k++){
        for (j=0;j<5;j++)
            printf("%lu ", state[k][j]);
        printf("\n");
    }
    printf("\n");

    //Chi
    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            state[i][j] = state[i][j] ^ ((~state[(i+1)%5][j]) & state[(i+2)%5][j]);

    printf("Finita Chi:\n");
    for(k=0;k<5;k++){
        for (j=0;j<5;j++)
            printf("%lu ", state[k][j]);
        printf("\n");
    }
    printf("\n");

    //Iota
    state[0][0] ^= roundConstant[round];

    printf("Finita Iota:\n");
    for(k=0;k<5;k++){
        for (j=0;j<5;j++)
            printf("%lu ", state[k][j]);
        printf("\n");
    }
    printf("\n");
    sleep(10000);

}

void Keccak(uint r, char *input, uint inputLen, char *output, uint requiredOutputLen){

    uint64_t state[5][5];
    int i,j,k;

    if(r >= 1600 || r % 8 != 0){
        printf("Il bitrate deve essere minore di 1600 e multiplo di 8 bit!\n");
        return;
    }

    //Inizializzazione stato a 0
    memset(state, 0, sizeof(state));

    //Padding
    int padSize = ((r - inputLen*8 % r)%r)/8;
    int paddedInputLen = inputLen + padSize;
    unsigned char *paddedInput = calloc(paddedInputLen,1);
    unsigned char *padding = calloc(padSize,1);

    /*DEBUG PRINTF
    printf("Il messaggio in input e': %s\n", input);
    printf("Quindi la lunghezza dell'input e': %dbyte == %dbit\n", inputLen, inputLen*8);
    printf("Visto che il bitrate e' %dbit, il padding sarà lungo %dbyte == %dbit\n", r, padSize, padSize*8);*/

    if(padSize == 1)
        padding[0] = 0x86;
    else{
        padding[0] = 0x06;
        for(i=1; i<padSize-1; i++){
            padding[i] = 0x00;
        }
        padding[padSize-1] = 0x80;
    }

    memcpy(paddedInput, input, inputLen);
    memcpy((paddedInput+inputLen), padding, padSize);

    //Fase di Assorbimento
    //Per ogni blocco di input da r bit disponibile
    for(i=0; i<paddedInputLen/(r/8); i++){
        memcpy(state,(paddedInput+(r/8)*i),r/8);
        //DEBUG
        /*unsigned char prova[200];
        memcpy(prova,state,200);
        for (j=0;j<200;j++){
            if(j%8==0)
			         printf("\n");
            printf("%02hhx ", prova[j]);
        }
        printf("\n");*/

        for(k=0;k<5;k++){
            for (j=0;j<5;j++)
                printf("%lu ", state[k][j]);
            printf("\n");
        }
        //END DEBUG
        for(j=0;j<24;j++){
            Keccak_f(state, j);
        }
    }

    //Fase di squeezing abbozzata
    char preoutput[r];
    memcpy(preoutput, state, r/8);
    memcpy(output, preoutput, requiredOutputLen/8);

}

int main(){

    int rate,outlen,i;
    char input[1024]; //per il momento funziona con al massimo 1024 caratteri in input

    printf("Inserisci messaggio in input: ");
    gets(input); //apertissimi ai buffer overrun, cazzomene

    printf("Inserisci rate r: ");
    scanf("%d", &rate);

    printf("Inserisci lunghezza output richiesta: ");
    scanf("%d", &outlen);

    char *output = malloc(outlen);

    Keccak(rate, input, strlen(input), output, outlen);

    for(i=0;i<outlen/8;i++)
        printf("%02hhx",output[i]);
    printf("\n");

    return 0;
}
