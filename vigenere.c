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
int main() {
    srand(time(NULL));
    char continuar = 's';

    do{
        char mensagem[256];
        char cifra[256];
        char mensagemCifrada[256];
        char mensagemDecifrada[256];
        int chaveValida = 0;
        printf("Digite a mensagem: ");
        fgets(mensagem, sizeof(mensagem), stdin);

        mensagem[strcspn(mensagem, "\n")] = '\0';

        paraMaiusculas(mensagem);

        if(!verificaStringValida(mensagem)) {
            printf("Erro: A mensagem deve conter apenas letras.\n");
            continue;
        }

        do{
            printf("Como deseja criptografar a mensagem?\n");
            printf("1. Cifra Aleatoria\n");
            printf("2. Digitar a Cifra\n");

            switch (getchar()) {
                case '1':

                    while (getchar() != '\n');

                    printf("Cifra Aleatória selecionada.\n");
                
                    generateRandomCipher(cifra, strlen(mensagem));
                    chaveValida = 1;
                    break;
                case '2':

                    while (getchar() != '\n');

                    printf("Digite a cifra: ");

                    fgets(cifra, sizeof(cifra), stdin);
                    cifra[strcspn(cifra, "\n")] = '\0';
                    paraMaiusculas(cifra);
                    if(!verificaStringValida(cifra)) {
                        printf("Erro: A cifra deve conter apenas letras.\n");
                        continue;
                    }else if(strlen(cifra) != strlen(mensagem)) {
                        printf("Erro: O tamanho da cifra deve ser igual ao tamanho da mensagem.\n");
                        continue;
                    }else{
                        printf("Cifra digitada: %s\n", cifra);
                        // Implementação da criptografia com a cifra digitada aqui
                        chaveValida = 1;
                    }
                    break;
                    default:
                        while (getchar() != '\n');
                        printf("Opção inválida.\n");
                }

        } while (!chaveValida);    

        cifrar(mensagem, cifra, mensagemCifrada);
        decifrar(mensagemCifrada, cifra, mensagemDecifrada);

        printf("Mensagem Cifrada: %s\n", mensagemCifrada);
        printf("Mensagem Decifrada: %s\n", mensagemDecifrada);

        printf("\nDeseja criptografar outra mensagem? (S/N): ");
        continuar = getchar();
        while (getchar() != '\n');
    }while(continuar == 's' || continuar == 'S');
}