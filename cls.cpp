#include "cls.h"
#include <windows.h>

void cls()
{
	HANDLE console = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	::GetConsoleScreenBufferInfo(console, &csbi);
	COORD origin = { 0, 0 };
	DWORD written;
	::FillConsoleOutputCharacterA(console, ' ', csbi.dwSize.X * csbi.dwSize.Y,
		origin, &written);
	::FillConsoleOutputAttribute(console, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y,
		origin, &written);
	::SetConsoleCursorPosition(console, origin);
}