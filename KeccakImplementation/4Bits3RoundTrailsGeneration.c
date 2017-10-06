#include "KeccakTools.c"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define WEIGHT_THRESHOLD 50

//Generazione di tutti gli stati con 4bit attivi ed esattamente 0 o 2 bit attivi in ogni colonna.
int main(int argc, char const *argv[]) {

    uint64_t state[5][5];
    uint64_t tmpState[5][5];
    int x,y,yOff,weight,count=0;
    int x1,y1,z1,y1Off;
    char sep[87];
    printf("Generazione di tutti gli stati con 4bit attivi ed un numero pari di bit in ogni colonna.\n");
    printf("Premere un tasto per iniziare...\n\n");
    getchar();

    memset(sep, '*', 86);
    sep[86] = 0;
    memset(state, 0, 200);
    int weights[128] = {0};

    //Genera le coordinate della prima coppia
    for(x=0;x<5;x++){
        for(y=0;y<4;y++){
            state[y][x] = 0x01UL;
            for(yOff=y+1;yOff<5;yOff++){
                state[yOff][x] = 0x01UL;
                //Genera le coordinate della seconda coppia (qui si ha anche z)
		for(z1=0;z1<64;z1++){
                    for(x1=x;x1<5;x1++){			    
		        //Se le due coppie si trovano nelle stessa colonna, continua alla prossima iterazione
			if(x==x1 && z1==0)
			    continue;
                        for(y1=0;y1<4;y1++){
                            state[y1][x1] ^= 0x01UL<<z1;
                            for(y1Off=y1+1;y1Off<5;y1Off++){
                                state[y1Off][x1] ^= 0x01UL << z1;
                                memcpy(tmpState, state, 200);
                                ForwardPropagateNRoundTrail(tmpState, 3, 0, &weight);
                                weights[weight]++;
				//Se il peso del trail è inferiore alla soglia stabilita, stampa il cammino
                                if(weight < WEIGHT_THRESHOLD){
                                    printf(ANSI_COLOR_RED"%s\n"ANSI_COLOR_RESET,sep);
                                    memcpy(tmpState, state, 200);
                                    VerboseForwardPropagateNRoundTrail(tmpState, 3, 0, &weight);
                                    printf("Peso totale: %d\n", weight);
                                    printf(ANSI_COLOR_RED"%s\n"ANSI_COLOR_RESET,sep);
                                }
                                count++;
                                state[y1Off][x1] ^= 0x01UL << z1;
                            }
                            state[y1][x1] ^= 0x01UL<<z1;
                        }
                    }
                }
                state[yOff][x] = 0;
            }
            state[y][x] = 0;
        }
    }

    printf("Totale stati esaminati: %d\n\n", count);
    for(x=0;x<128;x++)
        if(weights[x]>0)
            printf("Numero di 3-RoundTrails di peso %d: %d\n",x,weights[x]);
    
    int i,tot=0;
    for(i=61;i<75;i++)
	tot+=weights[i];
    printf("61-74: %d\n",tot);
    tot=0;
    for(;i<100;i++)
	tot+=weights[i];
    printf("75-99: %d\n",tot);
    tot=0;
    for(;i<105;i++)
	tot+=weights[i];
    printf("100-104: %d\n",tot);
    
    
    return 0;
}
