# UTF-8-Encoding für Umlaute (ä, ö, ü, ß)
set encoding utf8

# Titel des Plots
set title "Contour-Plot: Höhenprofile der StopPlaces"

# Achsenbeschriftungen
set xlabel "Longitude"
set ylabel "Latitude"

set view map        # 2D-Draufsicht (x/y-Ebene)
set key outside     # Legende außerhalb des Plots platzieren

# Farbpalette für Höhencodierung mit scharfen Grenzen
set palette model RGB defined ( \
    305 "#081d58", 310 "#081d58", 310.001 "#41b6c4", 315 "#41b6c4", 315.001 "#2ca25f", \
    320 "#2ca25f", 320.001 "#fd8d3c", 325 "#fd8d3c", 325.001 "#de2d26", 330 "#de2d26" )
set cbrange [305:330]
set cblabel "Höhe (m)"
set cbtics (305, 310, 315, 320, 325)

# Gitterlinien im Plot anzeigen
set grid

# Plotten der Punkte mit Farbcodierung nach Höhe
plot "stopplace.dat" u 1:2:3 w p pt 7 ps 1 palette notitle

# Warten, damit sich das Plotfenster nicht sofort schließt
pause -1 "Press any key to exit"
