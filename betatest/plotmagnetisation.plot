set terminal pngcairo size 700,524 enhanced font 'Verdana,10'
set output "magnetisation.png"
set title "Magnetisation vs Beta"
set autoscale
set ylabel "Magnetisation"
set xlabel "Beta"
plot "magnetisation.dat" using 1:2:3 with yerrorbars
