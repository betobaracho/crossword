#pragma once

#include <string>
#include <vector>

using namespace std;

class AssistenteIA {
public:
    AssistenteIA(bool usarIA = false, int tipoIA = 0);
    string consultarOpenAI(string prompt);
    string tentarAdivinhar(
        int numero,
        int tamanho,
        string dica,
        string letrasConhecidas,
        std::vector<string> tentativasErradas
    );
    string consultarOllama(string prompt);
    string extrairRespostaOpenAI(const string& json);
    string gerarArquivoPalavras(int nivel = 1);
    void setUsarIa(bool usar);
private:
    bool usarOpenAI = false;
    bool usarIA = false;
};