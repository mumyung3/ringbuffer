#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <Windows.h>
using namespace std;
#define BUFSIZE 1024
class RingBuffer {
public:

	RingBuffer() : front(0), rear(0), datasize(0) { memset(buffer, 0, BUFSIZE); }

	int front;
	int rear;
	char buffer[BUFSIZE];
	int datasize;
	bool IsEmpty() {

		if (front == rear) return true;
		return false;
	}
	bool IsFull() {

		if (front == (rear + 1) % BUFSIZE) return true;
		return false;
	}

	bool Enqueue(const char* pData, int iSize) {
		// 인큐 크기 > 남은 공간
		if (iSize > BUFSIZE - 1 - GetDataSize()) {
			printf("데이터를 넣을공간이 부족합니다.\n");
			return false;
		}
		int firstChunk = min(iSize, BUFSIZE - rear);
		memcpy(&buffer[rear], pData, firstChunk);
		memcpy(&buffer[0], pData + firstChunk, iSize - firstChunk);
		rear = (rear + iSize) % BUFSIZE;
		return true;
	}
	bool Dequeue(char* pDest, int iSize) {
		// 디큐 크기 > 저장량
		if (iSize > GetDataSize()) {
			printf("뽑아낼 데이터 부족합니다.\n");
			return false;
		}
		int firstChunk = min(iSize, BUFSIZE - front);
		memcpy(pDest, &buffer[front], firstChunk);
		memcpy(pDest + firstChunk, &buffer[0], iSize - firstChunk);
		front = (front + iSize) % BUFSIZE;
		return true;
	}

	int GetDataSize() {
		datasize = (rear - front + BUFSIZE) % BUFSIZE;
		return datasize;
	}
};

int main() {
	RingBuffer ring;
	char dummy[2 * BUFSIZE] = "TESTDATA";
	for (int i = 0; i < 2 * BUFSIZE; i++)
		dummy[i] = "TESTDATA"[i % 8];

	char outBuf[2 * BUFSIZE] = {};
	int enqSize = 512;
	int deqSize = 512;

	while (1) {
		if (GetAsyncKeyState('A') & 0x8000) { deqSize += 1; Sleep(100); }
		if (GetAsyncKeyState('B') & 0x8000) { deqSize -= 1; Sleep(100); }
		if (GetAsyncKeyState('C') & 0x8000) { enqSize += 1; Sleep(100); }
		if (GetAsyncKeyState('D') & 0x8000) { enqSize -= 1; Sleep(100); }
		if (GetAsyncKeyState('E') & 0x8000) { deqSize = BUFSIZE; Sleep(100); }
		if (GetAsyncKeyState('F') & 0x8000) { enqSize = BUFSIZE; Sleep(100); }

		bool enqueueResult = ring.Enqueue(dummy, enqSize);
		if (!enqueueResult) {
			__debugbreak();
			break;
		}
		bool dequeueResult = ring.Dequeue(outBuf, deqSize);
		if (!dequeueResult) {
			__debugbreak();
			break;
		}

		printf("enqSize=%d enq=%s | deqSize=%d deq=%s | 저장량=%d\n",
			enqSize, enqueueResult ? "OK" : "FAIL",
			deqSize, dequeueResult ? "OK" : "FAIL",
			ring.GetDataSize());

		Sleep(100);
	}
	return 0;
}