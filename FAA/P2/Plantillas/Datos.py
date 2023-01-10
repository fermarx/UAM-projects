# -*- coding: utf-8 -*-

# coding: utf-8
from queue import Empty
import pandas as pd
import numpy as np


class Datos:

    # Constructor: procesar el fichero para asignar correctamente las variables nominalAtributos, datos y diccionarios
    def __init__(self, nombreFichero):  

        if (nombreFichero is None): return

        pf = pd.read_csv(nombreFichero, dtype={'Class':object})
        self.nominalAtributos = []
        self.dicAux = {}
        self.diccionario = {}

        for i, col in enumerate(pf.columns):

            # Guarda el primer dato de cada columna para saber si es nominal o no
            dato_columna=pf.iloc[0][col]
            # Crea la lista de valores donde se incluye True si el atributo es nominal o False en caso de que no lo sea
            if (type(dato_columna) == str):
                self.nominalAtributos.append(True)
            elif (type(dato_columna) != object):
                self.nominalAtributos.append(False)
            else:
                return ValueError

            # Crear el diccionario, tiene la forma de: {"Nombre_Columna": {"nombre-original": valor_numerico}}
            # Ordena en orden alfabetico los nombres de las columnas(Nombre_Columna), para que sea más legible al leerlo y esté mas ordenado
            valores = np.unique(pf.iloc[:, i])
            valores.sort()
            # Crea la llave "Nombre_Columna" si no existía antes
            if dato_columna not in self.dicAux:
                self.dicAux[pf.columns[i]] = {}
                self.diccionario[pf.columns[i]] = {}
            # Crea un valor en cada "Nombre_Columna". Este valor es otro diccionario, donde la llave es el nombre original y el valor, un valor numerico
            # Si el valor original es numerico, se queda con el valor original (o directamente una clave vacia?) AL MENOS ESO ES LO QUE YO HE ENTIENDIDO, mira el german.csv como funciona
            if self.nominalAtributos[i] == False:
                for valor, clave in enumerate(valores):
                    self.dicAux[pf.columns[i]][clave] = []
                    self.diccionario[pf.columns[i]] = {}
            else:
                for valor, clave in enumerate(valores):
                    self.dicAux[pf.columns[i]][clave] = float(valor)
                    self.diccionario[pf.columns[i]][clave] = int(valor)
        
        # Crea la misma tabla que en el csv pero con los valores nominales cambiados por los diccionarios
        self.datos = np.empty((len(pf.index), len(pf.columns)),dtype=float)
        # Itera las filas del archivo inicial para guardar los datos numericos en el array
        for i, nom in enumerate(pf.iterrows()):
            for j, col in enumerate(nom[1].items()):
                if self.dicAux[col[0]][col[1]] == []:
                    self.datos[i][j] = float(col[1])
                else: 
                    self.datos[i][j] = float(self.dicAux[col[0]][col[1]])

        

    # Devuelve el subconjunto de los datos cuyos �ndices se pasan como argumento
    def extraeDatos(self,idx):
        if idx is None: return None
        return self.datos[idx,:]

