#include "KeccakTools.c"

int main(int argc, char const *argv[]) {

    uint64_t state[5][5];
    int i,j;
    FILE *fp;

    if(argc != 2){
        printf("Error! usage:inverseTheta <first_trail_state_input>\n");
        exit(1);
    }

    if (!(fp=fopen(argv[1],"r"))) {
        printf("Error: '%s' no such file or directory\n",argv[1]);
        exit(1);
    }

    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            fscanf(fp,"%lx", &state[i][j]);
    fclose(fp);

    PrintTrailState(state);
    Keccak_f_Theta(state);
    Keccak_f_Rho(state);
    Keccak_f_Pi(state);
    PrintTrailState(state);
    Keccak_f_InversePi(state);
    Keccak_f_InverseRho(state);
    Keccak_f_InverseTheta(state);
    PrintTrailState(state);
    return 0;
}
