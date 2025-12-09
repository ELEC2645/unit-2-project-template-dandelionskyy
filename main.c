#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#include <strings.h>
#endif
#include "db/parser.h"
#include "db/executor.h"
#include "db/csv_loader.h"
#include "test_framework/test_runner.h"
#include "ai/ai_helper.h"
#include "utils/string_utils.h"

// Global variables
Table* current_table = NULL;

// Function declarations
void handle_load_csv(void);
void handle_sql_query(void);
void handle_run_tests(void);
void handle_ai_assist(void);
void handle_professional_calc(void);
void cleanup(void);

// Menu handling functions (template style)
static void main_menu(void);
static void print_main_menu(void);
static int  get_user_input(void);
static void select_menu_item(int input);
static void go_back_to_main(void);

int main(void)
{
    printf("===============================================\n");
    printf("|                                             |\n");
    printf("|   ELECTRONIC COMPONENT DATABASE             |\n");
    printf("|        & CALCULATION SYSTEM                 |\n");
    printf("|                                             |\n");
    printf("|   Electronic Information Engineering        |\n");
    printf("|          Project Application                |\n");
    printf("|                                             |\n");
    printf("===============================================\n");
    printf("Lightweight SQL Database Engine v1.0\n\n");
    
    /* this will run forever until we call exit(0) in select_menu_item() */
    for(;;) {
        main_menu();
    }
    /* not reached */
    return 0;
}

static void main_menu(void)
{
    print_main_menu();
    {
        int input = get_user_input();
        select_menu_item(input);
    }
}

static int get_user_input(void)
{
    enum { MENU_ITEMS = 6 };   /* 1..5 = items, 6 = Exit */
    char buf[128];
    int valid_input = 0;
    int value = 0;

    do {
        printf("\nSelect item: ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            /* EOF or error; bail out gracefully */
            puts("\nInput error. Exiting.");
            exit(1);
        }

        // strip trailing newline
        buf[strcspn(buf, "\r\n")] = '\0';

        if (!is_integer(buf)) {
            printf("Enter an integer!\n");
            valid_input = 0;
        } else {
            value = (int)strtol(buf, NULL, 10);
            if (value >= 1 && value <= MENU_ITEMS) {
                valid_input = 1;
            } else {
                printf("Invalid menu item!\n");
                valid_input = 0;
            }
        }
    } while (!valid_input);

    return value;
}

static void select_menu_item(int input)
{
    switch (input) {
        case 1:
            handle_load_csv();
            go_back_to_main();
            break;
        case 2:
            handle_sql_query();
            go_back_to_main();
            break;
        case 3:
            handle_run_tests();
            go_back_to_main();
            break;
        case 4:
            handle_ai_assist();
            go_back_to_main();
            break;
        case 5:
            handle_professional_calc();
            go_back_to_main();
            break;
        default:
            printf("Thank you for using MiniDB-TestAI!\n");
            cleanup();
            exit(0);
    }
}

static void print_main_menu(void)
{
    printf("\n----------- Main menu -----------\n");
    printf("\n"
           "\t\t\t\t\t\t\n"
           "\t1. Load CSV File\t\t\n"
           "\t2. Execute SQL Query\t\t\n"
           "\t3. Run Automated Tests\t\t\n"
           "\t4. AI Assisted Query\t\t\n"
           "\t5. Professional Calculation Tools\t\t\n"
           "\t6. Exit System\t\t\t\t\n"
           "\t\t\t\t\t\t\n");
    printf("---------------------------------------------\n");
}

static void go_back_to_main(void)
{
    char buf[64];
    do {
        printf("\nEnter 'b' or 'B' to go back to main menu: ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput error. Exiting.");
            exit(1);
        }
        buf[strcspn(buf, "\r\n")] = '\0'; /* strip newline */
    } while (!(buf[0] == 'b' || buf[0] == 'B') || buf[1] != '\0');
}


