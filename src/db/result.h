#ifndef RESULT_H
#define RESULT_H

#include "table.h"

// 查询结果操作函数
void print_query_result(const QueryResult* result);
void export_result_to_csv(const QueryResult* result, const char* filename);

#endif // RESULT_H
