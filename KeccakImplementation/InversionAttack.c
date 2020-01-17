#include "KeccakTools.c"

void InverseKeccak_f_round(uint64_t state[5][5], int round){
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
    
    //Il parametro r è passato in bit, ma per comodità viene convertito in byte, poichè il controllo ci garantisce di lavorare con multipli di byte
    r=r/8;

    //Inizializzazione stato a 0
    memset(victimState, 0, sizeof(victimState));

    //PADDING
    int padSize, paddedInputLen;
    unsigned char *paddedInput;
    unsigned char *padding;

    padSize = ((r - inputLen % r)%r);

    //Se il padSize è 0 significa che:
    //  -il blocco in input è vuoto
    //  -il blocco in input è di esattamente r bit
    //In entrambi i casi quello che devo fare è aggiungere un intero blocco di r bit di padding
    if(padSize == 0)
        padSize = r;
    paddedInputLen = inputLen + padSize;
    paddedInput = malloc(paddedInputLen);
    padding = malloc(padSize);

    //Il padding di SHA3 dato dal Nist è:
    //    -per SHA3   M||01||10*1
    //    -per SHAKE  M||1111||10*1
    //Nel caso di un solo byte: 01100001 --> 0x61. Qui entra in gioco il bit ordering: il byte 0x61 viene scritto in memoria a partire dal bit meno significativo 10000110 e il C lo legge come 0x61, ma io voglio la stringa di bit esatta, quindi devo girarlo al contrario come 0x86.
    if(padSize == 1)
        switch (PADTYPE) {
            //standard Keccak 10*1 padding
            case 0: padding[0] = 0x81;
                    break;
            //padding per SHA3
            case 1: padding[0] = 0x86;
                    break;
            //padding per SHAKE
            case 2: padding[0] = 0x9F;
                    break;
        }
    else{
        switch (PADTYPE) {
            //standard Keccak 10*1 padding
            case 0: padding[0] = 0x01;
                    break;
            //padding per SHA3
            case 1: padding[0] = 0x06;
                    break;
            //padding per SHAKE
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
    for(i=0; i<paddedInputLen/r; i++){
        for(j=0;j<r;j++){
            ((char *)victimState)[j] ^= (paddedInput+r*i)[j];
        }
        for(j=0;j<24;j++){
            Keccak_f(victimState, j);
        }
    }
    //FASE DI SQUEEZING
    while(requiredOutputLen > 0){
        if(requiredOutputLen > r){
            memcpy(output, victimState, r);
            output += r;
            requiredOutputLen -= r;
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
    char stolenInput[200];

    printf("Inserisci messaggio in input: ");
    gets(input);
    
    if(strlen(input) < 136)
	printf("\nMessaggio attaccabile!\n");
    else
	printf("\nMessaggio non attaccabile\n");

    char *output = malloc(64);

    VictimKeccak(1088, input, strlen(input), output, 256/8, 1);

    printf("\nSHA3-256:\n");
    for(i=0;i<256/8;i++)
        printf("%02hhx",output[i]);
    printf("\n\n");

    printf("Stato della memoria al termine della computazione di SHA3-256:\n");
    PrintState(victimState);
    for(i=23;i>=0;i--)
        InverseKeccak_f_round(victimState,i);
    
    printf("Stato della memoria dopo aver invertito i 24 round:\n");
    PrintState(victimState);
    
    memcpy(stolenInput,victimState,200);
    
    printf("Messaggio in chiaro:\""	);
	
	i=0;
	while(stolenInput[i] != 0 && stolenInput[i] != 0x86 && stolenInput[i] != 0x06)
		putchar(stolenInput[i++]);
	
	printf("\"\n");

    return 0;
}
