import numpy as np
import struct
import os
import matplotlib.pyplot as plt

horizons_folder = "input/jpl_horizons"
output_folder = "output/snapshots"
results_folder = "results"
os.makedirs(results_folder, exist_ok=True)

def load_binary_data(filename):
    with open(filename, "rb") as f:
        num_particles = struct.unpack("I", f.read(4))[0]
        data = np.fromfile(f, dtype=np.float64, count=num_particles * 8)
        data = data.reshape((num_particles, 8))
        # ignore sun (first body)
        # data = data[1:]
        # ignore the last body (pluto)
        # data = data[:-1]

    return data

def compute_energies(data):
    # data shape (num_bodies, 8) [x, y, z, vx, vy, vz, mass, radius]
    pos = data[:, :3]
    vel = data[:, 3:6]
    mass = data[:, 6]
    # kin energy: 0.5 * m * v^2
    v2 = np.sum(vel**2, axis=1)
    kinetic = 0.5 * mass * v2
    total_kinetic = np.sum(kinetic)
    # pot energy: -G * m_i m_j / r_ij
    G = 4 * np.pi**2 # in AU^3 / (M_sun * yr^2)
    potential = 0.0
    n = len(mass)
    for i in range(n):
        for j in range(i+1, n):
            r = np.linalg.norm(pos[i] - pos[j])
            if r > 0:
                potential -= G * mass[i] * mass[j] / r
    return total_kinetic, potential, total_kinetic + potential

horizons_energies = []
output_energies = []
pos_diff = []

# assumes filenames are sortable by time
files = sorted([f for f in os.listdir(horizons_folder) if f.endswith(".bin")])

for filename in files:
    horizons_file = os.path.join(horizons_folder, filename)
    output_file = os.path.join(output_folder, filename)
    if not os.path.exists(output_file):
        print(f"Warning: {output_file} does not exist")
        continue
    horizons_data = load_binary_data(horizons_file)
    output_data = load_binary_data(output_file)
    # Energies
    horizons_energies.append(compute_energies(horizons_data))
    output_energies.append(compute_energies(output_data))
    # Position error
    pos_err = np.linalg.norm(horizons_data[:, :3] - output_data[:, :3], axis=1)
    pos_diff.append(pos_err)

horizons_energies = np.array(horizons_energies)  # shape: (timesteps, 3)
output_energies = np.array(output_energies)
pos_diff = np.array(pos_diff)  # shape: (timesteps, num_bodies)

horizons_mean_energies = np.mean(horizons_energies, axis=0)
output_mean_energies = np.mean(output_energies, axis=0)

# Plot energies
plt.figure(figsize=(12, 7))
plt.xlabel("Time (yr)")
plt.ylabel(r"Energy $\frac{ \text{AU}^2 \cdot M_{\text{sun}} }{ \text{yr}^2 }$")
labels = ["Kinetic", "Potential", "Total"]
for i in range(3):
    plt.plot(horizons_energies[:, i], label=f"Horizons {labels[i]}", linestyle="-", linewidth=4, alpha=0.6)
    plt.plot(output_energies[:, i], label=f"Simulated {labels[i]}", linestyle=":", linewidth=4, alpha=0.8)
plt.legend()
plt.grid()
plt.tight_layout()
plt.savefig(os.path.join(results_folder, "energies_over_time.png"))

# Plot position error
plt.figure(figsize=(10, 6))
plt.xlabel("Time (yr)")
plt.ylabel("Position Difference (AU)")
for body_idx in range(pos_diff.shape[1]):
    plt.plot(pos_diff[:, body_idx])
plt.grid()
plt.tight_layout()
plt.savefig(os.path.join(results_folder, "position_difference.png"))
