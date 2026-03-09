#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <Windows.h>
#include "Console.h"
using namespace std;
#define BUFSIZE 100
#define GT_SIZE 100000

char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

// T 는 F,R 같은 위치.
// 데이터는 항상 F부터 시작 R위치는 데이터를 넣어야할 장소.

void Buffer_Flip(void);

void Buffer_Clear(void);

void Sprite_Draw(int iX, int iY, char chSprite);

// ===================== 정답 배열 =====================
char gt[GT_SIZE];
int gtHead = 0, gtTail = 0;

void GT_Push(const char* data, int size) {
	memcpy(&gt[gtTail], data, size);
	gtTail += size;
}
void GT_Pop(char* dest, int size) {
	memcpy(dest, &gt[gtHead], size);
	gtHead += size;
}
int GT_Size() { return gtTail - gtHead; }

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
	while (1) {


		Buffer_Clear();

		// 버퍼 셀 그리기

		Buffer_Flip();

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

void DrawLayout() {
	system("cls");
	cs_MoveCursor(0, 0);  wprintf(L"╔══════════════════════════════════════════════╗");
	cs_MoveCursor(0, 1);  wprintf(L"║         RingBuffer Validation Test           ║");
	cs_MoveCursor(0, 2);  wprintf(L"╠══════════════════════════════════════════════╣");
	cs_MoveCursor(0, 3);  wprintf(L"║ 연산:                                        ║");
	cs_MoveCursor(0, 4);  wprintf(L"║ 크기:                                        ║");
	cs_MoveCursor(0, 5);  wprintf(L"║ 데이터:                                      ║");
	cs_MoveCursor(0, 6);  wprintf(L"╠══════════════════════════════════════════════╣");
	cs_MoveCursor(0, 7);  wprintf(L"║ 링버퍼 결과:                                 ║");
	cs_MoveCursor(0, 8);  wprintf(L"║ 정답   결과:                                 ║");
	cs_MoveCursor(0, 9);  wprintf(L"╠══════════════════════════════════════════════╣");
	cs_MoveCursor(0, 10); wprintf(L"║ 테스트 횟수:                                 ║");
	cs_MoveCursor(0, 11); wprintf(L"║ PASS:        FAIL:                           ║");
	cs_MoveCursor(0, 12); wprintf(L"╠══════════════════════════════════════════════╣");
	cs_MoveCursor(0, 13); wprintf(L"║ 상태:                                        ║");
	cs_MoveCursor(0, 14); wprintf(L"╚══════════════════════════════════════════════╝");
}