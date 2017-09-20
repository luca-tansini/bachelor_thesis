#include "KeccakTools.c"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//Generazione di tutti gli stati con 4bit attivi ed esattamente 0 o 2 bit attivi in ogni colonna.
int main(int argc, char const *argv[]) {

    uint64_t state[5][5];
    uint64_t tmpState[5][5];
    int x,y,yOff,weight,count=0;
    int x1,y1,z1,y1Off;
    char sep[87];
    printf("Generazione di tutti gli stati con 4bit attivi ed esattamente 0 o 2 bit attivi in ogni colonna.\n");
    printf("Premere un tasto per iniziare...\n\n");
    getchar();

    memset(sep, '*', 86);
    sep[86] = 0;
    memset(state, 0, 200);
    int weights[128] = {0};

    for(x=0;x<5;x++){
        for(y=0;y<4;y++){
            state[y][x] = 0x01UL;
            for(yOff=1;y+yOff<5;yOff++){
                state[y+yOff][x] = 0x01UL;
                for(z1=0;z1<64;z1++){
                    for(x1=x;x1<5;x1++){
                        if(x==x1 && z1==0)
                            continue;
                        for(y1=0;y1<4;y1++){
                            state[y1][x1] ^= 0x01UL<<z1;
                            for(y1Off=1;y1+y1Off<5;y1Off++){
                                state[y1+y1Off][x1] ^= 0x01UL << z1;
                                memcpy(tmpState, state, 200);
                                ForwardPropagateNRoundTrail(tmpState, 3, 0, &weight);
                                weights[weight]++;
                                if(weight < 50){
                                    printf(ANSI_COLOR_RED"%s\n"ANSI_COLOR_RESET,sep);
                                    memcpy(tmpState, state, 200);
                                    VerboseForwardPropagateNRoundTrail(tmpState, 3, 0, &weight);
                                    printf("Peso totale: %d\n", weight);
                                    printf(ANSI_COLOR_RED"%s\n"ANSI_COLOR_RESET,sep);
                                }
                                count++;
                                state[y1+y1Off][x1] ^= 0x01UL << z1;
                            }
                            state[y1][x1] ^= 0x01UL<<z1;
                        }
                    }
                }
                state[y+yOff][x] = 0;
            }
            state[y][x] = 0;
        }
    }

    printf("Stati esaminati: %d\n", count);
    for(x=0;x<128;x++)
        if(weights[x]>0)
            printf("Numero di 3-RoundTrails di peso %d: %d\n",x,weights[x]);
    return 0;
}
