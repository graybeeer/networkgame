#pragma once

#include <Windows.h>
#define MAX_BUFFER 256

struct KEYINFO {
    bool down{ false };
    bool pressed{ false };
    bool up{ false };
    bool dClick{ false };
    bool dClickStart{ false };
    float downStart{ 0.f };
    float dClickTime{ 0.2f };
};

struct KEYINPUT {
	KEYINFO keyInfo[MAX_BUFFER];

	void Input(float deltaTime) {
		for (int i = 0; i < MAX_BUFFER; ++i) {
			if (GetAsyncKeyState(i) & 0x8000) {
                if (keyInfo[i].down) {
                    keyInfo[i].pressed = true;
                    keyInfo[i].down = false;
                }
                else {
                    if (!keyInfo[i].pressed) {
                        if (keyInfo[i].dClickStart) {
                            keyInfo[i].dClick = true;
                        }
                        else {
                            keyInfo[i].dClickStart = true;
                        }
						keyInfo[i].down = true;
                    }
                }
            }
            else {
                if (keyInfo[i].up) {
                    keyInfo[i].up = false;
                }
                else if (keyInfo[i].down || keyInfo[i].pressed) {
                    keyInfo[i].up = true;
                    keyInfo[i].pressed = false;
                    keyInfo[i].down = false;
                }
            }
		}
	}
    
    void Update(float deltaTime) {
        for (int i = 0; i < MAX_BUFFER; ++i) {
            if (keyInfo[i].dClickStart) {
                keyInfo[i].downStart += deltaTime;
                if (keyInfo[i].downStart >= keyInfo[i].dClickTime) {
                    keyInfo[i].downStart = 0.f;
                    keyInfo[i].dClickStart = false;
                    keyInfo[i].dClick = false;
                }
            }
        }
    }

	void Reset() {
        memset(keyInfo, false, MAX_BUFFER);
	}

    KEYINFO& operator[](int idx) {
        return keyInfo[idx];
    }
};