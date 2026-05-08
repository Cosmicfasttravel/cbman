# What this is
This hooks into your clipboard and allows you to paste your last 3 clipboard copies in different hotkeys, it runs extremely lightweight with little overhead <2mb ram usage and <0.1% cpu usage

# Hotkeys
### CTRL + SHIFT + ALT + X = Exit
### CTRL + F1 = Paste slot 1
### CTRL + F2 = Paste slot 2
### CTRL + F3 = Paste slot 3

# To change them look for:
RegisterHotKey(hwnd, 1, MOD_CONTROL | MOD_SHIFT | MOD_ALT, 'X');  
RegisterHotKey(hwnd, 2, MOD_CONTROL, VK_F1);  
RegisterHotKey(hwnd, 3, MOD_CONTROL, VK_F2);  
RegisterHotKey(hwnd, 4, MOD_CONTROL, VK_F3);  
