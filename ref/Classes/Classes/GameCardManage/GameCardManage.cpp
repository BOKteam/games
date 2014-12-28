/*
 * GameCardManage.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: root
 */
#include "GameCardManage.h"
//#include "time.h"
#include "GameScene.h"
#include "ActorPlayer.h"
#include "ActorEnemy.h"
#include "CardImageManager.h"
#include "GameActorScoreManage.h"
#include "Resourse.h"
//BYTE GameCardManage::
GameCardManage::GameCardManage(GameScene* pGameScene)//1
{
	m_pGameScene = pGameScene;

	printf("InitGameCardManage");
	m_bStopProcess = false;

	m_sCurrentPowerCard = new PowerCard();

	Actor[0] = new ActorPlayer();
	Actor[1] = new ActorEnemy();
	Actor[2] = new ActorEnemy();

	Actor[0]->SetCardManager(this);
	Actor[1]->SetCardManager(this);
	Actor[2]->SetCardManager(this);

	BankerCard = new CardBase();

	m_sPlayerPos = ECPoint(110,490);
	m_sEnemyAPos = ECPoint(598,86);
	m_sEnemyBPos = ECPoint(110,86);
	m_sPlayerOutCardsPos = ECPoint(185,386);
	m_sEnemyAOutCardsPos = ECPoint(489,96);
	m_sEnemyBOutCardsPos = ECPoint(209,96);
	m_sPlayerLordImagePos = ECPoint(25,490);
	m_sEnemyALordImagePos = ECPoint(596,10);
	m_sEnemyBLordImagePos = ECPoint(105,10);
	CancelPlayerImagePos = ECPoint(330,400);
	CancelEnemyAImagePos = ECPoint(485,240);
	CancelEnemyBImagePos = ECPoint(224,240);

	Actor[0]->InitCardPos(m_sPlayerPos);
	Actor[0]->InitOutCardPos(m_sPlayerOutCardsPos);
	Actor[0]->InitRowType(eRT_Horizontal);
	Actor[0]->InitBankerPos(m_sPlayerLordImagePos);
	Actor[0]->InitCancelImagePos(CancelPlayerImagePos);
	Actor[0]->SetChair(0);

	Actor[1]->InitCardPos(m_sEnemyAPos);
	Actor[1]->InitOutCardPos(m_sEnemyAOutCardsPos);
	Actor[1]->InitRowType(eRT_Vertical);
	Actor[1]->InitBankerPos(m_sEnemyALordImagePos);
	Actor[1]->InitCancelImagePos(CancelEnemyAImagePos);
	Actor[1]->SetChair(1);

	Actor[2]->InitCardPos(m_sEnemyBPos);
	Actor[2]->InitOutCardPos(m_sEnemyBOutCardsPos);
	Actor[2]->InitRowType(eRT_Vertical);
	Actor[2]->InitBankerPos(m_sEnemyBLordImagePos);
	Actor[2]->InitCancelImagePos(CancelEnemyBImagePos);
	Actor[2]->SetChair(2);

	BankerCard->InitCardPos(ECPoint(270,30));
	BankerCard->InitRowType(eRT_Horizontal);
	//m_pActorPlayer->SetOriginPos(m_sPlayerPos,m_sPlayerOutCardsPos,m_sPlayerLordImagePos,CancelPlayerImagePos);
	//m_pActorEnemyA->SetOriginPos(m_sEnemyAPos,m_sEnemyAOutCardsPos,m_sEnemyALordImagePos,CancelEnemyAImagePos);
	//m_pActorEnemyB->SetOriginPos(m_sEnemyBPos,m_sEnemyBOutCardsPos,m_sEnemyBLordImagePos,CancelEnemyBImagePos);

	BYTE cbCardData[FULL_COUNT]=
	{
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
		0x4E,0x4F,
	};

	memcpy(m_cbCardData,cbCardData,sizeof(cbCardData));

	m_nCurrentBankerID = -1;
	m_nCurrentTime = 0.0f;
	m_nOperationTime = 0.0f;
	m_nCurrentBankerScore = 0;
	m_eCurrentTurn = eTS_Begin;
}
GameCardManage::~GameCardManage()
{
	ECEventDispatcher::Singleton()->SetIsResponseKeyboard(true);
	Debug::Log("Delete Card Manager");
	delete m_sCurrentPowerCard;
	delete ((ActorPlayer*)Actor[0]);
	delete ((ActorEnemy*)Actor[1]);
	delete ((ActorEnemy*)Actor[2]);
	delete BankerCard;
}
void GameCardManage::NewGame()
{
	//ECEventDispatcher::Singleton()->SetIsResponseKeyboard(false);

	m_eCurrentTurn = eTS_Begin;
}
void GameCardManage::CardDispatcher()
{
	for(int i=0;i<FULL_COUNT;i++)
	{
		int Temp = rand()%FULL_COUNT;

		BYTE TempCardNum = m_cbCardData[i];
		m_cbCardData[i] = m_cbCardData[Temp];
		m_cbCardData[Temp] = TempCardNum;
	}
	SetActorCard(Actor[0],0,16);
	SetActorCard(Actor[1],17,33);
	SetActorCard(Actor[2],34,50);
	SetActorCard(BankerCard,51,53);

	Actor[0]->InitAI();
	Actor[1]->InitAI();
	Actor[2]->InitAI();
	Actor[0]->Init();
	Actor[1]->Init();
	Actor[2]->Init();
	//printf("InitSuccess!!!!!!!!!!!!!!!!!!!1\n");
	BankerCard->Init();
	BankerCard->ShowCard(true);
	m_eCurrentTurn = eTS_Sort;

	m_nCurrentBankerID = -1;
	m_nCurrentTime = 0.0f;
	m_nOperationTime = 0.0f;
	m_nCurrentBankerScore = 0;

	SoundManager::shareSoundManager()->playEffect(NEWROUND);
}
void GameCardManage::SetActorCard(CardBase* Actor,int Form,int To)
{
	for(int i= Form;i<=To;i++)
	{
		//printf("%d      ",m_cbCardData[i]);
		Actor->m_nHandCardList[i-Form] = m_cbCardData[i];
		//printf("Init Enemy%d\n",Actor->m_nHandCardList[0]);
	}
	printf("\n");
}
void GameCardManage::ChooseLord()
{
	m_eCurrentActor = (EActorSequence)(rand()%eAS_Count);

	m_eCurrentTurn = eTS_CallBanker;
}
void GameCardManage::GameBegin()
{
	printf("Game Begin!!!!!!!!!!!!!!!!\n");

	m_eCurrentTurn = eTS_RandCard;

	Actor[0]->GameBegin();
	Actor[1]->GameBegin();
	Actor[2]->GameBegin();
	BankerCard->GameBegin();

	m_nCurrentBankerID = -1;
	m_nCurrentTime = 0.0f;
	m_nOperationTime = 0.0f;
	m_nCurrentBankerScore = 0;

	m_pGameScene->GetGameSceneUI()->HideResult();
	CardImageManager::Singleton()->GetView("Resources/GameUI/Lord_L.png")->SetIsVisual(false);
	CardImageManager::Singleton()->GetView("Resources/GameUI/Farmer_L.png")->SetIsVisual(false);
}
void GameCardManage::RenderCards()
{

}
void GameCardManage::GameOver(bool IsPlayerWin)
{

}
void GameCardManage::ClearLordCardArray()
{

}
void GameCardManage::ClearAllCardsOnBoard()
{

}
void GameCardManage::ShowLordCard()
{
	BankerCard->ShowCard(true);
}
void GameCardManage::HideLordCard()
{
	BankerCard->ShowCard(false);
}
void GameCardManage::RenderLordCards()
{

}
//float AllTime = 0.0f;
void GameCardManage::Update(float dt)
{
	Actor[1]->Draw();
	Actor[2]->Draw();
	BankerCard->Draw();
	Actor[0]->Draw();

	if(m_bStopProcess)
		return;
	//printf("Run Update\n");
	m_nCurrentTime+= dt;
	//AllTime+= dt;
	if(m_nCurrentTime>m_nOperationTime)
	{
		Process();
		//printf("Run Process\n");
	}
}
void GameCardManage::SetCurrentTurn(ETurnSequence Type)
{
	m_eCurrentTurn = Type;
}
void GameCardManage::SetNextActor()
{
	m_eCurrentActor = (EActorSequence)(((int)m_eCurrentActor+1)%(int)eAS_Count);
}
void GameCardManage::CallBanker(ActorBase* pActor)
{
	if(Actor[(int)m_eCurrentActor]->m_bIsCallBanker)
	{
		if(m_nCurrentBankerID!=-1)
		{
			//
			m_eCurrentTurn = eTS_SetBanker;
		}
		else
		{
			m_eCurrentTurn = eTS_Begin;
		}
		return;
	}


	int Temp = pActor->GetBanker(m_nCurrentBankerScore);
	//printf("Current Score %d \n",Temp);

	if(Temp==-1)
		return;

	SetTimer(2.0f);

	if(Temp>m_nCurrentBankerScore)
	{
		m_nCurrentBankerID = pActor->GetChairID();
	}
	m_nCurrentBankerScore = Temp;

	if(m_nCurrentBankerScore==3)
	{
		m_eCurrentTurn = eTS_SetBanker;
	}

	SetNextActor();
}
void GameCardManage::SetBanker()
{
	SetTimer(0.5f);
	ShowLordCard();
	BYTE Temp[20];
	memcpy(Temp,Actor[m_nCurrentBankerID]->m_nHandCardList,20);
	for(int i=0;i<BankerCard->GetCardCount(BankerCard->m_nHandCardList);i++)
	{
		Temp[17+i] = BankerCard->m_nHandCardList[i];
	}
	Actor[m_nCurrentBankerID]->Clear();
	memcpy(Actor[m_nCurrentBankerID]->m_nHandCardList,Temp,20);
	Actor[m_nCurrentBankerID]->Init();
	Actor[m_nCurrentBankerID]->SetBankerScore(m_nCurrentBankerScore);

	printf("\n CurrentActorCardLength----%d  \n",Actor[m_nCurrentBankerID]->m_pCardImageList.size());

	Actor[m_nCurrentBankerID]->SetBanker(Actor[m_nCurrentBankerID]);
	Actor[(m_nCurrentBankerID+1)%(int)eAS_Count]->SetBanker(Actor[m_nCurrentBankerID]);
	Actor[(m_nCurrentBankerID+2)%(int)eAS_Count]->SetBanker(Actor[m_nCurrentBankerID]);
	m_eCurrentTurn = eTS_OutBegin;
}
void GameCardManage::ActorOutCard(ActorBase* pActor)
{
	SetTimer(1.4f);

	if(pActor->GetChairID()==GetCurrentPowerCard()->nCurrentActor)
	{
		InitPowerCardActor(pActor->GetChairID());

		SetTimer(1.0f);
		return;
	}

	pActor->OutCard();

}
void GameCardManage::InitPowerCardActor(int ChairID)
{
	m_eCurrentActor = (EActorSequence)ChairID;
	m_sCurrentPowerCard->nCurrentActor = -1;
	memset(m_sCurrentPowerCard->nCurrentOutCard,'\0',sizeof(m_sCurrentPowerCard->nCurrentOutCard));
	m_sCurrentPowerCard->nCurrentCardCount = 0;

	Actor[0]->Hide();
	Actor[1]->Hide();
	Actor[2]->Hide();
}
void GameCardManage::SetGameOver(int nWinChairID)
{
	SetTimer(3.0f);

	Actor[0]->ShowCard(true);
	Actor[1]->ShowCard(true);
	Actor[2]->ShowCard(true);

	m_eCurrentTurn = eTS_End;
	if(GameActorScoreManage::Singleton()->SetActorWin(nWinChairID,m_nCurrentBankerID))
	{
		m_pGameScene->GetGameSceneUI()->Win();
		SoundManager::shareSoundManager()->playEffect(WIN_EFFECT);
	}
	else
	{
		m_pGameScene->GetGameSceneUI()->Lose();
		SoundManager::shareSoundManager()->playEffect(LOSE_EFFECT);
	}
}
void GameCardManage::Process()
{
	switch(m_eCurrentTurn)
	{
	case eTS_Begin:
		GameBegin();
		break;
	case eTS_RandCard:
		CardDispatcher();
		HideLordCard();
		break;
	case eTS_Sort:
		ChooseLord();
		break;
	case eTS_CallBanker:

		switch(m_eCurrentActor)
		{
		case eAS_First:
			CallBanker(Actor[0]);
			break;//Player->Enemy
		case eAS_Second:
			CallBanker(Actor[1]);
			break;//Enemy->Enemy
		case eAS_Third:
			CallBanker(Actor[2]);
			break;//Enemy->Player
		default:break;
		}

		break;
	case eTS_SetBanker:
		SetBanker();
		break;
	case eTS_OutBegin:

		Actor[0]->Begin();
		Actor[1]->Begin();
		Actor[2]->Begin();

		m_eCurrentTurn = eTS_OutCard;
		m_eCurrentActor = (EActorSequence)m_nCurrentBankerID;
		GameActorScoreManage::Singleton()->SetCurrentScore(m_nCurrentBankerScore);
		memset(m_sCurrentPowerCard->nCurrentOutCard,'\0',sizeof(m_sCurrentPowerCard->nCurrentOutCard));
		m_sCurrentPowerCard->nCurrentCardCount = 0;
		m_sCurrentPowerCard->nCurrentActor = -1;

		break;
	case eTS_OutCard:

		switch(m_eCurrentActor)
		{
		case eAS_First:ActorOutCard(Actor[0]);break;//Player->Enemy
		case eAS_Second:ActorOutCard(Actor[1]);break;//Enemy->Enemy
		case eAS_Third:ActorOutCard(Actor[2]);break;//Enemy->Player
		default:break;
		}
		break;
	case eTS_End:
		SetTimer(2.0f);
		Actor[0]->End();
		Actor[1]->End();
		Actor[2]->End();

		m_eCurrentTurn = eTS_Begin;
		break;
	default:break;
	}
}

void GameCardManage::SetTimer(float SpaceTime)
{
	m_nOperationTime = SpaceTime;
	m_nCurrentTime = 0.0f;
}
void GameCardManage::SetBomb()
{
	m_pGameScene->GetGameSceneUI()->PlayBombAnimtion(0.07f);
	GameActorScoreManage::Singleton()->SetBomb();
	SoundManager::shareSoundManager()->playEffect(BOMB_EFFECT);
}
