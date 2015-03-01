set terminal pngcairo size 700,524 enhanced font 'Verdana,10'
set output "energy.png"
set title "Energy vs Beta {/Symbol b}"
set key right bottom
set autoscale
set ylabel "Energy"
set xlabel "Beta {/Symbol b}"

f(x) = ( 1 - (sinh(a1 * x)**(-4) ) )**(1/8)
a1 = 1;
a2 = -1;
fit f(x) 'energy.dat' using 1:2 via a1

plot "energy.dat" using 1:2:3 with yerrorbars,\
a2 * ( 1 - (sinh(a1 * x)**(-4) ) )**(1/8)

