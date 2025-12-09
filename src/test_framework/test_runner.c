#include "test_runner.h"
#include "test_loader.h"
#include <time.h>
#include "../db/csv_loader.h"
#include "../db/parser.h"
#include "../db/executor.h"

// 函数声明
int run_data_load_test(TestCase* test_case);
int run_sql_query_test(TestCase* test_case, Table* data_table);
int run_functional_test(TestCase* test_case, Table* data_table);
int run_performance_test(TestCase* test_case, Table* data_table);

int run_test_suite(const char* test_file) {
    if (test_file == NULL) {
        printf("Test file path is empty\n");
        return -1;
    }

    TestSuite* suite = load_test_suite_from_file(test_file);
    if (suite == NULL) {
        printf("Cannot load test suite: %s\n", test_file);
        return -1;
    }

    printf("Running test suite: %s\n", suite->name);
    printf("Number of test cases: %d\n", suite->test_count);

    for (int i = 0; i < suite->test_count; i++) {
        TestCase* test_case = &suite->test_cases[i];
        printf("\nRunning test: %s\n", test_case->name);
        
        Table* data_table = NULL;
        
        // Load data file (if needed)
        if (strlen(test_case->data_file) > 0) {
            char data_path[256];
            snprintf(data_path, sizeof(data_path), "data/%s", test_case->data_file);
            data_table = load_csv(data_path);
            if (data_table == NULL) {
                test_case->status = TEST_ERROR;
                strcpy(test_case->error_message, "Data file loading failed");
                suite->failed_count++;
                continue;
            }
        }

        clock_t start = clock();
        int result = run_test_case(test_case, data_table);
        clock_t end = clock();
        
        test_case->execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        suite->total_time += test_case->execution_time;

        if (data_table != NULL) {
            free_table(data_table);
        }

        if (result == 0) {
            test_case->status = TEST_PASSED;
            suite->passed_count++;
            printf("[PASS] Test passed (%.3fs)\n", test_case->execution_time);
        } else {
            test_case->status = TEST_FAILED;
            suite->failed_count++;
            printf("[FAIL] Test failed: %s (%.3fs)\n", test_case->error_message, test_case->execution_time);
        }
    }

    // Generate test report
    printf("\n=== Test Results Summary ===\n");
    printf("Total test cases: %d\n", suite->test_count);
    printf("Passed: %d\n", suite->passed_count);
    printf("Failed: %d\n", suite->failed_count);
    printf("Skipped: %d\n", suite->skipped_count);
    printf("Total execution time: %.3fs\n", suite->total_time);

    int overall_result = (suite->failed_count == 0) ? 0 : 1;
    free_test_suite(suite);
    cleanup_test_resources();
    
    return overall_result;
}

int run_test_case(TestCase* test_case, Table* data_table) {
    if (test_case == NULL) {
        return -1;
    }

    switch (test_case->type) {
        case TEST_DATA_LOAD:
            return run_data_load_test(test_case);
        case TEST_SQL_QUERY:
            return run_sql_query_test(test_case, data_table);
        case TEST_FUNCTIONAL:
            return run_functional_test(test_case, data_table);
        case TEST_PERFORMANCE:
            return run_performance_test(test_case, data_table);
        default:
            strcpy(test_case->error_message, "Unknown test type");
            return -1;
    }
}

int run_data_load_test(TestCase* test_case) {
    if (strlen(test_case->data_file) == 0) {
        strcpy(test_case->error_message, "Data file not specified");
        return -1;
    }

    char data_path[256];
    snprintf(data_path, sizeof(data_path), "data/%s", test_case->data_file);
    
    Table* table = load_csv(data_path);
    if (table == NULL) {
        strcpy(test_case->error_message, "CSV file loading failed");
        return -1;
    }

    int result = 0;
    if (test_case->expected_row_count >= 0 && table->row_count != test_case->expected_row_count) {
        sprintf(test_case->error_message, "Expected %d rows, got %d rows", 
                test_case->expected_row_count, table->row_count);
        result = -1;
    }

    free_table(table);
    return result;
}

int run_sql_query_test(TestCase* test_case, Table* data_table) {
    if (data_table == NULL) {
        strcpy(test_case->error_message, "Data table not loaded");
        return -1;
    }

    if (strlen(test_case->sql_query) == 0) {
        strcpy(test_case->error_message, "SQL query is empty");
        return -1;
    }

    Query* query = parse_query(test_case->sql_query);
    if (query == NULL) {
        strcpy(test_case->error_message, "SQL parsing failed");
        return -1;
    }

    QueryResult* result = execute_query(data_table, query);
    if (result == NULL || !result->success) {
        strcpy(test_case->error_message, "Query execution failed");
        free_query(query);
        return -1;
    }

    int test_result = verify_test_result(test_case, result->result_table);
    
    free_query_result(result);
    free_query(query);
    return test_result;
}

int run_functional_test(TestCase* test_case, Table* data_table) {
    // Simple implementation of functional test
    // In real projects, more complex functional validation can be added here
    strcpy(test_case->error_message, "Functional test not implemented");
    return -1;
}

int run_performance_test(TestCase* test_case, Table* data_table) {
    // Simple implementation of performance test
    // In real projects, performance benchmarks can be added here
    strcpy(test_case->error_message, "Performance test not implemented");
    return -1;
}

int verify_test_result(TestCase* test_case, Table* result_table) {
    if (result_table == NULL) {
        strcpy(test_case->error_message, "Result table is empty");
        return -1;
    }

    // Validate row count
    if (test_case->expected_row_count >= 0 && 
        result_table->row_count != test_case->expected_row_count) {
        sprintf(test_case->error_message, "Row count mismatch: expected %d, got %d",
                test_case->expected_row_count, result_table->row_count);
        return -1;
    }

    // Validate column count (if expected columns are specified)
    if (test_case->expected_column_count > 0 && 
        result_table->col_count != test_case->expected_column_count) {
        sprintf(test_case->error_message, "Column count mismatch: expected %d, got %d",
                test_case->expected_column_count, result_table->col_count);
        return -1;
    }

    return 0;
}

void cleanup_test_resources() {
    // Clean up resources that may have been created during testing
    // Currently no special resources need to be cleaned up
}
