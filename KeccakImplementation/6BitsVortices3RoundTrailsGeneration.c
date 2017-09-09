#include "KeccakTools.c"

//Per la stessa questione spiegata in Keccak-f.c, la tabella rotOffset va letta con gli indici y e x invertiti
//Non so cosa intenda il pdf quando dice che ci sono 600 Trail da 6Bit, ma a me ne trova solo 8 se considero che ogni trail ne ha 6 equivalenti * 64 equivalenti (=3072)
int getDepth(int x0, int y0, int x2, int y2, int x4, int y4){
    int depth;
    depth = rotOffset[y0][x0] + rotOffset[y2][x2] + rotOffset[y4][x4];
    depth -= (rotOffset[y2][x0] + rotOffset[y4][x2] + rotOffset[y0][x4]);
    return depth;
}

int main(int argc, char const *argv[]) {

    int x0,y0,x2,y2,x4,y4,cont=0;
    int z0,z2,z4;
    int weight;
    uint64_t state[5][5];

    printf("Generazione di tutti i 3RoundTrails basati su Kernel-Vortex da 6Bit.\n");
    printf("Non so cosa intenda il pdf quando dice che ci sono 600 Trail da 6Bit, ma a me ne trova solo 8 se considero che ogni trail ne ha 6 equivalenti(considerando i punti nella stessa sequenza ma partendo ogni volta da uno dopo) * 64 equivalenti(poichè posso shiftare l'intero stato sull asse z per la proprietà solita).\nIn totale sarebbero 3072\n\n");
    printf("Premere un tasto per iniziare...\n\n");
    getchar();

    for(x0=0;x0<3;x0++)
        for(y0=0;y0<5;y0++)
            for(x2=x0+1;x2<4;x2++){
                for(y2=0;y2<5;y2++){
                    if(y2==y0)
                        continue;
                    for(x4=x2+1;x4<5;x4++){
                        for(y4=0;y4<5;y4++){
                            if(y4 == y2)
                                continue;
                            if(getDepth(x0,y0,x2,y2,x4,y4) % 64 == 0){
                                cont++;
                                printf("Vortex #%d\nL'equazione ha soluzione per le coppie (%d,%d), (%d,%d) e (%d,%d)\n",cont,x0,y0,x2,y2,x4,y4);
                                z0 = 0;
                                z2 = z0 + rotOffset[y2][x0] - rotOffset[y2][x2];
                                z2 = mod(z2,64);
                                z4 = z0 + rotOffset[y0][x0] - rotOffset[y0][x4];
                                z4 = mod(z4,64);
                                printf("z0=0;z2=%d;z4=%d\n",z2,z4);
                                memset(state,0,200);
                                state[y0][x0] = 0x01UL << z0;
                                state[y2][x0] = 0x01UL << z0;
                                state[y2][x2] = 0x01UL << z2;
                                state[y4][x2] = 0x01UL << z2;
                                state[y4][x4] = 0x01UL << z4;
                                state[y0][x4] = 0x01UL << z4;
                                VerbosePropagate3RoundTrail(state,0,&weight);
                                printf("Peso totale:%d\n", weight);
                            }
                        }
                    }
                }
            }
    return 0;
}
