#include "../include/frame_queue.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

FrameQueue* init_queue() {
    FrameQueue *queue = (FrameQueue*)malloc(sizeof(FrameQueue));
    queue->front = queue->rear = NULL;
    queue->length = 0;
    return queue;
}

int enqueue(FrameQueue *queue, unsigned char *data, size_t size) {
    Frame *frame = (Frame*)malloc(sizeof(Frame));
    frame->data = (unsigned char*)malloc(size);
    memcpy(frame->data, data, size);
    frame->size = size;
    frame->next = NULL;

    if(!queue->rear) {
        queue->front = queue->rear = frame;
    } else {
        queue->rear->next = frame;
        queue->rear = frame;
    }
    queue->length++;
}

Frame* dequeue(FrameQueue *queue) {
    if(!queue->front) return NULL;

    Frame *frame = queue->front;
    queue->front = queue->front->next;
    if(!queue->front) queue->rear = NULL;
    queue->length--;
    return frame;
}

void free_frame(Frame *frame) {
    if(frame) {
        free(frame->data);
        free(frame);
    }
}

void free_queue(FrameQueue *queue) {
    Frame *frame;
    while((frame = dequeue(queue))) {
        free_frame(frame);
    }
    free(queue);
}
