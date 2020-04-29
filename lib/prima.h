#ifndef PRIMA_H
#define PRIMA_H

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

void square_and_multiply(mpz_t r, mpz_t a, mpz_t exp, mpz_t modulo);
int fermat(mpz_t n, int k);

int decomposition(mpz_t n, mpz_t result, mpz_t cst1);
int miller_rabin(mpz_t n , int repetition);

#endif
