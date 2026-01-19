#include "../include/camera_capture.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <string.h>

int open_camera(const char *device) {
    int fd = open(device, O_RDWR);
    if(fd < 0) {
        perror("Failed to open camera device");
    }
    return fd;
}

int configure_camera(int fd, int width, int height) {
    struct v4l2_format fmt = {0};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = width;
    fmt.fmt.pix.height = height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if(ioctl(fd, VIDIOC_S_FMT, &fmt) < 0) {
        perror("Failed to set camera format");
        return -1;
    }

    printf("Camera configured: %dx%d, YUYV\n", width, height);
    return 0;
}

int init_mmap(int fd, CameraBuffer **buffers, int *num_buffers) {
    struct v4l2_requestbuffers req = {0};
    req.count = 4;  // number of buffers
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if(ioctl(fd, VIDIOC_REQBUFS, &req) < 0) {
        perror("Failed to request buffers");
        return -1;
    }

    *num_buffers = req.count;
    *buffers = (CameraBuffer*)calloc(req.count, sizeof(CameraBuffer));
    if(!*buffers) return -1;

    for(int i = 0; i < req.count; i++) {
        struct v4l2_buffer buf = {0};
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if(ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) {
            perror("Failed to query buffer");
            return -1;
        }

        (*buffers)[i].length = buf.length;
        (*buffers)[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
        if((*buffers)[i].start == MAP_FAILED) {
            perror("Failed to mmap buffer");
            return -1;
        }

        // Queue the buffer
        if(ioctl(fd, VIDIOC_QBUF, &buf) < 0) {
            perror("Failed to queue buffer");
            return -1;
        }
    }

    // Start streaming
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(fd, VIDIOC_STREAMON, &type) < 0) {
        perror("Failed to start streaming");
        return -1;
    }

    return 0;
}

int capture_frame(int fd, CameraBuffer *buffers, unsigned char *dest, size_t buffer_size) {
    struct v4l2_buffer buf = {0};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    // Dequeue a filled buffer
    if(ioctl(fd, VIDIOC_DQBUF, &buf) < 0) {
        perror("Failed to dequeue buffer");
        return -1;
    }

    // Copy data to destination
    if(buf.bytesused > buffer_size) buf.bytesused = buffer_size;
    memcpy(dest, buffers[buf.index].start, buf.bytesused);

    // Re-queue buffer
    if(ioctl(fd, VIDIOC_QBUF, &buf) < 0) {
        perror("Failed to requeue buffer");
        return -1;
    }

    return buf.bytesused;
}

void close_camera(int fd, CameraBuffer *buffers, int num_buffers) {
    // Stop streaming
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(fd, VIDIOC_STREAMOFF, &type);

    // Unmap buffers
    for(int i = 0; i < num_buffers; i++) {
        munmap(buffers[i].start, buffers[i].length);
    }

    free(buffers);
    close(fd);
}
