#ifndef __CRYPTO__
#define __CRYPTO__
#include <gmp.h>
#include <string.h>

typedef struct {
    char n[1028];
    char g[1028];
} PublicKey;

typedef struct {
    char lambda[1028];
    char mu[1028];
    char n[1028];
} PrivateKey;

typedef struct {
    PublicKey public;
    PrivateKey private;
} KeyPair;

void generate_KeyPair(KeyPair* key, unsigned int bit_length);
void encrypt_vote(PublicKey* key, int vote, mpz_t c);
void decrypt_vote(PrivateKey* key, mpz_t c, mpz_t m);

void generateRandomPrimes(mpz_t p, mpz_t q, unsigned int bitLength);

void generate_keys(mpz_t n, mpz_t lambda, mpz_t g, mpz_t mu, unsigned int bit_length);
void encrypt(mpz_t c, mpz_t m, mpz_t n, mpz_t g);
void decrypt(mpz_t m, mpz_t c, mpz_t lambda, mpz_t mu, mpz_t n);

#endif
