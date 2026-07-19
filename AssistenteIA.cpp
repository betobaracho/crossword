#include "AssistenteIA.h"
#include "Configuracao.h"
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
AssistenteIA::AssistenteIA(bool ia, int tipoIA)
{
    usarIA = ia;
    if (tipoIA == 1) {
        usarOpenAI = true;
    }
    
}

string escaparJson(const string& texto) {
    string resultado;

    for (char c : texto) {

        switch (c) {

        case '\"':
            resultado += "\\\"";
            break;

        case '\n':
            resultado += "\\n";
            break;

        case '\r':
            break;

        case '\t':
            resultado += "\\t";
            break;

        default:
            resultado += c;
        }
    }

    return resultado;
}


string extrairRespostaOllama(const string& json) {
    string chave = "\"response\":\"";
    size_t inicio = json.find(chave);

    if (inicio == string::npos) {
        return "";
    }

    inicio += chave.length();

    size_t fim = json.find("\"", inicio);

    if (fim == string::npos) {
        return "";
    }

    return json.substr(inicio, fim - inicio);
}
static size_t escreverResposta(void* conteudo, size_t tamanho, size_t quantidade, string* resposta) {
    size_t total = tamanho * quantidade;
    resposta->append((char*)conteudo, total);
    return total;
}

string montarTentativasErradas(std::vector<string> tentativasErradas) {
    if (tentativasErradas.empty()) {
        return "Nenhuma";
    }

    string resultado;

    for (string tentativa : tentativasErradas) {
        resultado += "- " + tentativa + "\n";
    }

    return resultado;
}

string AssistenteIA::tentarAdivinhar(
    int numero,
    int tamanho,
    string dica,
    string letrasConhecidas,
    std::vector<string> tentativasErradas
    
) {
    string prompt =
        "Voce esta jogando palavras cruzadas.\n\n"
        "REGRAS:\n"
        "- responda somente UMA palavra\n"
        "- nao explique\n"
        "- nao use parenteses\n"
        "- nao escreva frases\n"
        "- saida deve conter apenas letras\n"
        "- resposta em portugues\n"
        "- nao repita nenhuma tentativa errada\n\n"
        "Dica: " + dica + "\n"
        "Quantidade de letras: " + to_string(tamanho) + "\n"
        "Letras conhecidas: " + letrasConhecidas + "\n"
        "Tentativas erradas anteriores:\n" + montarTentativasErradas(tentativasErradas) + "\n"
        "Resposta:";
    if(!usarOpenAI)
        return consultarOllama(prompt);
    return consultarOpenAI(prompt);
/*
    string json =
        "{"
        "\"model\":\"phi3:mini\","
        "\"prompt\":\"" + escaparJson(prompt) + "\","
        "\"stream\":false"
        "}";

    string respostaCompleta;

    CURL* curl = curl_easy_init();

    if (!curl) {
        return "erro";
    }
    
    cout<<this->consultarOpenAI();

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:11434/api/generate");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, escreverResposta);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respostaCompleta);

    CURLcode resultado = curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (resultado != CURLE_OK) {
        return "erro";
    }

    return extrairRespostaOllama(respostaCompleta);*/
}

string AssistenteIA::consultarOllama(string prompt) {
    string json =
        "{"
        "\"model\":\"gemma3:4b\","
        "\"prompt\":\"" + escaparJson(prompt) + "\","
        "\"stream\":false"
        "}";

    string respostaCompleta;

    CURL* curl = curl_easy_init();

    if (!curl) {
        return "erro";
    }

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:11434/api/generate");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, escreverResposta);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respostaCompleta);

    CURLcode resultado = curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (resultado != CURLE_OK) {
        return "erro";
    }

    return extrairRespostaOllama(respostaCompleta);
}

