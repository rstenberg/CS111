#! /usr/bin/gnuplot

# purpose:
#	 generate data reduction graphs for the multi-threaded list project
#
# input: lab2_add.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # add operations
#	5. run time (ns)
#	6. run time per operation (ns)
#	7. total sum at end of run (should be zero)
# 	
# 	lab2_list.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. run time per operation (ns)
#
# output:
#	lab2b_1.png ... throughput of mutex/spin locked operations vs number of threads
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.
#

# general plot parameters
set terminal png
set datafile separator ","

set title "Throughput vs number of threads for mutex and spin-lock adds/list operations"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Operations per second"
set logscale y 10
set output 'lab2b_1.png'
set key left top
plot \
	"< grep add-m lab2_add.csv" using ($2):(1000000000/($6)) \
	title 'mutex-add' with linespoints lc rgb 'blue', \
	"< grep add-s lab2_add.csv" using ($2):(1000000000/($6)) \
	title 'spin-add' with linespoints lc rgb 'orange', \
	"< grep list-none-m lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'mutex-list' with linespoints lc rgb 'red', \
	"< grep list-none-s lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'spin-lock-list' with linespoints lc rgb 'violet'


set title "Wait-for-lock-time and average time per operation vs number of competing threads"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Wait-for-lock-time and avg-time-per-operation"
set logscale y 10
set output 'lab2b_2.png'
set key left top
plot \
        "< grep list-none-m lab2b_list.csv" using ($2):($8) \
        title 'wait-for-lock-time' with linespoints lc rgb 'red', \
        "< grep list-none-s lab2b_list.csv" using ($2):($7) \
        title 'average-time-per-operation (nanoseconds)' with linespoints lc rgb 'violet'

set title "How many threads/iterations we can run without failure (w/ yielding)"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Iterations per thread"
set logscale y 10
set output 'lab2b_3.png'
set key left top
plot \
	"< grep list-id-none lab2b_list.csv" using ($2):($3) \
	title 'w/o sync' with points lc rgb 'green', \
	"< grep list-id-m lab2b_list.csv" using ($2):($3) \
        title 'mutex' with points lc rgb 'blue', \
	"< grep list-id-s lab2b_list.csv" using ($2):($3) \
	title 'spin-lock' with points lc rgb 'red'

set title "Throughout vs number of threads (Mutex)"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Throughput (total operations per second)"
set logscale y 10
set output 'lab2b_4.png'
set key left top
plot \
	"< grep list-none-m,[0-9]*,[0-9]*,1, lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '1-list' with linespoints lc rgb 'green', \
        "< grep list-none-m,[0-9]*,[0-9]*,4, lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '4-lists' with linespoints lc rgb 'red', \
        "< grep list-none-m,[0-9]*,[0-9]*,8, lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '8-lists' with linespoints lc rgb 'blue', \
        "< grep list-none-m,[0-9]*,[0-9]*,16, lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '16-lists' with linespoints lc rgb 'violet'

set title "Throughout vs number of threads (Spin-Lock)"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Throughput (total operations per second)"
set logscale y 10
set output 'lab2b_5.png'
set key left top
plot \
        "< grep list-none-s,[0-9]*,[0-9]*,1, lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '1-list' with linespoints lc rgb 'green', \
	"< grep list-none-s,[0-9]*,[0-9]*,4, lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '4-lists' with linespoints lc rgb 'red', \
	"< grep list-none-s,[0-9]*,[0-9]*,8, lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '8-lists' with linespoints lc rgb 'blue', \
	"< grep list-none-s,[0-9]*,[0-9]*,16, lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '16-lists' with linespoints lc rgb 'violet'
