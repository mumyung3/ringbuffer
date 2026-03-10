#pragma once

#include <stdio.h>
#include <Windows.h>
#include "Console.h"
#include "AnswerEngine.h"
#include "ringbuffer.h"
#include <ctime>
#include <io.h>
#include <fcntl.h>
#include <conio.h>


#define MAX_CHUNK 20 

using namespace std;

extern char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

extern AnswerEngine answerObject;

void Buffer_Flip(void);

void Buffer_Clear(void);

void Sprite_Draw(int iX, int iY, char chSprite);

void DrawLayout();

void UpdateDisplay(const wchar_t* op, int size, const char* data,
    const char* rbResult, const char* gtResult,
    int total, int pass, int fail, bool isFail);