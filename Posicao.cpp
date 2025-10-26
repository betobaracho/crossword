#include "Posicao.h"

// Construtor padrão: inicializa tudo zerado e desocupado
Posicao::Posicao() {
    linha = 0;
    coluna = 0;
    ocupada = false;
    letra = '.';
    oculta = true;
}

// Construtor com parâmetros
Posicao::Posicao(int linha, int coluna, char letra) {
    this->linha = linha;
    this->coluna = coluna;
    this->ocupada = false;
    this->letra = letra;
    this->oculta = true;
}

// Getters e setters
int Posicao::getLinha() const {
    return linha;
}

void Posicao::setLinha(int l) {
    linha = l;
}

int Posicao::getColuna() const {
    return coluna;
}

void Posicao::setColuna(int c) {
    coluna = c;
}

bool Posicao::isOcupada() const {
    return ocupada;
}

void Posicao::setOcupada(bool valor) {
    ocupada = valor;
}

char Posicao::getLetra() const {
    return letra;
}

void Posicao::setLetra(char l) {
    letra = l;
}

void Posicao::setOculta(bool valor)
{
    oculta = valor;
}

bool Posicao::getOculta() const{
    return oculta;
}
