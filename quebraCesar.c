#include <stdio.h>
#include <string.h>

double frequenciaPortugues[26] = {
    0.1463, 0.0104, 0.0388, 0.0499, 0.1257, 0.0102, 0.0130, 0.0128, 0.0618, 0.0040,
    0.0002, 0.0278, 0.0474, 0.0505, 0.1073, 0.0252, 0.0120, 0.0653, 0.0781, 0.0434,
    0.0463, 0.0167, 0.0001, 0.0021, 0.0001, 0.0047
};

void decifrar(char *mensagemCifrada, int chave, char *mensagemDecifrada) {
    for(int i = 0; i < strlen(mensagemCifrada); i++) {
        mensagemDecifrada[i] = ((mensagemCifrada[i] - 'A') - chave + 26) % 26 + 'A';
    }
    mensagemDecifrada[strlen(mensagemCifrada)] = '\0';
}

void contarLetras(char *texto, int contagem[26]) {
    for(int i = 0; i < 26; i++) {
        contagem[i] = 0;
    }
    for(int i = 0; i < strlen(texto); i++) {
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

double indiceCoincidenciaMutuo(char *texto) {
    int contagem[26];
    int n = strlen(texto);
    double soma = 0;

    contarLetras(texto, contagem);
    for(int i = 0; i < 26; i++) {
        soma = soma + ((double)contagem[i] / n) * frequenciaPortugues[i];
    }
    return soma;
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

void mostrarTop5(double score[26], char textos[26][256], int maiorEhMelhor) {
    int jaMostrado[26] = {0};

    for(int posicao = 1; posicao <= 5; posicao++) {
        int melhor = -1;

        for(int chave = 0; chave < 26; chave++) {
            if(jaMostrado[chave]) {
                continue;
            }
            if(melhor == -1) {
                melhor = chave;
            }else if(maiorEhMelhor && score[chave] > score[melhor]) {
                melhor = chave;
            }else if(!maiorEhMelhor && score[chave] < score[melhor]) {
                melhor = chave;
            }
        }

        jaMostrado[melhor] = 1;
        printf("%d. chave %c - score %.4f - %s\n", posicao, 'A' + melhor, score[melhor], textos[melhor]);
    }
}

int main() {
    char ciphertext[] = "MXOXYBKPSLZBZLKPBDRFRABZFCOXOXJBKPXDBJ";
    char textos[26][256];
    double icm[26];
    double qui[26];

    printf("Ciphertext: %s\n", ciphertext);
    printf("IC do ciphertext: %.4f (português: 0.0781, aleatório: 0.0385)\n", indiceCoincidencia(ciphertext));

    for(int chave = 0; chave < 26; chave++) {
        decifrar(ciphertext, chave, textos[chave]);
        icm[chave] = indiceCoincidenciaMutuo(textos[chave]);
        qui[chave] = quiQuadrado(textos[chave]);
    }

    printf("\nÍndice de coincidência mútuo (maior = melhor):\n");
    mostrarTop5(icm, textos, 1);

    printf("\nQui-quadrado (menor = melhor):\n");
    mostrarTop5(qui, textos, 0);

    return 0;
}
