#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <cmath>
#include <sstream>
#include <fstream>
#include "Orbit.hpp"
#include "Circle.hpp"
#include "Ellipse.hpp"

#define SCREEN_HEIGHT 900
#define SCREEN_WIDTH 1700
#define FONT_SIZE 16

void translateWindow(SDL_Renderer* renderer, int dx, int dy) {
    SDL_Rect rect;
    rect.x = dx;
    rect.y = dy;
    rect.w = SCREEN_WIDTH;
    rect.h = SCREEN_HEIGHT;
    std::cout << "Translating Window\n";
    SDL_RenderSetViewport(renderer, &rect);
}

void render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
}

void renderText(TTF_Font* font, SDL_Renderer* renderer, std::string text, int x, int y) {
    SDL_Color color = {255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}

float AUtoPixels(float au) {
    return au * 150.0f;
}

vector<Orbit> readOrbitsCSV(string filename, float cbx, float cby, int n) {
    vector<Orbit> orbits;
    fstream file(filename);
    int x = 0;
    if (file.is_open()) {
        string line;
        getline(file, line); // Skip first line
        while (getline(file, line)) {
            x++;
            if (x > n) {
                break;
            }
            stringstream ss(line);
            string token;
            vector<string> tokens;
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            if (stof(tokens[4]) < 0.7f && stof(tokens[7]) < 0.5f) { // Filter orbits with eccentricity > 0.7 and inclination < 0.5
                Orbit o = { cbx, cby, AUtoPixels(stof(tokens[5])), AUtoPixels(stof(tokens[6])), stof(tokens[7]), stof(tokens[4]), stof(tokens[13]) };
                orbits.push_back(o);
            } else {
                x--;
            }
        }
    }
    return orbits;
}

vector<Orbit> generateRandomOrbits(float centreX, float centreY, int n) {
    vector<Orbit> orbits;
    for (int f = 0; f < n; f++) {
        float a = rand() % 500 + 50;
        float p = rand() % (int) (a / 2) + 20;
        float i = 0;
        float e = (rand() % 100 + 1) / 100;
        float peri = (rand() % 360) * (M_PI / 180);
        Orbit o = { centreX, centreY, a, p, i, e, peri };
        orbits.push_back(o);
    }
    return orbits;
}

EllipsePoints getEllipsePoints(Ellipse e, SDL_Color color, int thickness) {
    EllipsePoints ep = EllipsePoints(e.rx, e.ry, e.cx, e.cy, e.rotation, color, thickness);
    return ep;
}

vector<EllipsePoints> getEllipsePoints(vector<Ellipse> ellipses, int thickness) {
    vector<EllipsePoints> ellipsePoints;
    for (Ellipse e : ellipses) {
        EllipsePoints ep = EllipsePoints(e.rx, e.ry, e.cx, e.cy, e.rotation, {255, 0, 0, 255}, thickness);
        ellipsePoints.push_back(ep);
    }
    return ellipsePoints;
}

void drawAll(vector<EllipsePoints> ellipses, CirclePoints centreBody, SDL_Renderer* renderer) {
    // Render all orbits
    for (EllipsePoints &e : ellipses) {
        e.draw(renderer);
    }
    centreBody.draw(renderer);
}

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL could not initialize\n");
        return 1;
    }
    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF could not initialize\n");
        return 1;
    }
    TTF_Font* font = TTF_OpenFont("assets/Roboto-Light.ttf", FONT_SIZE);
    SDL_Window* window = SDL_CreateWindow("Main Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Error creating window\n");
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Error creating renderer\n");
        return 1;
    }

    float centreBodyX = SCREEN_WIDTH / 2.0f;
    float centreBodyY = SCREEN_HEIGHT / 2.0f;
    SDL_Color yellow = {255, 255, 0, 255};
    // ellipses = convertOrbitsToEllipses(generateRandomOrbits(centreBodyX, centreBodyY, 3));
    vector<Orbit> orbits = readOrbitsCSV("data/asteroids.csv", centreBodyX, centreBodyY, 1000000);
    vector<EllipsePoints> ellipses = getEllipsePoints(convertOrbitsToEllipses(orbits), 1);

    // Add earth and mars
    EllipsePoints earth = getEllipsePoints(convertOrbitToEllipse({centreBodyX, centreBodyY, AUtoPixels(1.0f), AUtoPixels(1.0f), 0, 0, 0}), {0, 73, 191, 255}, 3);
    EllipsePoints mars = getEllipsePoints(convertOrbitToEllipse({centreBodyX, centreBodyY, AUtoPixels(1.5f), AUtoPixels(1.3814f), 0, 0.0934f, 1.5f}), { 219, 91, 0, 255 }, 3);
    EllipsePoints jupiter = getEllipsePoints(convertOrbitToEllipse({centreBodyX, centreBodyY, AUtoPixels(5.2f), AUtoPixels(5.035), 0, 0.0487f, 4.77f}), { 196, 167, 0, 255 }, 5);
    ellipses.push_back(earth);
    ellipses.push_back(mars);
    ellipses.push_back(jupiter);
    CirclePoints centreBody = CirclePoints(centreBodyX, centreBodyY, 15, yellow, true);

    bool redraw = true;
    bool quit = false;
    while (!quit) {
        Uint64 startPerf = SDL_GetPerformanceCounter();
        Uint64 startTicks = SDL_GetTicks();

        SDL_RenderClear(renderer);
        // Events loop

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                            translateWindow(renderer, 0, -10);
                        case SDL_SCANCODE_A:
                            translateWindow(renderer, -10, 0);
                        case SDL_SCANCODE_S:
                            translateWindow(renderer, 0, 10);
                        case SDL_SCANCODE_D:
                            translateWindow(renderer, 10, 0);
                        default:
                            break;
                    }
            }
        }

        if (redraw) {
            drawAll(ellipses, centreBody, renderer);
        }

        // Delay to maintain 60 FPS
        Uint64 endPerf = SDL_GetPerformanceCounter();
        float elapsed = (endPerf - startPerf) / (float)SDL_GetPerformanceFrequency();
        SDL_Delay(floor(16.666f - (elapsed / 1000.0f)));
        
        // Display FPS
        Uint64 endTicks = SDL_GetTicks();
        float frameTime = (endTicks - startTicks) / 1000.0f;
        std::string fps = "Current FPS: " + std::to_string(1.0f / frameTime);
        renderText(font, renderer, fps, SCREEN_WIDTH - 250, 10);
        if (redraw) {
            render(renderer);
            redraw = false;
        }
    }
    SDL_DestroyWindow(window);
}
