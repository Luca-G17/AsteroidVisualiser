#include <cmath>
#include <vector>

/**
 * @brief Orbit struct
 * \param cbx Centre Body x
 * \param cby Centre Body y
 * \param a Semimajor axis
 * \param p Perihelion distance
 * \param i Inclination
 * \param e Eccentricity = sqrt(1 - (b^2 / a^2))
 * \param peri Argument of perihelion
 */
typedef struct Orbit {
    float cbx;
    float cby;
    float a;
    float p;
    float i;
    float e;
    float peri;
} Orbit;

/**
 * @brief Ellipse struct
 * \param cx Center x
 * \param cy Center y
 * \param rx Radius along x-axis
 * \param ry Radius along y-axis
 * \param rotation Rotation
 */
typedef struct Ellipse {
    float cx;
    float cy;
    float rx;
    float ry;
    float rotation;
} Ellipse;

Ellipse convertOrbitToEllipse(Orbit orbit) {
    Ellipse ellipse;
    ellipse.ry = (orbit.p * cos(orbit.i) + (orbit.a - orbit.p) * cos(orbit.i));
    float newP = orbit.p * cos(orbit.i);
    ellipse.rx = (sqrt((1 - (orbit.e * orbit.e)) * (orbit.a * orbit.a)));
    ellipse.cy = orbit.cby - (ellipse.ry - newP) * cos(orbit.peri);
    ellipse.cx = orbit.cbx - (ellipse.ry - newP) * sin(orbit.peri);
    ellipse.rotation = orbit.peri;
    return ellipse;
}

std::vector<Ellipse> convertOrbitsToEllipses(std::vector<Orbit> orbits) {
    std::vector<Ellipse> ellipses;
    for (Orbit &orbit : orbits) {
        Ellipse ellipse = convertOrbitToEllipse(orbit);
        ellipses.push_back(ellipse);
    }
    return ellipses;
}

