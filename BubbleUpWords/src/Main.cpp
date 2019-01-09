#include "Main.h"

int main()
{
    if (!Word::Word::Init())
    {
        return EXIT_FAILURE;
    }

    screen = new wchar_t[SCREEN_WIDTH * SCREEN_HEIGHT];
    bytes_written = 0;
    hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
        0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    SetConsoleActiveScreenBuffer(hConsole);

    StartGame(screen);
    return 0;
}

void SetupGame(wchar_t* screen)
{
    ClearScreen(screen);
    
    // Setup game variables
    cycles = 0;
    missed_words = 0;
    correct_words = 0;

    ResetPlayerInputBuffer();
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
    // Start out with at least one word
    Word::AddNewWord();
    SetupGame(screen);

    while (1)
    {
        std::chrono::system_clock::time_point tp1 = std::chrono::system_clock::now();
        while ((std::chrono::system_clock::now() - tp1)
            < std::chrono::milliseconds(20))
        {
            bool key_was_pressed = false;
            wchar_t key_pressed = Event::GetKeyPresses(&key_was_pressed);

            if (key_was_pressed)
            {
                if (key_pressed == (wchar_t)VK_RETURN)
                {
                    bool word_removed = CheckIfEntryIsCorrect();
                    ResetPlayerInputBuffer();
                }
                else if (key_pressed == L'\0')
                {
                    continue;
                }
                else
                {
                    player_input_buffer[input_buffer_size] = key_pressed;
                    player_input_buffer[input_buffer_size + 1] = L'\0';
                    input_buffer_size++;
                }
            }
        }

        UpdateGame();
        Render(screen);
    }
}

void DrawUI(wchar_t* screen)
{
    for (register int i = 0; i < SCREEN_WIDTH; i++)
    {
        screen[((SCREEN_HEIGHT - 3) * SCREEN_WIDTH) + i] = L'=';
    }

    wsprintf(&screen[((SCREEN_HEIGHT - 2) * SCREEN_WIDTH) + 33],
        L"Misses: %d          Words Correct: %d          Word: %s",
        missed_words, correct_words, player_input_buffer);
}

void UpdateGame()
{
    int word_pos = 0;
    for (Word::Word* w : Word::all_words)
    {
        if (w->word_is_destroyed()) 
        {
            w->~Word();
            std::vector<Word::Word*>::iterator i = Word::all_words.begin() + word_pos;
            Word::all_words.erase(i);
            missed_words++;
        }
        else 
        {
            word_pos++;
            if ((cycles % 25) == 0)
                w->MoveWordUp();
        }
    }

    if (cycles <= 50) 
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
    DrawUI(screen);
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

bool CheckIfEntryIsCorrect()
{
    int word_pos = 0;
    input_buffer_size = 0;

    for (Word::Word* w : Word::all_words)
    {
        if (std::wcscmp(w->get_word(), player_input_buffer) == 0)
        {
            w->~Word();
            std::vector<Word::Word*>::iterator i = Word::all_words.begin() + word_pos;
            Word::all_words.erase(i);
            correct_words++;

            return true;
        }
        else {
            word_pos++;
        }
    }

    return false;
}

void ResetPlayerInputBuffer()
{
    for (register int i = 0; i < INPUT_BUFFER_SIZE; i++)
    {
        player_input_buffer[0] = L' ';
    }

    input_buffer_size = 0;

    player_input_buffer[input_buffer_size] = '\0';
}