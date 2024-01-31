#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "../common/include/crypto.h"
#include <time.h>

void generateRandomPrimes(mpz_t p, mpz_t q, unsigned int bitLength) {
    gmp_randstate_t state;
    gmp_randinit_default(state);

    // Utilisez un seed différent à chaque exécution pour plus d'aléatoire
    unsigned long seed = time(NULL);
    gmp_randseed_ui(state, seed);

    // Génère des nombres premiers aléatoires de la longueur spécifiée
    mpz_urandomb(p, state, bitLength);
    mpz_nextprime(p, p);

    mpz_urandomb(q, state, bitLength);
    mpz_nextprime(q, q);

    gmp_randclear(state);
}
void generate_KeyPair(KeyPair* key, unsigned int bit_length) {

    mpz_inits(key->public.n,key->private.n, key->private.lambda, key->public.g, key->private.mu, NULL);
    generate_keys(key->public.n, key->private.lambda, key->public.g, key->private.mu, bit_length);

    memcpy(key->private.n,key->public.n, sizeof(mpz_t));
}


void encrypt_vote(PublicKey* key, int vote, mpz_t c) {
    mpz_t m; 
    mpz_inits(m,c, NULL);

    mpz_set_ui(m, vote);
    encrypt(c, m, key->n, key->g);
}

void decrypt_vote(PrivateKey* key, mpz_t c, mpz_t m){
    mpz_inits(m, NULL);
    decrypt(m, c, key->lambda, key->mu, key->n);
}

// Key generation
void generate_keys(mpz_t n, mpz_t lambda, mpz_t g, mpz_t mu, unsigned int bit_length)
{
    mpz_t x, p, q, n_square;
    mpz_inits(x, p, q, n, lambda, n_square, NULL);

    // Generate two large prime numbers p and q
    //mpz_set_str(p, "61", 10);
    //mpz_set_str(q, "53", 10);
    generateRandomPrimes(p, q, bit_length);

    gmp_printf("p = %Zd\n", p);
    gmp_printf("q = %Zd\n", q);
    // Compute n = p * q
    mpz_mul(n, p, q);

    // Compute lambda = lcm(p-1, q-1)
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_lcm(lambda, p, q);

    // Compute n^2
    mpz_mul(n_square, n, n);
    // Choose g as n + 1
    mpz_set(g, n);
    mpz_add_ui(g, g, 1);

    // Compute mu = (L(g^lambda mod n^2))^-1 mod n
    mpz_powm(x, g, lambda, n_square);
    mpz_sub_ui(x, x, 1);
    mpz_div(x, x, n);
    mpz_invert(mu, x, n);
}

// Encryption
void encrypt(mpz_t c, mpz_t m, mpz_t n, mpz_t g)
{
    mpz_t r, n_square;
    mpz_inits(r, n_square, NULL);

    // Generate random number r
    mpz_set_ui(r, 17);

    // Compute n^2
    mpz_mul(n_square, n, n);

    // Compute c = g^m * r^n mod n^2
    mpz_powm(c, g, m, n_square);
    mpz_powm(r, r, n, n_square);
    mpz_mul(c, c, r);
    mpz_mod(c, c, n_square);
}

// Decryption
void decrypt(mpz_t m, mpz_t c, mpz_t lambda, mpz_t mu, mpz_t n)
{
    mpz_t n_square, temp;
    mpz_inits(n_square, temp, NULL);

    // Compute n^2
    mpz_mul(n_square, n, n);

    // Compute m = L(c^lambda mod n^2) * mu mod n
    mpz_powm(temp, c, lambda, n_square);
    mpz_sub_ui(temp, temp, 1);
    mpz_div(temp, temp, n);
    mpz_mul(m, temp, mu);
    mpz_mod(m, m, n);
}
