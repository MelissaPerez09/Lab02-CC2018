#include <SDL2/SDL.h>

const int FRAMEBUFFER_WIDTH = 80;
const int FRAMEBUFFER_HEIGHT = 80;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FRAMEBUFFER_SIZE = FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT;

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};
Color currentColor = {255, 192, 203, 255};

Color framebuffer[FRAMEBUFFER_SIZE];

bool cells[FRAMEBUFFER_WIDTH][FRAMEBUFFER_HEIGHT] = {false};

// Obtener las coordenadas válidas considerando el "loop" toroidal
void getValidCoordinates(int& x, int& y) {
    if (x < 0) {
        x = FRAMEBUFFER_WIDTH - 1;
    } else if (x >= FRAMEBUFFER_WIDTH) {
        x = 0;
    }

    if (y < 0) {
        y = FRAMEBUFFER_HEIGHT - 1;
    } else if (y >= FRAMEBUFFER_HEIGHT) {
        y = 0;
    }
}

int x = 10;
int y = 10;

// Function to set a specific pixel in the framebuffer to the currentColor
void point(int x, int y) {
    if (x >= 0 && x < FRAMEBUFFER_WIDTH && y >= 0 && y < FRAMEBUFFER_HEIGHT) {
        framebuffer[y * FRAMEBUFFER_WIDTH + x] = currentColor;
    }
}

void renderBuffer(SDL_Renderer* renderer) {
    // Create a texture
    SDL_Texture* texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,
        FRAMEBUFFER_WIDTH, 
        FRAMEBUFFER_HEIGHT
    );

    // Update the pixel data in the framebuffer
    for (int y = 0; y < FRAMEBUFFER_HEIGHT; ++y) {
        for (int x = 0; x < FRAMEBUFFER_WIDTH; ++x) {
            Color pixelColor = cells[x][y] ? currentColor : Color{128, 128, 144, 255};
            framebuffer[y * FRAMEBUFFER_WIDTH + x] = pixelColor;
        }
    }

    // Update the texture with the pixel data from the framebuffer
    SDL_UpdateTexture(
        texture, 
        NULL, 
        framebuffer, 
        FRAMEBUFFER_WIDTH * sizeof(Color)
    );

    // Copy the texture to the renderer
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Destroy the texture
    SDL_DestroyTexture(texture);
}

void render(SDL_Renderer* renderer) {
    // Draw a point
    point(x++, y++);

    // Render the framebuffer to the screen
    renderBuffer(renderer);
}

bool getNextCellState(int x, int y) {
    int aliveNeighbors = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            int targetX = (x + dx + FRAMEBUFFER_WIDTH) % FRAMEBUFFER_WIDTH;
            int targetY = (y + dy + FRAMEBUFFER_HEIGHT) % FRAMEBUFFER_HEIGHT;

            if (cells[targetX][targetY]) {
                aliveNeighbors++;
            }
        }
    }

    if (cells[x][y]) {
        return aliveNeighbors == 2 || aliveNeighbors == 3;
    } else {
        return aliveNeighbors == 3;
    }
}

void updateCells() {
    bool nextGeneration[FRAMEBUFFER_WIDTH][FRAMEBUFFER_HEIGHT] = {false};
    for (int y = 0; y < FRAMEBUFFER_HEIGHT; ++y) {
        for (int x = 0; x < FRAMEBUFFER_WIDTH; ++x) {
            nextGeneration[x][y] = getNextCellState(x, y);
        }
    }

    // Copy the next generation to the cells array
    for (int y = 0; y < FRAMEBUFFER_HEIGHT; ++y) {
        for (int x = 0; x < FRAMEBUFFER_WIDTH; ++x) {
            cells[x][y] = nextGeneration[x][y];
        }
    }
}



// Patrón: Block
void addBlockPattern(int startX, int startY) {
    cells[(startX + 0) % FRAMEBUFFER_WIDTH][(startY + 0) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 1) % FRAMEBUFFER_WIDTH][(startY + 0) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 0) % FRAMEBUFFER_WIDTH][(startY + 1) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 1) % FRAMEBUFFER_WIDTH][(startY + 1) % FRAMEBUFFER_HEIGHT] = true;
}

// Patrón: Blinker
void addBlinkerPattern(int startX, int startY) {
    cells[(startX + 0) % FRAMEBUFFER_WIDTH][(startY + 0) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 1) % FRAMEBUFFER_WIDTH][(startY + 0) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 2) % FRAMEBUFFER_WIDTH][(startY + 0) % FRAMEBUFFER_HEIGHT] = true;
}

