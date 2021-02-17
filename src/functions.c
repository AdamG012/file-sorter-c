#include <stdio.h>
#include <stdlib.h>

/**
 * Uppercase and lowercase independent comparison
 * Returns 0 if chars are equal or either is not a letter
 * Returns -1 if c1 is lowercase and less than c2
 * Returns 1 if c2 is lowercase and less than c1
 */
int to_lower(char c1, char c2) {

	// If c1 is lowercase and c2 is uppercase and less than
	if (c1 >= 97 && c1 <= 122 && (c2 >= 65 && c2 <= 90) && c1 - 32 < c2) {
		return -1;	
	}

	// If c2 is lowercase and c1 is uppercase and less than
	else if (c2 >= 97 && c2 <= 122 && (c1 >= 65 && c1 <= 90) && c2 - 32 < c1) {
		return 1;
	}

	return 0;
}


/**
 * Get the length of a file based on amount of lines
 * Returns -1 if file is null
 */
unsigned long long get_file_len(FILE* f) {
	// If the file is invalid
	if (f == NULL) {
		return -1;
	}

	char c;
	unsigned long long num_lines = 0;
	// Keep reading until EOF
	while ((c = fgetc(f)) != EOF) {
		// If we see a new line then increment
		if (c == '\n') {
			num_lines += 1;
		}
	}
	return num_lines;
}


/**
 * Go to the end of the line in given file
 * Used to update SEEK_CUR
 */
void empty_line(FILE* f) {
	char c = 0;
	while (((c = fgetc(f)) != EOF) && c != '\n');
}


/**
 * Print the line of the file at given position
 */
void print_line(FILE* f, off_t pos) {
	// Seek to the line to print
	fseek(f, pos, SEEK_SET);

	char c = 0;
	// While we do not hit a newline or EOF
	while ((c = fgetc(f)) != EOF && c != '\0' && c != '\n') {
		// Print the char
		printf("%c", c);
	}

	if (c == '\n') {
		printf("\n");
	}
}


/**
 * Compare two positions in a given file based on an alphanumeric order
 * The order parameter has two values  -1 signifies descending and 1 ascending
 */
int compare_alphanum(FILE* f, off_t i, off_t j, int order) {

	if (f == NULL) {
		return 0;
	}

	// The chars being read
	char c1 = 0, c2 = 0;

	while (c1 != EOF && c2 != EOF && c1 != '\n' && c2 != '\n') {
		if (c1 != c2) {
			break;
		}
		fseek(f, i++, SEEK_SET);
		c1 = fgetc(f);
		fseek(f, j++, SEEK_SET);
		c2 = fgetc(f); 
	}

	// Check if they are equal terminating chars
	if (c1 == c2 || (c1 == '\n' && c2 == '\0') || (c2 == '\n' && c1 == '\0')) {
		return 0;
	} else if (c1 == EOF || c1 == '\n') {
		return -1 * order;
	} else if (c2 == EOF || c2 == '\n') {
		return 1 * order;
	}

	int x = to_lower(c1, c2);

	if (x != 0) {
		return x * order;
	}

	return (c1 - c2) * order;
}


/**
 * Read the numerical notation and ignore zeros 
 * e.g. "000000001" should updated the offset i to where 1 is
 * Returns:
 * 	- updated offset
 * 	- -1 if invalid fseek
 */ 
off_t read_zeros(FILE* f, off_t i) {

	char c;
	// Check if there is an error while seeking
	if (fseek(f, i++, SEEK_SET) < 0) {
		return -1;
	}

	// Loop until we reach a non zero character
	while ((c = fgetc(f)) != EOF && c != '\n' && c == 48) {
		i += 1;
	}

	i -= 1;
	return i;

}


/**
 * Compare two lines in a file using numerical order
 * Order specifies whether ascending (> 0) or descending (< 0)
 */
int compare_num(FILE* f, off_t i, off_t j, int order) {
	if (f == NULL) {
		return 0;
	}

	// The chars being read
	char c1 = 48, c2 = 48;

	int found_diff = 0; 

	// Push offsets to next non zero character
	i = read_zeros(f, i);
	j = read_zeros(f, j);

	while (c1 != EOF && c2 != EOF && c1 != '\n' && c2 != '\n') {

		// If it is not a number
		if (c1 < 48 || c1 >= 58 || c2 < 48 || c2 >= 58) {
			found_diff = -2;	// set check to compare using alpha num
			i--, j--; 		// to set back one char
			break;
		}

		// Set to -1 if c1 is less or 1 if c2 is less
		if (!found_diff && c1 < c2) {
			found_diff = -1;
		} else if (!found_diff && c1 > c2) {
			found_diff = 1;
		}

		fseek(f, i++, SEEK_SET);	// Seek to the location at i
		c1 = fgetc(f);			// Read that character
		fseek(f, j++, SEEK_SET);	// Seek to the location at j
		c2 = fgetc(f); 			// Read that character
	}		

	// If they are not numeric then compare the rest on alphanumeric
	if (found_diff == -2) {
		return compare_alphanum(f, i, j, order);
	}


	// Check if they are equal terminating chars and EQUAL SIZE
	if (c1 == c2 || (c1 == '\n' && c2 == '\0') || (c2 == '\n' && c1 == '\0')) {
		return found_diff * order;
	} else if (c1 == EOF || c1 == '\n') {
		return -1 * order;
	} else if (c2 == EOF || c2 == '\n') {
		return 1 * order;
	}

	return found_diff * order;
}


