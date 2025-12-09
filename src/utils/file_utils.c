#include "file_utils.h"
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

int file_exists(const char* filename) {
    if (filename == NULL) {
        return 0;
    }

    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

long get_file_size(const char* filename) {
    if (filename == NULL) {
        return -1;
    }

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);

    return size;
}

int count_lines_in_file(const char* filename) {
    if (filename == NULL) {
        return -1;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    int line_count = 0;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        line_count++;
    }

    fclose(file);
    return line_count;
}

char* read_file_contents(const char* filename) {
    if (filename == NULL) {
        return NULL;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size <= 0) {
        fclose(file);
        return NULL;
    }

    // Allocate memory
    char* content = malloc(file_size + 1);
    if (content == NULL) {
        fclose(file);
        return NULL;
    }

    // Read file content
    size_t bytes_read = fread(content, 1, file_size, file);
    content[bytes_read] = '\0';

    fclose(file);
    return content;
}

int write_file_contents(const char* filename, const char* content) {
    if (filename == NULL || content == NULL) {
        return 0;
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return 0;
    }

    size_t bytes_written = fwrite(content, 1, strlen(content), file);
    fclose(file);

    return bytes_written == strlen(content);
}

int append_to_file(const char* filename, const char* content) {
    if (filename == NULL || content == NULL) {
        return 0;
    }

    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        return 0;
    }

    size_t bytes_written = fwrite(content, 1, strlen(content), file);
    fclose(file);

    return bytes_written == strlen(content);
}

int create_directory(const char* path) {
    if (path == NULL) {
        return 0;
    }

    // 检查目录是否已存在
    struct stat st;
    if (stat(path, &st) == 0) {
        return S_ISDIR(st.st_mode); // 已存在且是目录
    }

    // 创建目录
    return MKDIR(path) == 0;
}
