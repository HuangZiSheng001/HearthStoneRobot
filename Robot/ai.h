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
		controlMouse = ControlMouse::getInstance();
	}
	void process(GameInfo &gameInfo)
	{
		string NoChangeCard[4] = { "������͸��", "���٪��", "���Ŀ�ѧ��", "����������" };
		switch (gameInfo.state)
		{
		case STATE_STARTGAME:
			cout << "��Ϸ�׶Σ���ʼ��Ϸ,�����ʼ��ť����ʱ1��" << endl;
			controlMouse->touchPosition(800, 600);//�����ʼ
			Sleep(1000);
			gameInfo.state = STATE_CHANGECARDSTART;
			break;
		case STATE_CHANGECARDSTART:
			cout << "��Ϸ�׶Σ����ƽ׶ο�ʼ��ʶ��������" << gameInfo.currentNum << endl;
			if (gameInfo.currentNum < 3) break;
			if (gameInfo.currentNum == 3) gameInfo.first = true;
			else gameInfo.first = false;
			
			for (int i = 0; i < gameInfo.currentNum; ++i)
			{
				int flag = false;
				for (int j = 0; j < 4; ++j)
				{
					if (gameInfo.handCard[i].name == NoChangeCard[j])
					{
						flag = true;
					}
				}
				if (!flag)
				{
					cout << "��������,��ʱ0.1�룺" << gameInfo.handCard[i].name << endl;
					controlMouse->touchPosition(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
					Sleep(100);
				}
			}
			gameInfo.state = STATE_CHANGECARDEND;
			break;
		case STATE_CHANGECARDEND:
			cout << "��Ϸ�׶Σ����ƽ׶ν��������ȷ����ť����ʱ3��" << endl;
			//ȷ��
			controlMouse->touchPosition(500, 600);
			Sleep(3000);
			gameInfo.state = STATE_FIGHTSTART;
			break;
		case STATE_FIGHTSTART:
			cout << "��Ϸ�׶Σ���ʼս���׶Σ���ʱ10��" << endl;
			Sleep(10000);//�ȴ����ƽ���
			gameInfo.state = STATE_OTHERTURN;
			break;
		case STATE_SELFTURN_PLAY:
			cout << "��Ϸ�׶Σ��Լ����ƽ׶Σ���ʱ2��" << endl;
			Sleep(2000);
			gameInfo.currentSpend++;
			gameInfo.couldUseSpend = gameInfo.currentSpend;
			for (int i = 0; i < gameInfo.currentNum; ++i)//��򵥵�AI����ɶ��ɶ
			{
				if (gameInfo.handCard[i].behavior == "�³�" && gameInfo.handCard[i].spend <= gameInfo.couldUseSpend)
				{
					cout << "ai:���ƣ�����" << gameInfo.handCard[i].spend << "���÷��ã�" << gameInfo.couldUseSpend << "λ�ã�" << gameInfo.handCard[i].x << " " << gameInfo.handCard[i].y << endl;
					controlMouse->playCard(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
					gameInfo.couldUseSpend -= gameInfo.handCard[i].spend;
					Sleep(100);
				}
			}

			/*for (int i = 0; i < 15; ++i)
			{
				controlMouse->playCardToPlayer(140+60*i,460);
				Sleep(500);
			}*/
		//	controlMouse->touchPosition(900, 350);//����غϽ���
			gameInfo.state = STATE_SELFTURN_FIGHT;
			
			controlMouse->moveToPosition(1000, 700);
			Sleep(1000);
			break;
		case STATE_SELFTURN_FIGHT:
			cout << "��Ϸ�׶Σ�������ӽ׶�" << endl;
			for (int i = 0; i < gameInfo.selfMonsterNum; ++i)
			{
				bool flag = false;
				for (int j = 0; j < gameInfo.otherMonsterNum; ++j)
				{
					if (gameInfo.otherMonster[j].taugh)
					{
						flag = true;
						controlMouse->fightMonster(gameInfo.selfMonster[i].x, gameInfo.selfMonster[i].y, gameInfo.otherMonster[j].x, gameInfo.otherMonster[j].y);
						cout << gameInfo.selfMonster[i].name << "����������λ�ã�" << gameInfo.otherMonster[j].x<<gameInfo.otherMonster[j].y << endl;
						break;
					}
				}
				if (flag)
				{
					cout << "����ɨ��Է��������" << endl;
					ProcessImage *processImage = ProcessImage::getInstance();
					Mat src = processImage->getCurrentImage();
					Mat otherFloor;
					src(Rect(204, 229, 630, 118)).copyTo(otherFloor);
					processImage->recoOtherMonster(otherFloor, gameInfo);
				}
				else
				{
					controlMouse->playCardToPlayer(gameInfo.selfMonster[i].x, gameInfo.selfMonster[i].y);
					cout << gameInfo.selfMonster[i].name << "�������" << endl;
				}
			}
			
	
			controlMouse->touchPosition(650, 620);//ʹ��Ӣ�ۼ���
			gameInfo.state = STATE_OTHERTURN;//�ı���Ϸ״̬
			controlMouse->touchPosition(900, 350);//����غϽ���
			
			controlMouse->moveToPosition(1000, 700);//�ƶ��������⣬��ֹ����
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