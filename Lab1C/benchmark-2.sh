#! /bin/sh

#Benchmark2
echo ""; \
echo "*************************"; \
echo "Starting Benchmark-2 test"; \
echo "*************************"; \

cat -e <bench2.txt | sed 's/[0-9]/-2/g' | grep 'Benchmark-2\$' >/dev/null

times