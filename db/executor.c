#include "executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



// 执行查询并返回结果
QueryResult* execute_query(Table* table, Query* query) {
    if (table == NULL || query == NULL) {
        return NULL;
    }

    QueryResult* result = create_query_result();
    if (result == NULL) {
        return NULL;
    }

    Table* temp_table = table;

    // Execute WHERE condition filtering
    if (query->where_conditions != NULL) {
        temp_table = filter_rows(temp_table, query->where_conditions);
        if (temp_table == NULL) {
            strcpy(result->message, "Filter condition execution failed");
            result->success = 0;
            return result;
        }
    }

    // Execute column selection
    // column_count=0 means SELECT *
    if (query->column_count > 0 || query->column_count == 0) { 
        temp_table = select_columns(temp_table, query);
        if (temp_table == NULL) {
            strcpy(result->message, "Column selection execution failed");
            result->success = 0;
            return result;
        }
    }

    // Execute sorting
    if (strlen(query->order_by) > 0) {
        temp_table = sort_table(temp_table, query->order_by, query->sort_dir);
        if (temp_table == NULL) {
            strcpy(result->message, "Sort execution failed");
            result->success = 0;
            return result;
        }
    }

    result->result_table = temp_table;
    result->affected_rows = temp_table->row_count;
    result->success = 1;
    sprintf(result->message, "Query successful, returned %d rows", result->affected_rows);

    return result;
}



// 执行条件并返回过滤后的表
int evaluate_condition(const Table* table, int row, const Condition* condition) {
    if (table == NULL || condition == NULL || row < 0 || row >= table->row_count) {
        return 0;
    }

    int col_index = get_column_index(table, condition->column);
    if (col_index == -1) {
        return 0;
    }

    const char* cell_value = table->data[row][col_index];
    if (cell_value == NULL) {
        return 0;
    }

    switch (condition->op) {
        case OP_EQUAL:
            return strcmp(cell_value, condition->value) == 0;
        case OP_NOT_EQUAL:
            return strcmp(cell_value, condition->value) != 0;
        case OP_GREATER:
            // 数值比较
            if (table->columns[col_index].type == TYPE_INT || table->columns[col_index].type == TYPE_FLOAT) {
                double cell_num = atof(cell_value);
                double cond_num = atof(condition->value);
                return cell_num > cond_num;
            } else {
                // 字符串比较
                return strcmp(cell_value, condition->value) > 0;
            }
        case OP_LESS:
            if (table->columns[col_index].type == TYPE_INT || table->columns[col_index].type == TYPE_FLOAT) {
                double cell_num = atof(cell_value);
                double cond_num = atof(condition->value);
                return cell_num < cond_num;
            } else {
                return strcmp(cell_value, condition->value) < 0;
            }
        case OP_GREATER_EQUAL:
            if (table->columns[col_index].type == TYPE_INT || table->columns[col_index].type == TYPE_FLOAT) {
                double cell_num = atof(cell_value);
                double cond_num = atof(condition->value);
                return cell_num >= cond_num;
            } else {
                return strcmp(cell_value, condition->value) >= 0;
            }
        case OP_LESS_EQUAL:
            if (table->columns[col_index].type == TYPE_INT || table->columns[col_index].type == TYPE_FLOAT) {
                double cell_num = atof(cell_value);
                double cond_num = atof(condition->value);
                return cell_num <= cond_num;
            } else {
                return strcmp(cell_value, condition->value) <= 0;
            }
        case OP_LIKE:
            // 简单的LIKE模式匹配
            return strstr(cell_value, condition->value) != NULL;
        default:
            return 0;
    }
}




// NULL
Table* select_columns(const Table* table, const Query* query) {
    if (table == NULL || query == NULL) {
        return NULL;
    }

    // 如果column_count为0，表示SELECT *，选择所有列
    int selected_col_count = (query->column_count == 0) ? table->col_count : query->column_count;
    const char** selected_cols = malloc(selected_col_count * sizeof(char*));
    
    if (selected_cols == NULL) {
        return NULL;
    }

    if (query->column_count == 0) {
        // SELECT * 的情况
        for (int i = 0; i < table->col_count; i++) {
            selected_cols[i] = table->columns[i].name;
        }
    } else {
        // 选择特定列
        for (int i = 0; i < query->column_count; i++) {
            selected_cols[i] = query->columns[i];
        }
    }





    // 创建新表
    Table* result_table = create_table("query_result", selected_col_count, selected_cols);
    free(selected_cols);

    if (result_table == NULL) {
        return NULL;
    }

    // 复制数据
    for (int row = 0; row < table->row_count; row++) {
        const char** row_data = malloc(selected_col_count * sizeof(char*));
        if (row_data == NULL) {
            free_table(result_table);
            return NULL;
        }

        for (int col = 0; col < selected_col_count; col++) {
            int src_col_index;
            if (query->column_count == 0) 
            {
                // SELECT * 的情况
                src_col_index = col; 
            } 
            else 
            {
                src_col_index = get_column_index(table, query->columns[col]);
            }
            
            if (src_col_index != -1 && table->data[row][src_col_index] != NULL) 
            {
                row_data[col] = table->data[row][src_col_index];
            } 
            else 
            {
                row_data[col] = "";
            }
        }

        if (add_row(result_table, row_data) != 0) 
        {
            free(row_data);
            free_table(result_table);
            return NULL;
        }
        free(row_data);
    }

    return result_table;
}




