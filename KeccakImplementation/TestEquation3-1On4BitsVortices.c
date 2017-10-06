#include "KeccakTools.c"

//Il seguente codice dimostra che non vi sono possibili trail da 4 bit per l=6 -> w=64
int main(int argc, char const *argv[]) {

    int x0,y0,x2,y2,tmp,cont=0;

    for(x0=0;x0<5;x0++)
        for(y0=0;y0<5;y0++)
            for(x2=x0+1;x2<5;x2++){
                for(y2=y0+1;y2<5;y2++){
                    tmp = rotOffset[y0][x0] - rotOffset[y2][x0] + rotOffset[y2][x2] - rotOffset[y0][x2];
                    printf("tmp: %d\n", tmp);
		    cont ++;
                    if(mod(tmp,64) == 0){
                        printf("L'equazione ha soluzione per le coppie (%d,%d) e (%d,%d)\n", x0, y0, x2, y2);
                    }
                }
            }
	    
    printf("Totale stati esaminati: %d", cont);    

    return 0;
}
