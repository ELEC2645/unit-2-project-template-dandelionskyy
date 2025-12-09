# Electronic Component Database & Calculation System

A lightweight SQL database engine with professional electronic engineering calculation tools, designed for Electronic Information Engineering applications.

## Project Overview

This project combines database management capabilities with electronic engineering calculations, providing a comprehensive tool for managing electronic components and performing circuit analysis.

## Features

### Database Management
- **CSV File Loading**: Load electronic component data from CSV files
- **SQL Query Engine**: Execute SQL queries on component data
- **Data Filtering**: Filter components by category, specification, etc.
- **Automated Testing**: Run test suites to verify system functionality

### Professional Calculation Tools
- **Resistor Calculations**: Series and parallel resistance calculations
- **Voltage Divider**: Calculate output voltage for voltage divider circuits
- **RC Time Constant**: Calculate RC circuit timing characteristics
- **Filter Design**: Low pass filter cutoff frequency calculations

### AI Assistance
- Natural language query suggestions
- Circuit design recommendations
- Component selection guidance

## Electronic Information Engineering Integration

### Professional Relevance
This project demonstrates the application of database systems in electronic engineering contexts, including:

1. **Component Inventory Management**: Track electronic components with specifications
2. **Circuit Design Support**: Provide calculation tools for common circuit designs
3. **Data Analysis**: Analyze component usage patterns and inventory levels
4. **Educational Tool**: Learn both database concepts and electronic engineering principles

### Technical Skills Applied
- **Database Systems**: SQL query processing, data management
- **Embedded Systems**: Component data modeling for hardware applications
- **Signal Processing**: Filter design and frequency calculations
- **Circuit Analysis**: Electronic circuit parameter calculations

## Data Structure

### Components Database
The system includes sample data for common electronic components:
- Resistors, Capacitors, Inductors
- Microcontrollers and Logic ICs
- Diodes, Transistors, Crystals
- LEDs and other discrete components

### Circuit Designs
Example circuit designs with formulas and applications:
- Low Pass Filters
- Voltage Dividers
- LED Drivers
- RC Timing Circuits

## Usage

### Loading Data
1. Select "Load CSV File" from main menu
2. Choose from available CSV files in data/ directory
3. Available files: `components.csv`, `circuit_designs.csv`

### SQL Queries
Execute SQL queries on loaded data:
```sql
SELECT * FROM components WHERE category='Resistor'
SELECT component_name, quantity FROM components WHERE quantity < 50
```

### Professional Calculations
Access electronic engineering calculations:
- Resistor series/parallel combinations
- Voltage divider outputs
- RC time constants
- Filter cutoff frequencies

## Building and Running

### Compilation
```bash
build.bat
```

### Execution
```bash
minidb.exe
```

## Project Structure
```
MiniDB-TestAI-Project/
├── src/                    # Source code
├── data/                   # Electronic component data files
├── tests/                  # Test cases
├── build/                  # Compiled binaries
└── docs/                   # Documentation
```

## Educational Value

This project serves as an excellent demonstration of how database systems can be applied in electronic engineering contexts, bridging the gap between software development and hardware design.

## Future Enhancements

- Support for more complex circuit calculations
- Integration with component datasheets
- Visualization of circuit designs
- Export functionality for BOM (Bill of Materials)
