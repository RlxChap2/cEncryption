# Encryption Project: RC4 and Base64 Encoding/Decoding

This document explains the implementation of encryption and encoding techniques: RC4 encryption and Base64 encoding/decoding using the C programming language. The document provides an overview of the protocols, the logic behind them, and the respective code implementations.

---

## **1. RC4 Encryption/Decryption**

### **Protocol Overview**

RC4 is a symmetric stream cipher that generates a pseudorandom key stream. The key stream is XORed with the plaintext to produce ciphertext. The same process is used to decrypt the ciphertext back into plaintext.

### **Steps in RC4 Protocol**

1. **Key-Scheduling Algorithm (KSA):** Initializes a state array `S` and scrambles it based on the key.
2. **Pseudo-Random Generation Algorithm (PRGA):** Produces the keystream by continuously swapping elements in the state array.
3. **Encryption/Decryption:** XORs the generated keystream with the plaintext or ciphertext.

### **Code Implementation**

Exported code from the **[rc4.c](./rc4.c)** file:

```c
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
```

### **Key Points**

- Symmetric encryption: The same key is used for encryption and decryption.
- Initialization ensures the keystream is unique for the provided key.
- Suitable for educational purposes; avoid using RC4 for sensitive data due to vulnerabilities.

---

## **2. Base64 Encoding/Decoding**

### **Protocol Overview**

Base64 is an encoding scheme used to represent binary data in an ASCII string format. It is commonly used for encoding data for transfer over text-based protocols.

### **Steps in Base64 Protocol**

1. Divide the input data into 6-bit groups.
2. Map each 6-bit group to a Base64 character using the Base64 alphabet.
3. Add padding (`=`) if the input data's length is not a multiple of 3.

### **Code Implementation**

Exported code from the **[base64.c](./base64.c)** file:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Function to encode data in Base64
char *base64_encode(const unsigned char *data, size_t input_length) {
    size_t output_length = 4 * ((input_length + 2) / 3);
    char *encoded_data = malloc(output_length + 1);
    if (encoded_data == NULL) return NULL;

    int i, j;
    for (i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? data[i++] : 0;
        uint32_t octet_b = i < input_length ? data[i++] : 0;
        uint32_t octet_c = i < input_length ? data[i++] : 0;

        uint32_t triple = (octet_a << 16) | (octet_b << 8) | octet_c;

        encoded_data[j++] = base64_chars[(triple >> 18) & 0x3F];
        encoded_data[j++] = base64_chars[(triple >> 12) & 0x3F];
        encoded_data[j++] = i > input_length + 1 ? '=' : base64_chars[(triple >> 6) & 0x3F];
        encoded_data[j++] = i > input_length ? '=' : base64_chars[triple & 0x3F];
    }

    encoded_data[output_length] = '\0';
    return encoded_data;
}

// Function to decode Base64 data
unsigned char *base64_decode(const char *data, size_t *output_length) {
    size_t input_length = strlen(data);
    if (input_length % 4 != 0) return NULL;

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = malloc(*output_length);
    if (decoded_data == NULL) return NULL;

    int i, j;
    for (i = 0, j = 0; i < input_length;) {
        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : strchr(base64_chars, data[i++]) - base64_chars;
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : strchr(base64_chars, data[i++]) - base64_chars;
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : strchr(base64_chars, data[i++]) - base64_chars;
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : strchr(base64_chars, data[i++]) - base64_chars;

        uint32_t triple = (sextet_a << 18) | (sextet_b << 12) | (sextet_c << 6) | sextet_d;

        if (j < *output_length) decoded_data[j++] = (triple >> 16) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = triple & 0xFF;
    }

    return decoded_data;
}

int main() {
    unsigned char input[] = "Hello, Base64!";
    char *encoded = base64_encode(input, strlen((char *)input));
    printf("Encoded: %s\n", encoded);

    size_t decoded_length;
    unsigned char *decoded = base64_decode(encoded, &decoded_length);
    printf("Decoded: %.*s\n", (int)decoded_length, decoded);

    free(encoded);
    free(decoded);

    return 0;
}
```

### **Key Points**

- Base64 is not encryption; it’s an encoding technique.
- Encoded data can be easily decoded, so it’s not secure for sensitive information.
- Used in data serialization and transfer protocols.

---

## **Important Notes**

1. **RC4 Limitations:**
   - Vulnerable to cryptographic attacks. Avoid for secure applications.
2. **Base64 Misconception:**
   - Not a secure method for protecting data; use encryption for sensitive data.
3. **Memory Management:**
   - Ensure proper allocation and deallocation to avoid memory leaks.
4. **Testing:**
   - Test with varied input sizes to validate robustness.

---
