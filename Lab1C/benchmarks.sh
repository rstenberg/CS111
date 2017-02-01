#! /bin/sh

#######################################
############## Benchmarks #############
#######################################

# Create benchmark test input files
echo "Benchmark1" > a1.txt; \
echo "Benchmark2" > a2.txt; \
printf "benchmark 3\nBENCHMARK 3\nBenchmark 3" > a3.txt; \

#Benchmark1
echo ""; \
echo "*************************"; \
echo "Starting Benchmark-1 test"; \
echo "*************************"; \

printf "\n******** (simpsh-1) ********\n"; \
./simpsh --profile \
--rdonly a1.txt \
--pipe \
--pipe \
--creat --append --wronly error1.txt \
--creat --wronly output1.txt \
--command 0 2 5 cat \
--command 1 4 5 tr [:lower:] [:upper:] \
--command 3 6 5 grep BENCHMARK1 \
--wait; \
sleep 1

printf "\n******** (bash/dash-1) ********\n"; \
time cat <a1.txt | tr [:lower:] [:upper:] | grep BENCHMARK1 >/dev/null

# CLean up Benchmank1
rm -f a1.txt error1.txt output1.txt

# Benchmark2
echo ""; \
echo "*************************"; \
echo "Starting Benchmark-2 test"; \
echo "*************************"; \

printf "\n******** (simpsh-2) ********\n"; \
./simpsh --profile \
--rdonly a2.txt \
--pipe \
--pipe \
--creat --wronly output2.txt \
--creat --append --wronly error2.txt \
--command 0 2 6 cat -e \
--command 1 4 6 sed 's/[0-9]/-2/g' \
--command 3 5 6 grep 'Benchmark-2\$' \
--wait; \
sleep 1

printf "\n******** (bash/dash-2) ********\n"; \
time cat -e <a2.txt | sed 's/[0-9]/-2/g' | grep 'Benchmark-2\$' >/dev/null

# Clean up Benchmark2
rm -f a2.txt error2.txt output2.txt

# Benchmark3
echo ""; \
echo "*************************"; \
echo "Starting Benchmark-3 test"; \
echo "*************************"; \

printf "\n******** (simpsh-3) ********\n"; \
./simpsh --profile \
--rdonly a3.txt \
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

printf "\n******** (bash/dash-3) ********\n"; \
time sort a3.txt | tr [:lower:] [:upper:] | uniq -d | wc -l | grep '1' >/dev/null

# Clean up Benchmark3
rm -f a3.txt error3.txt output3.txt



