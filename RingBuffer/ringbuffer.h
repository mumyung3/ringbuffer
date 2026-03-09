#pragma once

#include <algorithm>
#include <Windows.h>

#define BUFSIZE 100

class RingBuffer {
public:

	RingBuffer();

	int front;
	int rear;
	char buffer[BUFSIZE];
	int datasize;
	bool IsEmpty();
	bool IsFull();
	bool Enqueue(const char* pData, int iSize);
	bool Dequeue(char* pDest, int iSize);

	int GetDataSize();
};
