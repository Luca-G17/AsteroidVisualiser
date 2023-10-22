#include <SDL2/SDL.h>
#include <vector>
#include "Circle.hpp"

using namespace std;

CirclePoints::CirclePoints(int xc, int yc, int r, SDL_Color color, bool filled) {
    this->color = color;
    this->points = midpointCircle(xc, yc, r);
    this->filled = filled;
}

void CirclePoints::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    if (!filled) {
        SDL_RenderDrawPoints(renderer, points, numOfPoints);
    } else {
        SDL_RenderDrawPoint(renderer, points[0].x, points[1].y);        
        for (unsigned int i = 0; i < numOfPoints; i += 2) {
            SDL_RenderDrawLine(renderer, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
        }
    }
}

void CirclePoints::pushPoint(vector<SDL_Point> &points, int x, int y) {
    SDL_Point p = {x, y};
    points.push_back(p);
}

SDL_Point* CirclePoints::midpointCircle(int xc, int yc, int r) {
    int x = r, y = 0;
    vector<SDL_Point> points;
    SDL_Point p1 = {(int) (x + xc), (int) (y + yc)}; // Right 
    points.push_back(p1);

    if (r > 0) {
        pushPoint(points, -x + xc, y + yc); // left
        pushPoint(points, y + xc, x + yc); // Top
        pushPoint(points, -y + xc, x + yc); // Top
    }
    int P = 1 - r;
    while (x > y) {
        y++;
        if (P <= 0) {
            P = P + 2 * y + 1;
        } else {
            x--;
            P = P + 2 * y - 2 * x + 1;
        }

        if (x < y) {
            break;
        }
        pushPoint(points, x + xc, y + yc);
        pushPoint(points, -x + xc, y + yc);
        pushPoint(points, x + xc, -y + yc);
        pushPoint(points, -x + xc, -y + yc);

        if (x != y) {
            pushPoint(points, y + xc, x + yc);
            pushPoint(points, -y + xc, x + yc);
            pushPoint(points, y + xc, -x + yc);
            pushPoint(points, -y + xc, -x + yc);
        }
    }
    SDL_Point* ps = new SDL_Point[points.size()];
    std::copy(points.begin(), points.end(), ps);
    this->numOfPoints = points.size();
    return ps;
}