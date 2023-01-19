#!/usr/bin/env python3

from pprint import pprint
import sys
import numpy as np
from utils import *

"""
Del correo:

En los datos, todas las series empiezan aproximadamente en -1h. Es decir,
cuando llega al 0 es cuando ponen el estimulo. Luego el tiempo de
duración de los metadatos sí debería estar bien.
"""


# Importante: Ejecutar toCSV.py sobre los archivos .dat para generar los archivos .csv
#             antes de ejecutar este script.
METADATA_FILE = "HT_Sensor_metadata.csv"
DATA_FILE = "HT_Sensor_dataset.csv"
OUTPUT_FILE = "dataset_estimulos.csv"
N_LINES = 928992
LAST_VALID_ID = 68


def main() -> int:

    # Leer las duraciones de los estimulos del archivo de metadatos
    duraciones = dict()
    with open(METADATA_FILE, "r") as f:
        next(f)  # Saltar la primera linea

        for line in f:
            words = line.split(",")
            idf, duration = words[0].strip(), words[-1].strip()
            duraciones[idf] = float(duration)

    # DEBUG: Imprimir las duraciones
    # _ = [print(f"{k}: {v}") for k, v in sorted(
    #      duraciones.items(), key=lambda x: int(x[0]))]

    # Leer el archivo de datos y agregar el campo de clase
    out = open(OUTPUT_FILE, "w")
    with open(DATA_FILE, "r") as f:
        header = next(f)  # Saltar la primera linea
        out.write(f"{header.strip()},class\n")

        for idx, line in enumerate(f):
            if idx % 1000 == 0:
                print_progress_bar(idx, N_LINES)

            words = line.split(",")
            idf, time = words[0], float(words[1])

            # 0: no estimulo, 1: estimulo
            cl = 1 if 0 < time < duraciones[idf] else 0
            
            if int(idf) > LAST_VALID_ID:
                cl = 0

            # Escribir la linea con el nuevo campo
            out.write(f"{line.strip()},{cl}\n")

    out.close()


if __name__ == "__main__":
    sys.exit(main())
