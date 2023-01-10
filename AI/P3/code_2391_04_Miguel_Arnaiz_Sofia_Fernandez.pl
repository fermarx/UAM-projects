write_log(S) :- open('error_logs.txt', append, Out), write(Out, S), write(Out, '\n'), close(Out).

/***************
* EJERCICIO 1. sum_pot_prod/4
*
*	ENTRADA:
*		X: Vector de entrada de numeros de valor real.
*		Y: Vector de entrada de numeros de valor real.
*		Potencia: Numero de valor entero, potencia.
*	SALIDA:
*		Resultado: Numero de valor real resultado de la operacion sum_pot_prod.
*
****************/
sum_pot_prod(_, _, Potencia, _) :- Potencia<0, write_log('ERROR 1.1 Potencia.'), !, fail.

sum_pot_prod(X, Y, _, _) :- length(X, A), length(Y, B), A=\=B, write_log('ERROR 1.2 Longitud.'), !, fail.

sum_pot_prod([AX|[]], [AY|[]], Potencia, Resultado) :-
    Resultado is (AX*AY)^Potencia.

sum_pot_prod([AX|BX], [AY|BY], Potencia, Resultado) :-
    sum_pot_prod(BX, BY, Potencia, Resultado2),
    Resultado is Resultado2+(AX*AY)^Potencia.




/***************
* EJERCICIO 2. segundo_penultimo/3
*
*       ENTRADA:
*               L: Lista de entrada de numeros de valor real.
*       SALIDA:
*               X : Numero de valor real. Segundo elemento.
*		Y : Numero de valor real. Penultimo elemento.
*
****************/
segundo_penultimo(L, _, _) :- length(L,A), A=<1, write_log('ERROR 1.1 Longitud.'), !, fail.

segundo_penultimo([AL, BL|[]], X, Y) :-
X is BL, Y is AL.

segundo_penultimo([_, BL|CL], X, Y) :-
length(CL, 1),
Y is BL,
X is BL.

segundo_penultimo([_, BL|CL], X, Y) :-
X is BL,
length(CL, A), A>=2,
penultimo(CL, _, Y).

penultimo([AL|BL], _, Y) :-
length(BL, 1), Y is AL.

penultimo([_|BL], _, Y) :-
length(BL, 2), BL is [ABL|_], Y is ABL.

penultimo([_|BL], _, Y) :-
length(BL, A), A>2, penultimo(BL, _, Y).



/***************
* EJERCICIO 3. sublista/5
*
*       ENTRADA:
*		L: Lista de entrada de cadenas de texto.
*		Menor: Numero de valor entero, indice inferior.
*		Mayor: Numero de valor entero, indice superior.
*		E: Elemento, cadena de texto.
*       SALIDA:
*		Sublista: Sublista de salida de cadenas de texto.
*
****************/
sublista(_, Menor, Mayor, _, _) :- Mayor<Menor, write_log('ERROR 3.2 Indices.'), !, fail.

sublista([AL|BL], Menor, Mayor, E, Sublista) :-
Menor =:= 1,
sublista(BL, Menor-1, Mayor-1, E, SublistaResult),
check_error([AL|SublistaResult], E),
append([AL], SublistaResult, Sublista).

sublista([AL|BL], Menor, Mayor, E, Sublista) :-
Menor>1,
sublista(BL, Menor-1, Mayor-1, E, Sublista).

sublista([AL|BL], Menor, Mayor, E, Sublista) :-
Mayor=:=1,
append([AL], [], Sublista).

sublista([AL|BL], Menor, Mayor, E, Sublista) :-
Mayor>1,
sublista(BL, Menor-1, Mayor-1, E, SublistaResult),
append([AL], SublistaResult, Sublista).

check_error(Sublista, E) :- not(member(E, Sublista)), write_log('ERROR 3.1 Elemento.'), !, fail.



