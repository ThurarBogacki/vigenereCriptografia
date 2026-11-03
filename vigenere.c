#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

void generateRandomCipher(char *cifra, int tamanho){
    for (int i = 0; i < tamanho; i++) {
        cifra[i] = 'A' + (rand() % 26);
    }
    cifra[tamanho] = '\0';
    printf("Cifra Aleatória gerada: %s\n", cifra);
}

void paraMaiusculas(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void cifrar(char *mensagem, char *cifra, char *mensagemCifrada) {
    for(int i = 0; i < strlen(mensagem); i++) {
        mensagemCifrada[i] = ((mensagem[i] - 'A') + (cifra[i] - 'A')) % 26 + 'A';
    }
    mensagemCifrada[strlen(mensagem)] = '\0';
}

void decifrar(char *mensagemCifrada, char *cifra, char *mensagemDecifrada) {
    for(int i = 0; i < strlen(mensagemCifrada); i++) {
        mensagemDecifrada[i] = ((mensagemCifrada[i] - 'A') - (cifra[i] - 'A') + 26) % 26 + 'A';
    }
    mensagemDecifrada[strlen(mensagemCifrada)] = '\0';
}

int verificaStringValida(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha((unsigned char)str[i])) {
            return 0; // String inválida
        }
    }
    return 1; // String válida
}

bool lerEntrada(char *prompt, char *entrada, int tamanho) {
    printf("%s", prompt);

    if (fgets(entrada, tamanho, stdin) == NULL) {
        return false;
    }

    if (strchr(entrada, '\n') == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    }

    entrada[strcspn(entrada, "\n")] = '\0';
    return true;
}

bool lerMensagem(char *mensagem, int tamanho) {
    while (true) {
        if (!lerEntrada("Digite a mensagem: ", mensagem, tamanho)) {
            return false;
        }

        paraMaiusculas(mensagem);

        if (verificaStringValida(mensagem)) {
            return true;
        }

        printf("Erro: A mensagem deve conter apenas letras.\n");
    }
}

bool lerCifra(char *mensagem, char *cifra, int tamanho) {
    char opcao[8];

    while (true) {
        printf("Como deseja criptografar a mensagem?\n");
        printf("1. Cifra Aleatoria\n");
        printf("2. Digitar a Cifra\n");
        printf("3. Cifra de Cesar\n");

        if (!lerEntrada("", opcao, sizeof(opcao))) {
            return false;
        }

        switch (opcao[0]) {
            case '1':
                generateRandomCipher(cifra, strlen(mensagem));
                return true;
            case '2':
                if (!lerEntrada("Digite a cifra: ", cifra, tamanho)) {
                    return false;
                }
                paraMaiusculas(cifra);
                if(!verificaStringValida(cifra)) {
                    printf("Erro: A cifra deve conter apenas letras.\n");
                }else if(strlen(cifra) != strlen(mensagem)) {
                    printf("Erro: O tamanho da cifra deve ser igual ao tamanho da mensagem.\n");
                }else{
                    printf("Cifra digitada: %s\n", cifra);
                    return true;
                }
                break;
            case '3':
                if (!lerEntrada("Digite a letra da cifra: ", cifra, tamanho)) {
                    return false;
                }
                paraMaiusculas(cifra);
                if(!verificaStringValida(cifra) || strlen(cifra) != 1) {
                    printf("Erro: A cifra deve conter apenas uma letra.\n");
                }else{
                    printf("Cifra digitada: %s\n", cifra);
                    for (int i = 0; i < strlen(mensagem); i++) {
                        cifra[i] = cifra[0];
                    }
                    cifra[strlen(mensagem)] = '\0';
                    return true;
                }
                break;
            default:
                printf("Opção inválida.\n");
        }
    }
}

int main() {
    srand(time(NULL));
    char continuar;

    do{
        char mensagem[256];
        char cifra[256];
        char mensagemCifrada[256];
        char mensagemDecifrada[256];
        char opcao[8];

        if (!lerMensagem(mensagem, sizeof(mensagem))) {
            return 0;
        }

        if (!lerCifra(mensagem, cifra, sizeof(cifra))) {
            return 0;
        }

        cifrar(mensagem, cifra, mensagemCifrada);
        decifrar(mensagemCifrada, cifra, mensagemDecifrada);

        printf("Mensagem Cifrada: %s\n", mensagemCifrada);
        printf("Mensagem Decifrada: %s\n", mensagemDecifrada);

        if (!lerEntrada("\nDeseja criptografar outra mensagem? (S/N): ", opcao, sizeof(opcao))) {
            return 0;
        }
        continuar = opcao[0];
    }while(continuar == 's' || continuar == 'S');
}