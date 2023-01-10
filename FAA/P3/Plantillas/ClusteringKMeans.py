
import collections
import random
import numpy as np
import sys
from Datos import *


def centro_masas(cluster):
    #Cluster es una lista de datos
    dic_centro_masa={}
    for key, value in cluster.items():
        val = np.array(value)
        dic_centro_masa[key]=[]
        lista_centro_masa=[]
        for col in range(val.shape[1]):
            lista_centro_masa.append(sum(val[:,col])/len(val[:,col]))
        dic_centro_masa[key]=lista_centro_masa
    return dic_centro_masa


def kmeans(k, datos):
    # Elegir k centroides aleatorios no repetidos
    clusters={}
    centroides=[]
    lista_centroide=[]
    n=0
    #Centroides contiene los números de las filas
    centroides=random.sample(range(datos.datos.shape[0]), k)
    end = False
    #Diccionario clusters{1:fila centroide,...} 
    # choose k data points as the initial centroids (cluster centers)
    for num_cluster, centroide in enumerate(centroides):
        for col in range(datos.datos.shape[1]-1):
            lista_centroide.append(float(datos.datos[centroide][col]))
        clusters[num_cluster]=lista_centroide
        lista_centroide=[]

    while end==False:
        clusters_last_iter = clusters.copy()
        diccionario_cluster={}
        #Asignar cada uno del resto de patrones al centroide más cercano (atendiendo a la distancia euclídea)
        for idx, fila in enumerate(datos.datos):
            min=sys.maxsize
            cluster_min=None
            for num_cluster, cluster in clusters.items():
                # compute the distance from X to each centroid:
                dist=np.linalg.norm(fila[:-1].astype(float)-np.array(cluster, dtype=float))
                if min>dist:
                    min=dist
                    cluster_min=num_cluster
            if cluster_min not in diccionario_cluster.keys(): diccionario_cluster[cluster_min]=[]
            diccionario_cluster[cluster_min].append(fila[:-1].astype(float).tolist())
        clusters=centro_masas(diccionario_cluster)
        # re-compute the centroid using the current cluster membersh1ps
        if clusters == clusters_last_iter:
            end = True
            return clusters, diccionario_cluster, n
        n+=1

