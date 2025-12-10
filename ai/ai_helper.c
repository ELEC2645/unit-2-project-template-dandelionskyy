#include "ai_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* obtain_ai_recommendation(const char* question, const Table* table) {
    if (question == NULL) {
        return NULL;
    }

    char* suggestion = malloc(512);
    if (suggestion == NULL) {
        return NULL;
    }

    /* 简单的AI建议逻辑 */
    char q_lower[256];
    strncpy(q_lower, question, sizeof(q_lower) - 1);
    q_lower[sizeof(q_lower) - 1] = '\0';
    
    for (char* p = q_lower; *p; p++) {
        *p = tolower(*p);
    }

    if (strstr(q_lower, "how") != NULL || strstr(q_lower, "what") != NULL) {
        if (strstr(q_lower, "query") != NULL || strstr(q_lower, "select") != NULL) {
            if (table != NULL) {
                sprintf(suggestion, "Use SELECT statement to query data. Example: SELECT * FROM %s WHERE condition", table->name);
            } else {
                strcpy(suggestion, "Use SELECT statement to query data. Example: SELECT * FROM table_name WHERE condition");
            }
        } else if (strstr(q_lower, "filter") != NULL) {
            strcpy(suggestion, "Use WHERE clause for filtering. Example: SELECT * FROM table_name WHERE column_name = 'value'");
        } else if (strstr(q_lower, "sort") != NULL) {
            strcpy(suggestion, "Use ORDER BY clause for sorting. Example: SELECT * FROM table_name ORDER BY column_name ASC/DESC");
        } else {
            strcpy(suggestion, "Please specify what you want to do, I can give SQL examples.");
        }
    } else if (strstr(q_lower, "select") != NULL || strstr(q_lower, "query") != NULL) {
        if (table != NULL) {
            sprintf(suggestion, "Table '%s' has %d columns: ", table->name, table->col_count);
            char col_list[256] = "";
            for (int i = 0; i < table->col_count && i < 5; i++) {
                strcat(col_list, table->columns[i].name);
                if (i < table->col_count - 1 && i < 4) {
                    strcat(col_list, ", ");
                }
            }
            if (table->col_count > 5) {
                strcat(col_list, " etc.");
            }
            strcat(suggestion, col_list);
        } else {
            strcpy(suggestion, "Please load a CSV file first for better suggestions.");
        }
    } else if (strstr(q_lower, "optimize") != NULL) {
        strcpy(suggestion, "SQL optimization tips: 1. Use column names instead of * 2. Add WHERE conditions 3. Create indexes for often queried columns");
    } else {
        strcpy(suggestion, "I can help: 1. Generate SQL queries 2. Optimize queries 3. Explain query plans 4. Give database suggestions");
    }

    return suggestion;
}

char* produce_sql_from_natural_text(const char* text, const Table* table) {
    if (text == NULL) {
        return NULL;
    }

    char* sql = malloc(512);
    if (sql == NULL) {
        return NULL;
    }

    char txt_lower[256];
    strncpy(txt_lower, text, sizeof(txt_lower) - 1);
    txt_lower[sizeof(txt_lower) - 1] = '\0';
    
    for (char* p = txt_lower; *p; p++) {
        *p = tolower(*p);
    }

    /* 简单的自然语言转SQL */
    if (strstr(txt_lower, "show all") != NULL || strstr(txt_lower, "view all") != NULL) {
        if (table != NULL) {
            sprintf(sql, "SELECT * FROM %s", table->name);
        } else {
            strcpy(sql, "SELECT * FROM table_name");
        }
    } else if (strstr(txt_lower, "find") != NULL || strstr(txt_lower, "search") != NULL) {
        /* 提取关键词用于条件查询 */
        if (table != NULL && table->col_count > 0) {
            sprintf(sql, "SELECT * FROM %s WHERE %s LIKE '%%keyword%%'", 
                   table->name, table->columns[0].name);
        } else {
            strcpy(sql, "SELECT * FROM table_name WHERE column_name LIKE '%keyword%'");
        }
    } else if (strstr(txt_lower, "count") != NULL || strstr(txt_lower, "number") != NULL) {
        if (table != NULL) {
            sprintf(sql, "SELECT COUNT(*) FROM %s", table->name);
        } else {
            strcpy(sql, "SELECT COUNT(*) FROM table_name");
        }
    } else if (strstr(txt_lower, "sort") != NULL) {
        if (table != NULL && table->col_count > 0) {
            sprintf(sql, "SELECT * FROM %s ORDER BY %s ASC", 
                   table->name, table->columns[0].name);
        } else {
            strcpy(sql, "SELECT * FROM table_name ORDER BY column_name ASC");
        }
    } else {
        strcpy(sql, "SELECT * FROM table_name");
    }

    return sql;
}

