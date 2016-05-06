#pragma once
#include <Windows.h>
#include "gameInfo.h"
#include "ControlMouse.h"
#include "define.h"
class AI
{
	HWND hWnd;
	ControlMouse *controlMouse;
public:
	AI(HWND h) :hWnd(h){
		controlMouse = new ControlMouse(hWnd);
	}
	void process(GameInfo &gameInfo)
	{
		switch (gameInfo.state)
		{
		case STATE_STARTGAME:
			cout << "��Ϸ�׶Σ���ʼ��Ϸ" << endl;
			controlMouse->touchPosition(800, 600);//�����ʼ
			Sleep(1000);
			gameInfo.state = STATE_CHANGECARDSTART;
			break;
		case STATE_CHANGECARDSTART:
			cout << "��Ϸ�׶Σ����ƽ׶ο�ʼ" << endl;
			if (gameInfo.currentNum == 3) gameInfo.first = true;
			else gameInfo.first = false;
			for (int i = 0; i < gameInfo.currentNum; ++i)
			{
				if (gameInfo.handCard[i].spend >= 2)
				{
					controlMouse->touchPosition(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
					Sleep(100);
				}
			}
			gameInfo.state = STATE_CHANGECARDEND;
			break;
		case STATE_CHANGECARDEND:
			cout << "��Ϸ�׶Σ����ƽ׶ν���" << endl;
			//ȷ��
			controlMouse->touchPosition(500, 600);
			Sleep(100);
			gameInfo.state = STATE_FIGHTSTART;
			break;
		case STATE_FIGHTSTART:
			cout << "��Ϸ�׶Σ���ʼս���׶�" << endl;
			Sleep(5000);//�ȴ����ƽ���
			gameInfo.state = STATE_OTHERTURN;
			break;
		case STATE_SELFTURN:
			cout << "��Ϸ�׶Σ��Լ����ƽ׶�" << endl;
			gameInfo.currentSpend++;
			gameInfo.couldUseSpend = gameInfo.currentSpend;
			for (int i = 0; i < gameInfo.currentNum; ++i)
			{
				if (gameInfo.handCard[i].spend <= gameInfo.couldUseSpend)
				{
					cout << "ai:���ƣ�����" << gameInfo.handCard[i].spend << "λ�ã�" << gameInfo.handCard[i].x << " " << gameInfo.handCard[i].y << endl;
					controlMouse->playCard(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
					gameInfo.couldUseSpend -= gameInfo.handCard[i].spend;
					Sleep(100);
				}
			}

			for (int i = 0; i < 10; ++i)
			{
				controlMouse->playCardToPlayer(140+60*i,460);
			}
			controlMouse->touchPosition(900, 350);//����غϽ���
			gameInfo.state = STATE_OTHERTURN;
			break;
		case STATE_OTHERTURN:
			cout << "��Ϸ�׶Σ����ֳ��ƽ׶�" << endl;
			break;
		default:
			break;
		}
		//controlMouse->moveToPosition(OutsideX, OutsideY);
	}
};