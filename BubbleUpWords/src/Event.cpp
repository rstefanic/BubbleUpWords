#include "Event.h"

#define KEY_PRESS_BOOL_DECL(key) bool Event::kp_##key = false;\
                                 bool Event::kp_old_##key = false;

#define SET_KP_EQUAL_TO_OLD(key) kp_old_##key = kp_##key

#define RESET_KP(key) kp_##key = false;\
                      kp_old_##key = false;

#define KEY_CHECK(c) if (kp_old_##c == true)\
                     {\
                         kp_old_##c = false;\
                         return false;\
                     }\
                     else\
                     {\
                         kp_##c = true;\
                         return true;\
                     }\
                     break;

KEY_PRESS_BOOL_DECL(A)
KEY_PRESS_BOOL_DECL(B)
KEY_PRESS_BOOL_DECL(C)
KEY_PRESS_BOOL_DECL(D)
KEY_PRESS_BOOL_DECL(E)
KEY_PRESS_BOOL_DECL(F)
KEY_PRESS_BOOL_DECL(G)
KEY_PRESS_BOOL_DECL(H)
KEY_PRESS_BOOL_DECL(I)
KEY_PRESS_BOOL_DECL(J)
KEY_PRESS_BOOL_DECL(K)
KEY_PRESS_BOOL_DECL(L)
KEY_PRESS_BOOL_DECL(M)
KEY_PRESS_BOOL_DECL(N)
KEY_PRESS_BOOL_DECL(O)
KEY_PRESS_BOOL_DECL(P)
KEY_PRESS_BOOL_DECL(Q)
KEY_PRESS_BOOL_DECL(R)
KEY_PRESS_BOOL_DECL(S)
KEY_PRESS_BOOL_DECL(T)
KEY_PRESS_BOOL_DECL(U)
KEY_PRESS_BOOL_DECL(V)
KEY_PRESS_BOOL_DECL(W)
KEY_PRESS_BOOL_DECL(X)
KEY_PRESS_BOOL_DECL(Y)
KEY_PRESS_BOOL_DECL(Z)
KEY_PRESS_BOOL_DECL(APOS)

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
    // ---------------------------------------------
    // Handle Special Characters first 
    // e.g. ReturnKey, Apostrophe, Backspace, etc.
    // ---------------------------------------------

    if (CheckForKeyPress(VK_RETURN)) 
    {
        *key_was_pressed = true;
        return (VK_RETURN);
    }

    // VK_OEM_7 == SingleQuote on US Keyboard Layout
    if (CheckForKeyPress(VK_OEM_7))
    {
        *key_was_pressed = true;
        if (kp_old_APOS == true)
        {
            kp_old_APOS = false;
            SetAllKPToOld();
            return L'\0';
        }
        else
        {
            kp_APOS = true;
            SetAllKPToOld();
            return L'\'';
        }
    }

    if (CheckForKeyPress(VK_BACK))
    {
        *key_was_pressed = true;
        return VK_BACK;
    }

    // All Regular A-Z Keys
    const wchar_t* all_keys = 
        L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (register int i = 0; i < 26; i++)
    {
        if (CheckForKeyPress(*(all_keys + i)))
        {
            wchar_t c = *(all_keys + i);
            *key_was_pressed = true;
            if (SetProperKP(c))
            {
                SetAllKPToOld();
                return c;
            }
            else 
            {
                SetAllKPToOld();
                return L'\0';
            }
        }
    }

    ResetAllKP();
    *key_was_pressed = false;
    return L'\0';
}

bool Event::SetProperKP(wchar_t c)
{
    switch (c)
    {
    case L'A':
        KEY_CHECK(A)
    case L'B':
        KEY_CHECK(B)
    case L'C':
        KEY_CHECK(C)
    case L'D':
        KEY_CHECK(D)
    case L'E':
        KEY_CHECK(E)
    case L'F':
        KEY_CHECK(F)
    case L'G':
        KEY_CHECK(G)
    case L'H':
        KEY_CHECK(H)
    case L'I':
        KEY_CHECK(I)
    case L'J':
        KEY_CHECK(J)
    case L'K':
        KEY_CHECK(K)
    case L'L':
        KEY_CHECK(L)
    case L'M':
        KEY_CHECK(M)
    case L'N':
        KEY_CHECK(N)
    case L'O':
        KEY_CHECK(O)
    case L'P':
        KEY_CHECK(P)
    case L'Q':
        KEY_CHECK(Q)
    case L'R':
        KEY_CHECK(R)
    case L'S':
        KEY_CHECK(S)
    case L'T':
        KEY_CHECK(T)
    case L'U':
        KEY_CHECK(U)
    case L'V':
        KEY_CHECK(V)
    case L'W':
        KEY_CHECK(W)
    case L'X':
        KEY_CHECK(X)
    case L'Y':
        KEY_CHECK(Y)
    case L'Z':
        KEY_CHECK(Z)
    default:
        return false;
    }
}

void Event::SetAllKPToOld()
{
    SET_KP_EQUAL_TO_OLD(A);
    SET_KP_EQUAL_TO_OLD(B);
    SET_KP_EQUAL_TO_OLD(C);
    SET_KP_EQUAL_TO_OLD(D);
    SET_KP_EQUAL_TO_OLD(E);
    SET_KP_EQUAL_TO_OLD(F);
    SET_KP_EQUAL_TO_OLD(G);
    SET_KP_EQUAL_TO_OLD(H);
    SET_KP_EQUAL_TO_OLD(I);
    SET_KP_EQUAL_TO_OLD(J);
    SET_KP_EQUAL_TO_OLD(K);
    SET_KP_EQUAL_TO_OLD(L);
    SET_KP_EQUAL_TO_OLD(M);
    SET_KP_EQUAL_TO_OLD(N);
    SET_KP_EQUAL_TO_OLD(O);
    SET_KP_EQUAL_TO_OLD(P);
    SET_KP_EQUAL_TO_OLD(Q);
    SET_KP_EQUAL_TO_OLD(R);
    SET_KP_EQUAL_TO_OLD(S);
    SET_KP_EQUAL_TO_OLD(T);
    SET_KP_EQUAL_TO_OLD(U);
    SET_KP_EQUAL_TO_OLD(V);
    SET_KP_EQUAL_TO_OLD(W);
    SET_KP_EQUAL_TO_OLD(X);
    SET_KP_EQUAL_TO_OLD(Y);
    SET_KP_EQUAL_TO_OLD(Z);
    SET_KP_EQUAL_TO_OLD(APOS);
}

void Event::ResetAllKP()
{
    RESET_KP(A)
    RESET_KP(B)
    RESET_KP(C)
    RESET_KP(D)
    RESET_KP(E)
    RESET_KP(F)
    RESET_KP(G)
    RESET_KP(H)
    RESET_KP(I)
    RESET_KP(J)
    RESET_KP(K)
    RESET_KP(L)
    RESET_KP(M)
    RESET_KP(N)
    RESET_KP(O)
    RESET_KP(P)
    RESET_KP(Q)
    RESET_KP(R)
    RESET_KP(S)
    RESET_KP(T)
    RESET_KP(U)
    RESET_KP(V)
    RESET_KP(W)
    RESET_KP(X)
    RESET_KP(Y)
    RESET_KP(Z)
    RESET_KP(APOS)
}