void handle_load_csv() {
    // Show available CSV files
    printf("Available CSV files in data/ directory:\n");
    
    // List CSV files in data directory - cross-platform
    FILE* pipe;
#ifdef _WIN32
    pipe = _popen("dir /b data\\*.csv 2>nul", "r");
#else
    pipe = popen("ls data/*.csv 2>/dev/null", "r");
#endif
    if (pipe != NULL) {
        char buffer[256];
        int file_count = 0;
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            // Remove newline character
            buffer[strcspn(buffer, "\r\n")] = 0;
            if (strlen(buffer) > 0) {
                // Extract just the filename from path (for Unix)
#ifndef _WIN32
                char* slash = strrchr(buffer, '/');
                if (slash) {
                    printf("  - %s\n", slash + 1);
                } else {
                    printf("  - %s\n", buffer);
                }
#else
                printf("  - %s\n", buffer);
#endif
                file_count++;
            }
        }
#ifdef _WIN32
        _pclose(pipe);
#else
        pclose(pipe);
#endif
        
        if (file_count == 0) {
            printf("  No CSV files found in data/ directory\n");
        }
    } else {
        printf("  Unable to list CSV files\n");
    }
    printf("\n");
    
    char filename[100];
    printf("Please enter CSV filename (located in data/ directory): ");
    scanf("%99s", filename);
    getchar(); // Clear the newline character from buffer
    
    // Build full path
    char full_path[150];
    snprintf(full_path, sizeof(full_path), "data/%s", filename);
    
    if (current_table != NULL) {
        free_table(current_table);
    }
    
    current_table = load_csv(full_path);
    if (current_table != NULL) {
        printf("Successfully loaded table: %s\n", current_table->name);
        printf("Rows: %d, Columns: %d\n", current_table->row_count, current_table->col_count);
    } else {
        printf("Failed to load CSV file: %s\n", full_path);
    }
}

void handle_sql_query() {
    if (current_table == NULL) {
        printf("Error: Please load a CSV file first\n");
        return;
    }
    
    // Display available SQL commands with examples
    printf("\nAvailable SQL Commands with Examples:\n");
    printf("1. Basic SELECT query:\n");
    printf("   SELECT * FROM %s\n", current_table->name);
    printf("2. SELECT specific columns:\n");
    printf("   SELECT %s, %s FROM %s\n", 
           current_table->columns[0].name, 
           current_table->columns[1].name, 
           current_table->name);
    printf("3. SELECT with WHERE condition:\n");
    printf("   SELECT * FROM %s WHERE %s = 'John'\n", 
           current_table->name, current_table->columns[1].name);
    printf("   SELECT * FROM %s WHERE %s > 30\n", 
           current_table->name, current_table->columns[2].name);
    printf("4. SELECT with LIKE condition:\n");
    printf("   SELECT * FROM %s WHERE %s LIKE '%%J%%'\n", 
           current_table->name, current_table->columns[1].name);
    printf("\nImportant Notes:\n");
    printf("- Use single quotes for string values: 'John'\n");
    printf("- Use LIKE with %% for pattern matching: '%%J%%'\n");
    printf("- Supported operators: =, !=, >, <, >=, <=, LIKE\n");
    printf("- Currently supports only single WHERE condition (no AND/OR)\n");
    printf("- For multiple conditions, use separate queries\n");
    printf("\nAvailable columns: ");
    for (int i = 0; i < current_table->col_count; i++) {
        printf("%s", current_table->columns[i].name);
        if (i < current_table->col_count - 1) {
            printf(", ");
        }
    }
    printf("\n\n");
    
    char query[512];
    printf("Please enter SQL query: ");
    fgets(query, sizeof(query), stdin);
    trim_string(query);
    
    if (strlen(query) == 0) {
        printf("Query cannot be empty\n");
        return;
    }
    
    Query* parsed_query = parse_query(query);
    if (parsed_query == NULL) {
        printf("SQL syntax error in query: %s\n", query);
        return;
    }
    
    QueryResult* result = execute_query(current_table, parsed_query);
    if (result != NULL) {
        print_query_result(result);
        free_query_result(result);
    } else {
        printf("Query execution failed\n");
    }
    
    free_query(parsed_query);
}

