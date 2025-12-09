#include "ai_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* get_ai_suggestion(const char* question, const Table* table) {
    if (question == NULL) {
        return NULL;
    }

    char* suggestion = malloc(512);
    if (suggestion == NULL) {
        return NULL;
    }

    // Simple AI suggestion logic
    // In real applications, this could integrate with actual AI models
    char question_lower[256];
    strncpy(question_lower, question, sizeof(question_lower) - 1);
    question_lower[sizeof(question_lower) - 1] = '\0';
    
    for (char* p = question_lower; *p; p++) {
        *p = tolower(*p);
    }

    if (strstr(question_lower, "how") != NULL || strstr(question_lower, "what") != NULL) {
        if (strstr(question_lower, "query") != NULL || strstr(question_lower, "select") != NULL) {
            if (table != NULL) {
                sprintf(suggestion, "You can use SELECT statement to query data. For example: SELECT * FROM %s WHERE condition", table->name);
            } else {
                strcpy(suggestion, "You can use SELECT statement to query data. For example: SELECT * FROM table_name WHERE condition");
            }
        } else if (strstr(question_lower, "filter") != NULL) {
            strcpy(suggestion, "Use WHERE clause for data filtering. For example: SELECT * FROM table_name WHERE column_name = 'value'");
        } else if (strstr(question_lower, "sort") != NULL) {
            strcpy(suggestion, "Use ORDER BY clause for sorting. For example: SELECT * FROM table_name ORDER BY column_name ASC/DESC");
        } else {
            strcpy(suggestion, "Please specify what functionality you want to learn about, I can provide corresponding SQL statement examples.");
        }
    } else if (strstr(question_lower, "select") != NULL || strstr(question_lower, "query") != NULL) {
        if (table != NULL) {
            sprintf(suggestion, "Current table '%s' contains %d columns: ", table->name, table->col_count);
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
            strcpy(suggestion, "Please load a data file first, then I can provide more specific query suggestions.");
        }
    } else if (strstr(question_lower, "optimize") != NULL) {
        strcpy(suggestion, "SQL optimization suggestions: 1. Use specific column names instead of * 2. Add appropriate WHERE conditions 3. Create indexes for frequently queried columns");
    } else {
        strcpy(suggestion, "I can help you: 1. Generate SQL query statements 2. Optimize existing queries 3. Explain query execution plans 4. Provide database usage suggestions");
    }

    return suggestion;
}

char* generate_sql_from_natural_language(const char* natural_language, const Table* table) {
    if (natural_language == NULL) {
        return NULL;
    }

    char* sql = malloc(512);
    if (sql == NULL) {
        return NULL;
    }

    char nl_lower[256];
    strncpy(nl_lower, natural_language, sizeof(nl_lower) - 1);
    nl_lower[sizeof(nl_lower) - 1] = '\0';
    
    for (char* p = nl_lower; *p; p++) {
        *p = tolower(*p);
    }

    // Simple natural language to SQL conversion
    if (strstr(nl_lower, "show all") != NULL || strstr(nl_lower, "view all") != NULL) {
        if (table != NULL) {
            sprintf(sql, "SELECT * FROM %s", table->name);
        } else {
            strcpy(sql, "SELECT * FROM table_name");
        }
    } else if (strstr(nl_lower, "find") != NULL || strstr(nl_lower, "search") != NULL) {
        // Extract keywords for simple conditional queries
        if (table != NULL && table->col_count > 0) {
            sprintf(sql, "SELECT * FROM %s WHERE %s LIKE '%%keyword%%'", 
                   table->name, table->columns[0].name);
        } else {
            strcpy(sql, "SELECT * FROM table_name WHERE column_name LIKE '%keyword%'");
        }
    } else if (strstr(nl_lower, "count") != NULL || strstr(nl_lower, "number") != NULL) {
        if (table != NULL) {
            sprintf(sql, "SELECT COUNT(*) FROM %s", table->name);
        } else {
            strcpy(sql, "SELECT COUNT(*) FROM table_name");
        }
    } else if (strstr(nl_lower, "sort") != NULL) {
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

char* optimize_sql_query(const char* sql, const Table* table) {
    if (sql == NULL) {
        return NULL;
    }

    char* optimized_sql = malloc(512);
    if (optimized_sql == NULL) {
        return NULL;
    }

    char sql_upper[512];
    strncpy(sql_upper, sql, sizeof(sql_upper) - 1);
    sql_upper[sizeof(sql_upper) - 1] = '\0';
    
    for (char* p = sql_upper; *p; p++) {
        *p = toupper(*p);
    }

    // Simple SQL optimization suggestions
    if (strstr(sql_upper, "SELECT *") != NULL) {
        if (table != NULL && table->col_count > 0) {
            // Suggest using specific column names
            strcpy(optimized_sql, "Suggestion: Use specific column names instead of *, for example: SELECT ");
            for (int i = 0; i < table->col_count && i < 3; i++) {
                strcat(optimized_sql, table->columns[i].name);
                if (i < table->col_count - 1 && i < 2) {
                    strcat(optimized_sql, ", ");
                }
            }
            if (table->col_count > 3) {
                strcat(optimized_sql, " etc.");
            }
            strcat(optimized_sql, " FROM ...");
        } else {
            strcpy(optimized_sql, "Suggestion: Use specific column names instead of * to improve query performance");
        }
    } else if (strstr(sql_upper, "LIKE '%") != NULL) {
        strcpy(optimized_sql, "Suggestion: Leading wildcard searches (LIKE '%...') cannot use indexes, consider other query methods");
    } else if (strstr(sql_upper, "ORDER BY") == NULL && strstr(sql_upper, "WHERE") != NULL) {
        strcpy(optimized_sql, "Query looks good. If you need to sort results, you can add ORDER BY clause");
    } else {
        strcpy(optimized_sql, "Query syntax is correct. For further optimization, please provide more context information");
    }

    return optimized_sql;
}

char* explain_query_plan(const char* sql, const Table* table) {
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

    // Simple query plan explanation
    if (strstr(sql_upper, "SELECT") != NULL) {
        strcpy(explanation, "Query plan:\n");
        
        if (strstr(sql_upper, "WHERE") != NULL) {
            strcat(explanation, "1. Execute WHERE condition filtering\n");
        }
        
        if (strstr(sql_upper, "ORDER BY") != NULL) {
            strcat(explanation, "2. Sort the results\n");
        }
        
        strcat(explanation, "3. Return query results");
        
        if (table != NULL) {
            char temp[100];
            sprintf(temp, "\nExpected to process %d rows of data", table->row_count);
            strcat(explanation, temp);
        }
    } else {
        strcpy(explanation, "Unable to parse query type, please check SQL syntax");
    }

    return explanation;
}
