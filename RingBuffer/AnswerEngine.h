#define GT_SIZE 100000
// ===================== 정답 배열 =====================

class AnswerEngine {
public:
	char gt[GT_SIZE];
	int gtHead;
	int gtTail;

public :
	void GT_Push(const char* data, int size);
	void GT_Pop(char* dest, int size);
	int GT_Size();

	AnswerEngine();
};
