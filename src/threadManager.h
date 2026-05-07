#ifndef CB_THREADMANAGER_H
#define CB_THREADMANAGER_H
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

/* Usage:
 * run(); once
 * Do something with popNext();
 * stop(); once
 */

class PollingThreadManager {
public:
    void run() {
        b_run = true;
        b_stop = false;

        poll();
    }

    void stop() {
        b_stop = true;
        b_run = false;
    }

    std::wstring popNext() {
        if (history.empty()) return {}; {
            std::lock_guard lock(historyMutex);

            std::wstring popped = history.front();

            return popped;
        }
    }

    std::wstring popIndex(const int i) {
         {
             std::lock_guard lock(historyMutex);
             if (history.empty()) return {};

             std::wstring popped;
             try {
                 if (i < 0 || i >= (int)history.size()) {
                     std::cout << "Not filled slot" << std::endl;
                     return {};
                 }
                 popped = history.at(i);
             }
             catch (std::exception& e) {
                 std::cout << "Not filled slot: " << e.what() << std::endl;
             }

             return popped;
        }
    }

    void setClipboardText(const std::wstring& text) {
        if (!OpenClipboard(NULL)) return;

        EmptyClipboard();

        size_t size = (text.size() + 1) * sizeof(wchar_t);
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);

        if (hMem) {
            memcpy(GlobalLock(hMem), text.c_str(), size);
            GlobalUnlock(hMem);

            SetClipboardData(CF_UNICODETEXT, hMem);
        }

        CloseClipboard();
    }

    void paste() {
        INPUT inputs[4] = {};

        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = VK_CONTROL;

        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = 'V';

        Sleep(10);

        inputs[2].type = INPUT_KEYBOARD;
        inputs[2].ki.wVk = 'V';
        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

        inputs[3].type = INPUT_KEYBOARD;
        inputs[3].ki.wVk = VK_CONTROL;
        inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

        SendInput(4, inputs, sizeof(INPUT));
    }

    void clear() {
        {
            std::lock_guard lock(historyMutex);
            history.clear();
        }
    }

    int getLatestHotkey() {
        if (clearPressed) {
            clearPressed = false;
            return -1;
        }

        if (exitPressed) {
            exitPressed = false;
            return -100;
        }

        if (hotkey1Pressed) {
            hotkey1Pressed = false;
            ++slots;
            return 1;
        }

        if (hotkey2Pressed) {
            hotkey2Pressed = false;
            ++slots;
            return 2;
        }

        if (hotkey3Pressed) {
            hotkey3Pressed = false;
            ++slots;
            return 3;
        }

        return 0;
    }

private:
    int slots = 0;

    std::deque<std::wstring> history;

    std::mutex historyMutex;

    std::atomic<bool> b_stop{false};
    std::atomic<bool> b_run{false};

    std::atomic<bool> clearPressed = false;
    std::atomic<bool> exitPressed = false;

    std::atomic<bool> hotkey1Pressed = false;
    std::atomic<bool> hotkey2Pressed = false;
    std::atomic<bool> hotkey3Pressed = false;



    void poll() {
        HWND hwnd = CreateWindowExW(
            0,
            L"STATIC",
            L"clipboard_listener",
            0,
            0, 0, 0, 0,
            HWND_MESSAGE,
            NULL,
            GetModuleHandleW(NULL),
            NULL
        );

        if (!hwnd) {
            std::cout << "HWND creation failed\n";
            return;
        }

        RegisterHotKey(hwnd, 100, MOD_CONTROL | MOD_SHIFT | MOD_ALT, 'C');
        RegisterHotKey(hwnd, 101, MOD_CONTROL | MOD_SHIFT | MOD_ALT, 'X');
        RegisterHotKey(hwnd, 1, MOD_CONTROL | MOD_SHIFT, '1');
        RegisterHotKey(hwnd, 2, MOD_CONTROL | MOD_SHIFT, '2');
        RegisterHotKey(hwnd, 3, MOD_CONTROL | MOD_SHIFT, '3');

        AddClipboardFormatListener(hwnd);

        MSG msg = {};

        while (!b_stop && GetMessage(&msg, NULL, 0, 0)) {
            if (msg.message == WM_HOTKEY && msg.wParam == 1) {
                hotkey1Pressed = true;
                std::cout << "Hotkey 1 - Pressed" << std::endl;
            }

            if (msg.message == WM_HOTKEY && msg.wParam == 2) {
                hotkey2Pressed = true;
                std::cout << "Hotkey 2 - Pressed" << std::endl;
            }

            if (msg.message == WM_HOTKEY && msg.wParam == 3) {
                hotkey3Pressed = true;
                std::cout << "Hotkey 3 - Pressed" << std::endl;
            }

            if (msg.message == WM_HOTKEY && msg.wParam == 100) {
                clearPressed = true;
                std::cout << "Clear Hotkey - Pressed" << std::endl;
            }

            if (msg.message == WM_HOTKEY && msg.wParam == 101) {
                exitPressed = true;
                std::cout << "Exit Hotkey - Pressed (Press another hotkey to exit)" << std::endl;
            }

            if (msg.message == WM_CLIPBOARDUPDATE) {
                {
                    std::lock_guard lock(historyMutex);
                    std::wstring text = openClipboard();

                    if (text.empty()) continue;

                    if (history.empty()){
                        history.push_back(text);
                        continue;
                    }

                    if (text == history.front()) continue;

                    history.push_back(text);

                    while (history.size() >= slots) {
                        history.pop_front();
                    }
                }
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (b_stop) break;
        }
        std::cout << "Listener Removed" << std::endl;

        RemoveClipboardFormatListener(hwnd);
    }

    std::wstring openClipboard() {
        if (!OpenClipboard(NULL))
            return {};

        std::wstring result;
        HANDLE hData = GetClipboardData(CF_UNICODETEXT);
        if (hData) {
            if (auto *text = static_cast<wchar_t *>(GlobalLock(hData))) {
                result = text;
                GlobalUnlock(hData);
            }
        }

        CloseClipboard();
        return result;
    }
};

#endif
