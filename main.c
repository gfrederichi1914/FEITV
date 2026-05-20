#include <stdio.h> // funções de entrada e saída
#include <stdlib.h> // memória e conversão
#include <string.h> // funções string

#define MAX_STR 100 // tamanho máximo de texto
#define MAX_VIDEOS_FAVORITOS 50	// máximo de vídeos por favoritos

// STRUCTS ==========================================================================================================================

// armazena os dados de cada usuario cadastrado
typedef struct {
	int id;
	char login[MAX_STR]; // utiliza-se 'um array de char
	char senha[MAX_STR]; // MAX_STR como parâmetro para o número de espaços no array
	char nome [MAX_STR];
} Usuario; // typedef para criar um apelido para nossa struct

// armazena os dados de cada video registrado
typedef struct {
	int id;
	char titulo[MAX_STR];
	char descricao[MAX_STR];
	char genero[MAX_STR];
	int ano;
	int curtidas;
} Video;

// armazena os dados de cada lista criada
typedef struct {
	int id;
	char nome[MAX_STR];
	char loginDono[MAX_STR];
	int idsVideos[MAX_VIDEOS_FAVORITOS];
	int totalVideos;
} Favorito;

// ARQUIVOS =========================================================================================================================

// salva um usuario no arquivo
 void salvarUsuario(Usuario u) {
 	FILE *f = fopen("usuarios.txt", "a"); // abre o arquivo
 	if (f == NULL) { // se não conseguiu abrir, desiste
 		return;
	 }
 	fprintf(f, "%d|%s|%s|%s\n", u.id, u.login, u.senha, u.nome); // escreve no arquivo os atributos do usuario 
 	fclose(f);
 }
 
 // lê o arquivo e carrega todos os usuários na memória, retorna o array e a quantidade por ponteiro total
 Usuario* carregarUsuarios(int *total){
 	FILE *f = fopen("usuarios.txt", "r"); // abre o arquivo no modo read
 	*total = 0; // total = guarda a quantidade de usuarios no array
 	if (f == NULL) {
		return NULL;
	}
	
	Usuario *lista = NULL; // aponta para uma lista vazia para poder alocar do zero
	char linha[400];
	
	while (fgets(linha, sizeof(linha), f)){ // lê uma linha por vez | sizeof retorna o tamanho em bytes
		lista = realloc(lista, (*total + 1) * sizeof(Usuario)); // realloc aumenta o array sob demanda | (quant de usuarios) * (bytes por usuario) = bytes necessário
		Usuario *u = &lista[*total]; // atalho para escrever diretamente no novo espaço do array | lista[*total] = ultimo espaço do array
		sscanf(linha, "%d|%99[^|]|%99[^|]|%99[^\n]", // lê os campos separados por |
			&u->id, u->login, u->senha, u->nome); // int id é o unico q não é naturalmente um ponteiro (o resto são arrays)
		(*total)++; // incrementa
	}
	
	fclose(f);
	return lista;
 }
 
 // reescreve todos os usuarios, usado para editar algo no meio da lista ou deletando algo
 void salvarTodosUsuarios(Usuario *lista, int total){
 	FILE *f = fopen("usuarios.txt", "w"); // funcao 'w' apaga tudo e escreve dnv
 	if (f == NULL){
 		return;
	 }
	 int i;
	 for (i = 0; i < total; i++){ // passa linha por linha
	 	fprintf(f, "%d|%s|%s|%s\n",
		 	lista[i].id, lista[i].login, lista[i].senha, lista[i].nome);
	 }
	 fclose(f);
 }
 
 // salva um video no arquivo
 void salvarVideo(Video v){
 	FILE *f = fopen("videos.txt", "a");
 	if (f == NULL) {
 		return;
	 }
	fprintf(f, "%d|%s|%s|%s|%d|%d\n",
        v.id, v.titulo, v.descricao, v.genero, v.ano, v.curtidas);
    fclose(f);
 }
 
 // lê o arquivo e carrega todos os vídeos na memória, retorna o array e a quantidade por ponteiro total
 Video* carregarVideos(int *total){
 	FILE *f = fopen("videos.txt", "r");
 	*total = 0;
 	if (f == NULL){
 		return;
	}
	
	Video *lista = NULL;
	char linha[400];
	
	while (fgets(linha, sizeof(linha), f)){
		lista = realloc(lista, (*total + 1) * sizeof(Video));
        Video *v = &lista[*total];
        sscanf(linha, "%d|%99[^|]|%99[^|]|%99[^|]|%d|%d",
            &v->id, v->titulo, v->descricao, v->genero, &v->ano, &v->curtidas);
        (*total)++;
	}
	
	fclose(f);
	return lista;
 }
 
 // reescreve todos os video, usado para editar algo no meio da lista ou deletando algo
 void salvarTodosVideos(Video *lista, int total) {
    FILE *f = fopen("videos.txt", "w");
    if (f == NULL) {
    	return;
	}
	int i;
    for (i = 0; i < total; i++) {
        fprintf(f, "%d|%s|%s|%s|%d|%d\n",
            lista[i].id, lista[i].titulo, lista[i].descricao,
            lista[i].genero, lista[i].ano, lista[i].curtidas);
    }
    fclose(f);
}

