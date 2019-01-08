#pragma once

#include <iostream>
#include <iterator>
#include <fstream>
#include <wctype.h>
#include <Windows.h>
#include <vector>

#define BOTTOM_OF_SCREEN 27
#define EDGE_OF_SCREEN 119

namespace Word {

    class Word
    {
    public:
        static bool Init();
        const wchar_t* get_word();
        const int get_word_length();
        int x();
        int y();
        void MoveWordUp();

        Word();
        ~Word();
    private:
        static std::vector<wchar_t*> s_all_available_words;
        static int s_all_words_size;

        const wchar_t* m_word;

        int m_word_length;
        int m_x;
        int m_y;

        int DetermineNewWordXPos();
    };

    static std::vector<Word*> all_words;

    static void AddNewWord()
    {
        Word* new_word = new Word();
        all_words.push_back(new_word);
    }
}