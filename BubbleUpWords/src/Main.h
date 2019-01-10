#pragma once

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <Windows.h>

#include "Word.h"
#include "Event.h"

// Constants
#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 30
#define INPUT_BUFFER_SIZE 100
#define MAX_AMOUNT_TO_MISS 5

// Using Namespaces
using namespace std::chrono_literals;

enum GAME_TYPE 
{
    MISS_WORDS,
    BEAT_THE_CLOCK
};

struct globals
{
    GAME_TYPE game_type;
    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point wpm_timer;
    wchar_t player_input_buffer[INPUT_BUFFER_SIZE];
    unsigned int old_word_count;
    unsigned int wpm;
    unsigned int input_buffer_size;
    unsigned int cycles;
    unsigned int missed_words;
    unsigned int correct_words;
};

// Screen Variables
wchar_t* screen;
HANDLE hConsole;
DWORD bytes_written;

// Forward Declarations
void MainMenu(wchar_t* screen, globals* g);
void SetupGame(wchar_t* screen, globals* g);
void ClearScreen(wchar_t* screen);
void StartGame(wchar_t* screen, globals* g);
void DrawUI(wchar_t* screen, globals* g);
void UpdateGame(globals* g);
void WriteWordsToBuffer(wchar_t* screen);
void Render(wchar_t* screen, globals* g);
bool CheckIfEntryIsCorrect(globals* g);
void ResetPlayerInputBuffer(globals* g);
void GameOver(wchar_t* screen, globals* g);
bool DetermineConditions(globals* g);
