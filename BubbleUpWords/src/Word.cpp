#include "Word.h"

namespace Word {

    std::vector<wchar_t*> Word::s_all_available_words = std::vector<wchar_t*>();
    int Word::s_all_words_size = 0;

    bool Word::Init()
    {
        s_all_words_size = 0;
        std::ifstream file("Data/dict.txt");
        char buffer[100];
        size_t len = 0;
        size_t out_size = 0;

        if (!file.is_open())
        {
            std::cout << "Could not find file to load words\n";
            return false;
        }
        else 
        {
            while (file.getline(buffer, 100))
            {
                len = strlen(buffer) + 1;
                wchar_t* word = new wchar_t[len];
                mbstowcs_s(&out_size, word, len, buffer, len - 1);
                s_all_available_words.push_back(word);
                s_all_words_size++;
            }

            std::cout << "Done\n";
            return true;
        }
    }

    Word::Word()
    {
        srand(std::time(NULL));
        int rand_word_pos = rand() % s_all_words_size - 1;
        const wchar_t* new_word = s_all_available_words[rand_word_pos];
        wchar_t* new_word_buffer = new wchar_t[100];
        m_word_length = 0;
        while (new_word[m_word_length] != '\0') {
            new_word_buffer[m_word_length] = towupper(new_word[m_word_length]);
            m_word_length++;
        }
        new_word_buffer[m_word_length] = '\0';

        m_word = new_word_buffer;
        m_y = BOTTOM_OF_SCREEN;
        m_x = DetermineNewWordXPos();
        m_word_is_destroyed = false;
    }

    int Word::DetermineNewWordXPos()
    {
        int new_x_pos;
        bool new_position_is_invalid = true;

        while (new_position_is_invalid)
        {
            new_position_is_invalid = false;
            new_x_pos = rand() % (EDGE_OF_SCREEN - m_word_length);
            int new_end_pos = new_x_pos + m_word_length;

            // Make sure that the new position is unique
            for (Word* w : all_words)
            {
                // First and  last position of the current word to test
                int w_x_pos = w->x();
                int w_end = w_x_pos + w->get_word_length();

                if ((new_x_pos < w_x_pos && new_end_pos < w_x_pos) ||
                    (new_x_pos > w_end && new_end_pos < w_end))
                {
                    new_position_is_invalid = true;
                    break;
                }
            }
        }

        return new_x_pos;
    }

    Word::~Word()
    {
    }

    const wchar_t* Word::get_word()
    {
        return m_word;
    }

    const int Word::get_word_length()
    {
        return m_word_length;
    }

    int Word::x() 
    {
        return m_x;
    }

    int Word::y()
    {
        return m_y;
    }

    void Word::MoveWordUp()
    {
        if (m_y > 0) {
            m_y--;
        }
        else {
            m_word_is_destroyed = true;
        }
    }
}
