#include "KeccakTools.c"

int getDepth(int x0, int y0, int x2, int y2, int x4, int y4){
	int depth;
	depth = rotOffset[y0][x0] + rotOffset[y2][x2] + rotOffset[y4][x4];
	depth -= (rotOffset[y2][x0] + rotOffset[y4][x2] + rotOffset[y0][x4]);
	return depth;
}

int main(int argc, char const *argv[]) {
	
	int i,cont=0;
	int weights[512] = {0};
	
	printf("Generazione di tutti i 3RoundTrails basati su Kernel-Vortex da 6Bit.\n");
	printf("Premere un tasto per iniziare...\n\n");
	getchar();
	
	#pragma omp parallel
	{
		uint64_t state[5][5];
		uint64_t tmpState[5][5];
		int weight;
		int x0,y0,y0Off,y2Off,y4Off,x2,y2,x4,y4;
		int z2,z4;

		memset(state,0,200);
	
		#pragma omp for
		//scambiati per permettere parallelismo
		for(z2=0;z2<64;z2++)
		for(x0=0;x0<5;x0++)
		for(y0=0;y0<4;y0++)
		for(y0Off=y0+1;y0Off<5;y0Off++)
		for(x2=x0;x2<5;x2++)
		for(y2=0;y2<4;y2++){
			if((y2==y0||y2==y0Off) && x2==x0 && z2==0) continue;
			for(y2Off=y2+1;y2Off<5;y2Off++){
				if((y2Off==y0||y2Off==y0Off) && x2==x0 && z2==0) continue;
				for(z4=0;z4<64;z4++)
				for(x4=x2;x4<5;x4++)
				for(y4=0;y4<4;y4++){
					if((y4==y0||y4==y0Off) && x4==x0 && z4==0) continue;
					if((y4==y2||y4==y2Off) && x4==x2 && z4==z2) continue;
					for(y4Off=y4+1;y4Off<5;y4Off++){
						if((y4Off==y0||y4Off==y0Off) && x4==x0 && z4==0) continue;
						if((y4Off==y2||y4Off==y2Off) && x4==x2 && z4==z2) continue;
						//CORPO DA ESEGUIRE
						state[y0][x0] ^= 0x01UL;
						state[y0Off][x0] ^= 0x01UL;
						state[y2][x2] ^= 0x01UL << z2;
						state[y2Off][x2] ^= 0x01UL << z2;
						state[y4][x4] ^= 0x01UL << z4;
						state[y4Off][x4] ^= 0x01UL << z4;
						//COSE
						memcpy(tmpState,state,200);
						ForwardPropagateNRoundTrail(tmpState,3,0,&weight);
						#pragma omp atomic
						cont++;
						
						if(weight<50){
							printf("************************\n");
							printf("(%d,%d), (%d,%d), (%d,%d), (%d,%d), (%d,%d), (%d,%d)\n",x0,y0,x0,y0Off,x2,y2,x2,y2Off,x4,y4,x4,y4Off);
							memcpy(tmpState,state,200);
							VerboseForwardPropagateNRoundTrail(tmpState,3,0,&weight);
							printf("Peso totale: %d\n",weight);
							printf("************************\n");
						}
						#pragma omp atomic
						weights[weight]++;
						
						//FINE COSE
						state[y0][x0] ^= 0x01UL;
						state[y0Off][x0] ^= 0x01UL;
						state[y2][x2] ^= 0x01UL << z2;
						state[y2Off][x2] ^= 0x01UL << z2;
						state[y4][x4] ^= 0x01UL << z4;
						state[y4Off][x4] ^= 0x01UL << z4;
					}
				}
			}
		}
	}
				
	printf("Stati totali esaminati: %d\n", cont);
	
	for(i=0;i<512;i++)
		if(weights[i])
			printf(" numero di vortici di peso %d: %d\n",i, weights[i]);
		
	return 0;
}
