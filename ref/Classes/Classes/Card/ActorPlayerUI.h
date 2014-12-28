/*
 * ActorPlayerUI.h
 *
 *  Created on: Oct 12, 2012
 *      Author: root
 */

#ifndef ACTORPLAYERUI_H_
#define ACTORPLAYERUI_H_
#include "EnveeCore.h"
using namespace NSEnveeCore;
class ActorPlayer;
class GameInput;
enum EShowType
{
	eST_None = 0,
	eST_Banker,
	eST_OutCard,

	eST_Count,
};
class ActorPlayerUI : public ECNode
{
public:
	void ClickScore_A();
	void ClickScore_B();
	void ClickScore_C();
	void ClickCancelBanker();
	void ClickHint();
	void ClickOutCard();
	void ClickCancelOut();
public:
	void ShowScoreBoard(int CurrentScore);
	void ShowOutCardBoard();
	void HideCardBoard();
	void HideBankerBoard();

	//virtual void Draw();

	ActorPlayerUI(ActorPlayer* pActorPlayer);
	~ActorPlayerUI();

	////////////////////////
	void InitInquireQuitPic();
	void ShowNotInquiQuit();
	void ShowInquiQuit();
	void SetInquiQuitForVisual(bool ShowFlag);
	bool GetIsQuitShow(){return m_bIsQuitShow;}
	void ClickExit();
	void SetButtonIsEnable(bool IsEnable);
private:
	bool m_bIsQuitShow;
	ECButton *InquireYes,*InquireNo,*m_pExitButton;
	ECView   *InquireForQuit;
private:
	ActorPlayer* m_pActorPlayer;
	GameInput* m_pGameInput;
	EShowType m_eShowType;
private:
	ECButton* m_pScore_A;
	ECButton* m_pScore_B;
	ECButton* m_pScore_C;
	ECButton* m_pCancelCallBanker;
	ECButton* m_pOutCard;
	ECButton* m_pHint;
	ECButton* m_pCancelOutCard;
};



#endif /* ACTORPLAYERUI_H_ */
