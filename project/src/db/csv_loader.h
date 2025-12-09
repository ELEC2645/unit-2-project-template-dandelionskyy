#ifndef CSV_LOADER_H
#define CSV_LOADER_H

#include "table.h"

// CSV加载函数
Table* load_csv(const char* filename);
int detect_data_type(const char* value);
void infer_column_types(Table* table);

#endif // CSV_LOADER_H
