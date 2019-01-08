#pragma once

#include <Windows.h>

#define KEY_PRESS_BOOLS(key) static bool kp_##key;\
                             static bool kp_old_##key;

class Event
{
public:
    Event();
    ~Event();
    static wchar_t GetKeyPresses(bool* key_was_pressed);
    
private:
    static bool CheckForKeyPress(wchar_t c);
    static bool SetProperKP(wchar_t c);
    static void SetAllKPToOld();
    static void ResetAllKP();

    KEY_PRESS_BOOLS(A)
    KEY_PRESS_BOOLS(B)
    KEY_PRESS_BOOLS(C)
    KEY_PRESS_BOOLS(D)
    KEY_PRESS_BOOLS(E)
    KEY_PRESS_BOOLS(F)
    KEY_PRESS_BOOLS(G)
    KEY_PRESS_BOOLS(H)
    KEY_PRESS_BOOLS(I)
    KEY_PRESS_BOOLS(J)
    KEY_PRESS_BOOLS(K)
    KEY_PRESS_BOOLS(L)
    KEY_PRESS_BOOLS(M)
    KEY_PRESS_BOOLS(N)
    KEY_PRESS_BOOLS(O)
    KEY_PRESS_BOOLS(P)
    KEY_PRESS_BOOLS(Q)
    KEY_PRESS_BOOLS(R)
    KEY_PRESS_BOOLS(S)
    KEY_PRESS_BOOLS(T)
    KEY_PRESS_BOOLS(U)
    KEY_PRESS_BOOLS(V)
    KEY_PRESS_BOOLS(W)
    KEY_PRESS_BOOLS(X)
    KEY_PRESS_BOOLS(Y)
    KEY_PRESS_BOOLS(Z)
    KEY_PRESS_BOOLS(APOS)
};
