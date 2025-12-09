#include "csv_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define INITIAL_CAPACITY 100

Table* load_csv(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return NULL;
    }

    char buffer[BUFFER_SIZE];
    
    // Read header
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fclose(file);
        printf("File is empty or read failed\n");
        return NULL;
    }

    // Parse column names
    char* token;
    char* col_names[MAX_COLUMNS];
    int col_count = 0;
    
    // Remove newline characters
    buffer[strcspn(buffer, "\r\n")] = 0;
    
    token = strtok(buffer, ",");
    while (token != NULL && col_count < MAX_COLUMNS) {
        // Remove leading and trailing spaces
        while (*token == ' ') token++;
        char* end = token + strlen(token) - 1;
        while (end > token && *end == ' ') end--;
        *(end + 1) = '\0';
        
        col_names[col_count] = malloc(strlen(token) + 1);
        strcpy(col_names[col_count], token);
        col_count++;
        
        token = strtok(NULL, ",");
    }

    // Create table
    char table_name[100];
    const char* last_slash = strrchr(filename, '/');
    const char* table_name_start = (last_slash != NULL) ? last_slash + 1 : filename;
    strncpy(table_name, table_name_start, sizeof(table_name) - 1);
    char* dot = strrchr(table_name, '.');
    if (dot != NULL) *dot = '\0';
    
    Table* table = create_table(table_name, col_count, (const char**)col_names);
    
    // Free column names memory
    for (int i = 0; i < col_count; i++) {
        free(col_names[i]);
    }

    if (table == NULL) {
        fclose(file);
        return NULL;
    }

    // Read data rows
    int row_count = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Remove newline characters
        buffer[strcspn(buffer, "\r\n")] = 0;
        
        if (strlen(buffer) == 0) continue;
        
        char* row_data[MAX_COLUMNS];
        int data_count = 0;
        
        token = strtok(buffer, ",");
        while (token != NULL && data_count < col_count) {
            // Remove leading and trailing spaces
            while (*token == ' ') token++;
            char* end = token + strlen(token) - 1;
            while (end > token && *end == ' ') end--;
            *(end + 1) = '\0';
            
            row_data[data_count] = token;
            data_count++;
            
            token = strtok(NULL, ",");
        }
        
        if (data_count == col_count) {
            if (add_row(table, (const char**)row_data) != 0) {
                printf("Failed to add row\n");
            } else {
                row_count++;
            }
        } else {
            printf("Skipping incomplete data row: %s\n", buffer);
        }
    }

    fclose(file);
    
    // Infer column types
    infer_column_types(table);
    
    printf("Successfully loaded %d rows of data\n", row_count);
    return table;
}

int detect_data_type(const char* value) {
    if (value == NULL || strlen(value) == 0) {
        return TYPE_UNKNOWN;
    }
    
    // Check if it's an integer
    char* end;
    strtol(value, &end, 10);
    if (*end == '\0') {
        return TYPE_INT;
    }
    
    // Check if it's a float
    strtod(value, &end);
    if (*end == '\0') {
        return TYPE_FLOAT;
    }
    
    // Default to string
    return TYPE_STRING;
}

void infer_column_types(Table* table) {
    if (table == NULL || table->row_count == 0) {
        return;
    }
    
    for (int col = 0; col < table->col_count; col++) {
        DataType detected_type = TYPE_UNKNOWN;
        
        // Check first few rows to infer type
        for (int row = 0; row < table->row_count && row < 10; row++) {
            if (table->data[row][col] != NULL) {
                DataType current_type = detect_data_type(table->data[row][col]);
                
                if (detected_type == TYPE_UNKNOWN) {
                    detected_type = current_type;
                } else if (detected_type != current_type) {
                    // If types are inconsistent, use more general type
                    if (detected_type == TYPE_INT && current_type == TYPE_FLOAT) {
                        detected_type = TYPE_FLOAT;
                    } else if (detected_type != TYPE_STRING) {
                        detected_type = TYPE_STRING;
                    }
                }
            }
        }
        
        // If no type detected, default to string
        if (detected_type == TYPE_UNKNOWN) {
            detected_type = TYPE_STRING;
        }
        
        table->columns[col].type = detected_type;
    }
}
