#include "AnswerEngine.h"
#include <string.h>

void AnswerEngine::GT_Push(const char* data, int size) {
	memcpy(&gt[gtTail], data, size);
	gtTail += size;
}
void AnswerEngine::GT_Pop(char* dest, int size) {
	memcpy(dest, &gt[gtHead], size);
	gtHead += size;
}
int AnswerEngine::GT_Size() { return gtTail - gtHead; }

AnswerEngine::AnswerEngine() : gtHead(0), gtTail(0) {
	memset(gt, 0,GT_SIZE);
}

