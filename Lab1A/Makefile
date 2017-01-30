SHELL := /bin/bash
CC=gcc
tar_files = lab1.c README Makefile

default:
	gcc -o simpsh -g lab1.c -std=c99
simpsh:
	gcc -o simpsh -g lab1.c -std=c99
check: default verboseTest openFilesTest commandTest creatOutputFile abortTest closeTest
	if [ -a test_log.txt ]; then \
		echo "Error: Smoke-test did not pass."; \
	else \ 
		echo "All tests succeeded."; \
	fi;
clean:
	rm -f *.tar.gz *.o simpsh
dist: default
	tar -czf lab1-304410501.tar.gz $(tar_files)


# Smoke-test to check if program works
# Will check: 
#	1) --verbose prints commands
#	2) --rdonly --wronly --rdwr files can be opened properly
#	3) --command i o e sort produces correct results and stores in correct location
#	5) --creat properly opens files that do not exist
#	5) --abort triggers segmentation fault appropriately
#	6) --close properly closes file so that it can't be accessed

verboseTest:
	touch temp.txt; \
	touch result.txt; \
	@ ./simpsh --verbose --rdonly temp.txt > result.txt; \
	if grep -Fxq "--rdonly temp.txt" result.txt; then \
	    echo "Verbose test passed"; \
	else
	    echo "Error: verbose option did not print command properly." >> test_log.txt; \ 
	fi
openFilesTest:
	touch rd.txt; \
	touch wr.txt; \
	touch rdwr.txt; \
	touch error.txt; \
	@ ./simpsh --rdonly rd.txt --wronly wr.txt --rdwr rdwr.txt > /dev/null 2>error.txt; \
	if [ -s error.txt ]; then \
		echo "Error: Trouble opening one of the files." >> test_log.txt; \ 
	fi; \
	rm -f rd.txt wr.txt rdwr.txt error.txt
commandTest:
	touch rd.txt; \
	touch wr.txt; \
	touch rdwr.txt; \
	touch desired_result.txt; \
	echo "a\nc\nb" > rd.txt; \
	echo "a\nb\nc" > desired_result.txt; \
	@ ./simpsh --rdonly rd.txt --wronly wr.txt --rdwr rdwr.txt --command 0 1 2 sort > /dev/null; \
	cmp wr.txt desired_result.txt; \
	if [ $$? -ne 0 ]; then \
		echo "Error: Contents of output file are not sorted properly. Command failed somewhere along the way." >> test_log.txt; \ 
	fi; \
	rm -f rd.txt wr.txt rdwr.txt desired_result.txt
creatOutputFile:
	touch error.txt; \
	@ ./simpsh --creat--wronly wr.txt > /dev/null 2>error.txt; \
	if [ -s error.txt ]; then \
		echo "Error: Could not create output file." >> test_log.txt; \ 
	fi; \
	rm -f error.txt wr.txt
abortTest:
	touch output.txt; \
	@ ./simpsh --abort > output.txt; \
	if [[ -s output.txt ]]; then \
		echo "Abort test passed"; \
	else
		echo "Error: Segmentation fault was not triggered by --abort option." >> test_log.txt; \
	fi; \
	rm output.txt
closeTest:
	touch rd.txt; \
	touch wr.txt; \
	touch rdwr.txt; \
	touch error.txt; \
	@ ./simpsh --rdonly rd.txt --wronly wr.txt --rdwr rdwr.txt --close 0 --command 0 1 2 sort > /dev/null 2>error.txt; \
	if [[ -s error.txt ]]; then \
		echo "Close test passed"; \
	else
		echo "Error: Closing file and then writing to it did not trigger an error" >> test_log.txt; \
	fi; \
	rm rd.txt wr.txt rdwr.txt;


