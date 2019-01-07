#pragma once

#include <Windows.h>

static class Event
{
public:
    Event();
    ~Event();
    static wchar_t GetKeyPresses(bool* key_was_pressed);

private:
    static bool CheckForKeyPress(wchar_t c);
};

