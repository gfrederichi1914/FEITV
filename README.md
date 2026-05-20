# FEItv 🎬

Plataforma de informações de vídeos desenvolvida em Linguagem C como projeto final da disciplina CCP230 na FEI.

---

## Sobre o Projeto

O FEItv permite que usuários se cadastrem, façam login, busquem filmes e séries, curtam títulos e gerenciem listas de favoritos. Todos os dados são persistidos em arquivos de texto, garantindo que nenhuma informação seja perdida ao encerrar o programa.

---

## Funcionalidades

- Cadastro e login de usuário
- Busca de vídeo por nome (busca parcial)
- Listagem de todos os vídeos
- Curtir e descurtir vídeos
- Criar, editar e excluir listas de favoritos
- Adicionar e remover vídeos das listas

---

## Tecnologias

- Linguagem C (C99)
- Arquivos de texto (.txt) para persistência de dados
- Dev-C++ 5.11 com compilador TDM-GCC 4.9.2

---

## Estrutura dos Arquivos
feitv/
├── main.c
├── videos.txt       # catalogo de videos (preenchido manualmente)
├── usuarios.txt     # usuarios cadastrados
├── curtidas.txt     # registro de curtidas
└── favoritos.txt    # listas de favoritos

### Formato dos arquivos

**usuarios.txt**
id|login|senha|nome
1|gabriel|senha123|Gabriel Chicole

**videos.txt**
id|titulo|descricao|genero|ano|curtidas
1|Inception|Um ladrao que rouba segredos dos sonhos|Ficcao Cientifica|2010|0

**curtidas.txt**
idUsuario|idVideo
1|3

**favoritos.txt**
id|nomeLista|loginDono|totalVideos|idVideo1|idVideo2|...
1|Para maratonar|gabriel|3|1|5|8

---

## Como Executar

1. Clone ou baixe o repositório
2. Abra o projeto no Dev-C++ ou qualquer compilador C
3. Certifique-se de que o arquivo `videos.txt` está na mesma pasta do executável
4. Compile e execute o `main.c`

---

## Pontos Técnicos

- Alocação dinâmica de memória com `malloc` e `realloc`
- Manipulação de arquivos com `fprintf`, `fscanf` e `fgets`
- Uso de `strtok` para parsing de linhas com campos variáveis
- Remoção de registros via arquivo temporário com `remove` e `rename`
- Structs para representação de entidades do sistema
