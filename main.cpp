#include <SDL.h>
#include <SDL_ttf.h> 
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>


std::chrono::time_point<std::chrono::steady_clock> speedIncreaseTime1;
std::chrono::time_point<std::chrono::steady_clock> speedIncreaseTime2;

int p1BallSpeedX = 0;
int p2BallSpeedX = 0;
int p1BallSpeedY = 0;
int p2BallSpeedY = 0;
int p1PaddleSpeed = 1;
int p2PaddleSpeed = 1;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int BLOCK_GAP = 2;
const int BLOCK_WIDTH = (SCREEN_WIDTH / 20) - BLOCK_GAP;
const int BLOCK_HEIGHT = 20 - BLOCK_GAP;
const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 20;
const int BALL_SIZE = 10;
const int PLAYER_LIFE = 3;
const int BALL1_initX = 400;
const int BALL2_initX = 800;
const int BALL_initY = 400;
int initialSpeed = 5;
int minBallY = 15 * (BLOCK_HEIGHT + BLOCK_GAP);
int maxBallY = SCREEN_HEIGHT - 50 - PADDLE_HEIGHT - BALL_SIZE;

const int TIMER_X = SCREEN_WIDTH / 2;
const int TIMER_Y = 20;
const int TIMER_FONT_SIZE = 24;

Uint32 startTime = SDL_GetTicks();
int elapsedTime = 0;
int remainingTime = 60000;

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

void randomItem(Ball& ball, int player) {
    int randNum = rand() % 5 + 1;
    if (randNum == 1) {
        if (player == 1) {
            speedIncreaseTime1 = std::chrono::steady_clock::now();
            p1PaddleSpeed = 4;
            if (ball.dx < 0)
                p1BallSpeedX = -2;
            else
                p1BallSpeedX = 2;
            if (ball.dy < 0)
                p1BallSpeedY = -2;
            else
                p1BallSpeedY = 2;
        }
        else {
            speedIncreaseTime2 = std::chrono::steady_clock::now();
            p2PaddleSpeed = 4;
            if (ball.dx < 0)
                p2BallSpeedX = -2;
            else
                p2BallSpeedX = 2;
            if (ball.dy < 0)
                p2BallSpeedY = -2;
            else
                p2BallSpeedY = 2;
        }
    }
}


