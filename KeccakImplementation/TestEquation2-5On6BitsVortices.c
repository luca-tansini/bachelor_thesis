#include "Keccak-f.c"

int getDepth(int x0, int y0, int x2, int y2, int x4, int y4){
    int depth;
    depth = rotOffset[x0][y0] + rotOffset[x2][y2] + rotOffset[x4][y4];
    depth -= (rotOffset[x0][y2] + rotOffset[x2][y4] + rotOffset[x4][y0]);
    return depth;
}

int main(int argc, char const *argv[]) {

    int x0,y0,x2,y2,x4,y4,depth,cont=0;
    /*
    for(x0=0;x0<5;x0++)
        for(y0=0;y0<5;y0++)
            for(x2=0;x2<5;x2++){
                if(x2 == x0)
                    continue;
                for(y2=0;y2<5;y2++){
                    if(y2==y0)
                        continue;
                    for(x4=0;x4<5;x4++){
                        if(x4 == x2)
                            continue;
                        for(y4=0;y4<5;y4++){
                            if(y4 == y2)
                                continue;
                            if(getDepth(x0,y0,x2,y2,x4,y4) % 64 == 0){
                                cont++;
                                printf("Vortex #%d\nL'equazione ha soluzione per le coppie (%d,%d), (%d,%d) e (%d,%d)\n",cont,x0,y0,x2,y2,x4,y4);
                            }
                        }
                    }
                }
            }

    */
    printf("depth:%d\n", getDepth(0,0,3,4,2,3));
    return 0;
}
