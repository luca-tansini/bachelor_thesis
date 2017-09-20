#include "Keccak-SHA3Instances.c"

int main(){

    int i,requiredOutputLen;
    char input[1024];  //massimo 1024 caratteri in input
    char output[1024]; //massimo 1024 caratteri in output

    printf("Inserisci messaggio in input: ");
    gets(input);

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

    printf("Keccak_224:\n");
    Keccak_224(input, strlen(input), output);
    for(i=0;i<224/8;i++)
        printf("%02hhx",output[i]);
    printf("\n\n");

    printf("Keccak_256:\n");
    Keccak_256(input, strlen(input), output);
    for(i=0;i<256/8;i++)
        printf("%02hhx",output[i]);
    printf("\n\n");

    printf("Keccak_384:\n");
    Keccak_384(input, strlen(input), output);
    for(i=0;i<384/8;i++)
        printf("%02hhx",output[i]);
    printf("\n\n");

    printf("Keccak_512:\n");
    Keccak_512(input, strlen(input), output);
    for(i=0;i<512/8;i++)
        printf("%02hhx",output[i]);
    printf("\n\n");

    printf("Che dimensione in bit vuoi per l'output per le XOF(SHAKE128 e SHAKE256)? (max 8192):\n");
    scanf("%d", &requiredOutputLen);

    printf("SHAKE128:\n");
    SHAKE128(input, strlen(input), output, requiredOutputLen);
    for(i=0;i<requiredOutputLen/8;i++)
        printf("%02hhx",output[i]);
    printf("\n\n");

    printf("SHAKE256:\n");
    SHAKE256(input, strlen(input), output, requiredOutputLen);
    for(i=0;i<requiredOutputLen/8;i++)
        printf("%02hhx",output[i]);
    printf("\n\n");

    return 0;
}
