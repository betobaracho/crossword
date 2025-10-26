#ifndef TABULEIRO_H
#define TABULEIRO_H
#include <vector>
#include "Palavra.h"
#include "Posicao.h"
class Tabuleiro {
public:
	Tabuleiro();
	void mostrarTabuleiro();
	void addPalavra(Palavra& p);
	bool checkarColisao(Palavra& p);
	std::string toString() const;
	void adicionarPalavraJogo(const Palavra& p);
	void mostrarDicas();
	void mostrarComProgresso() const;
	bool tentarDescobrirPalavra(int numero, const std::string& tentativa);
	int getAltura() const;
	int getComprimento() const;
	int  avaliarPosicao(const Palavra& p, int linha, int coluna, int sentido, int orientacao);
	void posicionarHeuristico(Palavra& p);

private:
	const int altura = 20;
	const int comprimento = 20;
	//char tabuleiro[20][20];
	Posicao tabuleiro[20][20];
	std::vector<Palavra> palavrasDoJogo;
};
#endif