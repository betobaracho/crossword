// Códigos ANSI para cores no console
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"


#include<iostream>
using std::cout;
#include <iomanip>
using std::setw;
using std::endl;
#include "Tabuleiro.h"
#include <sstream>

Tabuleiro::Tabuleiro() {
    cout << setw(2);
    for (int i = 0; i < comprimento; i++)
    {
        for (int j = 0; j < altura; j++)
        {
            Posicao p = Posicao(i,j,'.');
            tabuleiro[i][j] = p;
        }
    }
}
void Tabuleiro::mostrarTabuleiro() {
    std::cout << toString();
}

int Tabuleiro::avaliarPosicao(const Palavra& p, int linha, int coluna, int sentido, int orientacao) {
    const std::string& texto = p.getPalavra();
    const int len = (int)texto.size();

    int dx = 0, dy = 0;
    if (sentido == 0) { dx = (orientacao == 0 ? +1 : -1); dy = 0; }
    else { dx = 0; dy = (orientacao == 0 ? +1 : -1); }

    int score = 0;

    for (int k = 0; k < len; ++k) {
        int r = linha + dy * k;
        int c = coluna + dx * k;

        // limites
        if (r < 0 || r >= altura || c < 0 || c >= comprimento) return -9999;

        char cel = tabuleiro[r][c].getLetra();
        char letra = texto[k];

        if (cel == '.') {
            score += 1; // espaço livre
            // bônus por vizinhança ocupada (estimula cruzamentos)
            for (int vr = -1; vr <= 1; ++vr) {
                for (int vc = -1; vc <= 1; ++vc) {
                    if (vr == 0 && vc == 0) continue;
                    int nr = r + vr, nc = c + vc;
                    if (nr >= 0 && nr < altura && nc >= 0 && nc < comprimento) {
                        if (tabuleiro[nr][nc].getLetra() != '.') score += 3; // peso ajustado
                    }
                }
            }
        }
        else if (cel == letra) {
            score += 8; // cruzamento válido (peso alto)
        }
        else {
            return -9999; // colisão
        }
    }

    // leve preferência por bordas/cantos (opcional)
    if (linha < 3 || linha > altura - 3 || coluna < 3 || coluna > comprimento - 3) score += 1;

    return score;
}

void Tabuleiro::posicionarHeuristico(Palavra& p) {
    int melhorScore = -99999;
    int melhorLinha = 0, melhorColuna = 0, melhorSentido = 0, melhorOrientacao = 0;

    for (int sentido = 0; sentido < 2; ++sentido) {           // 0=horizontal, 1=vertical
        for (int orientacao = 0; orientacao < 2; ++orientacao) { // 0=dir/baixo, 1=esq/cima
            for (int r = 0; r < altura; ++r) {
                for (int c = 0; c < comprimento; ++c) {
                    int s = avaliarPosicao(p, r, c, sentido, orientacao);
                    if (s > melhorScore) {
                        melhorScore = s;
                        melhorLinha = r; melhorColuna = c;
                        melhorSentido = sentido; melhorOrientacao = orientacao;
                    }
                }
            }
        }
    }

    // fallback bobo se nada couber
    if (melhorScore < 0) {
        melhorLinha = 0; melhorColuna = 0; melhorSentido = 0; melhorOrientacao = 0;
    }

    p.setPosicao(Posicao(melhorLinha, melhorColuna));
    p.setSentido(melhorSentido);
    p.setOrientacao(melhorOrientacao);
}

/*
bool Tabuleiro::checkarColisao(Palavra& p) {
    int sentido = p.getSentido();
    int orientacao = p.getOrientacao();
    string palavra = p.getPalavra();
    Posicao posicao = p.getPosicao();
    int linha = posicao.getLinha();
    int coluna = posicao.getColuna();
    int tamanhoPalavra = p.getTamanho();
    int caracter = 0;
    
    if (sentido == 0) { //horizontal
        if (orientacao == 0) { //para a direita
            for (int i = coluna; i < coluna + tamanhoPalavra; i++) {

                if (i < 0 || i >= comprimento || linha < 0 || linha >= altura)
                    return false;

                if (tabuleiro[linha][i].getLetra() != palavra.at(caracter++) && tabuleiro[linha][i].getLetra() != '.') {
                    return false;
                }
            }
            return true;
        }
        else if (orientacao == 1) { //para a esquerda

           // for (int i = coluna + tamanhoPalavra; i > coluna; i--) {
            for (int i = coluna; i > coluna - tamanhoPalavra; i--) {
                if (i < 0 || i >= comprimento || linha < 0 || linha >= altura)
                    return false;
                if (tabuleiro[linha][i].getLetra() != palavra.at(caracter++) && tabuleiro[linha][i].getLetra() != '.') {
                    return false;

                }
            }
            return true;
        }
    }
    else if (sentido == 1) { //vertical
        if (orientacao == 0) { //para baixo
            for (int i = linha; i < linha + tamanhoPalavra; i++) {
                if (i < 0 || i >= comprimento || linha < 0 || linha >= altura)
                    return false;
                if (tabuleiro[i][coluna].getLetra() != palavra.at(caracter++) || tabuleiro[i][coluna].getLetra() != '.') {
                    return false;
                }
            }
            return true;
        }
        else if (orientacao == 1) { //para cima
            for (int i = linha; i > linha - tamanhoPalavra; i--) {
                if (i < 0 || i >= comprimento || linha < 0 || linha >= altura)
                    return false;
                if (tabuleiro[i][coluna].getLetra() != palavra.at(caracter++) && tabuleiro[i][coluna].getLetra() != '.') {
                    return false;
                }
            }
            return true;
        }
    }

    return true;
}
*/


