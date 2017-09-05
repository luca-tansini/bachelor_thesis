#include "Keccak-f.c"

int main(int argc, char const *argv[]) {

    uint64_t state[5][5] = {
        {0xF135E26BC449789A, 0x4D789AF135A226BC, 0x5E26BC4D78B5AF13, 0x89AF135E268FC4D7, 0x6BC4D789AF1D35E2},
        {0xF135E26BC449789A, 0x4D789AF135A226BC, 0x5E26BC4D78B5AF13, 0x89AF135E268FC4D7, 0x6BC4D789AF1D35E2},
        {0xF135E26BC449789A, 0x4D789AF135A226BC, 0x5E26BC4D78B5AF13, 0x89AF135E268FC4D7, 0x6BC4D789AF1D35E2},
        {0xF135E26BC4C9789A, 0x4D789AF135AA26BC, 0x5E26BC4D78B5AF13, 0x89AF135E268FC4D7, 0x6BC4D789AF1D35E2},
        {0xF135E26BC449789A, 0x4D789AF135A226BC, 0x5E26BC4D78B5AF13, 0x89AF135E268FC4D7, 0x6BC4D789AF1D35E2}
    };

    //Apllico i 3 round, tranne Chi che viene considerata identità e Iota che non fa nulla e vedo cosa succede
    printf("a0:\n");
    PrintState(state);
    Keccak_f_Theta(state);
    Keccak_f_Rho(state);
    Keccak_f_Pi(state);
    printf("b0:\n");
    PrintState(state);
    printf("a1:\n");
    PrintState(state);
    Keccak_f_Theta(state);
    Keccak_f_Rho(state);
    Keccak_f_Pi(state);
    printf("b1:\n");
    PrintState(state);
    printf("a2:\n");
    PrintState(state);
    Keccak_f_Theta(state);
    Keccak_f_Rho(state);
    Keccak_f_Pi(state);
    printf("b2:\n");
    PrintState(state);
    printf("a3:\n");
    PrintState(state);

    return 0;
}
