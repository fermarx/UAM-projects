#!/bin/bash

# compila los programas slow y fast si no están compilados.
make &> /dev/null
# inicializar variables
reps=10
P=5
Ninicio=$((256+256*$P))
Npaso=16
Nfinal=$((256+256*($P+1)))
fDAT=mult.dat
ftPNG=mult_time.png
fcPNG=mult_cache.png
fCACHE=cache3.dat

# borrar el fichero DAT y el fichero PNG
rm -f $fDAT $fcPNG $ftPNG

# generar el fichero DAT vacío
touch $fDAT

echo "Running normal and transposed..."

for ((N = Ninicio ; N <= Nfinal ; N += Npaso)); do
	echo "N: $N / $Nfinal..."

    multTime=$(valgrind --tool=cachegrind --cachegrind-out-file=$fCACHE ./matrix $N 2> /dev/null | grep 'time' | awk '{print $3}')
	multMisses=$(cg_annotate $fCACHE | head -n 30 | awk '{if($NF == "TOTALS") printf("%s %s\n", $5, $8)}' | sed 's/,//g')
    rm $fCACHE
    transTime=$(valgrind --tool=cachegrind --cachegrind-out-file=$fCACHE ./matrixT $N 2> /dev/null | grep 'time' | awk '{print $3}')
	transMisses=$(cg_annotate $fCACHE | head -n 30 | awk '{if($NF == "TOTALS") printf("%s %s\n", $5, $8)}' | sed 's/,//g')
    rm $fCACHE
	echo "$N $multTime $multMisses $transTime $transMisses" >> $fDAT
done

echo "Generating plots..."
gnuplot << END_GNUPLOT
set title "Normal-Trans Cache Misses"
set ylabel "Cache misses"
set xlabel "Matrix Size"
set key right bottom
set grid
set term png
set output "$fcPNG"
plot "$fDAT" using 1:3 with lines lw 2 title "normal read", \
     "$fDAT" using 1:4 with lines dt 2 title "normal write",\
     "$fDAT" using 1:6 with lines lw 2 title "trans read",\
     "$fDAT" using 1:7 with lines dt 2 title "trans write"
replot
quit
END_GNUPLOT
gnuplot << END_GNUPLOT
set title "Normal-Trans Execution Time"
set ylabel "Execution time (s)"
set xlabel "Matrix Size"
set key right bottom
set grid
set term png
set output "$ftPNG"
plot "$fDAT" using 1:2 with lines lw 2 title "normal", \
     "$fDAT" using 1:5 with lines lw 2 title "trans"
replot
quit
END_GNUPLOT