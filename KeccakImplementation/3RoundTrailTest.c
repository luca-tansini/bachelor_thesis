#include "Keccak-f.c"

int main(int argc, char const *argv[]) {

    uint64_t state[5][5];
    int i,j;
    int backwardExtended;
    FILE *fp;

    if(argc != 2){
        printf("Error! usage: 3RoundTrailTest <first_trail_state_input>\n");
        exit(1);
    }

    if (!(fp=fopen(argv[1],"r"))) {
        printf("Error: '%s' no such file or directory\n",argv[1]);
        exit(1);
    }

    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            fscanf(fp,"%lx", &state[i][j]);
    fscanf(fp,"%d",&backwardExtended);
    fclose(fp);

    //Apllico i 3 round, tranne Chi che viene considerata identità e Iota che non fa nulla e vedo cosa succede
    if(backwardExtended){
        printf("a0(before λ0):\n");
        PrintState(state);
        Keccak_f_Theta(state);
        Keccak_f_Rho(state);
        Keccak_f_Pi(state);
    }
    printf("b0(before χ0):\n");
    PrintState(state);
    printf("a1(before λ1):\n");
    PrintState(state);
    Keccak_f_Theta(state);
    Keccak_f_Rho(state);
    Keccak_f_Pi(state);
    printf("b1(before χ1):\n");
    PrintState(state);
    printf("a2(before λ2):\n");
    PrintState(state);
    Keccak_f_Theta(state);
    Keccak_f_Rho(state);
    Keccak_f_Pi(state);
    printf("b2(before χ2):\n");
    PrintState(state);

    return 0;
}
