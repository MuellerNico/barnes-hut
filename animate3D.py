from itertools import combinations, product
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.animation as animation
import pandas as pd

data = pd.read_csv("output/frames.csv")
tree = pd.read_csv("output/tree.csv")

# find bounding box
x_min, x_max = data['pos_x'].min(), data['pos_x'].max()
y_min, y_max = data['pos_y'].min(), data['pos_y'].max()
z_min, z_max = data['pos_z'].min(), data['pos_z'].max()
xlim = (x_min, x_max)
ylim = (y_min, y_max)
zlim = (z_min, z_max)

def animate(i):
    frame_data = data[data['frame'] == i]
    frame_tree = tree[tree['frame'] == i]
    
    ax.clear()
    ax.scatter(frame_data['pos_x'], frame_data['pos_y'], frame_data['pos_z'], c='blue')

    #Draw boxes around the octree nodes using pos as origin and sizes
    for _, row in frame_tree.iterrows():
        x0, y0, z0 = row['pos_x'], row['pos_y'], row['pos_z']
        dx, dy, dz = row['size_x'], row['size_y'], row['size_z']
        # 8 corners of the box
        corners = np.array([
            [x0,     y0,     z0],
            [x0+dx,  y0,     z0],
            [x0,     y0+dy,  z0],
            [x0,     y0,     z0+dz],
            [x0+dx,  y0+dy,  z0],
            [x0+dx,  y0,     z0+dz],
            [x0,     y0+dy,  z0+dz],
            [x0+dx,  y0+dy,  z0+dz]
        ])
        # Define the 12 edges as pairs of corner indices
        edges = [
            (0,1), (0,2), (0,3),
            (1,4), (1,5),
            (2,4), (2,6),
            (3,5), (3,6),
            (4,7),
            (5,7),
            (6,7)
        ]
        for start, end in edges:
            ax.plot3D(*zip(corners[start], corners[end]), color="r")
    
    ax.set_xlim(xlim)
    ax.set_ylim(ylim)
    ax.set_zlim(zlim)
    ax.set_xlabel('X Position')
    ax.set_ylabel('Y Position')
    ax.set_zlabel('Z Position')
    ax.set_title(f'Frame: {i}')
    ax.view_init(elev=15, azim=i)  # Rotate the POV by changing the azimuth angle

fig = plt.figure(figsize=(8, 8))
ax = fig.add_subplot(111, projection='3d')
ax.set_xlim(xlim)
ax.set_ylim(ylim)
ax.set_zlim(zlim)

# Create GIF
num_frames = data['frame'].nunique()
ani = animation.FuncAnimation(fig, animate, frames=num_frames, interval=100)
ani.save('results/simulation_3D.gif', writer='pillow', fps=10)