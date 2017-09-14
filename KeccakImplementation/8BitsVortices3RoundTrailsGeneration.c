#include "KeccakTools.c"

//Per la stessa questione spiegata in Keccak-f.c, la tabella rotOffset va letta con gli indici y e x invertiti
//Non so cosa intenda il pdf quando dice che ci sono 104040 Trail da 8Bit, ma a me ne trova solo 1112 se considero che ogni trail ne ha 8 equivalenti(considerando i punti nella stessa sequenza ma partendo ogni volta da uno dopo) * 64 equivalenti(poichè posso shiftare l'intero stato sull asse z per la proprietà solita). In totale sarebbero 569344.
int getDepth(int x0, int y0, int x2, int y2, int x4, int y4, int x6, int y6){
    int depth;
    depth = rotOffset[y0][x0] + rotOffset[y2][x2] + rotOffset[y4][x4] + rotOffset[y6][x6];
    depth -= (rotOffset[y2][x0] + rotOffset[y4][x2] + rotOffset[y6][x4] + rotOffset[y0][x6]);
    return depth;
}

int main(int argc, char const *argv[]) {

    int x0,y0,x2,y2,x4,y4,x6,y6,cont=0;
    int z0,z2,z4,z6;
    int weight;
    uint64_t state[5][5];
    uint64_t trailState[5][5];

    printf("Generazione di tutti i 3RoundTrails basati su Kernel-Vortex da 8Bit.\n");
    printf("Non so cosa intenda il pdf quando dice che ci sono 104040 Trail da 8Bit, ma a me ne trova solo 1112 se considero che ogni trail ne ha 8 equivalenti(considerando i punti nella stessa sequenza ma partendo ogni volta da uno dopo) * 64 equivalenti(poichè posso shiftare l'intero stato sull asse z per la proprietà solita).\nIn totale sarebbero 569344\n\n");
    printf("Premere un tasto per iniziare...\n\n");
    getchar();

    for(y0=0;y0<5;y0++)
        for(x0=0;x0<5;x0++)
            for(y2=0;y2<5;y2++){
                if(y2==y0)
                    continue;
                for(x2=0;x2<5;x2++){
                    if(x2==x0)
                        continue;
                    for(y4=0;y4<5;y4++){
                        if(y4==y2)
                            continue;
                        for(x4=0;x4<5;x4++){
                            if(x4==x2)
                                continue;
                            for(y6=0;y6<5;y6++){
                                if(y6==y4 || y6==y0)
                                    continue;
                                for(x6=0;x6<5;x6++){
                                    if(x6==x4 || x6==x0)
                                        continue;
                                    if(getDepth(x0,y0,x2,y2,x4,y4,x6,y6) % 64 == 0){
                                        cont++;
                                        z0 = 0;
                                        z2 = z0 + rotOffset[y2][x0] - rotOffset[y2][x2];
                                        z2 = mod(z2,64);
                                        z4 = z2 + rotOffset[y4][x2] - rotOffset[y4][x4];
                                        z4 = mod(z4,64);
                                        z6 = z0 + rotOffset[y0][x0] - rotOffset[y0][x6];
                                        z6 = mod(z6,64);
                                        memset(state,0,200);
                                        state[y0][x0] ^= 0x01UL << z0;
                                        state[y2][x0] ^= 0x01UL << z0;
                                        state[y2][x2] ^= 0x01UL << z2;
                                        state[y4][x2] ^= 0x01UL << z2;
                                        state[y4][x4] ^= 0x01UL << z4;
                                        state[y6][x4] ^= 0x01UL << z4;
                                        state[y6][x6] ^= 0x01UL << z6;
                                        state[y0][x6] ^= 0x01UL << z6;
                                        memcpy(trailState,state,200);
                                        ForwardPropagateNRoundTrail(trailState,3,0,&weight);
                                        if (weight<48){
                                            VerboseForwardPropagateNRoundTrail(state,3,0,&weight);
                                            printf("Peso totale:%d\n\n", weight);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
    printf("Totale 8Bit Vortices trovati: %d\n",cont);
    return 0;
}
