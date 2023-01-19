
from EstrategiaParticionado import ValidacionSimple
import pandas as pd
import os
from sklearn.metrics import confusion_matrix
from sklearn.metrics import RocCurveDisplay
import matplotlib.pyplot as plt
import seaborn as sns

class Clasificador:

    # dataset es el nombre del archivo con los datos
    # clasificador es una clase con una funcion fit(X, y) y una funcion predict(X) como las implementadas en sklearn
    # si queremos implementar nosotros un clasificador que no este en sklearn, solo tenemos que crear una clase con estas dos funciones
    def __init__(self, clasificador, dataFile, estrategiaParticionado=ValidacionSimple(0.8, 1)):
        self.clasificador = clasificador

        df = pd.read_csv(os.path.dirname(__file__) + '/' + dataFile)
        datos = df.to_numpy()
        particion = estrategiaParticionado.creaParticiones(datos)

        # En el futuro podriamos cambiar esto para que utilice todas las posibles particiones. Aqui estoy usando solo la primera. 
        self.train_set = datos[particion.indicesTrain[0]]
        self.test_set = datos[particion.indicesTest[0]]

    def train(self):
        self.clasificador = self.clasificador.fit(self.train_set[:, :-1], self.train_set[:, -1])

    def get_confusion_matrix(self, name):
        prediction = self.clasificador.predict(self.test_set[:, :-1])
        # set prediction to int
        prediction = [int(x) for x in prediction]
        real = [int(x) for x in self.test_set[:, -1]]

        labels_list = ['banana', 'wine']

        matrix = confusion_matrix(real, prediction)

        data = pd.DataFrame(matrix, index=labels_list, columns=labels_list)
        plt.figure(figsize=(8, 8))
        sns.heatmap(data, annot=True)
        plt.ylabel('True label')
        plt.xlabel('Predicted label')
        plt.title('Test set confusion matrix. ' + name)
        
        plt.savefig('figures/confusion_matrix_' + name + '.png')

    def get_roc_curve(self, name):
        RocCurveDisplay.from_estimator(self.clasificador, self.test_set[:, :-1], self.test_set[:, -1])
        plt.savefig('figures/roc_curve_' + name + '.png')

    def test(self):
        prediction = self.clasificador.predict(self.test_set[:, :-1])

        total = len(prediction)
        correct = 0
        for pred, real in zip(prediction, self.test_set[:, -1]):
            correct += pred == real

        return correct/total*100
    
    def predict(self, X):
        return self.clasificador.predict(X)

    
