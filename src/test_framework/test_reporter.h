#ifndef TEST_REPORTER_H
#define TEST_REPORTER_H

#include "testcase.h"

// 测试报告函数
void print_test_report(const TestSuite* suite);
void generate_html_report(const TestSuite* suite, const char* filename);
void print_test_case_result(const TestCase* test_case);

#endif // TEST_REPORTER_H
