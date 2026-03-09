#include "ringbuffer.h"

RingBuffer::RingBuffer() : front(0), rear(0), datasize(0) { memset(buffer, 0, BUFSIZE); }

bool RingBuffer::IsEmpty() {

	if (front == rear) return true;
	return false;
}
bool RingBuffer::IsFull() {

	if (front == (rear + 1) % BUFSIZE) return true;
	return false;
}

bool RingBuffer::Enqueue(const char* pData, int iSize) {
	// 인큐 크기 > 남은 공간
	if (iSize > BUFSIZE - 1 - GetDataSize()) {
		return false;
	}
	int firstChunk = min(iSize, BUFSIZE - rear);
	memcpy(&buffer[rear], pData, firstChunk);
	memcpy(&buffer[0], pData + firstChunk, iSize - firstChunk);
	rear = (rear + iSize) % BUFSIZE;
	return true;
}
bool RingBuffer::Dequeue(char* pDest, int iSize) {
	// 디큐 크기 > 저장량
	if (iSize > GetDataSize()) {
		return false;
	}
	int firstChunk = min(iSize, BUFSIZE - front);
	memcpy(pDest, &buffer[front], firstChunk);
	memcpy(pDest + firstChunk, &buffer[0], iSize - firstChunk);
	front = (front + iSize) % BUFSIZE;
	return true;
}

int RingBuffer::GetDataSize() {
	datasize = (rear - front + BUFSIZE) % BUFSIZE;
	return datasize;
}