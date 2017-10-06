#include "KeccakTools.c"

int getDepth(int x0, int y0, int x2, int y2, int x4, int y4){
    int depth;
    depth = rotOffset[y0][x0] + rotOffset[y2][x2] + rotOffset[y4][x4];
    depth -= (rotOffset[y2][x0] + rotOffset[y4][x2] + rotOffset[y0][x4]);
    return depth;
}

int main(int argc, char const *argv[]) {

    int x0,y0,x2,y2,x4,y4,cont=0;
    int z0,z2,z4;
    int weight,i;
    uint64_t state[5][5];
    int weights[80] = {0};

    printf("Generazione di tutti i 3RoundTrails basati su Kernel-Vortex da 6Bit.\n");
    printf("Premere un tasto per iniziare...\n\n");
    getchar();

    for(y0=0;y0<5;y0++){
     for(x0=0;x0<5;x0++){
      for(y2=0;y2<5;y2++){
          if(y0 == y2) continue;
       for(x2=x0+1;x2<5;x2++){
        for(y4=0;y4<5;y4++){
            if(y0 == y4) continue;
            if(y2 == y4) continue;
         for(x4=x2+1;x4<5;x4++){
	    cont++;
            if(mod(getDepth(x0,y0,x2,y2,x4,y4),64) == 0){
                z0 = 0;
                z2 = z0 + rotOffset[y2][x0] - rotOffset[y2][x2];
                z2 = mod(z2,64);
                z4 = z0 + rotOffset[y0][x0] - rotOffset[y0][x4];
                z4 = mod(z4,64);

                printf("L'equazione ha soluzione per le coppie (y,x) (%d,%d), (%d,%d) e (%d,%d)\n",y0,x0,y2,x2,y4,x4);
                printf("z0=0;z2=%d;z4=%d\n",z2,z4);
                memset(state,0,200);
                state[y0][x0] ^= 0x01UL << z0;
                state[y2][x0] ^= 0x01UL << z0;
                state[y2][x2] ^= 0x01UL << z2;
                state[y4][x2] ^= 0x01UL << z2;
                state[y4][x4] ^= 0x01UL << z4;
                state[y0][x4] ^= 0x01UL << z4;
                VerboseForwardPropagateNRoundTrail(state,3,0,&weight);
                weights[weight]++;
            }
         }
        }
       }
      }
     }
    }
    printf("Numero di stati generati: %d\n\n",cont);
    for(i=0;i<80;i++)
        if(weights[i])
            printf(" numero di vortici di peso %d: %d\n",i, weights[i]);
    return 0;
}
