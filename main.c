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

int sendCapture(void) {
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
        hRequest = WinHttpOpenRequest(hConnect, L"GET",
            NULL,
            NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            0);

    // Send a Request.
    if (hRequest)
        bResults = WinHttpSendRequest(hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS,
            0, WINHTTP_NO_REQUEST_DATA, 0,
            0, 0);

	if (!bResults)
		printf("Error %d has occurred.\n", GetLastError());

	// End the request.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	return 1;
}

int main(void) {
	sendCapture();

	return 1;
}