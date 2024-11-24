#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>

#define MULTIPLIER 1e9

// Function prototype for our assembly function
extern void calculate_acceleration(const double* matrix, const int rows, int* results);

// Function to calculate acceleration using C
void calculate_acceleration_c(const double* matrix, const int rows, int* results) {
    for (int i = 0; i < rows; i++) {
        // Convert km/h to m/s
        const double v1 = matrix[i * 3] * (1000.0 / 3600.0);
        const double v2 = matrix[i * 3 + 1] * (1000.0 / 3600.0);
        const double t = matrix[i * 3 + 2];

        // Calculate acceleration
        results[i] = (int)round((v2 - v1) / t);
    }
}

// Helper function to generate random double between min and max
double random_double(const double min, const double max) {
    const double scale = rand() / (double)RAND_MAX;
    return min + scale * (max - min);
}

// Function to generate test data
void generate_test_data(double* matrix, const int rows) {
    for (int i = 0; i < rows; i++) {
        // Initial velocity (0-80 km/h)
        matrix[i * 3] = random_double(0.0, 80.0);
        // Final velocity (initial + 20-100 km/h)
        matrix[i * 3 + 1] = matrix[i * 3] + random_double(20.0, 100.0);
        // Time (2-15 seconds)
        matrix[i * 3 + 2] = random_double(2.0, 15.0);
    }
}

// Function to verify results
void verify_results(const double* matrix, const int* results, const int rows) {
    int error_count = 0;

    for (int i = 0; i < rows; i++) {
        // Convert km/h to m/s
        const double v1 = matrix[i * 3] * (1000.0 / 3600.0);
        const double v2 = matrix[i * 3 + 1] * (1000.0 / 3600.0);
        const double t = matrix[i * 3 + 2];

        // Calculate expected acceleration
        const int expected = (int)round((v2 - v1) / t);
        if (results[i] != expected) {
            error_count++;
            if (error_count > 5) {
                printf("Stopping verification due to too many errors\n");
                return;
            }

            printf("Verification failed at row %d: Expected %d, Got %d\n",
                i, expected, results[i]);
        }
    }

    if (error_count == 0) printf("All results verified correctly!\n");
}

// Function to measure execution time using QueryPerformanceCounter
double measure_execution_time(void (*function)(const double*, const int, int*), const double* matrix, const int rows, int* results) {
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    function(matrix, rows, results);
    QueryPerformanceCounter(&end);
    return (double)(end.QuadPart - start.QuadPart) * MULTIPLIER / (double)frequency.QuadPart; // Time in nanoseconds (MULTIPLIER-dependent)
}

int main() {
    srand((unsigned int)time(NULL));
    const int test_sizes[] = { 10, 100, 1000, 10000 };
    const int num_sizes = sizeof(test_sizes) / sizeof(test_sizes[0]);

    // Single test with user input
    int y = 0;
    char buffer[100];

    printf("Enter the number of rows (cars): ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        y = strtol(buffer, NULL, 10);
    }

    if (y <= 0) {
        printf("Invalid number of rows. Must be greater than 0.\n\n");
    }

    if (y > 0) {
        double* matrix = malloc(y * 3 * sizeof(double));
        int* results = malloc(y * sizeof(int));

        // Check for memory allocation failure, exit if so
        if (matrix == NULL || results == NULL) {
            printf("Memory allocation failed\n");
            return 1;
        }

        printf("Enter the elements of the %d by 3 matrix:\n", y);
        for (int i = 0; i < y; i++) {
            for (int j = 0; j < 3; j++) {
                scanf_s("%lf", &matrix[i * 3 + j]);
            }
        }

        const double asm_time = measure_execution_time(calculate_acceleration, matrix, y, results);

        printf("\n");

        verify_results(matrix, results, y);
        printf("Calculated accelerations are:\n");
        for (int i = 0; i < y; i++) {
            printf("%d m/s^2\n", results[i]);
        }

        printf("\nExecution time using ASM: %lf nanoseconds\n", asm_time);

        const double c_time = measure_execution_time(calculate_acceleration_c, matrix, y, results);
        printf("Execution time using C: %lf nanoseconds\n\n", c_time);

        free(matrix);
        free(results);

        // Clear input buffer
        while (getchar() != '\n');
    }

    // Ask user if they want to run the tests
    printf("Do you want to run the auto-generated tests? (y/n): ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == 'n') {
            return 0;
        }
    }

    // For each test size
    for (int size_idx = 0; size_idx < num_sizes; size_idx++) {
        const int num_runs = 30;
        const int rows = test_sizes[size_idx];

        double* matrix = malloc(rows * 3 * sizeof(double));
        int* results = malloc(rows * sizeof(int));

        // Check for memory allocation failure, exit if so
        if (matrix == NULL || results == NULL) {
            printf("Memory allocation failed\n");
            return 1;
        }

        double total_time_asm = 0.0;
        double total_time_c = 0.0;

        printf("\nTesting with %d rows:\n", rows);

        // Run multiple times for average
        for (int run = 0; run < num_runs; run++) {
            // Generate new test data
            generate_test_data(matrix, rows);

            // Time the assembly function
            const double asm_time = measure_execution_time(calculate_acceleration, matrix, rows, results);
            total_time_asm += asm_time;

            // Verify results on first run
            if (run == 0) {
                verify_results(matrix, results, rows);
                printf("Sample output (first 5 rows):\n");
                for (int i = 0; i < (rows < 5 ? rows : 5); i++) {
                    printf("Row %d: %.1f km/h, %.1f km/h, %.1f s -> %d m/s^2\n",
                        i + 1, matrix[i * 3], matrix[i * 3 + 1],
                        matrix[i * 3 + 2], results[i]);
                }
            }

            // Time the C function
            const double c_time = measure_execution_time(calculate_acceleration_c, matrix, rows, results);
            total_time_c += c_time;
        }

        printf("Average execution time using ASM: %lf nanoseconds\n",
            total_time_asm / num_runs);
        printf("Average execution time using C: %lf nanoseconds\n",
            total_time_c / num_runs);

        free(matrix);
        free(results);
    }

    return 0;
}
