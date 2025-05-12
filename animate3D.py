from itertools import combinations, product
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.animation as animation
import pandas as pd

# Load the data once
data = pd.read_csv("out/data.csv")
tree = pd.read_csv("out/tree.csv")

def animate(i):
    frame_data = data[data['frame'] == i]
    frame_tree = tree[tree['frame'] == i]
    
    ax.clear()
    ax.scatter(frame_data['pos_x'], frame_data['pos_y'], frame_data['pos_z'], 
               s=frame_data['radius']*10, c='blue')

    # # Draw boxes around the octree nodes
    # for index, row in frame_tree.iterrows():
    #     x = row['pos_x']
    #     y = row['pos_y']
    #     z = row['pos_z']
    #     size_x = row['size_x']
    #     size_y = row['size_y']
    #     size_z = row['size_z']
    #     # Create a wireframe cube representing the octree node
    #     r = [x, x + size_x]
    #     for s, e in combinations(np.array(list(product(r, r, r))), 2):
    #         if np.sum(np.abs(s - e)) == r[1] - r[0]:
    #             ax.plot3D(*zip(s, e), color="r")
    
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 100)
    ax.set_zlim(0, 100)
    ax.set_xlabel('X Position')
    ax.set_ylabel('Y Position')
    ax.set_zlabel('Z Position')
    ax.set_title(f'Frame: {i}')
    ax.view_init(elev=30, azim=i)  # Rotate the POV by changing the azimuth angle

# Create a figure and 3D axis
fig = plt.figure(figsize=(8, 8))
ax = fig.add_subplot(111, projection='3d')
# Set the limits and labels
ax.set_xlim(0, 100)
ax.set_ylim(0, 100)
ax.set_zlim(0, 100)
# Create GIF
num_frames = data['frame'].nunique()
ani = animation.FuncAnimation(fig, animate, frames=num_frames, interval=100)
ani.save('simulation_3D.gif', writer='pillow', fps=10)