#include <SDL2/SDL.h>
#include <vector>

using namespace std;

class EllipsePoints {
    public:
        EllipsePoints(int rx, int ry, int xc, int yc, float rotation, SDL_Color color, int thickness);
        SDL_Point* parametricEllipse(int rx, int ry, int xc, int yc, float rot, int thickness);
        void draw(SDL_Renderer* renderer);
        void pushPoint(vector<SDL_Point> &points, int x, int y);
        void translate(int dx, int dy);
    private:
        SDL_Point *points;
        int numOfPoints;
        SDL_Color color;
};