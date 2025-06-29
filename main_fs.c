#include "filesystem.h"
//#include <stdio.h>
/*
int main() {
    // Criação do sistema de arquivos
    Directory* root = get_root_directory();

    // Diretórios de segundo nível
    TreeNode* dirSO = create_directory("SO");
    TreeNode* dirTEST = create_directory("TEST");
    btree_insert(root->tree, dirSO);
    btree_insert(root->tree, dirTEST);

    // Arquivo em SO
    TreeNode* file1 = create_txt_file("arquivo1.txt", "Arquivo de teste de SO.");
    btree_insert(dirSO->data.directory->tree, file1);

    // Listagem
    printf("--- Conteúdo do diretório ROOT ---\n");
    list_directory_contents(root);

    printf("\n--- Conteúdo do diretório SO ---\n");
    list_directory_contents(dirSO->data.directory);

    // Simulação de persistência
    FILE* img = fopen("prints/fs.img", "w");
    if (img) {
        fprintf(img, "ROOT\n");
        fprintf(img, "├── SO\n");
        fprintf(img, "│   └── arquivo1.txt: Arquivo de teste de SO.\n");
        fprintf(img, "└── TEST\n");
        fclose(img);
        printf("\nSistema de arquivos salvo em fs.img\n");
    } else {
        perror("Erro ao criar fs.img");
    }

    return 0;
}
*/ 

char toLower(char c){ // Caso o valor seja uma letra minuscula, será retornadoa em lowerCase
   if (c >= 'A' && c <= 'Z') {
      return c + 32;
   }
   return c;
}

// Verifica se as seis primeiras letras são em Lower Case
void checkIfIsAllLower(char *command){ 
   for(int i=0; i<6 && command[i] != '\0'; i++){
      if(command[i] < 'a' || command[i] > 'z'){
         command[i] = toLower(command[i]);
      }
   }
}

int checkFunction(char *command, Directory* root){
    char token[5][100] = {0};
    int tokenCount = 0;
    int coluna = 0;
    int linha = 0;

    for(int i = 0; command[i] != '\0' && command[i] != '\n';i++){
        if(command[i] == ' '){
            token[linha][coluna] = '\0';
            linha++;
            coluna = 0;
        }
        else 
        {
            token[linha][coluna] = command[i];
            coluna++;
        }
        
    }
    token[linha][coluna] = '\0';
    tokenCount = linha+1;

    //Verifica se a o primeiro token = mkdir
    if (strcmp(token[0], "mkdir")== 0 && strcmp(token[1], "")== 0)
    {
        printf("You not entered a director name\n");
    }
    else if(strcmp(token[0], "mkdir")== 0){
        //Cria todos os diretorios que o token for diferente de nulo
        for(int i = 0; i < tokenCount; i++){
            if(strcmp(token[i], "mkdir")!= 0){

                btree_insert(root->tree, create_directory(token[i]));
                
                printf("Created %s\n", token[i]);
            }
        }
    }
    else if (strcmp(token[0], "rmdir")== 0)
    {
        printf("Remove DIR\n");
    }
    else if (strcmp(token[0], "touch")== 0)
    {
        //btree_insert(root->data.directory->tree, create_txt_file("arquivo1.txt", "Arquivo de teste de SO."));

        printf("Created file\n");
    }
    else if (strcmp(token[0], "rm")== 0)
    {
        printf("Removed file\n");
    }
    else if (strcmp(token[0], "ls")== 0)
    {
        list_directory_contents(root);
    }
    else if (strcmp(token[0], "cd")== 0)
    {
        // TreeNode* next = btree_search(currentDir->tree, token[1]);
        // if (next && next->type == DIRECTORY_TYPE) {
        //     currentNode = next;
        //     currentDir = next->data.directory;
        // } else {
        //     printf("Diretório não encontrado.\n");
        // }
        printf("Go to new path\n");
    }
    else{
        printf("Command not found\n");
    }

    return 0;
}



int PROMPT(){
    char text[100]= "";
    // Directory* root = get_root_directory();
    // char dir[100] = "";

    TreeNode* currentNode = create_directory("ROOT");
    Directory* currentDir = currentNode -> data.directory;

    while (1)
    {
        printf("-> %s $ ",currentNode->name);
        fgets(text, 100, stdin);
        //scanf("%s",text); //"%d",
        
        checkIfIsAllLower(text);

        if(strcmp(text, "")== 0 || strcmp(text, "\n")== 0){

        }
        else if(strncmp(text, "quit",4)== 0){ //defini por pegar apenas as 4 primeiras letras ao inves de ter que verificar "quit\n"
            printf("Closing PROMPT\n");
            break;
        }
        else {
            checkFunction(text,currentDir);
        }
    }
    
    return 0;
}




int main(){
    PROMPT();

    return 0;
}