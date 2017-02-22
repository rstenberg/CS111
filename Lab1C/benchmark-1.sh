#! /bin/sh

#Benchmark1
echo ""; \
echo "*************************"; \
echo "Starting Benchmark-1 test"; \
echo "*************************"; \

cat <bench1.txt | tr [:lower:] [:upper:] | grep BENCHMARK1 >/dev/null

times