

# Asteroid Ellipse Rendering
# 
# Download data from  
#
# Converting from 3D Keplerian elements to (RadiusX, RadiusY, CentreX, CentreY, Rotation):
# a = Semimajor axis
# p = Perihelion distance
# i = Inclination
# e = Eccentricity = sqrt(1 - (b^2 / a^2))
# peri = Argument of perihelion
# x = Centre Body x
# y = Centre Body y
# - Project perihelion and apohelion onto 2D plane: a' = pcos(i) + (a - p)cos(i) = RadiusY
# - RadiusX = (1 - e^2) * a^2
# - Rotation = peri
# - CentreY = y - psin(peri)
# - CentreX = x - psin(peri)