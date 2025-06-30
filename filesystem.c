#include "filesystem.h"

// Funcao para criar o caminho da arvore
BTree* btree_create() { 
    BTree* tree = malloc(sizeof(BTree));
    tree->root = NULL;
    return tree;
}

//Funcao para criar um node do tipo TXT
TreeNode* create_txt_file(const char* name, const char* content) {
    File* file = malloc(sizeof(File));
    file->name = strdup(name);
    file->content = strdup(content);
    file->size = strlen(content);

    TreeNode* node = malloc(sizeof(TreeNode));
    node->name = strdup(name);
    node->type = FILE_TYPE;
    node->data.file = file;
    return node;
}

//Funcao para criar um node do tipo DIR
TreeNode* create_directory(const char* name, TreeNode* parent) {
    Directory* dir = malloc(sizeof(Directory));
    dir->tree = btree_create();

    TreeNode* node = malloc(sizeof(TreeNode));
    node->name = strdup(name);
    node->type = DIRECTORY_TYPE;
    node->parent = parent;
    node->data.directory = dir;
    return node;
}

//-----------------------------{ Delete Functions }-----------------------------

void delete_txt_file(BTree* tree, const char* name) {

  TreeNode* target = btree_search(tree, name);

  if (!target) {
    printf("File '%s' not found.\n", name);
    return;
  }

  if (target->type != FILE_TYPE) {
    printf("'%s' is not a file.\n", name);
    return;
  }


  btree_delete(tree, name);

  free(target->name);
  free(target->data.file->content);
  free(target->data.file);
  free(target);

  printf("File '%s' deleted successfully.\n", name);
}

void btree_delete_node(BTreeNode* node, const char* name) {
  int i = 0;
  
  while (i < node->num_keys && strcmp(name, node->keys[i]->name) != 0) {
    i++;
  }

  if (i == node->num_keys) {
    // Não encontrou aqui
    if (node->leaf) {
      printf("'%s' not found in this node.\n", name);
    } else {
      btree_delete_node(node->children[i], name);
    }
  } 
  
  else {  
    if (node->leaf) {
      // Remove direto do vetor keys[]
      for (int j = i; j < node->num_keys - 1; j++) {
        node->keys[j] = node->keys[j + 1];
      }
      node->num_keys--;
    } 
    
    else {
      printf("Deletion from non-leaf not implemented.\n");
    }
  }
}

void btree_delete(BTree* tree, const char* name) {
  if(tree->root != NULL){
    btree_delete_node(tree->root,name);
  }
}

void delete_directory(BTree* tree, const char* name) {
  TreeNode* target = btree_search(tree,name);

  if(!target){ // verficar se existe
    printf("Directory %s not found\n", name);
    return;
  }

  if(target->type != DIRECTORY_TYPE){
    printf("%s's not a Directory\n",name);
    return;
  }

  Directory* dir = target->data.directory;

  //Verifica se esta vazio
  if(dir->tree->root != NULL && dir->tree->root->num_keys > 0){
    printf("%s is not empty\n", name);
    return;
  }

  btree_delete(tree,name);
  printf("Deleted %s directory\n", name);

  //Limpar a memoria
  free(target->name);
  free(dir->tree);
  free(dir);
  free(target);
}

//------------------------------------------------------------------------------

Directory* get_root_directory() {
  Directory* root = malloc(sizeof(Directory));
  root->tree = btree_create();
  return root;
}

void change_directory(TreeNode **currentNode, Directory **currentDir, TreeNode *next) {
  *currentNode = next;
  *currentDir = next->data.directory;
}

void list_directory_contents(Directory* dir) {
  btree_traverse(dir->tree->root);
}

//-----------------------------{ Search Functions }-----------------------------

