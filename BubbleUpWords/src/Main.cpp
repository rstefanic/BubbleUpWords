#include "Main.h"

int main()
{
    if (!Word::Word::Init())
    {
        return EXIT_FAILURE;
    }

    screen = new wchar_t[SCREEN_WIDTH * SCREEN_HEIGHT];
    bytes_written = 0;
    ClearScreen(screen);
    hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
        0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    SetConsoleActiveScreenBuffer(hConsole);

    StartGame(screen);
    return 0;
}

void ClearScreen(wchar_t* screen)
{
    for (register int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        screen[i] = L' ';
    }
}

void StartGame(wchar_t* screen)
{
    Word::AddNewWord();

    while (1)
    {
        std::this_thread::sleep_for(200ms);
        UpdateGame();
        Render(screen);
    }
}

void UpdateGame()
{
    for (Word::Word* w : Word::all_words)
    {
        w->MoveWordUp();
    }

    if (cycles < 5) 
    {
        cycles++;
    }
    else {
        cycles = 0;
        Word::AddNewWord();
    }
}

void Render(wchar_t* screen)
{
    ClearScreen(screen);
    WriteWordsToBuffer(screen);
    WriteConsoleOutputCharacter(hConsole, screen,
        SCREEN_WIDTH * SCREEN_HEIGHT, { 0, 0 }, &bytes_written);
}

void WriteWordsToBuffer(wchar_t* screen)
{
    for (Word::Word* w : Word::all_words)
    {
        wsprintf(&screen[w->x() + (SCREEN_WIDTH * w->y())],
            w->get_word());
    }
}

