#include "filesystem.h"

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

int execFunction(char *command, Directory** currentDir, TreeNode** currentNode){
    char token[120][116] = {0};
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
        for(int i = 1; i < tokenCount; i++){
            if(btree_search( (*currentDir)->tree, token[i] ) == NULL){

                btree_insert((*currentDir)->tree, create_directory(token[i],*currentNode));
                
                printf("Created %s\n", token[i]);
            }
            else{
                printf("The Directory %s already exist\n", token[i]);    
            }
        }
    }
    
    else if (strcmp(token[0], "rmdir")== 0)
    {
        if(strcmp(token[1], "")== 0){
            printf("You not entered any directory's name to delete\n");
            return 0;
        }

        for(int i = 1; i < tokenCount; i++){
            delete_directory((*currentDir)->tree,token[i]);
        }
    }
    
    else if (strcmp(token[0], "touch")== 0)
    {
        if(strcmp(token[1], "")== 0){
            printf("You not entered a name to the file\n");
            return 0;
        }
        char content[1048576 + 1] = ""; //Touch tem no maximo 1MB
        
        //Recriar o conteudo do arquivo
        for(int i = 2; i < tokenCount; i++){
            strcat(content, token[i]);
            if (i < tokenCount - 1) {
                strcat(content, " ");
            }
        }


        char fileName[120];
        snprintf(fileName, sizeof(fileName),"%s.txt", token[1]);

        if( btree_search( (*currentDir)->tree, fileName ) == NULL
        ){

            btree_insert(
                (*currentDir)->tree
                ,create_txt_file(fileName, content)
            );

            printf("Created %s\n", fileName);
        }        
        else{
            printf("The file %s already exist\n", fileName);
        }
    }
    
    else if (strcmp(token[0], "rm")== 0)
    {
        if(strcmp(token[1], "")== 0){
            printf("You not entered any file's name to delete\n");
            return 0;
        }

        for(int i = 1; i < tokenCount; i++){
            delete_txt_file((*currentDir)->tree,token[i]);
        }
    }
    
    else if (strcmp(token[0], "ls")== 0)
    {
        list_directory_contents(*currentDir);
    }
    
    else if (strcmp(token[0], "cd")== 0 && strcmp(token[1], "")== 0){
        printf("You not entered any directory\n");
    }

    else if (strcmp(token[0], "cd")== 0)
    {
        if(strcmp(token[1], "..")== 0){
            if((*currentNode)->parent == NULL){
                printf("U cann`t go back, because u are on the ROOT\n");
            }
            else{
                change_directory(currentNode,currentDir,(*currentNode)->parent);
                // *currentNode = (*currentNode)->parent;
                // *currentDir = (*currentNode)->data.directory;
            }
        }
        else{
            TreeNode* next = btree_search((*currentDir)->tree, token[1]);
            if (next && next->type == DIRECTORY_TYPE) {
                change_directory(currentNode,currentDir,next);
                // *currentNode = next;
                // *currentDir = next->data.directory;
            } 
            
            else {
                printf("Directory not found.\n");
            }
        }
    }

    else if (strcmp(token[0], "print")== 0 ){
        printf("Chegou aqui\n");
        printTree(*currentNode);
    }
    
    else if(strncmp(token[0], "quit",4)== 0){ //defini por pegar apenas as 4 primeiras letras ao inves de ter que verificar "quit\n"
        printf("Closing PROMPT\n");
        return 1;
    }
    
    else{
        printf("Command not found\n");
    }

    return 0;
}



int PROMPT(){
    char text[1048576 + 1]= "";
    // Directory* root = get_root_directory();
    // char dir[100] = "";

    TreeNode* currentNode = create_directory("ROOT",NULL);
    Directory* currentDir = currentNode -> data.directory;

    while (1)
    {
        
        printf("-> %s $ ",currentNode->name);
        fgets(text, 100, stdin);
        //scanf("%s",text); //"%d",
        
        checkIfIsAllLower(text);

        if(strcmp(text, "")== 0 || strcmp(text, "\n")== 0){

        }
        else {
            if( execFunction(text,&currentDir,&currentNode) == 1)
            {
                break;
            }
            
        }
    }
    
    return 0;
}




int main(){
    PROMPT();

    return 0;
}