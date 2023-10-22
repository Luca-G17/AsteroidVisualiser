import numpy as np
import matplotlib.pyplot as plt

def draw_rotated_ellipse(Cx, Cy, a, b, theta):
    # Generate angles from 0 to 2π
    angles = np.linspace(0, 2*np.pi, 1000)

    # Compute x and y coordinates of points on the rotated ellipse
    x = Cx + b * np.cos(angles) * np.cos(theta) - a * np.sin(angles) * np.sin(theta)
    y = Cy + b * np.cos(angles) * np.sin(theta) + a * np.sin(angles) * np.cos(theta)

    # Round the coordinates to the nearest integer
    x = np.round(x).astype(int)
    y = np.round(y).astype(int)

    # Create a blank image canvas
    canvas = np.zeros((2*Cy+1, 2*Cx+1), dtype=np.uint8)

    # Set the points on the canvas corresponding to the ellipse coordinates
    canvas[y, x] = 255

    return canvas

# Example usage
Cx = 100  # X-coordinate of the ellipse center
Cy = 100  # Y-coordinate of the ellipse center
a = 80    # Semimajor axis length
b = 50    # Semiminor axis length
theta = np.pi / 4  # Rotation angle in radians

# Draw the rotated ellipse
canvas = draw_rotated_ellipse(Cx, Cy, a, b, theta)

# Display the result
plt.imshow(canvas, cmap='gray')
plt.axis('off')
plt.show()