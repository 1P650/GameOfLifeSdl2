#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <SDL.h>

#include "menuButton.h"
#include "inputTextBox.h"
#include "colors.h"

#define nullptr NULL

#define FIELD_W 640
#define FIELD_H 640

#define WINDOW_W 640+320
#define WINDOW_H 640

#define FIELD_SIZE 64

#define CELL_SIZE FIELD_W/FIELD_SIZE

#define BTN_SIZE_ST 50

#define BTN_SIZE_WW 100

#define FPS_DEFAULT 12

#define BTN_MARGIN 10

#define NUMDEAD_DEFAULT 3
#define NUMALLOW_DEFAULT 2


#define INNER_STATES 3
#define AUTOMATONS_COUNT 5



*/
//const int WINDOW_W = 640;
//const int WINDOW_H = 480;

struct cell{
    unsigned int color;
    unsigned char state;
    unsigned char automaton;
};

unsigned char APP_RUNNING = 0, SIM_RUNNING = 0;
struct cell FIELD[FIELD_SIZE][FIELD_SIZE];

unsigned int FPS = 24;

unsigned char AUTOMATONS[AUTOMATONS_COUNT][INNER_STATES] = { {0,0,0}, {0,1,2}, {0,2,1}, {1,2,0}, {2,1,0} };
unsigned int COLORS[3] = {COLOR_F_C0, COLOR_F_C1, COLOR_F_C2};

SDL_Window *window = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *image = NULL;

SDL_Texture *TXR_PLAYB_PRESSED = NULL;
SDL_Texture *TXR_PLAYB_REST = NULL;
SDL_Texture *TXR_CLEARB_ALL = NULL;

struct menuButton PLAY = {0, {FIELD_W+BTN_MARGIN, BTN_MARGIN, BTN_SIZE_ST, BTN_SIZE_ST}};
struct menuButton CLEAR = {0, {FIELD_W+BTN_MARGIN*2+BTN_SIZE_ST, BTN_MARGIN, BTN_SIZE_ST, BTN_SIZE_ST}};



struct inputTextBox FPS_ITEXTBOX = {0, {FIELD_W+BTN_MARGIN*2+BTN_SIZE_ST*4, BTN_MARGIN*2 + BTN_SIZE_ST, BTN_SIZE_ST, BTN_SIZE_ST/2},
                               COLOR_INPUT_BACK, COLOR_INPUT_BORDER};


struct inputTextBox CURR_AUTOMATON = {0, {FIELD_W+BTN_MARGIN*2, BTN_MARGIN*2 + BTN_SIZE_ST, BTN_SIZE_ST, BTN_SIZE_ST/2},
                                      COLOR_INPUT_BACK, COLOR_INPUT_BORDER};
int currAutomaton = 0;

void update();
int getAliveNeighbours( int i,  int j);

int getMaxColorNeighbours(int i, int j);


int onCellClicked(int x, int y);
void onButtonClicked(struct menuButton *button);

void render(SDL_Renderer *renderer);
void renderField(SDL_Renderer* renderer);
void renderMenu(SDL_Renderer* renderer);
extern void renderButton(SDL_Renderer* renderer, struct menuButton* BUTTON);

void update(){
struct cell FIELD_NEW[FIELD_SIZE][FIELD_SIZE];
    for(int i = 0; i < FIELD_SIZE; ++i){
    for(int j = 0; j < FIELD_SIZE; ++j){
            int maxColor = getMaxColorNeighbours(i,j);
            int aliveCells = getAliveNeighbours(i,j);
            FIELD_NEW[i][j].state = AUTOMATONS[FIELD[i][j].automaton][FIELD[i][j].state];
            FIELD_NEW[i][j].automaton = getMaxAutomataNeighbours(i,j);
            FIELD_NEW[i][j].color = COLORS[FIELD_NEW[i][j].state];
        }
    }

    for(int i = 0; i < FIELD_SIZE; ++i){
        for(int j = 0; j < FIELD_SIZE; ++j){
            FIELD[i][j]=FIELD_NEW[i][j];
        }
    }
}



void updateFPS(){
    FPS = atoi(&FPS_ITEXTBOX.TEXT);
}

void updateAutomaton(){
    currAutomaton = atoi(&CURR_AUTOMATON.TEXT);
}


char isNumber(char * a){
    int i = 0;
    while(a[i] !=0){
        if(a[i] <= '0' || a[i] >= '9') return 0;
        i++;
    }
    return 1;
}


