#pragma once
#include "EasyGraphics.h"
#include <string>
#include <random>
#include <ctime>
#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>




using namespace std;
class FollowMe :	public EasyGraphics
{
public:
	enum direction{none ,left, right, up, down};
	struct tile
	{
		bool visible = true;
		std::wstring filepath = L"assets\\darkSquare.bmp";
		
	};
	wstring temp, gameOver;
	FollowMe();
	~FollowMe();
	bool start = false, cheatMode = false;
	bool validMove = true;
	direction previousMove , userDirection, animation;
	bool userMovement = false;
	wstring message = L"",sprite;
	vector<int> programPath;
	vector<int> userPath;
	int MOVE = 0,moves, currentX,currentY,xPos,yPos,vectorX,vectorY,spriteX,spriteY;
	int currentMove = 0, animationCounter, cheatColour;
	int mx, my, keyPressed, timerVal = 0, ANIMATION = 1, MOVEMENT_TIMER = 2, level = 1, hPosX, hPosY, width = 4, height = 4;
	vector<vector<tile> > squares
	{
		{ tile{},tile{}, tile{}, tile{},tile{},tile{} },
		{ tile{},tile{}, tile{}, tile{},tile{},tile{}},
		{tile{},tile{}, tile{}, tile{},tile{},tile{}},
		{tile{},tile{}, tile{}, tile{},tile{},tile{}},
		{tile{},tile{}, tile{}, tile{},tile{},tile{}},
		{tile{},tile{}, tile{}, tile{},tile{},tile{}}
	};




	virtual void onDraw();
	virtual void resetGrid();
	virtual void generatePath();
	virtual void inputPath();
	virtual void levelPlayer();
	virtual void showPath();
	virtual bool userMoveAndValidate(direction direction);
	virtual void incorretChoice();
	virtual void restart();
	virtual void moveSprite(int startX,int startY,direction direction);


	
	virtual	void onKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	virtual void onMouseMove(UINT nFlags, int x, int y);
	virtual void onLButtonDown(UINT nFlags, int x, int y);
	virtual void onTimer(UINT nIDEvent);


	
	
	


};

