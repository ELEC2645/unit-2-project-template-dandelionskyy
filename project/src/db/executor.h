#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "table.h"

// 查询执行函数
QueryResult* execute_query(Table* table, Query* query);
int evaluate_condition(const Table* table, int row, const Condition* condition);
Table* select_columns(const Table* table, const Query* query);
Table* filter_rows(const Table* table, const Condition* conditions);
Table* sort_table(const Table* table, const char* column, SortDirection direction);

#endif // EXECUTOR_H
