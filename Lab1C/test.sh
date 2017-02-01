#! /bin/sh

# Smoke-test to check if program works
# Will check: 
#	1) --verbose prints commands
#	2) --rdonly --wronly --rdwr files can be opened properly
#	3) --command i o e sort produces correct results and stores in correct location
#	5) --creat properly opens files that do not exist
#	5) --abort triggers segmentation fault appropriately
#	6) --close properly closes file so that it can't be accessed

#verboseTest:
rm -f test_log.txt; \
echo "Starting Verbose Test"; \
touch temp.txt; \
touch result.txt; \
./simpsh --verbose --rdonly temp.txt > result.txt; \
sleep 1; \
if [ -s result.txt ]; then \
    echo "Verbose test passed"; \
else \
    echo "Error: verbose option did not print command properly." >> test_log.txt; \
fi; \
rm -f temp.txt result.txt

#openFilesTest:
echo "Starting File Opening Test"; \
touch rd.txt; \
touch wr.txt; \
touch rdwr.txt; \
touch error.txt; \
./simpsh --rdonly rd.txt --wronly wr.txt --rdwr rdwr.txt > /dev/null 2>error.txt; \
sleep 1; \
if [ -s error.txt ]; then \
	echo "Error: Trouble opening one of the files." >> test_log.txt; \
fi; \
rm -f rd.txt wr.txt rdwr.txt error.txt

#commandTest:
echo "Starting Command Test"; \
touch rd.txt; \
touch wr.txt; \
touch rdwr.txt; \
touch desired_result.txt; \
printf "a\nc\nb" > rd.txt; \
printf "a\nb\nc" > desired_result.txt; \
./simpsh --rdonly rd.txt --wronly wr.txt --rdwr rdwr.txt --command 0 1 2 sort; \
sleep 1; \
cmp wr.txt desired_result.txt; \
if [ $? -ne 1 ]; then \
	echo "Error: Contents of output file are not sorted properly. Command failed somewhere along the way." >> test_log.txt; \
fi; \
rm -f rd.txt wr.txt rdwr.txt desired_result.txt

#creatOutputFile:
echo "Starting Create Output Test"; \
touch error.txt; \
./simpsh --creat --wronly wr.txt > /dev/null 2>error.txt; \
if [ -s error.txt ]; then \
	echo "Error: Could not create output file." >> test_log.txt; \
fi; \
rm -f error.txt wr.txt

#abortTest:
echo "Starting Abort Test"; \
touch input.txt; \
touch output.txt; \
touch error.txt; \
printf "b\na" > input.txt; \
./simpsh --abort -rdonly input.txt --wronly output.txt --wronly error.txt --command 0 1 2 sort; \
sleep 1; \
if [ -s output.txt ]; then \
	echo "Error: Segmentation fault was not triggered by --abort option." >> test_log.txt; \
else \
	echo "Abort test passed"; \
fi; \
rm -f input.txt output.txt error.txt 

#closeTest:
echo "Starting Close Test"; \
touch rd.txt; \
touch wr.txt; \
touch error.txt; \
./simpsh --rdonly rd.txt --wronly wr.txt --wronly error.txt --close 0 --command 0 1 2 sort 2>error.txt; \
sleep 1; \
if [ -s error.txt ]; then \
	echo "Close test passed"; \
else \
	echo "Error: Closing file and then writing to it did not trigger an error" >> test_log.txt; \
fi; \
rm rd.txt wr.txt error.txt;


# Check that all tests passed
if [ -a test_log.txt ]; then \
echo "Error: Smoke-test did not pass."; \
else \
echo "All tests succeeded."; \
fi;

