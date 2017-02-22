#! /bin/sh

#######################################
############## Benchmarks #############
#######################################

#Benchmark1
echo ""; \
echo "*************************"; \
echo "Starting Benchmark-1 test"; \
echo "*************************"; \

printf "\n******** (simpsh-1) ********\n"; \
./simpsh --profile \
--rdonly bench1.txt \
--pipe \
--pipe \
--creat --append --wronly error1.txt \
--creat --wronly output1.txt \
--command 0 2 5 cat \
--command 1 4 5 tr [:lower:] [:upper:] \
--command 3 6 5 grep BENCHMARK1 \
--wait; \
sleep 1

# Clean up Benchmank1
rm -f error1.txt output1.txt

# Benchmark2
echo ""; \
echo "*************************"; \
echo "Starting Benchmark-2 test"; \
echo "*************************"; \

printf "\n******** (simpsh-2) ********\n"; \
./simpsh --profile \
--rdonly bench2.txt \
--pipe \
--pipe \
--creat --wronly output2.txt \
--creat --append --wronly error2.txt \
--command 0 2 6 cat -e \
--command 1 4 6 sed 's/[0-9]/-2/g' \
--command 3 5 6 grep 'Benchmark-2\$' \
--wait; \
sleep 1

# Clean up Benchmark2
rm -f error2.txt output2.txt

# Benchmark3
echo ""; \
echo "*************************"; \
echo "Starting Benchmark-3 test"; \
echo "*************************"; \

printf "\n******** (simpsh-3) ********\n"; \
./simpsh --profile \
--rdonly bench3.txt \
--pipe \
--pipe \
--pipe \
--pipe \
--creat --append --wronly output3.txt \
--creat --append --wronly error3.txt \
--command 0 2 10 sort \
--command 1 4 10 tr [:lower:] [:upper:] \
--command 3 6 10 uniq -d \
--command 5 8 10 wc -l \
--command 7 9 10 grep '1' \
--wait; \
sleep 1

# Clean up Benchmark3
rm -f error3.txt output3.txt



