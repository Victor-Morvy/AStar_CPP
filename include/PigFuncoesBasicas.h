#ifndef _PigFuncoesBasicas_
#define _PigFuncoesBascias_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <string.h>
#include <time.h>
#include <vector>
#include <map>
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <iomanip>

//separa uma string em palavras, usando os delimitadores indicados
std::vector<std::string> PIGSeparaPalavras(std::string texto,std::string delim){
    std::vector<std::string> resp;
    std::string strAtual = "";

    for (int i=0;i<texto.size();i++){

        if (delim.find(texto[i]) != std::string::npos){//achou delimitadores
            resp.push_back(strAtual);

            strAtual = texto[i];
            if (texto[i]!='\n'){
                resp.push_back(strAtual);
                strAtual = "";
            }

        }else strAtual += texto[i];
    }

    if (strAtual!=""){
        resp.push_back(strAtual);
    }
    return resp;
}

//verifica se uma string possui apenas digitos de 0 a 9
bool PIGSomenteNumeros(std::string frase){
    for (int i=0;i<frase.size();i++)
        if (frase[i]<'0'||frase[i]>'9')
            return false;
    return true;
}

//verifica se duas cores s�o iguais
inline bool PIGCoresIguais(PIG_Cor cor1, PIG_Cor cor2){
    return cor1.r==cor2.r&&cor1.g==cor2.g&&cor1.b==cor2.b&&cor1.a==cor2.a;
}

//cria uma cor a partir de uma string com um valor hexadecimal de 8 algarismos RRGGBBAA. Ex: 0xFF0000FF (vermelho)
PIG_Cor PIGCriaCor(char *stringHexa){
    unsigned long total = strtoul(stringHexa,0,16);//transforma a string em um inteiro (decimal)
    PIG_Cor cor;
    cor.a = total %256;
    total /= 256;
    cor.b = total %256;
    total /= 256;
    cor.g = total %256;
    total /= 256;
    cor.r = total %256;
    return cor;
}

//multiplica as componentes RGB da cor pelo fator
PIG_Cor PIGMultiplicaCor(PIG_Cor cor, float valor){
    PIG_Cor resp = cor;
    resp.r *= valor;
    resp.g *= valor;
    resp.b *= valor;
    return resp;
}

//troca a posi��o dos bytes de uma word(16bits)
uint16_t PIGTroca2Bytes(uint16_t valor){
    return (valor/256)+((valor%256)*256);
}

//mistura duas cores com uma propor��o entre elas
PIG_Cor MixCor(PIG_Cor iniCor, PIG_Cor fimCor, double porc){
    PIG_Cor resp;
    resp.r = fimCor.r*porc + (iniCor.r)*(1-porc);
    resp.g = fimCor.g*porc + (iniCor.g)*(1-porc);
    resp.b = fimCor.b*porc + (iniCor.b)*(1-porc);
    resp.a = fimCor.a*porc + (iniCor.a)*(1-porc);
    return resp;
}

//retorna o diretorio onde est� o execut�vel
std::string PIGGetDiretorioAtual(){
    char *dir = SDL_GetBasePath();
    if (dir){
        std::string resp(dir);
        CHDIR(dir);
        free(dir);
        return resp;
    }else return "./";
}

//retorna "valor" limitado entre [vMin, vMax]
int PIGLimitaValor(int valor, int minimo,int maximo){
    if (valor<minimo) return minimo; //valor n�o pode ficar menor que o m�nimo informado
    else if (valor>maximo) return maximo;//valor n�o pode ficar maior que o m�ximo informado
    else return valor;
}

//cria uma lista de strings contendo o nome do arquivos de um diret�rio
std::vector<std::string> PIGListaArquivosDiretorio(std::string path) {
   std::vector<std::string> resp;
   struct dirent *entry;
   DIR *dir = opendir(path.c_str());

   if (dir != NULL) {
       while ((entry = readdir(dir)) != NULL) {
        resp.push_back(entry->d_name);
       }
       closedir(dir);
   }
   return resp;
}
#endif // _PigFuncoesBasicas_
