# What this is
This hooks into your clipboard and allows you to paste your last 3 clipboard copies in different hotkeys, it runs extremely lightweight with little overhead <2mb ram usage and <0.1% cpu usage

# Hotkeys
### CTRL + SHIFT + ALT + X = Exit
### CTRL + F1 = Paste slot 1
### CTRL + F2 = Paste slot 2
### CTRL + F3 = Paste slot 3

<br>

# To change them look for:
RegisterHotKey(hwnd, 1, MOD_CONTROL | MOD_SHIFT | MOD_ALT, 'X');  
RegisterHotKey(hwnd, 2, MOD_CONTROL, VK_F1);  
RegisterHotKey(hwnd, 3, MOD_CONTROL, VK_F2);  
RegisterHotKey(hwnd, 4, MOD_CONTROL, VK_F3);  

<br>

# FAQ
## My computer says that it protected my device
### Click more info then click run anyway, this program does not install any malware and is not malware itself

<br>

## My computer wont allow me to exit the program
### If you are unable to exit the cbman.exe file by exiting the terminal or by shutting it down in task manager, a restart will instantly kill the program and it will not rerun on startup

<br>

## If this base exe is 0.13mb what does it install
### Nothing! The exe is the whole manager

<br>

## My antivirus flagged it as malware
### This is a false positive, most likely because it uses WIN API calls that can be exploited by malware

<br>

## Enjoy!
