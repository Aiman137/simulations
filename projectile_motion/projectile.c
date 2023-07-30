#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h> // SDL graphics library

#define GRAVITY 9.81 // Acceleration due to gravity (m/s^2)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    double x;
    double y;
} Vector2D;

void updatePosition(Vector2D *position, Vector2D *velocity, double timeStep) {
    position->x += velocity->x * timeStep;
    position->y += velocity->y * timeStep;
}

void updateVelocity(Vector2D *velocity, double timeStep) {
    velocity->y -= GRAVITY * timeStep;
}

int main() {
    double initialVelocity; // Initial velocity (m/s)
    double launchAngle;     // Launch angle (degrees)
    double timeStep = 0.001;  // Time step (s)
    Vector2D position = {0.0, 0.0};
    Vector2D velocity;

    // Get user input for initial velocity and launch angle
    printf("Enter the initial velocity (m/s): ");
    scanf("%lf", &initialVelocity);
    printf("Enter the launch angle (degrees): ");
    scanf("%lf", &launchAngle);

    // Convert launch angle to radians
    double launchAngleRad = launchAngle * M_PI / 180.0;

    // Calculate initial velocity components
    velocity.x = initialVelocity * cos(launchAngleRad);
    velocity.y = initialVelocity * sin(launchAngleRad);

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("Projectile Motion Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Simulation loop with visualization
    while (position.y >= 0.0) {
        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw the projectile on the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect projectileRect = { (int)position.x, 600 - (int)position.y, 5, 5 };
        SDL_RenderFillRect(renderer, &projectileRect);

        // Update the display
        SDL_RenderPresent(renderer);

        updateVelocity(&velocity, timeStep);
        updatePosition(&position, &velocity, timeStep);

        timeStep += 0.001; // Increment time by the time step
        SDL_Delay(100); // Delay for smooth visualization
    }

    // Clean up and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

