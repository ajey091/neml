#!/usr/bin/env python3

import sys
sys.path.append('../..')

import numpy as np

from neml.cp import crystallography
from neml.math import rotations

import matplotlib.pyplot as plt

if __name__ == "__main__":
  N = 300

  orientations = rotations.random_orientations(N)

  sgroup = crystallography.SymmetryGroup("432")

  angles = []

  for i in range(len(orientations)):
    for j in range(i+1, len(orientations)):
      o1 = orientations[i]
      o2 = orientations[j]
      m = sgroup.misorientation(o1,o2)
      axis, angle = m.to_axis_angle()

      angles.append(angle)

  angles = np.rad2deg(angles)
  
  plt.figure()
  plt.hist(angles, bins = 30)
  plt.show()

  
  Np = N * (N-1)
  orientations1 = rotations.random_orientations(Np)
  orientations2 = rotations.random_orientations(Np)

  mis = sgroup.misorientation_block(orientations1, orientations2)
  angles = [np.rad2deg(m.to_axis_angle()[1]) for m in mis]
  plt.figure()
  plt.hist(angles, bins = 30)
  plt.show()
