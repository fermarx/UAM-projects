from abc import ABCMeta,abstractmethod
from Datos import *


class Clasificador:
  
  # Clase abstracta
  __metaclass__ = ABCMeta
  
  def __init__(self) -> None:    
      self.lista_posteriori=[]
      self.priori=[]
  
  # Metodos abstractos que se implementan en casa clasificador concreto
  @abstractmethod
  # TODO: esta funcion debe ser implementada en cada clasificador concreto. Crea el modelo a partir de los datos de entrenamiento
  # datosTrain: matriz numpy con los datos de entrenamiento
  # nominalAtributos: array bool con la indicatriz de los atributos nominales
  # diccionario: array de diccionarios de la estructura Datos utilizados para la codificacion de variables discretas
  def entrenamiento(self,datosTrain,nominalAtributos,diccionario):
    pass
  
  
  @abstractmethod
  # TODO: esta funcion debe ser implementada en cada clasificador concreto. Devuelve un numpy array con las predicciones
  # datosTest: matriz numpy con los datos de validaci�n
  # nominalAtributos: array bool con la indicatriz de los atributos nominales
  # diccionario: array de diccionarios de la estructura Datos utilizados para la codificacion de variables discretas
  def clasifica(self,datosTest,nominalAtributos,diccionario):
    pass
  
  
  # Obtiene el numero de aciertos y errores para calcular la tasa de fallo
  def error(self,datos,pred):
    cont = 0
    for i in range(len(datos[:,-1])):
      if datos[i,-1] != float(pred[i]):
        cont +=1
    return cont/len(pred)
    
    
  # Realiza una clasificacion utilizando una estrategia de particionado determinada
  def validacion(self,particionado,dataset,clasificador,seed=None):
    lista_errores = []
    particiones = particionado.creaParticiones(dataset)
    
    for i in range(len(particiones)):
      datos_train = dataset.extraeDatos(particiones[i].indicesTrain)
      datos_test = dataset.extraeDatos(particiones[i].indicesTest)

      clasificador.entrenamiento(datos_train, dataset.nominalAtributos, dataset.diccionario)
      # El método clasifica devuelve una lista con las clases estimadas (una por cada instancia del dataset de test).
      clasificacion = clasificador.clasifica(datos_test,dataset.nominalAtributos, dataset.diccionario)
      lista_errores.append(self.error(datos_test.astype(float), clasificacion))      

    return lista_errores

