#include "Event.h"

Event::Event()
{
}

Event::~Event()
{
}

bool Event::CheckForKeyPress(wchar_t c)
{
    return GetAsyncKeyState((unsigned short)c) & 0x8000;
}

wchar_t Event::GetKeyPresses(bool* key_was_pressed)
{
    const wchar_t return_key = (wchar_t)VK_RETURN;

    if (CheckForKeyPress(return_key)) 
    {
        *key_was_pressed = true;
        return (return_key);
    }

    const wchar_t* all_keys = 
        L"ABCDEFGHIJKLMNOPQRSTUVQXYZabcdefghijklmnopqrstuvqxyz'";

    for (register int i = 0; i < 53; i++)
    {
        if (CheckForKeyPress(*(all_keys + i)))
        {
            *key_was_pressed = true;
            return *(all_keys + i);
        }
    }

    *key_was_pressed = false;
    return L'\0';
}