void handle_run_tests() {
    printf("Running automated tests...\n");
    
    // Show available test files
    printf("Available test files in tests/ directory:\n");
    
    // List test files in tests directory - cross-platform
    FILE* pipe;
#ifdef _WIN32
    pipe = _popen("dir /b tests\\*.txt 2>nul", "r");
#else
    pipe = popen("ls tests/*.txt 2>/dev/null", "r");
#endif
    if (pipe != NULL) {
        char buffer[256];
        int file_count = 0;
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            // Remove newline character
            buffer[strcspn(buffer, "\r\n")] = 0;
            if (strlen(buffer) > 0) {
                // Extract just the filename from path (for Unix)
#ifndef _WIN32
                char* slash = strrchr(buffer, '/');
                if (slash) {
                    printf("  - %s\n", slash + 1);
                } else {
                    printf("  - %s\n", buffer);
                }
#else
                printf("  - %s\n", buffer);
#endif
                file_count++;
            }
        }
#ifdef _WIN32
        _pclose(pipe);
#else
        pclose(pipe);
#endif
        
        if (file_count == 0) {
            printf("  No test files found in tests/ directory\n");
        }
    } else {
        printf("  Unable to list test files\n");
    }
    printf("\n");
    
    char test_file[100];
    printf("Please enter test case file (located in tests/ directory): ");
    
    // 使用fgets代替scanf，更好地处理输入
    if (fgets(test_file, sizeof(test_file), stdin) == NULL) {
        printf("Input error\n");
        return;
    }
    
    // 去除换行符
    test_file[strcspn(test_file, "\r\n")] = '\0';
    
    // 如果输入为空，则返回
    if (strlen(test_file) == 0) {
        printf("No test file specified\n");
        return;
    }
    
    // Build full path
    char full_path[150];
    snprintf(full_path, sizeof(full_path), "tests/%s", test_file);
    
    int result = run_test_suite(full_path);
    if (result == 0) {
        printf("All tests passed!\n");
    } else {
        printf("Some tests failed\n");
    }
    
    // 调试信息：确认函数正常返回
    printf("DEBUG: handle_run_tests() completed, returning to go_back_to_main()\n");

    // **重要修改：清除输入缓冲区**
    // 确保在 go_back_to_main() 之前，输入流中没有残留的换行符或其他字符。
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void handle_ai_assist() {
    printf("AI Assistance Feature\n");
    printf("You can ask questions like:\n");
    printf("  - 'How to query all resistors?'\n");
    printf("  - 'Find capacitors under $1'\n");
    printf("  - 'Show me components from Texas Instruments'\n");
    printf("\nPlease enter your question or query requirement: ");
    
    char question[256];
    
    // 直接使用fgets，不先清除缓冲区
    if (fgets(question, sizeof(question), stdin) == NULL) {
        printf("Input error\n");
        return;
    }
    
    trim_string(question);
    
    if (strlen(question) == 0) {
        printf("Question cannot be empty\n");
        return;
    }
    
    char* suggestion = get_ai_suggestion(question, current_table);
    if (suggestion != NULL) {
        printf("\nAI Suggestion: %s\n", suggestion);
        free(suggestion);
    } else {
        printf("Unable to generate AI suggestion\n");
    }
}

