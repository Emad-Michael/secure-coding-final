#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "hash_utils.h"

#define MAX_BUF 256

int main() {
    char in_user[MAX_BUF], in_pass[MAX_BUF];
    char f_user[MAX_BUF], f_salt[MAX_BUF], f_hash[MAX_BUF];
    int f_attempts, success = 0;

    printf("Username: ");
    fgets(in_user, MAX_BUF, stdin);
    in_user[strcspn(in_user, "\n")] = 0;

    printf("Password: ");
    fgets(in_pass, MAX_BUF, stdin);
    in_pass[strcspn(in_pass, "\n")] = 0;

    FILE *fptr = fopen("hashed_users.txt", "r");
    if (!fptr) return 1;

    while (fscanf(fptr, "%s %s %s %d", f_user, f_salt, f_hash, &f_attempts) != EOF) {
        if (strcmp(in_user, f_user) == 0) {
            if (f_attempts >= 3) {
                printf("Account locked. Wait 5 seconds.\n");
                sleep(5);
                fclose(fptr);
                return 1;
            }
            char gen_hash[129];
            hash_password(in_pass, (unsigned char*)f_salt, gen_hash);
            if (strcmp(gen_hash, f_hash) == 0) success = 1;
            break;
        }
    }
    fclose(fptr);
    if (success) printf("Login successful!\n");
    else printf("Login failed.\n");
    return 0;
}