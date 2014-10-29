/*
    Daniela B. Formiga
    João Pedro Lopes
    Luiz C. Carvalho
*/


#include <SDL/SDL.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <SDL/SDL_gfxPrimitives.h>
using namespace std;


#define PI 3.14159265
#define GRAVIDADE 9.8


// declaracao de variaveis globais
SDL_Surface * screen;


void saida(int *bateu, int *acertou, int *cont, double *X, double *maximo, int *passou){

    int num = *cont+1;


    string alvo="não acertou";
    string obstaculo;

    if (*bateu==0 && *passou==0){
        obstaculo="ultrapassou";
        if (*acertou==0)
            alvo="acertou";
        }
    else if(*bateu==0 && *passou==1)
        obstaculo="não ultrapassou";

    else if (*bateu==1)
        obstaculo="atingiu";

    //Imprime no arquivo de saída os resultados de cada lançamento

    cout << "=========================================================================================" << endl;
    cout << "O projétil " << num << " " << obstaculo << " o obstáculo \ne " << alvo << " o alvo..."<<endl ;
    cout << "Sua altura máxima foi " << *maximo << " metros \ne seu alcance foi de " << *X <<" metros"<< endl;
    cout << "=========================================================================================" << endl;

}
double calculaDistanciaPontos(double x1, double y1, double x2, double y2){
    double cat1, cat2, hip, d=0.0;

    cat1 = x1 - x2;
    if ( cat1 < 0 ){
        cat1 = cat1*-1;
    }

    cat2 = y1 - y2;
    if ( cat2 < 0 ){
        cat2 = cat2*-1;
    }


    hip = cat1*cat1 + cat2*cat2;
    d = sqrt ( hip );

    if (d < 0){
        d = d * -1;
    }

    return d;
}
void colisoes(double *jan_A, int *alvo_raio, int *alvo_X, int *pos_alvo, double *X, double *Y,
              double *obs_P, double *obs_A, double *obs_L, int *bateu, int *acertou, int *passou){
    double distancia;

    distancia = calculaDistanciaPontos((double) *alvo_X, (double) *pos_alvo, *X, *Y);
    if(*X>= *obs_P && *X<= *obs_P + *obs_L && *Y >=  *jan_A - *obs_A && *bateu == 0)
        *bateu = 1; // Significa que bateu

    if (distancia <= *alvo_raio)
        *acertou=0; // significa que acertou

    if (*X>*obs_P+*obs_L)
        *passou=0; //significa que passou do obstáculo

    //para passar do alvo e acertar, todos precisam ser ZERO

}

//Função que executa os cálculos físicos
void fisica(double *tiro_angulo, double *tiro_velocidade, float *tiro_X, float *tiro_Y,
            double *tempo, double *gravidade, double *maximo, double *tempo_total){


        *tiro_angulo = *tiro_angulo * PI/180.0;// transforma o angulo em radianos
       double sen_tiro = sin(*tiro_angulo); //seno do tiro convertido para radianos
       double cos_tiro = cos(*tiro_angulo); //cosseno do tiro convertido para radianos

        *tiro_X = cos_tiro * (*tiro_velocidade * *tempo);
        *tiro_Y = ((sen_tiro * (*tiro_velocidade * *tempo)) - ((pow(*tempo,2.0) * *gravidade))/2);

        double tempo_subida = *tiro_velocidade * sen_tiro/ *gravidade;
        *maximo = ((*tiro_velocidade * sen_tiro) * tempo_subida)-((pow(tempo_subida,2.0)) * *gravidade)/2.0;
        *tempo_total = 2* tempo_subida;

}

int leitura(double *janela_largura, double *janela_altura, double *obstaculo_altura, double *obstaculo_largura,
            double *obstaculo_pos, int *alvo_X, int *alvo_Y, int *alvo_raio, int *ponto_inicial,
            double *tiro_angulo, double *tiro_velocidade, int *cont){

    char ignore[20];// recebe as palavras "janela, ponto_inicial, obstáculo, tiro... que não são utilizadas pelo programa

    ifstream in ("entrada.txt"); // Indica que arquivo esta sendo usado como entraa (ifstream == in fstream)

    if (!in)// Caso não exista o arquivo para leitura
    {
        cout << "Arquivo nao existe";
        return 1;
    }

    //Lê o tamanho da janela
    in >> ignore;
    in >> *janela_largura;
    in >> *janela_altura;

    //Lê o ponto de partida do tiro
    in >> ignore;
    in >> *ponto_inicial;

    //Lê o tamanho e posição do obstáculo
    in >> ignore;
    in >> *obstaculo_pos;
    in >> *obstaculo_altura;
    in >> *obstaculo_largura;

    //Lê o tamanho e posição do alvo
    in >> ignore;
    in >> *alvo_X;
    in >> *alvo_Y;
    in >> *alvo_raio;


    for(int var=-1; var<*cont; var++){// Lê todos  os tiros e armazena o tiro que está sendo utilizado
        in >> ignore;
        in >> *tiro_angulo;
        in >> *tiro_velocidade;
    }


}

void fisicaParabola(double *x, double *y){
    double a0 = 5.00649350649352;
    double a1 =	0.428787878787942;
    double a2 =	-1.56534090909095;
    double a3 =	0.438210227272737;
    double a5 =	0.0014322916666667;
    double a4 =	-0.0433238636363646;
    if(*x<=12){
        *x = *x + 0.1;
    }
    *y = a0 + a1* *x + a2*pow(*x,2) + a3*pow(*x,3) + a4*pow(*x,4) + a5*pow(*x,5);

}

