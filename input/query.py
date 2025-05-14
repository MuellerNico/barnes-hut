from astroquery.jplhorizons import Horizons
import pandas as pd
from astropy.time import Time
from datetime import datetime, timedelta

# This script queries the JPL Horizons system for the positions and velocities of solar system bodies
# Data is saved in astronomical units (AU), years (yr) and solar masses (M_sun)

# Define target body IDs (JPL Horizons)
bodies = {
    'Sun': 10,
    'Mercury': 199,
    'Venus': 299,
    'Earth': 399,
    # 'Moon': 301,
    'Mars': 499,
    'Jupiter': 599,
    'Saturn': 699,
    'Uranus': 799,
    'Neptune': 899,
    'Pluto': 999
}

# date range (10 days)
start_year = 2000
end_year = 2025
dates = [datetime(year, 1, 1) for year in range(start_year, end_year + 1)]
dates = Time(dates).jd

# query each body
data = []
for name, body_id in bodies.items():
    obj = Horizons(id=body_id, location='@sun', epochs=dates)
    vec = obj.vectors()
    # print(f"Data for {name}:") # Debug
    # print(vec)
    for i in range(len(vec)):
        row = {
            'name': name,
            'date (JD)': dates[i],
            'x (AU)': vec['x'][i],
            'y (AU)': vec['y'][i],
            'z (AU)': vec['z'][i],
            'vx (AU/yr)': vec['vx'][i] * 365.25,
            'vy (AU/yr)': vec['vy'][i] * 365.25,
            'vz (AU/yr)': vec['vz'][i] * 365.25,
        }
        data.append(row)

# Mass in kg, Radius in meters
solar_system_data = {
    'Sun':    {'mass': 1.989e30, 'radius': 6.957e8},
    'Mercury':{'mass': 3.301e23, 'radius': 2.440e6},
    'Venus':  {'mass': 4.867e24, 'radius': 6.052e6},
    'Earth':  {'mass': 5.972e24, 'radius': 6.371e6},
    'Moon':   {'mass': 7.347e22, 'radius': 1.737e6},
    'Mars':   {'mass': 6.417e23, 'radius': 3.390e6},
    'Jupiter':{'mass': 1.898e27, 'radius': 6.991e7},
    'Saturn': {'mass': 5.683e26, 'radius': 5.823e7},
    'Uranus': {'mass': 8.681e25, 'radius': 2.536e7},
    'Neptune':{'mass': 1.024e26, 'radius': 2.462e7},
    'Pluto':  {'mass': 1.309e22, 'radius': 1.188e6},
}

M_SUN = 1.989e30  # mass of the Sun in kg
AU = 1.496e11  # astronomical unit in meters

# Add mass and radius to dataset
for row in data:
    name = row['name']
    if name in solar_system_data:
        row['mass (M_sun)'] = solar_system_data[name]['mass'] / M_SUN
        row['radius (AU)'] = solar_system_data[name]['radius'] / AU
    else:
        print("Warning: unknown body")
        row['mass (M_sun)'] = 0
        row['radius (AU)'] = 0

# Export to CSV
df = pd.DataFrame(data)
df.to_csv("solar_system.csv", index=False)
print(df)
