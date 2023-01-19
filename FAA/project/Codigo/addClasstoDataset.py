#!/usr/bin/env python3

"""
@file     python.py
@date     DD/MM/YYYY
@version  0.0.1
@license  GNU General Public License v2.0
@url      github.com/Julynx/RepositoryName
@author   Julio Cabria (username@domain.ext)
"""

import random
import os
import sys
import numpy as np

METADATA_FILE = "HT_Sensor_metadata.csv"
DATA_FILE = "HT_Sensor_dataset.csv"
header = "id,time,R1,R2,R3,R4,R5,R6,R7,R8,Temp.,Humidity,Class"

def main() -> int:

    getClassFromId = {}
    with open(METADATA_FILE, "r") as f:
        next(f) # skip header
        for line in f:
            words = line.split(",")
            Id = words[0]
            Class = words[2]
            getClassFromId[Id] = Class

    with_class = []
    with open(DATA_FILE, "r") as f:
        next(f) # skip header
        for line in f:
            words = line.split(",")
            Id = words[0]
            Class = getClassFromId[Id]
            with_class.append(f"{line.strip()},{Class}")

    with open("dataset.csv", "w") as f:
        f.write(header + "\n")
        for line in with_class:
            f.write(line + "\n")

    return 0


if __name__ == "__main__":
    main()
