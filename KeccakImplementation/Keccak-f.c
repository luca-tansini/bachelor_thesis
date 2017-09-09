/*
Nella seguente implementazione di Keccak-f vengono usati i prametri standard degli algoritmi SHA3 del NIST:
    l = 6 --> b = 1600 --> #round = 24
    segue che r + c deve essere 1600 e per comodità assumiamo che r sia multiplo di 8bit
Quindi al momento lo stato è rappresentato da una matrice 5x5 di uint64_t, che in alcune situazioni (tipo l'input e l'output) verrà utilizzato come 200 byte consecutivi di caratteri ASCII.


***********************************************
VISUALIZZAZIONE DEL MESSAGGIO NELLA MATRICE:  *
(Tralasciando l'asse z)                       *
Messaggio M = 0,1,2,3,4,5,6,7,8,9,A,B,C,...,O *
                                              *
A[x][y] = M[5y + x]:                          *
                                              *
M[0] = A[0][0] = 0;                           *
M[1] = A[1][0] = 1;                           *
M[2] = A[2][0] = 2;                           *
     .                                        *
     .                                        *
M[17]= A[2][3] = H;                           *
     .                                        *
     .                                        *
M[24]= A[4][4] = O;                           *
                                              *
A:                                            *
                          w=64                *
                        /                     *
                      3                       *
                    2                         *
    y           ↗ 1                           *
    ↓          z0                             *
    0 0 1 2 3 4                               *
    1 5 6 7 8 9                               *
    2 A B C D E                               *
    3 F G H I J                               *
    4 K L M N O                               *
    x→0 1 2 3 4                               *
                                              *
***********************************************
IMPORTANTE: Nella rappresentazione della matrice che ho scelto in C, risulta più comodo vedere lo stato memorizzato per colonne cioè scambiare x e y:

A[x][y] = state[y][x];

Questo perchè in c un vettore bidimensionale A[5][5] viene salvato in memoria come:

    A[0][0],A[0][1],A[0][2],A[0][3],A[0][4],
    A[1][0],A[1][1],A[1][2],A[1][3],A[1][4],
    .
    .
    A[4][0],A[4][1],A[4][2],A[4][3],A[4][4]

*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned int uint;

//Lookup table per le rotazioni di Rho
int rotOffset[5][5] = {
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

//In theta lavoro per colonne, quindi ciclo prima su x
void Keccak_f_Theta(uint64_t state[5][5]){
    int y,x;

    uint64_t par[5],rot[5];
    //Precomputo la parità di tutte le colonne in par
    for(x=0;x<5;x++){
        par[x] = 0;
        for(y=0;y<5;y++)
            par[x] ^= state[y][x];
    }
    //Precomputo la rotazione di tutte le colonne in rot
    for(x=0;x<5;x++){
        rot[x] = rotLeft(par[x],1);
    }
    //Sommo rot[x+1] + par[x-1]
    for(x=0;x<5;x++){
        for(y=0;y<5;y++){
            state[y][x] ^= rot[(x+1)%5] ^ par[(x-1+5)%5];
        }
    }
}

void Keccak_f_Rho(uint64_t state[5][5]){
    int y,x;
    for(y=0;y<5;y++)
        for(x=0;x<5;x++)
            state[y][x] = rotLeft(state[y][x], rotOffset[y][x]);
}

void Keccak_f_Pi(uint64_t state[5][5]){
    int y,x;
    uint64_t tempState[5][5];
    for(y=0;y<5;y++)
        for(x=0;x<5;x++)
            //Qui vengono scambiate y e x perchè è la trasformazione che lo richiede
            tempState[(x*2 + 3*y)%5][y] = state[y][x];
    memcpy(state,tempState,200);
}

void Keccak_f_Chi(uint64_t state[5][5]){
    int y,x;
    uint64_t tempState[5][5];
    for(y=0;y<5;y++)
        for(x=0;x<5;x++)
            tempState[y][x] = state[y][x] ^ ((~state[y][(x+1)%5]) & state[y][(x+2)%5]);
    memcpy(state,tempState,200);
}

void Keccak_f_Iota(uint64_t state[5][5], int round){
    state[0][0] ^= roundConstant[round];
}

void Keccak_f(uint64_t state[5][5], int round){

    //Theta
    Keccak_f_Theta(state);

    //Rho
    Keccak_f_Rho(state);

    //Pi
    Keccak_f_Pi(state);

    //Chi
    Keccak_f_Chi(state);

    //Iota
    Keccak_f_Iota(state,round);
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

    //Il padding di SHA3 dato dal Nist dovrebbe essere M||01||10*1
    //Nel caso di un solo byte: 01100001 --> 0x61. Qui entra in gioco il bit ordering: il byte 0x61 viene scritto in memoria a partire dal bit meno significativo 10000110 e il C lo legge come 0x61, ma io voglio la stringa di bit esatta, quindi devo girarlo al contrario come 0x86.
    if(padSize == 1)
        padding[0] = SHA3PAD?0x86:0x81;
    else{
        padding[0] = SHA3PAD?0x06:0x01;
        for(i=1; i<padSize-1; i++){
            padding[i] = 0x00;
        }
        padding[padSize-1] = 0x80;
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
