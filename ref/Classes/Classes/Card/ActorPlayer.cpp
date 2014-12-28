/*
 * ActorPlayer.cpp
 *
 *  Created on: Oct 8, 2012
 *      Author: root
 */

#include "ActorPlayer.h"
#include "CardImageManager.h"
#include "GameCardManage.h"
#include "Tool.h"
#include "ActorPlayerUI.h"
#include "Resourse.h"
ActorPlayer::ActorPlayer()
{
	m_pActorPlayerUI = new ActorPlayerUI(this);
}
ActorPlayer::~ActorPlayer()
{
	Debug::Log("Delete ActorPlayer");
	Clear();
	m_pActorPlayerUI->RemoveAllChild();
	delete m_pActorPlayerUI;
}

void ActorPlayer::OutCard()
{
	Hide();

	m_pActorPlayerUI->ShowOutCardBoard();
}

void ActorPlayer::ClickHint()
{
	for(unsigned int j=0;j<m_pCardImageList.size();j++)
	{
		m_pCardImageList[j]->bIsSelect = false;
	}

	m_pAI->SearchOutCard(m_nHandCardList, GetCardCount(m_nHandCardList),
			m_pCardManage->GetCurrentPowerCard()->nCurrentOutCard,
			m_pCardManage->GetCurrentPowerCard()->nCurrentCardCount,
			m_pCardManage->GetCurrentPowerCard()->nCurrentActor, m_nChairID,
			m_nOutCardList);

	for(int i=0;i<m_nOutCardList.cbCardCount;i++)
	{
		for(unsigned int j=0;j<m_pCardImageList.size();j++)
		{
			if(m_pCardImageList[j]->nCardID==m_nOutCardList.cbResultCard[i])
			{
				m_pCardImageList[j]->bIsSelect = true;
			}
		}
	}
}

bool ActorPlayer::CancelSelect()
{
	bool IsCancelSelect = false;
	for(unsigned int j=0;j<m_pCardImageList.size();j++)
	{
		if(m_pCardImageList[j]->bIsSelect)
		{
			m_pCardImageList[j]->bIsSelect = false;
			IsCancelSelect = true;
		}
	}
	return IsCancelSelect;
}

void ActorPlayer::ClickOutCard()
{
	memset(m_nOutCardList.cbResultCard, '\0',sizeof(m_nOutCardList.cbResultCard));
	m_nOutCardList.cbCardCount = 0;

	GetOutCard();

	if (CheckCouldOutCard()&&m_nOutCardList.cbCardCount > 0)
	{
		SoundManager::shareSoundManager()->playEffect(OUTCARD_EFFECT);

		switch(m_pAI->GetCardType(m_nOutCardList.cbResultCard,m_nOutCardList.cbCardCount))
		{
			case CT_MISSILE_CARD:m_pCardManage->SetBomb();break;
			case CT_BOMB_CARD:m_pCardManage->SetBomb();break;
			default:break;
		}
		memcpy(m_pCardManage->GetCurrentPowerCard()->nCurrentOutCard,
				m_nOutCardList.cbResultCard, m_nOutCardList.cbCardCount);
		m_pCardManage->GetCurrentPowerCard()->nCurrentActor = m_nChairID;
		m_pCardManage->GetCurrentPowerCard()->nCurrentCardCount = m_nOutCardList.cbCardCount;

		RemoveCardArray();

		m_pCardManage->SetNextActor();

		m_pActorPlayerUI->HideCardBoard();
	}
}

void ActorPlayer::ClickCancelOut()
{
//	if(CancelSelect())
//		return;
	CancelSelect();

	if(m_pCardManage->GetCurrentPowerCard()->nCurrentActor== -1)
		return;

	m_nOperationType = eOT_NotOutCard;

	m_pCardManage->SetNextActor();

	m_pActorPlayerUI->HideCardBoard();

}

void ActorPlayer::GetOutCard()
{
	for(int j=0;j<m_pCardImageList.size();j++)
	{
		if(m_pCardImageList[j]->bIsSelect == true)
		{
			m_nOutCardList.cbResultCard[m_nOutCardList.cbCardCount] = m_pCardImageList[j]->nCardID;
			m_nOutCardList.cbCardCount++;
		}
	}
}

