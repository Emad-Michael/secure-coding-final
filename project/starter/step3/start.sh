#!/bin/bash
# Compile the generator
gcc generate_hashed_users.c hash_utils.c -o generate_hashed_users -lcrypto
# Run it to create the hashed_users.txt file
./generate_hashed_users
# Start the login app loop
while true; do
  /app/login
done