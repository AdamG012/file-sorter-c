/****************************************
 * PROGRAM - SORT
 * **** DESCRIPTION ****
 * Sort is a program that will read the contents of a file and output the contents in a sorted order to stdout.
 * The program can sort the data by their alpha-numeric or numerical representation. 
 *
 * **** INSTRUCTIONS ****
 * This program will accept the following command line arguments:
 * 	- First argument is the file name, this is required
 * 	- Second optional argument specifies the output order as either ascending order (-a) or descending(-d), by default it is ascending if the argument is omitted
 * 	- Third optional argument specifies the comparison method, if the contents to be compared by their alpha-numeric (-alph) or numerical (-num) representation, default is alpha-numeric representation
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <bsd/string.h>
#include "functions.h"

int process_arguments(int argc, char** argv, int* sort_method, int* order);
int sort_output(int, int, unsigned long long, FILE*);


/**
 * Given the order, sort method, length of file (lines) and file display the contents of the file in the selected order
 * Returns 0 if successful
 * Returns -1 if an error occurred while reading
 */
int sort_output(int order, int sort_method, unsigned long long len, FILE* f) {

	// Select the method to compare on
	int (*compare)(FILE*, off_t, off_t, int) = (sort_method) ? compare_alphanum : compare_num;

	unsigned long long count = 0; 		// The current amount sorted
	unsigned long long repeated = 1; 	// The number of times we need to see a value before we print a repeated variable
	off_t min_line = -1; 			// The offset in the file of the current min line
	off_t prev_line = -1;			// The offset in the file of the previous line

	// Loop until we have sorted every element
	while (count < len) {
		
		min_line = -1; 			// Set min_line found to false
		int repeat = repeated; 		// Helper to not lose value of repeated
		
		// While we do not read EOF
		for (unsigned long long i = 0; i < len; i++) {

			// Get the position of the start of the line
			off_t pos_f = ftell(f);
			if (pos_f < 0) {
				return -1;
			}

			// If not the first min value then compare to previous min
			int compare_prev = (prev_line < 0) ? -1 : compare(f, prev_line, pos_f, order);

		       	// Check whether we found a repeated value and it has reached limit
			if (!compare_prev && repeat-- == 0) {
				min_line = pos_f;
				break;
			}

			// If we have not have the first value yet
			else if (min_line == -1 && (count == 0 || compare_prev < 0)) {
				min_line = pos_f;
			} 

			// Else if min is found and the value is smaller than current min and larger than previous output
			else if (min_line != -1 && compare(f, pos_f, min_line, order) < 0 && (count == 0 || compare_prev < 0)) {
				min_line = pos_f;
			}


			if (fseek(f, pos_f, SEEK_SET) < 0) { 	// Move back to the start of the current line
				return -1;

			}
			empty_line(f);				// Move to the end of this line
		}

		// Print the line from the min position
		print_line(f, min_line);

		if (count != 0 && compare(f, prev_line, min_line, order) == 0) {
			repeated++;
		} else {
			repeated = 1;
		}

					
		prev_line = min_line;			// Set the previous line to the minimum
		if (fseek(f, 0L, SEEK_SET) < 0) {	// Seek back to the start and if error return -1
			return -1;	
		}
		count++;				// Increment the count
	}

	return 0;
}


/**
 * Process the arguments given number of arguments, arguments, and set the arguments to the desired values.
 * Scans for -a, -b, -alpha, -num in a loop taking the LAST value set
 * Output
 * 	- Return -1 if invalid argument
 * 	- Return 0 if valid
 */
int process_arguments(int argc, char** argv, int* sort_method, int* order) {
	for (int i = 2; i < argc; i++) {
		// Check for ASCENDING ORDER
		if (strncmp(argv[i], "-a", 3) == 0) {
			*order = 1;
		}
		// CHECK FOR DESCENDING ORDER
		else if (strncmp(argv[i], "-b", 3) == 0) {
			*order = -1;
		}

		// CHECK FOR NUMERIC SORTING
		else if (strncmp(argv[i], "-num", 5) == 0) {
			*sort_method = 0;
		}

		// CHECK FOR ALPHA-NUMERIC SORTING
		else if (strncmp(argv[i], "-alpha", 7) == 0) {
			*sort_method = 1;
		}

		// Else return invalid
		else {
			printf("Invalid argument: %s.\n", argv[i]);
			return -1;
		}
	}

	return 0;
}


int main(int argc, char** argv) {

	// Check whether the number of arguments is valid
	if (argc < 2 || argc > 4) {
		printf("Invalid number of arguments\n");
		return 1;
	}

	FILE* f = fopen(argv[1], "r");

	// Check if the filename is valid
	if (f == NULL) {
		printf("Invalid file.\n");
		return 1;
	}

	int order = 1; // default ascending
	int sort_method = 1; // default alpha numeric representation

	if (process_arguments(argc, argv, &sort_method, &order) != 0) {
		fclose(f);
		return 1;
	}

	// Get the file length and then rewind the file pointer
	unsigned long long len = get_file_len(f);
	rewind(f);
	
	// Sort the output given the order, sort method and length
	if (sort_output(order, sort_method, len, f) < 0) {
		printf("Error while reading file\n");
		fclose(f); 
		return -1;
	}

	fclose(f); 
}
