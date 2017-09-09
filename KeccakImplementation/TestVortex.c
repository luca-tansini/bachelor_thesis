#include "KeccakTools.c"

int main(int argc, char const *argv[]) {

    FILE *fp;
    int i,j;
    uint64_t state[5][5];

    if(argc != 2){
        printf("Error! usage:TestVortex <first_trail_state_input>\n");
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
    Keccak_f_Rho(state);
    Keccak_f_Pi(state);
    PrintTrailState(state);

    return 0;
}
