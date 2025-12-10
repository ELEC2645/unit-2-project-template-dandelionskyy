#ifndef AI_HELPER_H
#define AI_HELPER_H

#include "../db/table.h"

/* AI assistance functions */
char* obtain_ai_recommendation(const char* user_query, const Table* dataset);
char* produce_sql_from_natural_text(const char* natural_text, const Table* dataset);
char* enhance_sql_statement(const char* sql_command, const Table* dataset);
char* elucidate_query_execution(const char* sql_command, const Table* dataset);

#endif // AI_HELPER_H
