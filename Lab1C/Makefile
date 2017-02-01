SHELL := /bin/bash
CC=gcc
tar_files = lab1.c README Makefile test.sh benchmarks.sh

default:
	gcc -o simpsh -g lab1.c -std=c99
simpsh:
	gcc -o simpsh -g lab1.c -std=c99
check: default
	chmod +x ./test.sh
	./test.sh
clean:
	rm -f *.tar.gz *.o simpsh
dist: default
	tar -zcvf lab1-304410501.tar.gz --transform 's,^,lab1-304410501/,' $(tar_files)