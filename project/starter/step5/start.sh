#!/bin/bash

# Compile the password hasher and utility
gcc generate_hashed_users.c hash_utils.c -o generate_hashed_users -lcrypto

# Generate the hashed_users.txt from users.txt (Removes plaintext passwords)
./generate_hashed_users

# Start the login application loop
while true; do
  /app/login
done