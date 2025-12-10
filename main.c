/* Activate POSIX and X/Open capabilities for popen/pclose operations */
#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#endif

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

/* 全局变量 */
Table* cur_table = NULL;

/* Function prototypes */
void process_csv_import(void);
void execute_sql_statement(void);
void perform_automated_testing(void);
void activate_ai_support(void);
void run_engineering_calculations(void);
void release_resources(void);

/* Menu management routines */
static void display_primary_menu(void);
static void render_menu_interface(void);
static int  acquire_user_selection(void);
static void process_menu_option(int option);
static void navigate_to_main_menu(void);

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
    
    /* Program continues indefinitely until exit(0) is called in process_menu_option() */
    while(1) {
        display_primary_menu();
    }
    /* This point is never reached */
    return 0;
}

static void display_primary_menu(void)
{
    render_menu_interface();
    {
        int user_option = acquire_user_selection();
        process_menu_option(user_option);
    }
}

static int acquire_user_selection(void)
{
    enum { TOTAL_MENU_CHOICES = 6 };   /* Options 1-5 are features, 6 is Exit */
    char input_line[128];
    int selection_is_valid = 0;
    int chosen_value = 0;

    /* Determine if stdin is a terminal (interactive session) */
    int terminal_mode = 1;
#ifdef _WIN32
    terminal_mode = _isatty(_fileno(stdin));
#else
    terminal_mode = isatty(fileno(stdin));
#endif

    if (!terminal_mode) {
        /* Non-interactive environment (e.g., CI/CD pipeline) */
        /* Automatically select exit option for graceful termination */
        printf("\nNon-interactive environment detected. Exiting automatically.\n");
        return TOTAL_MENU_CHOICES; /* Return exit choice */
    }

    do {
        printf("\nChoose menu option: ");
        
        if (!fgets(input_line, sizeof(input_line), stdin)) {
            /* EOF or read error; terminate gracefully */
            puts("\nInput failure. Program termination.");
            exit(1);
        }

        /* Remove trailing newline characters */
        input_line[strcspn(input_line, "\r\n")] = '\0';

        if (!is_integer(input_line)) {
            printf("Please input an integer value!\n");
            selection_is_valid = 0;
        } else {
            chosen_value = (int)strtol(input_line, NULL, 10);
            if (chosen_value >= 1 && chosen_value <= TOTAL_MENU_CHOICES) {
                selection_is_valid = 1;
            } else {
                printf("Invalid menu selection!\n");
                selection_is_valid = 0;
            }
        }
    } while (!selection_is_valid);

    return chosen_value;
}

static void process_menu_option(int option)
{
    switch (option) {
        case 1:
            process_csv_import();
            navigate_to_main_menu();
            break;
        case 2:
            execute_sql_statement();
            navigate_to_main_menu();
            break;
        case 3:
            perform_automated_testing();
            navigate_to_main_menu();
            break;
        case 4:
            activate_ai_support();
            navigate_to_main_menu();
            break;
        case 5:
            run_engineering_calculations();
            navigate_to_main_menu();
            break;
        default:
            printf("Thank you for using MiniDB-TestAI!\n");
            release_resources();
            exit(0);
    }
}

static void render_menu_interface(void)
{
    printf("\n----------- Primary Menu -----------\n");
    printf("\n"
           "\t\t\t\t\t\t\n"
           "\t1. Import CSV Data File\t\t\n"
           "\t2. Run SQL Query Statement\t\t\n"
           "\t3. Execute Automated Tests\t\t\n"
           "\t4. Activate AI Query Support\t\t\n"
           "\t5. Engineering Calculation Tools\t\t\n"
           "\t6. Terminate Application\t\t\t\n"
           "\t\t\t\t\t\t\n");
    printf("---------------------------------------------\n");
}

static void navigate_to_main_menu(void)
{
    char user_response[64];
    do {
        printf("\nType 'b' or 'B' to return to primary menu: ");
        if (!fgets(user_response, sizeof(user_response), stdin)) {
            puts("\nInput malfunction. Exiting program.");
            exit(1);
        }
        user_response[strcspn(user_response, "\r\n")] = '\0'; /* eliminate newline */
    } while (!(user_response[0] == 'b' || user_response[0] == 'B') || user_response[1] != '\0');
}