int main(int arg, char **args){

    int jogoAtivo = 1;
    int velocidade = 5;
    int cont=0; // esse contador serve para saber quais tiros já foram lidos na função "leitura"

    //variáveis lidas no arquivo txt (Externas)
    double jan_L, jan_A; //tamanho da janela
    double obs_A, obs_L, obs_P; // altura larguta e posição do obstáculo
    int alvo_X, alvo_Y, alvo_raio; // posição e tamanho do alvo
    int ponto_I; // ponto de onde sai o tiro
    double tiro_A, tiro_V; // angulo e velocidade do tiro
    float tiro_X, tiro_Y; // pontos por onde o tiro irá passar

    //variáveis criadas fora do arquivo txt (internas)
    double tempo=0; // utilizado para calcular as posições dos pontos
    double tempo_total;
    double gravidade = (9.8);
    double maximo;// ajuda a definir quando o projetil entrou em queda
    int aux=0; // Para saber se o trajeto do tiro já terminou
    int bateu=0, acertou=1, passou=1; // utilizados para saber se acertou o alvo e/ou se bateu no obstáculo
    double xInicial = 0.3;
    double yInicial = 0;

    //chama a função que lê o arquivo
    leitura (&jan_L, &jan_A, &obs_A, &obs_L, &obs_P, &alvo_X, &alvo_Y, &alvo_raio, &ponto_I,
             &tiro_A, &tiro_V, &cont);


    // PREPARAR AMBIENTE
    // inicializar a SDL

    //erro na SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout<<"Nao foi possível inicializar a SDL:"<<SDL_GetError()<<endl;
        return 1;
    }

    // criar janela principal
    screen = SDL_SetVideoMode(jan_L, jan_A, 32, SDL_SWSURFACE);

    //erro na configuração de vídeo
    if(screen == NULL){
        cout<< "SDL_SetVideoMode: "<<SDL_GetError()<<endl;
        return 2;
    }

   //Para poder esperar a tecla na troca de tiro
    SDL_KeyboardEvent event;

    int garantia = 0;
    // GAME LOOP
    while( jogoAtivo == 1){


        //chama a função que lê o arquivo
        leitura (&jan_L, &jan_A, &obs_A, &obs_L, &obs_P, &alvo_X, &alvo_Y,
                 &alvo_raio, &ponto_I, &tiro_A, &tiro_V, &cont);

        if (tiro_A==0 && tiro_V==0)//Finaliza o programa quando o ângulo e a velocidade forem zero
            break;

        //Chama a função que calcula o ponto do tiro
        fisica(&tiro_A, &tiro_V, &tiro_X, &tiro_Y,
               &tempo, &gravidade, &maximo, &tempo_total);


        // desenha Obstaculo na tela
        int pos_obs = jan_A-obs_A;
        //SDL_Rect obstaculo={obs_P, pos_obs, obs_L, obs_A};// Define a posição e tamanho do obstáculo
        //SDL_FillRect(screen, &obstaculo, 0xFF0000);


        // desenha Alvo na tela
        int pos_alvo = jan_A-alvo_Y;
        //filledCircleColor(screen, alvo_X, pos_alvo, alvo_raio, 0xFFFFFFFF);
        //filledCircleColor(screen, alvo_X, pos_alvo, alvo_raio/4*3, 0xFF0000FF);
        //filledCircleColor(screen, alvo_X, pos_alvo, alvo_raio/2, 0xFFFFFFFF);
        //filledCircleColor(screen, alvo_X, pos_alvo, alvo_raio/4, 0xFFF0000FF);



        // desenha o tiro
        fisicaParabola(&xInicial, &yInicial);


        yInicial *= 50;
        xInicial *= 50;

        double X, Y;
        if(garantia){
            X=tiro_X+ponto_I;
            Y=(jan_A-tiro_Y);
            yInicial = jan_A - yInicial;
        }
        else {
            Y = jan_A; // coloca o Y do CENTRO do tiro no mesmo nível do Y do obstáculo, no caso o "chão" da janela
            yInicial = jan_A;
            X = ponto_I; // coloxa o X do CENTRO do tiro no ponto inicial de lançamento
            garantia++; // garante que o ponto inicial só será executado na primeira vez
        }

        //Desenha o ponto onde está o tiro

        filledCircleColor(screen, xInicial, yInicial, 2, 0xFF0000FF);
        yInicial /= 50;
        xInicial /= 50;

        // Atualiza a tela
        SDL_Flip(screen);

        // delay para a janela ficar aparecendo
        SDL_Delay(15);// 100 milisegundos = 10 fps

        //A partir daqui, o programa checa se o tiro atingiu o obstáculo, atingiu o alvo ou errou o alvo

        colisoes(&jan_A, &alvo_raio, &alvo_X, &pos_alvo, &X, &Y, &obs_P, &obs_A, &obs_L, &bateu, &acertou, &passou);

        if (bateu==1)
            aux+=2;

        if (acertou==0)
            aux+=2;

        if (X>jan_L ||X<0)
            aux+=2;

        if(Y<=alvo_Y && aux == 0)
            aux++;

        if(Y>=alvo_Y && aux == 1)
            aux++;

        if(Y>jan_A || X>jan_L)
            aux+=2;


        if (aux>1){
            saida(&bateu, &acertou, &cont, &X, &maximo, &passou);
            //SDL_FillRect(screen, NULL, 0x000000);

            //Reinicializando as variáveis
            cont++;
            aux=0;
            bateu=0;
            acertou=1;
            garantia=0;
            tempo = 0;
            passou=1;
        }

        else{
            tempo+= 0.1;
        }

    }


    //FINALIZAR AMBIENTE
    // finaliza SDL libera os recursos alocados
    SDL_Quit();
    return 0;
}
