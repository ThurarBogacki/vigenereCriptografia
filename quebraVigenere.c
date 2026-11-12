#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define KEYSIZE_MINIMO 2
#define KEYSIZE_MAXIMO 40
#define TAMANHO_REPETICAO 3
#define IC_LIMITE 0.06
#define MAX_TEXTO 10000
#define MAX_DISTANCIAS 100000

double frequenciaPortugues[26] = {
    0.1463, 0.0104, 0.0388, 0.0499, 0.1257, 0.0102, 0.0130, 0.0128, 0.0618, 0.0040,
    0.0002, 0.0278, 0.0474, 0.0505, 0.1073, 0.0252, 0.0120, 0.0653, 0.0781, 0.0434,
    0.0463, 0.0167, 0.0001, 0.0021, 0.0001, 0.0047
};

int distancias[MAX_DISTANCIAS];

int lerCiphertext(char *arquivo, char *texto) {
    FILE *entrada = fopen(arquivo, "r");
    int n = 0;
    int letra;

    if(entrada == NULL) {
        return -1;
    }

    letra = fgetc(entrada);
    while(letra != EOF && n < MAX_TEXTO - 1) {
        letra = toupper(letra);
        if(letra >= 'A' && letra <= 'Z') {
            texto[n] = letra;
            n++;
        }
        letra = fgetc(entrada);
    }
    texto[n] = '\0';

    fclose(entrada);
    return n;
}

void decifrar(char *mensagemCifrada, char *chave, char *mensagemDecifrada) {
    int n = strlen(mensagemCifrada);
    int tamanhoChave = strlen(chave);

    for(int i = 0; i < n; i++) {
        int letraChave = chave[i % tamanhoChave] - 'A';
        mensagemDecifrada[i] = ((mensagemCifrada[i] - 'A') - letraChave + 26) % 26 + 'A';
    }
    mensagemDecifrada[n] = '\0';
}

void contarLetras(char *texto, int contagem[26]) {
    int n = strlen(texto);

    for(int i = 0; i < 26; i++) {
        contagem[i] = 0;
    }
    for(int i = 0; i < n; i++) {
        contagem[texto[i] - 'A']++;
    }
}

double indiceCoincidencia(char *texto) {
    int contagem[26];
    int n = strlen(texto);
    double soma = 0;

    contarLetras(texto, contagem);
    for(int i = 0; i < 26; i++) {
        soma = soma + contagem[i] * (contagem[i] - 1);
    }
    return soma / (n * (n - 1));
}

double quiQuadrado(char *texto) {
    int contagem[26];
    int n = strlen(texto);
    double soma = 0;

    contarLetras(texto, contagem);
    for(int i = 0; i < 26; i++) {
        double esperado = n * frequenciaPortugues[i];
        soma = soma + (contagem[i] - esperado) * (contagem[i] - esperado) / esperado;
    }
    return soma;
}

int ehRepeticao(char *texto, int i, int j) {
    for(int k = 0; k < TAMANHO_REPETICAO; k++) {
        if(texto[i + k] != texto[j + k]) {
            return 0;
        }
    }
    return 1;
}

int distanciasEntreRepeticoes(char *texto, int distancias[]) {
    int ultimo = strlen(texto) - TAMANHO_REPETICAO;
    int quantidade = 0;

    for(int i = 0; i <= ultimo; i++) {
        for(int j = i + 1; j <= ultimo; j++) {
            if(ehRepeticao(texto, i, j) && quantidade < MAX_DISTANCIAS) {
                distancias[quantidade] = j - i;
                quantidade++;
            }
        }
    }
    return quantidade;
}

int maiorKeysize(char *texto) {
    int metade = strlen(texto) / 2;

    if(metade < KEYSIZE_MAXIMO) {
        return metade;
    }
    return KEYSIZE_MAXIMO;
}

void contarFatores(int distancias[], int quantidade, int maior, int contagem[]) {
    for(int keysize = 0; keysize <= KEYSIZE_MAXIMO; keysize++) {
        contagem[keysize] = 0;
    }
    for(int i = 0; i < quantidade; i++) {
        for(int keysize = KEYSIZE_MINIMO; keysize <= maior; keysize++) {
            if(distancias[i] % keysize == 0) {
                contagem[keysize]++;
            }
        }
    }
}

void blocoTransposto(char *texto, int keysize, int inicio, char *bloco) {
    int n = strlen(texto);
    int tamanho = 0;

    for(int i = inicio; i < n; i = i + keysize) {
        bloco[tamanho] = texto[i];
        tamanho++;
    }
    bloco[tamanho] = '\0';
}

