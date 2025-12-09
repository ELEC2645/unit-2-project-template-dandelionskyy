#ifndef TEST_LOADER_H
#define TEST_LOADER_H

#include "testcase.h"

// 测试用例加载函数
TestSuite* load_test_suite_from_file(const char* filename);
TestCase* parse_test_case_line(const char* line);
TestType parse_test_type(const char* type_str);

#endif // TEST_LOADER_H
