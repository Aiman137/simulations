#include <stdio.h>
#include <math.h>
#include <stdlib.h> // For dynamic memory allocation
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define POINT_RADIUS 5
#define POINT_SPEED 200.0
#define NUM_POINTS 100 // Number of points

typedef struct {
    double x;
    double y;
} Vector2D;

typedef struct {
    Vector2D position;
    Vector2D velocity;
} Point;

void updatePosition(Point *point, double deltaTime) {
    point->position.x += point->velocity.x * deltaTime;
    point->position.y += point->velocity.y * deltaTime;
}

void handleReflection(Point *point) {
    if (point->position.x < POINT_RADIUS || point->position.x > WINDOW_WIDTH - POINT_RADIUS) {
        point->velocity.x = -point->velocity.x;
    }

    if (point->position.y < POINT_RADIUS || point->position.y > WINDOW_HEIGHT - POINT_RADIUS) {
        point->velocity.y = -point->velocity.y;
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Reflecting Point Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Dynamically allocate memory for the points array
    Point *points = (Point *)malloc(NUM_POINTS * sizeof(Point));

    // Initialize points with random positions and velocities
    for (int i = 0; i < NUM_POINTS; i++) {
        points[i].position.x = rand() % WINDOW_WIDTH;
        points[i].position.y = rand() % WINDOW_HEIGHT;
        points[i].velocity.x = (rand() % 2 == 0) ? POINT_SPEED : -POINT_SPEED;
        points[i].velocity.y = (rand() % 2 == 0) ? POINT_SPEED : -POINT_SPEED;
    }

    Uint32 prevTime = SDL_GetTicks();

    while (1) {
        SDL_Event event;
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        Uint32 currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - prevTime) / 1000.0;
        prevTime = currentTime;

        for (int i = 0; i < NUM_POINTS; i++) {
            updatePosition(&points[i], deltaTime);
            handleReflection(&points[i]);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_Rect pointRect = { (int)(points[i].position.x - POINT_RADIUS), (int)(points[i].position.y - POINT_RADIUS), 2 * POINT_RADIUS, 2 * POINT_RADIUS };
            SDL_RenderFillRect(renderer, &pointRect);
        }

        SDL_RenderPresent(renderer);
    }

    // Free dynamically allocated memory
    free(points);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

