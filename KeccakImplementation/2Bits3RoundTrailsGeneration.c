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
    int i,j,k,weight;
    char sep[87];

    memset(sep, '*', 86);
    sep[86] = 0;
    memset(state, 0, 200);

    for(i=0;i<5;i++){
        for(j=0;j<4;j++){
            state[j][i] = 1;
            for(k=1;j+k<5;k++){
                state[j+k][i] = 1;
                memcpy(trailState, state, 200);
                printf(ANSI_COLOR_RED"%s\n"ANSI_COLOR_RESET,sep);
                VerbosePropagate3RoundTrail(trailState, 0, &weight);
                printf("Peso totale: %d\n", weight);
                printf(ANSI_COLOR_RED"%s\n"ANSI_COLOR_RESET,sep);
                state[j+k][i] = 0;
            }
            state[j][i] = 0;
        }
    }

    return 0;
}