// salva uma lista no arquivo
void salvarFavorito(Favorito fav) {
    FILE *f = fopen("favoritos.txt", "a");
    if (f == NULL) {
    	return;
	}
    fprintf(f, "%d|%s|%s|%d", 
		fav.id, fav.nome, fav.loginDono, fav.totalVideos);
	int i;
    for (i = 0; i < fav.totalVideos; i++) {
        fprintf(f, "|%d", fav.idsVideos[i]); // adiciona cada id de video na linha
    }
    fprintf(f, "\n"); // termina linha
    fclose(f);
}

// lê o arquivo e carrega todas as listas na memória, retorna o array e a quantidade por ponteiro total
Favorito* carregarFavoritos(int *total) {
    FILE *f = fopen("favoritos.txt", "r");
    *total = 0;
    if (f == NULL) {
    	return;
	}

    Favorito *lista = NULL;
    char linha[1000];

    while (fgets(linha, sizeof(linha), f)) {
        lista = realloc(lista, (*total + 1) * sizeof(Favorito));
        Favorito *fav = &lista[*total];
        fav->totalVideos = 0;

        char *token = strtok(linha, "|"); // pega o primeiro pedaço ate o |
        fav->id = atoi(token); // converte o string '1' para int 1

        token = strtok(NULL, "|"); // pega o próximo pedaço
        strncpy(fav->nome, token, MAX_STR); // copia para struct

        token = strtok(NULL, "|");
        strncpy(fav->loginDono, token, MAX_STR); // strncpy copia uma string para outra com limite

        token = strtok(NULL, "|");
        fav->totalVideos = atoi(token); // sabe quantos ids vai ler
        
		int i;
        for (i = 0; i < fav->totalVideos; i++) { // percorre a quantidade de videos q tem na lista
            token = strtok(NULL, "|\n"); // le cada id de vídeo
            if (token) fav->idsVideos[i] = atoi(token);
        }

        (*total)++;
    }

    fclose(f);
    return lista;
}

