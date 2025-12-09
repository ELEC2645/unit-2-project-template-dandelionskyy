#ifndef TESTCASE_H
#define TESTCASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEST_NAME_LEN 100
#define MAX_TEST_DESC_LEN 256
#define MAX_SQL_LEN 512
#define MAX_EXPECTED_ROWS 1000

// 测试用例类型
typedef enum {
    TEST_SQL_QUERY,
    TEST_DATA_LOAD,
    TEST_FUNCTIONAL,
    TEST_PERFORMANCE
} TestType;

// 测试用例状态
typedef enum {
    TEST_PASSED,
    TEST_FAILED,
    TEST_SKIPPED,
    TEST_ERROR
} TestStatus;

// 单个测试用例
typedef struct {
    char name[MAX_TEST_NAME_LEN];
    char description[MAX_TEST_DESC_LEN];
    TestType type;
    char sql_query[MAX_SQL_LEN];
    char data_file[100];
    int expected_row_count;
    char expected_columns[50][50];
    int expected_column_count;
    TestStatus status;
    char error_message[256];
    double execution_time;
} TestCase;

// 测试套件
typedef struct {
    char name[MAX_TEST_NAME_LEN];
    TestCase* test_cases;
    int test_count;
    int passed_count;
    int failed_count;
    int skipped_count;
    double total_time;
} TestSuite;

// 测试用例操作函数
TestCase* create_test_case(const char* name, const char* description, TestType type);
void free_test_case(TestCase* test_case);
TestSuite* create_test_suite(const char* name);
void free_test_suite(TestSuite* suite);
int add_test_case(TestSuite* suite, TestCase* test_case);

#endif // TESTCASE_H
