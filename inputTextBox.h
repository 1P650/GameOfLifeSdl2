#ifndef INPUTTEXTBOX_H
#define INPUTTEXTBOX_H

#define TEXT_LEN 20

#ifndef SDL_TTF_H_
#include <SDL_ttf.h>
#endif



struct inputTextBox{
    char IS_FOCUSED;
    SDL_Rect POSRECT;
    int COLOR_BACKGROUND, COLOR_BORDER;
    char TEXT[TEXT_LEN];
    char MULTITEXT;
    char MAX_SYM;

};

unsigned char pointInInputTextBox(struct inputTextBox* I, int x, int y){
    return ( (x >= I->POSRECT.x) && (x < (I->POSRECT.x + I->POSRECT.w)) &&
            (y >= I->POSRECT.y) && (y < (I->POSRECT.y + I->POSRECT.h)) );
}


void renderInputTextBox(SDL_Renderer* renderer, struct inputTextBox* TEXT_BOX){
    int TEXTBOX_BACK_COLOR = TEXT_BOX->COLOR_BACKGROUND;
    int TEXTBOX_BORDER_COLOR = TEXT_BOX->COLOR_BORDER;
    SDL_SetRenderDrawColor(renderer, TEXTBOX_BACK_COLOR>>24, TEXTBOX_BACK_COLOR>>16, TEXTBOX_BACK_COLOR>>8, TEXTBOX_BACK_COLOR);
    SDL_RenderFillRect(renderer,&TEXT_BOX->POSRECT);
    if(TEXT_BOX->IS_FOCUSED){
        SDL_SetRenderDrawColor(renderer, TEXTBOX_BORDER_COLOR>>24, TEXTBOX_BORDER_COLOR>>16, TEXTBOX_BORDER_COLOR>>8, TEXTBOX_BORDER_COLOR);
        SDL_RenderDrawRect(renderer, &TEXT_BOX->POSRECT);
    }

    renderInputTextBox_Text(renderer, TEXT_BOX);

}



void renderInputTextBox_Text(SDL_Renderer* renderer,  struct inputTextBox* TEXT_BOX){
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

#endif // INPUTTEXTBOX_H
