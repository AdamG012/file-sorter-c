#include <stdio.h>
#include <stdlib.h>

#define LEN 512

int to_lower(char, char);
unsigned long long get_file_len(FILE*);
void empty_line(FILE*);
void print_line(FILE*, off_t);
int compare_alphanum(FILE*, off_t, off_t, int);
int compare_num(FILE*, off_t, off_t, int);
off_t read_zeros(FILE*, off_t);