/***************
* EJERCICIO 4. espacio_lineal/4
*
*       ENTRADA:
*               Menor: Numero de valor entero, valor inferior del intervalo.
*               Mayor: Numero de valor entero, valor superior del intervalo.
*               Numero_elementos: Numero de valor entero, numero de valores de la rejilla.
*       SALIDA:
*               Rejilla: Vector de numeros de valor real resultante con
*               la rejilla.
*
****************/
espacio_lineal(Menor, Mayor, _, _) :- Mayor<Menor, write_log('ERROR 4.1 Indices.'), !, fail.

espacio_lineal(Menor, Mayor, Numero_elementos, Rejilla) :-
    Div is (Mayor-Menor)/(Numero_elementos - 1),
    espacio_lineal_aux(Menor, Mayor, Div, RejillaAux),
    append([Menor], RejillaAux, Rejilla).

espacio_lineal_aux(UltimoElem, Mayor, Div, Rejilla):-
    UltimoElem=:=Mayor,
    append([], [], Rejilla).

espacio_lineal_aux(UltimoElem, Mayor, Div, Rejilla):-
    Res is UltimoElem+Div,
    espacio_lineal_aux(Res, Mayor, Div, RejillaAux),
    append([Res], RejillaAux, Rejilla).


/***************
* EJERCICIO 5. normalizar/2
*
*       ENTRADA:
*		Distribucion_sin_normalizar: Vector de numeros reales de entrada. Distribucion sin normalizar.
*       SALIDA:
*		Distribucion: Vector de numeros reales de salida. Distribucion normalizada.
*
****************/
error([AL|_]) :- AL<0, write_log('ERROR 5.1. Negativos.'), !, fail.
error([AL|_]) :- AL>=0.

normalizar([AL|BL], Distribucion) :-
    normalizar_suma([AL|BL], Z),
    normalizar_division([AL|BL], Z, Distribucion).

normalizar_suma([], 0).
normalizar_suma([AL|BL], Z):-
    error([AL|BL]),
    normalizar_suma(BL, Z2),
    Z is Z2 + AL.

normalizar_division([AL|[]], Z, Distribucion) :-
    Res is AL/Z,
    append([Res], [], Distribucion).

normalizar_division([AL|BL], Z, Distribucion) :-
    Res is AL/Z,
    normalizar_division(BL, Z, Distribucion2),
    append([Res], Distribucion2, Distribucion).

/***************
* EJERCICIO 6. divergencia_kl/3
*
*       ENTRADA:
*		D1: Vector de numeros de valor real. Distribucion.
*		D2: Vector de numeros de valor real. Distribucion.
*       SALIDA:
*		KL: Numero de valor real. Divergencia KL.
*
****************/
divergencia_kl([AD1|_], [_|_], _) :- AD1=<0.0, write_log('ERROR 6.1. Divergencia KL no definida.'), !, fail.
divergencia_kl([_|_], [AD2|_], _) :- AD2=<0.0, write_log('ERROR 6.1. Divergencia KL no definida.'), !, fail.


divergencia_kl([AD1|[]], [AD2|[]], KL) :-
    KL is AD1*log(AD1/AD2).

divergencia_kl([AD1|BD1], [AD2|BD2], KL) :-
    divergencia_kl(BD1, BD2, KLaux),
    KL is KLaux+AD1*log(AD1/AD2).




/***************
* EJERCICIO 7. producto_kronecker/3
*
*       ENTRADA:
*		Matriz_A: Matriz de numeros de valor real.
*		Matriz_B: Matriz de numeros de valor real.
*       SALIDA:
*		Matriz_bloques: Matriz de bloques (matriz de matrices) de numeros reales.
*
****************/
producto_kronecker(Matriz_A, Matriz_B, Matriz_bloques) :- print('Error. Este ejercicio no esta implementado todavia.'), !, fail.

/***************
* EJERCICIO 8a. distancia_euclidea/3
*
*       ENTRADA:
*               X1: Vector de numeros de valor real.
*               X2: Vector de numeros de valor real.
*       SALIDA:
*               D: Numero de valor real. Distancia euclidea.
*
****************/
distancia_euclidea(X1, X2, D) :- print('Error. Este ejercicio no esta implementado todavia.'), !, fail.

