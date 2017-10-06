#include "KeccakTools.c"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//Generazione di tutti i trails con due soli bit in colonna
//Sfruttando la proprietà per cui shiftare l'intero stato sull'asse z genera stati equivalenti sono solo 50 gli stati possibili
int main(int argc, char const *argv[]) {

    printf("Generazione di tutti i 3RoundTrails con due soli bit attivi, posizionati nella stessa colonna.\n");
    printf("Sfruttando la proprietà per cui shiftare l'intero stato sull'asse z genera stati equivalenti sono solo 50 gli stati possibili.\n\n");
    printf("Premere un tasto per iniziare...\n\n");
    getchar();

    uint64_t state[5][5];
    uint64_t trailState[5][5];
    int x,y,yOff,weight;
    int i,weights[80]={0};
    char sep[87];

    memset(sep, '*', 86);
    sep[86] = 0;
    memset(state, 0, 200);

    for(x=0;x<5;x++){
        for(y=0;y<4;y++){
            state[y][x] = 1;
            for(yOff=y+1;yOff<5;yOff++){
                state[yOff][x] = 1;
                memcpy(trailState, state, 200);
                printf(ANSI_COLOR_RED"%s\n"ANSI_COLOR_RESET,sep);
                VerboseForwardPropagateNRoundTrail(trailState,3,0, &weight);
                printf("Peso totale: %d\n", weight);
		weights[weight]++;
                state[yOff][x] = 0;
            }
            state[y][x] = 0;
        }
    }
    
    for(i=0;i<80;i++)
        if(weights[i])
            printf(" numero di vortici di peso %d: %d\n",i, weights[i]);

    return 0;
}
