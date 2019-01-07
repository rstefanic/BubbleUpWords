#pragma once

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <thread>
#include <Windows.h>

#include "Word.h"
#include "Event.h"

// Using Namespaces
using namespace std::chrono_literals;

// Constants
#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 30
#define INPUT_BUFFER_SIZE 100
#define MAX_AMOUNT_TO_MISS 5

// Screen Variables
wchar_t* screen;
HANDLE hConsole;
DWORD bytes_written;

// GamePlay Variables
wchar_t player_input_buffer[INPUT_BUFFER_SIZE];
unsigned int input_buffer_size;
unsigned int cycles;
unsigned int missed_words;
unsigned int correct_words;

// Forward Declarations
void SetupGame(wchar_t* screen);
void ClearScreen(wchar_t* screen);
void StartGame(wchar_t* screen);
void DrawUI(wchar_t* screen);
void UpdateGame();
void WriteWordsToBuffer(wchar_t* screen);
void Render(wchar_t* screen);
bool CheckIfEntryIsCorrect();