double indiceCoincidenciaMedio(char *texto, int keysize) {
    char bloco[MAX_TEXTO];
    double soma = 0;

    for(int inicio = 0; inicio < keysize; inicio++) {
        blocoTransposto(texto, keysize, inicio, bloco);
        soma = soma + indiceCoincidencia(bloco);
    }
    return soma / keysize;
}

int ehCandidato(int keysize, int contagem[]) {
    int maximo = 0;

    for(int i = KEYSIZE_MINIMO; i <= KEYSIZE_MAXIMO; i++) {
        if(contagem[i] > maximo) {
            maximo = contagem[i];
        }
    }
    return 2 * contagem[keysize] >= maximo;
}

int escolherKeysize(char *texto, int contagem[], int maior) {
    int melhor = -1;
    double melhorIc = 0;

    for(int keysize = KEYSIZE_MINIMO; keysize <= maior; keysize++) {
        if(ehCandidato(keysize, contagem)) {
            double ic = indiceCoincidenciaMedio(texto, keysize);

            if(ic >= IC_LIMITE) {
                return keysize;
            }
            if(melhor == -1 || ic > melhorIc) {
                melhor = keysize;
                melhorIc = ic;
            }
        }
    }
    return melhor;
}

char quebrarBloco(char *bloco) {
    char texto[MAX_TEXTO];
    char chave[2];
    char melhorChave = 'A';
    double melhorQui = -1;

    chave[1] = '\0';
    for(int numero = 0; numero < 26; numero++) {
        chave[0] = 'A' + numero;
        decifrar(bloco, chave, texto);

        double qui = quiQuadrado(texto);

        if(melhorQui < 0 || qui < melhorQui) {
            melhorChave = chave[0];
            melhorQui = qui;
        }
    }
    return melhorChave;
}

void encontrarChave(char *texto, int keysize, char *chave) {
    char bloco[MAX_TEXTO];

    for(int inicio = 0; inicio < keysize; inicio++) {
        blocoTransposto(texto, keysize, inicio, bloco);
        chave[inicio] = quebrarBloco(bloco);
    }
    chave[keysize] = '\0';
}

void mostrarTop5(char *texto, int contagem[], int maior) {
    int jaMostrado[KEYSIZE_MAXIMO + 1] = {0};

    printf("\nKasiski (mais votos = melhor):\n");
    for(int posicao = 1; posicao <= 5; posicao++) {
        int melhor = -1;

        for(int keysize = KEYSIZE_MINIMO; keysize <= maior; keysize++) {
            if(jaMostrado[keysize]) {
                continue;
            }
            if(melhor == -1 || contagem[keysize] > contagem[melhor]) {
                melhor = keysize;
            }
        }

        if(melhor == -1) {
            return;
        }

        jaMostrado[melhor] = 1;
        printf("%d. KEYSIZE %2d - votos %6d - IC médio %.4f\n", posicao, melhor, contagem[melhor], indiceCoincidenciaMedio(texto, melhor));
    }
}

int main() {
    char ciphertext[MAX_TEXTO];
    char mensagemDecifrada[MAX_TEXTO];
    char chave[KEYSIZE_MAXIMO + 1];
    int contagem[KEYSIZE_MAXIMO + 1];

    int n = lerCiphertext("ciphertext.txt", ciphertext);

    if(n == -1) {
        printf("Erro: não foi possível abrir o arquivo ciphertext.txt.\n");
        return 1;
    }
    if(n < 2 * KEYSIZE_MINIMO) {
        printf("Erro: o ciphertext deve ter pelo menos %d letras.\n", 2 * KEYSIZE_MINIMO);
        return 1;
    }

    printf("Ciphertext: %d letras\n", n);
    printf("IC do ciphertext: %.4f (português: 0.0781, aleatório: 0.0385)\n", indiceCoincidencia(ciphertext));

    int maior = maiorKeysize(ciphertext);
    int quantidade = distanciasEntreRepeticoes(ciphertext, distancias);

    contarFatores(distancias, quantidade, maior, contagem);
    printf("Repetições de %d letras encontradas: %d\n", TAMANHO_REPETICAO, quantidade);
    mostrarTop5(ciphertext, contagem, maior);

    int keysize = escolherKeysize(ciphertext, contagem, maior);

    encontrarChave(ciphertext, keysize, chave);
    decifrar(ciphertext, chave, mensagemDecifrada);

    printf("\nKEYSIZE: %d\n", keysize);
    printf("Chave: %s\n", chave);
    printf("\nMensagem decifrada:\n%s\n", mensagemDecifrada);

    return 0;
}
