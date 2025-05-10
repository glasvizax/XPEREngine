#pragma once

#ifdef _DEBUG
	#include <iostream>
	#include "glad/glad.h"

	#define LOG_INFO_F(__format__, ...) \
		printf("[INFO] [%s:%d] - " __format__ " \n", __FILE__, __LINE__, ##__VA_ARGS__)

	#define LOG_WARNING_F(__format__, ...) \
		printf("[WARNING] [%s:%d] - " __format__ " \n", __FILE__, __LINE__, ##__VA_ARGS__)

	#define LOG_ERROR_F(__format__, ...) \
		printf("[ERROR] [%s:%d] - " __format__ " \n", __FILE__, __LINE__, ##__VA_ARGS__)

	#define LOG_INFO_S(__cstr__) \
		printf("[INFO] [%s:%d] - %s \n", __FILE__, __LINE__, __cstr__)

	#define LOG_WARNING_S(__cstr__) \
		printf("[WARNING] [%s:%d] - %s \n", __FILE__, __LINE__, __cstr__)

	#define LOG_ERROR_S(__cstr__) \
		printf("[ERROR] [%s:%d] - %s \n", __FILE__, __LINE__, __cstr__)

#else

	#define LOG_INFO_F(__format__, ...) \
		do                              \
		{                               \
		}                               \
		while (0)
	#define LOG_WARNING_F(__format__, ...) \
		do                                 \
		{                                  \
		}                                  \
		while (0)
	#define LOG_ERROR_F(__format__, ...) \
		do                               \
		{                                \
		}                                \
		while (0)

	#define LOG_INFO_S(__cstr__) \
		do                       \
		{                        \
		}                        \
		while (0)
	#define LOG_WARNING_S(__cstr__) \
		do                          \
		{                           \
		}                           \
		while (0)
	#define LOG_ERROR_S(__cstr__) \
		do                        \
		{                         \
		}                         \
		while (0)

#endif
