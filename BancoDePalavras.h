#ifndef BANCODEPALAVRAS_H
#define BANCODEPALAVRAS_H

#include <string>
#include <vector>
#include "Palavra.h"

class BancoDePalavras {
public:
    BancoDePalavras(const std::string& caminhoArquivo, const std::string& conteudo="");

    bool carregar();                         // Lê o arquivo e carrega as palavras
    Palavra obterAleatoria() const;          // Retorna uma palavra aleatória
    void listarPalavras();             // Exibe todas no console
    bool vazio() const;                      // Retorna true se não carregou nada
    void listarPalavrasEscondidas();
    int getQuantidade() const;
    Palavra getPalavra(int) const;    
    string converterQuebrasDeLinha(string texto);
    bool recarregarArquivo(const std::string& conteudo);
private:
    std::string arquivo;                     // Caminho do arquivo
    std::vector<Palavra> palavras;           // Vetor de palavras
};

#endif
#pragma once
