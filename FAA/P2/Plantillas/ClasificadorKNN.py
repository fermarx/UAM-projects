from Clasificador import *

class ClasificadorKNN(Clasificador):
    
  def __init__(self, normaliza, k) -> None:
    # Variable que guarda si hay que normalizar los datos o no
    self.normaliza = normaliza
    # Lista con los datos normalizados del train
    self.normalizado_train = []
    # Diccionario con las medias y las std
    self.diccMeanStd = {}
    # valor k de vecinos
    self.k = k
    super().__init__()

  # TODO: esta funcion debe ser implementada en cada clasificador concreto. Crea el modelo a partir de los datos de entrenamiento
  # datosTrain: matriz numpy con los datos de entrenamiento
  # nominalAtributos: array bool con la indicatriz de los atributos nominales
  # diccionario: array de diccionarios de la estructura Datos utilizados para la codificacion de variables discretas
  def entrenamiento(self, datosTrain, nominalAtributos, diccionario):
    """ Normalización o el cálculo de las
        medias/desviaciones para la normalización"""
    self.normalizado_train = []
    self.diccMeanStd = {}

    self.calcularMediasDesv(datosTrain,diccionario)

    if (self.normaliza is True):  self.normalizado_train = self.normalizarDatos(datosTrain,diccionario, nominalAtributos)
    else:  self.normalizado_train = datosTrain.astype(float)
    

  # TODO: esta funcion debe ser implementada en cada clasificador concreto. Devuelve un numpy array con las predicciones
  # datosTest: matriz numpy con los datos de validaci�n
  # nominalAtributos: array bool con la indicatriz de los atributos nominales
  # diccionario: array de diccionarios de la estructura Datos utilizados para la codificacion de variables discretas
  def clasifica(self,datosTest, nominalAtributos, diccionario):
    """calcula el círculo o la bola de los K vecinos y se
      estima como clase prevista la clase mayoritaria en esa “vecindad”"""
    valores = []
    normalizado_test = []
    
    if (self.normaliza is True): normalizado_test = self.normalizarDatos(datosTest.astype(float), diccionario, nominalAtributos)
    else: normalizado_test = datosTest.astype(float)

    for test in normalizado_test[:,:-1].astype(float):
      distancias = []

      # Calculamos las distancias a cada uno de los datos que teniamos
      for train in self.normalizado_train[:,:-1].astype(float):
        sum = 0.0
        for i in range(len(train)):
          sum += LA.linalg.norm(test[i] - train[i])
        distancias.append(math.sqrt(sum))

      k_datos = self.normalizado_train[np.argsort(distancias)[:self.k]]
      valores.append(mode(k_datos[:,-1]))

    return valores


  # calculará las medias y desviaciones típicas de uno o varios atributos continuos 
  # según lo que se pase en el argumento datos
  def calcularMediasDesv(self,datos,diccionario):
    for i, valor in enumerate(diccionario):
      if(valor != "Class"):
        self.diccMeanStd[valor] = {}
        # Calculamos la media
        self.diccMeanStd[valor]["Media"] = np.mean(datos[:,i].astype(float))
        # Calculamos la std
        self.diccMeanStd[valor]["Std"] = np.std(datos[:,i].astype(float))


  # normalizará cada uno de los atributos continuos en la matriz datos utilizando 
  # las medias y desviaciones típicas obtenidas en calcularMediasDesv
  def normalizarDatos(self, datos, diccionario, nominalAtributos): 
    i = 0
    total = np.zeros(datos.shape, dtype=float)
    for valor in diccionario:
      if(valor != "Class" and nominalAtributos[i] is False):
        if self.diccMeanStd[valor]["Std"] == 0: 
          total[:,i] = (datos[:,i].astype(float) - self.diccMeanStd[valor]["Media"]) / 0.05
        else:
          total[:,i] = (datos[:,i].astype(float) - self.diccMeanStd[valor]["Media"]) / self.diccMeanStd[valor]["Std"]
      else: 
        total[:,i] = datos[:, i]
      i += 1
      
    return total.astype(str)