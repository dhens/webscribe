#include <stdio.h>
#include <Windows.h>
#include <time.h>
int main(void) {
	char capture;

	// Time stuff
	time_t t;
	t = time(NULL);

	HWND window;
	AllocConsole();
	window = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(window, 1);

	// Hide window
	while (1) {
		Sleep(20);
		if (kbhit()) {
			capture = getch();
			switch (capture) {
			default:
				printf("%c", capture);
			}
		}
	}
}