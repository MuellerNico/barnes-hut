from astropy.coordinates import SkyCoord
from astropy import units as u
from astroquery.gaia import Gaia
import numpy as np
import struct

Gaia.ROW_LIMIT = 10000  # max number of stars

# Simpler, safer query
query = """
SELECT TOP 10000
    source_id, ra, dec, parallax, pmra, pmdec, radial_velocity, phot_g_mean_mag
FROM gaiadr3.gaia_source
WHERE parallax > 1 AND radial_velocity IS NOT NULL AND pmra IS NOT NULL AND pmdec IS NOT NULL
"""

job = Gaia.launch_job_async(query)
results = job.get_results()

# Constants
K = 4.74047  # km/s per mas/yr * kpc
AU_IN_KM = 1.496e+8
YEAR_IN_SEC = 365.25 * 24 * 3600
M_sun = 4.83

positions = []
velocities = []
masses = []
radii = []

def estimate_mass_from_mag(g_mag, plx):
    d_pc = 1000.0 / plx
    M = g_mag - 5 * (np.log10(d_pc) - 1)
    L = 10 ** (-0.4 * (M - M_sun))
    mass = L ** (1 / 3.5)  # in solar masses
    return mass

for row in results:
    if row["parallax"] <= 0:
        continue

    distance_pc = 1.0 / (row["parallax"] * 1e-3)
    coord = SkyCoord(ra=row["ra"] * u.deg, dec=row["dec"] * u.deg, distance=distance_pc * u.pc)
    x, y, z = coord.cartesian.xyz.to(u.au).value

    vx = K * row["pmra"] / row["parallax"]  # km/s
    vy = K * row["pmdec"] / row["parallax"]
    vz = row["radial_velocity"]

    vx, vy, vz = np.array([vx, vy, vz]) * YEAR_IN_SEC / AU_IN_KM  # km/s to AU/yr

    mass = estimate_mass_from_mag(row["phot_g_mean_mag"], row["parallax"])  # fallback
    radius = 6.957e5 / AU_IN_KM  # just some placeholder

    positions.append((x, y, z))
    velocities.append((vx, vy, vz))
    masses.append(mass)
    radii.append(radius)

# Write to binary
with open("gaia_stars.bin", "wb") as f:
    f.write(struct.pack("I", len(positions)))  # uint32 count
    for pos, vel, m, r in zip(positions, velocities, masses, radii):
        f.write(struct.pack("dddddddd", *pos, *vel, m, r))