void process_csv_import() {
    // Show CSV files in data directory
    printf("Available CSV files in data/:\n");
    
    FILE* pipe;
    int file_count = 0;
    
    // Use different commands for Windows and Unix
#ifdef _WIN32
    pipe = _popen("dir /b data\\*.csv 2>nul", "r");
#else
    pipe = popen("ls data/*.csv 2>/dev/null", "r");
#endif

    if (pipe) {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), pipe)) {
            // Remove newline
            buffer[strcspn(buffer, "\r\n")] = 0;
            if (buffer[0]) {
                // On Unix, extract filename from path
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
        printf("  Failed to list CSV files\n");
    }
    
    printf("\n");
    
    // Get filename from user
    char filename[100];
    printf("Enter CSV filename (in data/ directory): ");
    scanf("%99s", filename);
    getchar(); // Clear input buffer
    
    // Build full path
    char path[150];
    snprintf(path, sizeof(path), "data/%s", filename);
    
    // Free previous table if exists
    if (cur_table) {
        free_table(cur_table);
        cur_table = NULL;
    }
    
    // Load CSV file
    cur_table = load_csv(path);
    if (cur_table) {
        printf("Loaded table '%s' successfully\n", cur_table->name);
        printf("Rows: %d, Columns: %d\n", cur_table->row_count, cur_table->col_count);
    } else {
        printf("Failed to load CSV file: %s\n", path);
    }
}

