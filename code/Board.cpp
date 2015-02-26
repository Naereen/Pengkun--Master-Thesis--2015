#include "Board.h"
#include <string>
#include <cstdlib>
#include <iostream>
#define FALSE 0
#define TRUE 1

Board::Board(int n,Piece ** p=NULL){
	pieces = p;
	numberOfPieces = n;
}

//currently text mode display
void Board::display(){
	//if pieces have not been instantiated
	using namespace std;
	if(board==NULL)
		return ;
	int l = 8;
	int i,j;
	for(i=l-1;i>-1;i--){
		for(j=0;j<l;j++){
			string output = "";
			if(board[i][j]==NULL)
				output = "X";
			else if(board[i][j]->getColor() == 'B'){
				string black  = "B_";
				output = black + board[i][j]->getIdentifier();
			}
			else{
				string white = "W_";
				output = white +  board[i][j]->getIdentifier();
			}
			cout << output << " ";
		}	
		cout << endl;
	}
	cout << endl << endl;

}

void Board::initialize(){
	//instantiate 32 piecess
	pieces  = new Piece*[32];
      	int i=0;
      	Rook * rook1 = new Rook('W',NULL);
      	pieces[i++] = rook1;
     	Knight * knight1 = new Knight('W',NULL);
     	pieces[i++] = knight1;
        Bishop * bishop1 = new  Bishop('W',NULL);
	pieces[i++] = bishop1;
	Queen * queen1 = new Queen('W',NULL);
	pieces[i++] = queen1;
        King *king1 = new King('W',NULL);
	pieces[i++] = king1;
	Bishop * bishop2 = new Bishop('W',NULL); pieces[i++] = bishop2;
        Knight *knight2 = new Knight('W',NULL);
     	pieces[i++] = knight2;
	Rook * rook2= new Rook('W',NULL);
      	pieces[i++] = rook2;

        int j=0;
        for(;j<16;j++){
		if(j<8){
			Pawn * pawn = new Pawn('W',NULL);
			pieces[i++] =pawn;
		}
		else{
			Pawn * pawn = new Pawn('B',NULL);
			pieces[i++] = pawn;
			}
     	 }
        Rook *rook3 = new Rook('B',NULL);
      	pieces[i++] = rook3;
     	Knight * knight3 = new Knight('B',NULL);
     	pieces[i++] =knight3;
        Bishop *bishop3 = new Bishop('B',NULL);
	pieces[i++] = bishop3;
	Queen * queen2 = new Queen('B',NULL);
	pieces[i++] = queen2;
        King * king2 = new King('B',NULL);
	pieces[i++] = king2;
	Bishop * bishop4 = new Bishop('B',NULL);
	pieces[i++] = bishop4;
        Knight * knight4 = new Knight('B',NULL);
     	pieces[i++] = knight4;
	Rook * rook4 = new Rook('B',NULL);
      	pieces[i] = rook4;

	//set the position of these piecess
        for(i=0;i<16;i++){
	       	int * l =  new int[2];
	       	l[0] = i/8;
	       	l[1] = i%8;
	       	pieces[i]->setPosition(l);
	}
	for(i=48;i<64;i++){
		int * l = new int[2];
		l[0] = i/8;
		l[1] = i%8;
		pieces[i-32]->setPosition(l); 
	}

	//initialization of the board
	int l  = 8;
        for(i=0;i<l;i++)
		for(j=0;j<l;j++)
			board[i][j] = NULL;
	for(i=0;i<numberOfPieces;i++){
		int * p = pieces[i]->getPosition();
		board[p[0]][p[1]] = pieces[i]; 

	}
}
bool Board::moveIsLegal(int * from, int * to){
	if(from[0]<8 && from[0]>-1 && from[1]>-1 && from[1]<8 && to[0]<8 && to[0]>-1 && to[1]<8 && to[1]>-1){
		Piece * p = board[from[0]][from[1]];
		
		if(p!=NULL){
			if(p->moveIsLegal(to))
				p->makeMove(to);
			else
				return FALSE;
		}
		else
			return FALSE;
	}
	else{
		return FALSE;
	}	

}

void Board::makeMove(int * from, int *to){
	board[to[0]][to[1]] = board[from[0]][from[1]];
	board[from[0]][from[1]] =  NULL;
}

int main(){
	using namespace std;
	Board start =  Board(32);
	start.initialize();
	start.display();
	string command = "";
	while(1){
		getline(cin,command);
		if(command=="abort" || command=="quit"){
			exit(1);
		}
		int a,b,c,d;
		a = command[0]-'a';
		b = command[1]-'1';
		c = command[2] - 'a';
		d = command[3] - '1';
		
		int * from = new int[2];
		int * to = new int[2];
		from[0] = b;
		from[1] = a;
		to[0] = d;
		to[1] = c;
		if(start.moveIsLegal(from,to)){
			start.makeMove(from,to);
			start.display();
		}
		else
			cout << "error movement"<< endl;
		
	}

}
