#include <SDL2/SDL.h>
#include <vector>
#include "Ellipse.hpp"

using namespace std;


void EllipsePoints::pushPoint(vector<SDL_Point> &points, int x, int y) {
    SDL_Point p = {x, y};
    points.push_back(p);
}

void EllipsePoints::translate(int dx, int dy) {
    for (int i = 0; i < numOfPoints; i++) {
        points[i].x += dx;
        points[i].y += dy;
    }
}

SDL_Point* EllipsePoints::parametricEllipse(int rx, int ry, int xc, int yc, float rot, int thickness) {
    vector<SDL_Point> v;
    rot = -rot;
    float step = 1.0f / max(rx, ry);
    // Derivatives
    // dx/d(theta) = -rx * sin(theta) * cos(rot) - ry * cos(theta) * sin(rot)
    // dy/d(theta) = -rx * sin(theta) * sin(rot) + ry * cos(theta) * cos(rot)
    for (float theta = 0; theta < 2 * M_PI; theta += step) {
        float dx = rx * cos(theta) * cos(rot) - ry * sin(theta) * sin(rot);
        float dy = rx * cos(theta) * sin(rot) + ry * sin(theta) * cos(rot);
        float x = xc + dx;
        float y = yc + dy;
        pushPoint(v, x, y);

        for (int i = 1; i < thickness; i++) {
            // Add parallel curve
            float xp = -rx * sin(theta) * cos(rot) - ry * cos(theta) * sin(rot);
            float yp = -rx * sin(theta) * sin(rot) + ry * cos(theta) * cos(rot);

            float denom = sqrt(xp * xp + yp * yp);
            float xd = dx + -i * yp / denom;
            float yd = dy - -i * xp / denom;
            pushPoint(v, xc + xd, yc + yd);
        }
    }

    SDL_Point* points = new SDL_Point[v.size()];
    std::copy(v.begin(), v.end(), points);
    this->numOfPoints = v.size();
    return points;
}

void EllipsePoints::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoints(renderer, points, this->numOfPoints);
}

EllipsePoints::EllipsePoints(int rx, int ry, int xc, int yc, float rotation, SDL_Color color, int thickness) {
    this->points = parametricEllipse(rx, ry, xc, yc, rotation, thickness);
    this->color = color;
}