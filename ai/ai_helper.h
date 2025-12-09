#ifndef AI_HELPER_H
#define AI_HELPER_H

#include "../db/table.h"

// AI辅助函数
char* get_ai_suggestion(const char* question, const Table* table);
char* generate_sql_from_natural_language(const char* natural_language, const Table* table);
char* optimize_sql_query(const char* sql, const Table* table);
char* explain_query_plan(const char* sql, const Table* table);

#endif // AI_HELPER_H
