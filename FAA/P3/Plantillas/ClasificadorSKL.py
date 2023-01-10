# importar bibliotecas
import pandas as pd
import numpy as np
from Datos import *
from EstrategiaParticionado import *
from sklearn.model_selection import ShuffleSplit, KFold
from sklearn.naive_bayes import MultinomialNB, GaussianNB, CategoricalNB
from sklearn.preprocessing import StandardScaler
from sklearn.neighbors import KNeighborsClassifier
from sklearn.cluster import KMeans
from sklearn.linear_model import SGDClassifier

def iniciarSKL(numParticiones, porTest):

    # Proporciona los índices para dividir los datos en subconjuntos de training y test
    vSimple = ShuffleSplit(n_splits = numParticiones, test_size = porTest)
    # Genera los índices de las particiones
    vCruzada = KFold(numParticiones, shuffle=True)

    # Leemos los datos
    datasetT = Datos('../ConjuntosDatosIntroFAA/tic-tac-toe.csv')  
    X_T = datasetT.datos[:,[i for i in range(datasetT.datos.shape[1]-1)]]
    y_T = datasetT.datos[:,-1]

    datasetG = Datos('../ConjuntosDatosIntroFAA/german.csv')  
    X_G = datasetG.datos[:,[i for i in range(datasetG.datos.shape[1]-1)]]
    y_G = datasetG.datos[:,-1]

    return vSimple, vCruzada, X_T, y_T, X_G, y_G


""" Naive Bayes classifier for multinomial models.
    The multinomial Naive Bayes classifier is suitable for classification with 
    discrete features (e.g., word counts for text classification). The
    multinomial distribution normally requires integer feature counts. However,
    in practice, fractional counts such as tf-idf may also work."""
def MultinomialNBSKL(X, y, val, alpha):

    if (X is None) or (y is None) or (val is None) or (alpha < 0): return None

    errores = []
    for train_index, test_index, in val.split(X):
        
        # Infomrmation extracted from: https://github.com/scikit-learn/scikit-learn/blob/main/sklearn/naive_bayes.py

        # args: alpha: float or array-like of shape (n_features,), default=1.0
        #              Additive (Laplace/Lidstone) smoothing parameter (set alpha=0 and force_alpha=True, for no smoothing).
        #              alpha=0 no realiza ningún tipo de suavizado y alpha=1 implementa la corrección de Laplace
        #       fit_prior: bool, default=True
        #                   Whether to learn class prior probabilities or not.
        #                   El valor a priori de cada clase obtenido a partir de los datos
        multinomial = MultinomialNB(alpha=alpha)

        # args: X: array-like of shape (n_samples, n_features)
        #          Training vectors, where `n_samples` is the number of samples and `n_features` is the number of features.
        #       y : array-like of shape (n_samples,)
        #           Target values.
        try:
            multinomial.fit(X[train_index].astype(int), y[train_index].astype(int))
            errores.append(multinomial.score(X[test_index].astype(int), y[test_index].astype(int)))
        except: pass

    # Si hay error
    if len(errores) == 0:
        return 1, 0 
    
    return 1 - np.mean(errores), np.std(errores)


""" Can perform online updates to model parameters via :meth:`partial_fit`.
    La verosimilitud de las variables se asume que sigue una distribución
    normal."""
def GaussianNBSKL(X, y, val):
    
    if (X is None) or (y is None) or (val is None): return None

    errores = []
    for train_index, test_index, in val.split(X):
        # Infomrmation extracted from: https://github.com/scikit-learn/scikit-learn/blob/main/sklearn/naive_bayes.py
        gaussian = GaussianNB()

        # args: X: array-like of shape (n_samples, n_features)
        #          Training vectors, where `n_samples` is the number of samples and `n_features` is the number of features.
        #       y : array-like of shape (n_samples,)
        #           Target values.
        try:
            gaussian.fit(X[train_index].astype(int), y[train_index].astype(int))
            errores.append(gaussian.score(X[test_index].astype(int), y[test_index].astype(int)))
        except: pass

    # Si hay error
    if len(errores) == 0:
        return 1, 0 
    
    return 1 - np.mean(errores), np.std(errores)


""" The categorical Naive Bayes classifier is suitable for classification with
    discrete features that are categorically distributed. The categories of
    each feature are drawn from a categorical distribution."""
def CategoricalNBSKL(X, y, val, alpha):
    
    if (X is None) or (y is None) or (val is None) or (alpha < 0): return None

    errores = []
    for train_index, test_index, in val.split(X):
        
        # Infomrmation extracted from: https://github.com/scikit-learn/scikit-learn/blob/main/sklearn/naive_bayes.py

        # args:     alpha: float, default=1.0
        #                  Additive (Laplace/Lidstone) smoothing parameter (set alpha=0 and force_alpha=True, for no smoothing). 
        categorical = CategoricalNB(alpha = alpha)

        # args: X: array-like of shape (n_samples, n_features)
        #          Training vectors, where `n_samples` is the number of samples and `n_features` is the number of features.
        #       y : array-like of shape (n_samples,)
        #           Target values.
        try:
            categorical.fit(X[train_index].astype(int), y[train_index].astype(int))
            errores.append(categorical.score(X[test_index].astype(int), y[test_index].astype(int)))
        except: pass

    # Si hay error
    if len(errores) == 0:
        return 1, 0 
    
    return 1 - np.mean(errores), np.std(errores)


#Probar con indians-pima-diabetes, wdbc
def vecinos_proximos_SKL(X, y, k, val, normaliza):
    if (X is None) or (y is None) or (k < 0): return None
    errores = []
    for train_index, test_index, in val.split(X):
        if normaliza==True:
            escalar = StandardScaler()
            x_train=escalar.fit_transform(X[train_index].astype(int))
            x_test=escalar.fit_transform(X[test_index].astype(int))
        else: 
            x_train = X[train_index].astype(int)
            x_test = X[test_index]
        clasificador = KNeighborsClassifier(n_neighbors=k, metric='minkowski',p=2)
        clasificador.fit(x_train,y[train_index].astype(int))
        errores.append(clasificador.score(x_test.astype(int), y[test_index].astype(int)))
    return 1 - np.mean(errores), np.std(errores)


def kmeansSKL(num_clusters, datos):
    if (num_clusters is None) or (datos is None): return None

    clasificador = KMeans(n_clusters=num_clusters, init='k-means++',tol=0.0001)
    clasificador.fit(datos.datos)
    pred = clasificador.predict(datos.datos)
    centroids = clasificador.cluster_centers_
    return centroids, pred



def regresion_logistica_SKL(X, y, epocas, val, cteaprend):
    if (X is None) or (y is None) or (epocas < 0): return None
    errores = []
    for train_index, test_index, in val.split(X):
        x_train = X[train_index].astype(int)
        x_test = X[test_index]
        clasificador = SGDClassifier(max_iter=epocas, learning_rate='optimal', eta0=cteaprend)
        clasificador.fit(x_train,y[train_index].astype(int))
        errores.append(clasificador.score(x_test.astype(int), y[test_index].astype(int)))
    return 1 - np.mean(errores), np.std(errores)