void execute_sql_statement() {
    if (cur_table == NULL) {
        printf("Error: Please load a CSV file first\n");
        return;
    }
    
    /* 显示可用的SQL命令和示例 */
    printf("\nAvailable SQL Commands with Examples:\n");
    printf("1. Basic SELECT query:\n");
    printf("   SELECT * FROM %s\n", cur_table->name);
    printf("2. SELECT specific columns:\n");
    printf("   SELECT %s, %s FROM %s\n", 
           cur_table->columns[0].name, 
           cur_table->columns[1].name, 
           cur_table->name);
    printf("3. SELECT with WHERE condition:\n");
    printf("   SELECT * FROM %s WHERE %s = 'John'\n", 
           cur_table->name, cur_table->columns[1].name);
    printf("   SELECT * FROM %s WHERE %s > 30\n", 
           cur_table->name, cur_table->columns[2].name);
    printf("4. SELECT with LIKE condition:\n");
    printf("   SELECT * FROM %s WHERE %s LIKE '%%J%%'\n", 
           cur_table->name, cur_table->columns[1].name);
    printf("\nImportant Notes:\n");
    printf("- Use single quotes for string values: 'John'\n");
    printf("- Use LIKE with %% for pattern matching: '%%J%%'\n");
    printf("- Supported operators: =, !=, >, <, >=, <=, LIKE\n");
    printf("- Currently supports only single WHERE condition (no AND/OR)\n");
    printf("- For multiple conditions, use separate queries\n");
    printf("\nAvailable columns: ");
    for (int i = 0; i < cur_table->col_count; i++) {
        printf("%s", cur_table->columns[i].name);
        if (i < cur_table->col_count - 1) {
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
    
    QueryResult* result = execute_query(cur_table, parsed_query);
    if (result != NULL) {
        print_query_result(result);
        free_query_result(result);
    } else {
        printf("Query execution failed\n");
    }
    
    free_query(parsed_query);
}

void perform_automated_testing() {
    printf("Executing automated test procedures...\n");
    
    /* Display accessible test files */
    printf("Test files accessible in tests/ directory:\n");
    
    /* Enumerate test files in tests directory - cross-platform methodology */
    FILE* test_pipe;
#ifdef _WIN32
    test_pipe = _popen("dir /b tests\\*.txt 2>nul", "r");
#else
    test_pipe = popen("ls tests/*.txt 2>/dev/null", "r");
#endif
    if (test_pipe != NULL) {
        char test_buffer[256];
        int test_file_count = 0;
        while (fgets(test_buffer, sizeof(test_buffer), test_pipe) != NULL) {
            /* Eliminate newline character */
            test_buffer[strcspn(test_buffer, "\r\n")] = 0;
            if (strlen(test_buffer) > 0) {
                /* Extract filename from path (Unix systems) */
#ifndef _WIN32
                char* path_divider = strrchr(test_buffer, '/');
                if (path_divider) {
                    printf("  - %s\n", path_divider + 1);
                } else {
                    printf("  - %s\n", test_buffer);
                }
#else
                printf("  - %s\n", test_buffer);
#endif
                test_file_count++;
            }
        }
#ifdef _WIN32
        _pclose(test_pipe);
#else
        pclose(test_pipe);
#endif
        
        if (test_file_count == 0) {
            printf("  No test files discovered in tests/ directory\n");
        }
    } else {
        printf("  Could not enumerate test files\n");
    }
    printf("\n");
    
    char test_filename[100];
    printf("Input test case filename (residing in tests/ directory): ");
    
    /* Employ fgets for superior input handling */
    if (fgets(test_filename, sizeof(test_filename), stdin) == NULL) {
        printf("Input malfunction\n");
        return;
    }
    
    /* Remove newline characters */
    test_filename[strcspn(test_filename, "\r\n")] = '\0';
    
    /* Return if input is empty */
    if (strlen(test_filename) == 0) {
        printf("No test file indicated\n");
        return;
    }
    
    /* Construct complete file path */
    char test_complete_path[150];
    snprintf(test_complete_path, sizeof(test_complete_path), "tests/%s", test_filename);
    
    int test_result = run_test_suite(test_complete_path);
    if (test_result == 0) {
        printf("All test cases succeeded!\n");
    } else {
        printf("Certain test cases encountered failures\n");
    }
    
    /* Diagnostic information: confirm function normal return */
    printf("DEBUG: perform_automated_testing() finalized, reverting to navigate_to_main_menu()\n");

    /* **Critical modification: purge input buffer** */
    /* Ensure no residual newline or other characters remain in input stream before navigate_to_main_menu() */
    int character;
    while ((character = getchar()) != '\n' && character != EOF);
}

void activate_ai_support() {
    printf("AI Support Functionality\n");
    printf("Sample inquiries you can make:\n");
    printf("  - 'How to query all resistors?'\n");
    printf("  - 'Find capacitors under $1'\n");
    printf("  - 'Show me components from Texas Instruments'\n");
    printf("\nInput your inquiry or query requirement: ");
    
    char user_inquiry[256];
    
    /* Directly employ fgets without prior buffer clearance */
    if (fgets(user_inquiry, sizeof(user_inquiry), stdin) == NULL) {
        printf("Input malfunction\n");
        return;
    }
    
    trim_string(user_inquiry);
    
    if (strlen(user_inquiry) == 0) {
        printf("Inquiry cannot be blank\n");
        return;
    }
    
    char* suggestion = obtain_ai_recommendation(user_inquiry, cur_table);
    if (suggestion != NULL) {
        printf("\nAI Suggestion: %s\n", suggestion);
        free(suggestion);
    } else {
        printf("Unable to generate AI suggestion\n");
    }
}

void run_engineering_calculations() {
    printf("=== Engineering Calculation Utilities ===\n");
    printf("Electronic Circuit Computations\n\n");
    
    int calculation_selection;
    printf("Available Computational Modules:\n");
    printf("1. Series Resistor Network Calculation\n");
    printf("2. Parallel Resistor Network Calculation\n");
    printf("3. Voltage Divider Network Calculation\n");
    printf("4. RC Circuit Time Constant Computation\n");
    printf("5. Low-Pass Filter Corner Frequency\n");
    printf("6. Revert to Primary Menu\n");
    printf("Select computational module (1-6): ");
    
    scanf("%d", &calculation_selection);
    getchar(); /* Purge input buffer */
    
    switch (calculation_selection) {
        case 1: {
            /* Series Resistor Network Calculation */
            double resistance1, resistance2, resistance3 = 0.0;
            printf("Series Resistor Network Calculation\n");
            printf("Input resistor values (Ohms). Input 0 for unutilized resistors:\n");
            printf("Resistance R1 (Ohms): "); scanf("%lf", &resistance1);
            printf("Resistance R2 (Ohms): "); scanf("%lf", &resistance2);
            printf("Resistance R3 (Ohms): "); scanf("%lf", &resistance3);
            getchar(); /* Purge input buffer */
            
            double aggregate_resistance = resistance1 + resistance2 + resistance3;
            printf("Aggregate series resistance: %.2f Ohms\n", aggregate_resistance);
            break;
        }
        
        case 2: {
            /* Parallel Resistor Network Calculation */
            double resistance1, resistance2, resistance3 = 0.0;
            printf("Parallel Resistor Network Calculation\n");
            printf("Input resistor values (Ohms). Input 0 for unutilized resistors:\n");
            printf("Resistance R1 (Ohms): "); scanf("%lf", &resistance1);
            printf("Resistance R2 (Ohms): "); scanf("%lf", &resistance2);
            printf("Resistance R3 (Ohms): "); scanf("%lf", &resistance3);
            getchar(); /* Purge input buffer */
            
            double reciprocal_sum = 0.0;
            if (resistance1 > 0) reciprocal_sum += 1.0 / resistance1;
            if (resistance2 > 0) reciprocal_sum += 1.0 / resistance2;
            if (resistance3 > 0) reciprocal_sum += 1.0 / resistance3;
            
            if (reciprocal_sum > 0) {
                double equivalent_resistance = 1.0 / reciprocal_sum;
                printf("Equivalent parallel resistance: %.2f Ohms\n", equivalent_resistance);
            } else {
                printf("Error: No legitimate resistors provided\n");
            }
            break;
        }
        
        case 3: {
            /* Voltage Divider Network Calculation */
            double input_voltage, resistance1, resistance2;
            printf("Voltage Divider Network Calculation\n");
            printf("Input source voltage (V): "); scanf("%lf", &input_voltage);
            printf("Input resistance R1 (Ohms): "); scanf("%lf", &resistance1);
            printf("Input resistance R2 (Ohms): "); scanf("%lf", &resistance2);
            getchar(); /* Purge input buffer */
            
            if (resistance1 > 0 && resistance2 > 0) {
                double output_voltage = input_voltage * (resistance2 / (resistance1 + resistance2));
                printf("Output voltage: %.2f V\n", output_voltage);
            } else {
                printf("Error: Invalid resistance parameters\n");
            }
            break;
        }
        
        case 4: {
            /* RC Circuit Time Constant Computation */
            double resistance, capacitance;
            printf("RC Circuit Time Constant Computation\n");
            printf("Input resistance value (Ohms): "); scanf("%lf", &resistance);
            printf("Input capacitance value (F): "); scanf("%lf", &capacitance);
            getchar(); /* Purge input buffer */
            
            if (resistance > 0 && capacitance > 0) {
                double time_constant = resistance * capacitance;
                printf("RC time constant (tau): %.6f seconds\n", time_constant);
                printf("Charging duration to 63.2%%: %.6f seconds\n", time_constant);
                printf("Charging duration to 95%%: %.6f seconds\n", 3 * time_constant);
            } else {
                printf("Error: Invalid parameter values\n");
            }
            break;
        }
        
        case 5: {
            /* Low-Pass Filter Corner Frequency */
            double resistance, capacitance;
            printf("Low-Pass Filter Corner Frequency\n");
            printf("Input resistance value (Ohms): "); scanf("%lf", &resistance);
            printf("Input capacitance value (F): "); scanf("%lf", &capacitance);
            getchar(); /* Purge input buffer */
            
            if (resistance > 0 && capacitance > 0) {
                double corner_frequency = 1.0 / (2 * 3.1415926535 * resistance * capacitance);
                printf("Corner frequency: %.2f Hz\n", corner_frequency);
                printf("Corner frequency: %.2f kHz\n", corner_frequency / 1000.0);
            } else {
                printf("Error: Invalid parameter values\n");
            }
            break;
        }
        
        case 6:
            printf("Reverting to primary menu...\n");
            break;
            
        default:
            printf("Invalid module selection\n");
    }
}

void release_resources() {
    if (cur_table != NULL) {
        free_table(cur_table);
        cur_table = NULL;
    }
}
