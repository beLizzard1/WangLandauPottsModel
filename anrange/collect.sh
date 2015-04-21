filename='target.list'
filelines=`cat $filename`

for i in $filelines
do
	anaverage=`tail -n 30 energy$i/an.dat | awk '{ sum+=$1} END {print sum/30}'`
	energy=$i
	echo "$anaverage"
	echo "$energy"
	echo "$energy $anaverage" >> output
done

