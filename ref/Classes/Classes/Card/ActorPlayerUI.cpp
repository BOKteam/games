/*
 * ActorPlayerUI.cpp
 *
 *  Created on: Oct 12, 2012
 *      Author: root
 */
#include "ActorPlayerUI.h"
#include "ActorPlayer.h"
#include "GameInput.h"
#include "GameCardManage.h"
#include "HomePageControl.h"
ActorPlayerUI::ActorPlayerUI(ActorPlayer* pActorPlayer)
{
	m_pActorPlayer = pActorPlayer;

	m_pGameInput = new GameInput(this,m_pActorPlayer);

	m_pScore_A = ECButton::InitWithFile("Resources/GameUI/Game_BtnNor_CallScore1.png","Resources/GameUI/Game_BtnDown_CallScore1.png",callback_selector(ActorPlayerUI::ClickScore_A),this);
	m_pScore_A->SetPosition((ECPoint)ECPoint(180,420));
	this->AddChild(m_pScore_A,10);

	m_pScore_B = ECButton::InitWithFile("Resources/GameUI/Game_BtnNor_CallScore2.png","Resources/GameUI/Game_BtnDown_CallScore2.png",callback_selector(ActorPlayerUI::ClickScore_B),this);
	m_pScore_B->SetPosition((ECPoint)ECPoint(290,420));
	this->AddChild(m_pScore_B,10);

	m_pScore_C = ECButton::InitWithFile("Resources/GameUI/Game_BtnNor_CallScore3.png","Resources/GameUI/Game_BtnDown_CallScore3.png",callback_selector(ActorPlayerUI::ClickScore_C),this);
	m_pScore_C->SetPosition((ECPoint)ECPoint(400,420));
	this->AddChild(m_pScore_C,10);

	m_pCancelCallBanker = ECButton::InitWithFile("Resources/GameUI/PassLord01.png","Resources/GameUI/PassLord02.png",callback_selector(ActorPlayerUI::ClickCancelBanker),this);
	m_pCancelCallBanker->SetPosition((ECPoint)ECPoint(510,420));
	this->AddChild(m_pCancelCallBanker,10);

	m_pOutCard = ECButton::InitWithFile("Resources/GameUI/Out01.png","Resources/GameUI/Out02.png",callback_selector(ActorPlayerUI::ClickOutCard),this);
	m_pOutCard->SetPosition((ECPoint)ECPoint(260,420));
	this->AddChild(m_pOutCard,10);

	m_pHint = ECButton::InitWithFile("Resources/GameUI/Hint01.png","Resources/GameUI/Hint02.png",callback_selector(ActorPlayerUI::ClickHint),this);
	m_pHint->SetPosition((ECPoint)ECPoint(480,420));
	this->AddChild(m_pHint,10);

	m_pCancelOutCard = ECButton::InitWithFile("Resources/GameUI/Pass01.png","Resources/GameUI/Pass02.png",callback_selector(ActorPlayerUI::ClickCancelOut),this);
	m_pCancelOutCard->SetPosition((ECPoint)ECPoint(370,420));
	this->AddChild(m_pCancelOutCard,10);

	m_eShowType = eST_OutCard;
	HideCardBoard();

	m_eShowType = eST_Banker;
	HideBankerBoard();

	InitInquireQuitPic();
}
ActorPlayerUI::~ActorPlayerUI()
{
	this->RemoveAllChild();
	delete m_pGameInput;
	/*delete m_pScore_A;
	delete m_pScore_B;
	delete m_pScore_C;
	delete m_pCancelCallBanker;
	delete m_pOutCard;
	delete m_pHint;
	delete m_pCancelOutCard;*/
}
void ActorPlayerUI::ClickScore_A()
{
	m_pActorPlayer->SetBankerScore(1);
	HideBankerBoard();
}
void ActorPlayerUI::ClickScore_B()
{
	m_pActorPlayer->SetBankerScore(2);
	HideBankerBoard();
}
void ActorPlayerUI::ClickScore_C()
{
	m_pActorPlayer->SetBankerScore(3);
	HideBankerBoard();
}
void ActorPlayerUI::ClickCancelBanker()
{
	m_pActorPlayer->SetBankerScore(-1);
	HideBankerBoard();
}
void ActorPlayerUI::ClickHint()
{
	m_pActorPlayer->ClickHint();
}
void ActorPlayerUI::ClickOutCard()
{
	m_pActorPlayer->ClickOutCard();
}
void ActorPlayerUI::ClickCancelOut()
{
	m_pActorPlayer->ClickCancelOut();
}
void ActorPlayerUI::ShowScoreBoard(int CurrentScore)
{
	if(m_eShowType == eST_Banker)
		return;

	m_pGameInput->SetFocusCallBanker(CurrentScore);

	if(CurrentScore<1)
	{	m_pScore_A->SetIsDisable(false);m_pScore_A->SetIsVisual(true);}
	if(CurrentScore<2)
	{	m_pScore_B->SetIsDisable(false);m_pScore_B->SetIsVisual(true);}
	if(CurrentScore<3)
	{	m_pScore_C->SetIsDisable(false);m_pScore_C->SetIsVisual(true);}

	m_pCancelCallBanker->SetIsDisable(false);m_pCancelCallBanker->SetIsVisual(true);

	m_eShowType = eST_Banker;
}
void ActorPlayerUI::ShowOutCardBoard()
{
	if(m_eShowType == eST_OutCard)
		return;

	m_pGameInput->SetFocusGame();

	m_pOutCard->SetIsDisable(false);
	m_pOutCard->SetIsVisual(true);

	m_pHint->SetIsDisable(false);
	m_pHint->SetIsVisual(true);

	m_pCancelOutCard->SetIsDisable(false);
	m_pCancelOutCard->SetIsVisual(true);

	m_eShowType = eST_OutCard;
}

