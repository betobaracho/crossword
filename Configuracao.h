#pragma once

#include <string>
#include <unordered_map>

class Configuracao {
public:
    explicit Configuracao(const std::string& caminhoArquivo);

    bool carregar();
    std::string getValor(const std::string& secao, const std::string& chave) const;

private:
    std::string arquivo;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> valores;
};