TreeNode* btree_search_node(BTreeNode* node,const char* name){
  if(node == NULL) {return NULL;} //Arvore vazia

  int i = 0;
  //Verifica se esta no lugar certo
  while (i < node->num_keys && strcmp(name, node->keys[i]->name) > 0)
  {
    i++;
  }
  
  // Verifica se realmente encontrou
  if (i < node->num_keys && strcmp(name, node->keys[i]->name) == 0)
  {
    return node->keys[i];
  }
  
  //verifica se nao e' uma folha(arquivo .txt)
  if(node->leaf){
    return NULL;
  }

  return btree_search_node(node->children[i],name);

}

TreeNode* btree_search(BTree* tree, const char* name) {
  return btree_search_node(tree->root, name);
}

//------------------------------------------------------------------------------


//-----------------------------{ Insert Region }-----------------------------
// Cria um novo BTreeNode 
BTreeNode* btree_create_node(int leafNode) {
  BTreeNode* node = malloc(sizeof(BTreeNode));
  node->num_keys = 0; // Começa vazio
  node->leaf = leafNode;  // 1 = node, 0 = nao e' node

  // Inicializa todos os ponteiros de filhos como NULL
  for (int i = 0; i < 2 * BTREE_ORDER; i++) {
    node->children[i] = NULL;
  }
  
  return node;
}

//Funcao de SPLIT
/* Obs: I'm tired to make comments in all code line in this function, so, I`ll explain everthing here (Vai ser em portugues mesmo)
  A funcao tem como finalidade fazer a divisao de um root, ou seja, quando o negocio la' bater a quantidade maxima de nos ele deve ser
  dividido em 2(No caso vai ser criado mais 2 negocio do tipo raiz) onde o no da direita vai ficar com a copia da metade da direita
  o que ja existia(aquele que chegou no maximo de nos) vai ficar com os valores da esquerda e o novo no "raiz/pai" vai ficar com o no
  da metade do dois
*/
void btree_split_child(BTreeNode* parent, int i, BTreeNode* y) {
  BTreeNode* z = btree_create_node(y->leaf); //cria novo no
  z->num_keys = BTREE_ORDER - 1; //pega metade la

  //copia os valores da direita pro novo no
  for (int j = 0; j < BTREE_ORDER - 1; j++) {
    z->keys[j] = y->keys[j + BTREE_ORDER];
  }

  //se for um DIR, vai copiar os valores da direita tambem
  if (!y->leaf) {
    for (int j = 0; j < BTREE_ORDER; j++) {
      z->children[j] = y->children[j + BTREE_ORDER];
    }
  }

  //primeiro filho vai ficar com os valores da esquerda
  y->num_keys = BTREE_ORDER - 1; 

  //Faz o movimento dos filhotes
  for (int j = parent->num_keys; j >= i + 1; j--) {
    parent->children[j + 1] = parent->children[j];
  }
  
  //o pai adota o novo filho dele
  parent->children[i + 1] = z;

  //Reordena os filhos do papai, para ssim abrir espaco das chaves
  for (int j = parent->num_keys - 1; j >= i; j--) {
    parent->keys[j + 1] = parent->keys[j];
  }
  
  //sobe a chave do meio para o pai
  parent->keys[i] = y->keys[BTREE_ORDER - 1];
  parent->num_keys++;
}


