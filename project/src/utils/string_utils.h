#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 字符串工具函数
void trim_string(char* str);
char* to_lower_case(const char* str);
char* to_upper_case(const char* str);
int string_contains(const char* str, const char* substr);
char** split_string(const char* str, const char* delimiter, int* count);
void free_string_array(char** array, int count);
int is_numeric(const char* str);
int is_integer(const char* str);

#endif // STRING_UTILS_H
