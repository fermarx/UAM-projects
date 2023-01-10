from Datos import Datos
from EstrategiaParticionado import *
from Clasificador import *

numParticiones = 5
numeroEjec = 2
porTest = 30

dataset = Datos('../ConjuntosDatosIntroFAA/german.csv')

print("Validación simple con Dataset tic-tac-toe")
estrategiaVS = ValidacionSimple(numeroEjec, porTest)
particionesVS = estrategiaVS.creaParticiones(dataset)
"""for i in range(numeroEjec):
    print(f"Indices utilizados para Test nº{i}: {particionesVS[i].indicesTest}")
    print(f"Indices utilizados para Train nº{i}: {particionesVS[i].indicesTrain}")"""


print("\nValidación cruzada con Dataset tic-tac-toe")
estrategiaVC = ValidacionCruzada(numParticiones)
particionesVC = estrategiaVC.creaParticiones(dataset)
"""for i in range(numParticiones):
    print(f"Indices utilizados para Test nº{i}: {particionesVC[i].indicesTest}")
    print(f"Indices utilizados para Train nº{i}: {particionesVC[i].indicesTrain}")"""

# clasificador = Clasificador.ClasificadorNaiveBayes()

# errores = Clasificador.validacion(estrategiaVC,dataset,clasificador)