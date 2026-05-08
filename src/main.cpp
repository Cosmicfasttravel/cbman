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
        std::wstring s;

        s = mgr->popNext();

        if (!s.empty()) {
            mgr->setClipboardText(s);
            mgr->paste();
        }

        Sleep(1);
    }

    mgr->stop();
    t.join();
    return 0;
}
