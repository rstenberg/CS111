SHELL := /bin/bash
CC=gcc
tar_files = lab2_add.c README Makefile lab2_add.gp lab2_add.csv lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png lab2_add-5.png

default:
	gcc -lpthread -o lab2_add -g lab2_add.c -std=c99
	gcc -lpthread -o lab2_list lab2_list.c SortedList.c -std=c99
lab2_add: 	default
build: 		default
tests: 		default
	chmod +x ./lab2a_sample_script.sh
	./lab2a_sample_script.sh
graphs: tests
	~/gnuplot/bin/gnuplot lab2_add.gp
dist: graphs
	tar -zcvf lab2-304410501.tar.gz --transform 's,^,lab2-304410501/,' $(tar_files)
tarball: dist
clean:
	rm -f *.tar.gz *.o lab2_add