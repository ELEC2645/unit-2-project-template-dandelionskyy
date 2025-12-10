#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




Operator parse_operator(const char* op_str) {
    if (strcmp(op_str, "=") == 0) return OP_EQUAL;
    if (strcmp(op_str, "!=") == 0) return OP_NOT_EQUAL;
    if (strcmp(op_str, ">") == 0) return OP_GREATER;
    if (strcmp(op_str, "<") == 0) return OP_LESS;
    if (strcmp(op_str, ">=") == 0) return OP_GREATER_EQUAL;
    if (strcmp(op_str, "<=") == 0) return OP_LESS_EQUAL;
    if (strcmp(op_str, "LIKE") == 0) return OP_LIKE;
    return OP_EQUAL; // 默认
}

QueryType parse_query_type(const char* sql) {
    char sql_upper[512];
    strncpy(sql_upper, sql, sizeof(sql_upper) - 1);
    sql_upper[sizeof(sql_upper) - 1] = '\0';
    
    for (char* p = sql_upper; *p; p++) {
        *p = toupper(*p);
    }
    
    if (strstr(sql_upper, "SELECT") != NULL) return QUERY_SELECT;
    if (strstr(sql_upper, "WHERE") != NULL) return QUERY_FILTER;
    if (strstr(sql_upper, "COUNT") != NULL || strstr(sql_upper, "SUM") != NULL || 
        strstr(sql_upper, "AVG") != NULL || strstr(sql_upper, "MAX") != NULL || 
        strstr(sql_upper, "MIN") != NULL) return QUERY_AGGREGATE;
    if (strstr(sql_upper, "ORDER BY") != NULL) return QUERY_SORT;
    
    return QUERY_SELECT; // 默认
}

void free_condition(Condition* condition) {
    while (condition != NULL) {
        Condition* next = condition->next;
        free(condition);
        condition = next;
    }
}





// 定义常量
Query* parse_query(const char* sql) {
    if (sql == NULL || strlen(sql) == 0) {
        return NULL;
    }

    Query* query = create_query();
    if (query == NULL) {
        return NULL;
    }

    // 简单的SQL解析实现
    // 这里只实现基本的SELECT查询解析
    char sql_copy[1000];
    strncpy(sql_copy, sql, sizeof(sql_copy) - 1);
    sql_copy[sizeof(sql_copy) - 1] = '\0';

    // 转换为大写便于解析
    for (char* p = sql_copy; *p; p++) {
        *p = toupper(*p);
    }

    // 检测查询类型
    if (strstr(sql_copy, "SELECT") != NULL) {
        query->type = QUERY_SELECT;
        
        // 简单的列解析
        char* select_start = strstr(sql_copy, "SELECT");
        char* from_start = strstr(sql_copy, "FROM");
        
        if (from_start != NULL) {
            // 解析列名
            char columns_part[200];
            strncpy(columns_part, select_start + 6, from_start - select_start - 6);
            columns_part[from_start - select_start - 6] = '\0';
            
            // 去除空格
            char* col_token = strtok(columns_part, ",");
            while (col_token != NULL && query->column_count < MAX_COLUMNS) {
                // 去除前后空格
                while (*col_token == ' ') col_token++;
                char* end = col_token + strlen(col_token) - 1;
                while (end > col_token && *end == ' ') end--;
                *(end + 1) = '\0';
                
                if (strcmp(col_token, "*") == 0) {
                    // 选择所有列
                    query->column_count = 0; // 特殊标记表示选择所有列
                    break;
                } else {
                    strncpy(query->columns[query->column_count], col_token, MAX_COLUMN_NAME_LEN - 1);
                    query->column_count++;
                }
                
                col_token = strtok(NULL, ",");
            }
            
            // 解析表名
            char* where_start = strstr(from_start, "WHERE");
            char table_part[500];
            
            if (where_start != NULL) {
                strncpy(table_part, from_start + 4, where_start - from_start - 4);
            } else {
                strncpy(table_part, from_start + 4, sizeof(table_part) - 1);
            }
            table_part[sizeof(table_part) - 1] = '\0';
            
            // 去除空格
            char* table_name = table_part;
            while (*table_name == ' ') table_name++;
            char* table_end = table_name + strlen(table_name) - 1;
            while (table_end > table_name && *table_end == ' ') table_end--;
            *(table_end + 1) = '\0';
            
            strncpy(query->table_name, table_name, sizeof(query->table_name) - 1);
            
            // 解析WHERE条件
            if (where_start != NULL) {
                char* condition_str = where_start + 5;
                while (*condition_str == ' ') condition_str++;
                
                // 简单的条件解析
                char condition[500];
                strncpy(condition, condition_str, sizeof(condition) - 1);
                condition[sizeof(condition) - 1] = '\0';
                
                // 检查是否包含AND/OR（当前不支持）
                if (strstr(condition, " AND ") != NULL || strstr(condition, " OR ") != NULL) {
                    // 不支持多个条件，返回解析错误
                    free_query(query);
                    return NULL;
                }
                
                // 查找操作符
                char* op_pos = NULL;
                char* operators[] = {"=", "!=", ">", "<", ">=", "<=", "LIKE"};
                Operator ops[] = {OP_EQUAL, OP_NOT_EQUAL, OP_GREATER, OP_LESS, 
                                 OP_GREATER_EQUAL, OP_LESS_EQUAL, OP_LIKE};
                
                for (int i = 0; i < 7; i++) {
                    op_pos = strstr(condition, operators[i]);
                    if (op_pos != NULL) {
                        Condition* cond = malloc(sizeof(Condition));
                        if (cond != NULL) {
                            // 提取列名
                            strncpy(cond->column, condition, op_pos - condition);
                            cond->column[op_pos - condition] = '\0';
                            
                            // 去除列名前后空格
                            char* col_start = cond->column;
                            while (*col_start == ' ') col_start++;
                            char* col_end = col_start + strlen(col_start) - 1;
                            while (col_end > col_start && *col_end == ' ') col_end--;
                            *(col_end + 1) = '\0';
                            if (col_start != cond->column) {
                                memmove(cond->column, col_start, strlen(col_start) + 1);
                            }
                            
                            // 设置操作符
                            cond->op = ops[i];
                            
                            // 提取值
                            char* value_start = op_pos + strlen(operators[i]);
                            while (*value_start == ' ') value_start++;
                            
                            // 去除值的前后空格和引号
                            char* value_end = value_start + strlen(value_start) - 1;
                            while (value_end > value_start && (*value_end == ' ' || *value_end == '\'' || *value_end == '"')) value_end--;
                            *(value_end + 1) = '\0';
                            
                            if (*value_start == '\'' || *value_start == '"') {
                                value_start++;
                            }
                            
                            strncpy(cond->value, value_start, MAX_CELL_LEN - 1);
                            cond->next = NULL;
                            
                            query->where_conditions = cond;
                        }
                        break;
                    }
                }
            }
        }
    } 
    else 
    {
        // 不支持其他查询类型
        free_query(query);
        return NULL;
    }

    return query;
}