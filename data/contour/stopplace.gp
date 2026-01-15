
set encoding utf8

set title "Contour-Plot: Höhenprofile der StopPlaces"
set xlabel "Longitude"
set ylabel "Latitude"


set view map               
unset surface              
set contour base            
set key outside             


set dgrid3d 60,60 qnorm 2


set cntrparam levels discrete 305, 310, 315, 320, 325


set grid


set linetype 1 lc rgb "#081d58" lw 2   # dunkelblau
set linetype 2 lc rgb "#de2d26" lw 2   # hellblau
set linetype 3 lc rgb "#fd8d3c" lw 2   # grün
set linetype 4 lc rgb "#2ca25f" lw 2   # orange
set linetype 5 lc rgb "#41b6c4" lw 2   # rot


splot "stopplace.dat" u 1:2:3 w l ls 1 title "Isohöhenlinien (305–325)", \
      "" u 1:2:3 w l ls 2 notitle, \
      "" u 1:2:3 w l ls 3 notitle, \
      "" u 1:2:3 w l ls 4 notitle, \
      "" u 1:2:3 w l ls 5 notitle


pause -1 "Press any key to exit"