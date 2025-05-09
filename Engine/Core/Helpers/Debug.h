#pragma once

#ifdef _DEBUG
	#include <iostream>
	#include "glad/glad.h"

	#define PRINT(__msg__) \
		printf("%s\n", std::string(__msg__).c_str())

	#define LOG_INFO(__msg__) \
		printf("[INFO] [%s:%d] - %s\n", __FILE__, __LINE__, std::string(__msg__).c_str())

	#define LOG_WARNING(__msg__) \
		printf("[WARNING] [%s:%d] - %s\n", __FILE__, __LINE__, std::string(__msg__).c_str())

	#define LOG_ERROR(__msg__) \
		printf("[ERROR] [%s:%d] - %s\n", __FILE__, __LINE__, std::string(__msg__).c_str())

	#define LOG_WARNING_IF(__cnd__, __msg__)                                                         \
		do                                                                                           \
		{                                                                                            \
			if ((__cnd__))                                                                           \
			{                                                                                        \
				printf("[WARNING] [%s:%d] - %s\n", __FILE__, __LINE__, std::string(__msg__).c_str()) \
			}                                                                                        \
		}                                                                                            \
		while (0, 0)

	#define LOG_ERROR_IF(__cnd__, __msg__)                                                           \
		do                                                                                           \
		{                                                                                            \
			if ((__cnd__))                                                                           \
			{                                                                                        \
				printf("[WARNING] [%s:%d] - %s\n", __FILE__, __LINE__, std::string(__msg__).c_str()) \
			}                                                                                        \
		}                                                                                            \
		while (0, 0)

#else

	#define LOG_INFO(__msg__) \
		do                    \
		{                     \
		}                     \
		while (0)
	#define LOG_WARNING(__msg__) \
		do                       \
		{                        \
		}                        \
		while (0)
	#define LOG_ERROR(__msg__) \
		do                     \
		{                      \
		}                      \
		while (0)

	#define LOG_WARNING_IF(__cnd__, __msg__) \
		do                                 \
		{                                  \
		}                                  \
		while (0)

	#define LOG_ERROR_IF(__cnd__, __msg__)  \
		do                                 \
		{                                  \
		}                                  \
		while (0)


#endif
