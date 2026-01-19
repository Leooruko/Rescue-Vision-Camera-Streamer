# Compiler and flags
CC = gcc
CFLAGS = -Iinclude
LDFLAGS = -lcurl

# Source files
SRC = camera_agent/src/main.c camera_agent/src/frame_queue.c camera_agent/src/camera_capture.c camera_agent/src/sender.c

# Output binary
TARGET = camera_streamer

# Build everything in one step
all:
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(TARGET)

# Clean compiled files
clean:
	rm -f $(TARGET)

