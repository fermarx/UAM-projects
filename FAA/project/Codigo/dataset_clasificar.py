import numpy as np
import os
import pandas as pd
import sys

def max(f, datos, columna):
    f.write(","+str(np.max(datos.iloc[:,columna])))
    return 1

def min(f, datos, columna):
    f.write(","+str(np.min(datos.iloc[:,columna])))
    return 1
    
def media(f, datos, columna):
    f.write(","+str(np.mean(datos.iloc[:,columna])))
    return 1

def des_tipica(f, datos, columna):
    f.write(","+str(np.std(datos.iloc[:,columna])))
    return 1

def mediana(f, datos, columna):
    f.write(","+str(np.median(datos.iloc[:,columna])))
    return 1

def eliminar_sensor(datos, columna):
    df=datos.drop(datos.columns[columna], axis=1)
    return df


header="ID,MAXR2,MINR2,MAXR3,MINR3,MAXR4,MINR4,MAXR6,MINR6,MAXR7,MINR7,MAXR8,MINR8,class"

METADATA_FILE = "HT_Sensor_metadata.csv"
DATA_FILE="dataset_estimulos.csv"
OUTPUT_FILE="dataset_clasificador.csv"
def main() ->int:

    getClassFromId = {}
    with open(METADATA_FILE, "r") as f:
        next(f) # skip header
        for line in f:
            words = line.split(",")
            Id = words[0]
            Class = words[2]
            getClassFromId[Id] = Class
    f.close()

    pf = pd.read_csv(DATA_FILE, dtype={'Class':object})
    with open(OUTPUT_FILE, "w") as f:
        #Header
        f.write(header + "\n")

        #Filas que sí tengan un estímulo
        df = pf[pf.iloc[:, -1] == 1]

        #Lista de id
        id_dataset=np.unique(df.iloc[:, 0])
        #Si se van a eliminar columnas/sensores
        df=eliminar_sensor(df, 10)
        df=eliminar_sensor(df, 10)
        df=eliminar_sensor(df, 6)
        df=eliminar_sensor(df, 2)
        #Recorre las id
        for i in id_dataset:
            aux_f=df[df.iloc[:, 0] == i]
            f.write(str(i))
            #Recorre las columnas, hacer que empiece en 2 si la estructura es id, tiempo, sensores...
            for j in range(2,df.shape[1]-1):
                max(f,aux_f,j)
                min(f,aux_f,j)
                #media(f,aux_f,j)
                #des_tipica(f,aux_f,j)
                #mediana(f,aux_f,j)

            #Asigna clase 0 si es plátano y 1 si es vino
            if(getClassFromId[str(i)]=="banana"):
                f.write(",0" + "\n")
            if(getClassFromId[str(i)]=="wine"):
                f.write(",1" + "\n")
            #f.write("\n")

        f.close()


    return 0


if __name__ == "__main__":
    sys.exit(main())