void ActorPlayerUI::HideCardBoard()
{
	if(m_eShowType != eST_OutCard)
		return;

	m_pGameInput->DisableFocus();

	m_pOutCard->SetIsDisable(true);
	m_pHint->SetIsDisable(true);
	m_pCancelOutCard->SetIsDisable(true);

	m_pOutCard->SetIsVisual(false);
	m_pHint->SetIsVisual(false);
	m_pCancelOutCard->SetIsVisual(false);

	m_eShowType = eST_None;
}
void ActorPlayerUI::HideBankerBoard()
{
	if(m_eShowType != eST_Banker)
		return;

	m_pGameInput->DisableFocus();

	m_pScore_A->SetIsDisable(true);
	m_pScore_B->SetIsDisable(true);
	m_pScore_C->SetIsDisable(true);
	m_pCancelCallBanker->SetIsDisable(true);

	m_pScore_A->SetIsVisual(false);
	m_pScore_B->SetIsVisual(false);
	m_pScore_C->SetIsVisual(false);
	m_pCancelCallBanker->SetIsVisual(false);

	m_eShowType = eST_None;
}
/////////////////////////////////
void ActorPlayerUI::ClickExit()
{
	 HomePageControl::GetInterface()->InitCursorData();
	 ECView *sc = HomePageControl::GetInterface()->MainMenuSence();
	 ECDirector::Singleton()->SetRunScene(sc);
}
void ActorPlayerUI::InitInquireQuitPic()
{
	InquireForQuit = ECView::InitWithFile("Resources/GameUI/b_sure.png");
	InquireForQuit->SetPosition((ECPoint)ECPoint(300,170));
	this->AddChild(InquireForQuit,15);
	InquireYes = ECButton::InitWithFile("Resources/GameUI/yes.png","Resources/GameUI/yes02.png",callback_selector(ActorPlayerUI::ClickExit),this);
	InquireYes->SetPosition((ECPoint)ECPoint(314,245));
	this->AddChild(InquireYes,16);
	InquireNo = ECButton::InitWithFile("Resources/GameUI/delete.png","Resources/GameUI/delete02.png",callback_selector(ActorPlayerUI::ShowNotInquiQuit),this);
	InquireNo->SetPosition((ECPoint)ECPoint(444,245));
	this->AddChild(InquireNo,16);
	SetInquiQuitForVisual(false);

	ECView* SelectButton = ECView::InitWithFile("Resources/GameUI/select01.png");
	SelectButton->SetPosition((ECPoint)ECPoint(845,425));
	this->AddChild(SelectButton,10);

	m_pExitButton = ECButton::InitWithFile("Resources/GameUI/back01.png","Resources/GameUI/back02.png",callback_selector(ActorPlayerUI::ShowInquiQuit),this);
	m_pExitButton->SetPosition((ECPoint)ECPoint(845,485));
	this->AddChild(m_pExitButton,10);

	printf("Init Exit\n");

	m_bIsQuitShow = false;
}
void ActorPlayerUI::ShowNotInquiQuit()
{

	//ECEventDispatcher::Singleton()->SetIsResponseKeyboard(false);
	SetInquiQuitForVisual(false);
	SetButtonIsEnable(true);
	m_pActorPlayer->GetCardManage()->StopProcess(false);

	m_bIsQuitShow = false;
}
void ActorPlayerUI::ShowInquiQuit()
{

	SetButtonIsEnable(false);
	SetInquiQuitForVisual(true);
	m_pActorPlayer->GetCardManage()->StopProcess(true);

	m_bIsQuitShow = true;
}
void ActorPlayerUI::SetInquiQuitForVisual(bool ShowFlag)
{
	InquireForQuit->SetIsVisual(ShowFlag);
	InquireYes->SetIsVisual(ShowFlag);
	InquireYes->SetIsDisable(!ShowFlag);
	InquireNo->SetIsVisual(ShowFlag);
	InquireNo->SetIsDisable(!ShowFlag);
}
void ActorPlayerUI::SetButtonIsEnable(bool IsEnable)
{
	if(m_eShowType == eST_Banker)
	{
		ECEventDispatcher::Singleton()->SetIsResponseKeyboard(!IsEnable);
		m_pScore_A->SetIsDisable(!IsEnable);
		m_pScore_B->SetIsDisable(!IsEnable);
		m_pScore_C->SetIsDisable(!IsEnable);
		m_pCancelCallBanker->SetIsDisable(!IsEnable);
	}
	else if(m_eShowType == eST_OutCard)
	{
		ECEventDispatcher::Singleton()->SetIsResponseKeyboard(!IsEnable);
		m_pOutCard->SetIsDisable(!IsEnable);
		m_pHint->SetIsDisable(!IsEnable);
		m_pCancelOutCard->SetIsDisable(!IsEnable);
	}

	m_pExitButton->SetIsDisable(!IsEnable);
}




