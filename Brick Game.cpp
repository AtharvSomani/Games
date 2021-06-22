#include<iostream>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<time.h>

//Constant Functions
#define SCREEN_WIDTH 52
#define SCREEN_HEIGHT 20

#define MINX 2
#define MINY 2
#define MAXX 49
#define MAXY 19


using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

//Bricks Coordinates 
int bricks[24][2]={
	                    {2,7},{2,12},{2,17},{2,22},{2,27},{2,32},{2,37},{2,42},
	                    {4,7},{4,12},{4,17},{4,22},{4,27},{4,32},{4,37},{4,42},
	                    {6,7},{6,12},{6,17},{6,22},{6,27},{6,32},{6,37},{6,42}
                  };
                  
int visibleBricks[24] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
int sliderPos[2] = {18,22};
int ballPos[2] = {17,26};
int startBall = 0;
int dir = 2;//1 - TopRight, 2 - TopLeft, 3 - BottomLeft, 4 - BottomRight
int bricksLeft = 24;
int win = 0;
int lose = 0;

void gotoxy(int x, int y) //get cursor at Desired placed.
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
	
} 
void setcursor(bool visible,DWORD size)// set bool visible = 0 - invisible, bool visible = 1-visible
{
	if(size == 0)
	{
		size = 20;// default cursor size Changing number 1 to 20 ,decrease size width
	}
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
	
}

//Function for drawing Border
void drawBorder(){
	gotoxy(0,0);
	cout<<"---------------------------------------------------------";
	gotoxy(0,SCREEN_HEIGHT);
	cout<<"---------------------------------------------------------";
	
	for(int i = 0;i<SCREEN_HEIGHT;i++)
	{
		gotoxy(0,i);
		cout<<"|";
		gotoxy(SCREEN_WIDTH,i);
		cout<<"|";
		
	}
}

//Funtion for drawing bricks
void drawBricks()
{
	for(int i = 0;i<24;i++)
	{
		if(visibleBricks[i] == 1)
		{
			gotoxy(bricks[i][1],bricks[i][0]);
			cout<<"[||]";
		}
	}
}

//Funtion for Ball Hit Slider
void ballHitSlider(){
	if(ballPos[1]>=sliderPos[1] && ballPos[1]<=sliderPos[1]+8)
	{
		if(ballPos[0] == sliderPos[0]-1)
		{
			if(dir == 3)     // if the ball is approaching from bottom left
				dir = 2;     //then it will move in top left  direction
			else if(dir == 4)     //if ball is approaching slider from bottom right 
				dir = 1;          //it will move to top right direction
			  
		}
	}
}

// Function for BallHit Bricks

void ballHitBrick()
{
	for(int i =0;i<24;i++)
	{
		if(visibleBricks[i]==1)
		{
			if(ballPos[1]>=bricks[i][1] && ballPos[1]<=bricks[i][1]+8)
			{
				if(ballPos[0] == bricks[i][0])      //ball hit bricks bricks left will be reduced 
				{
					visibleBricks[i] = 0;
					bricksLeft--;
				}
			}
		}
	}
}

//Play Function
void play()
{
	while(1)
	{
		system("cls");
		drawBricks();
		drawBorder();
		
		gotoxy(sliderPos[1],sliderPos[0]);
		cout<<"+++++++++";
		
		gotoxy(ballPos[1],ballPos[0]);
		cout<<"0";
		if(kbhit())
		{
			char ch = getch();
			if(ch == 'd'||ch == 'D'||ch==77)        // To move slider to right
			{
				if(sliderPos[1]<44)
					sliderPos[1]=sliderPos[1]+2;
			}
			if(ch == 'a'||ch=='A'||ch==75)         // to move slider to left
			{
				if(sliderPos[1]>2)
					sliderPos[1] = sliderPos[1]-2;
			}
			if(ch == 32){
				startBall = 1;
			}
			if(ch == 27)
			{
				break;
			}
		}
		
		if(startBall == 0)                   //function to rebound ball from border
		{
			if( dir == 1)
			{
				ballPos[0] = ballPos[0]-1;       
				ballPos[1] = ballPos[1]+2;
				if(ballPos[1] > MAXX)
				{
					dir = 2;
				}
			
			    else if( ballPos[0]<MINY)
			    {
				    dir = 4;
			    }
		    }
			else if(dir == 2)                   //Top left
			{
				ballPos[0] = ballPos[0]-1;
				ballPos[1] = ballPos[1]-2;
				if(ballPos[0]<MINY)
				{
					dir = 3;
				}
				else if(ballPos[1] < MINX)
				{
					dir = 1;
				}
			}
			
			else if(dir == 3)                 //Bottom left
			{
				ballPos[0]=ballPos[0]+1;
				ballPos[1]=ballPos[1]-2;
				
				if(ballPos[0] > MAXY)
				{
					lose = 1;
					break;
				}
				else if(ballPos[1] < MINX)
				{
					dir = 4;
				}
			}
			
			else if(dir == 4)  //BOTTOM RIGHT
			{
				ballPos[0]=ballPos[0]+1;
				ballPos[1]=ballPos[1]+2;
				if(ballPos[1]>MAXX)
				{
					dir = 3;
				}
				else if(ballPos[0] > MAXY)
				{
					lose = 1;
					break;
				}
				
			}
			
			ballHitSlider();
			
		}
		ballHitBrick();
		if(bricksLeft == 0)
		{
			win =  1;
			break;
		}
		
		Sleep(30);
	}
	
	if(lose == 1)               // after losing the game function to display that you lose
	{
		system("cls");
		
		gotoxy(10,5);
		cout<<"--------------------------";
		gotoxy(10,6);
		cout<<"|       you lose       |";
		gotoxy(10,7);
		cout<<"--------------------------";
		
		gotoxy(10,9);
		cout<<"Press any key to go  back to Menu";
		getch();
		
	}
	if(win == 1)              // after winning the game function to display that you win
	{
		system("cls");
		gotoxy(10,5);
		cout<<"---------------------------";
		gotoxy(10,6);
		cout<<"|       You win           |";
		gotoxy(10,7);
		cout<<"----------------------------";
		
		gotoxy(10,9);
		cout<<"Press any key to go back to Menu";
		getch();
		
	}
}

void instructions()   //funtions to display instructions
{
	system("cls");
	cout<<"Instructions .";
	cout<<"\n---------------";
	cout<<"\n1. Use 'a' key to move slider to left ";
	cout<<"\n2. Use 'd' key to move slider to right ";
	cout<<"\n3. Press spacebar to start the game";
	cout<<"\n\nPress any  key to go back to menu ";
	getch();
	
}
int main()
{
	setcursor(0,0);               //function to display menu for the game
	
	do
	{
		system("cls");
		gotoxy(10,5);
		cout<<"----------------------------";
		gotoxy(10,6);
		cout<<"|       BRICK GAME          |";
		gotoxy(10,7);
		cout<<"----------------------------";
		gotoxy(10,9);
		cout<<"1. Start Game ";
		gotoxy(10,10);
		cout<<"2. Instructions";
		gotoxy(10,11);
		cout<<"3. Quit ";
		gotoxy(10,13);
		cout<<"Selection Option:";
		char  op = getche();
		
		if( op =='1') play();
		else if( op =='2') instructions();
		else if( op =='3') exit(0);
		
	}while(1);
	play();
	cout<<endl<<endl;
	return 0;
	
}
