#pragma once

/*
	Copyright (c) 2021 QuickNET

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

class WinConsole {
public:

	explicit WinConsole(char const* const consoleWindowTitle)
		: consoleWindowHandle(createConsole(consoleWindowTitle)) {}

	WinConsole(WinConsole const& wincon) = default;
	WinConsole& operator=(WinConsole const&) = default;

	WinConsole(WinConsole&& wincon) = default;
	WinConsole& operator=(WinConsole&& wincon) = default;

	// destroy the console on destruction of the object
	~WinConsole() {
		close();
	}

	// forcefully close the console window
	void close() {
		removeIoRiderect();

		FreeConsole();

		// PostMessageA is used here because DestroyWindow has shown that it occasionally doesn't like to work 
		PostMessageA(consoleWindowHandle, WM_CLOSE, 0, 0);
	}

	// create a console window
	HWND createConsole(char const* const consoleWindowTitle) {
		AllocConsole();

		redirectIoToConsole();

		setTitle(consoleWindowTitle);

		return GetConsoleWindow();
	}

	// disable closing of console window
	WinConsole& disableCloseButton() {
		EnableMenuItem(GetSystemMenu(consoleWindowHandle, 0), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

		return *this;
	}

	// enable closing of console window
	WinConsole& enableCloseButton() {
		EnableMenuItem(GetSystemMenu(consoleWindowHandle, 0), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);

		return *this;
	}

	// minimize the console to your taskbar
	WinConsole& minimize() {
		ShowWindow(consoleWindowHandle, SW_MINIMIZE);

		return *this;
	}

	// hide the console window completely
	WinConsole& hide() {
		ShowWindow(consoleWindowHandle, SW_HIDE);

		return *this;
	}

	// recreate the console window
	WinConsole& reallocateConsole(char const* const consoleWindowTitle = nullptr) {
		close();

		consoleWindowHandle = createConsole(consoleWindowTitle);

		return *this;
	}

	// allow the console window to be below other windows again
	WinConsole& removeAlwaysOnTop() {
		SetWindowPos(consoleWindowHandle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		ShowWindow(consoleWindowHandle, SW_NORMAL);

		return *this;
	}

	// restore the console window from being minimized or hidden
	WinConsole& restore() {
		ShowWindow(consoleWindowHandle, SW_RESTORE);

		return *this;
	}

	// force the console window to stay on top of other windows
	WinConsole& setAlwaysOnTop() {
		SetWindowPos(consoleWindowHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		ShowWindow(consoleWindowHandle, SW_NORMAL);

		return *this;
	}

	// set the title of the console window
	WinConsole& setTitle(char const* const consoleWindowTitle) {
		if(consoleWindowTitle) {
			SetConsoleTitleA(consoleWindowTitle);
		}

		return *this;
	}

private:

	HWND consoleWindowHandle {};

	void redirectIoToConsole() {
		FILE* file = nullptr;

		freopen_s(&file, "CONIN$", "r", stdin);
		freopen_s(&file, "CONOUT$", "w", stdout);
		freopen_s(&file, "CONOUT$", "w", stderr);
	}

	void removeIoRiderect() {
		FILE* file = nullptr;

		freopen_s(&file, "NUL:", "r", stdin);
		freopen_s(&file, "NUL:", "w", stdout);
		freopen_s(&file, "NUL:", "w", stderr);
	}

};
