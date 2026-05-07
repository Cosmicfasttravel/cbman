#include <iostream>
#include <string>
#include <windows.h>
#include "threadManager.h"

int main() {
    const auto mgr = std::make_unique<PollingThreadManager>();

    std::thread t([mgr = mgr.get()] {
        mgr->run();
    });

    bool running = true;
    while (running) {
        const int i = mgr->getLatestHotkey();
        std::wstring s;

        if (i > 0) s = mgr->popIndex(i - 1);
        else if (i == -1) mgr->clear();
        else if (i == -100) running = false;

        if (!s.empty()) {
            mgr->setClipboardText(s);
            mgr->paste();
        }

        Sleep(10);
    }

    mgr->stop();
    t.join();
    return 0;
}
