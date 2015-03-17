set terminal pngcairo size 700,524 enhanced font 'Verdana,10'
set output "anconvergence.png"
set title "a_{n} vs n"
set key right bottom
set autoscale
set ylabel "a_{n}"
set xlabel "n"

plot "an.dat" using 1:2
