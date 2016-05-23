#pragma once
#include <Windows.h>
#include "gameInfo.h"
#include "ControlMouse.h"
#include "define.h"
#include "processImage.h"
bool cmp(Card a, Card b)
{
	return a.spend < b.spend;
}
class AI
{
	HWND hWnd;
	ControlMouse *controlMouse;
	int havePlayNum;
public:
	AI(HWND h) :hWnd(h){
		controlMouse = ControlMouse::getInstance();
	}
	void startGame(GameInfo &gameInfo)
	{
		cout << "��Ϸ�׶Σ���ʼ�µ�һ����ϰģʽ,ѡ����ʿ�������ʼ��ť����ʱ1��" << endl;
		//��ϰģʽ��
		controlMouse->touchPosition(835,655);//���ѡ��ť
		controlMouse->touchPosition(845, 495);//ѡ����ʿ
		controlMouse->touchPosition(850, 645);//�����ʼ��ť
		Sleep(1000);//��ʱ1s
		gameInfo.state = STATE_CHANGECARDSTART;
	}
	void changeCardStart(GameInfo &gameInfo)
	{
		cout << "��Ϸ�׶Σ����ƽ׶ο�ʼ" << endl;
		cout << "ʶ��������" << gameInfo.currentNum << endl;
		if (gameInfo.currentNum == 3)
		{
			gameInfo.first = true;
		}
		else if (gameInfo.currentNum == 4)
		{
			gameInfo.first = false;
		}
		else
		{
			cout << "���Ƹ���ʶ�����" << endl;
			return;//ʶ�����
		}
		string NoChangeCard[4] = { "������͸��", "���٪��", "���Ŀ�ѧ��", "����������" };
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
				cout << "�������ƣ�" << gameInfo.handCard[i].name << endl;
				controlMouse->touchPosition(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
			}
		}
		gameInfo.state = STATE_CHANGECARDEND;
	}
	
	void process(GameInfo &gameInfo)
	{
		switch (gameInfo.state)
		{
		case STATE_STARTGAME:
			startGame(gameInfo);
			break;
		case STATE_CHANGECARDSTART:
			changeCardStart(gameInfo);			
			break;
		case STATE_CHANGECARDEND:
			cout << "��Ϸ�׶Σ����ƽ׶ν��������ȷ����ť" << endl;
			//ȷ��
			controlMouse->touchPosition(500, 600);
			gameInfo.state = STATE_FIGHTSTART;
			if(gameInfo.first) Sleep(15000);//�ȴ����ƽ���
			else Sleep(10000);
			break;
		case STATE_FIGHTSTART:
			cout << "��Ϸ�׶Σ���ʼս���׶Σ���ʱ10��" << endl;
			Sleep(2000);
			gameInfo.state = STATE_OTHERTURN;
			break;
		case STATE_SELFTURN_PLAY:
			cout << "��Ϸ�׶Σ��Լ����ƽ׶�" << endl;
			cout << "��ǰ���ã�" << gameInfo.currentSpend << " ���÷���:" << gameInfo.couldUseSpend << endl;
			
			//�����ư���������
			sort(gameInfo.handCard, gameInfo.handCard + gameInfo.currentNum, cmp);
			/*for (int i = 0; i < gameInfo.currentNum; ++i)
			{
				cout << gameInfo.handCard[i].name;
			}*/
			//if (gameInfo.currentSpend == 1)//1�����
			{
				
				for (int i = 0; i < gameInfo.currentNum; ++i)
				{
					if (!gameInfo.first && gameInfo.handCard[i].behavior == "�³�" && gameInfo.handCard[i].spend == gameInfo.couldUseSpend + 1)
					{
						controlMouse->playCard(gameInfo.handCard[0].x, gameInfo.handCard[0].y);//���˱�
						gameInfo.currentNum -= 1;
						gameInfo.first = true;//���˱ҳ���
						++gameInfo.couldUseSpend;
						cout << "�����˱ң�����ʶ������λ��" << endl;
						return;
						
						//cout << "ai:���ƣ�����" << gameInfo.handCard[i].spend << " ���÷��ã�" << gameInfo.couldUseSpend << " λ�ã�" << gameInfo.handCard[i].x << " " << gameInfo.handCard[i].y << endl;
						//controlMouse->playCard(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
						//gameInfo.couldUseSpend -= gameInfo.handCard[i].spend;
						//++havePlayNum;//�ѳ�������+1�����������˱�
						//Sleep(100);
						//break;
					}
					if (gameInfo.handCard[i].behavior == "�³�" && gameInfo.handCard[i].spend <= gameInfo.couldUseSpend)
					{
						if (gameInfo.handCard[i].type == "����")
						{
							if (gameInfo.haveWeapon) continue;
							else gameInfo.haveWeapon = true;
						}
						cout << "ai:���ƣ�" << gameInfo.handCard[i].name<<" ����" << gameInfo.handCard[i].spend << " ���÷��ã�" << gameInfo.couldUseSpend << " λ�ã�" << gameInfo.handCard[i].x << " " << gameInfo.handCard[i].y << endl;
						controlMouse->playCard(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
						gameInfo.couldUseSpend -= gameInfo.handCard[i].spend;
						++havePlayNum;//�ѳ�������+1�����������˱�
						if (gameInfo.couldUseSpend + !gameInfo.first < gameInfo.handCard[i].spend) break;//�����ڳ�����������ɨ��
						return;//����ɨ�裬ȷ������λ��
						
					}
				
					
					
				}

				for (int i = 0; i < gameInfo.currentNum; ++i)
				{
					if (gameInfo.handCard[i].behavior == "����" && gameInfo.handCard[i].spend <= gameInfo.couldUseSpend)
					{
						cout << "ai:���ƣ�" << gameInfo.handCard[i].name<<" ����" << gameInfo.handCard[i].spend << " ���÷��ã�" << gameInfo.couldUseSpend << " λ�ã�" << gameInfo.handCard[i].x << " " << gameInfo.handCard[i].y << endl;
						controlMouse->playCardToPlayer(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
						gameInfo.couldUseSpend -= gameInfo.handCard[i].spend;
						++havePlayNum;//�ѳ�������+1�����������˱�
						if (gameInfo.couldUseSpend + !gameInfo.first < gameInfo.handCard[i].spend) break;//�����ڳ�����������ɨ��
						return;//����ɨ�裬ȷ������λ��
					}
				}
			}
			
			//for (int i = 0; i < gameInfo.currentNum; ++i)//��򵥵�AI����ɶ��ɶ
			//{
			//	if (gameInfo.handCard[i].behavior == "�³�" && gameInfo.handCard[i].spend!=0 && gameInfo.handCard[i].spend <= gameInfo.couldUseSpend)
			//	{
			//		cout << "ai:���ƣ�����" << gameInfo.handCard[i].spend << " ���÷��ã�" << gameInfo.couldUseSpend << " λ�ã�" << gameInfo.handCard[i].x << " " << gameInfo.handCard[i].y << endl;
			//		controlMouse->playCard(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
			//		gameInfo.couldUseSpend -= gameInfo.handCard[i].spend;
			//		Sleep(100);
			//	}
			//}

			gameInfo.state = STATE_SELFTURN_FIGHT;
			
			controlMouse->moveToPosition(OutSideX, OutSideY);
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
						cout << "����ɨ�賡�����" << endl;
						return;
					}
				}
				if (!flag)
				{
					controlMouse->playCardToPlayer(gameInfo.selfMonster[i].x, gameInfo.selfMonster[i].y);
					cout << gameInfo.selfMonster[i].name << "�������" << endl;
				}
			}
			
	
			controlMouse->touchPosition(HeroSkillX, HeroSkillY);//ʹ��Ӣ�ۼ���
			gameInfo.state = STATE_OTHERTURN;//�ı���Ϸ״̬
			gameInfo.currentSpend++;
			gameInfo.couldUseSpend = gameInfo.currentSpend;
			havePlayNum = 0;
			controlMouse->touchPosition(EndTurnX, EndTurnY);//����غϽ���
			
			controlMouse->moveToPosition(OutSideX, OutSideY);//�ƶ��������⣬��ֹ����
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