// reescreve todas as listas, usado para editar algo no meio da lista ou deletando algo
void salvarTodosFavoritos(Favorito *lista, int total) {
    FILE *f = fopen("favoritos.txt", "w");
    if (f == NULL) {
    	return;
	}
	int i;
	int j;
    for (i = 0; i < total; i++) {
        fprintf(f, "%d|%s|%s|%d", lista[i].id, lista[i].nome,
                lista[i].loginDono, lista[i].totalVideos);
        for (j = 0; j < lista[i].totalVideos; j++) {
            fprintf(f, "|%d", lista[i].idsVideos[j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}
 
// USUARIOS =========================================================================================================================

// gera um id para o usuario
int gerarIdUsuario() {
    int total;
    Usuario *lista = carregarUsuarios(&total); // carrega todos os usuarios no arquivo para atualizar o total
    int id = total + 1; // id do usuario anterior +1
    free(lista); // libera a memoria alocada
    return id;
}

// verifica se o login ja existe
int loginExiste(char *login) {
    int total;
    Usuario *lista = carregarUsuarios(&total); // atualiza usuarios
    int i;
    for (i = 0; i < total; i++) {
        if (strcmp(lista[i].login, login) == 0) { // percorre a lista comparando com strcmp, se for igual retorna 0
            free(lista);
            return 1; // login ja existe
        }
    }
    free(lista);
    return 0; // login disponivel
}

// cria um novo usuario
void cadastrarUsuario() {
    Usuario u;
    printf("\n=== CADASTRO ===\n");

    printf("Nome: ");
    fgets(u.nome, MAX_STR, stdin); // fgets armazena uma string
    u.nome[strcspn(u.nome, "\n")] = '\0'; // remove o \n do final

    printf("Login: ");
    fgets(u.login, MAX_STR, stdin);
    u.login[strcspn(u.login, "\n")] = '\0';

    if (loginExiste(u.login)) {
        printf("Login ja cadastrado!\n");
        return;
    }

    printf("Senha: ");
    fgets(u.senha, MAX_STR, stdin);
    u.senha[strcspn(u.senha, "\n")] = '\0';

    u.id = gerarIdUsuario();
    salvarUsuario(u); // salvar o usuario
    printf("Usuario cadastrado com sucesso!\n");
}


Usuario* fazerLogin(Usuario *usuarioLogado) {
    char login[MAX_STR], senha[MAX_STR];
    printf("\n=== LOGIN ===\n");

    printf("Login: ");
    fgets(login, MAX_STR, stdin);
    login[strcspn(login, "\n")] = '\0'; // substitui o \n pelo fim da string \0

    printf("Senha: ");
    fgets(senha, MAX_STR, stdin);
    senha[strcspn(senha, "\n")] = '\0';

    int total;
    Usuario *lista = carregarUsuarios(&total); // atualiza usuarios

	int i;
    for (i = 0; i < total; i++) { // verifica toda a lista de usuarios, se o login existe
        if (strcmp(lista[i].login, login) == 0 &&
            strcmp(lista[i].senha, senha) == 0) {
            *usuarioLogado = lista[i];
            free(lista);
            printf("Bem-vindo, %s!\n", usuarioLogado->nome);
            return usuarioLogado;
        }
    }

    free(lista);
    printf("Login ou senha incorretos!\n");
    return NULL;
}

// VIDEOS =============================================================================================================================

// gera id para cada vídeo
int gerarIdVideo() {
    int total;
    Video *lista = carregarVideos(&total);
    int id = total + 1;
    free(lista);
    return id;
}

// lista todos os vídeos
void listarVideos() {
    int total;
    Video *lista = carregarVideos(&total); // atualiza todos os videos

    if (total == 0) {
        printf("Nenhum video cadastrado!\n");
        free(lista);
        return;
    }

    printf("\n=== VIDEOS ===\n");
    int i;
    for (i = 0; i < total; i++) {
        printf("ID: %d\n", lista[i].id);
        printf("Titulo: %s\n", lista[i].titulo);
        printf("Descricao: %s\n", lista[i].descricao);
        printf("Genero: %s\n", lista[i].genero);
        printf("Ano: %d\n", lista[i].ano);
        printf("Curtidas: %d\n", lista[i].curtidas);
        printf("-------------------\n");
    }

    free(lista);
}

// busca video por nome
void buscarVideoPorNome() {
    char busca[MAX_STR];
    printf("\nDigite o nome do video: ");
    fgets(busca, MAX_STR, stdin);
    busca[strcspn(busca, "\n")] = '\0';

    int total;
    Video *lista = carregarVideos(&total);
    int achou = 0;

    printf("\n=== RESULTADOS ===\n");
    int i;
    for (i = 0; i < total; i++) {
        if (strstr(lista[i].titulo, busca) != NULL) { // strstr busca uma string dentro de outra, retorna o ponteiro, se NAO for nulo ela exibe o video
            printf("ID: %d\n", lista[i].id);
            printf("Titulo: %s\n", lista[i].titulo);
            printf("Descricao: %s\n", lista[i].descricao);
            printf("Genero: %s\n", lista[i].genero);
            printf("Ano: %d\n", lista[i].ano);
            printf("Curtidas: %d\n", lista[i].curtidas);
            printf("-------------------\n");
            achou = 1;
        }
    }

    if (!achou) printf("Nenhum video encontrado!\n");
    free(lista);
}

// curte e descurte videos
void curtirDescurtirVideo(Usuario *u) {
    int idVideo;
    printf("\nDigite o ID do video: ");
    scanf("%d", &idVideo);
    getchar(); // limpa o \n do buffer

    int total;
    Video *lista = carregarVideos(&total);

	int i;
    for (i = 0; i < total; i++) {
        if (lista[i].id == idVideo) {
            // verifica se ja curtiu
            FILE *f = fopen("curtidas.txt", "r");
            char linha[200];
            int jaCurtiu = 0;
            int linhaCurtida = -1;
            int totalLinhas = 0;

            if (f != NULL) {
                while (fgets(linha, sizeof(linha), f)) { // verifica se o usuario ja curtiu esse video percorrendo o curtidas.txt
                    int idU, idV;
                    sscanf(linha, "%d|%d", &idU, &idV); // le o id do usuario e do video
                    if (idU == u->id && idV == idVideo) {
                        jaCurtiu = 1;
                        linhaCurtida = totalLinhas; // guarda qual linha precisa remover
                    }
                    totalLinhas++;
                }
                fclose(f);
            }

            if (jaCurtiu) {
                // descurtir: remove a linha do arquivo
                lista[i].curtidas--;
                salvarTodosVideos(lista, total);

                // nao da pra apagar linha direto em C, usa arquivo temporario
				// copia tudo para o temp pulando a linha da curtida
				// deleta o original e renomeia o temp
                FILE *fc = fopen("curtidas.txt", "r");
                FILE *ft = fopen("curtidas_temp.txt", "w");
                int linha_atual = 0;
                while (fgets(linha, sizeof(linha), fc)) {
                    if (linha_atual != linhaCurtida) {
                        fprintf(ft, "%s", linha);
                    }
                    linha_atual++;
                }
                fclose(fc);
                fclose(ft);
                remove("curtidas.txt");
                rename("curtidas_temp.txt", "curtidas.txt");

                printf("Video descurtido!\n");
            } else {
                // curtir
                lista[i].curtidas++;
                salvarTodosVideos(lista, total);

                FILE *fc = fopen("curtidas.txt", "a");
                fprintf(fc, "%d|%d\n", u->id, idVideo);
                fclose(fc);

                printf("Video curtido!\n");
            }

            free(lista);
            return;
        }
    }

    printf("Video nao encontrado!\n");
    free(lista);
}

// FAVORITOS ==============================================================================================================================

// gera id para cada favorito
int gerarIdFavorito() {
    FILE *f = fopen("favoritos.txt", "r");
    if (f == NULL) {
		return 1;
	} // arquivo nao existe ainda, primeiro id é 1
    fclose(f);

    int total;
    Favorito *lista = carregarFavoritos(&total); // atualiza lista de favoritos
    int id = total + 1;
    free(lista); // libera espaço na memoria
    return id;
}

// cria uma lista
void criarFavorito(Usuario *u) {
    Favorito fav;
    fav.id = gerarIdFavorito();
    fav.totalVideos = 0;
    strncpy(fav.loginDono, u->login, MAX_STR); // copia para o login do dono o login do usuario q criou a lista

    printf("\nNome da lista: ");
    fgets(fav.nome, MAX_STR, stdin);
    fav.nome[strcspn(fav.nome, "\n")] = '\0';

    salvarFavorito(fav);
    printf("Lista criada com sucesso!\n");
}

// lista todas as listas do usuario
void listarFavoritosDoUsuario(Usuario *u) {
    int total;
    Favorito *lista = carregarFavoritos(&total);
    int achou = 0;

    printf("\n=== SUAS LISTAS ===\n");
    int i;
    for (i = 0; i < total; i++) {
        if (strcmp(lista[i].loginDono, u->login) == 0) {
            printf("ID: %d | Nome: %s | Videos: %d\n",
                   lista[i].id, lista[i].nome, lista[i].totalVideos);
            achou = 1;
        }
    }

    if (!achou) printf("Voce nao tem listas criadas!\n");
    free(lista);
}

// edita o nome da lista
void editarFavorito(Usuario *u) {
    listarFavoritosDoUsuario(u);

    int idFav;
    printf("\nDigite o ID da lista para editar: ");
    scanf("%d", &idFav);
    getchar();

    int total;
    Favorito *lista = carregarFavoritos(&total);

	int i;
    for (i = 0; i < total; i++) {
        if (lista[i].id == idFav && strcmp(lista[i].loginDono, u->login) == 0) {
            printf("Novo nome da lista: ");
            fgets(lista[i].nome, MAX_STR, stdin);
            lista[i].nome[strcspn(lista[i].nome, "\n")] = '\0';

            salvarTodosFavoritos(lista, total);
            printf("Lista editada com sucesso!\n");
            free(lista);
            return;
        }
    }

    printf("Lista nao encontrada!\n");
    free(lista);
}

// exclue a lista
void excluirFavorito(Usuario *u) {
    listarFavoritosDoUsuario(u);

    int idFav;
    printf("\nDigite o ID da lista para excluir: ");
    scanf("%d", &idFav);
    getchar();

    int total;
    Favorito *lista = carregarFavoritos(&total);
    int achou = 0;

	int i;
	int j;
    for (i = 0; i < total; i++) {
        if (lista[i].id == idFav && strcmp(lista[i].loginDono, u->login) == 0) {
            // move todos os elementos uma posicao pra tras
            for (j = i; j < total - 1; j++) {
                lista[j] = lista[j + 1]; // move elementos para preencher o espaco do excluido
            }
            total--;
            achou = 1;
            break;
        }
    }

    if (achou) {
        salvarTodosFavoritos(lista, total);
        printf("Lista excluida com sucesso!\n");
    } else {
        printf("Lista nao encontrada!\n");
    }

    free(lista);
}

// adiciona video a lista
void adicionarVideoFavorito(Usuario *u) {
    listarFavoritosDoUsuario(u);

    int idFav;
    printf("\nDigite o ID da lista: ");
    scanf("%d", &idFav);
    getchar();

    listarVideos();

    int idVideo;
    printf("\nDigite o ID do video: ");
    scanf("%d", &idVideo);
    getchar();

    int total;
    Favorito *lista = carregarFavoritos(&total);

	int i;
	int j;
    for (i = 0; i < total; i++) {
        if (lista[i].id == idFav && strcmp(lista[i].loginDono, u->login) == 0) {
            if (lista[i].totalVideos >= MAX_VIDEOS_FAVORITOS) {
                printf("Lista cheia!\n");
                free(lista);
                return;
            }
            for (j = 0; j < lista[i].totalVideos; j++) {
                if (lista[i].idsVideos[j] == idVideo) { // verifica se video ja esta na lista
                    printf("Video ja esta na lista!\n");
                    free(lista);
                    return;
                }
            }
            lista[i].idsVideos[lista[i].totalVideos] = idVideo;
            lista[i].totalVideos++;
            salvarTodosFavoritos(lista, total);
            printf("Video adicionado!\n");
            free(lista);
            return;
        }
    }

    printf("Lista nao encontrada!\n");
    free(lista);
}

// remove video da lista
void removerVideoFavorito(Usuario *u) {
    listarFavoritosDoUsuario(u);

    int idFav;
    printf("\nDigite o ID da lista: ");
    scanf("%d", &idFav);
    getchar();

    int total;
    Favorito *lista = carregarFavoritos(&total);

	int i;
	int j;
	int k;
    for (i = 0; i < total; i++) {
        if (lista[i].id == idFav && strcmp(lista[i].loginDono, u->login) == 0) {
            printf("\nVideos na lista:\n");
            for (j = 0; j < lista[i].totalVideos; j++) {
                printf("ID: %d\n", lista[i].idsVideos[j]);
            }

            int idVideo;
            printf("\nDigite o ID do video para remover: ");
            scanf("%d", &idVideo);
            getchar();

            for (j = 0; j < lista[i].totalVideos; j++) {
                if (lista[i].idsVideos[j] == idVideo) {
                    for (k = j; k < lista[i].totalVideos - 1; k++) {
                        lista[i].idsVideos[k] = lista[i].idsVideos[k + 1]; // move elementos para preencher o espaco do removido
                    }
                    lista[i].totalVideos--;
                    salvarTodosFavoritos(lista, total);
                    printf("Video removido!\n");
                    free(lista);
                    return;
                }
            }
            printf("Video nao encontrado na lista!\n");
            free(lista);
            return;
        }
    }

    printf("Lista nao encontrada!\n");
    free(lista);
}

// MENUS =================================================================================================================================

void menuFavoritos(Usuario *u) {
    int opcao;
    do {
        printf("\n=== FAVORITOS ===\n");
        printf("1. Criar lista\n");
        printf("2. Editar lista\n");
        printf("3. Excluir lista\n");
        printf("4. Adicionar video a lista\n");
        printf("5. Remover video da lista\n");
        printf("6. Ver minhas listas\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: criarFavorito(u); break;
            case 2: editarFavorito(u); break;
            case 3: excluirFavorito(u); break;
            case 4: adicionarVideoFavorito(u); break;
            case 5: removerVideoFavorito(u); break;
            case 6: listarFavoritosDoUsuario(u); break;
            case 0: break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

void menuUsuario(Usuario *u) {
    int opcao;
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("Logado como: %s\n", u->nome);
        printf("1. Buscar video por nome\n");
        printf("2. Listar todos os videos\n");
        printf("3. Curtir/Descurtir video\n");
        printf("4. Gerenciar favoritos\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: buscarVideoPorNome(); break;
            case 2: listarVideos(); break;
            case 3: curtirDescurtirVideo(u); break;
            case 4: menuFavoritos(u); break;
            case 0: printf("Ate logo, %s!\n", u->nome); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

int main() {
    int opcao;
    do {
        printf("\n=== FEItv ===\n");
        printf("1. Cadastrar\n");
        printf("2. Login\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                cadastrarUsuario();
                break;
            case 2: {
                Usuario u;
                if (fazerLogin(&u) != NULL) {
                    menuUsuario(&u);
                }
                break;
            }
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}