// 过滤表的行
Table* filter_rows(const Table* table, const Condition* conditions) 
{
    if (table == NULL || conditions == NULL) 
    {
        return NULL;
    }

    // 创建新表
    Table* result_table = create_table("filtered_result", table->col_count, NULL);
    if (result_table == NULL) 
    {
        return NULL;
    }

    // 复制列信息
    for (int i = 0; i < table->col_count; i++) 
    {
        strcpy(result_table->columns[i].name, table->columns[i].name);
        result_table->columns[i].type = table->columns[i].type;
    }
    result_table->col_count = table->col_count;

    // 应用过滤条件
    for (int row = 0; row < table->row_count; row++) 
    {
        int match = 1;
        const Condition* current_condition = conditions;
        
        // 检查所有AND条件
        while (current_condition != NULL && match) 
        {
            if (!evaluate_condition(table, row, current_condition)) 
            {
                match = 0;
            }
            current_condition = current_condition->next;
        }

        if (match) 
        {
            // 复制匹配的行
            const char** row_data = malloc(table->col_count * sizeof(char*));
            if (row_data == NULL) 
            {
                free_table(result_table);
                return NULL;
            }

            for (int col = 0; col < table->col_count; col++) 
            {
                if (table->data[row][col] != NULL) 
                {
                    row_data[col] = table->data[row][col];
                } else {
                    row_data[col] = "";
                }
            }

            if (add_row(result_table, row_data) != 0)
             {
                free(row_data);
                free_table(result_table);
                return NULL;
            }
            free(row_data);
        }
    }

    return result_table;
}




// 对表进行排序
Table* sort_table(const Table* table, const char* column, SortDirection direction) {
    if (table == NULL || column == NULL) {
        return NULL;
    }

    int col_index = get_column_index(table, column);
    if (col_index == -1) {
        return NULL;
    }

    // 创建新表（复制原表）
    Table* result_table = create_table("sorted_result", table->col_count, NULL);
    if (result_table == NULL) {
        return NULL;
    }

    // 复制列信息
    for (int i = 0; i < table->col_count; i++) {
        strcpy(result_table->columns[i].name, table->columns[i].name);
        result_table->columns[i].type = table->columns[i].type;
    }
    result_table->col_count = table->col_count;

    // 复制所有数据
    for (int row = 0; row < table->row_count; row++) {
        const char** row_data = malloc(table->col_count * sizeof(char*));
        if (row_data == NULL) {
            free_table(result_table);
            return NULL;
        }

        for (int col = 0; col < table->col_count; col++) {
            if (table->data[row][col] != NULL) {
                row_data[col] = table->data[row][col];
            } else {
                row_data[col] = "";
            }
        }

        if (add_row(result_table, row_data) != 0) {
            free(row_data);
            free_table(result_table);
            return NULL;
        }
        free(row_data);
    }


    // 简单的冒泡排序
    for (int i = 0; i < result_table->row_count - 1; i++) {
        for (int j = 0; j < result_table->row_count - i - 1; j++) {
            const char* value1 = result_table->data[j][col_index];
            const char* value2 = result_table->data[j + 1][col_index];
            
            int should_swap = 0;
            
            if (table->columns[col_index].type == TYPE_INT || table->columns[col_index].type == TYPE_FLOAT) {
                double num1 = atof(value1);
                double num2 = atof(value2);
                if (direction == SORT_ASC) {
                    should_swap = num1 > num2;
                } else {
                    should_swap = num1 < num2;
                }
            } 
            else 
            {
                // 字符串比较
                if (direction == SORT_ASC) {
                    should_swap = strcmp(value1, value2) > 0;
                } 
                else 
                {
                    should_swap = strcmp(value1, value2) < 0;
                }
            }

            if (should_swap) {
                // 交换行数据
                char** temp = result_table->data[j];
                result_table->data[j] = result_table->data[j + 1];
                result_table->data[j + 1] = temp;
            }
        }
    }

    return result_table;
}
