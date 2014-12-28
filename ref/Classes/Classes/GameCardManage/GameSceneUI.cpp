/*
 * GameSceneUI.cpp
 *
 *  Created on: Nov 16, 2011
 *      Author: root
 */
#include "GameSceneUI.h"
#include "HomePageControl.h"
#include "GameScene.h"
#include "GameActorScoreManage.h"
#include "ActorPlayer.h"
#include "ActorPlayerUI.h"
#include "GameCardManage.h"
GameSceneUI::GameSceneUI(GameScene* pGameScene)
{
	m_pGameScene = pGameScene;

	Init();

	m_bIsPlay = false;
}
void GameSceneUI::Init()
{
	InitSceneBackground();
	InitScoreBoard();
	InitResultBoard();
}
void GameSceneUI::InitSceneBackground()
{
	ECView* Background = ECView::InitWithFile("Resources/GameUI/bg.png");
	this->AddChild(Background,-10);

	ECView* Chair_A = ECView::InitWithFile("Resources/GameUI/Player.png");
	Chair_A->SetPosition(ECPoint(20,570));
	this->AddChild(Chair_A,5);

	ECView* Chair_B = ECView::InitWithFile("Resources/GameUI/Computer_A.png");
	Chair_B->SetPosition(ECPoint(680,30));
	this->AddChild(Chair_B,5);

	ECView* Chair_C = ECView::InitWithFile("Resources/GameUI/Computer_B.png");
	Chair_C->SetPosition(ECPoint(10,30));
	this->AddChild(Chair_C,5);
	char path[64];

	for(int i=1;i<=BOMB_EFFECT_COUNT;i++)
	{
		sprintf(path,"Resources/Effect/%d.png",i);
		m_pBombList[i-1] = CardImageManager::Singleton()->GetView(path);
	}
}
void GameSceneUI::InitResultBoard()
{
	m_pWinView = ECView::InitWithFile("Resources/GameUI/Win.png");
	m_pWinView->SetPosition(ECPoint(195,150));
	m_pGameScene->GetGameFrontUI()->AddChild(m_pWinView,10);

	m_pLoseView = ECView::InitWithFile("Resources/GameUI/Lose.png");
	m_pLoseView->SetPosition(ECPoint(195,150));
	m_pGameScene->GetGameFrontUI()->AddChild(m_pLoseView,10);

	HideResult();
}
void GameSceneUI::InitScoreBoard()
{
	 SDL_Color Wincolor = {255,123,123};
	 m_pCurrentScore = ECLable::InitWithString("Resources/GameUI/ZiTi.ttf","0",Wincolor,25);
	 m_pCurrentScore->SetPosition((ECPoint)ECPoint(940,350));
	 this->AddChild(m_pCurrentScore,10);

	 m_pComputerScore_A = ECLable::InitWithString("Resources/GameUI/ZiTi.ttf","0",Wincolor,25);
	 m_pComputerScore_A->SetPosition((ECPoint)ECPoint(920,236));
	 this->AddChild(m_pComputerScore_A,10);

	 m_pPlayerScore = ECLable::InitWithString("Resources/GameUI/ZiTi.ttf","0",Wincolor,25);
	 m_pPlayerScore->SetPosition((ECPoint)ECPoint(920,182));
	 this->AddChild(m_pPlayerScore,10);

	 m_pComputerScore_B = ECLable::InitWithString("Resources/GameUI/ZiTi.ttf","0",Wincolor,25);
	 m_pComputerScore_B->SetPosition((ECPoint)ECPoint(920,293));
	 this->AddChild(m_pComputerScore_B,10);

	 Schedule(schedule_selector(GameSceneUI::UpdateScoreBoard),0.2f);
}
void GameSceneUI::UpdateScoreBoard(float dt)
{
	 char Score[16] = {0};
	 sprintf(Score, "%d",GameActorScoreManage::Singleton()->GetCurrentScore());
	 m_pCurrentScore->SetString(Score);

	 sprintf(Score, "%d",GameActorScoreManage::Singleton()->GetActorScore(0));
	 m_pPlayerScore->SetString(Score);

	 sprintf(Score, "%d",GameActorScoreManage::Singleton()->GetActorScore(1));
	 m_pComputerScore_A->SetString(Score);

	 sprintf(Score, "%d",GameActorScoreManage::Singleton()->GetActorScore(2));
	 m_pComputerScore_B->SetString(Score);
}
void GameSceneUI::PlayBombAnimtion(float nFrameTime)
{
	m_nCurrentFrameID = 0;
	m_nFrameTime = nFrameTime;
	m_nCountTime = 0.0f;
	m_bIsPlay = true;
}
void GameSceneUI::UpdateAnimation(evTime dt)
{
	if(!m_bIsPlay)
		return;

	m_nCountTime += dt;

	m_pBombList[m_nCurrentFrameID]->SetPosition(ECPoint(230,0));
	m_pBombList[m_nCurrentFrameID]->Draw();

	if(m_nCountTime<m_nFrameTime)
		return;
	m_nCountTime -= m_nFrameTime;
	m_nCurrentFrameID++;

	if(m_nCurrentFrameID>=(BOMB_EFFECT_COUNT-1))
		m_bIsPlay = false;
}
void GameSceneUI::Update(evTime dt)
{
	ECNode::Update(dt);

	UpdateAnimation(dt);
}
void GameSceneUI::Win()
{
	m_pWinView ->SetIsVisual(true);

	((ActorPlayer*)m_pGameScene->GetGameCardManage()->Actor[0])->GetActorPlayerUI()->SetButtonIsEnable(false);
}

void GameSceneUI::Lose()
{
	m_pLoseView ->SetIsVisual(true);

	((ActorPlayer*)m_pGameScene->GetGameCardManage()->Actor[0])->GetActorPlayerUI()->SetButtonIsEnable(false);
}
bool GameSceneUI::GetIsShowResult()
{
	return m_pWinView->GetIsVisual()||m_pLoseView->GetIsVisual();
}
void GameSceneUI::HideResult()
{
	((ActorPlayer*)m_pGameScene->GetGameCardManage()->Actor[0])->GetActorPlayerUI()->SetButtonIsEnable(true);

	m_pWinView ->SetIsVisual(false);
	m_pLoseView ->SetIsVisual(false);
}
