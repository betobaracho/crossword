#include <iostream>
#include <windows.h>
#include <ctime>
#include "BancoDePalavras.h"
#include "Tabuleiro.h"
using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand(static_cast<unsigned>(time(nullptr)));

    Tabuleiro tabuleiro;
    BancoDePalavras banco("palavras.txt");
    banco.carregar();

    cout << "Gerando tabuleiro heurístico...\n";
    tabuleiro.gerarTabuleiroHeuristico(banco);

    int opcao = -1;
    while (opcao != 0) {
        cout << "\n=== MENU ===\n";
        cout << "1 - Mostrar dicas\n";
        cout << "2 - Tentar adivinhar uma palavra\n";
        cout << "3 - Mostrar tabuleiro (progresso)\n";
        cout << "0 - Sair\n";
        cout << "Escolha: ";
        cin >> opcao;

        if (opcao == 1) tabuleiro.mostrarDicas();
        else if (opcao == 2) {
            int numero;
            string tentativa;
            tabuleiro.mostrarDicas();
            cout << "\nDigite o número da palavra: ";
            cin >> numero;
            cout << "Digite sua resposta: ";
            cin >> tentativa;

            if (tabuleiro.tentarDescobrirPalavra(numero, tentativa))
                tabuleiro.mostrarComProgresso();
            else
                cout << "Tente novamente!\n";
        }
        else if (opcao == 3) tabuleiro.mostrarComProgresso();
    }

    cout << "Fim de jogo!\n";
    return 0;
}
