set terminal svg
set xrange [0:500]
set yrange [-500:0]

set output "data/G_seed.svg"
datafile = 'data/G_seed.dat'
plot datafile i 0 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 1 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 2 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 3 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 4 using 1:($2*-1) with lp lw 2 title columnheader(1)

set output "data/G_carlos.svg"
datafile = 'data/G_carlos.dat'
plot datafile i 0 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 1 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 2 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 3 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 4 using 1:($2*-1) with lp lw 2 title columnheader(1)

set output "data/G_greed.svg"
datafile = 'data/G_greed.dat'
plot datafile i 0 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 1 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 2 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 3 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 4 using 1:($2*-1) with lp lw 2 title columnheader(1)

set output "data/HC_seed.svg"
datafile = 'data/HC_seed.dat'
plot datafile i 0 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 1 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 2 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 3 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 4 using 1:($2*-1) with lp lw 2 title columnheader(1)

set output "data/HC_result.svg"
datafile = 'data/HC_result.dat'
plot datafile i 0 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 1 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 2 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 3 using 1:($2*-1) with lp lw 2 title columnheader(1), datafile i 4 using 1:($2*-1) with lp lw 2 title columnheader(1)


#datafile = 'test.dat'
#stats datafile
#plot for [IDX=1:STATS_blocks] datafile index (IDX-1) using 1:($2*-1) with lines title columnheader(1)
