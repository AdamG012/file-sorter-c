# File Sorter

## Installation

### Build

1. `git clone https://github.sydney.edu.au/agha0431/soft3410hw2.git`
2. `cd soft3410hw2/src`
3. `make` 


### Clean

Just type in `make clean` to clean binaries and object files.


## Usage

`./fsort <file_name> [OPTIONS]`

Options:

- `-a` - sort in ascending order
- `-b` - sort in descending order
- `-alpha` sort alphanumeric
- `-num` sort numeric


## Conditions:

- Line length can be arbitrary and file length can be arbitrary. However, to get the length of the file or count number of lines sorted/ouputted an `unsigned long long` is used, which in fact could overflow if provided a file length outside its bounds.
- Sorting for numerical favours numeric then alphabetical order:

e.g. This file

```
1
2
20
3
abc
abc1
abc2
bca123
123a
123oko  
```

Will be sorted to this:

```
1
2
3
20
123a
123oko
abc
abc100
abc22
bca123
```

- '\n' characters are taken literally and hence is before most characters. Lines are not stripped and spaces are not skipped.

## Description

This program will output a given file in sorted order based on either optional arguments:

- Alpha-numeric order *(default)*
- Numerical order

And can sort in either ascending `-a` (default), or descending, `-b` order.



### Implementation

The way this problem was approached was in my opinion a bit all over the place. This final implementation is decent but I would probably go back and make it more modular and definitely implement more defensive checks. Initially I attempted to in order to break the problem down was to deal with any file length but have a defined maximum line length. Once this was complete I was able to substitute my current solution which involved a custom read function which varied based on whether it was sorting on alphanumeric or numeric. 

Given a `FILE*` pointer, two variables were managed in order to compare, store and display the line:

- `min_line` which is the offset of the current minimum line from `SEEK_SET`
- `prev_line` which is the offset of the previous minimum line from `SEEK_SET`

The file pointer would be offset to the start of the file and each line would be compared char by char to the `min_line` offset and `prev_line` offset. If the current line is less than the minimum line (based on descending/ascending) and greater or equal to the previous line than it would update the `min_line` offset to that offset of the line at that point.

This would be done $n$ times and operated in a way similar to selection sort as the min or max was selected every time. The outer loop ensured every line was selected at most once and hence the overall algorithm has a complexity of $O(n^{2})$ not including line comparisons. With these it would be $O(n^{2}m)$ where m is the length of all the chars.

