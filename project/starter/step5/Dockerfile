# Use the official GCC image
FROM gcc:latest

# Step 5 Fix: ONLY install what is necessary. 
# We keep net-tools and nmap for scanning/verification as requested.
RUN apt-get update && \
    apt-get install -y build-essential net-tools nmap libssl-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Step 3 Fix: Copy ALL files needed for hashing and login
COPY login.c /app/
COPY hash_utils.c /app/
COPY hash_utils.h /app/
COPY generate_hashed_users.c /app/
COPY users.txt /app/
COPY start.sh /app/

# Make start.sh executable
RUN chmod +x /app/start.sh

# Step 5 Fix: Principle of Least Privilege for network access
# Block all by default, then allow only what's needed (standard practice)
RUN echo "ALL: ALL" > /etc/hosts.deny && \
    echo "sshd: ALL" > /etc/hosts.allow

# Step 5 Fix: Ensure we run the secure start script, NOT xinetd or FTP
CMD ["/bin/bash", "/app/start.sh"]