void Tabuleiro::addPalavra(Palavra& p) {
    const std::string& texto = p.getPalavra();
    const int len = (int)texto.size();

    int r0 = p.getPosicao().getLinha();
    int c0 = p.getPosicao().getColuna();

    int dx = 0, dy = 0;
    if (p.getSentido() == 0) { dx = (p.getOrientacao() == 0 ? +1 : -1); dy = 0; }
    else { dx = 0; dy = (p.getOrientacao() == 0 ? +1 : -1); }

    for (int k = 0; k < len; ++k) {
        int r = r0 + dy * k;
        int c = c0 + dx * k;

        if (r < 0 || r >= altura || c < 0 || c >= comprimento) return;

        char letra = texto[k];
        tabuleiro[r][c].setLetra(letra);
        tabuleiro[r][c].setOcupada(true);

        // se guarda as posições na Palavra:
        p.adicionarPosicao(Posicao(r, c, letra));
    }
}

bool Tabuleiro::checkarColisao(Palavra& p) {
    const std::string& texto = p.getPalavra();
    const int len = (int)texto.size();

    int r0 = p.getPosicao().getLinha();
    int c0 = p.getPosicao().getColuna();

    int dx = 0, dy = 0;
    if (p.getSentido() == 0) { dx = (p.getOrientacao() == 0 ? +1 : -1); dy = 0; }
    else { dx = 0; dy = (p.getOrientacao() == 0 ? +1 : -1); }

    for (int k = 0; k < len; ++k) {
        int r = r0 + dy * k;
        int c = c0 + dx * k;

        if (r < 0 || r >= altura || c < 0 || c >= comprimento) return false;

        char cel = tabuleiro[r][c].getLetra();
        char letra = texto[k];

        if (cel != '.' && cel != letra) return false; // só permite vazio ou mesma letra
    }
    return true;
}


std::string Tabuleiro::toString() const {
    std::ostringstream buffer;
    for (int i = 0; i < comprimento; i++) {
        for (int j = 0; j < altura; j++) {
            buffer << tabuleiro[i][j].getLetra() << " ";
        }
        buffer << '\n';
    }
    return buffer.str();
}

bool Tabuleiro::tentarDescobrirPalavra(int numero, const std::string& tentativa) {
    // Ajuste de índice: jogador digita 1,2,3... então subtrai 1
    int idx = numero - 1;
    if (idx < 0 || idx >= (int)palavrasDoJogo.size()) {
        std::cout << "Número inválido.\n";
        return false;
    }

    Palavra& p = palavrasDoJogo[idx];
    if (tentativa == p.getPalavra()) {
        p.setDescoberta(true);
        p.getPosicao().setOculta(false);

        
        for (Posicao posicao : p.getPosicoes()) {            
            tabuleiro[posicao.getLinha()][posicao.getColuna()].setOculta(false);
        }
                       
        std::cout << "✅ Você acertou a palavra \"" << p.getPalavra() << "\"!\n";
        return true;
    }
    else {
        std::cout << "❌ Palavra incorreta. Tente novamente.\n";
        return false;
    }
}

void Tabuleiro::adicionarPalavraJogo(const Palavra& p) {
    palavrasDoJogo.push_back(p);
}

