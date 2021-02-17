# USAGE 
# *****
# make			# Compiles the code
# make clean		# removes all binaries and objects
.PHONY = all clean

all: fsort

fsort: fsort.o functions.o
	@echo "Linking all object files..."
	gcc -Wall -Werror -lm -o fsort fsort.o functions.o -lbsd

%.o: %.c
	@echo "Creating object file.."
	gcc -c -Wall -Werror -o $@ $<

clean:
	@echo "Cleaning up object files and binaries..."
	rm -f *.o fsort
