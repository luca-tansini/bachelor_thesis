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

void Keccak(uint r, char *input, uint inputLen, char *output, uint requiredOutputLen){

    uint64_t state[5][5];
    int i;

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

    /*DEBUG PRINTF
    printf("Il messaggio in input e': %s\n", input);
    printf("Quindi la lunghezza dell'input e': %dbyte == %dbit\n", inputLen, inputLen*8);
    printf("Visto che il bitrate e' %dbit, il padding sarà lungo %dbyte == %dbit\n", r, padSize, padSize*8);*/

    //nel caso ci sia un solo bit di padding padding = 10000001 = 0x81
    if(padSize == 1)
        padding[0] = (char) 0x81;
    else{
        padding[0] = (char) 0x80;
        for(i=1; i<padSize-1; i++){
            padding[i] = (char) 0x0;
        }
        padding[padSize-1] = (char) 0x01;
    }

    memcpy(paddedInput, input, inputLen);
    memcpy((paddedInput+inputLen), padding, padSize);

    /*DEBUG PRINTF
    printf("Padding: ");
    for(i=0;i<padSize;i++)
        printf("0x%02hhx ", padding[i]);
    printf("\n");

    printf("PaddedInput:\n");
    for(i=0;i<paddedInputLen;i++)
        printf("%d: %02hhx\n", i, paddedInput[i]);
    printf("\n");*/
    
}

int main(){

    int rate;
    char input[1024]; //per il momento funziona con al massimo 1024 caratteri in input

    printf("Inserisci messaggio in input: ");
    gets(input); //apertissimi ai buffer overrun, cazzomene

    printf("Inserisci rate r: ");
    scanf("%d", &rate);

    Keccak(rate, input, strlen(input), NULL, -1);

    return 0;
}