void handle_professional_calc() {
    printf("=== Professional Calculation Tools ===\n");
    printf("Electronic Engineering Calculations\n\n");
    
    int calc_choice;
    printf("Available Calculations:\n");
    printf("1. Resistor Series Calculation\n");
    printf("2. Resistor Parallel Calculation\n");
    printf("3. Voltage Divider Calculation\n");
    printf("4. RC Time Constant Calculation\n");
    printf("5. Low Pass Filter Cutoff Frequency\n");
    printf("6. Return to Main Menu\n");
    printf("Please select calculation type (1-6): ");
    
    scanf("%d", &calc_choice);
    getchar(); // Clear buffer
    
    switch (calc_choice) {
        case 1: {
            // Resistor Series Calculation
            double r1, r2, r3 = 0;
            printf("Resistor Series Calculation\n");
            printf("Enter resistor values (Ohms). Enter 0 for unused resistors:\n");
            printf("R1 (Ohms): "); scanf("%lf", &r1);
            printf("R2 (Ohms): "); scanf("%lf", &r2);
            printf("R3 (Ohms): "); scanf("%lf", &r3);
            getchar(); // Clear buffer
            
            double total = r1 + r2 + r3;
            printf("Total series resistance: %.2f Ohms\n", total);
            break;
        }
        
        case 2: {
            // Resistor Parallel Calculation
            double r1, r2, r3 = 0;
            printf("Resistor Parallel Calculation\n");
            printf("Enter resistor values (Ohms). Enter 0 for unused resistors:\n");
            printf("R1 (Ohms): "); scanf("%lf", &r1);
            printf("R2 (Ohms): "); scanf("%lf", &r2);
            printf("R3 (Ohms): "); scanf("%lf", &r3);
            getchar(); // Clear buffer
            
            double total = 0;
            if (r1 > 0) total += 1.0 / r1;
            if (r2 > 0) total += 1.0 / r2;
            if (r3 > 0) total += 1.0 / r3;
            
            if (total > 0) {
                total = 1.0 / total;
                printf("Total parallel resistance: %.2f Ohms\n", total);
            } else {
                printf("Error: No valid resistors entered\n");
            }
            break;
        }
        
        case 3: {
            // Voltage Divider Calculation
            double vin, r1, r2;
            printf("Voltage Divider Calculation\n");
            printf("Enter input voltage (V): "); scanf("%lf", &vin);
            printf("Enter R1 (Ohms): "); scanf("%lf", &r1);
            printf("Enter R2 (Ohms): "); scanf("%lf", &r2);
            getchar(); // Clear buffer
            
            if (r1 > 0 && r2 > 0) {
                double vout = vin * (r2 / (r1 + r2));
                printf("Output voltage: %.2f V\n", vout);
            } else {
                printf("Error: Invalid resistor values\n");
            }
            break;
        }
        
        case 4: {
            // RC Time Constant Calculation
            double r, c;
            printf("RC Time Constant Calculation\n");
            printf("Enter resistance (Ohms): "); scanf("%lf", &r);
            printf("Enter capacitance (F): "); scanf("%lf", &c);
            getchar(); // Clear buffer
            
            if (r > 0 && c > 0) {
                double tau = r * c;
                printf("RC time constant (tau): %.6f seconds\n", tau);
                printf("Charging time to 63.2%%: %.6f seconds\n", tau);
                printf("Charging time to 95%%: %.6f seconds\n", 3 * tau);
            } else {
                printf("Error: Invalid values\n");
            }
            break;
        }
        
        case 5: {
            // Low Pass Filter Cutoff Frequency
            double r, c;
            printf("Low Pass Filter Cutoff Frequency\n");
            printf("Enter resistance (Ohms): "); scanf("%lf", &r);
            printf("Enter capacitance (F): "); scanf("%lf", &c);
            getchar(); // Clear buffer
            
            if (r > 0 && c > 0) {
                double fc = 1.0 / (2 * 3.1415926535 * r * c);
                printf("Cutoff frequency: %.2f Hz\n", fc);
                printf("Cutoff frequency: %.2f kHz\n", fc / 1000);
            } else {
                printf("Error: Invalid values\n");
            }
            break;
        }
        
        case 6:
            printf("Returning to main menu...\n");
            break;
            
        default:
            printf("Invalid selection\n");
    }
}

void cleanup() {
    if (current_table != NULL) {
        free_table(current_table);
        current_table = NULL;
    }
}
