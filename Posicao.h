#ifndef POSICAO_H
#define POSICAO_H

class Posicao {
public:
    Posicao();                                // construtor padr�o
    Posicao(int linha, int coluna, char letra = ' ');    // construtor com par�metros

    // M�todos de acesso (getters e setters)
    int getLinha() const;
    void setLinha(int l);

    int getColuna() const;
    void setColuna(int c);

    bool isOcupada() const;
    void setOcupada(bool valor);

    char getLetra() const;
    void setLetra(char l);

    bool getOculta() const;
    void setOculta(bool valor);

private:
    int linha;
    int coluna;
    bool ocupada;
    bool oculta;
    char letra;
};

#endif
#pragma once
