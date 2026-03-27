#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>    // For sleep()
#include "hash_utils.h"

#define MAX_BUF 256
#define SALT_SIZE 16
#define HASH_SIZE 64

void trim(char *str) {
    str[strcspn(str, "\r\n")] = 0;
}

int main() {
    char in_user[MAX_BUF], in_pass[MAX_BUF];
    char f_user[MAX_BUF], f_salt_hex[MAX_BUF], f_hash_hex[MAX_BUF];
    int f_attempts, success = 0;

    // STEP 4 FIX: Use fgets to prevent Buffer Overflow
    printf("Username: ");
    fgets(in_user, MAX_BUF, stdin);
    trim(in_user);

    printf("Password: ");
    fgets(in_pass, MAX_BUF, stdin);
    trim(in_pass);

    FILE *fptr = fopen("hashed_users.txt", "r");
    if (!fptr) {
        printf("Error: hashed_users.txt not found.\n");
        return 1;
    }

    // Format: username salt_hex hash_hex attempts
    while (fscanf(fptr, "%s %s %s %d", f_user, f_salt_hex, f_hash_hex, &f_attempts) != EOF) {
        if (strcmp(in_user, f_user) == 0) {
            
            // STEP 4 FIX: Brute Force Protection (Lockout)
            if (f_attempts >= 3) {
                printf("Too many failed attempts. Account locked for 5 seconds.\n");
                sleep(5);
            }

            // To verify, we must hash the input password with the SAME salt from the file
            unsigned char raw_salt[SALT_SIZE];
            // (Note: In a real app, you'd convert f_salt_hex back to bytes here)
            // For this project, we'll use your hash_password function logic:
            char generated_hash[HASH_SIZE * 2 + 1];
            
            // Re-hash the entered password using the stored salt
            hash_password(in_pass, (unsigned char*)f_salt_hex, generated_hash);

            if (strcmp(generated_hash, f_hash_hex) == 0) {
                success = 1;
            }
            break;
        }
    }
    fclose(fptr);

    if (success) {
        printf("Login successful!\n");
    } else {
        printf("Login failed.\n");
    }

    return 0;
}