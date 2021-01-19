#include "FollowMe.h"



FollowMe::FollowMe()
{
	srand((int)time(0));
	setImmediateDrawMode(false);

}


FollowMe::~FollowMe()
{
}

void FollowMe::onTimer(UINT nIDEvent)
{
	if (nIDEvent == ANIMATION)
	{
		if (animationCounter < 51)
		{
			switch (animation)
			{
			case FollowMe::left:
				spriteX -= 1;
				break;
			case FollowMe::right:
				spriteX += 1;
				break;
			case FollowMe::up:
				spriteY -= 1;
				break;
			case FollowMe::down:
				spriteY += 1;
				break;
			default:
				break;
			}
			animationCounter += 1;
			onDraw();
		}
		else
			killTimer(ANIMATION);
	}
	if (nIDEvent == MOVE)
	{
		generatePath();
	}
	
	
}

void FollowMe::onDraw()
{
	clearScreen(WHITE);
	drawBitmap(L"assets\\background.bmp",0, 0, 700, 700);
	drawBitmap(L"assets\\logo.bmp", 25, 25, 300, 75);
	setBackColour(GREEN);
	setPenColour(BLACK, 5);
	
	drawRectangle(50, 575, 200, 50, true);
	drawRectangle(300, 575, 200, 50, true);
	setTextColour(BLACK);
	setFont(25, L"Helvetica");
	drawText("START", 95, 580);
	drawText("RESTART", 325, 580);
	drawText(message.c_str(), 250, 150);
	

	for (int i = 0; i < squares.size(); i++) {
		for (int j = 0; j < squares[i].size(); j++)
		{
			if (squares[i][j].visible == true)
				drawBitmap((squares[i][j].filepath).c_str(), (25 + i * 50), (125 + j * 50), 50, 50);
		}
		
	}

	
	drawBitmap(gameOver.c_str(), 150, 200, 400, 300); // GAME OVER IMAGE
	drawBitmap(sprite.c_str(), spriteX, spriteY, 40, 40); //SPRITE

	setPenColour(cheatColour, 5);
	if (userMovement == true && cheatMode)
	{
		for (int i = 0; i < ((programPath.size()) / 2); i++)
		{
			if (((i * 2) + 3) < programPath.size())
			{
				int startX = programPath[i * 2];
				int startY = programPath[(i * 2) + 1];
				int endX = programPath[(i * 2) + 2];
				int endY = programPath[(i * 2) + 3];
				drawLine((25 + startX * 50)+25, (125 + startY * 50)+25, (25 + endX * 50)+25, (125 + endY * 50)+25);
			}
		}
	}


	
	ifstream inFile;
	inFile.open("assets\\highscore.txt");
	string myText;
	inFile >> myText;
	inFile.close();
	drawRectangle(50, 515, 300, 50, true);
	drawText("HIGH SCORE:", 60, 520);
	drawText(myText.c_str(), 285, 520);

	EasyGraphics::onDraw();

}
void FollowMe::onMouseMove(UINT nFlags, int x, int y)
{
	mx = x;
	my = y;
	onDraw();
}

void FollowMe::onKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	keyPressed = nChar;
	if (userMovement == true && keyPressed != 67)
	{
		
		if (currentMove < level + 3) // WITHIN CORRECT NUMBER OF MOVES FOR THE LEVEL
		{
			bool validationCheck;
			if (keyPressed == 38) //UP
			{
				validationCheck = userMoveAndValidate(up);
			}
			else if (keyPressed == 40) //DOWN
			{
				validationCheck = userMoveAndValidate(down);
			}
			else if (keyPressed == 39) //RIGHT
			{
				validationCheck = userMoveAndValidate(right);
			}
			else if (keyPressed == 37) //LEFT
			{
				validationCheck = userMoveAndValidate(left);
			}
			if (!validationCheck) // INCORRECT MOVEMENT
			{
				incorretChoice();
			}

		}
		else if (currentMove == level + 3)// REACHED LAST MOVE WITHOUT FAILURE
		{
			int temp = level; // STORES LEVEL DURING RESET
			restart();
			level = temp + 1;// GIVES CORRECT LEVEL VALUE
			resetGrid();
			showPath();
		}
	}
	if (keyPressed == 67)
		if (cheatMode)
			cheatMode = false;
		else
			cheatMode = true;

	onDraw();
}





void FollowMe::resetGrid()
{
	if (level <5)
	{
		width = 4;
		height = 4;
	}else if (level < 10)
	{
		width = 5;
		height = 5;
	}else
	{
		width = 6;
		height = 6;
	}
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
			squares[i][j].visible = false; //RESETS ALL SQUARES AS NON VISIBLE			
	}
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			squares[i][j].visible = true;
			squares[i][j].filepath = L"assets\\darkSquare.bmp";//SETS REQUIRED SQUARES AS VISIBLE
		}
	}
	sprite = L"";
}








void FollowMe::onLButtonDown(UINT nFlags, int x, int y)
{
	
	if (mx > 50 && mx < 250 && my>575 && my < 625 && start == false)
	{
		//START
		levelPlayer();
	}
	
	else if (mx > 300 && mx < 500 && my>575 && my < 625)
	{
		//RESTART
		restart();
		resetGrid();
	}
	
	onDraw();
}

void FollowMe::showPath()
{
	previousMove = none;
	currentX = (rand() % width);
	currentY = (rand() % height);
	squares[currentX][currentY].filepath = L"assets\\lightSquare.bmp";
	programPath.insert(programPath.end(), currentX);
	programPath.insert(programPath.end(), currentY);
	moves = level + 2;
	onDraw();
	setTimer(MOVE, 1000);
	onDraw();

}

