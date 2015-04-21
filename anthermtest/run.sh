rm -R -- */
rm *.dat

dimgrid=24
#filename='averagecriticalenergies.list'
filename='q8only.list'

while read -r i
do
	qvalue=`echo $i |awk '{print $1}'`
	target=`echo $i |awk '{print $2}'`
	echo "Running $qvalue"
	mkdir $qvalue
	cd $qvalue
	cp ../param.cfg .
	sed -i '/dim_q\s=\s.*/c\dim_q = '$qvalue'' param.cfg
	sed -i '/dim_grid\s=\s.*/c\dim_grid = '$dimgrid'' param.cfg
	sed -i '/target_e\s=\s.*/c\target_e = '$target'' param.cfg
	sed -i '/target_width\s=\s.*/c\target_width = '$dimgrid'.0' param.cfg
	../potts.app param.cfg
	cd ..
done < "$filename"
