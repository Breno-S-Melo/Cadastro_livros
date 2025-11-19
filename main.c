#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct {
    char titulo[50];
    char genero[50];
    int data;
    float preco;
}reg;

//prototipo das funcoes
void limpa_buffer(void);
void ler_string(char *buffer, int tam);
int tamanho(FILE *arq);
void cadastrar (FILE *arq);
void consultar (FILE *arq);
void gerar_arquivo_txt(FILE *arq);
void exluir(FILE *arq);

void cadastrar(FILE *arq){
    reg livro;
    printf("---Cadastrar livro---\n");
    printf("Registro número: %d\n", tamanho(arq) + 1);
    tamanho(arq) +1;
    
    //ler string
    printf("Nome do livro: ");
    ler_string(livro.titulo, sizeof(livro.titulo));
    
    printf("Gênero do livro: ");
    ler_string(livro.genero, sizeof(livro.genero));
    
    printf("Ano de lançamento do livro: ");
    scanf("%d", &livro.data);
    limpa_buffer();
    
    printf("Preço do livro:");
    scanf("%f", &livro.preco);
    limpa_buffer();
    
    fseek(arq, 0, SEEK_END);
    fwrite(&livro, sizeof(reg), 1, arq);
    
    printf("Livro cadastrado.");
}

void ler_string(char *buffer, int tam) {
    fgets(buffer,tam,stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

void limpa_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int tamanho(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    long bytes = ftell(arq);
    return bytes / sizeof(reg);
}

void consultar(FILE*arq) {
    int indice;
    int total = tamanho(arq);
    printf("\n--- CONSULTAR LIVRO ---\n");
    if (total == 0) {
        printf("Nenhum livro cadastrado.\n");
        return;
    }
    printf("Existem %d livros .\n", total);
    printf("Digite o registro a ser consultado (índice começa em 0): ");
    scanf("%d", &indice);
    limpa_buffer();
    if (indice >= 0 && indice < total) {
        reg livro_lido;
        
        fseek(arq, indice * sizeof(reg), SEEK_SET);
        
        fread(&livro_lido, sizeof(reg), 1, arq);

        printf("\n--- DADOS DO REGISTRO %d ---\n", indice);
        printf("Titulo: %s\n", livro_lido.titulo);
        printf("Genero: %s\n", livro_lido.genero);
        printf("Ano: %d\n", livro_lido.data);
        printf("Preco: R$ %.2f\n", livro_lido.preco);
    } else {
        printf("Erro: Registro nao encontrado!\n");
    }
}

void gerar_arquivo_txt(FILE *arq) {
    char nome_txt[] = "relatorio_livros.txt";
    FILE *txt = fopen(nome_txt, "w"); // Abre/Cria arquivo de texto

    if (txt == NULL) {
        printf("Erro ao criar o arquivo TXT.\n");
        return;
    }

    int total = tamanho(arq);
    reg l;
    
    fprintf(txt, "--- RELATORIO DE LIVROS ---\n");
    fprintf(txt, "Total de registros: %d\n\n", total);

    fseek(arq, 0, SEEK_SET); 
    for (int i = 0; i < total; i++) {
        fread(&l, sizeof(reg), 1, arq);
        fprintf(txt, "Registro %d:\n", i);
        fprintf(txt, "Titulo: %s\n", l.titulo);
        fprintf(txt, "Genero: %s\n", l.genero);
        fprintf(txt, "Ano: %d\n", l.data);
        fprintf(txt, "Preco: R$ %.2f\n", l.preco);
        fprintf(txt, "-------------------------\n");
    }

    fclose(txt);
    printf("\nArquivo '%s' gerado com sucesso!\n", nome_txt);
}

int main()
{
    setlocale(LC_ALL, "Portuguese");
    
    FILE *arquivo = fopen("livros.bin", "r+b");
    if(arquivo == NULL) {
        arquivo = fopen("livros.bin", "w+b");
        if(arquivo ==NULL){
            printf("Erro ao abrir arquivo.\n");
            return 1;
        }
    }
    
    int op;
    do{
        printf("-----MENU-----\n\n");
        printf("1. Cadastrar Livro\n");
        printf("2. Consultar\n");
        printf("3. Consultar total de livros.\n");
        printf("4. Gerar relatório.\n");
        printf("0. Sair\n\n");
        printf("Opção:");
        scanf("%d", &op);
        limpa_buffer();
        
        switch(op) {
            case 1:
                cadastrar(arquivo);
                break;
            case 2:
                consultar(arquivo);
                break;
            case 3:
                printf("\nTotal de registros no arquivo: %d\n", tamanho(arquivo));
                break;
            case 4:
                gerar_arquivo_txt(arquivo);
                break;
            case 0:
                printf("Finalizando.");
                break;
            default:
                printf("Opção inválida\n");
        }
    }while (op != 0);
    
    fclose(arquivo);
    return 0;
}