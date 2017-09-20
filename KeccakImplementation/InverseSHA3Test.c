#include "KeccakTools.c"

void InverseKeccak_f(uint64_t state[5][5], int round){
    Keccak_f_Iota(state,round);
    Keccak_f_InverseChi(state);
    Keccak_f_InversePi(state);
    Keccak_f_InverseRho(state);
    Keccak_f_InverseTheta(state);
}

uint64_t victimState[5][5];

void VictimKeccak(uint r, char *input, uint inputLen, char *output, uint requiredOutputLen, uint PADTYPE){

    int i,j;

    if(r >= 1600 || r % 8 != 0){
        printf("Il bitrate deve essere minore di 1600 e multiplo di 8 bit!\n");
        return;
    }

    //Inizializzazione stato a 0
    memset(victimState, 0, sizeof(victimState));

    //PADDING
    int padSize, paddedInputLen;
    unsigned char *paddedInput;
    unsigned char *padding;

    padSize = ((r - inputLen*8 % r)%r)/8;

    if(padSize == 0)
        padSize = r/8;
    paddedInputLen = inputLen + padSize;
    paddedInput = calloc(paddedInputLen,1);
    padding = calloc(padSize,1);

    if(padSize == 1)
        switch (PADTYPE) {
            case 0: padding[0] = 0x81;
                    break;
            case 1: padding[0] = 0x86;
                    break;
            case 2: padding[0] = 0x9F;
                    break;
        }
    else{
        switch (PADTYPE) {
            case 0: padding[0] = 0x01;
                    break;
            case 1: padding[0] = 0x06;
                    break;
            case 2: padding[0] = 0x1F;
                    break;
        }
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
            ((char *)victimState)[j] ^= (paddedInput+(r/8)*i)[j];
        }
        for(j=0;j<24;j++){
            Keccak_f(victimState, j);
        }
    }
    //FASE DI SQUEEZING
    while(requiredOutputLen > 0){
        if(requiredOutputLen > r/8){
            memcpy(output, victimState, r/8);
            output += r/8;
            requiredOutputLen -= r/8;
            for(j=0;j<24;j++){
                Keccak_f(victimState, j);
            }
        }
        else{
            memcpy(output, victimState, requiredOutputLen);
            requiredOutputLen = 0;
        }
    }
}

int main(int argc, char const *argv[]) {

    int i;
    char input[1024];

    printf("Inserisci messaggio in input: ");
    gets(input);

    char *output = malloc(64);

    VictimKeccak(1088, input, strlen(input), output, 256/8, 1);

    printf("SHA3_256:\n");
    for(i=0;i<256/8;i++)
        printf("%02hhx",output[i]);
    printf("\n\n");

    PrintState(victimState);
    for(i=23;i>=0;i--)
        InverseKeccak_f(victimState,i);
    PrintState(victimState);
    for(i=0;i<200;i++)
        putchar(*(  ((unsigned char *)victimState)+i) );
    printf("\n");

    return 0;
}
