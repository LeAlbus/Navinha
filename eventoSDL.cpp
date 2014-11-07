/*
    Daniela B. Formiga
    João Pedro Lopes
    Luiz C. Carvalho

    Trabalho de PI
    TODO: Escrever descricao
*/


#include <SDL/SDL.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_image.h>
using namespace std;




// declaracao de variaveis globais
SDL_Surface * screen;



using namespace std;

int main(int arg, char **args){

    int jogoAtivo = 1;





    // PREPARAR AMBIENTE
    // inicializar a SDL

    //erro na SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout<<"Nao foi possível inicializar a SDL:"<<SDL_GetError()<<endl;
        return 1;
    }

    // criar janela principal
    screen = SDL_SetVideoMode(600, 800, 32, SDL_SWSURFACE);

    //erro na configuração de vídeo
    if(screen == NULL){
        cout<< "SDL_SetVideoMode: "<<SDL_GetError()<<endl;
        return 2;
    }

   //Para poder esperar a tecla na troca de tiro
    SDL_KeyboardEvent event;

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        //success = false;
    }

    int garantia = 0;
    SDL_Surface *image;
    image=IMG_Load("images/FundoMenu.png");
    if(!image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
    }
    // GAME LOOP
    while( jogoAtivo == 1){



        //SDL_Rect obstaculo={obs_P, pos_obs, obs_L, obs_A};// Define a posição e tamanho do obstáculo
        //SDL_FillRect(screen, &obstaculo, 0xFF0000);



        // Atualiza a tela
        //SDL_Flip(screen);

        // delay para a janela ficar aparecendo
        SDL_Delay(15);// 100 milisegundos = 10 fps

        //A partir daqui, o programa checa se o tiro atingiu o obstáculo, atingiu o alvo ou errou o alvo





    }


    //FINALIZAR AMBIENTE
    // finaliza SDL libera os recursos alocados
    SDL_Quit();
    return 0;
}
