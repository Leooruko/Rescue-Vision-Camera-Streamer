#ifndef SENDER_H
#define SENDER_H

#include <stddef.h>

int send_frame(unsigned char *data, size_t size, const char *url);

#endif