void Tabuleiro::mostrarDicas(){
    cout << CYAN << "\n=== DICAS ===\n" << RESET;
   // cout << "\n=== DICAS ===\n";
    int i = 1;
    for (Palavra p : palavrasDoJogo) {
        cout << YELLOW << setw(2) << i++ << ". " << RESET;
       // cout << i++ << ". ";
        if (p.isDescoberta()) {
            cout << GREEN << p.getPalavra() << RESET;
            cout << " " << MAGENTA << "(✔ " << p.getDica() << ")" << RESET;
        }
        else {
            cout << WHITE;
            for (int j = 0; j < p.getTamanho(); j++) {
                if (tabuleiro[p.getPosicoes().at(j).getLinha()][p.getPosicoes().at(j).getColuna()].getOculta()) {
                    cout << "_ ";
                }
                else cout << p.getPosicoes().at(j).getLetra();
             /*   if (p.getPosicao().getOculta())
                    cout << "_ ";
                else cout << p.getPosicoes().at(j).getLetra();*/
            }
               
            cout << RESET;
            cout << " " << CYAN << "(" << p.getDica() << ")" << RESET;
        }
        cout << endl;
    }
}

int Tabuleiro::getAltura() const {
    return altura;
}

int Tabuleiro::getComprimento() const {
    return comprimento;
}


void Tabuleiro::mostrarComProgresso() const {
    cout << "\n=== TABULEIRO ===\n";
  
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < comprimento; j++) {

            if (tabuleiro[i][j].isOcupada()) { //tem uma letra de uma palavra
                if (tabuleiro[i][j].getOculta()) { //a palavra que tem a letra ainda não foi descoberta
                   // cout << 'X';
                    cout << tabuleiro[i][j].getLetra();
                }
                else { //tem uma letra e esta palavra já foi descoberta
                    cout << tabuleiro[i][j].getLetra();
                }
            }
            else { //não tem uma letra
                cout << tabuleiro[i][j].getLetra();
            }         
            cout << ' ';
        }
        cout << endl;
    }
}


void Tabuleiro::addPalavra(Palavra& p) {
    int sentido = p.getSentido();
    int orientacao = p.getOrientacao();
    string palavra = p.getPalavra();
    Posicao posicao = p.getPosicao();
    int linha = posicao.getLinha();
    int coluna = posicao.getColuna();
    int tamanhoPalavra = p.getTamanho();
    int caracter = 0;
    if (sentido == 0) // horizontal
    {
        if (orientacao == 0) // para a direita
        {
            
            for (int i = coluna; i < tamanhoPalavra + coluna; i++)
            {
                if (i < 0 || i >= comprimento || linha < 0 || linha >= altura)
                    break;

                if (linha < 0 || linha >= altura || coluna < 0 || coluna >= comprimento) {
                    std::cerr << "[ERRO] Fora dos limites: (" << linha << ", " << coluna << ")\n";
                    return;
                }

                char letra = palavra.at(caracter++);
                Posicao pLetra(linha, i, letra);
                pLetra.setOcupada(true);                
                p.adicionarPosicao(pLetra);
                tabuleiro[linha][i] = pLetra;
            }
        }
        else if (orientacao == 1) //para a esquerda
        {            
            for (int i = coluna - 1; i > (coluna - 1) - tamanhoPalavra; i--)
            {
                if (i < 0 || i >= comprimento || linha < 0 || linha >= altura)
                    break;

                if (linha < 0 || linha >= altura || coluna < 0 || coluna >= comprimento) {
                    std::cerr << "[ERRO] Fora dos limites: (" << linha << ", " << coluna << ")\n";
                    return;
                }

                char letra = palavra.at(caracter++);
                Posicao pLetra(linha, i, letra);
                pLetra.setOcupada(true);
                p.adicionarPosicao(pLetra);
                tabuleiro[linha][i] = pLetra; //'1';//palavra[caracter++];'_'; //
            }
        }
    }
    else if (sentido == 1) //vertical
    {
        if (orientacao == 0) //para baixo
        {
            for (int i = linha; i < tamanhoPalavra + linha; i++)
            {
                if (i < 0 || i >= comprimento || linha < 0 || linha >= altura)
                    break;

                if (linha < 0 || linha >= altura || coluna < 0 || coluna >= comprimento) {
                    std::cerr << "[ERRO] Fora dos limites: (" << linha << ", " << coluna << ")\n";
                    return;
                }

                char letra = palavra.at(caracter++);
                Posicao pLetra(i, coluna, letra);
                pLetra.setOcupada(true);
                p.adicionarPosicao(pLetra);
                tabuleiro[i][coluna] = pLetra;
            }
        }
        else //para cima
        {
            for (int i = linha; i > linha - tamanhoPalavra; i--)
            {
                if (i < 0 || i >= comprimento || linha < 0 || linha >= altura)
                    break;

                if (linha < 0 || linha >= altura || coluna < 0 || coluna >= comprimento) {
                    std::cerr << "[ERRO] Fora dos limites: (" << linha << ", " << coluna << ")\n";
                    return;
                }
                char letra = palavra.at(caracter++);
                Posicao pLetra(i, coluna, letra);
                pLetra.setOcupada(true);
                p.adicionarPosicao(pLetra);
                tabuleiro[i][coluna] = pLetra;
            }
        }
    }
}
