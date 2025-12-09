#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLUMNS 50
#define MAX_COLUMN_NAME_LEN 50
#define MAX_CELL_LEN 100
#define INITIAL_CAPACITY 100

// 列数据类型枚举
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_UNKNOWN
} DataType;

// 列定义
typedef struct {
    char name[MAX_COLUMN_NAME_LEN];
    DataType type;
} Column;

// 表格结构
typedef struct {
    char name[100];
    Column columns[MAX_COLUMNS];
    int col_count;
    char*** data;  // 三维数组: data[row][col][cell]
    int row_count;
    int capacity;
} Table;

// 查询类型枚举
typedef enum {
    QUERY_SELECT,
    QUERY_FILTER,
    QUERY_AGGREGATE,
    QUERY_SORT
} QueryType;

// 聚合函数类型
typedef enum {
    AGG_NONE,
    AGG_COUNT,
    AGG_SUM,
    AGG_AVG,
    AGG_MAX,
    AGG_MIN
} AggregateType;

// 排序方向
typedef enum {
    SORT_ASC,
    SORT_DESC
} SortDirection;

// 条件操作符
typedef enum {
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_GREATER_EQUAL,
    OP_LESS_EQUAL,
    OP_LIKE
} Operator;

// 条件结构
typedef struct Condition {
    char column[MAX_COLUMN_NAME_LEN];
    Operator op;
    char value[MAX_CELL_LEN];
    struct Condition* next;  // 用于AND条件链
} Condition;

// 查询结构
typedef struct {
    QueryType type;
    char table_name[100];
    char columns[MAX_COLUMNS][MAX_COLUMN_NAME_LEN];
    int column_count;
    Condition* where_conditions;
    char group_by[MAX_COLUMN_NAME_LEN];
    AggregateType aggregate;
    char aggregate_column[MAX_COLUMN_NAME_LEN];
    char order_by[MAX_COLUMN_NAME_LEN];
    SortDirection sort_dir;
    int limit;
} Query;

// 查询结果结构
typedef struct {
    Table* result_table;
    int affected_rows;
    char message[256];
    int success;
} QueryResult;

// 表格操作函数
Table* create_table(const char* name, int col_count, const char** col_names);
void free_table(Table* table);
int add_row(Table* table, const char** row_data);
void print_table(const Table* table);
int get_column_index(const Table* table, const char* column_name);

// 查询相关函数
Query* create_query();
void free_query(Query* query);
QueryResult* create_query_result();
void free_query_result(QueryResult* result);
void print_query_result(const QueryResult* result);

#endif // TABLE_H
