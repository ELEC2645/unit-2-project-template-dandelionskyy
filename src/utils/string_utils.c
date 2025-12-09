#include "string_utils.h"

void trim_string(char* str) {
    if (str == NULL) {
        return;
    }

    char* start = str;
    char* end = str + strlen(str) - 1;

    // 去除开头空格
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    // 去除结尾空格
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    // 移动字符串到开头
    if (start != str) {
        memmove(str, start, end - start + 1);
    }

    // 添加终止符
    str[end - start + 1] = '\0';
}

char* to_lower_case(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    char* result = malloc(strlen(str) + 1);
    if (result == NULL) {
        return NULL;
    }

    char* p = result;
    while (*str) {
        *p++ = tolower((unsigned char)*str++);
    }
    *p = '\0';

    return result;
}

char* to_upper_case(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    char* result = malloc(strlen(str) + 1);
    if (result == NULL) {
        return NULL;
    }

    char* p = result;
    while (*str) {
        *p++ = toupper((unsigned char)*str++);
    }
    *p = '\0';

    return result;
}

int string_contains(const char* str, const char* substr) {
    if (str == NULL || substr == NULL) {
        return 0;
    }

    return strstr(str, substr) != NULL;
}

char** split_string(const char* str, const char* delimiter, int* count) {
    if (str == NULL || delimiter == NULL || count == NULL) {
        return NULL;
    }

    // 复制字符串以便修改
    char* str_copy = malloc(strlen(str) + 1);
    if (str_copy == NULL) {
        return NULL;
    }
    strcpy(str_copy, str);

    // 计算分割后的元素数量
    int element_count = 0;
    char* token = strtok(str_copy, delimiter);
    while (token != NULL) {
        element_count++;
        token = strtok(NULL, delimiter);
    }

    // 分配结果数组
    char** result = malloc(element_count * sizeof(char*));
    if (result == NULL) {
        free(str_copy);
        return NULL;
    }

    // 重新分割字符串
    strcpy(str_copy, str);
    element_count = 0;
    token = strtok(str_copy, delimiter);
    while (token != NULL) {
        result[element_count] = malloc(strlen(token) + 1);
        if (result[element_count] == NULL) {
            // 清理已分配的内存
            for (int i = 0; i < element_count; i++) {
                free(result[i]);
            }
            free(result);
            free(str_copy);
            return NULL;
        }
        strcpy(result[element_count], token);
        element_count++;
        token = strtok(NULL, delimiter);
    }

    free(str_copy);
    *count = element_count;
    return result;
}

void free_string_array(char** array, int count) {
    if (array == NULL) {
        return;
    }

    for (int i = 0; i < count; i++) {
        if (array[i] != NULL) {
            free(array[i]);
        }
    }
    free(array);
}

int is_numeric(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }

    char* end;
    strtod(str, &end);
    return *end == '\0';
}

int is_integer(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }

    char* end;
    strtol(str, &end, 10);
    return *end == '\0';
}
