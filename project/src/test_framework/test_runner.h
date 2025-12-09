#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include "testcase.h"
#include "../db/table.h"

// 测试运行函数
int run_test_suite(const char* test_file);
int run_test_case(TestCase* test_case, Table* data_table);
int verify_test_result(TestCase* test_case, Table* result_table);
void cleanup_test_resources();

#endif // TEST_RUNNER_H
