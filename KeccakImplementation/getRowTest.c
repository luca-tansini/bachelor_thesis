#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "KeccakTools.c"

int main(int argc, char const *argv[]){
    FILE *fp;
    int i,j;
    uint64_t state[5][5];

    if(argc != 2){
        printf("Error! usage: getRowTest <first_trail_state_input>\n");
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

    uint64_t row;

    getRow(state, 4, 63, &row);
    printf("%lX\n", row);

    //uint64_t test = 0x0000400000000000UL;

}
