import numpy as np
import pandas as pd
from EstrategiaParticionado import *
from ClasificadorKNN import *
from ClasificadorRL import *
from ClasificadorNB import *


def matriz_confusion(pred, datos):
    vp, vn, fp, fn, = (0, 0, 0, 0)

    for p, d in zip(pred, datos):
        if d == 1:
            if p == d: vp += 1
            else: fn += 1
        else:
            if p == d: vn += 1
            else:fp += 1
    return vp, vn, fp, fn


def matriz_confusion_diccionario(datos, pred, num_clases):
    if(num_clases!=len(pred.keys())): return None
    matriz = np.empty([num_clases,num_clases], dtype=int)
    matriz*=0
    for fila in datos:
        for key, valor in pred.items():
            val = np.array(valor)
            for fila_matriz in val:
                if np.array_equal(fila_matriz, fila[:-1]):
                #Verdaderos positivos
                    if key==fila[-1]: matriz[key,key]+=1
                    else: matriz[fila[-1].astype(int),key]+=1
                    break    
    return matriz


def espacioROC(dataset, clasificador):
    vSimple = ValidacionSimple(10, 20)
    particiones = vSimple.creaParticiones(dataset)

    datos_train  = dataset.datos[particiones[0].indicesTrain]
    datos_test = dataset.datos[particiones[0].indicesTest]

    clasificador.entrenamiento(datos_train, dataset.nominalAtributos, dataset.diccionario)
    clasificacion = clasificador.clasifica(datos_test, dataset.nominalAtributos, dataset.diccionario)
    datos = dataset.datos[:, -1][particiones[0].indicesTest]

    vp, vn, fp, fn =  matriz_confusion(clasificacion, datos)
    tpf = vp/(vp+fn)
    fpr = fp/(vn+fp)
    return tpf, fpr


def matriz_ROC(dataset, clasificador):
    fpr_list = []
    tpr_list = []
    for i in range(10):
        clas = clasificador
        tpr, fpr = espacioROC(dataset, clas)
        tpr_list.append(tpr)
        fpr_list.append(fpr)
    return np.mean(tpr_list), np.mean(fpr_list) 