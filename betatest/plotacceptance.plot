set terminal pngcairo size 700,524 enhanced font 'Verdana,10'
set output "Acceptance.png"
set title "Acceptance vs Beta"
set autoscale
set ylabel "Acceptance"
set xlabel "Beta"
plot "acceptance.dat" using 1:2
