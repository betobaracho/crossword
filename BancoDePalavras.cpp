#include "BancoDePalavras.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>  // rand, srand
#include <ctime>
#include <filesystem>
BancoDePalavras::BancoDePalavras(const std::string& caminhoArquivo)
    : arquivo(caminhoArquivo) {
    srand(static_cast<unsigned>(time(nullptr)));
}

bool BancoDePalavras::carregar() {
    std::ifstream fin(arquivo);
    if (!fin.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << arquivo << std::endl;
     //   std::cerr << "Diretório atual: " << std::filesystem::current_path() << std::endl;
        return false;
    }

    std::string linha;
    while (std::getline(fin, linha)) {
        if (linha.empty()) continue;

        std::istringstream ss(linha);
        std::string texto, dica;

        if (std::getline(ss, texto, ';') && std::getline(ss, dica)) {
            Palavra p;
            p.setPalavra(texto);
            p.setDica(dica);
            palavras.push_back(p);
        }
    }

    fin.close();
    return !palavras.empty();
}

Palavra BancoDePalavras::obterAleatoria() const {
    if (palavras.empty()) return Palavra();
    int indice = rand() % palavras.size();
    return palavras[indice];
}

void BancoDePalavras::listarPalavras()  {
    for (auto& p : palavras) {
        std::cout << "- " << p.getPalavra() << " — " << p.getDica() << std::endl;
    }
}

void BancoDePalavras::listarPalavrasEscondidas() {
    for (Palavra palavra : palavras) {
        std::cout << palavra.getPalavra();
    }
}

bool BancoDePalavras::vazio() const {
    return palavras.empty();
}

int BancoDePalavras::getQuantidade() const {
    return static_cast<int>(palavras.size());
}

Palavra BancoDePalavras::getPalavra(int posicao) const
{
    return palavras.at(posicao);
}
