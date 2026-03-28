#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "hash_utils.h"

#define MAX_BUF 256
#define SALT_SIZE 16
#define HASH_SIZE 64

void trim(char *str) {
    str[strcspn(str, "\r\n")] = 0;
}

void update_counter(const char *target_user, int new_attempts) {
    FILE *fptr = fopen("hashed_users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    char f_user[MAX_BUF], f_salt[MAX_BUF], f_hash[MAX_BUF];
    int f_attempts;

    if (!fptr || !temp) return;

    while (fscanf(fptr, "%s %s %s %d", f_user, f_salt, f_hash, &f_attempts) != EOF) {
        if (strcmp(f_user, target_user) == 0) {
            fprintf(temp, "%s %s %s %d\n", f_user, f_salt, f_hash, new_attempts);
        } else {
            fprintf(temp, "%s %s %s %d\n", f_user, f_salt, f_hash, f_attempts);
        }
    }
    fclose(fptr);
    fclose(temp);
    rename("temp.txt", "hashed_users.txt");
}

int main() {
    char in_user[MAX_BUF], in_pass[MAX_BUF];
    char f_user[MAX_BUF], f_salt_hex[MAX_BUF], f_hash_hex[MAX_BUF];
    int f_attempts, success = 0;

    // FIX: Use fgets to prevent Buffer Overflow
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

    while (fscanf(fptr, "%s %s %s %d", f_user, f_salt_hex, f_hash_hex, &f_attempts) != EOF) {
        if (strcmp(in_user, f_user) == 0) {
            // FIX: Lockout Threshold (Step 4)
            if (f_attempts >= 3) {
                printf("Too many failed attempts. Account locked for 5 seconds.\n");
                sleep(5);
            }

            char generated_hash[HASH_SIZE * 2 + 1];
            hash_password(in_pass, (unsigned char*)f_salt_hex, generated_hash);

            if (strcmp(generated_hash, f_hash_hex) == 0) {
                success = 1;
                update_counter(f_user, 0); 
            } else {
                update_counter(f_user, f_attempts + 1); 
            }
            break;
        }
    }
    fclose(fptr);

    if (success) printf("Login successful!\n");
    else printf("Login failed.\n");

    return 0;
}