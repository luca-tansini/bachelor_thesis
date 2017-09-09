#include "Keccak-f.c"

//Il seguente codice dimostra che non vi sono possibili trail da 4 bit per l=6 -> w=64
int main(int argc, char const *argv[]) {

    int x0,y0,x2,y2,tmp;

    for(x0=0;x0<5;x0++)
        for(y0=0;y0<5;y0++)
            for(x2=0;x2<5;x2++){
                if(x2 == x0)
                    continue;
                for(y2=0;y2<5;y2++){
                    if(y2==y0)
                        continue;
                    tmp = rotOffset[x0][y0] - rotOffset[x0][y2] + rotOffset[x2][y2] - rotOffset[x2][y0];
                    printf("tmp: %d\n", tmp);
                    if(tmp % 64 == 0){
                        printf("L'equazione ha soluzione per le coppie (%d,%d) e (%d,%d)\n", x0, y0, x2, y2);
                    }
                }
            }

    return 0;
}