void getCellFieldPos(int x, int y, int* i, int*j){
    *i = (y/(int)(CELL_SIZE));
    *j = (x/(int)(CELL_SIZE));
}
int onCellClicked(int x, int y){
    int i = (y/(int)(CELL_SIZE));
    int j = (x/(int)(CELL_SIZE));
    if(FIELD[i][j].state == 0) {FIELD[i][j].state = 1; FIELD[i][j].color = COLORS[1]; FIELD[i][j].automaton = currAutomaton; }
    else {FIELD[i][j].state = 0;FIELD[i][j].color = COLORS[0]; FIELD[i][j].automaton = 0;}
    return FIELD[i][j].state;
}




void clearField(){
    for(int i = 0; i < FIELD_SIZE; ++i){
        for(int j  = 0; j < FIELD_SIZE; ++j){
            FIELD[i][j].state = 0;
            FIELD[i][j].automaton = 0;
            FIELD[i][j].color = COLORS[0];
        }
    }
}
int getAliveNeighbours( int i,  int j){
        int up = (i-1) + (i==0) * FIELD_SIZE;
        int down = (i+1) - (i==FIELD_SIZE-1)*FIELD_SIZE;
        int left = (j-1) + (j==0) * FIELD_SIZE;
        int right = (j+1) - (j==FIELD_SIZE-1) * FIELD_SIZE;
        int count = (FIELD[up][j].state!=0) +
                    (FIELD[down][j].state!=0) +
                    (FIELD[i][left].state!=0) +
                    (FIELD[i][right].state!=0) +
                    (FIELD[up][left].state!=0) +
                    (FIELD[up][right].state!=0) +
                    (FIELD[down][left].state!=0) +
                    (FIELD[down][right].state!=0);

        return count;
}

int getMaxAutomataNeighbours(int i, int j){
    int up = (i-1) + (i==0) * FIELD_SIZE;
    int down = (i+1) - (i==FIELD_SIZE-1)*FIELD_SIZE;
    int left = (j-1) + (j==0) * FIELD_SIZE;
    int right = (j+1) - (j==FIELD_SIZE-1) * FIELD_SIZE;
    int N[3];
    int MAX = 0, CMAX = 0;
    int count[AUTOMATONS_COUNT];
    for(int k = 0; k < AUTOMATONS_COUNT;++k){
        int count = (FIELD[up][j].automaton==k) +
                     (FIELD[down][j].automaton==k) +
                     (FIELD[i][left].automaton==k) +
                     (FIELD[i][right].automaton==k) +
                     (FIELD[up][left].automaton==k) +
                     (FIELD[up][right].automaton==k) +
                     (FIELD[down][left].automaton==k) +
                     (FIELD[down][right].automaton==k);
        if(count >= CMAX){
            CMAX = count;
            MAX = k;
        }

    }

    return MAX;

}
int getMaxColorNeighbours( int i,  int j){
    int up = (i-1) + (i==0) * FIELD_SIZE;
    int down = (i+1) - (i==FIELD_SIZE-1)*FIELD_SIZE;
    int left = (j-1) + (j==0) * FIELD_SIZE;
    int right = (j+1) - (j==FIELD_SIZE-1) * FIELD_SIZE;
    int N[3];

    int count0 = (FIELD[up][j].state==0) +
                (FIELD[down][j].state==0) +
                (FIELD[i][left].state==0) +
                (FIELD[i][right].state==0) +
                (FIELD[up][left].state==0) +
                (FIELD[up][right].state==0) +
                (FIELD[down][left].state==0) +
                (FIELD[down][right].state==0);
    int count1 = (FIELD[up][j].state==1) +
                 (FIELD[down][j].state==1) +
                 (FIELD[i][left].state==1) +
                 (FIELD[i][right].state==1) +
                 (FIELD[up][left].state==1) +
                 (FIELD[up][right].state==1) +
                 (FIELD[down][left].state==1) +
                 (FIELD[down][right].state==1);
    int count2 = (FIELD[up][j].state==2) +
                 (FIELD[down][j].state==2) +
                 (FIELD[i][left].state==2) +
                 (FIELD[i][right].state==2) +
                 (FIELD[up][left].state==2) +
                 (FIELD[up][right].state==2) +
                 (FIELD[down][left].state==2) +
                 (FIELD[down][right].state==2);

    return count0 > count1 ? (count0 > count2 ? 0:2):(count1 > count2 ? 1:2);
}


void render(SDL_Renderer* renderer){
    renderField(renderer);
    renderMenu(renderer);
    SDL_RenderPresent(renderer);
}



