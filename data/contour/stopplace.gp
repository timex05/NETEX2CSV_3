# 1. Output-Einstellungen (optional, z.B. für ein PNG)
# set terminal pngcairo size 800,600
# set output 'contour_plot.png'

# 2. Titel und Achsenbeschriftung
set title "Contour-Plot: Höhenprofile der StopPlaces"
set xlabel "Longitude"
set ylabel "Latitude"

# 3. Kontur-Modus aktivieren
set view map               # Draufsicht (2D-Projektion)
unset surface              # Gitterlinien der Oberfläche ausblenden
set contour base           # Konturen auf der Basisebene zeichnen
set key outside            # Legende rechts außerhalb des Plots

# 4. Gitter-Interpolation (für unregelmäßige Daten zwingend)
# Erstellt ein 60x60 Gitter aus den verstreuten Punkten
set dgrid3d 60,60 qnorm 2

# 5. Definition der 5+ Höhenlinien (Levels)
# Wir wählen aussagekräftige Schritte zwischen 305m und 330m
set cntrparam levels discrete 305, 310, 315, 320, 325, 330

# 6. Optische Verfeinerung
set grid
set palette rgbformulae 33,13,10  # Farbschema für die Legende

# Plotbefehl: Greift auf die Datei stopplace.dat zu
# Spalte 1: Long, Spalte 2: Lat, Spalte 3: Altitude
splot "stopplace.dat" using 1:2:3 with lines title "Höhenlinien [m]"
pause -1 "Press any key to exit"