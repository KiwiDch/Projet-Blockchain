#include "../common/include/crypto.h"
#include <glib.h>

int main() {
    KeyPair keys;
    generate_KeyPair(&keys, 200);

    int vote = 1;
    mpz_t c,m;

    encrypt_vote(&keys.public, vote,c);

    gmp_printf("Encrypted message: %Zd\n", c);
    decrypt_vote(&keys.private, c,m);

    gmp_printf("Decrypted message : %Zd\n", m);

    return 0;
}