void renderField(SDL_Renderer* renderer){
    SDL_Rect CELL = {0,0, CELL_SIZE, CELL_SIZE};
    for(int i = 0; i < FIELD_SIZE; ++i){
        for(int j = 0; j < FIELD_SIZE; ++j){
            CELL.x = j*CELL_SIZE;
            CELL.y = i*CELL_SIZE;
            int color = FIELD[i][j].color;
            SDL_SetRenderDrawColor(renderer, color>>24, color>>16, color>>8, color);
            SDL_RenderFillRect(renderer, &CELL);
            if(SIM_RUNNING == 0){
                SDL_SetRenderDrawColor(renderer, COLOR_F_OUTLINE>>24, COLOR_F_OUTLINE>>16, COLOR_F_OUTLINE>>8, COLOR_F_OUTLINE);
                SDL_RenderDrawRect(renderer, &CELL);
            }

        }
    }
}
void renderMenu(SDL_Renderer* renderer){

    SDL_Rect MENU_BACK = {FIELD_W, 0, WINDOW_W-FIELD_W, WINDOW_H};

    int err = SDL_SetRenderDrawColor(renderer, COLOR_MENU_BACK>>24, COLOR_MENU_BACK>>16, COLOR_MENU_BACK>>8, COLOR_MENU_BACK);
    SDL_RenderFillRect(renderer, &MENU_BACK);
    renderButton(renderer, &PLAY);
    renderButton(renderer, &CLEAR);
    renderInputTextBox(renderer, &FPS_ITEXTBOX);
    renderInputTextBox(renderer, &CURR_AUTOMATON);

}

int initTextures(SDL_Renderer * renderer){
    SDL_Surface* image = SDL_LoadBMP("D:/Projects/C/SDL_Tutorials/SDL_1/SDL_1/TXR_PLAY_REST.bmp");
    if(image == NULL){
        printf("Error image REST\n");
        printf("%s\n", SDL_GetError());
        return -1;
    }

    TXR_PLAYB_REST = SDL_CreateTextureFromSurface( renderer, image );

    SDL_FreeSurface( image );

    if( TXR_PLAYB_REST == NULL) {
        printf("Error TXR REST\n");
        printf("%s\n", SDL_GetError());
        return -1;
    }

    image = SDL_LoadBMP("D:/Projects/C/SDL_Tutorials/SDL_1/SDL_1/TXR_PLAY_PRESSED.bmp");

    if(image == NULL){
        printf("Error image PRESSED\n");
        printf("%s\n", SDL_GetError());
        return -1;
    }

    TXR_PLAYB_PRESSED = SDL_CreateTextureFromSurface(renderer, image);


    if( TXR_PLAYB_PRESSED == NULL) {
        printf("Error TXR REST\n");
        printf("%s\n", SDL_GetError());
        return -1;
    }

    image = SDL_LoadBMP("D:/Projects/C/SDL_Tutorials/SDL_1/SDL_1/TXT_CLEAR_ICO.bmp");

    if(image == NULL){
        printf("Error image CLEAR\n");
        printf("%s\n", SDL_GetError());
        return -1;
    }

    TXR_CLEARB_ALL = SDL_CreateTextureFromSurface( renderer, image );

    SDL_FreeSurface(image);
    image = NULL;

    PLAY.TEXTURE_REST = TXR_PLAYB_REST;
    PLAY.TEXTURE_PRESSED = TXR_PLAYB_PRESSED;

    CLEAR.TEXTURE_PRESSED = TXR_CLEARB_ALL;
    CLEAR.TEXTURE_REST = TXR_CLEARB_ALL;

    return 0;
}