char* enhance_sql_statement(const char* sql, const Table* table) {
    if (sql == NULL) {
        return NULL;
    }

    char* optimized = malloc(512);
    if (optimized == NULL) {
        return NULL;
    }

    char sql_upper[512];
    strncpy(sql_upper, sql, sizeof(sql_upper) - 1);
    sql_upper[sizeof(sql_upper) - 1] = '\0';
    
    for (char* p = sql_upper; *p; p++) {
        *p = toupper(*p);
    }

    /* 简单的SQL优化建议 */
    if (strstr(sql_upper, "SELECT *") != NULL) {
        if (table != NULL && table->col_count > 0) {
            /* 建议使用具体的列名 */
            strcpy(optimized, "Suggestion: Use specific column names instead of *, e.g.: SELECT ");
            for (int i = 0; i < table->col_count && i < 3; i++) {
                strcat(optimized, table->columns[i].name);
                if (i < table->col_count - 1 && i < 2) {
                    strcat(optimized, ", ");
                }
            }
            if (table->col_count > 3) {
                strcat(optimized, " etc.");
            }
            strcat(optimized, " FROM ...");
        } else {
            strcpy(optimized, "Suggestion: Use specific column names instead of * to improve performance");
        }
    } else if (strstr(sql_upper, "LIKE '%") != NULL) {
        strcpy(optimized, "Suggestion: Leading wildcard searches (LIKE '%...') cannot use indexes, try other methods");
    } else if (strstr(sql_upper, "ORDER BY") == NULL && strstr(sql_upper, "WHERE") != NULL) {
        strcpy(optimized, "Query looks OK. Add ORDER BY if you need sorting");
    } else {
        strcpy(optimized, "Query syntax is correct. For more optimization, give more context");
    }

    return optimized;
}

char* elucidate_query_execution(const char* sql, const Table* table) {
    if (sql == NULL) {
        return NULL;
    }

    char* explanation = malloc(512);
    if (explanation == NULL) {
        return NULL;
    }

    char sql_upper[512];
    strncpy(sql_upper, sql, sizeof(sql_upper) - 1);
    sql_upper[sizeof(sql_upper) - 1] = '\0';
    
    for (char* p = sql_upper; *p; p++) {
        *p = toupper(*p);
    }

    /* 简单的查询计划解释 */
    if (strstr(sql_upper, "SELECT") != NULL) {
        strcpy(explanation, "Query plan:\n");
        
        if (strstr(sql_upper, "WHERE") != NULL) {
            strcat(explanation, "1. Apply WHERE condition filter\n");
        }
        
        if (strstr(sql_upper, "ORDER BY") != NULL) {
            strcat(explanation, "2. Sort the results\n");
        }
        
        strcat(explanation, "3. Return query results");
        
        if (table != NULL) {
            char temp[100];
            sprintf(temp, "\nWill process about %d rows", table->row_count);
            strcat(explanation, temp);
        }
    } else {
        strcpy(explanation, "Cannot understand query type, check SQL syntax");
    }

    return explanation;
}