void moveBall(Ball& ball, std::vector<Block>& blocks, Paddle& paddle1, Paddle& paddle2, int& score1, int& score2) {


    if (ball.color.r == 0 && ball.color.b == 255) {
        ball.rect.x += ball.dx + p1BallSpeedX;
        ball.rect.y += ball.dy + p1BallSpeedY;
    }
    else if (ball.color.r == 255 && ball.color.b == 0) {
        ball.rect.x += ball.dx + p2BallSpeedX;
        ball.rect.y += ball.dy + p2BallSpeedY;
    }

    if (ball.rect.x <= 0 || ball.rect.x + BALL_SIZE >= SCREEN_WIDTH) {
        ball.dx = -ball.dx;
        if (ball.color.r == 0 && ball.color.b == 255) {
            p1BallSpeedX = -p1BallSpeedX;
        }
        else if (ball.color.r == 255 && ball.color.b == 0) {
            p2BallSpeedX = -p2BallSpeedX;
        }
    }
    if (ball.rect.y <= 0) {
        ball.dy = -ball.dy;
        if (ball.color.r == 0 && ball.color.b == 255) {
            p1BallSpeedY = -p1BallSpeedY;
        }
        else if (ball.color.r == 255 && ball.color.b == 0) {
            p2BallSpeedY = -p2BallSpeedY;
        }
    }

    if (ball.rect.y + BALL_SIZE >= SCREEN_HEIGHT) {
        if (checkCollision(ball.rect, paddle1.rect)) {
            ball.dy = -ball.dy;
            if (ball.color.r == 0 && ball.color.b == 255) {
                p1BallSpeedY = -p1BallSpeedY;
            }
            else if (ball.color.r == 255 && ball.color.b == 0) {
                p2BallSpeedY = -p2BallSpeedY;
            }
        }
        else if (checkCollision(ball.rect, paddle2.rect)) {
            ball.dy = -ball.dy;
            if (ball.color.r == 0 && ball.color.b == 255) {
                p1BallSpeedY = -p1BallSpeedY;
            }
            else if (ball.color.r == 255 && ball.color.b == 0) {
                p2BallSpeedY = -p2BallSpeedY;
            }
        }
        else {
            if (ball.color.r == 0 && ball.color.b == 255) {
                paddle1.life -= 1;
                if (paddle1.life <= 0) {
                    ball.rect.x = 1 + BALL_SIZE;
                    ball.rect.y = SCREEN_WIDTH - BALL_SIZE - 1;
                    p1BallSpeedX = 0;
                    p1BallSpeedY = 0;
                    p1PaddleSpeed = 1;
                    ball.dx = 0;
                    ball.dy = 0;
                }
                else {
                    ball.rect.x = BALL1_initX;
                    ball.rect.y = BALL_initY;
                    p1BallSpeedX = 0;
                    p1BallSpeedY = 0;
                    p2PaddleSpeed = 1;
                    ball.dx = -initialSpeed;
                    int num = (rand() % 3);
                    if (num == 0) ball.dy = -5;
                    else if (num == 1) ball.dy = -7;
                    else ball.dy = -3;
                }
            }
            else if (ball.color.r == 255 && ball.color.b == 0) {
                paddle2.life -= 1;
                if (paddle2.life <= 0) {
                    ball.rect.x = 1 + BALL_SIZE;
                    ball.rect.y = SCREEN_WIDTH - BALL_SIZE - 1;
                    p2BallSpeedX = 0;
                    p2BallSpeedY = 0;
                    ball.dx = 0;
                    ball.dy = 0;
                }
                else {
                    ball.rect.x = BALL2_initX;
                    ball.rect.y = BALL_initY;
                    p2BallSpeedX = 0;
                    p2BallSpeedY = 0;
                    ball.dx = initialSpeed;
                    int num = (rand() % 3);
                    if (num == 0) ball.dy = -5;
                    else if (num == 1) ball.dy = -7;
                    else ball.dy = -3;
                }
            }
        }
    }

    for (auto& block : blocks) {
        if (block.alive && checkCollision(ball.rect, block.rect)) {
            ball.dy = -ball.dy;
            if (ball.color.r == 0 && ball.color.b == 255) {
                p1BallSpeedY = -p1BallSpeedY;
            }
            else if (ball.color.r == 255 && ball.color.b == 0) {
                p2BallSpeedY = -p2BallSpeedY;
            }
            block.alive = false;
            if (ball.color.r == 0 && ball.color.b == 255) {
                score1++;
                randomItem(ball, 1);
            }
            else {
                randomItem(ball, 2);
                score2++;
            }
            break;
        }
    }

    if (checkCollision(ball.rect, paddle1.rect) && ball.color.r == 0 && ball.color.b == 255) {
        ball.dy = -ball.dy;
        if (ball.color.r == 0 && ball.color.b == 255) {
            p1BallSpeedY = -p1BallSpeedY;
        }
        else if (ball.color.r == 255 && ball.color.b == 0) {
            p2BallSpeedY = -p2BallSpeedY;
        }
    }

    if (checkCollision(ball.rect, paddle2.rect) && ball.color.r == 255 && ball.color.b == 0) {
        ball.dy = -ball.dy;
        if (ball.color.r == 0 && ball.color.b == 255) {
            p1BallSpeedY = -p1BallSpeedY;
        }
        else if (ball.color.r == 255 && ball.color.b == 0) {
            p2BallSpeedY = -p2BallSpeedY;
        }
    }
}

