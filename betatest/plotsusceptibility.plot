set terminal pngcairo size 700,524 enhanced font 'Verdana,10'
set output "susceptibility.png"
set title "Susceptibility vs Beta"
set autoscale
set ylabel "Susceptibility"
set xlabel "Beta"
plot "susceptibility.dat" using 1:2:3 with yerrorbars
