set terminal svg
#set terminal png
set output "stations.svg"

set xrange [0:500]
set yrange [0:500]

plot "test.dat" i 0 using 1:2 with lp lw 2 title columnheader(1), "test.dat" i 1 using 1:2 with lp lw 2 title columnheader(1)

#datafile = 'test.dat'
#stats datafile
#plot for [IDX=1:STATS_blocks] datafile index (IDX-1) using 1:2 with lines title columnheader(1)
