/*
Nella seguente implementazione di Keccak-f vengono usati i prametri standard degli algoritmi SHA3 del NIST:
    l = 6 --> b = 1600 --> #round = 24
    egue che r + c deve essere 1600 e per comodità assumiamo che r sia multiplo di 8bit
Quindi al momento lo stato è rappresentato da una matrice 5x5 di uint64_t, che in alcune situazioni (tipo l'input e l'output) verrà utilizzato come 200 byte consecutivi di caratteri ASCII.
IMPORTANTISSIMOOOO: La rappresetazione (a parte combinare in modo strano gli indici che non ci interessa) ha le Righe chiamate Y e le colonne chiamate X,
Quindi state[i][j] rappresenta la riga j e colonna i della matrice
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned int uint;

//Lookup table per le rotazioni di Rho
uint64_t rotOffset[5][5] = {//DA CAMBIARE PER L'ORIENTAMENTO
    {0,1,62,28,27},
    {36,44,6,55,20},
    {3,10,43,25,39},
    {41,45,15,21,8},
    {18,2,61,56,14}
};

uint64_t roundConstant[24] = {
    0x0000000000000001, 0x0000000000008082,
    0x800000000000808A, 0x8000000080008000,
    0x000000000000808B, 0x0000000080000001,
    0x8000000080008081, 0x8000000000008009,
    0x000000000000008A, 0x0000000000000088,
    0x0000000080008009, 0x000000008000000A,
    0x000000008000808B, 0x800000000000008B,
    0x8000000000008089, 0x8000000000008003,
    0x8000000000008002, 0x8000000000000080,
    0x000000000000800A, 0x800000008000000A,
    0x8000000080008081, 0x8000000000008080,
    0x0000000080000001, 0x8000000080008008
};

uint64_t rotLeft(uint64_t x, int n) {
  return ((x << n) | (x >> (32 - n)));
}

void Keccak_f(uint64_t state[5][5], int round){

    int i,j,k;

    //Theta
    uint64_t col1,col2;
    col1=col2=0;
    for(i=0;i<5;i++){
        for(j=0;j<5;j++){
            //parità della colonna con x+1 y' e z-1
            for(k=0;k<5;k++){
                col1 ^= state[k][(i+1)%5];
            }
            //rotazione per ottenere z-1
            col1 = rotLeft(col1,1);
            //parità della colonna con x-1 y' e z
            for(k=0;k<5;k++){
                col2 ^= state[k][(i-1+5)%5]; //è stato aggiunto il +5 perchè in C l'operatore % è il resto e non il modulo...
            }
            state[j][i] ^= col1 ^ col2;
        }
    }

    //Rho
    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            state[j][i] = rotLeft(state[j][i], rotOffset[j][i]);

    //Pi
    uint64_t temp_state[5][5];

    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            temp_state[(i*2 + 3*j)%5][j] = state[j][i];

    memcpy(state, temp_state, sizeof(temp_state));

    //Chi
    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            state[j][i] = state[j][i] ^ ((~state[j][(i+1)%5]) & state[j][(i+2)%5]);

    //Iota
    state[0][0] ^= roundConstant[round];
}

void Keccak(uint r, char *input, uint inputLen, char *output, uint requiredOutputLen){

    uint64_t state[5][5];
    int i,j;

    if(r >= 1600 || r % 8 != 0){
        printf("Il bitrate deve essere minore di 1600 e multiplo di 8 bit!\n");
        return;
    }

    //Inizializzazione stato a 0
    memset(state, 0, sizeof(state));

    //Padding
    int padSize = ((r - inputLen*8 % r)%r)/8;
    int paddedInputLen = inputLen + padSize;
    unsigned char *paddedInput = malloc(paddedInputLen);
    unsigned char *padding = malloc(padSize);

    //nel caso ci sia un solo bit di padding padding = 10000001 = 0x81
    //EDIT: il padding viene modificato in 1010*1 per la condizione strana di SHA3
    if(padSize == 1)
        padding[0] = (char) 0xA1;
    else{
        padding[0] = (char) 0xA0;
        for(i=1; i<padSize-1; i++){
            padding[i] = (char) 0x0;
        }
        padding[padSize-1] = (char) 0x01;
    }

    memcpy(paddedInput, input, inputLen);
    memcpy((paddedInput+inputLen), padding, padSize);

    //Fase di Assorbimento
    //Per ogni blocco di input da r bit disponibile
    for(i=0; i<paddedInputLen/(r/8); i++){
        memcpy(state,(paddedInput+(r/8)*i),r/8);
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
