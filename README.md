

# Asteroid Ellipse Rendering

This project will draw the ellipses of orbits of asteroids using Keplarian elements from the Nasa Small Body Database  

Download data from [Nasa Small Body Database Query](https://ssd.jpl.nasa.gov/tools/sbdb_query.html), retrieve the following columns:  
spkid, full_name, neo, pha, e, a, q, i, om, w, ma, ad, n, per

- This website only allows you to download small batches of rows at a time so I split my downloads into batches filtered by inclination range. combineData.py will combine multiple csv files with the columns above into a single file.


# Converting from 3D Keplerian elements to (RadiusX, RadiusY, CentreX, CentreY, Rotation):

| Term | Definition |
| ---- | -----------------------|
| a | Semimajor axis length |
| p | Perihelion distance |
| i | Inclination |
| e | Eccentricity |
| peri | Argument of Perihelion |
| x | Centre Body X |
| y | Centre Body Y |
| Centre X | Centre of ellipse X |
| Centre Y | Centre of ellipse Y |  

Project perihelion and Apohelion onto plane:  
$\text{RadiusY} = p\cos(i) + (a - p)\cos(i)$  
$\text{RadiusX} = (1 - e^2) * a^2$  
$\text{Rotation} = \text{peri}$  
$\text{CentreY} = y - p\sin(\text{peri})$  
$\text{CentreX} = x - p\sin(\text{peri})$
