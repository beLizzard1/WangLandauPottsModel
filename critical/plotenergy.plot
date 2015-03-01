set terminal pngcairo size 700,524 enhanced font 'Verdana,10'
set output "energy.png"
set title "Energy vs Beta"
set autoscale
set ylabel "Energy"
set xlabel "Beta {/Symbol b}"
plot "energy.dat" using 1:2:3 with yerrorbars
