#ifndef OBJECT_ARROW_H
#define OBJECT_ARROW_H

typedef struct Object Object;
typedef Object Arrow;

int create_arrow(Arrow* arrow);
void destroy_arrow(Arrow* arrow);

#endif