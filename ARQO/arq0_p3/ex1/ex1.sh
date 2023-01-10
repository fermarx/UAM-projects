# Ejemplo script, para P3 arq 2019-2020

#!/bin/bash

# inicializar variables
P=5 # team_number number
Npaso=64 # increment
Repetitons=10
Ninicio=$((10000+1024*$P)) # N
Nfinal=$((10000+1024*($P+1))) # to Nfinal

fDAT=time_slow_fast.dat
fPNG=time_slow_fast.png
fMEAN=time_slow_fast_mean.dat


# borrar el fichero DAT y el fichero PNG
rm -f $fDAT $fPNG $fMEAN

# generar el fichero DAT vacío
touch $fDAT

echo "Running slow and fast..."
# bucle para N desde P hasta Q 
#for N in $(seq $Ninicio $Npaso $Nfinal);
for ((i = 1; i <= $Repetitons; i += 1)); do
	for ((N = Ninicio ; N <= Nfinal ; N += Npaso)); do
		echo "N: $N / $Nfinal..."

		# ejecutar los programas slow y fast consecutivamente con tamaño de matriz N
		# para cada uno, filtrar la línea que contiene el tiempo y seleccionar la
		# tercera columna (el valor del tiempo). Dejar los valores en variables
		# para poder imprimirlos en la misma línea del fichero de datos
		slowTime=$(./slow $N | grep 'time' | awk '{print $3}')
		fastTime=$(./fast $N | grep 'time' | awk '{print $3}')

		# <N> <tiempo “slow”> <tiempo “fast”>
		echo "$N	$slowTime	$fastTime" >> $fDAT

	done
done

echo "Calculating means..."
for ((N = Ninicio ; N <= Nfinal ; N += Npaso)); do
    means=$(awk -v N=$((N)) '{if ($1 == N) printf ("%s %s\n",$2,$3);}'  < $fDAT | awk -v i=$((reps)) '{{s+=$1}; {f+=$2};} END {printf("%s %s\n",s/i,f/i)}')
    echo "$N $means" >> $fMEAN
done

echo "Generating plot..."
# llamar a gnuplot para generar el gráfico y pasarle directamente por la entrada
# estándar el script que está entre "<< END_GNUPLOT" y "END_GNUPLOT"
gnuplot << END_GNUPLOT
set title "Slow-Fast Execution Time"
set ylabel "Execution time (s)"
set xlabel "Matrix Size"
set key right bottom
set grid
set term png
set output "$fPNG"
plot "$fDAT" using 1:2 with lines lw 2 title "slow", \
     "$fDAT" using 1:3 with lines lw 2 title "fast", \
replot
quit
END_GNUPLOT
