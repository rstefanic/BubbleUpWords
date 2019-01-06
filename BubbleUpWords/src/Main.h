#pragma once

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <experimental/filesystem>

#include "Word.h"

using namespace std::chrono_literals;

#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 30

// Screen Variables
wchar_t* screen;
HANDLE hConsole;
DWORD bytes_written;

// GamePlay Variables
unsigned int cycles = 0;

void ClearScreen(wchar_t* screen);
void StartGame(wchar_t* screen);
void UpdateGame();
void WriteWordsToBuffer(wchar_t* screen);
void Render(wchar_t* screen);
