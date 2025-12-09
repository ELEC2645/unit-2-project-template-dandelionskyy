# Test Cases Directory

This directory contains automated test case configuration files.

## File Description

### cases1.txt
- **Description**: Basic functionality test cases
- **Content**: Includes data loading, simple queries, condition filtering tests
- **Purpose**: Validate core functionality

### cases2.txt
- **Description**: Advanced functionality test cases
- **Content**: Includes sorting, multi-condition queries, performance tests
- **Purpose**: Validate extended functionality

## Test Case Format

Test case files use simple text format, one test case per line:

```
Test Name|Test Type|SQL Query|Data File|Expected Rows|Description
```

### Field Description

- **Test Name**: Unique identifier for the test
- **Test Type**: SQL_QUERY, DATA_LOAD, FUNCTIONAL, PERFORMANCE
- **SQL Query**: SQL statement to execute
- **Data File**: Data file to use (located in data directory)
- **Expected Rows**: Expected number of rows to return (-1 means don't check)
- **Description**: Detailed description of the test

## Examples

```
Basic Query|SQL_QUERY|SELECT * FROM sample1|sample1.csv|10|Test full table query
Condition Filter|SQL_QUERY|SELECT * FROM sample1 WHERE age > 30|sample1.csv|3|Test age filtering
Data Loading|DATA_LOAD||sample1.csv|10|Test CSV file loading
```

## Running Tests

Use the "Run Automated Tests" function in the main program to execute test cases.