void renderTimer(SDL_Renderer* renderer) {
    int seconds = (remainingTime - elapsedTime) / 1000;

    int minutes = seconds / 60;
    seconds %= 60;

    SDL_Color textColor = { 0, 0, 0, 255 };
    char timerText[20];
    sprintf(timerText, "Time: %02d:%02d", minutes, seconds);

    TTF_Font* font = TTF_OpenFont("arial.ttf", TIMER_FONT_SIZE);
    if (font == NULL) {
        std::cerr << "Failed to load font! Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, timerText, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = { TIMER_X, TIMER_Y, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

void updateTimer() {
    Uint32 currentTime = SDL_GetTicks();
    elapsedTime = currentTime - startTime;

    remainingTime = 60000 - elapsedTime;
    if (remainingTime < 0) {
        remainingTime = 0;
    }
}

void renderScores(SDL_Renderer* renderer, int score1, int score2) {
    SDL_Color textColor = { 0, 0, 0, 255 };
    char scoreText[20];
    sprintf(scoreText, "Player 1: %d", score1);
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (font == NULL) {
        std::cerr << "Failed to load font! Error: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = { 20, 20, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);

    sprintf(scoreText, "Player 2: %d", score2);
    font = TTF_OpenFont("arial.ttf", 24);
    if (font == NULL) {
        std::cerr << "Failed to load font! Error: " << TTF_GetError() << std::endl;
        return;
    }
    textSurface = TTF_RenderText_Solid(font, scoreText, textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect = { SCREEN_WIDTH - textSurface->w - 20, 20, textSurface->w, textSurface->h }; // 상단 우측에 위치하도록 설정
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

void renderWinner(SDL_Renderer* renderer, int score1, int score2) {
    SDL_Color textColor = { 0, 0, 0, 255 };
    std::string winnerText;
    if (score1 > score2) {
        winnerText = "Player 1 Wins!";
    }
    else if (score1 < score2) {
        winnerText = "Player 2 Wins!";
    }
    else {
        winnerText = "It's a Tie!";
    }

    TTF_Font* font = TTF_OpenFont("arial.ttf", 36);
    if (font == NULL) {
        std::cerr << "Failed to load font! Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, winnerText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textWidth = 0;
    int textHeight = 0;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textRect = { (SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - textHeight) / 2, textWidth, textHeight };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    renderScores(renderer, score1, score2);

    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}


int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window* window = SDL_CreateWindow("Block Breaker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Paddle paddle1 = { { SCREEN_WIDTH / 4 - PADDLE_WIDTH / 2, SCREEN_HEIGHT - 50, PADDLE_WIDTH, PADDLE_HEIGHT }, PLAYER_LIFE, { 135, 206, 250, 255 } };
    Paddle paddle2 = { { 3 * SCREEN_WIDTH / 4 - PADDLE_WIDTH / 2, SCREEN_HEIGHT - 50, PADDLE_WIDTH, PADDLE_HEIGHT }, PLAYER_LIFE, { 255, 182, 193, 255 } };

    std::vector<Block> blocks;
    initBlocks(blocks);

    srand(time(0));

    Ball ball1 = {
       {BALL1_initX, BALL_initY, BALL_SIZE, BALL_SIZE },
       -initialSpeed,
       (rand() % 3),
       { 0, 0, 255, 255 }
    };
    if (ball1.dy == 0) ball1.dy= -5;
    else if (ball1.dy == 1) ball1.dy= -7;
    else ball1.dy = -3;

    Ball ball2 = {
        {BALL2_initX, BALL_initY, BALL_SIZE, BALL_SIZE },
        initialSpeed,
        (rand() % 3),
        { 255, 0, 0, 255 }
    };
    if (ball2.dy == 0) ball2.dy = -5;
    else if (ball2.dy == 1) ball2.dy = -7;
    else ball2.dy = -3;
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
            paddle1.rect.x -= (5 + p1PaddleSpeed);
        }
        if (currentKeyStates[SDL_SCANCODE_D] && paddle1.rect.x < SCREEN_WIDTH - PADDLE_WIDTH) {
            paddle1.rect.x += (5 + p1PaddleSpeed);
        }
        if (currentKeyStates[SDL_SCANCODE_LEFT] && paddle2.rect.x > 0) {
            paddle2.rect.x -= (5 + p2PaddleSpeed);
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT] && paddle2.rect.x < SCREEN_WIDTH - PADDLE_WIDTH) {
            paddle2.rect.x += (5 + p2PaddleSpeed);
        }

        auto currentTime = std::chrono::steady_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::seconds>(currentTime - speedIncreaseTime1);
        auto duration2 = std::chrono::duration_cast<std::chrono::seconds>(currentTime - speedIncreaseTime2);
        if (duration1.count() >= 5) {
            p1BallSpeedX = 0;
            p1BallSpeedY = 0;
            p1PaddleSpeed = 1;
        }
        if (duration2.count() >= 5) {
            p2BallSpeedX = 0;
            p2BallSpeedY = 0;
            p2PaddleSpeed = 1;
        }

        moveBall(ball1, blocks, paddle1, paddle2, score1, score2);
        moveBall(ball2, blocks, paddle1, paddle2, score1, score2);

        updateTimer();

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

        renderScores(renderer, score1, score2);

        renderTimer(renderer);

        if ((remainingTime - elapsedTime <= 0) || ((paddle1.life <= 0) && (paddle2.life <= 0))) {
            renderWinner(renderer, score1, score2);

            quit = true;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