void FollowMe::levelPlayer()
{
	start = true;
	//LEVEL
	resetGrid();
	showPath();
	
	

}

void FollowMe::generatePath()
{
	
	if (moves > 0)
	{
		moves -= 1;
		bool validMove = true;
		do
		{
			validMove = true;
			int pos = (rand() % 4);
			if (pos == 0)
			{
				// MOVE UP
				if (((currentY - 1) > -1) && (previousMove != down))
				{
					//VALID MOVEMENT
					squares[currentX][(currentY - 1)].filepath = L"assets\\lightSquare.bmp";
					moveSprite(currentX, currentY, up);
					currentY -= 1;
					previousMove = up;
				}
				else
				{
					//INVALID MOVEMENT
					validMove = false;
				}
			}
			else if (pos == 1)
			{
				// MOVE RIGHT
				if (((currentX + 1) < width) && (previousMove != left))
				{
					//VALID MOVEMENT
					squares[currentX + 1][(currentY)].filepath = L"assets\\lightSquare.bmp";
					moveSprite(currentX, currentY, right);
					currentX += 1;
					previousMove = right;
				}
				else
				{
					//INVALID MOVEMENT
					validMove = false;
				}
			}
			else if (pos == 2)
			{
				// MOVE DOWN
				if (((currentY + 1) < height) && (previousMove != up))
				{
					//VALID MOVEMENT
					squares[currentX][(currentY)+1].filepath = L"assets\\lightSquare.bmp";
					moveSprite(currentX, currentY, down);
					currentY += 1;
					previousMove = down;
				}
				else
				{
					//INVALID MOVEMENT
					validMove = false;
				}
			}
			else if (pos == 3)
			{
				// MOVE LEFT
				if (((currentX - 1) > -1) && previousMove != right)
				{
					//VALID MOVEMENT
					squares[currentX - 1][(currentY)].filepath = L"assets\\lightSquare.bmp";
					moveSprite(currentX, currentY, left);
					currentX -= 1;
					previousMove = left;
				}
				else
				{
					//INVALID MOVEMENT
					validMove = false;
				}
			}
			
		} while (!validMove);
		programPath.insert(programPath.end(),currentX);
		programPath.insert(programPath.end(), currentY);
	}
	else
	{
		killTimer(ANIMATION);
		resetGrid();
		killTimer(MOVE);
		onDraw();
		inputPath();
	}
	onDraw();
}

void FollowMe::inputPath()
{
	
	bool correctMovement = true;
	
	
	vectorX = currentMove * 2;
	vectorY = (currentMove * 2) + 1;
	xPos = programPath[vectorX];
	yPos = programPath[vectorY];
	squares[xPos][yPos].filepath = L"assets\\lightSquare.bmp";
	spriteX = (25 + xPos * 50) + 5;
	spriteY = (125 + yPos * 50) + 5;
	sprite = L"assets\\sprite.bmp";
	currentMove += 1;
	bool correct = true;
	userMovement = true;
	
	

	
		

	
}

bool FollowMe::userMoveAndValidate(direction direction)
{
	bool myReturn;
	
	vectorX = currentMove * 2;
	vectorY = (currentMove * 2) + 1;
	xPos = programPath[vectorX];
	yPos = programPath[vectorY];
	switch (direction)
	{
	case FollowMe::left:
		
		if (xPos == (programPath[vectorX - 2] - 1))
		{
			squares[xPos][yPos].filepath = L"assets\\lightSquare.bmp";
			moveSprite((programPath[vectorX - 2]), yPos, left);
			currentMove += 1;
			myReturn = true;
		}
		else
			myReturn = false;
		break;
	case FollowMe::right:
		if (xPos == (programPath[vectorX - 2] +1))
		{
			squares[xPos][yPos].filepath = L"assets\\lightSquare.bmp";
			moveSprite((programPath[vectorX - 2]), yPos, right);
			currentMove += 1;
			myReturn = true;
		}
		else
			myReturn = false;
		break;
	case FollowMe::up:
		if (yPos == (programPath[vectorY - 2] - 1))
		{
			squares[xPos][yPos].filepath = L"assets\\lightSquare.bmp";
			moveSprite(xPos, (programPath[vectorY - 2] ), up);
			currentMove += 1;
			myReturn = true;
		}
		else
			myReturn = false;
		break;
	case FollowMe::down:
		if (yPos == (programPath[vectorY - 2] + 1))
		{
			squares[xPos][yPos].filepath = L"assets\\lightSquare.bmp";
			moveSprite(xPos, (programPath[vectorY - 2] ), down);
			currentMove += 1;
			myReturn = true;
		}
		else
			myReturn = false;
		break;
	default:
		break;
	
	}
	return myReturn;
}

void FollowMe::incorretChoice()
{
	userMovement = false;
	start = false;
	gameOver = L"assets\\gameOver.bmp";
	ofstream inFile;
	inFile.open("assets\\highscore.txt");
	inFile << to_string(level);
	inFile.close();

}

void FollowMe::restart()
{
	userMovement = false;
	start = false;
	currentMove = 0;
	gameOver = L"";
	level = 1;
	width = 4;
	height = 4;
	userPath.erase(userPath.begin(), userPath.end());
	programPath.erase(programPath.begin(), programPath.end()); 
	killTimer(MOVE);
	sprite = L"";
	killTimer(ANIMATION);
	animationCounter = 0;

}

void FollowMe::moveSprite(int startX, int startY, direction direction)
{
	spriteX = (25 + startX * 50)+5;
	spriteY = (125 + startY * 50)+5;
	sprite = L"assets\\sprite.bmp";
	animation = direction;
	animationCounter = 0;
	setTimer(ANIMATION, 10);
	onDraw();
	
}