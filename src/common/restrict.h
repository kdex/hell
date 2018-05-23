#pragma once
#ifndef RESTRICT
	#ifdef _MSC_VER
		#define RESTRICT __restrict
	#else
		#define RESTRICT restrict
	#endif
#endif