#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 文件工具函数
int file_exists(const char* filename);
long get_file_size(const char* filename);
int count_lines_in_file(const char* filename);
char* read_file_contents(const char* filename);
int write_file_contents(const char* filename, const char* content);
int append_to_file(const char* filename, const char* content);
int create_directory(const char* path);

#endif // FILE_UTILS_H
