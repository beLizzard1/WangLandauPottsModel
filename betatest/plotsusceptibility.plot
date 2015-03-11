set terminal pngcairo size 700,524 enhanced font 'Verdana,10'
set output "susceptibility.png"
set title "Susceptibility vs Beta {/Symbol b}"
set key right bottom
set autoscale
set ylabel "Susceptibility"
set xlabel "Beta {/Symbol b}"
plot "susceptibility.dat" using 1:2
