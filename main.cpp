// g++ -std=c++20 main.cpp -o window -I/opt/homebrew/opt/sdl2/include -L/opt/homebrew/opt/sdl2/lib -lSDL2
// ./window
// make

#include <SDL2/SDL.h>
#include <vector>

struct Point {
    int x;
    int y;
};

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO); // Initialize SDL

    SDL_Window* window = SDL_CreateWindow(
        "abys",            // Window title
        SDL_WINDOWPOS_CENTERED,  // X-position
        SDL_WINDOWPOS_CENTERED,  // Y-position
        800,                     // Width
        600,                     // Height
        SDL_WINDOW_SHOWN         // Window flags
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool quit = false;
    bool isDrawing = false; // Flag to indicate if the mouse button is down
    SDL_Event event;

    std::vector<Point> currentStroke; // Store current stroke points
    std::vector<std::vector<Point>> allStrokes; // Store all strokes
    std::vector<std::vector<Point>> undoStack;  // Store strokes for undo

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isDrawing = true;
                    currentStroke.clear(); // Start a new stroke
                    Point startPoint = { event.button.x, event.button.y };
                    currentStroke.push_back(startPoint);
                }
            } else if (event.type == SDL_MOUSEMOTION && isDrawing) {
                Point point = { event.motion.x, event.motion.y };
                currentStroke.push_back(point); // Add point to current stroke
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isDrawing = false;
                    allStrokes.push_back(currentStroke); // Store the current stroke
                    currentStroke.clear();
                    undoStack.clear(); // Clear undo stack when a new stroke is completed
                }
            } else if (event.type == SDL_KEYDOWN) {
                if ((event.key.keysym.sym == SDLK_z) && (event.key.keysym.mod & KMOD_GUI)) {
                    if (!allStrokes.empty()) {
                        undoStack.push_back(allStrokes.back()); // Add stroke to undo stack
                        allStrokes.pop_back(); // Remove the most recent stroke
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set background color to white
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set color to black
        for (const auto& stroke : allStrokes) {
            for (size_t i = 1; i < stroke.size(); ++i) {
                SDL_RenderDrawLine(renderer, stroke[i - 1].x, stroke[i - 1].y, stroke[i].x, stroke[i].y);
            }
        }

        if (isDrawing && !currentStroke.empty()) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set color to black
            for (size_t i = 1; i < currentStroke.size(); ++i) {
                SDL_RenderDrawLine(renderer, currentStroke[i - 1].x, currentStroke[i - 1].y, currentStroke[i].x, currentStroke[i].y);
            }
        }

        SDL_RenderPresent(renderer); // Update the screen
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window); // Destroy the window
    SDL_Quit(); // Quit SDL

    return 0;
}
