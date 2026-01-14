set title "Straßenbahn Linienverläufe 3D"
set xlabel "Longitude"
set ylabel "Latitude"
set zlabel "Altitude (m)"
set xrange [11.04:11.16]
set yrange [49.35:49.55]
set zrange [250:380]

set ticslevel 0
set grid

set view 60, 35
set border linewidth 1.5

set style line 1 lc rgb "blue" lw 2 pt 7 ps 1.5
set style line 2 lc rgb "blue" lw 1 dt 2   # dotted vertical lines

splot "routes.dat" using 1:2:3 with linespoints ls 1 notitle, \
    "routes.dat" using 1:2:(0):(0):(0):3 with vectors \
        nohead ls 2 notitle

pause -1 "Press any key to exit"
