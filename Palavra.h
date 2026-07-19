#ifndef PALAVRA_H
#define PALAVRA_H

#include <string>
using std::string;
#include "Posicao.h"
#include<vector>
//#include "Tabuleiro.h"

class Palavra {
public:
    Palavra();
    void setPalavra(string);
    string getPalavra();
    int getTamanho();
    Posicao getPosicao();
    void setPosicao(Posicao);
    int getOrientacao();
    void setOrientacao(int);
    int getSentido();
    void setSentido(int);
    string getDica();
    void setDica(string);
   // void mostrarPalavra();
    bool isDescoberta();
    void setDescoberta(bool);    
    void gerarPosicaoAleatoria(int alturaMax, int larguraMax);
    void adicionarPosicao(const Posicao& p);
    std::vector<Posicao> getPosicoes() const;
    void setTentativas(int t);
    int getTentativas();
    void incrementartentativas();
    void mostrarComoDescoberta();
    void adicionarTentativaErrada(string tentativa);
    std::vector<string> getTentativasErradas() const;
    string letrasConhecidas();

private:
    string palavra;
    int tamanho;
    Posicao posicao;
    int sentido;
    int orientacao;
    string dica;
    bool descoberta;    
    int tentativas;
    std::vector<Posicao> posicoes;
    std::vector<string> tentativasErradas;
};

#endif