// Insere uma chave (TreeNode*) em um nó que ainda NAO esta' cheio.
// Se precisar, divide filhos cheios no caminho para garantir espaço.
void btree_insert_nonfull(BTreeNode* x, TreeNode* k) {
  // Começa do indice da última chave ocupada
  int i = x->num_keys - 1;

  // Se este no e' uma FOLHA (não tem filhos)
  if (x->leaf) {
    // Enquanto nao achou a posicao correta para a nova chave:
    // Desloca todas as chaves maiores para a direita para abrir espaco
    while (i >= 0 && strcmp(k->name, x->keys[i]->name) < 0) {
      x->keys[i + 1] = x->keys[i]; // move chave para a direita
      i--;
    }

    // Coloca a nova chave na posicao certa
    x->keys[i + 1] = k;
    x->num_keys++; // atualiza o número de chaves no no
  
  } 

  // Se NAO e' folha: precisa encontrar o filho certo e inserir la'
  else {
    // Acha o filho correto para onde a nova chave deve ir
    while (i >= 0 && strcmp(k->name, x->keys[i]->name) < 0) {
      i--;
    }
    i++; // Corrige indice: filho certo e' na direita da ultima chave menor

    // Verifica se o filho escolhido esta CHEIO
    if (x->children[i]->num_keys == 2 * BTREE_ORDER - 1) {
      // Se o filho estiver cheio, divide ele em dois filhos
      btree_split_child(x, i, x->children[i]);

      // Depois de dividir, pode ser que a chave precise ir para a metade nova
      if (strcmp(k->name, x->keys[i]->name) > 0) {
        i++; // avança para o filho da direita
      }
    }

    // Recursivamente insere no filho apropriado (que agora nao esta cheio)
    btree_insert_nonfull(x->children[i], k);
  }
}


void btree_insert(BTree* tree, TreeNode* node) {
    //Verifica se a raiz e' nula, se for vai ser criado um no la'
    if (tree->root == NULL) {
        tree->root = btree_create_node(1);
    }

    //Pega a raiz(Criada anteriormente ou nao)
    BTreeNode* r = tree->root;

    //Verifica se estourou o numero de nos
    if (r->num_keys == 2 * BTREE_ORDER - 1) {
      BTreeNode* s = btree_create_node(0); //Cria um no que nao e' folha
      s->children[0] = r;
      btree_split_child(s, 0, r);// divide em dois o filho e sobe o no do meio para o pai
      btree_insert_nonfull(s, node); //Como foi dividio, ele so' precisa inserir o novo no no lugar certo
      tree->root = s;
    } 
    else {
      btree_insert_nonfull(r, node); //insere o no em um campo vazio
    }
}

//---------------------------------------------------------------------------
void btree_print_tree(BTreeNode* node, FILE* f, int depth, const char* prefix) {
  if (node == NULL) return;

  for (int i = 0; i < node->num_keys; i++) {
    TreeNode* t = node->keys[i];

    for (int d = 0; d < depth; d++) fprintf(f, "%s", prefix);

    if (t->type == FILE_TYPE) {
      fprintf(f, "└── %s: %s\n", t->name, t->data.file->content);
    } else if (t->type == DIRECTORY_TYPE) {
      fprintf(f, "├── %s\n", t->name);
      btree_print_tree(t->data.directory->tree->root, f, depth + 1, "│   ");
    }
  }

    if (!node->leaf && node->num_keys > 0) {
      btree_print_tree(node->children[node->num_keys], f, depth + 1, "│   ");
  }
}



void printTree(TreeNode * dirNode){
  if (!dirNode || dirNode->type != DIRECTORY_TYPE) {
    printf("Invalid directory.\n");
    return;
  }

  char filename[200];
  snprintf(filename, sizeof(filename), "prints/print_%s.img", dirNode->name);

  FILE* img = fopen(filename, "w");
  if (!img) {
      perror("Error creating print file");
      return;
  }

  fprintf(img, "%s\n", dirNode->name);
  btree_print_tree(dirNode->data.directory->tree->root, img, 0, "");

  fclose(img);
  printf("Directory printed to %s\n", filename);
}


void btree_traverse(BTreeNode* tree) {
  BTreeNode* node = tree;
  
  if (node == NULL) return;

  int i;
  for(i = 0; i < node->num_keys; i++){

    //vai para o filho da esquerda
    if(!node->leaf){
      btree_traverse(node->children[i]);
    }

    //Imprime as chaves atuais
    TreeNode* t = node->keys[i];
    if(t->type == FILE_TYPE){
      printf("[FILE] %s\n", t->name);
    }
    else if(t->type == DIRECTORY_TYPE){
      printf("[DIR] %s\n",t->name);
    }
  }

  if(!node->leaf){
    //btree_traverse(node->children[i]);
    btree_traverse(node);
  }
}