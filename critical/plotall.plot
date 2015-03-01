set terminal pngcairo size 1750,1310 enhanced font 'Verdana,10'
set output 'MultipleGridSizes.png'
set autoscale

set key right bottom

set title 'Magnetisation vs Beta ({/Symbol b}) various Grid Sizes'
set xlabel '{/Symbol b} (1/{/Symbol k}T)'
set ylabel 'Magnetisation/Volume'
plot for [i=10:25:5] 'dim'.i.'-magnetisation.dat' using 1:2:3 with yerrorbars


