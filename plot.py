# create an animation of 2D N-body simulation. Each frame/snapshot is a csv file containing the system state
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd

def animate(i):
    # Read the data from the CSV file
    data = pd.read_csv(f"out/frame_{i}.csv")
    tree = pd.read_csv(f"out/tree_{i}.csv")
    
    # Clear the current plot
    ax.cla()
    
    # Plot the positions of the bodies
    ax.scatter(data['pos_x'], data['pos_y'], s=10)

    # draw boxes around the quadtree nodes
    for index, row in tree.iterrows():
        x = row['pos_x']
        y = row['pos_y']
        size_x = row['size_x']
        size_y = row['size_y']
        # Create a rectangle representing the quadtree node
        rect = plt.Rectangle((x, y), size_x, size_y, linewidth=1, edgecolor='r', facecolor='none')
        ax.add_patch(rect)
    
    # Set the limits and labels
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 100)
    ax.set_xlabel('X Position')
    ax.set_ylabel('Y Position')
    ax.set_title(f'Time Step: {i}')
    # Optionally, you can add a grid
    ax.grid(True)

# Create a figure and axis
fig, ax = plt.subplots(figsize=(8, 8))
# Set the limits and labels
ax.set_xlim(0, 100)
ax.set_ylim(0, 100)
# Optionally, you can add a grid
ax.grid(True)
# Create an animation
ani = animation.FuncAnimation(fig, animate, frames=100, interval=100)
# Save the animation as a GIF
ani.save('n_body_simulation.gif', writer='pillow', fps=10)