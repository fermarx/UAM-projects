from abc import ABCMeta,abstractmethod
import random
from Datos import Datos 
import numpy as np


class Particion():
  
  # Esta clase mantiene la lista de �ndices de Train y Test para cada partici�n del conjunto de particiones  
  def __init__(self):
    self.indicesTrain=[]
    self.indicesTest=[]


class EstrategiaParticionado:
  
  # Clase abstracta
  __metaclass__ = ABCMeta

  # Atributos: cada estrategia es diferente. Se pasan en el constructor 
  def __init__(self):
    self.particiones = []

  @abstractmethod
  def creaParticiones(self,datos,seed=None):
    pass
  

class ValidacionSimple(EstrategiaParticionado):
  
  # Constructor que dividirá los datos de entrada en dos conjuntos, uno para entrenamiento y otro para prueba en función del porcentaje que se especifique
  # Args: 
  #     numeroEjec: Numero de ejecuciones
  #     porTest: Porcentaje para dividir los conjuntos de entrenamiento 
  def __init__(self, numeroEjec, porTest):
    
    self.numeroEjec = numeroEjec
    self.porTest = porTest       
    super().__init__()

  # Crea particiones segun el metodo tradicional de division de los datos segun el porcentaje deseado y el n�mero de ejecuciones deseado
  # Devuelve una lista de particiones (clase Particion)
  def creaParticiones(self,datos,seed=None):
    random.seed(seed)
    self.particiones = []

    # Calculando el porcentaje del conjunto test
    filas = datos.datos.shape[0]
    porcentaje = (self.porTest*filas)//100
    
    # Creando lista de indices para usar luego para apuntar a la matriz
    indices = list(range(filas))

    for rep in range(0, self.numeroEjec):
      # Haciendo que la lista sea ordenada aleatoriamente
      indicesAleatorios = np.random.permutation(indices)
      
      # Creamos una particion nueva le adjudicamos los indices aleatorios segun el porcentaje
      newParticion = Particion()
      newParticion.indicesTrain = indicesAleatorios[porcentaje:]
      newParticion.indicesTest = indicesAleatorios[:porcentaje]

      # Añadimos la particion a la lista de particiones
      self.particiones.append(newParticion)
      
    return self.particiones

        
      
class ValidacionCruzada(EstrategiaParticionado):

  # Constructor que dividirá los datos de entrada en dos conjuntos, uno para entrenamiento y otro para prueba en función del porcentaje que se especifique
  # Args: 
  #     numParticiones: Numero de particiones
  def __init__(self, numParticiones):
    
    self.numParticiones = numParticiones
    super().__init__()


  # Crea particiones segun el metodo de validacion cruzada.
  # El conjunto de entrenamiento se crea con las nfolds-1 particiones y el de test con la particion restante
  # Esta funcion devuelve una lista de particiones (clase Particion)
  def creaParticiones(self,datos,seed=None):   
    random.seed(seed)
    self.particiones = []

    filas = datos.datos.shape[0]
    
    # Creando lista de indices para usar luego para apuntar a la matriz
    indices = list(range(filas))

    # Haciendo que la lista sea ordenada aleatoriamente
    indicesAleatorios = np.random.permutation(indices)
    # Dividiendo los datos del fichero de entrada de forma aleatoria entre el número de particiones especificado
    grupos = filas // self.numParticiones

    # Creamos las listas de train y test
    for i in range(0, self.numParticiones):

      # Creamos una particion nueva y ponemos que el test se vaya moviendo segun los grupos que se hayan creado anteriormente
      newParticion = Particion()
      newParticion.indicesTest = indicesAleatorios[i*grupos:i*grupos+grupos]
      
      # Al haber creado el grupo del test, metemos los indices con los que vamos a entrenar en la lista de entrenar
      for j in range(0, len(indicesAleatorios)):
        if j not in newParticion.indicesTest:
          newParticion.indicesTrain.append(indicesAleatorios[j])
      
      self.particiones.append(newParticion)
      
    return self.particiones

