#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

#define BEFORE 1
#define AFTER 0

int row, col, wcnt=0, remaining, c;
char **alang, **blang;
FILE *wordFile;

void progressBar(int percent);
void printToCenter(char* word);
void instructionsBefore();
void instructionsAfter();
void refreshScreen(char* word, int percent, char state);
void mainCycle();

int main(int argc, char *argv[]){
	//we gonna check, if we got an argument.
	if (argc != 2){
		printf("Usage: %s <word_file>\n", argv[0]);
		return 1;
	}
	
	//we gonna check, if that file exists.
	wordFile = fopen (argv[1], "r");
	if (wordFile == NULL){
		printf(" File \"%s\" does not exists.\n", argv[1]);
		return 2;
	}
	//first, we gonna check the number og lines
	
	while( (c=fgetc(wordFile)) != EOF){
		if (c=='\n') wcnt++; 
	}

	//we gonna go back to the first line
	rewind(wordFile);
	
	//we gonna allocate shit
	alang = (char**)malloc(wcnt*sizeof(char*));	
	blang = (char**)malloc(wcnt*sizeof(char*));
	//then we gonna allocate each shit one by one
	int i;
	for(i=0;i<wcnt;i++){
		alang[i] = (char*)malloc(sizeof(char*));
		blang[i] = (char*)malloc(sizeof(char*));
	}

	//we gonna put some shit into them
	i=0;
	while (EOF != fscanf(wordFile, "%[^*;]%*c%[^*\n]%*c", alang[i], blang[i])){
		i++;
	}

	fclose(wordFile);
	initscr();
	keypad(stdscr, 1);
	noecho();
	cbreak();
	mainCycle();
	clear();
	printToCenter("Congratulations, you've learned all the words!");
	refresh();
	getch();
	endwin();
	return 0;
}

void mainCycle(){
	int j;
	remaining=wcnt;
	while(remaining!=0){
		for(j=0;j<=wcnt;j++){
			if (alang[j]!=NULL) refreshScreen(alang[j],(remaining*100)/wcnt,BEFORE);
				else continue;
			while ('\n' != getch());
			if (blang[j]!=NULL) refreshScreen(blang[j],(remaining*100)/wcnt,AFTER);
			//TODO calculating percent twice
			while(1){
				int chr=getch();
				if(chr=='k') break;
				if(chr=='s'){
					alang[j]=NULL;
					blang[j]=NULL;
					remaining--;
					break;
				printw("wow");
				}
			}
		}
	}
}

void refreshScreen(char* word, int percent, char state){
	//here we gonna paint a nice text user interface
	//for that, first we gonna print a progress bar, then start displaying words
	clear();
	printToCenter(word);
	switch(state){
		case BEFORE:
			instructionsBefore();
			progressBar(percent);
			printToCenter(word);
			break;
		case AFTER:
			instructionsAfter();
			progressBar(percent);
			printToCenter(word);
			break;
	}
	
}

void printToCenter(char* word){
	getmaxyx(stdscr, row, col);
	move((row/2.0)-2, (col/2.0)-(strlen(word)/2.0));
	printw(word);
}

void progressBar(int percent){
	getmaxyx(stdscr, row, col);
	//first, we gonna calculate, how many characters do we want.
	int currentRow,currentCol,percentBlocks;
	percentBlocks=col*(percent/100.0);
	printf("%d\n", percentBlocks);
	int j;
	for(j=0; j<percentBlocks;j++){
		move(row-2,j);
		addch(ACS_CKBOARD);
	}
}

void instructionsBefore(){
	move(row-1,2);
	printw("Enter: show answer");
}

void instructionsAfter(){
	move(row-1,2);
	printw("s: I knew it.");
	move(row-1, col-2-strlen("K: I did not know it."));
	printw("k: I did not know it.");
}
