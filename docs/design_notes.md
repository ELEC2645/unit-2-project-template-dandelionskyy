# MiniDB-TestAI Design Documentation

## Project Overview

MiniDB-TestAI is a lightweight SQL database engine that integrates automated testing framework and AI assistance features. The project adopts a modular design for easy extension and maintenance.

## Architecture Design

### Core Modules

#### 1. Database Engine Module (db/)
- **csv_loader**: CSV file loading and data parsing
- **parser**: SQL query syntax parsing
- **executor**: Query execution and data processing
- **result**: Query result management and output
- **table**: Data structure and type definitions

#### 2. Testing Framework Module (test_framework/)
- **test_loader**: Test case loading and parsing
- **test_runner**: Test execution and validation
- **test_reporter**: Test result report generation
- **testcase**: Test case data structure

#### 3. AI Assistance Module (ai/)
- **ai_helper**: AI query suggestions and optimization
- Natural language to SQL conversion
- Query optimization suggestions
- Execution plan explanation

#### 4. Utility Module (utils/)
- **string_utils**: String processing utilities
- **file_utils**: File operation utilities

## Data Structure Design

### Table Structure
```c
typedef struct {
    char name[100];
    Column columns[MAX_COLUMNS];
    int col_count;
    char*** data;  // Three-dimensional array: data[row][col][cell]
    int row_count;
    int capacity;
} Table;
```

### Query Structure
```c
typedef struct {
    QueryType type;
    char table_name[100];
    char columns[MAX_COLUMNS][MAX_COLUMN_NAME_LEN];
    int column_count;
    Condition* where_conditions;
    // ... other fields
} Query;
```

## Feature Characteristics

### 1. SQL Support
- SELECT queries
- WHERE condition filtering
- ORDER BY sorting
- Basic aggregate functions
- LIKE pattern matching

### 2. Data Loading
- CSV file format support
- Automatic type inference
- Memory management optimization

### 3. Testing Framework
- Automated test cases
- Test result validation
- Test report generation

### 4. AI Assistance
- Natural language queries
- SQL optimization suggestions
- Query plan explanation

## Performance Considerations

### Memory Management
- Dynamic memory allocation
- Data paging loading
- Memory leak protection

### Query Optimization
- Condition filtering optimization
- Sorting algorithm selection
- Index support (future extension)

## Extensibility Design

### Plugin Architecture
- Modular design for easy extension
- Unified interface definitions
- Hot-pluggable component support

### Data Type Support
- Basic data types (INT, FLOAT, STRING)
- Extensible custom types
- Type conversion and validation

## Security Considerations

### Input Validation
- SQL injection protection
- File path validation
- Memory boundary checking

### Error Handling
- Comprehensive error code system
- Exception handling
- Resource cleanup mechanism

## Deployment and Operation

### Compilation Requirements
- C99 standard compiler
- Standard C library support
- Cross-platform compatibility

### Runtime Environment
- Command line interface
- Batch mode support
- Configuration file management

## Future Planning

### Short-term Goals
- Improve existing functionality
- Performance optimization
- Test coverage enhancement

### Long-term Goals
- Distributed support
- More SQL syntax support
- Graphical interface development
- Cloud service integration