// Patrón: Pentadecathlon
void addPentaPattern(int startX, int startY) {
    // Patrón Pentadecathlon con orientación vertical
    cells[(startX + 0) % FRAMEBUFFER_WIDTH][(startY + 0) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 0) % FRAMEBUFFER_WIDTH][(startY + 1) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 0) % FRAMEBUFFER_WIDTH][(startY + 2) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 1) % FRAMEBUFFER_WIDTH][(startY + 1) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 2) % FRAMEBUFFER_WIDTH][(startY + 0) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 2) % FRAMEBUFFER_WIDTH][(startY + 1) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 2) % FRAMEBUFFER_WIDTH][(startY + 2) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 3) % FRAMEBUFFER_WIDTH][(startY + 0) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 3) % FRAMEBUFFER_WIDTH][(startY + 1) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 3) % FRAMEBUFFER_WIDTH][(startY + 2) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 4) % FRAMEBUFFER_WIDTH][(startY + 0) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 4) % FRAMEBUFFER_WIDTH][(startY + 1) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 4) % FRAMEBUFFER_WIDTH][(startY + 2) % FRAMEBUFFER_HEIGHT] = true;
}

// Patrón: Glider
void addGliderPattern(int startX, int startY) {
    cells[(startX + 1) % FRAMEBUFFER_WIDTH][(startY + 0) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 2) % FRAMEBUFFER_WIDTH][(startY + 1) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 0) % FRAMEBUFFER_WIDTH][(startY + 2) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 1) % FRAMEBUFFER_WIDTH][(startY + 2) % FRAMEBUFFER_HEIGHT] = true;
    cells[(startX + 2) % FRAMEBUFFER_WIDTH][(startY + 2) % FRAMEBUFFER_HEIGHT] = true;
}

// Patrón: Pulsar

bool pulsarPattern[3][3] = {
    {false, true, false},
    {true, true, true},
    {false, true, false}
};
void addPulsarPattern(int startX, int startY) {
    int width = 3;
    int height = 3;

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            int targetX = (startX + x) % FRAMEBUFFER_WIDTH;
            int targetY = (startY + y) % FRAMEBUFFER_HEIGHT;
            cells[targetX][targetY] = pulsarPattern[x][y];
        }
    }
}



int main() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("The Game Of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event event;

    int centerX = FRAMEBUFFER_WIDTH / 2;
    int centerY = FRAMEBUFFER_HEIGHT / 2;

    // Agregar patrón Block
    addBlockPattern(40, 20);
    addBlockPattern(20, 40);
    
    // Agregar patrón Blinker
    addBlinkerPattern(centerX - 5, centerY);
    addBlinkerPattern(centerX - 15, centerY - 10);
    addBlinkerPattern(centerX - 25, centerY - 20);
    addBlinkerPattern(centerX - 35, centerY - 30);
    addBlinkerPattern(centerX + 5, centerY + 10);
    addBlinkerPattern(centerX + 15, centerY + 20);
    addBlinkerPattern(centerX + 25, centerY + 30);

    // Agregar patrón Pentadecathlon
    for (int i = 0; i < 10; ++i) {
        addPentaPattern(centerX, centerY);
        centerY += 10;
    }

    int pentadecathlonX = 20;
    int pentadecathlonY = 20;
    for (int i = 0; i < 10; ++i) {
        addPentaPattern(pentadecathlonX, pentadecathlonY);
        pentadecathlonY += 5; 
    }

    // Agregar patrón Glider
    for (int i = 0; i < 15; ++i) {
        addGliderPattern(centerX + i * 5, centerY);
    }
    for (int i = 0; i < 15; ++i) {
        addGliderPattern(centerX + i * 5, centerY + 50); 
    }

    // Agregar patrón Pulsar
    int pulsarX = 30;
    int pulsarY = 10;
    for (int i = 0; i < 10; ++i) {
        addPulsarPattern(pulsarX, pulsarY);
        pulsarX += 10;
    }

    int pulsarX2 = 10;
    int pulsarY2 = 60;
    for (int i = 0; i < 10; ++i) {
        addPulsarPattern(pulsarX2, pulsarY2);
        pulsarX2 += 10;
    }

    bool isRunning = true;
    while (isRunning) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        SDL_RenderClear(renderer);

        render(renderer);
        updateCells(); // Agregamos la función para evolucionar los patrones

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}