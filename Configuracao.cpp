#include "Configuracao.h"

#include <algorithm>
#include <cctype>
#include <fstream>

namespace {
std::string removerEspacos(const std::string& texto) {
    const auto inicio = std::find_if_not(texto.begin(), texto.end(),
        [](unsigned char caractere) { return std::isspace(caractere); });
    const auto fim = std::find_if_not(texto.rbegin(), texto.rend(),
        [](unsigned char caractere) { return std::isspace(caractere); }).base();

    if (inicio >= fim) {
        return "";
    }

    return std::string(inicio, fim);
}
}

Configuracao::Configuracao(const std::string& caminhoArquivo)
    : arquivo(caminhoArquivo) {
}

bool Configuracao::carregar() {
    std::ifstream entrada(arquivo);
    if (!entrada.is_open()) {
        return false;
    }

    valores.clear();
    std::string secaoAtual;
    std::string linha;

    while (std::getline(entrada, linha)) {
        linha = removerEspacos(linha);

        if (linha.empty() || linha[0] == ';' || linha[0] == '#') {
            continue;
        }

        if (linha.front() == '[' && linha.back() == ']') {
            secaoAtual = removerEspacos(linha.substr(1, linha.size() - 2));
            continue;
        }

        const std::size_t separador = linha.find('=');
        if (separador == std::string::npos || secaoAtual.empty()) {
            continue;
        }

        const std::string chave = removerEspacos(linha.substr(0, separador));
        const std::string valor = removerEspacos(linha.substr(separador + 1));
        if (!chave.empty()) {
            valores[secaoAtual][chave] = valor;
        }
    }

    return true;
}

std::string Configuracao::getValor(const std::string& secao, const std::string& chave) const {
    const auto secaoEncontrada = valores.find(secao);
    if (secaoEncontrada == valores.end()) {
        return "";
    }

    const auto valorEncontrado = secaoEncontrada->second.find(chave);
    if (valorEncontrado == secaoEncontrada->second.end()) {
        return "";
    }

    return valorEncontrado->second;
}
