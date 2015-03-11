set terminal pngcairo size 700,524 enhanced font 'Verdana,10'
set output "specificheat.png"
set title "Specific Heat vs Beta {/Symbol b}"
set key right bottom
set autoscale
set ylabel "Specific Heat"
set xlabel "Beta {/Symbol b}"
plot "specificheat.dat" using 1:2
