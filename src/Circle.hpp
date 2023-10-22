#include <SDL2/SDL.h>
#include <vector>

using namespace std;

class CirclePoints {
    public:
        CirclePoints(int xc, int yc, int r, SDL_Color color, bool filled);
        SDL_Point* midpointCircle(int xc, int yc, int r);
        void draw(SDL_Renderer* renderer);
        void pushPoint(vector<SDL_Point> &points, int x, int y);
    private:
        SDL_Point* points;
        int numOfPoints;
        SDL_Color color;
        bool filled;
};