#!/usr/bin/env python3

import random
import os
import sys
from utils import *

def main() -> int:
    """
    Main function
    """
    # Read filename from command line
    try:
        filename = sys.argv[1]

    except IndexError:
        print("No file specified")
        return 1

    N_LINES = sum(1 for _ in open(filename))

    # Open file and read lines
    try:
        fin = open(filename, "r")
        fout = open(f"{filename[:-4]}.csv", "w")

        for idx, line in enumerate(fin):

            if idx % 1000 == 0:
                print_progress_bar(idx, N_LINES)

            words = line.split()
            buffer = []

            for word in words:
                buffer.append(word.strip())
                buffer.append(",")

            buffer.pop()
            fout.write("".join(buffer) + "\n")

        fin.close()
        fout.close()

    except FileNotFoundError:
        print("File not found")
        return 1


if __name__ == "__main__":
    main()
