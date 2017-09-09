#include "KeccakTools.c"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//Generazione di tutti i trails con 4bit attivi ed ogni colonna ha esattamente 0 o 2 bit attivi. Solamente i trail con peso <= 50 vengono stampati
//Gli stati possibili sono esattamente (64 * 50) * (64*50 - 10) = 10208000
int main(int argc, char const *argv[]) {

    uint64_t state[5][5];
    uint64_t tmpState[5][5];
    int i,j,k,l,weight;
    int x,y,z,y1;
    char sep[87];

    printf("Generazione di tutti i trails con 4bit attivi ed esattamente 0 o 2 bit attivi in ogni colonna.\nSolamente i trail con peso <= 50 vengono stampati\n");
    printf("Gli stati in esame sono esattamente (64 * 50) * (64*50 - 10) = 10208000\n");

    memset(sep, '*', 86);
    sep[86] = 0;
    memset(state, 0, 200);

    for(l=0;l<64;l++){
        for(i=0;i<5;i++){
            for(j=0;j<4;j++){
                state[j][i] = 0x01UL<<l;
                for(k=1;j+k<5;k++){
                    state[j+k][i] = 0x01UL << l;
                    for(z=0;z<64;z++){
                        for(x=0;x<5;x++){
                            //Se sto considerando la stessa colonna del ciclo esterno skippa
                            if(z==l && x==i)
                                continue;
                            for(y=0;y<4;y++){
                                state[y][x] ^= 0x01UL<<z;
                                for(y1=1;y+y1<5;y1++){
                                    state[y+y1][x] ^= 0x01UL << z;
                                    memcpy(tmpState, state, 200);
                                    Propagate3RoundTrail(tmpState, 0, &weight);
                                    if(weight <= 50){
                                        printf(ANSI_COLOR_RED"%s\n"ANSI_COLOR_RESET,sep);
                                        memcpy(tmpState, state, 200);
                                        VerbosePropagate3RoundTrail(tmpState, 0, &weight);
                                        printf("Peso totale: %d\n", weight);
                                        printf(ANSI_COLOR_RED"%s\n"ANSI_COLOR_RESET,sep);
                                    }
                                    state[y+y1][x] ^= 0x01UL << z;
                                }
                                state[y][x] ^= 0x01UL<<z;
                            }
                        }
                    }
                    state[j+k][i] = 0;
                }
                state[j][i] = 0;
            }
        }
    }
    return 0;
}
