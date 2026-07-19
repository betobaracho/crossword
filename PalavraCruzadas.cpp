#include <iostream>
#include <windows.h>
#include <ctime>
#include "BancoDePalavras.h"
#include "Tabuleiro.h"
#include "AssistenteIA.h"

using namespace std;



int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand(static_cast<unsigned>(time(nullptr)));
    cout << "Escolhendo palavras para o jogo\n";
    Tabuleiro tabuleiro;
    AssistenteIA ia(false);
    BancoDePalavras banco("palavras.txt", ia.gerarArquivoPalavras(1));
    
    banco.carregar();

    cout << "Gerando tabuleiro heurístico...\n";
    tabuleiro.gerarTabuleiroHeuristico(banco);

    int opcao = -1;
    while (opcao != 0) {
        cout << "\n=== MENU ===\n";
        cout << "1 - Mostrar dicas\n";
        cout << "2 - Tentar adivinhar uma palavra\n";
        cout << "3 - Mostrar tabuleiro (progresso)\n";
        cout << "4 - Tentar adivinhar - Assistente de IA\n";
        cout << "5 - Gerar novo tabuleiro\n";
        cout << "6 - Escolher o nível do jogo: 1 - Médio, 2 - Dificil, 3 - Super desafio\n";
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
        else if (opcao == 4) {
            

            int numeroPalavra;
            tabuleiro.mostrarDicas();
            cout << "Digite o número da palavra: ";
            cin >> numeroPalavra;
            
            Palavra p = tabuleiro.getPalavraPorNumero(numeroPalavra);
                
            cout << "Consultando IA...\n";
            string respostaIA = ia.tentarAdivinhar(
                numeroPalavra,
                p.getTamanho(),
                p.getDica(),
                p.letrasConhecidas(),
                p.getTentativasErradas()
            );
            
            cout << "IA respondeu: " << respostaIA << endl;
            if (tabuleiro.tentarDescobrirPalavra(numeroPalavra, respostaIA)) {
                cout << "A IA acertou!\n";
                tabuleiro.mostrarComProgresso();
            }
            else {
                cout << "A IA errou!\n";
            }
            //break;
        }
        else if (opcao == 5) {
            ia.setUsarIa(true);
            tabuleiro.limparPalavrasDoJogo();
            banco.recarregarArquivo(ia.gerarArquivoPalavras(1));
            tabuleiro.gerarTabuleiroHeuristico(banco);
        }
        else if (opcao == 3) tabuleiro.mostrarComProgresso();
    }

    cout << "Fim de jogo!\n";
    return 0;
}
