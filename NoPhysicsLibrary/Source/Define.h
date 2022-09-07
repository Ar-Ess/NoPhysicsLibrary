#pragma once

#define LOG(format, ...) Log(__FILE__, __LINE__, format, __VA_ARGS__)

void Log(const char file[], int line, const char* format, ...);

#define INT_TO_BOOL(integer) (((integer) != 0) ? 1 : 0)