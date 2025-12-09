#include "test_reporter.h"
#include <stdio.h>
#include <time.h>

void print_test_report(const TestSuite* suite) {
    if (suite == NULL) {
        printf("测试套件为空\n");
        return;
    }

    printf("\n=== 详细测试报告 ===\n");
    printf("测试套件: %s\n", suite->name);
    printf("执行时间: %s", ctime(&(time_t){suite->total_time}));
    printf("测试统计: %d 通过, %d 失败, %d 跳过, %d 总计\n",
           suite->passed_count, suite->failed_count, suite->skipped_count, suite->test_count);
    
    double pass_rate = (suite->test_count > 0) ? 
        (double)suite->passed_count / suite->test_count * 100 : 0;
    printf("通过率: %.1f%%\n", pass_rate);

    printf("\n--- 测试用例详情 ---\n");
    for (int i = 0; i < suite->test_count; i++) {
        const TestCase* test_case = &suite->test_cases[i];
        print_test_case_result(test_case);
    }
}

void generate_html_report(const TestSuite* suite, const char* filename) {
    if (suite == NULL || filename == NULL) {
        return;
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法创建HTML报告文件: %s\n", filename);
        return;
    }

    fprintf(file, "<!DOCTYPE html>\n");
    fprintf(file, "<html>\n");
    fprintf(file, "<head>\n");
    fprintf(file, "    <title>MiniDB-TestAI 测试报告</title>\n");
    fprintf(file, "    <style>\n");
    fprintf(file, "        body { font-family: Arial, sans-serif; margin: 20px; }\n");
    fprintf(file, "        .header { background: #f0f0f0; padding: 10px; border-radius: 5px; }\n");
    fprintf(file, "        .summary { margin: 20px 0; }\n");
    fprintf(file, "        .test-case { margin: 10px 0; padding: 10px; border-left: 4px solid; }\n");
    fprintf(file, "        .passed { border-color: #4CAF50; background: #f9fff9; }\n");
    fprintf(file, "        .failed { border-color: #f44336; background: #fff9f9; }\n");
    fprintf(file, "        .skipped { border-color: #ff9800; background: #fffbf0; }\n");
    fprintf(file, "        .error { border-color: #9c27b0; background: #fdf7ff; }\n");
    fprintf(file, "        .time { color: #666; font-size: 0.9em; }\n");
    fprintf(file, "    </style>\n");
    fprintf(file, "</head>\n");
    fprintf(file, "<body>\n");

    fprintf(file, "    <div class=\"header\">\n");
    fprintf(file, "        <h1>MiniDB-TestAI 测试报告</h1>\n");
    fprintf(file, "        <p>测试套件: %s</p>\n", suite->name);
    fprintf(file, "        <p>生成时间: %s</p>", ctime(&(time_t){time(NULL)}));
    fprintf(file, "    </div>\n");

    fprintf(file, "    <div class=\"summary\">\n");
    fprintf(file, "        <h2>测试统计</h2>\n");
    fprintf(file, "        <p>总测试用例: %d</p>\n", suite->test_count);
    fprintf(file, "        <p>通过: %d</p>\n", suite->passed_count);
    fprintf(file, "        <p>失败: %d</p>\n", suite->failed_count);
    fprintf(file, "        <p>跳过: %d</p>\n", suite->skipped_count);
    
    double pass_rate = (suite->test_count > 0) ? 
        (double)suite->passed_count / suite->test_count * 100 : 0;
    fprintf(file, "        <p>通过率: %.1f%%</p>\n", pass_rate);
    fprintf(file, "        <p>总执行时间: %.3f秒</p>\n", suite->total_time);
    fprintf(file, "    </div>\n");

    fprintf(file, "    <div class=\"details\">\n");
    fprintf(file, "        <h2>测试用例详情</h2>\n");
    
    for (int i = 0; i < suite->test_count; i++) {
        const TestCase* test_case = &suite->test_cases[i];
        const char* status_class = "";
        const char* status_text = "";
        
        switch (test_case->status) {
            case TEST_PASSED:
                status_class = "passed";
                status_text = "✓ 通过";
                break;
            case TEST_FAILED:
                status_class = "failed";
                status_text = "✗ 失败";
                break;
            case TEST_SKIPPED:
                status_class = "skipped";
                status_text = "↷ 跳过";
                break;
            case TEST_ERROR:
                status_class = "error";
                status_text = "⚠ 错误";
                break;
        }

        fprintf(file, "        <div class=\"test-case %s\">\n", status_class);
        fprintf(file, "            <h3>%s - %s</h3>\n", test_case->name, status_text);
        fprintf(file, "            <p><strong>描述:</strong> %s</p>\n", test_case->description);
        
        if (test_case->type == TEST_SQL_QUERY && strlen(test_case->sql_query) > 0) {
            fprintf(file, "            <p><strong>SQL查询:</strong> <code>%s</code></p>\n", test_case->sql_query);
        }
        
        if (strlen(test_case->data_file) > 0) {
            fprintf(file, "            <p><strong>数据文件:</strong> %s</p>\n", test_case->data_file);
        }
        
        if (test_case->expected_row_count >= 0) {
            fprintf(file, "            <p><strong>预期行数:</strong> %d</p>\n", test_case->expected_row_count);
        }
        
        fprintf(file, "            <p class=\"time\">执行时间: %.3f秒</p>\n", test_case->execution_time);
        
        if (test_case->status != TEST_PASSED && strlen(test_case->error_message) > 0) {
            fprintf(file, "            <p><strong>错误信息:</strong> %s</p>\n", test_case->error_message);
        }
        
        fprintf(file, "        </div>\n");
    }

    fprintf(file, "    </div>\n");
    fprintf(file, "</body>\n");
    fprintf(file, "</html>\n");

    fclose(file);
    printf("HTML报告已生成: %s\n", filename);
}

void print_test_case_result(const TestCase* test_case) {
    if (test_case == NULL) {
        return;
    }

    const char* status_icon = "";
    const char* status_text = "";
    
    switch (test_case->status) {
        case TEST_PASSED:
            status_icon = "✓";
            status_text = "通过";
            break;
        case TEST_FAILED:
            status_icon = "✗";
            status_text = "失败";
            break;
        case TEST_SKIPPED:
            status_icon = "↷";
            status_text = "跳过";
            break;
        case TEST_ERROR:
            status_icon = "⚠";
            status_text = "错误";
            break;
    }

    printf("%s %s: %s\n", status_icon, test_case->name, status_text);
    printf("  描述: %s\n", test_case->description);
    
    if (test_case->type == TEST_SQL_QUERY && strlen(test_case->sql_query) > 0) {
        printf("  SQL查询: %s\n", test_case->sql_query);
    }
    
    if (strlen(test_case->data_file) > 0) {
        printf("  数据文件: %s\n", test_case->data_file);
    }
    
    if (test_case->expected_row_count >= 0) {
        printf("  预期行数: %d\n", test_case->expected_row_count);
    }
    
    printf("  执行时间: %.3f秒\n", test_case->execution_time);
    
    if (test_case->status != TEST_PASSED && strlen(test_case->error_message) > 0) {
        printf("  错误信息: %s\n", test_case->error_message);
    }
    
    printf("\n");
}
