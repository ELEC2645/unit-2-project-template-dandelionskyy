#include "test_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TestSuite* load_test_suite_from_file(const char* filename) {
    if (filename == NULL) {
        return NULL;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open test file: %s\n", filename);
        return NULL;
    }

    TestSuite* suite = create_test_suite(filename);
    if (suite == NULL) {
        fclose(file);
        return NULL;
    }

    char buffer[512];
    int line_number = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        line_number++;
        
        // Skip empty lines and comment lines
        if (buffer[0] == '#' || buffer[0] == '\n' || buffer[0] == '\r') {
            continue;
        }

        // Remove newline characters
        buffer[strcspn(buffer, "\r\n")] = 0;

        TestCase* test_case = parse_test_case_line(buffer);
        if (test_case != NULL) {
            add_test_case(suite, test_case);
        } else {
            printf("Warning: Line %d parsing failed: %s\n", line_number, buffer);
        }
    }

    fclose(file);
    return suite;
}

TestCase* parse_test_case_line(const char* line) {
    if (line == NULL || strlen(line) == 0) {
        return NULL;
    }

    char line_copy[512];
    strncpy(line_copy, line, sizeof(line_copy) - 1);
    line_copy[sizeof(line_copy) - 1] = '\0';

    char* tokens[6];
    int token_count = 0;
    
    char* token = strtok(line_copy, "|");
    while (token != NULL && token_count < 6) {
        tokens[token_count] = token;
        token_count++;
        token = strtok(NULL, "|");
    }

    if (token_count < 6) {
        return NULL;
    }

    TestCase* test_case = create_test_case(tokens[0], tokens[5], parse_test_type(tokens[1]));
    if (test_case == NULL) {
        return NULL;
    }

    // Set SQL query
    if (strlen(tokens[2]) > 0) {
        strncpy(test_case->sql_query, tokens[2], sizeof(test_case->sql_query) - 1);
    }

    // Set data file
    if (strlen(tokens[3]) > 0) {
        strncpy(test_case->data_file, tokens[3], sizeof(test_case->data_file) - 1);
    }

    // Set expected row count
    test_case->expected_row_count = atoi(tokens[4]);

    return test_case;
}

TestType parse_test_type(const char* type_str) {
    if (strcmp(type_str, "SQL_QUERY") == 0) return TEST_SQL_QUERY;
    if (strcmp(type_str, "DATA_LOAD") == 0) return TEST_DATA_LOAD;
    if (strcmp(type_str, "FUNCTIONAL") == 0) return TEST_FUNCTIONAL;
    if (strcmp(type_str, "PERFORMANCE") == 0) return TEST_PERFORMANCE;
    // 添加不带TEST_前缀的兼容性支持
    if (strcmp(type_str, "TEST_SQL_QUERY") == 0) return TEST_SQL_QUERY;
    if (strcmp(type_str, "TEST_DATA_LOAD") == 0) return TEST_DATA_LOAD;
    if (strcmp(type_str, "TEST_FUNCTIONAL") == 0) return TEST_FUNCTIONAL;
    if (strcmp(type_str, "TEST_PERFORMANCE") == 0) return TEST_PERFORMANCE;
    return TEST_SQL_QUERY; // Default
}

// Test case operation functions implementation
TestCase* create_test_case(const char* name, const char* description, TestType type) {
    TestCase* test_case = malloc(sizeof(TestCase));
    if (test_case == NULL) {
        return NULL;
    }

    strncpy(test_case->name, name, MAX_TEST_NAME_LEN - 1);
    strncpy(test_case->description, description, MAX_TEST_DESC_LEN - 1);
    test_case->type = type;
    test_case->sql_query[0] = '\0';
    test_case->data_file[0] = '\0';
    test_case->expected_row_count = -1;
    test_case->expected_column_count = 0;
    test_case->status = TEST_PASSED;
    test_case->error_message[0] = '\0';
    test_case->execution_time = 0.0;

    return test_case;
}

void free_test_case(TestCase* test_case) {
    if (test_case != NULL) {
        free(test_case);
    }
}

TestSuite* create_test_suite(const char* name) {
    TestSuite* suite = malloc(sizeof(TestSuite));
    if (suite == NULL) {
        return NULL;
    }

    strncpy(suite->name, name, MAX_TEST_NAME_LEN - 1);
    suite->test_cases = NULL;
    suite->test_count = 0;
    suite->passed_count = 0;
    suite->failed_count = 0;
    suite->skipped_count = 0;
    suite->total_time = 0.0;

    return suite;
}

void free_test_suite(TestSuite* suite) {
    if (suite == NULL) {
        return;
    }

    if (suite->test_cases != NULL) {
        for (int i = 0; i < suite->test_count; i++) {
            free_test_case(&suite->test_cases[i]);
        }
        free(suite->test_cases);
    }
    free(suite);
}

int add_test_case(TestSuite* suite, TestCase* test_case) {
    if (suite == NULL || test_case == NULL) {
        return -1;
    }

    TestCase* new_cases = realloc(suite->test_cases, (suite->test_count + 1) * sizeof(TestCase));
    if (new_cases == NULL) {
        return -1;
    }

    suite->test_cases = new_cases;
    suite->test_cases[suite->test_count] = *test_case;
    suite->test_count++;

    free(test_case); // Free original pointer after copying
    return 0;
}
