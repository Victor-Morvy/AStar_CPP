#include "PIG.h"
#include <math.h>
#include <tgmath.h>

#define BASE60FPS 0.01666666666666666

#define TAMXCAMPO 10
#define TAMYCAMPO 10



class Vec2D
{
public:
    int x, y;

    Vec2D(int vx, int vy) : x(vx), y(vy)
    {}

};


enum nodeConfig{node, isA, isB};

class PathCell
{
private:


public:

    PathCell(bool isBlocked, int posx, int posy, int type = node, bool visited = false) : isblocked(isBlocked), pos(Vec2D(posx, posy)), whatAmI(type), isVisited(visited)
    {
        //std::cout << "Criado: isBlocked: " << isBlocked << " | posx " << posx << " | posy " << posy
        //                                   << " | type: " << type << " | visited: " << visited << endl;

        parent = nullptr;
        parentId = -1;
    }

    int whatAmI;

    Vec2D pos;

    int G = 0; // distance from starting node
    int H = 0; //distance from end node
    int F = 0; //G+H

    void somaPeso()
    {
        F = G + H;
    }

    bool isblocked;

    bool isVisited;

    int parentId;
    PathCell* parent;

};

enum tipo{walkable, blocked, aSpawn, bSpawn};

/*
int campo[TAMYCAMPO][TAMXCAMPO] =   {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
                                    };
                                    */

