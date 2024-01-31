#ifndef __CRYPTO__
#define __CRYPTO__
#include <gmp.h>
#include <string.h>

typedef struct {
    mpz_t n;
    mpz_t g;
} PublicKey;

typedef struct {
    mpz_t lambda;
    mpz_t mu;
    mpz_t n;
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
