#pragma once

#define LOG(format, ...) Log(__FILE__, __LINE__, format, __VA_ARGS__)

void Log(const char file[], int line, const char* format, ...);

#define INT_TO_BOOL(integer) (((integer) != 0) ? 1 : 0)

// Deletes a buffer
#define RELEASE( x ) \
	{						\
	if( x != NULL )		\
		{					  \
	  delete x;			\
	  x = NULL;			  \
		}					  \
	}

struct Hint
{
	// ===================
	int Func() {}
	int initialsBelow = 5;
	const char* physics = "Constructor";

	// ===================
	int initialsIn = 5;
	void Playback() {}
	enum NewCreation
	{
		MACROS = 0,
		FUNCTIONS = 0,
		VARIABLES = 1,
		POINTERS = 0
	};

};