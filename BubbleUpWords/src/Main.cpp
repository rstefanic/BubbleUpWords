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
    globals g;

    MainMenu(screen, &g);
    StartGame(screen, &g);
    return 0;
}

void MainMenu(wchar_t* screen, globals* g)
{
    ClearScreen(screen);

    GAME_TYPE gt = MISS_WORDS;
    bool start = false;
    bool key_space = false;
    bool key_space_old = false;

    static const wchar_t* game_types[] = { L" Missed Words ", L"Beat The Clock" };

    while (!start)
    {
        wsprintf(&screen[10 * SCREEN_WIDTH + 51],
            L"Bubble Up Words");
        wsprintf(&screen[14 * SCREEN_WIDTH + 51],
            L"   GAME MODE  ");
        wsprintf(&screen[15 * SCREEN_WIDTH + 51],
            gt == MISS_WORDS ? game_types[0] : game_types[1]);
        wsprintf(&screen[20 * SCREEN_WIDTH + 32],
            L"Press 'Enter' to start or 'Space' to switch the game mode");
        WriteConsoleOutputCharacter(hConsole, screen,
            SCREEN_WIDTH * SCREEN_HEIGHT, { 0, 0 }, &bytes_written);

        key_space = GetAsyncKeyState(VK_SPACE) != 0;

        if (key_space && !key_space_old)
        {
            gt = gt == MISS_WORDS ? BEAT_THE_CLOCK : MISS_WORDS;
        }

        key_space_old = key_space;

        if (GetAsyncKeyState(VK_RETURN)) {
            start = true;
        }
    }

    g->game_type = gt;
}

void SetupGame(wchar_t* screen, globals* g)
{
    ClearScreen(screen);
    
    // Setup game variables
    g->cycles = 0;
    g->missed_words = 0;
    g->correct_words = 0;
    g->start_time = std::chrono::system_clock::now();
    ResetPlayerInputBuffer(g);
}

void ClearScreen(wchar_t* screen)
{
    for (register int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        screen[i] = L' ';
    }
}

void StartGame(wchar_t* screen, globals* g)
{
    // Start out with at least one word
    Word::AddNewWord();
    SetupGame(screen, g);

    while (DetermineConditions(g))
    {
        std::chrono::system_clock::time_point tp1 = std::chrono::system_clock::now();
        while ((std::chrono::system_clock::now() - tp1)
            < std::chrono::milliseconds(20))
        {
            bool key_was_pressed = false;
            wchar_t key_pressed = Event::GetKeyPresses(&key_was_pressed);

            if (key_was_pressed)
            {
                if (key_pressed == VK_RETURN)
                {
                    bool word_removed = CheckIfEntryIsCorrect(g);
                    ResetPlayerInputBuffer(g);
                }
                else if (key_pressed == VK_BACK)
                {
                    // Only decrease the buffer size and delete a character
                    // if it is not at the beginning of the buffer
                    if (g->input_buffer_size > 0) {
                        g->input_buffer_size--;
                        g->player_input_buffer[g->input_buffer_size] = L'\0';
                    }
                }
                else if (key_pressed == L'\0')
                {
                    continue;
                }
                else
                {
                    g->player_input_buffer[g->input_buffer_size] = key_pressed;
                    g->player_input_buffer[g->input_buffer_size + 1] = L'\0';
                    g->input_buffer_size++;
                }
            }
        }

        UpdateGame(g);
        Render(screen, g);
    }

    GameOver(screen);
    Render(screen, g);
}

void DrawUI(wchar_t* screen, globals* g)
{
    for (register int i = 0; i < SCREEN_WIDTH; i++)
    {
        screen[((SCREEN_HEIGHT - 3) * SCREEN_WIDTH) + i] = L'=';
    }

    if (g->game_type == MISS_WORDS)
    {
        wsprintf(&screen[((SCREEN_HEIGHT - 2) * SCREEN_WIDTH) + 33],
            L"Misses: %d/20       Words Correct: %d          Word: %s",
            g->missed_words, g->correct_words, g->player_input_buffer);
    }
    else
    {
        wchar_t* time_buffer = new wchar_t[5];
        size_t out_size;

        // Get the current time
        auto elapsed_time = std::chrono::system_clock::now() - g->start_time;
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(elapsed_time);

        // convert duration to wstring
        auto duration_s = std::to_string(duration.count());
        std::wstring duration_w = std::wstring(duration_s.begin(), duration_s.end());

        wsprintf(&screen[((SCREEN_HEIGHT - 2) * SCREEN_WIDTH) + 33],
            L"Time: %s s       Words Correct: %d          Word: %s",
            duration_w.c_str(), g->correct_words, g->player_input_buffer);
    }
}

void UpdateGame(globals* g)
{
    int word_pos = 0;
    for (Word::Word* w : Word::all_words)
    {
        if (w->word_is_destroyed()) 
        {
            w->~Word();
            std::vector<Word::Word*>::iterator i = Word::all_words.begin() + word_pos;
            Word::all_words.erase(i);
            g->missed_words++;
        }
        else 
        {
            word_pos++;
            if ((g->cycles % 25) == 0)
                w->MoveWordUp();
        }
    }

    if (g->cycles <= 50) 
    {
        g->cycles++;
    }
    else {
        g->cycles = 0;
        Word::AddNewWord();
    }
}

void Render(wchar_t* screen, globals* g)
{
    ClearScreen(screen);
    WriteWordsToBuffer(screen);
    DrawUI(screen, g);
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

bool CheckIfEntryIsCorrect(globals* g)
{
    int word_pos = 0;
    g->input_buffer_size = 0;

    for (Word::Word* w : Word::all_words)
    {
        if (std::wcscmp(w->get_word(), g->player_input_buffer) == 0)
        {
            w->~Word();
            std::vector<Word::Word*>::iterator i = Word::all_words.begin() + word_pos;
            Word::all_words.erase(i);
            g->correct_words++;

            return true;
        }
        else {
            word_pos++;
        }
    }

    return false;
}

void ResetPlayerInputBuffer(globals* g)
{
    for (register int i = 0; i < INPUT_BUFFER_SIZE; i++)
    {
        g->player_input_buffer[0] = L' ';
    }

    g->input_buffer_size = 0;

    g->player_input_buffer[g->input_buffer_size] = '\0';
}

void GameOver(wchar_t* screen)
{
    wsprintf(&screen[15 + (SCREEN_WIDTH * 40)],
        L"*** GAME OVER ***");
}

bool DetermineConditions(globals* g)
{
    if (g->game_type == MISS_WORDS)
    {
        return (g->missed_words < 20);
    }
    else
    {
        return ((std::chrono::system_clock::now() - g->start_time) < std::chrono::minutes(2));
    }
}
