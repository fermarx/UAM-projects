import random
from Clasificador import *
import math

class ClasificadorRL(Clasificador):
  
  def __init__(self, epoca, cteaprend) -> None:    
    self.w = None
    self.epoca = epoca
    self.cteaprend = cteaprend
    super().__init__()
  

  def entrenamiento(self,datosTrain,nominalAtributos,diccionario):
    """
    Se aplica el algoritmo de entrenamiento por descenso de gradiente iterativo
    w: vector de pesos; 
    tn: 1 o 0 (valor de clase)
    x: vector de entrenamiento
    """
    # Se parte de un vector ŵ generado aleatoriamente y de una constante de aprendizaje η
    self.w = np.array([random.uniform(-0.5, 0.5) for _ in range(len(nominalAtributos)-1)])

    # Para cada ejemplo ŷ del conjunto de entrenamiento:
    for i in range(self.epoca):
      for dato in datosTrain:
        # Calcular la probabilidad a posteriori de pertenencia a la primera clase:
        x = dato[:-1]
        tn = dato[-1]
        sigmoid = self.sigmoidal(np.dot(self.w, x)) # yn = σ(wT xn)

        # Actualizar el valor de la proyección:
        self.w = self.w - (self.cteaprend*(sigmoid-tn))*x # w = w – η(yn - tn) xn
  

  def clasifica(self,datosTest,nominalAtributos,diccionario):
    """
    Con el vector w ajustado en entrenamiento, para cada ejemplo de test se calcula la probabilidad de
    pertenencia a cada clase a partir de yn = σ(wT xn)
    w: vector de pesos;
    x: vector de test
    """
    aux = []
    for dato in datosTest:
      x = dato[:-1]
      prob = self.sigmoidal(np.dot(self.w, x))
      if prob > 0.5: aux.append(1)
      else: aux.append(0)
        
    return aux


  def sigmoidal(self, x):
    """
    La función exponencial, math.exp, puede presentar un problema de overflow cerca del valor 700. 
    Para evitar el error capturar la excepción y devolver cero.
    """
    if x > 700: return 1
    elif x < -700: return 0
    return 1 / (1 + math.exp(-x))


  def confianza(self, datos_test):
    aux = []
    for dato in datos_test:
      prob = self.sigmoidal(np.dot(self.w, dato[:-1]))
      vp, fp = False, False
      if dato[-1] == 1 and prob > 0.5: vp = True
      elif dato[-1] == 0 and prob > 0.5: fp = True
      aux.append((prob, vp, fp))
      
    return aux
  
