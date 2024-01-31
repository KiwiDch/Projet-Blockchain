#include "../common/include/crypto.h"
#include <glib.h>
#include <stdio.h>

int main() {
    KeyPair keys;
    printf("gen");
    fflush(stdout);
    generate_KeyPair(&keys, 200);

    int vote = 10;
    mpz_t c,m;
    printf("encrypt\n");
    fflush(stdout);
    encrypt_vote(&keys.public, vote,c);

    gmp_printf("Encrypted message: %Zd\n", c);
    decrypt_vote(&keys.private, c,m);

    gmp_printf("Decrypted message : %Zd\n", m);

    return 0;
}