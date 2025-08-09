#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#ifndef SDL_TTF_H_
#include <SDL_ttf.h>
#endif

struct menuButton{
    char STATE;
    SDL_Rect POSRECT;
    int COLOR_REST, COLOR_PRESSED;
    SDL_Texture* TEXTURE_REST;
    SDL_Texture* TEXTURE_PRESSED;
};

unsigned char pointInButton(struct menuButton *B, int x, int y){
    return ( (x >= B->POSRECT.x) && (x < (B->POSRECT.x + B->POSRECT.w)) &&
            (y >= B->POSRECT.y) && (y < (B->POSRECT.y + B->POSRECT.h)) );
}


void renderButton(SDL_Renderer* renderer, struct menuButton* BUTTON){
    if(BUTTON->TEXTURE_PRESSED == 0 || BUTTON->TEXTURE_REST == 0){
        int BUTTON_CURRCOLOR = BUTTON->STATE*BUTTON->COLOR_PRESSED + (!BUTTON->STATE)*BUTTON->COLOR_REST;
        SDL_SetRenderDrawColor(renderer, BUTTON_CURRCOLOR>>24, BUTTON_CURRCOLOR>>16, BUTTON_CURRCOLOR>>8, BUTTON_CURRCOLOR);
        SDL_RenderFillRect(renderer,&BUTTON->POSRECT);
    }
    else {
        SDL_RenderCopy(renderer, BUTTON->STATE == 0 ? BUTTON->TEXTURE_REST: BUTTON->TEXTURE_PRESSED, 0, &BUTTON->POSRECT);
    }

}
#endif // MENUBUTTON_H
