# Acceleration Calculator (C and x86-64 Assembly)

This program calculates the acceleration of vehicles based on initial velocity, final velocity, and time using both C and x86-64 Assembly implementations. The program demonstrates the performance benefits of assembly optimization for numerical computations.

#### <I> The video demo for our group can be accessed in this [Google Drive](https://drive.google.com/file/d/1z0xOglCtpH92wrGmcC5Z3yWRD0olXClF/view?usp=sharing). </i>

## Members
- Roi Victor Roberto
- Matthew Heinz Sanchez

## Implementation Details

### C Implementation
- Handles input/output operations
- Manages memory allocation
- Generates random test data
- Verifies calculation results
- Times execution performance

### Assembly Implementation
- Converts velocities from km/h to m/s
- Calculates acceleration: (v2 - v1) / t
- Converts results from double to integer
- Optimized for SIMD operations

## Performance Analysis

The program was tested with different input sizes (10, 100, 1000, 10000 rows), with each test repeated 30 times to obtain average execution times. Here are the results:

### Test Results

| Input Size | ASM Time (ns) | C Time (ns) | Speedup Factor |
|------------|---------------|-------------|----------------|
| 10         | 80.000000     | 490.000000  | 6.13x         |
| 100        | 133.333333    | 4570.000000 | 34.28x        |
| 1000       | 1113.333333   | 44286.666667| 39.78x        |
| 10000      | 11526.666667  | 470656.666667| 40.83x       |

### Analysis
1. **Small Inputs (10 rows)**:
   - Assembly shows modest performance advantage
   - Overhead from function calls more significant at small scales
   - 6.13x speedup over C implementation

2. **Medium Inputs (100-1000 rows)**:
   - Assembly demonstrates significant performance benefits
   - Speedup factors of 34.28x and 39.78x respectively
   - Efficient SIMD utilization becomes apparent

3. **Large Inputs (10000 rows)**:
   - Assembly shows maximum performance advantage
   - 40.83x speedup over C implementation
   - Demonstrates excellent scalability

4. **General Observations**:
   - Assembly implementation consistently outperforms C
   - Speedup factor increases with input size
   - Both implementations maintain calculation accuracy

## Program Output Screenshots

### Test Results with Correctness Verification

![Screenshot 2024-11-25 000609](https://github.com/user-attachments/assets/be55e315-10ab-4828-8965-05e60d89f811)

1. The result in each row in a test case is verified.
   - If there are inconsistencies, the following message is displayed: ```Verification failed at row X: Expected val_1, Got val_2```
   - If there are none, the following is shown instead: ```All results verified correctly!```
2. Only the first five errors in a test case are shown; if there are, the rest are truncated in the display.
3. The output of the first five rows is also displayed.

## Usage

1. Download the source code and open the Visual Studio solution file
2. Compile the program with both C and Assembly source files
3. Run the executable
4. Choose between:
   - Manual input mode: Enter custom values
   - Automated test mode: Run performance benchmarks

### Input Format
- Number of rows (cars)
- For each car:
  - Initial velocity (km/h)
  - Final velocity (km/h)
  - Time interval (seconds)

### Output Format
- Acceleration in m/s²
- Execution times for both implementations
- Verification of results

## Build Requirements

- Visual Studio
- x86-64 compatible processor
- NASM assembler
- C compiler with C11 support
- Windows operating system (for [QueryPerformanceCounter](https://learn.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter))
