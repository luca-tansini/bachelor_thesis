#include "Keccak-SHA3Instances.c"

int main(){

    int i;
    char input[1024]; //massimo 1024 caratteri in input

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

    return 0;
}
