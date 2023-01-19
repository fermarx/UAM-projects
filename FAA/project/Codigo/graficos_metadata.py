import matplotlib.pyplot as plt
import pandas as pd

# Cargar el dataset
data = pd.read_csv("HT_Sensor_metadata.csv")


# Crear un gráfico de barras para mostrar la distribución de las clases
data["class"].value_counts().plot(kind='bar')
plt.xlabel("Clase")
plt.ylabel("Frecuencia")
plt.title("Distribución de las clases")
plt.show()

# Crear un gráfico de dispersión para comparar t0 y dt
colors = {'banana':'yellow', 'wine':'red', 'background':'blue'}
plt.scatter(data.t0, data.dt, c=[colors[i] for i in data["class"]])
plt.xlabel("Tiempo de inicio del estímulo (t0)")
plt.ylabel("Duración del estímulo (dt)")
plt.title("Comparación de t0 y dt")
plt.show()

# Crear un gráfico de caja para mostrar la distribución de t0 para cada clase
data.boxplot(column='t0', by='class')
plt.xlabel("Clase")
plt.ylabel("Tiempo de inicio del estímulo (t0)")
plt.title("Distribución de t0 por clase")
plt.show()
