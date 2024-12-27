#include <stdio.h>
#include <string.h>

// Function to initialize the RC4 key-scheduling algorithm (KSA)
void rc4_initialize(unsigned char *key, int key_length, unsigned char *S) {
    int i, j = 0;
    unsigned char temp;

    // Initialize the state array
    for (i = 0; i < 256; i++) {
        S[i] = i;
    }

    // Scramble the state array using the key
    for (i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % key_length]) % 256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }
}

// Function to encrypt or decrypt the data using RC4 (PRGA)
void rc4_encrypt_decrypt(unsigned char *data, int data_length, unsigned char *S) {
    int i = 0, j = 0, k, t;
    unsigned char temp;

    for (k = 0; k < data_length; k++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;

        // Swap S[i] and S[j]
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;

        // Generate the key stream and XOR it with the data
        t = (S[i] + S[j]) % 256;
        data[k] ^= S[t];
    }
}

int main() {
    unsigned char key[] = "secretkey";  // RC4 key
    unsigned char data[] = "Hello, RC4!";  // Data to encrypt
    unsigned char S[256];  // State array
    int data_length = strlen((char *)data);

    printf("Original data: %s\n", data);

    // Initialize the RC4 state
    rc4_initialize(key, strlen((char *)key), S);

    // Encrypt the data
    rc4_encrypt_decrypt(data, data_length, S);
    printf("Encrypted data: ");
    for (int i = 0; i < data_length; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");

    // Reinitialize the RC4 state for decryption
    rc4_initialize(key, strlen((char *)key), S);

    // Decrypt the data
    rc4_encrypt_decrypt(data, data_length, S);
    printf("Decrypted data: %s\n", data);

    return 0;
}
