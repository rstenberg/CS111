#! /bin/sh

#Benchmark3
echo ""; \
echo "*************************"; \
echo "Starting Benchmark-3 test"; \
echo "*************************"; \

sort bench3.txt | tr [:lower:] [:upper:] | uniq -d | wc -l | grep '1' >/dev/null

times