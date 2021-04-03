#include <stdio.h>
#include <gmp.h>
#include <iostream>

int main() {


    #define LIMIT 1500000 /*size of integers array*/
    #define PRIMES 100000 /*size of primes array*/

    int i,j,numbers[LIMIT];
    int primes[PRIMES];

    FILE *myFile;
    myFile = fopen("mersenneinds.txt", "r");

    /*read file into array*/
    int mArray[64];

    for (int i = 0; i < 31; i++) {
        fscanf(myFile, "%d", &mArray[i]);
    }

    /*fill the array with natural numbers*/
    for (i=0;i<LIMIT;i++){
        numbers[i]=i+2;
    }

    /*sieve the non-primes*/
    for (i=0;i<LIMIT;i++){
        if (numbers[i]!=-1){
            for (j=2*numbers[i]-2;j<LIMIT;j+=numbers[i])
                numbers[j]=-1;
        }
    }

    /*transfer the primes to their own array*/
    j = 0;
    for (i=0;i<LIMIT && j<PRIMES;i++)
        if (numbers[i]!=-1)
            primes[j++] = numbers[i];

    mpz_t M, L, T;
    mpz_init(M);
    mpz_init(L);
    mpz_init(T);

    for (int j = 0; j < 31; j++) {
        mpz_ui_pow_ui(M,2,mArray[j]);
        mpz_sub_ui(M,M,1);
        mpz_init_set_d(L, 4);
        for (int k = 1; k < mArray[j] - 1; k++) {
            mpz_mul(T,L,L);
            mpz_sub_ui(T,T,2);
            mpz_mod(L,T,M);
        }
        if (mpz_cmp_d(L,0)==0) {
            std::cout << "p = " << mArray[j] << ": ";
            gmp_printf("%Zd\n",M);
            std::cout << std::flush;
        }
    }

    return 0;
}
