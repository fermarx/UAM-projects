# Ejemplo script, para P3 arq 2019-2020

#!/bin/bash

# inicializar variables
P=5 # team_number number
Npaso=64 # increment
Ninicio=$((2000+512*$P)) # N
Nfinal=$(( 2000+512*($P+1))) # to Nfinal
cachemin=1024
cachemax=8192
fDAT=cache_
freadPNG=cache_lectura.png
fwritePNG=cache_escritura.png
fCACHE=cache2.dat

# borrar el fichero DAT y el fichero PNG
rm -f $fDAT*.dat $fwPNG $frPNG $fCACHE

echo "Running ex2 cache misses R/W..."

for ((i=cachemin; i<=cachemax; i=i*2)); do
	echo "Cache size: $i"
	# bucle para N desde P hasta Q 
	#for N in $(seq $Ninicio $Npaso $Nfinal);
	for ((N = Ninicio ; N <= Nfinal ; N += Npaso)); do
		echo "N: $N / $Nfinal..."

        valgrind --tool=cachegrind --I1=$i,1,64 --D1=$i,1,64 --LL=8192,1,64 --cachegrind-out-file=$fCACHE ./slow $N &> /dev/null
    	slowMisses=$(cg_annotate $fCACHE | head -n 30 | awk '{if($NF == "TOTALS") printf("%s %s\n", $5, $8)}' | sed 's/,//g')
        rm $fCACHE
        
		valgrind --tool=cachegrind --I1=$i,1,64 --D1=$i,1,64 --LL=8192,1,64 --cachegrind-out-file=$fCACHE ./fast $N &> /dev/null
    	fastMisses=$(cg_annotate $fCACHE | head -n 30 | awk '{if($NF == "TOTALS") printf("%s %s\n", $5, $8)}' | sed 's/,//g')
        rm $fCACHE
    	
		echo "$N $slowMisses $fastMisses" >> $fDAT$i.dat
	done
done

echo "Generating plot..."
# llamar a gnuplot para generar el gráfico y pasarle directamente por la entrada
# estándar el script que está entre "<< END_GNUPLOT" y "END_GNUPLOT"
gnuplot << END_GNUPLOT
set title "Slow-Fast Read Misses"
set ylabel "Read Misses"
set xlabel "Matrix Size"
set key right bottom
set grid
set term png
set output "$freadPNG"
increment = "1024 2048 4096 8192"
plot for [cache in increment] "$fDAT".cache.".dat" using 1:2 with lines lw 2 title cache_"{slow}", \
     for [cache in increment] "$fDAT".cache.".dat" using 1:4 with lines lw 2 title cache_"{fast}"
replot
quit
END_GNUPLOT
gnuplot << END_GNUPLOT
set title "Slow-Fast Write Misses"
set ylabel "Write Misses"
set xlabel "Matrix Size"
set key right bottom
set grid
set term png
set output "$fwritePNG"
increment = "1024 2048 4096 8192"
plot for [cache in increment] "$fDAT".cache.".dat" using 1:3 with lines lw 2 title cache_"{slow}", \
     for [cache in increment] "$fDAT".cache.".dat" using 1:5 with lines lw 2 title cache_"{fast}"
replot
quit
END_GNUPLOT
