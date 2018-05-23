#pragma once
#ifdef _MSC_VER
	#define NO_RETURN __declspec(noreturn)
#else
	#include <stdnoreturn.h>
	#define NO_RETURN noreturn
#endif