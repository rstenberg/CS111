SHELL := /bin/bash
CC=gcc
tar_files = lab2_add.c lab2_list.c README Makefile lab2_add.csv lab2b_list.csv lab2b_csv_script.sh SortedList.h SortedList.c lab2B.gp lab2b_1.png lab2b_2.png lab2b_3.png lab2b_4.png lab2b_5.png profile.gperf

default:
	gcc -lpthread -o lab2_add -g lab2_add.c -std=c99
	gcc -lpthread -o lab2_list -g lab2_list.c SortedList.c -std=c99
build: 	default
tests: 	default
	chmod +x ./lab2b_csv_script.sh
	./lab2b_csv_script.sh
profile: default
	LD_PRELOAD=./libprofiler.so CPUPROFILE=./raw.gperf ./lab2_list --threads=12 --iterations=1000 --sync=s
	pprof --text ./lab2_list ./raw.gperf > profile.gperf
	pprof --list=thread_function ./lab2_list ./raw.gperf >> profile.gperf
graphs: tests
	/usr/local/cs/bin/gnuplot lab2B.gp
tarball: graphs profile
	tar -zcvf lab2-304410501.tar.gz $(tar_files)
dist: 
	tar -zcvf lab2-304410501.tar.gz $(tar_files)
clean:
	rm -f *.tar.gz *.o lab2_add lab2_list *.csv *.png *.gperf
