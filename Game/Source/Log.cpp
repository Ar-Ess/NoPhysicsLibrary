#include "Log.h"

#include <windows.h>
#include <stdio.h>
#include <string>

void Log(const char file[], int line, const char* format, ...)
{
	static char tmpString1[4096];
	static char tmpString2[4096];
	static va_list ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmpString1, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmpString2, 4096, "\n%s(%d) : %s", file, line, tmpString1);

	OutputDebugString(tmpString2);
}

bool SameString(std::string a, std::string b)
{
	bool ret = true;

	if (a.size() != b.size()) return false;

	for (unsigned int i = 0; i < a.size(); i++)
	{
		ret = (a[i] == b[i]);
		if (!ret) return false;
	}

	return true;
}