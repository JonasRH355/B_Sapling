#include "filesystem.h"
#include <stdio.h>
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

char toLower(char c){ // Caso o valor seja uma letra minuscula, será retornadoa em Uppercase
   if (c >= 'A' && c <= 'Z') {
      return c + 32;
   }
   return c;
}

// Verifica se as seis primeiras letras são em Uppercase
void checkIfIsAllLower(char *command){ 
   for(int i=0; i<6 && command[i] != '\0'; i++){
      if(command[i] < 'a' || command[i] > 'z'){
         command[i] = toLower(command[i]);
      }
   }
}

int checkFunction(char *command){
    char token[5][100];
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

    if (strcmp(token[0], "mkdir")== 0)
    {
        printf("Created DIR\n");
    }
    else if (strcmp(token[0], "rmdir")== 0)
    {
        printf("Remove DIR\n");
    }
    else if (strcmp(token[0], "touch")== 0)
    {
        printf("Created file\n");
    }
    else if (strcmp(token[0], "rm")== 0)
    {
        printf("Removed file\n");
    }
    else if (strcmp(token[0], "ls")== 0)
    {
        printf("List of paths\n");
    }
    else if (strcmp(token[0], "cd")== 0)
    {
        printf("Go to new path\n");
    }
    else{
        printf("Command not found\n");
    }

}



int PROMPT(){
    char text[100]= "";
    char dir[100] = "";

    while (1)
    {
        printf("->%s $ ",dir);
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
            checkFunction(text);
        }
    }
    
    return 0;
}




int main(){
    PROMPT();

    return 0;
}