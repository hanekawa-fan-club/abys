CC = g++
CFLAGS = -std=c++20 -I/opt/homebrew/opt/sdl2/include
LDFLAGS = -L/opt/homebrew/opt/sdl2/lib -lSDL2
TARGET = window

$(TARGET): main.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)