/***************
* EJERCICIO 8b. calcular_distancias/3
*
*       ENTRADA:
*               X_entrenamiento: Matriz de numeros de valor real donde cada fila es una instancia representada por un vector.
*               X_test: Matriz de numeros de valor real donde cada fila es una instancia representada por un vector. Instancias sin etiquetar.
*       SALIDA:
*               Matriz_resultados: Matriz de numeros de valor real donde cada fila es un vector con la distancia de un punto de test al conjunto de entrenamiento X_entrenamiento.
*
****************/
calcular_distancias(X_entrenamiento, X_test, Matriz_resultados) :- print('Error. Este ejercicio no esta implementado todavia.'), !, fail.

/***************
* EJERCICIO 8c. predecir_etiquetas/4
*
*       ENTRADA:
*               Y_entrenamiento: Vector de valores alfanumericos de una distribucion categorica. Cada etiqueta corresponde a una instancia de X_entrenamiento.
*               K: Numero de valor entero.
*               Matriz_resultados: Matriz de numeros de valor real donde cada fila es un vector con la distancia de un punto de test al conjunto de entrenamiento X_entrenamiento.
*       SALIDA:
*               Y_test: Vector de valores alfanumericos de una distribucion categorica. Cada etiqueta corresponde a una instancia de X_test.
*
****************/
predecir_etiquetas(Y_entrenamiento, K, Matriz_resultados, Y_test) :- print('Error. Este ejercicio no esta implementado todavia.'), !, fail.

/***************
* EJERCICIO 8d. predecir_etiqueta/4
*
*       ENTRADA:
*               Y_entrenamiento: Vector de valores alfanumericos de una distribucion categorica. Cada etiqueta corresponde a una instancia de X_entrenamiento.
*               K: Numero de valor entero.
*               Vec_distancias: Vector de valores reales correspondiente a una fila de Matriz_resultados.
*       SALIDA:
*               Etiqueta: Elemento de valor alfanumerico.
*
****************/
predecir_etiqueta(Y_entrenamiento, K, Vec_distancias, Etiqueta) :- print('Error. Este ejercicio no esta implementado todavia.'), !, fail.

/***************
* EJERCICIO 8e. calcular_K_etiquetas_mas_relevantes/4
*
*       ENTRADA:
*               Y_entrenamiento: Vector de valores alfanumericos de una distribucion categorica. Cada etiqueta corresponde a una instancia de X_entrenamiento.
*               K: Numero de valor entero.
*               Vec_distancias: Vector de valores reales correspondiente a una fila de Matriz_resultados.
*       SALIDA:
*		K_etiquetas: Vector de valores alfanumericos de una distribucion categorica.
*
****************/
calcular_K_etiquetas_mas_relevantes(Y_entrenamiento, K, Vec_distancias, K_etiquetas) :- print('Error. Este ejercicio no esta implementado todavia.'), !, fail.

/***************
* EJERCICIO 8f. calcular_etiqueta_mas_relevante/2
*
*       ENTRADA:
*               K_etiquetas: Vector de valores alfanumericos de una distribucion categorica.
*       SALIDA:
*               Etiqueta: Elemento de valor alfanumerico.
*
****************/
calcular_etiqueta_mas_relevante(K_etiquetas, Etiqueta) :- print('Error. Este ejercicio no esta implementado todavia.'), !, fail.

/***************
* EJERCICIO 8g. k_vecinos_proximos/5
*
*       ENTRADA:
*		X_entrenamiento: Matriz de numeros de valor real donde cada fila es una instancia representada por un vector.
*		Y_entrenamiento: Vector de valores alfanumericos de una distribucion categorica. Cada etiqueta corresponde a una instancia de X_entrenamiento.
*		K: Numero de valor entero.
*		X_test: Matriz de numeros de valor real donde cada fila es una instancia representada por un vector. Instancias sin etiquetar.
*       SALIDA:
*		Y_test: Vector de valores alfanumericos de una distribucion categorica. Cada etiqueta corresponde a una instancia de X_test.
*
****************/
k_vecinos_proximos(X_entrenamiento, Y_entrenamiento, K, X_test, Y_test) :- print('Error. Este ejercicio no esta implementado todavia.'), !, fail.
