import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd

# load data 
data = pd.read_csv("output/data.csv")
tree = pd.read_csv("output/tree.csv")

def animate(i):
    frame_data = data[data['frame'] == i]
    frame_tree = tree[tree['frame'] == i]
    
    ax.clear()
    ax.scatter(frame_data['pos_x'], frame_data['pos_y'], s=frame_data['radius']*10, c='blue')

    # # Draw boxes around the quadtree nodes
    # for index, row in frame_tree.iterrows():
    #     x = row['pos_x']
    #     y = row['pos_y']
    #     size_x = row['size_x']
    #     size_y = row['size_y']
    #     # Create a rectangle representing the quadtree node
    #     rect = plt.Rectangle((x, y), size_x, size_y, linewidth=1, edgecolor='r', facecolor='none')
    #     ax.add_patch(rect)
    
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 100)
    ax.set_xlabel('X Position')
    ax.set_ylabel('Y Position')
    ax.set_title(f'Frame: {i}')

fig, ax = plt.subplots(figsize=(8, 8))
ax.set_xlim(0, 100)
ax.set_ylim(0, 100)

# Create GIF
num_frames = data['frame'].nunique()
ani = animation.FuncAnimation(fig, animate, frames=num_frames, interval=100)
ani.save('results/simulation.gif', writer='pillow', fps=10)