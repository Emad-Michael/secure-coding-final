FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y gcc make libssl-dev

# Set the working directory
WORKDIR /app

# Explicitly copy files to ensure they exist in the build context
COPY login.c .
COPY hash_utils.c .
COPY hash_utils.h .
COPY generate_hashed_users.c .
COPY users.txt .
COPY start.sh .

# Fix permissions for the startup script
RUN chmod +x start.sh

# Compile the hashing tool and the login application
RUN gcc -o generate_hashed_users generate_hashed_users.c hash_utils.c -lssl -lcrypto
RUN gcc -o login login.c hash_utils.c -lssl -lcrypto

# Run the startup script
CMD ["./start.sh"]