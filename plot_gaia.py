import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from mpl_toolkits.mplot3d import Axes3D  # Needed for 3D plotting

data = pd.read_csv("output/frames.csv")
frame_data = data[data['frame'] == 0]

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(frame_data['pos_x'], frame_data['pos_y'], frame_data['pos_z'], c='blue', s=0.05, alpha=0.2)
# ax.view_init(elev=30)
plt.savefig("results/gaia.png")
