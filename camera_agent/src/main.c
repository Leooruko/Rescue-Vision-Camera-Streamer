#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "../include/camera_capture.h"
#include "../include/frame_queue.h"
#include "../include/sender.h"

#define WIDTH 640
#define HEIGHT 480
#define BACKEND_URL "http://your-backend-url/receive-frame"

static volatile int running = 1;

// Handle Ctrl+C gracefully
void handle_sigint(int sig) {
    running = 0;
}

int main() {
    signal(SIGINT, handle_sigint);

    // Open camera
    int fd = open_camera("/dev/video0");
    if(fd < 0) return 1;

    // Configure resolution & format
    if(configure_camera(fd, WIDTH, HEIGHT) < 0) {
        close(fd);
        return 1;
    }

    // Initialize memory-mapped buffers
    CameraBuffer *buffers;
    int num_buffers;
    if(init_mmap(fd, &buffers, &num_buffers) < 0) {
        close(fd);
        return 1;
    }

    // Initialize frame queue
    FrameQueue *queue = init_queue(10);  // max 10 frames in queue

    size_t buffer_size = WIDTH * HEIGHT * 2;
    unsigned char buffer[buffer_size];

    printf("Starting capture loop. Press Ctrl+C to stop.\n");

    while(running) {
        int bytes = capture_frame(fd, buffers, buffer, buffer_size);
        if(bytes < 0) {
            fprintf(stderr, "Failed to capture frame\n");
            continue;
        }

        // Enqueue captured frame
        if(enqueue(queue, buffer, bytes) < 0) {
            fprintf(stderr, "Queue full, dropping frame\n");
            continue;
        }

        // Dequeue and send frame
        Frame *frame = dequeue(queue);
        if(frame) {
            if(send_frame(frame->data, frame->size, BACKEND_URL) != 0) {
                fprintf(stderr, "Failed to send frame\n");
            }
            free_frame(frame);
        }
    }

    printf("Stopping capture...\n");

    // Cleanup
    close_camera(fd, buffers, num_buffers);
    free_queue(queue);

    return 0;
}
