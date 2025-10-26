#ifndef BANCODEPALAVRAS_H
#define BANCODEPALAVRAS_H

#include <string>
#include <vector>
#include "Palavra.h"

class BancoDePalavras {
public:
    BancoDePalavras(const std::string& caminhoArquivo);

    bool carregar();                         // L� o arquivo e carrega as palavras
    Palavra obterAleatoria() const;          // Retorna uma palavra aleat�ria
    void listarPalavras();             // Exibe todas no console
    bool vazio() const;                      // Retorna true se n�o carregou nada
    void listarPalavrasEscondidas();
    int getQuantidade() const;
    Palavra getPalavra(int) const;
private:
    std::string arquivo;                     // Caminho do arquivo
    std::vector<Palavra> palavras;           // Vetor de palavras
};

#endif
#pragma once
