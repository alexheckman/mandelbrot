#!/usr/bin/env python
import sys
import numpy as np

from matplotlib import pyplot as plt

if __name__ == "__main__":
  if len(sys.argv) < 1:
    raise Exception("Must provide a file path as first argument")
  
  d = np.genfromtxt(sys.argv[1], dtype=str, delimiter=",")
  print d.shape
  dt = d.transpose()
  matrix = []
  for row in d:
    matrix.append([tuple(map(int,item.split(":"))) for item in row])
    
  npm = np.array(matrix)
  plt.imshow(npm)
  plt.show()
