import matplotlib.pyplot as plt
import pandas as pd

# Cargar el dataset
data = pd.read_csv("dataset_estimulos.csv")

meta = pd.read_csv("HT_Sensor_metadata.csv")

banana = meta[meta["class"] == "banana"]
banana_id = banana["id"]
wine = meta[meta["class"] == "wine"]
wine_id = wine["id"]

# Crear una figura y un eje
fig, ax = plt.subplots()
for i in wine_id:
    data = data[data["id"] == i]
    # comentar si queremos hacer graficas de antes del filtrado
    data = data[data["class"] == 1]

    # Graficar todos los sensores en el eje
    ax.plot(data["time"], data["R1"], label="R1")
    ax.plot(data["time"], data["R2"], label="R2")
    ax.plot(data["time"], data["R3"], label="R3")
    ax.plot(data["time"], data["R4"], label="R4")
    ax.plot(data["time"], data["R5"], label="R5")
    ax.plot(data["time"], data["R6"], label="R6")
    ax.plot(data["time"], data["R7"], label="R7")
    ax.plot(data["time"], data["R8"], label="R8")
    ax.plot(data["time"], data["Temp."], label="Temp.")
    ax.plot(data["time"], data["Humidity"], label="Humidity")

# Añadir etiquetas y títulos
ax.set_xlabel("Tiempo")
ax.set_ylabel("Valor medido")
ax.set_title("Comparación de sensores vs tiempo en vino")
ax.legend(frameon=False, fontsize=12, bbox_to_anchor=(-0.08,0.9), handletextpad=0)

# Mostrar el gráfico
plt.show()
