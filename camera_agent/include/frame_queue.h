#ifndef FRAME_QUEUE_H
#define FRAME_QUEUE_H

#include <stdlib.h>

typedef struct Frame {
    unsigned char *data;
    size_t size;
    struct Frame *next;
} Frame;

typedef struct {
    Frame *front;
    Frame *rear;
    size_t length;
} FrameQueue;

FrameQueue* init_queue();
int enqueue(FrameQueue *queue, unsigned char *data, size_t size);
Frame* dequeue(FrameQueue *queue);
void free_frame(Frame *frame);
void free_queue(FrameQueue *queue);

#endif
