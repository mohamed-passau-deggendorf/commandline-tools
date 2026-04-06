#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#define PROBA 0.3

int main(int argc, char *argv[]) {
    double p = PROBA;
    
    srand(time(NULL));  
    
    printf("Good luck to find a job !!!\n");
    
redo: ;
    double r = (double)rand() / RAND_MAX;
    
    if (r < p) goto end;
    if (p == 0) goto rubbish;
    
    p = p * PROBA;
    
    printf("\rYou are rejected, the probability to find a job is set to %g ", p);
    fflush(stdout);  
    sleep(1);        
    
    goto redo;
    
end:
    printf("\nCongratulations! You finally got a new job !!!\n");
    return EXIT_SUCCESS;
    
rubbish:
    printf("\nYou will definitively stay unemployed !!!\n");
    return EXIT_FAILURE;
}
