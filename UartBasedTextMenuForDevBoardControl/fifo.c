#include "fifo.h"

void FIFO_Init(struct FIFO *Fifo){
	Fifo -> tail = 0;
	Fifo -> head = 0;
}

void FIFO_Empty(struct FIFO *Fifo){
	Fifo -> head = Fifo -> tail;
}

int FIFO_IsEmpty(struct FIFO *Fifo){
	return (Fifo -> head == Fifo -> tail);
}

int FIFO_IsEmpty2(struct FIFO *Fifo){
	return (Fifo -> head - 1 == Fifo -> tail);
}

int FIFO_IsFull(struct FIFO *Fifo){
	return ((Fifo->tail) - (Fifo -> head) == 1) ||
			((Fifo->tail == 0) && (Fifo->head==BUFFERSIZE));
}

int FIFO_Put(struct FIFO *Fifo, char Data){

	if (FIFO_IsFull(Fifo)){
		return -1;
	}

	Fifo->buffer[Fifo->head] = Data;
	//Fifo->head = (Fifo->head + 1) & BUFFERSIZE;
	if(Fifo->head > BUFFERSIZE - 1){
		Fifo->head=0;
	} else {
		Fifo->head++;
	}

	return 1;
}

int FIFO_Get(struct FIFO *Fifo, char* Data){
	if ((Fifo -> head != Fifo -> tail)){
		*Data = Fifo->buffer[Fifo->tail];
		//Fifo->tail = (Fifo->tail + 1) & BUFFERSIZE;
		if(Fifo->tail > BUFFERSIZE - 1){
			Fifo->tail=0;
		} else {
			Fifo->tail++;
		}

		return 1;
	} else {
		return -1;
	}
}
