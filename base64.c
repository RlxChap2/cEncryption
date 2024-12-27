#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Base64 Encoding table
static const char base64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Function to encode a string to Base64
char* base64_encode(const unsigned char* input, size_t len) {
    size_t output_len = 4 * ((len + 2) / 3); // Calculate output size
    char* output = malloc(output_len + 1);  // Allocate memory for output
    if (!output) return NULL;

    for (size_t i = 0, j = 0; i < len;) {
        unsigned int octet_a = i < len ? input[i++] : 0;
        unsigned int octet_b = i < len ? input[i++] : 0;
        unsigned int octet_c = i < len ? input[i++] : 0;

        unsigned int triple = (octet_a << 16) | (octet_b << 8) | octet_c;

        output[j++] = base64_table[(triple >> 18) & 0x3F];
        output[j++] = base64_table[(triple >> 12) & 0x3F];
        output[j++] = i > len + 1 ? '=' : base64_table[(triple >> 6) & 0x3F];
        output[j++] = i > len ? '=' : base64_table[triple & 0x3F];
    }

    output[output_len] = '\0'; // Null-terminate the string
    return output;
}

// Function to decode a Base64 string
unsigned char* base64_decode(const char* input, size_t* out_len) {
    if (!input) return NULL;

    size_t len = strlen(input);
    if (len % 4 != 0) return NULL; // Invalid Base64 string length

    *out_len = len / 4 * 3;
    if (input[len - 1] == '=') (*out_len)--;
    if (input[len - 2] == '=') (*out_len)--;

    unsigned char* output = malloc(*out_len);
    if (!output) return NULL;

    for (size_t i = 0, j = 0; i < len;) {
        unsigned int sextet_a = input[i] == '=' ? 0 & i++ : strchr(base64_table, input[i++]) - base64_table;
        unsigned int sextet_b = input[i] == '=' ? 0 & i++ : strchr(base64_table, input[i++]) - base64_table;
        unsigned int sextet_c = input[i] == '=' ? 0 & i++ : strchr(base64_table, input[i++]) - base64_table;
        unsigned int sextet_d = input[i] == '=' ? 0 & i++ : strchr(base64_table, input[i++]) - base64_table;

        unsigned int triple = (sextet_a << 18) | (sextet_b << 12) | (sextet_c << 6) | sextet_d;

        if (j < *out_len) output[j++] = (triple >> 16) & 0xFF;
        if (j < *out_len) output[j++] = (triple >> 8) & 0xFF;
        if (j < *out_len) output[j++] = triple & 0xFF;
    }

    return output;
}

// Example usage
int main() {
    const char* text = "Hello, Base64!";
    size_t encoded_len;

    // Encode
    char* encoded = base64_encode((const unsigned char*)text, strlen(text));
    printf("Encoded: %s\n", encoded);

    // Decode
    size_t decoded_len;
    unsigned char* decoded = base64_decode(encoded, &decoded_len);
    printf("Decoded: %.*s\n", (int)decoded_len, decoded);

    // Cleanup
    free(encoded);
    free(decoded);

    return 0;
}
