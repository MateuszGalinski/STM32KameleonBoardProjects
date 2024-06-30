#pragma once

#define BUFFERSIZE 40

typedef struct FIFO{
	char buffer[BUFFERSIZE + 1];
	unsigned int tail;
	unsigned int head;
} FIFO;

void FIFO_Init(struct FIFO *Fifo);
void FIFO_Empty(struct FIFO *Fifo);
int FIFO_IsEmpty(struct FIFO *Fifo);
int FIFO_IsEmpty2(struct FIFO *Fifo);
int FIFO_IsFull(struct FIFO *Fifo);
int FIFO_Put(struct FIFO *Fifo, char Data);
int FIFO_Get(struct FIFO *Fifo, char* Data);
