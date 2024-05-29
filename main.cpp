#include <SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BLOCK_GAP = 2;
const int BLOCK_WIDTH = (SCREEN_WIDTH / 20) - BLOCK_GAP;
const int BLOCK_HEIGHT = 20 - BLOCK_GAP;
const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 20;
const int BALL_SIZE = 10;
const int PLAYER_LIFE = 3;
const int BALL1_initX = 250;
const int BALL2_initX = 550;
const int BALL_initY = 400;
int initialSpeed = 3;
int minBallY = 15 * (BLOCK_HEIGHT + BLOCK_GAP);
int maxBallY = SCREEN_HEIGHT - 50 - PADDLE_HEIGHT - BALL_SIZE;

struct Paddle {
    SDL_Rect rect;
    int life;
    SDL_Color color;
};

struct Ball {
    SDL_Rect rect;
    int dx;
    int dy;
    SDL_Color color;
};

struct Block {
    SDL_Rect rect;
    bool alive;
};

void initBlocks(std::vector<Block>& blocks) {
    blocks.clear();
    for (int y = 0; y < 15; ++y) {
        for (int x = 0; x < 20; ++x) {
            Block block;
            block.rect = { x * (BLOCK_WIDTH + BLOCK_GAP), y * (BLOCK_HEIGHT + BLOCK_GAP), BLOCK_WIDTH, BLOCK_HEIGHT };
            block.alive = true;
            blocks.push_back(block);
        }
    }
}

bool checkCollision(SDL_Rect a, SDL_Rect b) {
    return SDL_HasIntersection(&a, &b);
}

void moveBall(Ball& ball, std::vector<Block>& blocks, Paddle& paddle1, Paddle& paddle2, int& score1, int& score2) {
    ball.rect.x += ball.dx;
    ball.rect.y += ball.dy;

    if (ball.rect.x <= 0 || ball.rect.x + BALL_SIZE >= SCREEN_WIDTH) {
        ball.dx = -ball.dx;
    }
    if (ball.rect.y <= 0) {
        ball.dy = -ball.dy;
    }

    if (ball.rect.y + BALL_SIZE >= SCREEN_HEIGHT) {
        if (checkCollision(ball.rect, paddle1.rect)) {
            ball.dy = -ball.dy;
        }
        else if (checkCollision(ball.rect, paddle2.rect)) {
            ball.dy = -ball.dy;
        }
        else {
            ball.dy = -ball.dy;
            if (ball.color.r == 0 && ball.color.b == 255) {
                paddle1.life -= 1;
                ball.rect.x = BALL1_initX;
                ball.rect.y = BALL_initY;
            }
            else if (ball.color.r == 255 && ball.color.b == 0) {
                paddle2.life -= 1;
                ball.rect.x = BALL2_initX;
                ball.rect.y = BALL_initY;
            }
        }
    }

    for (auto& block : blocks) {
        if (block.alive && checkCollision(ball.rect, block.rect)) {
            ball.dy = -ball.dy;
            block.alive = false;
            if (ball.color.r == 0 && ball.color.b == 255) {
                score1++;
            }
            else {
                score2++;
            }
            break;
        }
    }

    if (checkCollision(ball.rect, paddle1.rect) && ball.color.r == 0 && ball.color.b == 255) {
        ball.dy = -ball.dy;
    }

    if (checkCollision(ball.rect, paddle2.rect) && ball.color.r == 255 && ball.color.b == 0) {
        ball.dy = -ball.dy;
    }
}

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Block Breaker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Paddle paddle1 = { { SCREEN_WIDTH / 4 - PADDLE_WIDTH / 2, SCREEN_HEIGHT - 50, PADDLE_WIDTH, PADDLE_HEIGHT }, PLAYER_LIFE, { 135, 206, 250, 255 } };
    Paddle paddle2 = { { 3 * SCREEN_WIDTH / 4 - PADDLE_WIDTH / 2, SCREEN_HEIGHT - 50, PADDLE_WIDTH, PADDLE_HEIGHT }, PLAYER_LIFE, { 255, 182, 193, 255 } };

    std::vector<Block> blocks;
    initBlocks(blocks);

    srand(time(0));
    Ball ball1 = {
    {BALL1_initX, BALL_initY, BALL_SIZE, BALL_SIZE },
    initialSpeed,
    -initialSpeed,
    { 0, 0, 255, 255 }
    };

    Ball ball2 = {
        {BALL2_initX, BALL_initY, BALL_SIZE, BALL_SIZE },
        -initialSpeed,
        -initialSpeed,
        { 255, 0, 0, 255 }
    };

    int score1 = 0;
    int score2 = 0;

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        if (currentKeyStates[SDL_SCANCODE_A] && paddle1.rect.x > 0) {
            paddle1.rect.x -= 5;
        }
        if (currentKeyStates[SDL_SCANCODE_D] && paddle1.rect.x < SCREEN_WIDTH - PADDLE_WIDTH) {
            paddle1.rect.x += 5;
        }
        if (currentKeyStates[SDL_SCANCODE_LEFT] && paddle2.rect.x > 0) {
            paddle2.rect.x -= 5;
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT] && paddle2.rect.x < SCREEN_WIDTH - PADDLE_WIDTH) {
            paddle2.rect.x += 5;
        }

        moveBall(ball1, blocks, paddle1, paddle2, score1, score2);
        moveBall(ball2, blocks, paddle1, paddle2, score1, score2);

        /* life 관리, 현재 주석안의 내용은 게임을 끝내기에 수정할 필요성 있음
        if (paddle1.life <= 0 || paddle2.life <= 0) {
            quit = true;
        }
        */
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for (const auto& block : blocks) {
            if (block.alive) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderFillRect(renderer, &block.rect);
            }
        }

        SDL_SetRenderDrawColor(renderer, paddle1.color.r, paddle1.color.g, paddle1.color.b, paddle1.color.a);
        SDL_RenderFillRect(renderer, &paddle1.rect);
        SDL_SetRenderDrawColor(renderer, paddle2.color.r, paddle2.color.g, paddle2.color.b, paddle2.color.a);
        SDL_RenderFillRect(renderer, &paddle2.rect);

        SDL_SetRenderDrawColor(renderer, ball1.color.r, ball1.color.g, ball1.color.b, ball1.color.a);
        SDL_RenderFillRect(renderer, &ball1.rect);
        SDL_SetRenderDrawColor(renderer, ball2.color.r, ball2.color.g, ball2.color.b, ball2.color.a);
        SDL_RenderFillRect(renderer, &ball2.rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
