# UTF-8-Encoding für Umlaute (ä, ö, ü, ß)
set encoding utf8

# Titel des Plots
set title "Straßenbahn Linienverläufe 3D"

# Achsenbeschriftungen
set xlabel "Longitude"
set ylabel "Latitude"
set zlabel "Altitude (m)"

# Achsenbereiche (Längen-, Breiten- und Höhenbereich)
set xrange [11.04:11.16]
set yrange [49.35:49.55]
set zrange [250:380]

set ticslevel 0               # Projektion der Ticks auf die Grundebene
set grid                      # Gitterlinien anzeigen
set view 60, 35               # Blickwinkel (Elevation, Azimut)
set border linewidth 1.5      # Rahmenstärke des Plots

# Automatische Linien-/Punktstile aus Benutzerdefinitionen
set style increment user

# Linienfarben und -stile für einzelne Straßenbahnlinien
set style line 1 lc rgb "#E41A1C" lw 2 pt 7  # Rot
set style line 2 lc rgb "#377EB8" lw 2 pt 7  # Blau
set style line 3 lc rgb "#4DAF4A" lw 2 pt 7  # Grün
set style line 4 lc rgb "#000000" lw 2 pt 7  # Schwarz
set style line 5 lc rgb "#4d4d4d" lw 2 pt 7  # Dunkelgrau
set style line 6 lc rgb "#ff8000" lw 2 pt 7  # Orange
set style line 7 lc rgb "#2b00a0" lw 2 pt 7  # Dunkelblau
set style line 8 lc rgb "#27853b" lw 2 pt 7  # Dunkelgrün

# Stil für Höhenprojektionen (vertikale Linien nach unten)
set style line 10 lc rgb "gray50" lw 1 dt 2

# Plotte Linienverläufe und Höhenprojektionen (Vektoren zur Grundebene)
splot "routes.dat" index 0 with linespoints ls 1 title "Linie 1", \
      "routes.dat" index 1 with linespoints ls 2 title "Linie 2", \
      "routes.dat" index 2 with linespoints ls 3 title "Linie 3", \
      "routes.dat" index 3 using ($1+0.001):2:3 with linespoints ls 4 title "Linie 4", \
      "routes.dat" index 4 with linespoints ls 5 title "Linie 5", \
      "routes.dat" index 5 with linespoints ls 6 title "Linie 6", \
      "routes.dat" index 0 using 1:2:(0):(0):(0):3 with vectors nohead ls 10 notitle, \
      "routes.dat" index 1 using 1:2:(0):(0):(0):3 with vectors nohead ls 10 notitle, \
      "routes.dat" index 2 using 1:2:(0):(0):(0):3 with vectors nohead ls 10 notitle, \
      "routes.dat" index 3 using 1:2:(0):(0):(0):3 with vectors nohead ls 10 notitle, \
      "routes.dat" index 4 using 1:2:(0):(0):(0):3 with vectors nohead ls 10 notitle, \
      "routes.dat" index 5 using 1:2:(0):(0):(0):3 with vectors nohead ls 10 notitle, \
      "routes.dat" index 6 using 1:2:(0):(0):(0):3 with vectors nohead ls 10 notitle

# Warten, damit sich das Plotfenster nicht sofort schließt
pause -1 "Press any key to exit"
