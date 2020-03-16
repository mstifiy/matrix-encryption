/*
NAME：Matrix encryption(C=AB)
DEVELOPER：MSTIFIY
TIME：2020.3.13.16:41 to 2020.3.14.22:30 to 2020.3.15.23:43
VERSION：2.0
*/
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>
#include<conio.h>
#define MAX 520
#define KEYORDER 3					
#define JUDGE(num) num%KEYORDER?num/KEYORDER+1:num/KEYORDER
#define REACTIONTIME 20000
#define RANDMAX 10

char codeList[] = { '^','a','b','c','d','e','f',
					'g','h','i','j','k','l','m',
					'n','o','p','q','r','s','t',
					'u','v','w','x','y','z','!',
					',','.','?','\'' };
int keyMatrix[KEYORDER][KEYORDER] = { {1,1,1},
				      {-1,0,1},
				      {0,1,1} };
double invKeyMat[KEYORDER][KEYORDER] = { 0 };
char text[MAX] = { '\0' };
int **originalMatrix;
int **ciphertMatrix;
int reversibility;
int row;
HANDLE hout;
typedef enum 
{
	EncryptionMode = 1, DecryptionMode, DeveloperMode
}Mode;

void init()
{
	reversibility = 1;
	hout = GetStdHandle(STD_OUTPUT_HANDLE);	
}
void gotoxy(int x,int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(hout, pos);
}
int** dynamicAllocII(int Row, int Column)
{
	int **p, i, j;
	p = (int**)malloc(sizeof(int*)*Row);
	for (i = 0; i < Row; i++)
		p[i] = (int*)malloc(sizeof(int)*Column);
	//init matrix=0
	for (i = 0; i < Row; i++)
		for (j = 0; j < Column; j++)
		{
			p[i][j] = 0;
		}
	return p;
}
void askState(int **p)
{
	if (p == NULL)
	{
		printf("Failed to dynamic alloc object!\n");
		exit(1);
	}
}
void titleLayout(char *string)
{
	char a[5][25] = {' '}, i, k = 0;
	for (i = 0; i < 25; i++)
	{
		a[0][i] = '#';
		a[4][i] = '#';
	}
	for (i = (25-strlen(string))/2-1; i < 21; i++)
	{
		a[2][i] = string[k++];
	}
	for (i = 0; i < 5; i++)
	{
		a[i][0] = '#';
		a[i][24] = '#';
	}
	SetConsoleTextAttribute(hout, BACKGROUND_BLUE|FOREGROUND_INTENSITY);
	for (i = 0; i < 5; i++)
	{
		for (k = 0; k < 25; k++)
		{
			printf("%c",a[i][k]);
		}
		printf("\n");
	}
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY);
	printf("\n\n\n");
}
void showMat(int **p)
{
	int i, j;
	for (i = 0; i < (JUDGE(strlen(text))); i++)
	{
		for (j = 0; j < KEYORDER; j++)
		{
			printf("%d	", p[i][j]);
		}
		printf("\n");
	}
}
void show(int **p,int r)
{
	int i, j;
	for (i = 0; i < r; i++)
	{
		for (j = 0; j < KEYORDER; j++)
		{
			printf("%d	", p[i][j]);
		}
		printf("\n");
	}
}
void freeMat(int **p,int _row)
{
	int i;
	for (i = 0; i < _row;i++)
	{
		free(p[i]);
	}
	free(p);
}
void delay(int num)
{
	int i, j;
	for (i = 0; i < num; i++)
		for (i = 0; i < num; i++)
			for (j = 0; j < num; j++)
				for (i = 0; i < num; i++)
				{
					;
				}
}
int CalAij(int(*d)[KEYORDER], int _i, int _j)				
{
	int _d[KEYORDER - 1][KEYORDER - 1], i, j, i_, j_, Mij, Aij;
	for (i_ = 0, i = 1; i <= KEYORDER; i++, i_++)
		for (j_ = 0, j = 1; j <= KEYORDER; j++, j_++)
		{
			if (i == _i) i++;
			if (i > KEYORDER) break;			
			if (j == _j) j++;
			if (j > KEYORDER) break;			
			_d[i_][j_] = d[i - 1][j - 1];
		}
	Mij = _d[0][0] * _d[1][1] - _d[0][1] * _d[1][0];
	Aij = (int)pow(-1, _i + _j)*Mij;
	return Aij;
}
void CalInvKeyMat()
{
	int i, j, result = 0;
	for ( i = 1; i <= KEYORDER; i++)
	{
		result = result + keyMatrix[0][i - 1] * CalAij(keyMatrix, 1, i);
	}
	for (i = 0; i < KEYORDER; i++)
		for (j = 0; j < KEYORDER; j++)
		{
			invKeyMat[j][i] = CalAij(keyMatrix, i + 1, j + 1) / (double)result;
		}
}
void setKeyMat()
{
	int result = 0, i, j, a;
	printf("please input the new key-matrix:\n");
	for (i = 0; i < KEYORDER; i++)
		for (j = 0; j < KEYORDER; j++)
		{
			scanf("%d", &keyMatrix[i][j]);
		}
	for ( a = 1; a <= KEYORDER; a++)
	{
		result = result + keyMatrix[0][a - 1] * CalAij(keyMatrix, 1, a);
	}
	if (result != 0)
	{
		printf("set successfully!\n");
		reversibility = 1;

	}
	else
	{
		printf("matrix is not reversible, which may lead to failure to crack!!!\n");
		reversibility = 0;
	}	
}
void inquireKeyMat()
{
	int i, j;
	printf("the present key-matrix:\n");
	for (i = 0; i < KEYORDER; i++)
	{
		for (j = 0; j < KEYORDER; j++)
		{
			printf("%d	", keyMatrix[i][j]);
		}
		printf("\n");
	}	
}
void getText()
{
	printf("please input the text(the space must be replaced by'^'!):\n");
	scanf("%s", text);
	strlwr(text);
}
int** textToMatrix()
{
	int **mata, i, j, z, k = 0;
	mata = dynamicAllocII(JUDGE(strlen(text)), KEYORDER);
	askState(mata);
	for (i = 0; i < (JUDGE(strlen(text))); i++)
		for (j = 0; j < KEYORDER; j++)
			if (text[k] != '\0')
			{
				for (z = 0; z < strlen(codeList); z++)
					if (codeList[z] == text[k]) 
					{ 
						mata[i][j] = z;
						k++; 
						break;
					}
			}
	return mata;
}
int** encryption(int **matA,int (*matB)[KEYORDER])
{
	int **matC, i, j, z, sum;
	matC = dynamicAllocII(JUDGE(strlen(text)), KEYORDER);
	askState(matC);
	for (i = 0; i < (JUDGE(strlen(text))); i++)			//priority：(A?B:C)<(A<B)!!!
		for (j = 0; j < KEYORDER; j++)
		{
			sum = 0;
			for (z = 0; z < KEYORDER; z++)
					sum = sum + matA[i][z] * matB[z][j];
			matC[i][j] = sum;
		}
	return matC;
}
int** getMatrix()
{
	int **matc, i, j;
	printf("please input the row number(the matrix must be %d column!)：",(int)KEYORDER);
	scanf("%d", &row);
	matc = dynamicAllocII(row, KEYORDER);
	askState(matc);
	printf("Then,please input the ciphert matrix：\n");
	for (i = 0; i < row; i++)
		for (j = 0; j < KEYORDER; j++)
		{
			scanf("%d", &matc[i][j]);
		}
	return matc;
}
int** decryption()						
{
	int **matA;
	int i, j, z;
	double sum;
	matA = dynamicAllocII(row, KEYORDER);
	askState(matA);
	CalInvKeyMat();
	for (i = 0; i < row; i++)			
		for (j = 0; j < KEYORDER; j++)
		{
			sum = 0;
			for (z = 0; z < KEYORDER; z++)
				sum = sum + ciphertMatrix[i][z] * invKeyMat[z][j];
			matA[i][j] = (int)sum;
		}
	return matA;
}
char* matrixToText()
{
	char *results;
	results = (char*)malloc(sizeof(char)*(row*KEYORDER + 1));
	askState(&results);
	int i, j, z, k = 0;
	for (i = 0; i < row; i++)
		for (j = 0; j < KEYORDER; j++)
			for (z = 0; z < strlen(codeList); z++)
				if (z == originalMatrix[i][j])
				{
					results[k] = codeList[z];
					k++;
					break;
				}
	results[k] = '\0';
	return results;
}
void randSetKeyMat()
{
	int i, j, result = 0, a;
	srand((unsigned)time(NULL));
RAND:
	for (i = 0; i < KEYORDER; i++)
		for (j = 0; j < KEYORDER; j++)
		{
			keyMatrix[i][j] = rand() % RANDMAX*pow(-1, rand() % RANDMAX);
		}
	for (a = 1; a <= KEYORDER; a++)
	{
		result = result + keyMatrix[0][a - 1] * CalAij(keyMatrix, 1, a);
	}
	if (result != 0)
	{
		printf("set successfully!\nthe present matrix：\n");
		for (i = 0; i < KEYORDER; i++)
		{
			for (j = 0; j < KEYORDER; j++)
			{
				printf("%d	", keyMatrix[i][j]);
			}
			printf("\n");
		}	
		reversibility = 1;
	}
	else
	{
		reversibility = 0;
		goto RAND;
	}
}
void encryptionMode()
{
	getText();
	originalMatrix = textToMatrix();
	ciphertMatrix = encryption(originalMatrix, keyMatrix);
	printf("the encrypted Matrix：\n");
	showMat(ciphertMatrix);
	freeMat(originalMatrix, JUDGE(strlen(text)));
	freeMat(ciphertMatrix, JUDGE(strlen(text)));
}
void decryptionMode()
{
	char *string;
	if (reversibility == 1)
	{
		ciphertMatrix = getMatrix();
		originalMatrix = decryption();
		string = matrixToText();
		printf("the text：%s\n", string);
		free(string);
		freeMat(originalMatrix, row);
		freeMat(ciphertMatrix, row);
	}
	else
		printf("the key-matrix is not reversible!\nplease change the key-matrix!\n");
}
void eggShell()
{
	int i, x, y, num;
	printf("told me a lucky number：");
	scanf("%d", &num);
	system("mode con cols=70 lines=30");
	srand((unsigned)time(NULL));
	gotoxy(11, 15);
	printf("接下来你将收到来自屏幕里某个小可爱的%d个飞吻！",num);
	delay(2 * REACTIONTIME);
	system("cls");
	for (i = 0; i<num; i++)
	{
		x = rand() % 55;
		y = rand() % 30;
		gotoxy(x, y);
		printf("（づ￣3￣）づ");
		delay(REACTIONTIME);
		system("cls");
	}
	system("mode con cols=100 lines=40");
}
int developerMode()
{
	int tifiy;
	printf("Welcome my dear developer!\n");
	delay(REACTIONTIME);
	printf("Let's me guess...\n");
	delay(REACTIONTIME);
	printf("emmmmm...\n");
	delay(REACTIONTIME);
	printf("so, what do you want to do:\n	inquireKeyMat：1\n	setKeyMat：2\n	randSetKeyMat：3\n	exit：4\n");
	while (1)
	{
		scanf("%d", &tifiy);
		switch (tifiy)
		{
		case 1:inquireKeyMat(); break;
		case 2:setKeyMat(); break;
		case 3:randSetKeyMat(); break;
		case 4:printf("ByeBye~~~\n\n"); return 0;
		case 520:eggShell(); break;
		}
		printf("\nwhat do you want to do next：");
	}
}
void hereIsMe()
{
	int i;
	gotoxy(30, 5);
	for (i = 0; i < 5; i++)
	{
		delay(REACTIONTIME);
		printf("·");
	}
	printf("by mstifiy\n");
	delay(REACTIONTIME);
	gotoxy(0, 2);
	printf("n(*≧▽≦*)n");
	delay(REACTIONTIME);
	gotoxy(0, 9);
}

void main()
{
	Mode mode;
	init();
	gotoxy(0,3);
	titleLayout("MATRIX ENCRYPTION");
	hereIsMe();
	printf("Explain：\nEncryptionMode：1\nDecryptMode：2\n");
	while (1)
	{
		printf("\nplease choose the mode:");
		scanf("%d", &mode);
		switch (mode)
		{
		case 1:system("cls"); printf("\n\n\n\n*******************EncryptionMode*******************\n"); encryptionMode(); break;
		case 2:system("cls"); printf("\n\n\n\n*******************DecryptionMode*******************\n"); decryptionMode(); break;
		case 3:system("cls"); printf("\n\n\n\n*******************DeveloperMode*******************\n"); developerMode(); break;
		}
	}
}
