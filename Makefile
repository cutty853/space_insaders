CC = gcc

PROG_NAME = space_invaders

CFLAGS = -Wall -Wextra
LDFLAGS = -lSDL -lm -lSDL_image -lSDL_gfx -lSDL_ttf

BIN_DIR = bin
OBJ_DIR = obj

SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(BIN_DIR)/$(PROG_NAME)

$(BIN_DIR)/$(PROG_NAME): $(OBJECTS)
	${CC} -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJ_DIR)/*.o
