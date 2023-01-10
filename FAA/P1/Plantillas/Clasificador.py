from abc import ABCMeta,abstractmethod
from ssl import ALERT_DESCRIPTION_UNSUPPORTED_EXTENSION
from Datos import *
import scipy.stats as norm


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
      if float(datos[:,-1][i]) != pred[i]:
        cont +=1
    return cont/len(pred)
    
  # Realiza una clasificacion utilizando una estrategia de particionado determinada
  def validacion(self,particionado,dataset,clasificador,laplace,seed=None):
    lista_errores = []
    particiones = particionado.creaParticiones(dataset)
    
    for i in range(len(particiones)):
      datos_train = dataset.extraeDatos(particiones[i].indicesTrain)
      datos_test = dataset.extraeDatos(particiones[i].indicesTest)

      clasificador.entrenamiento(datos_train, dataset.nominalAtributos, dataset.diccionario, laplace)
      # El método clasifica devuelve una lista con las clases estimadas (una por cada instancia del dataset de test).
      clasificacion = clasificador.clasifica(datos_test,dataset.nominalAtributos, dataset.diccionario)

      lista_errores.append(self.error(datos_test, clasificacion))      

    return lista_errores

class ClasificadorNaiveBayes(Clasificador):
    
    def __init__(self) -> None:
      self.lista_posteriori = []  
      self.priori = []
      super().__init__()

    # Obtiene las diferentes probabilidades (a priori, a posteriori) en función del conjunto de datos de train 
    # Atributos: datosTrain: matriz numpy con los datos de entrenamiento
    #            nominalAtributos: array bool con la indicatriz de los atributos nominales
    #            diccionario: array de diccionarios de la estructura Datos utilizados para la codificacion de variables discretas
    def entrenamiento(self,datosTrain,nominalAtributos,diccionario,laplace):      
      if (datosTrain is None) or (nominalAtributos is None) or (diccionario is None): return 
      
      # Hacemos una lista np.empty 
      self.posteriori = np.empty(len(datosTrain), dtype=object)
      num_clases = len(diccionario["Class"])
      col_clases = datosTrain[:,-1]
      
      # Calculamos las probabilidades a priori
      (unique, counts) = np.unique(col_clases, return_counts=True)
      self.priori = np.asarray((unique, (counts)/sum(counts))).T

      for i, valor in enumerate(diccionario):
        num_valores=len(diccionario[valor])

        if nominalAtributos[i] is False and valor!="Class":
          # NB supone que el atributo sigue una distribución normal
          matriz = np.zeros([2,2], dtype=float)
          falso = []
          verdadero = []
          for j, valor_atributo in enumerate(datosTrain[:,i]):
            valor_atributo_int = int(valor_atributo)
            if (datosTrain[valor_atributo_int, -1]==0): falso.append(int(datosTrain[valor_atributo_int, i]))
            else: verdadero.append(int(datosTrain[valor_atributo_int, i]))

          # Calculamos la media (µ) y la desviación típica (σ) 
          matriz[0][0] = np.mean(falso)
          matriz[0][1] = np.mean(verdadero)
          matriz[1][0] = np.std(falso)
          matriz[1][1] = np.std(verdadero)
          self.lista_posteriori.append(matriz)

        elif nominalAtributos[i] is True and valor!="Class":
          matriz = np.zeros([num_valores,num_clases], dtype=float)
          # Contar las frecuencias de aparición en el dataset de train.  
          # La probabilidad resulta de la relación entre casos favorables y casos totales por cada valor de clase.
          for j, valor_atributo in enumerate(datosTrain[:,i]):
            valor_atributo_int=int(valor_atributo)
            clase=int(datosTrain[j,-1])
            matriz[valor_atributo_int, clase] +=1

          # División entre número verdaderos y falsos sin correción de Laplace
          if(unique[0]=='0' and laplace==0):
            matriz[:,0] /= counts[0]
            matriz[:,1] /= counts[1]
          elif (unique[0]=='1' and laplace==0):
            matriz[:,0] /= counts[1]
            matriz[:,1] /= counts[0]
          # División entre número verdaderos y falsos con correción de Laplace
          elif (unique[0]=='0' and laplace==1):
            matriz +=1 
            matriz[:,0] /= counts[0]+num_valores
            matriz[:,1] /= counts[1]+num_valores
          else:
            matriz +=1
            matriz[:,0] /= counts[1]+num_valores
            matriz[:,1] /= counts[0]+num_valores
          self.lista_posteriori.append(matriz)
      pass


    # Calcula la clase más plausible, es decir, el valor de clase con máxima probabilidad a posteriori
    # Atributos: datosTest: matriz numpy con los datos de validaci�n
    #            nominalAtributos: array bool con la indicatriz de los atributos nominales
    #            diccionario: array de diccionarios de la estructura Datos utilizados para la codificacion de variables discretas
    # Return: Devuelve para cada instancia de test, la predicción de la clase, es decir, la clase estimada a la que pertenecerá 
    #         esa instancia, teniendo en cuenta los valores de sus atributos
    def clasifica(self,datosTest,nominalAtributos,diccionario):
      
      if (datosTest is None) or (nominalAtributos is None) or (diccionario is None): return None
      np.seterr(divide='ignore',invalid='ignore')

      valores = []
      for j in range(len(datosTest)):
        prob_falso = 1
        prob_verdadero = 1
        for i, valor in enumerate(diccionario):
          if nominalAtributos[i] is True and valor!="Class":
            matriz = self.lista_posteriori[i]
            prob_falso *= matriz[int(datosTest[j,i]),0]
            prob_verdadero *= matriz[int(datosTest[j,i]),1]
          
          # Para aplicar P(Ai|c) en atributos continuos, usamos la expresión de la distribución normal, sustituyendo el valor del
          # atributo en la instancia de test (x) y la media (µ) y la desviación típica (σ) calculadas durante el entrenamiento.
          elif nominalAtributos[i] is False and valor!="Class":
            matriz = self.lista_posteriori[i]
            prob_falso *= norm.norm.pdf(int(datosTest[j,i]), matriz[0,0], matriz[1,0])
            prob_verdadero *= norm.norm.pdf(int(datosTest[j,i]), matriz[0,1], matriz[1,1])

        prob_falso *= float(self.priori[0][1])
        prob_verdadero *= float(self.priori[1][1])
        if prob_verdadero > prob_falso:
          valores.append(1)
        else:
          valores.append(0)
      
      return valores 

      