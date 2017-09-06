#include "Keccak-f.c"

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

void Keccak_224(char *input, uint inputLen, char *output){
    Keccak(1152, input, inputLen, output, 224/8, 0);
}

void Keccak_256(char *input, uint inputLen, char *output){
    Keccak(1088, input, inputLen, output, 256/8, 0);
}

void Keccak_384(char *input, uint inputLen, char *output){
    Keccak(832, input, inputLen, output, 384/8, 0);
}

void Keccak_512(char *input, uint inputLen, char *output){
    Keccak(576, input, inputLen, output, 512/8, 0);
}
