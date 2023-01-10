import numpy as np

def matriz_confusion(datos, pred):
    aux = np.empty([2,2], dtype=int)
    aux*=0

    for i in range(len(datos[:,-1])):
    #Verdaderos positivos
        if pred[i]==1 and pred[i]==datos[i,-1]: aux[0,0]+=1
    #Verdaderos negativos  
        elif pred[i]==0 and pred[i]==datos[i,-1]: aux[1,1]+=1    
    #Falso positivo
        elif pred[i]==1 and pred[i]!=datos[i,-1]: aux[1,0]+=1
    #Falso negativo
        else: aux[0,1]+=1
    return aux


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

