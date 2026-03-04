#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <Windows.h>
#include "Console.h"
using namespace std;
#define BUFSIZE 100

char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

// T 는 F,R 같은 위치.
// 데이터는 항상 F부터 시작 R위치는 데이터를 넣어야할 장소.

void Buffer_Flip(void);

void Buffer_Clear(void);

void Sprite_Draw(int iX, int iY, char chSprite);

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
			cs_MoveCursor(0, 40); // 고정 위치
			printf("데이터를 넣을공간이 부족합니다.");
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

			cs_MoveCursor(0, 40); // 고정 위치
			printf("뽑아낼 데이터 부족합니다.");
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

	cs_Initial();
	RingBuffer ring;
	char dummy[2 * BUFSIZE] = "TESTDATA";
	for (int i = 0; i < 2 * BUFSIZE; i++)
		dummy[i] = "TESTDATA"[i % 8];

	char outBuf[2 * BUFSIZE] = {};
	int enqSize = 30;
	int deqSize = 30;

	bool flag = true;
	while (1) {
		// 디큐 빼는거 조절
		if (GetAsyncKeyState('Q') & 0x8000) { deqSize += 1; Sleep(100); }
		if (GetAsyncKeyState('W') & 0x8000) { deqSize -= 1; Sleep(100); }
		// 인큐 넣는거 조절
		if (GetAsyncKeyState('E') & 0x8000) { enqSize += 1; Sleep(100); }
		if (GetAsyncKeyState('R') & 0x8000) { enqSize -= 1; Sleep(100); }

		if (GetAsyncKeyState('T') & 0x8000) { deqSize = BUFSIZE; Sleep(100); }
		if (GetAsyncKeyState('Y') & 0x8000) { enqSize = BUFSIZE; Sleep(100); }

		bool enqueueResult = false;
		bool dequeueResult = false;
		if (flag) {
			enqueueResult = ring.Enqueue(dummy, enqSize);
			if (!enqueueResult) {
				__debugbreak();
				break;
			}
			flag = false;


		}
		else {
			dequeueResult = ring.Dequeue(outBuf, deqSize);
			if (!dequeueResult) {
				__debugbreak();
				break;
			}
			flag = true;

		}


		Buffer_Clear();

		// 버퍼 셀 그리기
		for (int i = 0; i < BUFSIZE; i++) {
			char ch = ring.buffer[i];
			Sprite_Draw(i, 5, ch);
		}
		// idx 표시
		for (int i = 0; i < BUFSIZE; i++) {
			char idx = '*';
			Sprite_Draw(i, 3, idx);
		}

		// 채워진 구간 '-' 표시
		if (ring.front < ring.rear) {
			for (int i = ring.front; i < ring.rear; i++)
				Sprite_Draw(i, 4, '-');
		}
		else if (ring.front > ring.rear) {
			for (int i = ring.front; i < BUFSIZE; i++)
				Sprite_Draw(i, 4, '-');
			for (int i = 0; i < ring.rear; i++)
				Sprite_Draw(i, 4, '-');
		}
		// F, R은 위에 덮어쓰기
		Sprite_Draw(ring.front, 4, 'F');
		Sprite_Draw(ring.rear, 4, 'R');
		if (ring.front == ring.rear) {
			Sprite_Draw(ring.rear, 4, 'T');

		}
		Buffer_Flip();


		if (flag) {
			cs_MoveCursor(0, 20); // 고정 위치
			outBuf[deqSize] = '\0';
			printf("디큐 : %s", outBuf);
			cs_MoveCursor(0, 40); // 고정 위치
			printf("데이터를 뺐습니다. deqSize=%d deq=%s | 버퍼 저장량=%d",
				deqSize, dequeueResult ? "OK" : "FAIL", ring.GetDataSize());

		}
		else {

			cs_MoveCursor(0, 20); // 고정 위치
			printf("인큐 : %.*s", enqSize, dummy);
			cs_MoveCursor(0, 40); // 고정 위치
			printf("데이터를 넣었습니다. enqSize=%d enq=%s | 버퍼 저장량=%d",
				enqSize, enqueueResult ? "OK" : "FAIL", ring.GetDataSize());

		}

		Sleep(1000);
	}
	return 0;
}


void Buffer_Flip(void)
{
	int iCnt;
	for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		cs_MoveCursor(0, iCnt);
		// %s 안전하게 처리
		printf("%s", szScreenBuffer[iCnt]);
	}
}

void Buffer_Clear(void)
{
	int iCnt;
	memset(szScreenBuffer, ' ', dfSCREEN_WIDTH * dfSCREEN_HEIGHT);

	for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = '\0';
	}

}

void Sprite_Draw(int iX, int iY, char chSprite)
{
	if (iX < 0 || iY < 0 || iX >= dfSCREEN_WIDTH - 1 || iY >= dfSCREEN_HEIGHT)
		return;

	szScreenBuffer[iY][iX] = chSprite;
}
