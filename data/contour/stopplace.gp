# UTF-8-Encoding für Umlaute (ä, ö, ü, ß)
set encoding utf8

# Titel des Plots
set title "Contour-Plot: Höhenprofile der StopPlaces"

# Achsenbeschriftungen
set xlabel "Longitude"
set ylabel "Latitude"

set view map        # 2D-Draufsicht (x/y-Ebene)
unset surface       # Keine Flächen-/Rasterdarstellung
set contour base    # Konturlinien auf der Grundebene zeichnen
set key outside     # Legende außerhalb des Plots platzieren

# Interpolation des unregelmäßigen Punktgitters
set dgrid3d 60,60 qnorm 2

# Definition diskreter Höhenstufen für Konturlinien
set cntrparam levels discrete 305, 310, 315, 320, 325

# Gitterlinien im Plot anzeigen
set grid

# Definition von Linientypen (Farben und Linienbreite)
set linetype 1 lc rgb "#081d58" lw 2   # Dunkelblau
set linetype 2 lc rgb "#de2d26" lw 2   # Rot
set linetype 3 lc rgb "#fd8d3c" lw 2   # Orange
set linetype 4 lc rgb "#2ca25f" lw 2   # Grün
set linetype 5 lc rgb "#41b6c4" lw 2   # Türkis

# Plotten der Konturlinien (gleiche Daten, unterschiedliche Linienstile)
splot "stopplace.dat" u 1:2:3 w l ls 1 title "Isohöhenlinien (305–325)", \
      "" u 1:2:3 w l ls 2 notitle, \
      "" u 1:2:3 w l ls 3 notitle, \
      "" u 1:2:3 w l ls 4 notitle, \
      "" u 1:2:3 w l ls 5 notitle

# Warten, damit sich das Plotfenster nicht sofort schließt
pause -1 "Press any key to exit"
