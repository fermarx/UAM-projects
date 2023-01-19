from abc import ABCMeta, abstractmethod
import random


class Particion():

    # Esta clase mantiene la lista de �ndices de Train y Test para cada partici�n del conjunto de particiones
    def __init__(self, indicesTrain, indicesTest):
        self.indicesTrain = indicesTrain
        self.indicesTest = indicesTest

    #####################################################################################################


class EstrategiaParticionado:

    # Clase abstracta
    __metaclass__ = ABCMeta

    # Atributos: deben rellenarse adecuadamente para cada estrategia concreta. Se pasan en el constructor

    @abstractmethod
    # TODO: esta funcion deben ser implementadas en cada estrategia concreta
    def creaParticiones(self, datos, seed=None):
        pass

    #####################################################################################################


class ValidacionSimple(EstrategiaParticionado):

    # Establecemos la proporcion de datos para el training con respecto del total. 80% seria 0.8
    def __init__(self, proporcion_train, k):
        super().__init__()

        self.proporcion = proporcion_train
        self.k = k

    # Crea particiones segun el metodo tradicional de division de los datos segun el porcentaje deseado y el n�mero de ejecuciones deseado
    # Devuelve una lista de particiones (clase Particion)
    def creaParticiones(self, datos, seed=None):
        random.seed(seed)
        indicesTrain = []
        indicesTest = []
        for i in range(self.k):
            num_datos = datos.shape[0]
            indices = list(range(num_datos))
            random.shuffle(indices)
            num_datos_training = int(num_datos*self.proporcion)
            indicesTrain.append(indices[:num_datos_training])
            indicesTest.append(indices[num_datos_training:])

        return Particion(indicesTrain, indicesTest)

    #####################################################################################################


class ValidacionCruzada(EstrategiaParticionado):

    # Establecemos el valor k que indica la cantidad de grupos en los que dividir los datos
    def __init__(self, k):
        super().__init__()
        self.k = k

    # Crea particiones segun el metodo de validacion cruzada.
    # El conjunto de entrenamiento se crea con las nfolds-1 particiones y el de test con la particion restante
    # Esta funcion devuelve una lista de particiones (clase Particion)
    def creaParticiones(self, datos, seed=None):
        random.seed(seed)
        num_datos = datos.shape[0]
        indices = list(range(num_datos))
        random.shuffle(indices)

        step = int(num_datos/self.k)
        
        indicesTrain = []
        indicesTest = []
        for i in range(self.k - 1):
            indicesTrain.append(indices[:i*step] + indices[(i+1)*step:])
            indicesTest.append(indices[i*step: (i+1)*step])
            
        # El ultimo grupo puede contener menos elementos
        indicesTrain.append(indices[:(self.k-1)*step])
        indicesTest.append(indices[(self.k-1)*step:])
        
        return Particion(indicesTrain, indicesTest)