int main(int argv, char** args)
{


    if(SDL_Init(SDL_INIT_VIDEO)<0){
        printf("Error SDL Init\n");
        return 1;
    }
    TTF_Init();


    window = SDL_CreateWindow("Rule Dynamic Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);

    if(window == NULL){
        printf("Error Window Init");
        return 1;
    }

    APP_RUNNING = 1, SIM_RUNNING = 0;



    SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderClear( renderer );
    clearField();
    //SDL_SetTextInputRect(&INPUT_FIELD.POSRECT);


    FPS_ITEXTBOX.MULTITEXT = 1;
    FPS_ITEXTBOX.MAX_SYM = 3;

    CURR_AUTOMATON.MULTITEXT = 1;
    CURR_AUTOMATON.MAX_SYM = 3;



    if(initTextures(renderer) != 0 ){
        printf("Error init Textures");
        goto label_end;
    }

    int x = 0, y = 0;
    int i_prev = -1,j_prev = -1;
    char lastAction = -1;
    int32_t S1 = SDL_GetTicks();
    int32_t S1_MOUSE = SDL_GetTicks();
    while(APP_RUNNING){
        SDL_Event e;
        if(!SIM_RUNNING){
            uint32_t MOUSE_STATE = SDL_GetMouseState(&x, &y);

            if(SDL_BUTTON(MOUSE_STATE) == SDL_BUTTON_LMASK){
                if(x < FIELD_W && x > 0 && y < FIELD_H && y > 0){
                    //Мы нажали левую кнопку мыши
                    //Если она была нажата, то надо отследить, что
                    //состояние клетки не равно состоянию предыдущей
                    int i = 0, j = 0;
                    getCellFieldPos(x,y,&i,&j);
                    int32_t S2 = SDL_GetTicks();
                    int32_t MS = 1000/60;
                    if(S2 - S1_MOUSE > MS){

                        if(i_prev == -1){
                           onCellClicked(x,y);
                           i_prev = i, j_prev = j;
                        }

                        else{
                            if(FIELD[i][j].state != FIELD[i_prev][j_prev].state) {
                                onCellClicked(x,y);
                                i_prev = i, j_prev = j;
                            }

                        }


                        S1_MOUSE = S2;

                    }
                }
            }
            else{
                i_prev = -1, j_prev = -1;
            }
        }

        while(SDL_PollEvent(&e)!=0){
            switch(e.type){
            case SDL_QUIT:{
                APP_RUNNING = 0;
                goto label_end;
                break;
            }

            case SDL_KEYDOWN:{
                if(e.key.keysym.sym == SDLK_SPACE){
                    if(!FPS_ITEXTBOX.IS_FOCUSED && !CURR_AUTOMATON.IS_FOCUSED){
                        PLAY.STATE = !PLAY.STATE, SIM_RUNNING = !SIM_RUNNING;
                    }

                }

                if(e.key.keysym.sym == SDLK_BACKSPACE){
                    if(SIM_RUNNING == 0){
                         if(FPS_ITEXTBOX.IS_FOCUSED){
                            char *p = FPS_ITEXTBOX.TEXT;
                            p[strlen(p)-1] = 0;
                        }

                         else if (CURR_AUTOMATON.IS_FOCUSED){
                             char *p = CURR_AUTOMATON.TEXT;
                             p[strlen(p)-1] = 0;
                         }
                    }
                }
                break;
            }


            case SDL_MOUSEBUTTONDOWN:{
                x = e.motion.x;
                y = e.motion.y;

                //if(x < FIELD_W && !SIM_RUNNING) onCellClicked(x,y);

                if(pointInButton(&PLAY,x,y)){
                        PLAY.STATE = !PLAY.STATE, SIM_RUNNING = !SIM_RUNNING;
                    }

                if(pointInButton(&CLEAR, x, y)){
                        SIM_RUNNING = 0;
                        PLAY.STATE = 0;
                        clearField();
                    }

                if(SIM_RUNNING == 0){
                        if (pointInInputTextBox(&FPS_ITEXTBOX, x,y)){
                            FPS_ITEXTBOX.IS_FOCUSED = 1;
                            CURR_AUTOMATON.IS_FOCUSED = 0;
                            SDL_StopTextInput();
                            SDL_SetTextInputRect(&FPS_ITEXTBOX.POSRECT);
                            SDL_StartTextInput();
                        }
                        else if(pointInInputTextBox(&CURR_AUTOMATON, x,y)){
                            FPS_ITEXTBOX.IS_FOCUSED = 0;
                            CURR_AUTOMATON.IS_FOCUSED = 1;
                            SDL_StopTextInput();
                            SDL_SetTextInputRect(&CURR_AUTOMATON.POSRECT);
                            SDL_StartTextInput();
                        }
                        else{
                            FPS_ITEXTBOX.IS_FOCUSED = 0;
                            CURR_AUTOMATON.IS_FOCUSED = 0;
                            SDL_StopTextInput();
                        }
                    }
                else{
                    FPS_ITEXTBOX.IS_FOCUSED = 0;
                    CURR_AUTOMATON.IS_FOCUSED = 0;
                    SDL_StopTextInput();
                }


                break;
            }

            case SDL_TEXTINPUT:{

                if(SIM_RUNNING == 0){

                    if(FPS_ITEXTBOX.IS_FOCUSED){

                        if(strlen(&FPS_ITEXTBOX.TEXT) < 3) {
                        strcat(FPS_ITEXTBOX.TEXT, e.text.text);
                        updateFPS();
                        }
                    }

                    else if(CURR_AUTOMATON.IS_FOCUSED){

                        if(strlen(&CURR_AUTOMATON.TEXT) < 3) {
                            strcat(CURR_AUTOMATON.TEXT, e.text.text);
                            updateAutomaton();
                        }
                    }
                }


                break;
            }


            default:
                break;
            }

        }


        if(SIM_RUNNING){

            FPS_ITEXTBOX.IS_FOCUSED = 0;
            CURR_AUTOMATON.IS_FOCUSED = 0;
        }


        //rendering cycle

        renderMenu(renderer);

        if(SIM_RUNNING){
            int32_t S2 = SDL_GetTicks();
            if(FPS>0){
                int32_t MS = 1000/FPS;
                if(S2 - S1 > MS){
                    S1 = SDL_GetTicks();
                    update();
                }
            }

        }
        else{
            S1 = SDL_GetTicks();
        }
        renderField(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60);
        }

    label_end:
    SDL_StopTextInput();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}


