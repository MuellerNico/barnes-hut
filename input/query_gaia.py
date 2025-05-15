from astroquery.gaia import Gaia
from astropy.coordinates import SkyCoord
import astropy.units as u

# Define the search parameters
coord = SkyCoord(ra=266.4*u.degree, dec=-29.0*u.degree, frame='icrs')
radius = 1.0 * u.degree

# Construct and execute the query
query = f"""
SELECT TOP 10000
  source_id, ra, dec, parallax, pmra, pmdec, radial_velocity
FROM gaiadr3.gaia_source
WHERE CONTAINS(
  POINT('ICRS', ra, dec),
  CIRCLE('ICRS', {coord.ra.degree}, {coord.dec.degree}, {radius.to(u.degree).value})
)=1
AND parallax IS NOT NULL
AND pmra IS NOT NULL
AND pmdec IS NOT NULL
AND radial_velocity IS NOT NULL
"""
job = Gaia.launch_job_async(query)
results = job.get_results()
# Print the first 5 rows of the results
print(results[:5])