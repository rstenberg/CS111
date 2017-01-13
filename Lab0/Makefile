SHELL := /bin/bash
CC=gcc
tar_files = lab0.c README Makefile *.png

default:
	gcc -o lab0 lab0.c
check: default copyTest openInput creatOutput segfaultCatch
	if [ -a test_log.txt ]; then \
		echo "Error: Smoke-test did not pass."; \
	else \ 
		echo "All tests succeeded."; \
	fi;
clean:
	rm -f *.tar.gz *.o lab0 
dist: default
	tar -czf lab0-304410501.tar.gz $(tar_files)


# Smoke-test to check if program works
# Will check: 
#	1) Text is properly copied from input to output
#	2) "--input=<file>" option can open input file
#	3) "--output<file>" option can open output file
#	4) "--segfault" triggers segmentation fault appropriately
#	5) "--catch" will catch a segmentation fault so that it does not trigger and report and error

copyTest:
	echo "Smoke-test" > input.txt; \
	@ ./lab0 --input=input.txt --output.=output.txt; \
	cmp input.txt output.txt; \
	if [ $$? -ne 0 ]; then \
		echo "Error: Contents of output file are different than contents of input file." >> test_log.txt; \ 
	fi; \
	rm -f input.txt output.txt
openInput:
	echo "Smoke-test" > input.txt; \
	./lab0 --input=input.txt > /dev/null; \
	if [ $$? -eq 1 ]; then \
		echo "Error: Could not open input file with read-only permission." >> test_log.txt; \ 
	fi; \
	rm -f input.txt
creatOutput:
	echo "Smoke-test" > input.txt; \
	./lab0 --input=input.txt --output.=output.txt >> /dev/null; \
	if [ $$? -eq 2 ]; then \
		echo "Error: Could not open output file with write permission." >> test_log.txt; \ 
	fi; \
	rm -f input.txt output.txt
segfaultCatch:
	./lab0 --segfault --catch > &>/dev/null; \
	if [[ $$? -ne 3 ]]; then \
		echo "Error: Segmentation fault was not caught properly when catch option set." > test_log.txt \
	fi;