int campo[TAMYCAMPO][TAMXCAMPO] =   {{0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
                                     {0, 0, 0, 0, 0, 1, 1, 1, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 2, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
                                    };

void desenhaCampo()
{
    int nowY = 800;
    int tam = 64;

    for(int iy = 0; iy < TAMYCAMPO ; iy++)
    {
        for(int ix = 0; ix < TAMYCAMPO; ix++)
        {

            int campoVal = campo[iy][ix];

            PIG_Cor cor;

            if(campoVal == walkable)
                cor = BRANCO;
            else if(campoVal == blocked)
                cor = {10, 9, 9, 255};
            else
                cor = AZUL;

            DesenhaRetangulo(ix*tam + 32, nowY, tam-5, tam-5, cor, 0);

        }
        nowY -= tam;
    }

}



std::vector<PathCell*> pathCellVector;

bool checkNodeExists(int nodeX, int nodeY)
{
    //std::cout << "size pathCellVector " << pathCellVector.size() << std::endl;
    for(auto n : pathCellVector)
    {
        if(n->pos.x == nodeX && n->pos.y == nodeY)
        return true; //if exists
    }

   return false; //if not exists
}

PathCell* getSpawn(char value) //'a' spawn A, 'b' spawnB
{
    PathCell* p = nullptr;
    int i = 0;

    for(auto n : pathCellVector)
   {
       if(value == 'a')
        {
            if (n->whatAmI == aSpawn)
            {
                p = pathCellVector.at(i);
                return p;
            }
        }
        else if(value == 'b')
        {
           if (n->whatAmI == bSpawn)
            {
                p = pathCellVector.at(i);
                return p;
            }
        }
        i++;
   }

   return p;


}

PathCell* getPathCelByPos(int vx, int vy)
{
    PathCell* p = nullptr;

    for(auto n : pathCellVector)
    {
        if(n->pos.x == vx && n->pos.y == vy)
            p = n;
    }

    return p;
}

int distanciaEntre(PathCell A, PathCell B); // declarando antes de usar
int createNeighborNode(PathCell* theNode)
{
    theNode->isVisited = true;

    int valToReturn = 15;
    //std::cout << "kkkkk" << valToReturn << std::endl;
    for(int iy = -1; iy <=1; iy++)//y
    {
        for(int ix = -1; ix <=1; ix++)//x
        {
            //std::cout << "zzzz " << checkNodeExists(ix + theNode->pos.x, iy + theNode->pos.y);
            // && n.pos.y >= 0 && n.pos.x >= 0 && n.pos.x
            int nowX = ix + theNode->pos.x;
            int nowY = iy + theNode->pos.y;

            if(campo[nowY][nowX] == 0 && nowX >= 0 && nowX < TAMXCAMPO && nowY >= 0 && nowY < TAMYCAMPO)
            {
                int newPathCellIndex = pathCellVector.size()-1;

                PathCell* pa = getSpawn('a');
                if (pa == nullptr)
                    return;

                PathCell* pb = getSpawn('b');
                if (pb == nullptr)
                    return;

                PathCell* pathp = getPathCelByPos(nowX, nowY);
                if(checkNodeExists(nowX, nowY) && !pathp->isVisited && !pathp->isblocked && pathp->whatAmI == walkable)
                {

                    PathCell* pNow = getPathCelByPos(nowX, nowY);

                    int distNowToB = distanciaEntre(*pathp, *pb);


                    int nowG = theNode->G + distanciaEntre(*theNode, *pNow);
                    int nowH = distNowToB;
                    int nowF = nowG + nowH;

                    if(nowF < pNow->F)
                    {
                        pNow->G = nowG;
                        pNow->H = nowH;
                        pNow->F = nowF;

                        pNow->parent = theNode;
                    }


                    /*

                    int distNowToB = distanciaEntre(*pathp, *pb);
                    valToReturn = distNowToB;

                    //pathCellVector[newPathCellIndex].parentId

                    pathp->parent = theNode;
                    std::cout << "Pos TheNode x: " << pathp->parent->pos.x << ", y: " << pathp->parent->pos.y << std::endl;

                    pathp->G = distanciaEntre(*pathp, *pa);
                    pathp->H = distNowToB;
                    pathp->somaPeso();
                    //pathCellVector[newPathCellIndex]->parent = theNode;
                    */
                }
                else if(!checkNodeExists(nowX, nowY))
                {

                    //std::cout << "bbbb\n";
                    pathCellVector.push_back(new PathCell(false, ix+ theNode->pos.x, iy + theNode->pos.y, walkable));
                    //set distance values
                    newPathCellIndex = pathCellVector.size()-1;

                    PathCell* pa = getSpawn('a');
                    if (pa == nullptr)
                        return;

                    PathCell* pb = getSpawn('b');
                    if (pb == nullptr)
                        return;

                    int distNowToB = distanciaEntre(*pathCellVector.at(newPathCellIndex), *pb);
                    valToReturn = distNowToB;

                    //pathCellVector[newPathCellIndex].parentId

                    pathCellVector[newPathCellIndex]->parent = theNode;
                    //std::cout << "Pos TheNode x: " << pathCellVector[newPathCellIndex]->parent->pos.x << ", y: " << pathCellVector[newPathCellIndex]->parent->pos.y << std::endl;

                    pathCellVector[newPathCellIndex]->G = theNode->G + distanciaEntre(*pathCellVector.at(newPathCellIndex), *theNode);
                    pathCellVector[newPathCellIndex]->H = distNowToB;
                    pathCellVector[newPathCellIndex]->somaPeso();
                    //std::cout << "Peso: " << pathCellVector[newPathCellIndex]->F << " | A: x:" << pa->pos.x << " y:" << pa->pos.y << " dist: " << pathCellVector[newPathCellIndex]->G <<
                    //                         " | B: x:" << pb->pos.x << " y:" << pb->pos.y << " dist: " << pathCellVector[newPathCellIndex]->H << std::endl;

                    if(pathCellVector[newPathCellIndex]->F <= 14)
                    {
                        //std::cout << "Passou! Achou!\n";
                        theNode->isVisited = true;
                        return 1;//achou!

                    }

                    ////std::cout << "LLLL" << distNowToB << std::endl;
                    ////std::cout << "kkkkkk " << checkNodeExists(ix + theNode->pos.x, iy + theNode->pos.y) << std::endl;

                }
            }




        }
        theNode->isVisited = true;
    }
    return 0;

}

int distanciaEntre(PathCell A, PathCell B) //find path a to b
{
    //std::cout << "fffs2s2s2s2s2s2s2s2s2s2ff\n";

    //valor final da somat�ria
    int somatoria;

    //custo para path diagonal e n�o diagonal
    int custoDiagonal = 14;
    int custoNotDiagonal = 10;

    //quantidade de paths visitados
    int ndiagonal=0;
    int nNotDiagonal=0;

    //posicao atual da visita
    int nowX, nowY;

    //posicao do objetivo
    float posBX = B.pos.x;
    float posBY = B.pos.y;

    //declarando posical inicial
    nowX = A.pos.x;
    nowY = A.pos.y;


    //calculating the path distance
    while(nowX != posBX && nowY != posBY)
    {
        int difX = posBX - nowX;
        int difY = posBY - nowY;

        //std::cout << nowX << ", " << nowY << " | " << posBX << ", " << posBY << std::endl;

        //if(difX != 0 && difY != 0)

        if (difX != 0 || difY != 0)
        {
            if(difX != 0 && difY != 0)
            {
               ndiagonal++;
            }
            else
            {
                nNotDiagonal++;
            }

            int aX = abs(difX);
            int aY = abs(difY);
            nowX += (difX/aX);
            nowY += (difY/aY);
            //std::cout << "Now x: " << nowX << ", y: " << nowY << std::endl <<
            //             "B x: " << posBX << ", y: " << posBY << std::endl;

        }
        else
        {
            //std::cout << "teste\n";
        }



    }

    int difX = abs(posBX - nowX);
    int difY = abs(posBY - nowY);

    nNotDiagonal += difX + difY;

    somatoria = (ndiagonal * custoDiagonal) + (nNotDiagonal * custoNotDiagonal);

    //std::cout << "DIAGONAL: " << ndiagonal << " | NAO DIAGONAL: " << nNotDiagonal << " | x: " << difX << " y: " << difY << std::endl;
    //std::cout << "Somatoria: " << somatoria << "A: x:" << A.pos.x << " y:" << A.pos.y <<
    //             "B: x:" << B.pos.x << " y:" << B.pos.y << std::endl;
    return fabs(somatoria);
}

void iniciaNodeMap()
{
    //std::cout << "dddd\n";
    for(int i = 0; i < TAMYCAMPO; i++)
    {
        for(int j = 0; j < TAMXCAMPO; j++)
        {
            if(campo[i][j] == aSpawn)
            {
                pathCellVector.push_back(new PathCell(false, j, i, aSpawn));
            }
            else if(campo[i][j] == bSpawn)
            {
                pathCellVector.push_back(new PathCell(false, j, i, bSpawn));
            }
            else if(campo[i][j] == blocked)
            {
                pathCellVector.push_back(new PathCell(true, j, i, blocked, true));
            }
        }
    }
}

PathCell* getMenorPeso()
{
    //std::cout << "ccccc\n";
    PathCell* p = nullptr;

    int i = 0;
    for(auto n : pathCellVector)
    {
        if (!n->isVisited && !n->isblocked && n->whatAmI == walkable)
        {
            if(!p)
            {
                p = pathCellVector.at(i);
            }
            else
            {
                if(n->F < p->F )
                {
                    p = pathCellVector.at(i);
                }
                if(n->F == p->F && n->H < p->H)
                    p = pathCellVector.at(i);
            }

        }
        i++;
    }
    //std::cout << "Menor valor achado - peso: " << p->F << " x: " << p->pos.x << " y: " << p->pos.y << " H: " << p->H << std::endl;
    return p;
}

int findPath(PathCell* a, PathCell* b)
{
    //std::cout << "aaaa\n";
    int achou = 0;
    bool run = true;

    PathCell* pa;
    pa = a;

    while(run)
    {


        if(createNeighborNode(pa))
        {
            //std::cout << "esta aqui\n " ;
            run = false;
            achou = 1;

            return 1;
        }
        else
        {
            pa = getMenorPeso();

            if(pa->H <= 14)
            {
                //std::cout << "ASUJDI HASUDH ASUDH AUISDH ASUIDH ASUIDH ASIUD ASD AS" << std::endl;
                run = false;
                achou = 1;

                return 1;
            }

            //std::cout << "posx: " << pa->pos.x << " | posy: " << pa->pos.y << " | visitado " << pa->isVisited << std::endl;
            //if(!pa)
            //    std::cout << "esta nulo\n";

        }

        if (pathCellVector.size() >= (TAMYCAMPO * TAMXCAMPO))
            return 0;


    }
    //std::cout << "ACABOU!\n " ;

}


void printaFounded(PathCell* a);
void desenhaPath()
{
    int nowY = 800;
    int tam = 64;

    PIG_Cor cor = {0,0,0,0};

    for(auto n : pathCellVector)
    {
        int cPosX = n->pos.x;
        int cPosY = n->pos.y;


        if(n->whatAmI == aSpawn || n->whatAmI == bSpawn)
        {
            if(n->whatAmI == aSpawn && n->isVisited)
            {
                //cor = ROXO;
                //std::cout << "Quem e? " << n->whatAmI << std::endl;
            }

            if(n->whatAmI == bSpawn && n->isVisited)
            {
                //cor = LARANJA;
                //std::cout << "Quem e? " << n->whatAmI << std::endl;
            }

        }
        else if (n->whatAmI == walkable)
        {
            if (n->isVisited)
            {
                cor = VERMELHO;
            }
            else
            {
                cor = VERDE;
            }

        }

        DesenhaRetangulo(cPosX*tam + tam/2, nowY - cPosY * tam, tam-5, tam-5, cor, 0);
    }

    //printaFounded
    int pathCellVecSize = pathCellVector.size();


    PathCell* sss = pathCellVector[pathCellVecSize-1];

    //std::cout << "tamanho cel vec size aaa: " << pathCellVecSize << std::endl;

    //cor = AMARELO;

    /*
    for(int i = pathCellVecSize - 1; i >= 0 ; i--)
    {
            //sss = &pathCellVecSize[i];
            if(pathCellVector[i].parent != nullptr && pathCellVector[i].whatAmI == walkable)
            {

                if(sss->parent == pathCellVector[i].parent)
                {
                    DesenhaRetangulo(sss->pos.x*tam + tam/2, nowY - sss->pos.y * tam, tam-5, tam-5, cor, 0);

                    std::cout << sss->pos.x << " " << sss->pos.y << std::endl;

                    sss = sss->parent;
                }
            }
            else
            {
                break;
            }
    }
    */

    printaFounded(sss);
}

void printaFounded(PathCell* a)
{
    int tam = 64;
    PIG_Cor cor;

    if(a->parent != nullptr)
    {
        cor = AMARELO;
        DesenhaRetangulo(a->pos.x*tam + tam/2, 800 - a->pos.y * tam, tam-5, tam-5, cor, 0);

        //std::cout << "TOP x: " << a->pos.x << "y: " << a->pos.y << " | What am i: " << a->whatAmI << std::endl;

        printaFounded(a->parent);
    }

}

int fonte;
int fonte2;
void desenhaEscritas()
{
    int startY = 800;
    int tam = 64;
    Vec2D posG = Vec2D(21, 43); //distance from starting node
    Vec2D posH = Vec2D(43, 43); //distance from ending node
    Vec2D posF = Vec2D(32, 21); //total distance

    char textoEsq[5];
    char textoDir[5];
    char textoCentro[5];



    //char String[100];
    //sprintf(String, "E ai manow, X: %.0f Y: %.0f, FPS: %.0f", posX, posY, fps);

    //EscreverEsquerda(String, 100, 700, VERDE, fonte, 0);

    SDL_Color color = PRETO;
    for(int i = pathCellVector.size()-1; i>=0; i--)
    {
        PathCell* n = pathCellVector.at(i);


        if(!pathCellVector.at(i)->parent)
        {


            break;
        }


        int tamyy = 775;
        //PathCell* n = pathCellVector.at(pathCellVector.size()-11);

        //std::cout << n->H << endl;

        sprintf(textoEsq, "%i", n->G);// distance from starting node
        sprintf(textoDir, "%i", n->H);//distance from end node
        sprintf(textoCentro, "%i", n->F);//G+H

        //std::cout << n->pos.x << std::endl;

        int compensarX = -3;
        int compensarY = 10;

        EscreverCentralizada(textoEsq, compensarX+(tam/2) + n->pos.x * tam + posG.x, compensarY+ tamyy - (n->pos.y * tam) + posG.y, color, fonte);
        EscreverCentralizada(textoDir, compensarX+(tam/2) + n->pos.x * tam + posH.x, compensarY+ tamyy - (n->pos.y * tam) + posH.y, color, fonte);
        EscreverCentralizada(textoCentro, compensarX + (tam/2) + n->pos.x * tam + posF.x, compensarY+ tamyy - (n->pos.y * tam)  + posF.y, color, fonte);



    }

    //ESCREVER NO A E NO B TA BUGADO
    int cX = 28;
    int cY = 15;
    int tamSpr = 64;


    PathCell* sA = getSpawn('a');
    PathCell* sB = getSpawn('b');

    char texto[5];



    sprintf(texto, "A");
    EscreverCentralizada(texto, (tamSpr/2) + sA->pos.x * tamSpr + cX, cY + 800 - sA->pos.y * tamSpr, color, fonte2);

    sprintf(texto, "B");
    EscreverCentralizada(texto, (tamSpr/2) + sB->pos.x * tamSpr + cX, cY + 800 - sB->pos.y * tamSpr, color, fonte2);


    /*
    for (auto n : pathCellVector)
    {
        sprintf(textoEsq, "%i", n->G);
        sprintf(textoDir, "%i", n->H);
        sprintf(textoCentro, "%i", n->F);

        EscreverCentralizada(textoEsq, (tam/2) + n->pos.x * tam + posG.x, 800 - (n->pos.y * tam) + posG.y);
        EscreverCentralizada(textoDir, (tam/2) + n->pos.x * tam + posH.x, 800 - (n->pos.y * tam) + posH.y);
        EscreverCentralizada(textoCentro, (tam/2) + n->pos.x * tam + posF.x, 800 - (n->pos.y * tam)  + posF.y);
    }
    */

}

PIG_Evento evento;          //evento ser tratadoi a cada pssada do loop principal
PIG_Teclado meuTeclado;     //vari�vel como mapeamento do teclado

double posX = 50, posY = 50;
int obj;
int timerDesenho, timerGeral;
double anguloObj;
float fps;

void desenhar()
{
    //ser� feita aprepa��o do frame que ser� exibido na tela
    IniciaDesenho();

    desenhaCampo();

    desenhaPath();

    desenhaEscritas();

    //aqui o evento deve ser tratado e as coisas devem ser atualizadas
    /*if(meuTeclado[TECLA_BARRAESPACO])
    {

        DesenhaRetangulo(posX, posY, 50, 100, AZUL, 0);

    }*/

    //DesenhaLinhaSimples(300, 300, 700, 700, BRANCO);

    //DesenhaRetangulo(posX, posY, 100, 100, AZUL, 0);

    //char String[100];
    //sprintf(String, "E ai manow, X: %.0f Y: %.0f, FPS: %.0f", posX, posY, fps);

    //EscreverEsquerda(String, 100, 700, VERDE, fonte, 0);

    DesenhaObjeto(obj);
    //todas as chamadas de desenho devem ser feitas aqui na ordem desejada




    //o frame totalmente pronto ser� mostrado na tela
    EncerraDesenho();
}

void verificarTeclado()
{

    double velocidade = 1.3;
    if(meuTeclado[TECLA_a])
        posX -= velocidade;

    if(meuTeclado[TECLA_d])
        posX += velocidade;

    if(meuTeclado[TECLA_w])
        posY += velocidade;

    if(meuTeclado[TECLA_s])
        posY -= velocidade;
}

void configuracoesIniciais()
{
    iniciaNodeMap();

    findPath((getSpawn('a')), (getSpawn('b')));
    std::cout << "inicia jogo\n";

    //criando o jogo (aplica��o)
    CriaJogo("Meu Jogo");

    //associando o teclado (basta uma �nica vez) com a vari�vel meuTeclado
    meuTeclado = GetTeclado();

    timerDesenho = CriaTimer();
    timerGeral = CriaTimer();



    obj = CriaObjeto("idle1.png", 0, 1);
    SetDimensoesObjeto(obj, 472/2, 680/2);
    SetPivoObjeto(obj, (200/2), 250/2);

    fonte = CriaFonteNormal("..\\fontes\\arial.ttf", 12, PRETO);
    fonte2 = CriaFonteNormal("..\\fontes\\arial.ttf", 26, CINZA);

    anguloObj = 0;
}

int main( int argc, char* args[] ){

    configuracoesIniciais();

    //loop principal do jogo
    while(JogoRodando())
    {
        fps = GetFPS();

        //pega um evento que tenha ocorrido desde a �ltima passada do loop
        evento = GetEvento();

        MoveObjeto(obj, posX, posY);


        if(TempoDecorrido(timerGeral) >= BASE60FPS)
        {
            verificarTeclado();

            SetAnguloObjeto(obj, anguloObj);

            anguloObj += 0.1;

            ReiniciaTimer(timerGeral);
        }

        if(TempoDecorrido(timerDesenho) >= BASE60FPS)
        {
            desenhar();

            ReiniciaTimer(timerDesenho);
        }
    }

    //o jogo ser� encerrado
    FinalizaJogo();

    return 0;
}





/*

int nG = theNode->G + lilDist;
                    if(nG < pathp->G)
                    {
                        pathp->G = nG;
                        pathp->somaPeso();
                        pathp->parent = theNode
                    }
*/