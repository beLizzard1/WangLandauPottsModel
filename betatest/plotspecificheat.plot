set terminal pngcairo size 700,524 enhanced font 'Verdana,10'
set output "specificheat.png"
set title "Specific Heat vs Beta"
set autoscale
set ylabel "Specific Heat"
set xlabel "Beta"
plot "specificheat.dat" using 1:2:3 with yerrorbars
