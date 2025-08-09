#ifndef TEXTBOX_H
#define TEXTBOX_H

#define TTEXT_LEN 20

#ifndef SDL_TTF_H_
#include <SDL_ttf.h>
#endif

struct textBox{
    SDL_Rect POSRECT;
    char TEXT[TTEXT_LEN];
    char MULTITEXT;
    char MAX_SYM;
};

void renderTextBox(SDL_Renderer* renderer, struct textBox* TEXT_BOX){
    int fontSize = 20;
    TTF_Font* Sans = TTF_OpenFont("D:/Projects/C/SDL_Tutorials/SDL_1/SDL_1/FONT.ttf", fontSize);
    SDL_Color BLACK = {0x00, 0x00, 0x00};

    SDL_Surface* surfaceMessage =
        TTF_RenderText_Solid(Sans, &TEXT_BOX->TEXT, BLACK);

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect TEXTPOSRECT = TEXT_BOX->POSRECT;
    if(strlen(&TEXT_BOX->TEXT) >= 1 && TEXT_BOX->MULTITEXT){
        TEXTPOSRECT.w = TEXT_BOX->POSRECT.w / TEXT_BOX->MAX_SYM * strlen(&TEXT_BOX->TEXT);
    }
    SDL_RenderCopy(renderer, Message, NULL, &TEXTPOSRECT);


    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
    TTF_CloseFont(Sans);

}


#endif // TEXTBOX_H
