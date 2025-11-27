CC = gcc
CFLAGS = -O2 -Iinclude -Wall -Wextra
SRCS = src/main.c src/game.c src/board.c src/leaderboard.c src/utils.c
OBJS = $(SRCS:.c=.o)
TARGET = sudoku4

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)
