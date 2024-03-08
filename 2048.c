//gcc: -lncurses
#include <ncurses.h> //Importation des differentes bibliotheque 
#include <time.h>
#include <stdlib.h>
 
int board[4][4]; //initialisation de la matrice de taille 4 x 4
int score= 0; //initialisation du score a 0
enum key {UP, DOWN, LEFT, RIGHT, FINISH}; //initialisation des differents touches possible


void INIT_2048(){ //apel de differentes fonctions permettant le bon fonction du jeux et la biblitheque ncurses
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    srand(time(NULL));
}


void DONE_2048(){ //gère la fin de partie
    endwin();
    exit(0);
}

void display_board(){ //affiche le plateau de jeux avec les bons nombres dans les cases
  initscr();
  clear();
  printw("==2048============================\n====================== score :  %i\n",score);
  for (int i=0;i<4;i++){
    printw("\n+-------+-------+-------+-------+");
    for(int j=0;j<2;j++){
      if (j==1){
        printw("\n");
        for (int k =0; k<4; k++){
          if (board[i][k]==0){ //si la case doit contenir le chiffre 0 elle affiche plusieurs espaces sinon elle affiche le nombre
            printw("|       ");
          }else {
            printw("| %5d ",board[i][k]); 
          }
        }
        printw("|");
      }
      printw("\n|       |       |       |       |");
    }
  }
  printw("\n+-------+-------+-------+-------+\n");
  refresh();  
} 

void init_board(){ //initialise toutes les cases de la matrcie a 0
    for (int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            board[i][j]= 0;
        }
    }
}

int count_empty(){ //compte le nombre de cases vides
  int cpt = 0;
  for (int i =0; i< 4; i++){
    for (int j = 0; j< 4; j++){
      if (board[i][j] == 0){
        cpt++;
      }
    }
  }
  return cpt;
}

void add_two(int empty){ //ajoute un 2 de manière aléatoire dans la matrice
  int Case = random() % empty;
  int cpt =0;
  for (int i=0; i<4;i++){
    for (int j = 0; j< 4; j++){
      if (board[i][j]==0){
        if (Case == cpt){
          board[i][j]=2;
        }
        cpt++;
      }
    }
  }
}
 
int game_over(int add){ //gere le jeux et decide si le jeux peut continuer ou pas
  if (count_empty()<=0){
    printw("=============GAME OVER============\n============(press a key)=========\n");
    refresh();
    getch();
    return 1;
  }
  else if (count_empty()>=1 && add ==1){
    add_two(count_empty());
    display_board();
    return 0;
  }
}

int shift_board(){ //décale le plateau vers la gauche
  int cpt=0;
  for (int i =0; i<4; i++){
    for (int j = 3; j>0;j--){
      if (board[i][j-1]==0 && board[i][j]!=0){
        board[i][j-1]=board[i][j];
        board[i][j]=0;
        cpt+=1;
      }
    }
  }
  if (cpt == 0){
    return 0;
  }else{
    return 1;
  }
}

int update_board(){ //compression du tableau vers la gauche
  int cpt=0;
  shift_board();
  for (int i = 0; i<4; i++){ 
    for (int j =0; j<3; j++){ // j < 3 car en i = 3, i+1 = 4 or 4 n'existe pas 
      if (board[i][j]==board[i][j+1]){
        board[i][j] *=2;
        score+=(board[i][j]);
        board[i][j+1] = 0;
        cpt+=1;
      }
    }
  }
  shift_board(); 
  if (cpt == 0){
    return 0;
  }else{
    return 1;
  }
}

enum key get_key(){ //detecte si une touche a été utilisé au clavier et la renvoie
  while(1==1){
    int touche = getch();
    if (touche == KEY_UP){
      return UP;
    }
    else if (touche == KEY_DOWN){
      return DOWN;
    }
    else if (touche == KEY_LEFT){
      return LEFT;
    }
    else if (touche == KEY_RIGHT){
      return RIGHT;
    }
    else if (touche == KEY_BACKSPACE){
      return FINISH;
    }
  }
}

void swap(int* a, int* b){ //echange deux entiers en mémoire
  int tmp = *a;
  *a=*b;
  *b=tmp;
}

void mirror_board(){ // inverse chaque ligne de la matrice
  for (int i=0;i<4;i++){
      swap(&board[i][0],&board[i][3]);
      swap(&board[i][1],&board[i][2]);
  }
}

void pivot_board(){ //effectue une rotation a 90 degre dans le sens des aiguilles d'une montre
  for (int i=0;i<4;i++){
    for (int j=(i+1);j<4;j++){
      swap(&board[i][j],&board[j][i]);
    }
  }
  for (int i =0;i<4;i++){
    for (int j=0;j<2;j++){
      swap(&board[i][j], &board[i][3-j]);
    }
  }
}

int play(enum key dir){ //effectue les transformations du plateau selon la touche utilsé
  if (dir == LEFT){
    update_board();
  }
  if (dir == RIGHT){
    mirror_board();
    update_board(); 
    mirror_board();
  }
  if (dir == DOWN){
    pivot_board();
    update_board();
    pivot_board();
    pivot_board();
    pivot_board();
  }
  if (dir == UP){
    pivot_board();
    pivot_board();
    pivot_board();
    update_board();
    pivot_board();
  }
  if (dir == FINISH){
    DONE_2048();
  }
}

int main(){ //main qui appel les fonctions au bons moments
  int add= 1;
  INIT_2048();
  init_board();
  display_board();
  while(!game_over(add)){
    play(get_key());
  }
  DONE_2048();
}
