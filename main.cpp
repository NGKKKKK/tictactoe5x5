#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "PLAYGAME.h"

using namespace std;

int main(int argc, char* argv[]) {
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    SDL_Window* window = SDL_CreateWindow(TILE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;

    SDL_Texture* Menu = IMG_LoadTexture(renderer, "Image/Menu.png");
    SDL_Texture* PlayButtonIMG = IMG_LoadTexture(renderer, "Image/PlayButton.png");
    SDL_Texture* ExitButtonIMG = IMG_LoadTexture(renderer, "Image/ExitButton.png");
    SDL_Texture* RetryButtonIMG = IMG_LoadTexture(renderer, "Image/RetryButton.png");
    SDL_Texture* EndDrawIMG = IMG_LoadTexture(renderer, "Image/EndDraw.png");
    SDL_Texture* EndPlayerXIMG = IMG_LoadTexture(renderer, "Image/EndPLayerX.png");
    SDL_Texture* EndPlayerOIMG = IMG_LoadTexture(renderer, "Image/EndPlayerO.png");
    SDL_Texture* Board = IMG_LoadTexture(renderer, "Image/Board.png");
    SDL_Texture* X_Board = IMG_LoadTexture(renderer, "Image/X_Board.png");
    SDL_Texture* O_Board = IMG_LoadTexture(renderer, "Image/O_Board.png");

    bool game_running = true;
    int STATE = MENU_STATE;

    SDL_Rect PlayButton = { 150, 300, BUTTON_WIDTH, BUTTON_HEIGHT };
    SDL_Rect ExitButton = { 450, 300, BUTTON_WIDTH, BUTTON_HEIGHT };
    SDL_Rect RetryButton = { 300, 300, BUTTON_WIDTH, BUTTON_HEIGHT };

    int board[ROWS][COLS] = {};
    int currentPlayer = 1; // 1 - X, 2 - O
    int type_of_over = 0; //0 - Draw, 1 - PlayerX, 2 - PlayerO

    while (game_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) game_running = false;

            if (STATE == MENU_STATE) {
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    int x = event.button.x, y = event.button.y;
                    if (ClickCheck(PlayButton, x, y)) {
                        STATE = PLAY_STATE;
                        ResetBoard(board);
                        currentPlayer = 1;
                    }
                    if (ClickCheck(ExitButton, x, y)) {
                        STATE = EXIT_STATE;
                        game_running = false;
                    }
                }
            } else if (STATE == PLAY_STATE) {
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    int boardX = (SCREEN_WIDTH - CELL_SIZE * COLS) / 2;
                    int boardY = (SCREEN_HEIGHT - CELL_SIZE * ROWS) / 2;
                    int x = event.button.x - boardX;
                    int y = event.button.y - boardY;
                    if (x >= 0 && y >= 0 && x / CELL_SIZE < COLS && y / CELL_SIZE < ROWS) {
                        int row = y / CELL_SIZE, col = x / CELL_SIZE;
                        if (board[row][col] == 0) {
                            board[row][col] = currentPlayer;
                            int winner = CheckWin(board);
                            if (winner || BoardFull(board)){
                                STATE = GAME_OVER_STATE;
                                if (winner){
                                    if (winner == 1) type_of_over = 1;
                                    else if (winner == 2) type_of_over = 2;
                                }
                            }
                            else currentPlayer = (currentPlayer == 1) ? 2 : 1;
                        }
                    }
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) STATE = MENU_STATE;
            } else if (STATE == GAME_OVER_STATE) {
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                    int x = event.button.x, y = event.button.y;
                    if (ClickCheck(RetryButton, x, y)){
                        STATE = PLAY_STATE;
                        ResetBoard(board);
                        currentPlayer = 1;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        if (STATE == MENU_STATE) RenderMenu(renderer, Menu, PlayButtonIMG, ExitButtonIMG, PlayButton, ExitButton);
        else if (STATE == PLAY_STATE) RenderBoard(renderer, Board, X_Board, O_Board, board);
        else if (STATE == GAME_OVER_STATE) RenderEnd(renderer, EndDrawIMG, EndPlayerXIMG, EndPlayerOIMG, RetryButton, RetryButtonIMG, type_of_over);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
