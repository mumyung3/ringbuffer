#include "main.h"
char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH] = {};

AnswerEngine answerObject{};
RingBuffer rb{};
int oldRbRear = 0;
int oldAnswerObjectRear = 0;

void UpdateDisplay(const wchar_t* op, int size, const char* data,
    const char* rbResult, const char* gtResult,
    int total, int pass, int fail, bool isFail)
{
    // 연산
    cs_MoveCursor(10, 3); wprintf(L"%-10s", op);

    // 크기
    cs_MoveCursor(10, 4); wprintf(L"%-5d", size);

    // 데이터 (hex, 최대 20바이트)
    cs_MoveCursor(10, 5);
    int showLen = min(size, 20);
    for (int i = 0; i < showLen; i++) wprintf(L"%02X ", (unsigned char)data[i]);
    if (size > 20) wprintf(L"...");
    wprintf(L"          ");

    // 결과 비교 (Dequeue일 때만) 인큐일때 추카
    cs_MoveCursor(14, 7);
    if (rbResult) {
        for (int i = 0; i < min(size, 20); i++) wprintf(L"%02X ", (unsigned char)rbResult[i]);
        if (size > 20) wprintf(L"...");
        wprintf(L"     ");
    }
    else {
        for (int i = 0; i < min(size, 20); i++) wprintf(L"%02X ", (unsigned char)rb.buffer[(oldRbRear + i )% BUFSIZE]);
        if (size > 20) wprintf(L"...");
        wprintf(L"     ");
    }
    cs_MoveCursor(14, 8);
    if (gtResult) {
        for (int i = 0; i < min(size, 20); i++) wprintf(L"%02X ", (unsigned char)gtResult[i]);
        if (size > 20) wprintf(L"...");
        wprintf(L"     ");
    }
    else {
        for (int i = 0; i < min(size, 20); i++) wprintf(L"%02X ", (unsigned char)answerObject.gt[oldAnswerObjectRear + i]);
        if (size > 20) wprintf(L"...");
        wprintf(L"     ");
    }

    // 통계
    cs_MoveCursor(15, 10); wprintf(L"%-8d", total);
    cs_MoveCursor(8, 11); wprintf(L"%-6d", pass);
    cs_MoveCursor(21, 11); wprintf(L"%-6d", fail);

    // 상태
    cs_MoveCursor(9, 13);
    if (isFail) wprintf(L"★ FAIL - 불일치 발생! 테스트 중단          ");
    else        wprintf(L"OK                                         ");
}

// ===================== 메인 =====================
int main() {
    _setmode(_fileno(stdout), _O_U16TEXT);

    cs_Initial();
    srand((unsigned)time(nullptr));

    int total = 0, pass = 0, fail = 0;

    char enqData[MAX_CHUNK];
    char rbBuf[MAX_CHUNK];
    char gtBuf[MAX_CHUNK];

    while (true) {
        DrawLayout();

        int op = rand() % 2;       // 0=Enqueue, 1=Dequeue
        int size = rand() % MAX_CHUNK + 1;

        if (op == 0) { // Enqueue
            if (size > BUFSIZE - 1 - rb.GetDataSize()) continue;

            // 정답배열 공간 부족
            if (answerObject.gtTail + size > GT_SIZE) continue;

            for (int i = 0; i < size; i++)
                enqData[i] = (char)(rand() % 256);

            oldRbRear = rb.rear;
            rb.Enqueue(enqData, size);
            oldAnswerObjectRear = answerObject.gtTail;
            answerObject.GT_Push(enqData, size);

            int firstChunk = min(size, BUFSIZE - oldRbRear);

            bool isFail =!(
                (memcmp(rb.buffer + oldRbRear, answerObject.gt + oldAnswerObjectRear, firstChunk) == 0) &&
                (memcmp(rb.buffer, answerObject.gt + oldAnswerObjectRear + firstChunk, size - firstChunk) == 0));

            if (isFail) {
                fail++; 
            }
            else { pass++;  }

            total++;

            UpdateDisplay(L"Enqueue", size, enqData, nullptr, nullptr, total, pass, fail, isFail);
            
            if (isFail) {
                cs_MoveCursor(0, 16);
                wprintf(L"아무 키나 누르면 종료...");
                _getwch();
                return 1;
            }


        }
        else { // Dequeue
            if (size > rb.GetDataSize()) continue;
            if (size > answerObject.GT_Size()) {

                cs_MoveCursor(0, 16);
                wprintf(L"정답 버퍼 공간 부족 ...");
                _getwch();
                return 1;
            }

            rb.Dequeue(rbBuf, size);
            answerObject.GT_Pop(gtBuf, size);
            total++;

            bool isFail = (memcmp(rbBuf, gtBuf, size) != 0);
            if (isFail) fail++; else pass++;

            UpdateDisplay(L"Dequeue", size, gtBuf, rbBuf, gtBuf, total, pass, fail, isFail);

            if (isFail) {
                cs_MoveCursor(0, 16);
                wprintf(L"아무 키나 누르면 종료...");
                _getwch();
                return 1;
            }
        }

        //Sleep(3000); // 너무 빠르면 화면 못봄, 원하면 제거

    }
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