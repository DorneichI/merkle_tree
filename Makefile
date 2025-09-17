CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I/opt/homebrew/opt/openssl@3/include -I.
LDFLAGS = -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto
TARGET = equal
SRC = merkle_equal.c merkle.c print_utils.c hash_utils.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
