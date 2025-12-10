#include "result.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <string.h>
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif


// 定义常量
void print_query_result(const QueryResult* result) {
    if (result == NULL) {
        printf("Query result is empty\n");
        return;
    }

    if (!result->success) {
        printf("Query failed: %s\n", result->message);
        return;
    }
    
    printf("Query result: %s\n", result->message);
    
    if (result->result_table != NULL) {
        print_table(result->result_table);
    }
}





void export_result_to_csv(const QueryResult* result, const char* filename) {
    if (result == NULL || filename == NULL || !result->success || result->result_table == NULL) {
        printf("Cannot export result\n");
        return;
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Cannot create file: %s\n", filename);
        return;
    }
    //test4
    Table* table = result->result_table;
    
    // Write header
    for (int i = 0; i < table->col_count; i++) {
        fprintf(file, "%s", table->columns[i].name);
        if (i < table->col_count - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");

    // Write data
    for (int row = 0; row < table->row_count; row++) {
        for (int col = 0; col < table->col_count; col++) {
            if (table->data[row][col] != NULL) {
                // If contains comma or quote, wrap with quotes
                if (strchr(table->data[row][col], ',') != NULL || strchr(table->data[row][col], '"') != NULL) {
                    fprintf(file, "\"");
                    // Escape quotes
                    const char* p = table->data[row][col];
                    while (*p) {
                        if (*p == '"') {
                            fprintf(file, "\"\"");
                        } else {
                            fprintf(file, "%c", *p);
                        }
                        p++;
                    }
                    fprintf(file, "\"");
                } else {
                    fprintf(file, "%s", table->data[row][col]);
                }
            }
            
            if (col < table->col_count - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    //test2
    fclose(file);
    printf("Result exported to: %s\n", filename);
}

// 表格操作函数的实现
Table* create_table(const char* name, int col_count, const char** col_names) {
    Table* table = malloc(sizeof(Table));
    if (table == NULL) {
        return NULL;
    }

    strncpy(table->name, name, sizeof(table->name) - 1);
    table->name[sizeof(table->name) - 1] = '\0';
    //test1
    table->col_count = col_count;
    table->row_count = 0;
    table->capacity = INITIAL_CAPACITY;

    // 分配数据存储空间
    table->data = malloc(table->capacity * sizeof(char**));
    if (table->data == NULL) {
        free(table);
        return NULL;
    }

    for (int i = 0; i < table->capacity; i++) {
        table->data[i] = malloc(col_count * sizeof(char*));
        if (table->data[i] == NULL) {
            // 清理已分配的内存
            for (int j = 0; j < i; j++) {
                free(table->data[j]);
            }
            free(table->data);
            free(table);
            return NULL;
        }
        
        // 初始化指针为NULL
        for (int j = 0; j < col_count; j++) {
            table->data[i][j] = NULL;
        }
    }

    // 设置列名
    if (col_names != NULL) {
        for (int i = 0; i < col_count; i++) {
            strncpy(table->columns[i].name, col_names[i], MAX_COLUMN_NAME_LEN - 1);
            table->columns[i].name[MAX_COLUMN_NAME_LEN - 1] = '\0';
            table->columns[i].type = TYPE_UNKNOWN;
        }
    }

    return table;
}








// 添加行的函数实现
int add_row(Table* table, const char** row_data) {
    if (table == NULL || row_data == NULL) {
        return -1;
    }

    // 检查是否需要扩容
    if (table->row_count >= table->capacity) {
        int new_capacity = table->capacity * 2;
        char*** new_data = realloc(table->data, new_capacity * sizeof(char**));
        if (new_data == NULL) {
            return -1;
        }
        
        table->data = new_data;
        
        // 分配新的行空间
        for (int i = table->capacity; i < new_capacity; i++) {
            table->data[i] = malloc(table->col_count * sizeof(char*));
            if (table->data[i] == NULL) {
                return -1;
            }
            
            // 初始化指针为NULL
            for (int j = 0; j < table->col_count; j++) {
                table->data[i][j] = NULL;
            }
        }
        
        table->capacity = new_capacity;
    }

    // 复制行数据
    for (int i = 0; i < table->col_count; i++) {
        if (row_data[i] != NULL) {
            table->data[table->row_count][i] = malloc(strlen(row_data[i]) + 1);
            if (table->data[table->row_count][i] == NULL) {
                // 清理已分配的内存
                for (int j = 0; j < i; j++) {
                    free(table->data[table->row_count][j]);
                }
                return -1;
            }
            strcpy(table->data[table->row_count][i], row_data[i]);
        } else {
            table->data[table->row_count][i] = NULL;
        }
    }
    //test3
    table->row_count++;
    return 0;
}

void print_table(const Table* table) {
    if (table == NULL) {
        printf("Table is empty\n");
        return;
    }

    printf("\nTable: %s\n", table->name);
    printf("Rows: %d, Columns: %d\n\n", table->row_count, table->col_count);

    // 计算每列的最大宽度
    int col_widths[MAX_COLUMNS] = {0};
    
    for (int i = 0; i < table->col_count; i++) {
        col_widths[i] = strlen(table->columns[i].name);
    }
    // 计算每列的实际宽度
    for (int row = 0; row < table->row_count && row < 20; row++) { // 限制显示前20行
        for (int col = 0; col < table->col_count; col++) {
            if (table->data[row][col] != NULL) {
                int len = strlen(table->data[row][col]);
                if (len > col_widths[col]) {
                    col_widths[col] = len;
                }
            }
        }
    }

    // 打印表头
    for (int i = 0; i < table->col_count; i++) {
        printf("%-*s", col_widths[i] + 2, table->columns[i].name);
    }
    printf("\n");

    // 打印分隔线
    for (int i = 0; i < table->col_count; i++) {
        for (int j = 0; j < col_widths[i] + 2; j++) {
            printf("-");
        }
    }
    printf("\n");

    // 打印数据（限制显示前20行）
    // 限制显示前20行
    int display_rows = (table->row_count > 20) ? 20 : table->row_count;
    for (int row = 0; row < display_rows; row++) {
        for (int col = 0; col < table->col_count; col++) {
            if (table->data[row][col] != NULL) {
                printf("%-*s", col_widths[col] + 2, table->data[row][col]);
            } else {
                printf("%-*s", col_widths[col] + 2, "NULL");
            }
        }
        printf("\n");
    }

    if (table->row_count > 20) {
        printf("\n... %d more rows not displayed\n", table->row_count - 20);
    }
    printf("\n");
}




// 获取列索引的函数实现
int get_column_index(const Table* table, const char* column_name) {
    if (table == NULL || column_name == NULL) {
        return -1;
    }
    // 检查列名
    for (int i = 0; i < table->col_count; i++) {
        if (strcasecmp(table->columns[i].name, column_name) == 0) {
            return i;
        }
    }

    return -1;
}

// 查询相关函数的实现
Query* create_query() {
    Query* query = malloc(sizeof(Query));
    if (query == NULL) {
        return NULL;
    }

    // 初始化默认值
    query->type = QUERY_SELECT;
    query->table_name[0] = '\0';
    query->column_count = 0;
    query->where_conditions = NULL;
    query->group_by[0] = '\0';
    query->aggregate = AGG_NONE;
    query->aggregate_column[0] = '\0';
    query->order_by[0] = '\0';
    query->sort_dir = SORT_ASC;
    query->limit = -1;

    return query;
}



// 释放查询条件的内存
void free_query(Query* query) {
    if (query == NULL) {
        return;
    }

    free_condition(query->where_conditions);
    free(query);
}
// 释放条件链表的内存
QueryResult* create_query_result() {
    QueryResult* result = malloc(sizeof(QueryResult));
    if (result == NULL) {
        return NULL;
    }

    result->result_table = NULL;
    result->affected_rows = 0;
    result->message[0] = '\0';
    result->success = 0;

    return result;
}
// 释放查询结果的内存
void free_query_result(QueryResult* result) {
    if (result == NULL) {
        return;
    }
    // 释放结果表格的内存
    if (result->result_table != NULL) {
        free_table(result->result_table);
    }
    free(result);
}






//finished
void free_table(Table* table) {
    if (table == NULL) {
        return;
    }

    if (table->data != NULL) {
        for (int i = 0; i < table->capacity; i++) {
            if (table->data[i] != NULL) {
                for (int j = 0; j < table->col_count; j++) {
                    if (table->data[i][j] != NULL) {
                        free(table->data[i][j]);
                    }
                }
                free(table->data[i]);
            }
        }
        free(table->data);
    }

    free(table);
}