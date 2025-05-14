import pandas as pd
import numpy as np
import struct
import os
import matplotlib.pyplot as plt

# load binary data from simulation output and binary jpl horizons data and compare
# Binary format:
#   - 4 bytes: number of particles (uint32)
#   - 8 * 8 bytes: x, y, z, vx, vy, vz, mass, radius (double)

horizons_folder = "input/jpl_horizons"
output_folder = "output/snapshots"

def load_binary_data(filename):
    with open(filename, "rb") as f:
        num_particles = struct.unpack("I", f.read(4))[0]
        data = np.fromfile(f, dtype=np.float64, count=num_particles * 8)
        data = data.reshape((num_particles, 8))
        
    return data

# array of position differences for every body at each time step
pos_diff = []
vel_diff = []

for filename in os.listdir(horizons_folder):
    if filename.endswith(".bin"):
        horizons_file = os.path.join(horizons_folder, filename)
        output_file = os.path.join(output_folder, filename)
        
        if not os.path.exists(output_file):
            print(f"Warning: {output_file} does not exist")
            continue
        
        horizons_data = load_binary_data(horizons_file)
        output_data = load_binary_data(output_file)
        
        # for each body compute difference in position and velocity
        pos = np.linalg.norm(horizons_data[:, :3] - output_data[:, :3], axis=1)
        vel = np.linalg.norm(horizons_data[:, 3:6] - output_data[:, 3:6], axis=1)
        pos_diff.append(pos)
        vel_diff.append(vel)

# convert to numpy arrays for easier manipulation
pos_diff = np.array(pos_diff)  # shape: (time_steps, num_bodies)
vel_diff = np.array(vel_diff)

# plot position difference over time for each body
plt.figure(figsize=(10, 6))
plt.title("Position Difference Over Time")
plt.xlabel("Time Step")
plt.ylabel("Position Difference (AU)")
for body_idx in range(pos_diff.shape[1]):
    plt.plot(pos_diff[:, body_idx], label=f"Body {body_idx}")
plt.legend()
plt.grid()
plt.savefig("results/position_difference.png")
