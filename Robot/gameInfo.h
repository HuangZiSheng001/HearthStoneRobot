#pragma once
#include <iostream>
#include "define.h"
class Card
{
public:
	int spend;
	int attack;
	int life;
	int type;
	int x, y;
};
class GameInfo
{
public:
	GameInfo()
	{
		state = STATE_STARTGAME;
		currentSpend = 0;
	}
	STATE state;
	Card handCard[10];//���ƴ�С
	int currentNum;//������Ŀ
	bool first;//�Ⱥ���
	int currentSpend;//��ǰˮ����
	int couldUseSpend;

};
