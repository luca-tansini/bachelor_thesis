#include "KeccakTools.c"

int main(int argc, char const *argv[]) {

    uint64_t state[5][5];
    int i,j,weight;
    int backwardExtended;
    FILE *fp;

    if(argc != 2){
        printf("Error! usage:Test3RoundTrail <first_trail_state_input>\n");
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

    VerbosePropagate3RoundTrail(state, backwardExtended, &weight);

    printf("\nPeso totale: %d\n", weight);

    return 0;
}
