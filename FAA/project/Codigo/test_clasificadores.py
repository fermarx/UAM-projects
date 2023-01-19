from clasificador import Clasificador

from sklearn.tree import DecisionTreeClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.linear_model import LogisticRegression
import matplotlib.pyplot as plt

datasetFiles = {'dataset_clasificador.csv','dataset_estimulos.csv'}
#datasetFiles = ['dataset_estimulos.csv']
#datasetFiles = ['dataset_clasificador.csv']
numPruebas = 100



for datasetFile in datasetFiles:

    # Introducir en la lista los clasificadores que queremos probar. Deben ser clasificadores con una funcion 
    # fit(X, y) y una funcion predict(X) como las implementadas en sklearn
    clasificadores = [DecisionTreeClassifier(), KNeighborsClassifier(), RandomForestClassifier(n_estimators=10), GaussianNB(), LogisticRegression(max_iter=1000)]

    for clasificador in clasificadores:
        print('Clasificador: ' + str(clasificador) + '. Dataset ' + datasetFile)

        acc = 0
        for i in range(numPruebas):
            clasificadorPropio = Clasificador(clasificador, datasetFile)
            clasificadorPropio.train()
            acc += clasificadorPropio.test()

            #clasificadorPropio.get_confusion_matrix(str(clasificador).split('(')[0] + '_' + datasetFile.split('.')[0])
            #clasificadorPropio.get_roc_curve(str(clasificador).split('(')[0] + '_' + datasetFile.split('.')[0])
        acc /= numPruebas

        print('>> Accuracy on test set: ' + str(acc) + '%' + '\n')
