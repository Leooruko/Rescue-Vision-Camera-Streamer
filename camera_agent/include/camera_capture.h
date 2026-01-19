#ifndef CAMERA_CAPTURE_H
#define CAMERA_CAPTURE_H

#include <stddef.h>

typedef struct {
    void *start;
    size_t length;
} CameraBuffer;

int open_camera(const char *device);
int configure_camera(int fd, int width, int height);
int init_mmap(int fd, CameraBuffer **buffers, int *num_buffers);
int capture_frame(int fd, CameraBuffer *buffers, unsigned char *dest, size_t buffer_size);
void close_camera(int fd, CameraBuffer *buffers, int num_buffers);

#endif
