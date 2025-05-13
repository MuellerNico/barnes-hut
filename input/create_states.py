import pandas as pd
import struct
import os

# Split the big CSV file into binary files containing system state at different dates
# Binary format:
#   - 4 bytes: number of particles (uint32)
#   - 8 * 8 bytes: x, y, z, vx, vy, vz, mass, radius (double)

csv_file = "solar_system.csv"
out_dir = "snapshots"
os.makedirs(out_dir, exist_ok=True)

df = pd.read_csv(csv_file)

# Group by time step
for jd, group in df.groupby("date (JD)"):
    filename = os.path.join(out_dir, f"{jd:.1f}.bin")
    
    with open(filename, "wb") as f:
        # Write number of particles (uint32)
        f.write(struct.pack("I", len(group)))
        
        for _, row in group.iterrows():
            values = (
                row["x (AU)"], row["y (AU)"], row["z (AU)"],
                row["vx (AU/yr)"], row["vy (AU/yr)"], row["vz (AU/yr)"],
                row["mass (kg)"], row["radius (m)"]
            )
            f.write(struct.pack("8d", *values))  # 8 doubles
