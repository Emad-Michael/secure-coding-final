#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_utils.h"

#define SALT_SIZE 16
#define HASH_SIZE 64 // Adjust based on your hash_utils.c implementation

int main() {
    FILE *infile = fopen("users.txt", "r");
    FILE *outfile = fopen("hashed_users.txt", "w");

    if (!infile || !outfile) {
        printf("Error opening files. Make sure users.txt exists.\n");
        return 1;
    }

    char username[50], password[50];
    unsigned char salt[SALT_SIZE];
    char salt_hex[SALT_SIZE * 2 + 1];
    char hashed_password[HASH_SIZE * 2 + 1];

    while (fscanf(infile, "%s %s", username, password) != EOF) {
        // 1. Generate the raw salt bytes
        generate_salt(salt, SALT_SIZE);
        
        // 2. Convert raw salt to Hex string so we can store it in a text file
        bytes_to_hex(salt, SALT_SIZE, salt_hex);
        
        // 3. Hash the password using the raw salt
        hash_password(password, salt, hashed_password);

        // 4. Store: username salt(hex) hash(hex) attempts(0)
        fprintf(outfile, "%s %s %s 0\n", username, salt_hex, hashed_password, 0);
    }

    fclose(infile);
    fclose(outfile);
    printf("hashed_users.txt generated successfully.\n");
    return 0;
}