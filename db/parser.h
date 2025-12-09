#ifndef PARSER_H
#define PARSER_H

#include "table.h"

// SQL解析函数
Query* parse_query(const char* sql);
Operator parse_operator(const char* op_str);
QueryType parse_query_type(const char* sql);
void free_condition(Condition* condition);

#endif // PARSER_H
