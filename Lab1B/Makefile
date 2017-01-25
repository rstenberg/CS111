SHELL := /bin/bash
CC=gcc
tar_files = lab1.c README Makefile

default:
	gcc -o simpsh -g lab1.c -std=c99
simpsh:
	gcc -o simpsh -g lab1.c -std=c99
clean:
	rm -f *.tar.gz *.o simpsh
dist: default
	tar -czf lab1-304410501.tar.gz $(tar_files)