string AssistenteIA::consultarOpenAI(string prompt)
{
    string respostaCompleta;

    Configuracao configuracao("config.ini");
    if (!configuracao.carregar()) {
        return "erro: nao foi possivel ler config.ini";
    }

    const string apiKey = configuracao.getValor("OpenAI", "ApiKey");
    const string model = configuracao.getValor("OpenAI", "Model");
    if (apiKey.empty() || model.empty()) {
        return "erro: configuracao OpenAI incompleta";
    }

    string json =
        "{"
        "\"model\":\"" + escaparJson(model) + "\","
        "\"messages\":["
        "{"
        "\"role\":\"user\","
        "\"content\":\"" + escaparJson(prompt) + "\""
        "}"
        "]"
        "}";

    CURL* curl = curl_easy_init();

    if (!curl)
        return "erro";

    struct curl_slist* headers = nullptr;

    headers = curl_slist_append(
        headers,
        "Content-Type: application/json"
    );

    const string autorizacao = "Authorization: Bearer " + apiKey;
    headers = curl_slist_append(headers, autorizacao.c_str());

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        "https://api.openai.com/v1/chat/completions"
    );

    curl_easy_setopt(
        curl,
        CURLOPT_HTTPHEADER,
        headers
    );

    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDS,
        json.c_str()
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        escreverResposta
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &respostaCompleta
    );

  //  cout << "\nJSON ENVIADO PARA OPENAI:\n";
  //  cout << json << endl;

    CURLcode resultado = curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (resultado != CURLE_OK)
        return "erro";

    return extrairRespostaOpenAI(respostaCompleta);
}


void AssistenteIA::setUsarIa(bool usar)
{
    this->usarIA = usar;
}

string AssistenteIA::gerarArquivoPalavras(int nivel) {
       
    if (!this->usarIA) return "";
    string regrasNivel;

    if (nivel == 1) {
        regrasNivel =
            "NIVEL: MEDIO\n"
            "- Use palavras comuns ou moderadamente conhecidas.\n"
            "- As palavras devem ter entre 3 e 9 letras.\n"
            "- Temas: cotidiano, natureza, escola, tecnologia simples.\n";
    }
    else if (nivel == 2) {
        regrasNivel =
            "NIVEL: DIFICIL\n"
            "- Use palavras mais especificas e menos obvias.\n"
            "- As palavras devem ter entre 5 e 12 letras.\n"
            "- Temas: Esporte, Capitais de paises, Figuras historicas, Cotidiano, Tecnologia, desenhos animados, filmes.\n"
            "- As dicas podem exigir conhecimento geral mais avancado.\n";
    }
    else if (nivel == 3) {
        regrasNivel =
            "NIVEL: SUPER DESAFIO\n"
            "- Use termos tecnicos, cientificos ou culturais mais dificeis.\n"
            "- As palavras devem ter entre 6 e 14 letras.\n"
            "- Temas: engenharia, cinema classico, medicina, anatomia, fisica, biologia, astronomia e computacao.\n"
            "- As dicas devem ser mais indiretas, mas ainda justas.\n";
    }

    string prompt =
        "Voce esta gerando um arquivo de palavras para um jogo de palavras cruzadas.\n\n"
        + regrasNivel +
        "\nREGRAS GERAIS:\n"
        "- Gere exatamente 20 linhas.\n"
        "- Cada linha deve seguir o formato: palavra;dica\n"
        "- A palavra deve usar apenas letras de A a Z, sem acentos e sem cedilha.\n"
        "- A dica pode ter acentos.\n"
        "- Nao use espacos na palavra.\n"
        "- Nao use nomes proprios.\n"
        "- Nao use siglas.\n"
        "- Nao use numeros.\n"
        "- Nao repita palavras.\n"
        "- A dica nao pode conter a resposta.\n"
        "- Retorne apenas as 20 linhas.\n\n"
        "Exemplos validos para nivel medio:\n"
        "janela;abertura na parede\n"
        "relogio;aparelho que marca as horas\n\n"
        "Exemplos validos para nivel dificil:\n"
        "alavanca;maquina simples com ponto de apoio\n"
        "inercia;tendencia de manter o movimento\n"
        "organelo;estrutura funcional dentro da celula\n\n"
        "Exemplos validos para super desafio:\n"
        "entropia;medida associada a desordem\n"
        "mitocondria;organela ligada a producao de energia\n"
        "cisalhamento;deformacao causada por forcas paralelas\n\n"
        "Agora gere as 20 linhas.\n";
    
        if (!usarOpenAI)
            return consultarOllama(prompt);
        return consultarOpenAI(prompt);
}
string AssistenteIA::extrairRespostaOpenAI(const string& json) {
    string chave = "\"content\": \"";
    size_t inicio = json.find(chave);

    if (inicio == string::npos) {
        chave = "\"content\":\"";
        inicio = json.find(chave);
    }

    if (inicio == string::npos) {
        return "";
    }

    inicio += chave.length();

    size_t fim = json.find("\"", inicio);

    if (fim == string::npos) {
        return "";
    }

    return json.substr(inicio, fim - inicio);
}
