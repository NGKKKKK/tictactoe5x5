#ifndef PLAYGAME_H_INCLUDED
#define PLAYGAME_H_INCLUDED

#include <iostream>
#include <SDL.h>;
#include <SDL_image.h>
#include "defs.h"

using namespace std;

bool ClickCheck(const SDL_Rect &need, int x, int y) {
    return (x >= need.x && x <= need.x + need.w &&
            y >= need.y && y <= need.y + need.h);
}

int CheckWin(int board[ROWS][COLS]) {
    //Check hàng
    for (int i = 0; i < ROWS; i++) {
        if (board[i][0]){
            bool check = true;
            for (int j = 1; j < COLS; j++){
                if (board[i][j] != board[i][j-1]){
                    check = false;
                    break;
                }
            }
            if (check) return board[i][0];
        }
    }

    //Check cột
    for (int i = 0; i < COLS; i++) {
        if (board[0][i]){
            bool check = true;
            for (int j = 1; j < ROWS; j++){
                if (board[j][i] != board[j-1][i]){
                    check = false;
                    break;
                }
            }
            if (check) return board[0][i];
        }
    }

    //Check đường chéo trái
    if (board[0][0]){
        bool check = true;
        int i = 1;
        int j = 1;
        while (i < ROWS && j < COLS){
            if (board[i][j] != board[i-1][j-1]){
                check = false;
                break;
            }
            i++;
            j++;
        }
        if (check) return board[0][0];
    }

    //Check đường chéo phải
    if (board[0][COLS-1]){
        bool check = true;
        int i = 1;
        int j = COLS-2;
        while (i < ROWS && j >= 0){
            if (board[i][j] != board[i-1][j+1]){
                check = false;
                break;
            }
            i++;
            j--;
        }
        if (check) return board[0][COLS-1];
    }
    return 0;
}

bool BoardFull(int board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (board[i][j] == 0) return false;
    return true;
}

void RenderBoard(SDL_Renderer* renderer, SDL_Texture* Board, SDL_Texture* X_Board, SDL_Texture* O_Board, int board[ROWS][COLS]) {
    const int GAP = 5; // Khoảng cách giữa các ô
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Nền trắng
    SDL_RenderClear(renderer);

    int boardWidth = COLS * CELL_SIZE + (COLS - 1) * GAP;
    int boardHeight = ROWS * CELL_SIZE + (ROWS - 1) * GAP;
    int startX = (SCREEN_WIDTH - boardWidth) / 2;
    int startY = (SCREEN_HEIGHT - boardHeight) / 2;

    // Vẽ từng ô với khoảng cách
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            SDL_Rect cellRect = {
                startX + j * (CELL_SIZE + GAP),
                startY + i * (CELL_SIZE + GAP),
                CELL_SIZE,
                CELL_SIZE
            };
            //Ô mặc định
            if (board[i][j] == 0) SDL_RenderCopy(renderer, Board, NULL, &cellRect);

            // Hiển thị X hoặc O
            else if (board[i][j] == 1) SDL_RenderCopy(renderer, X_Board, NULL, &cellRect);
            else if (board[i][j] == 2) SDL_RenderCopy(renderer, O_Board, NULL, &cellRect);
        }
    }
}

void ResetBoard(int board[ROWS][COLS]){
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            board[i][j] = 0;
        }
    }
}

void RenderMenu(SDL_Renderer* renderer, SDL_Texture* Menu, SDL_Texture* PlayButtonIMG, SDL_Texture* ExitButtonIMG, SDL_Rect PlayButton, SDL_Rect ExitButton) {
    SDL_RenderCopy(renderer, Menu, NULL, NULL);
    SDL_RenderCopy(renderer, PlayButtonIMG, NULL, &PlayButton);
    SDL_RenderCopy(renderer, ExitButtonIMG, NULL, &ExitButton);
}

void RenderEnd(SDL_Renderer* renderer, SDL_Texture* EndDrawIMG, SDL_Texture* EndPlayerXIMG, SDL_Texture* EndPlayerOIMG,
                SDL_Rect RetryButton, SDL_Texture* RetryButtonIMG, int type_of_over){
    if (type_of_over == 0) SDL_RenderCopy(renderer, EndDrawIMG, NULL, NULL);
    else if (type_of_over == 1) SDL_RenderCopy(renderer, EndPlayerXIMG, NULL, NULL);
    else if (type_of_over == 2) SDL_RenderCopy(renderer, EndPlayerOIMG, NULL, NULL);
    SDL_RenderCopy(renderer, RetryButtonIMG, NULL, &RetryButton);
}

#endif // PLAYGAME_H_INCLUDED