bool ActorPlayer::CheckCouldOutCard()
{
	if(m_pCardManage->GetCurrentPowerCard()->nCurrentActor!=-1)
	{
		return m_pAI->CompareCard(m_pCardManage->GetCurrentPowerCard()->nCurrentOutCard,
				m_nOutCardList.cbResultCard,
				m_pCardManage->GetCurrentPowerCard()->nCurrentCardCount,
				m_nOutCardList.cbCardCount);
	}
	else
	{
		if(m_pAI->GetCardType(m_nOutCardList.cbResultCard,m_nOutCardList.cbCardCount)!=CT_ERROR)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void ActorPlayer::InitOneCard(std::vector<CardInfo*>& pList,BYTE nCardNum,int CurrentID)
{
	char path[64];
	sprintf(path,"Resources/GameUI/c%d.png",Tool::Singleton()->GetCardID(nCardNum)+1);

	CardInfo* Temp = new CardInfo();

	//ECView* TempView = CardImageManager::Singleton()->GetView(path);
	//printf("Resources/GameUI/c%d.png",Tool::Singleton()->GetCardID(nCardNum)+1);
	Temp->pFrontCard = ECButton::InitWithFile(path,path,callback_selector(CardInfo::SelectCard),Temp,128+CurrentID);
	Temp->bIsShowBack = false;
	Temp->nCardID = nCardNum;
	Temp->pBackCard = CardImageManager::Singleton()->GetView("Resources/GameUI/back.png");

	pList.push_back(Temp);

}
void ActorPlayer::ClearOut()
{
	Debug::Log("Clear Player Out Card");
	memset(m_nOutCardList.cbResultCard, '\0', 20);
	for(int i=0;i<m_pOutCardImageList.size();i++)
	{
		delete m_pOutCardImageList[i]->pFrontCard;
		delete m_pOutCardImageList[i];
	}
	m_pOutCardImageList.clear();
}
void ActorPlayer::RemoveCard(BYTE Card)
{
	for (int i = 0; i < GetCardCount(m_nHandCardList); i++)
	{
		if (m_nHandCardList[i] == Card)
		{
			for (int j = i; j < (GetCardCount(m_nHandCardList) - 1); j++)
			{
				m_nHandCardList[j] = m_nHandCardList[j + 1];
			}
			m_nHandCardList[GetCardCount(m_nHandCardList) - 1] = '\0';
			break;
		}
	}
	std::vector<CardInfo*>::iterator iter;
	for(iter=m_pCardImageList.begin();iter!=m_pCardImageList.end();iter++)
	{
		if(Card==(*iter)->nCardID)
		{
			delete (*iter)->pFrontCard;
			m_pCardImageList.erase(iter);
			return;
		}
	}
}
void ActorPlayer::ClearHand()
{
	Debug::Log("Clear Player Out Card");
	memset(m_nHandCardList,'\0',20);

	for(int j=0;j<m_pCardImageList.size();j++)
	{
		delete m_pCardImageList[j]->pFrontCard;
		delete m_pCardImageList[j];
	}
	m_pCardImageList.clear();
}
void ActorPlayer::Init()
{
	ActorBase::Init();
}
int ActorPlayer::GetBanker(int LastScore)
{
	m_pActorPlayerUI->ShowScoreBoard(LastScore);

	if(m_nBankerScore==-1)
	{
		m_nBankerScore=LastScore;

		m_nOperationType = eOT_NotCallBanker;

		m_bIsCallBanker = true;
		m_pActorPlayerUI->HideBankerBoard();
		return LastScore;
	}
	else if(m_nBankerScore!=0)
	{
		m_nOperationType = eOT_CallBanker;

		m_bIsCallBanker = true;
		m_pActorPlayerUI->HideBankerBoard();

		return m_nBankerScore;
	}
	else
	{
		return -1;
	}

	//return ActorBase::GetBanker(LastScore);
	//return 0;
}
void ActorPlayer::Draw()
{
	ActorBase::Draw();
	m_pActorPlayerUI->Update(0.05f);
}
