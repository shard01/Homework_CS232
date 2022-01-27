/* File: primes.c */
/* Author: Britton Wolfe */
/* Date: July 16, 2009 */
/* This program outputs all the primes in the range given */
/* by the command line arguments */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, const char** argv){

    int lowerBound, upperBound;

    if(argc != 3){
        fprintf(stderr, "USAGE: %s lowerBound upperBound\n", argv[0]);
        return -1;
    }
  
    lowerBound = atoi(argv[1]);
    upperBound = atoi(argv[2]);
  
    if(lowerBound < 1 || upperBound < 1){
        fprintf(stderr, "ERROR: both the lowerBound (%d) and the upperBound (%d)"
	          " must be positive.\n", lowerBound, upperBound);
        return -2;
    }
        int i, flag;
     while (lowerBound < upperBound) {
      flag = 0;

      // ignore numbers less than 2
      if (lowerBound<= 1) {
         ++lowerBound;
         continue;
      }

      // if lowowerBound is a non-prime number, flag will be 1
      for (i = 2; i <= lowerBound / 2; ++i) {

         if (lowerBound % i == 0) {
            flag = 1;
            break;
         }
      }

      if (flag == 0)
         printf ("%d" "\n",lowerBound);
      // to check prime for the next number
      // increase lowerBound by 1
      ++lowerBound;
   }
      
        
    return 0;
}
