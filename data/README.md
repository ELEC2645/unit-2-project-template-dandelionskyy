# Data Files Directory

This directory contains CSV data files for testing and demonstration.

## File Description

### sample1.csv
- **Description**: Simple user data example
- **Columns**: id, name, age, city, salary
- **Purpose**: Basic functionality testing

### sample2.csv  
- **Description**: Product inventory data example
- **Columns**: product_id, product_name, category, price, stock, supplier
- **Purpose**: Multi-column data testing

### big_data.csv
- **Description**: Large dataset test file
- **Columns**: Contains multiple fields of simulated data
- **Purpose**: Performance testing and big data processing

## Data Format Requirements

CSV files should follow these formats:
- First row contains column names (header)
- Use comma to separate fields
- Support text containing commas (use double quotes)
- Support null values (leave empty)

## Adding New Data

To add new test data:
1. Create new CSV file
2. Ensure first row contains column names
3. Update file description in README
4. Add corresponding test cases in tests directory
