#include <iostream>
#include "Palavra.h"
#include <cstdlib>
using std::cout;

/*Palavra::Palavra()
    : posicao(), palavra(""), tamanho(0), sentido(0), orientacao(0), dica(""), descoberta(false) {
}*/

Palavra::Palavra() {
    posicao = Posicao();
    palavra = "";
    tamanho = 0;
    sentido = 0;
    orientacao = 0;
    dica = "";
    descoberta = false;    
}

void Palavra::setPalavra(string palavraNova) {
    palavra = palavraNova;
}

string Palavra::getPalavra() {
    return palavra;
}

int Palavra::getTamanho() {
    return palavra.length();
}

Posicao Palavra::getPosicao() {
    return posicao;
}

void Palavra::setPosicao(Posicao posicaoPalavra) {
    posicao = posicaoPalavra;
}

int Palavra::getOrientacao() {
    return orientacao;
}

void Palavra::setOrientacao(int o) {
    orientacao = o;
}

int Palavra::getSentido() {
    return sentido;
}

void Palavra::setSentido(int s) {
    sentido = s;
}

string Palavra::getDica() {
    return dica;
}

void Palavra::setDica(string d) {
    dica = d;
}


bool Palavra::isDescoberta() {
    return descoberta;
}

void Palavra::setDescoberta(bool valor) {    
    descoberta = valor;
}

void Palavra::gerarPosicaoAleatoria(int alturaMax, int comprimentoMax) {
    int tamanho = getTamanho();
    sentido = rand() % 2;       // 0 = horizontal, 1 = vertical
    orientacao = rand() % 2;    // 0 = direita/baixo, 1 = esquerda/cima

    int linha = 0, coluna = 0;

    if (sentido == 0) { // horizontal
        if (orientacao == 0)
            coluna = rand() % (comprimentoMax - tamanho); // direita
        else
            coluna = rand() % (comprimentoMax - tamanho) + tamanho; // esquerda
        linha = rand() % alturaMax;
    }
    else { // vertical
        if (orientacao == 0)
            linha = rand() % (alturaMax - tamanho); // para baixo
        else
            linha = rand() % (alturaMax - tamanho) + tamanho; // para cima
        coluna = rand() % comprimentoMax;
    }

    posicao = Posicao(linha, coluna);
}

void Palavra::adicionarPosicao(const Posicao& p) {
    posicoes.push_back(p);
}

std::vector<Posicao> Palavra::getPosicoes() const {
    return posicoes;
}


/*
void Palavra::gerarPosicaoHeuristica(const Tabuleiro& tabuleiro) {
    int melhorScore = -99999;
    int melhorLinha = 0;
    int melhorColuna = 0;
    int melhorSentido = 0;
    int melhorOrientacao = 0;

    const int altura = tabuleiro.getAltura();
    const int comprimento = tabuleiro.getComprimento();

    // tenta todas as direções e orientações
    for (int sentido = 0; sentido < 2; ++sentido) { // 0=horizontal, 1=vertical
        for (int orientacao = 0; orientacao < 2; ++orientacao) {
            for (int linha = 0; linha < altura; ++linha) {
                for (int coluna = 0; coluna < comprimento; ++coluna) {
                    int score = tabuleiro.avaliarPosicao(*this, linha, coluna, sentido, orientacao);
                    if (score > melhorScore) {
                        melhorScore = score;
                        melhorLinha = linha;
                        melhorColuna = coluna;
                        melhorSentido = sentido;
                        melhorOrientacao = orientacao;
                    }
                }
            }
        }
    }

    // fallback (se nenhuma posição for válida)
    if (melhorScore < 0) {
        melhorLinha = rand() % altura;
        melhorColuna = rand() % comprimento;
        melhorSentido = rand() % 2;
        melhorOrientacao = rand() % 2;
    }

    // aplica a posição final à palavra
    setPosicao(Posicao(melhorLinha, melhorColuna));
    setSentido(melhorSentido);
    setOrientacao(melhorOrientacao);
}
*/