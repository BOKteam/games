/*
 * ActorBase.cpp
 *
 *  Created on: Oct 8, 2012
 *      Author: root
 */

#include "ActorBase.h"
#include "CardImageManager.h"
#include "GameCardManage.h"
#include "Resourse.h"
ActorBase::ActorBase()
{

	m_pAI = NULL;

	m_nBankerID = -1;
	m_bIsCallBanker = false;

	InitBack();

	//Clear();
}
void ActorBase::ClearOut()
{
	if(m_nOutCardList.cbCardCount == 0)
		return;

	memset(m_nOutCardList.cbResultCard, '\0',
			sizeof(m_nOutCardList.cbResultCard));
	m_nOutCardList.cbCardCount = 0;
	for (int i = 0; i < m_pOutCardImageList.size(); i++)
	{
		delete m_pOutCardImageList[i];
	}
	m_pOutCardImageList.clear();
}
void ActorBase::Clear()
{
	printf("Clear ALL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	ClearOut();
	ClearHand();

	m_bIsCallBanker = false;
	m_nBankerID = -1;
	m_nBankerScore = 0;
	//m_nOperationType = eOT_None;
}
ActorBase::~ActorBase()
{
	if (m_pAI != NULL)
	{
		delete m_pAI;
	}
}
void ActorBase::SetTurn()
{
}
void ActorBase::Init()
{
	InitCard();
}
void ActorBase::InitAI()
{
	if (m_pAI != NULL)
	{
		delete m_pAI;
		m_pAI = NULL;
	}

	m_pAI = new CAndroidAI();

	m_pAI->SetUserCard(m_pCardManage->Actor[0]->GetChairID(),
			m_pCardManage->Actor[0]->m_nHandCardList,
			GetCardCount(m_pCardManage->Actor[0]->m_nHandCardList));
	m_pAI->SetUserCard(m_pCardManage->Actor[1]->GetChairID(),
			m_pCardManage->Actor[1]->m_nHandCardList,
			GetCardCount(m_pCardManage->Actor[1]->m_nHandCardList));
	m_pAI->SetUserCard(m_pCardManage->Actor[2]->GetChairID(),
			m_pCardManage->Actor[2]->m_nHandCardList,
			GetCardCount(m_pCardManage->Actor[2]->m_nHandCardList));

}
void ActorBase::InitCard()
{

	m_pAI->SortCardList(m_nHandCardList, GetCardCount(m_nHandCardList),
			ST_ORDER);

	CardBase::InitCard();

}
void ActorBase::InitBack()
{
	m_pCancelImage = CardImageManager::Singleton()->GetView(
			"Resources/GameUI/Pass_Back.png");
	m_pBankerImage = CardImageManager::Singleton()->GetView(
			"Resources/GameUI/Lord_L.png");

	m_pCancelBankerImage = CardImageManager::Singleton()->GetView(
			"Resources/GameUI/PassLord_Back.png");
	m_pFarmerImage = CardImageManager::Singleton()->GetView(
			"Resources/GameUI/Farmer_L.png");

	m_pScoreImage[0] = CardImageManager::Singleton()->GetView(
			"Resources/GameUI/Game_BtnDown_CallScore1.png");
	m_pScoreImage[1] = CardImageManager::Singleton()->GetView(
			"Resources/GameUI/Game_BtnDown_CallScore2.png");
	m_pScoreImage[2] = CardImageManager::Singleton()->GetView(
			"Resources/GameUI/Game_BtnDown_CallScore3.png");
}
//void ActorBase::InitRowType(ERowType Type)
void ActorBase::RenderCard()
{
	//printf("Render Card Size %d\n",m_pCardImageList.size());
	for (int i = 0; i < m_pCardImageList.size(); i++)
	{
		if (m_pCardImageList[i]->bIsShowBack)
		{
			m_pCardImageList[i]->pBackCard->SetPosition(
					(m_pRowTypeOffect * CARD_SPACE * i) + m_pCardPos);
			m_pCardImageList[i]->pBackCard->Draw();
		}
		else
		{
			if (m_pCardImageList[i]->bIsSelect)
				m_pCardImageList[i]->pFrontCard->SetPosition(
						(m_pRowTypeOffect * CARD_SPACE * i) + m_pCardPos
								+ ECPoint(0, -20));
			else
			{
				ECNode* Temp = m_pCardImageList[i]->pFrontCard;
				Temp->SetPosition(ECPoint(0, 0));
				m_pCardImageList[i]->pFrontCard->SetPosition(
						(m_pRowTypeOffect * CARD_SPACE * i) + m_pCardPos);
			}

			m_pCardImageList[i]->pFrontCard->Draw();
		}
	}
}
void ActorBase::OutCard()
{
	Hide();
	//printf();
	m_pAI->SearchOutCard(m_nHandCardList, GetCardCount(m_nHandCardList),
			m_pCardManage->GetCurrentPowerCard()->nCurrentOutCard,
			m_pCardManage->GetCurrentPowerCard()->nCurrentCardCount,
			m_pCardManage->GetCurrentPowerCard()->nCurrentActor, m_nChairID,
			m_nOutCardList);

	if (m_nOutCardList.cbCardCount > 0)
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
	}
	else
	{
		m_nOperationType = eOT_NotOutCard;
	}

	m_pCardManage->SetNextActor();
}
void ActorBase::RemoveCardArray()
{
	m_pCardManage->Actor[0]->m_pAI->RemoveUserCardData(m_nChairID,m_nOutCardList.cbResultCard,m_nOutCardList.cbCardCount);
	m_pCardManage->Actor[1]->m_pAI->RemoveUserCardData(m_nChairID,m_nOutCardList.cbResultCard,m_nOutCardList.cbCardCount);
	m_pCardManage->Actor[2]->m_pAI->RemoveUserCardData(m_nChairID,m_nOutCardList.cbResultCard,m_nOutCardList.cbCardCount);

	for (int i = 0; i < m_nOutCardList.cbCardCount; i++)
	{
		if (m_nOutCardList.cbResultCard[i] == '\0')
			return;

		RemoveCard(m_nOutCardList.cbResultCard[i]);
		//printf("Current HandCardArray Size %d\n",GetCardCount(m_nHandCardList));
		//printf("Current HandCardImage Size %d\n",m_pCardImageList.size());
		InitOneCard(m_pOutCardImageList,m_nOutCardList.cbResultCard[i],i);
		//printf("Current OutCard Size %d\n",m_pOutCardImageList.size());
	}

	if(m_pCardImageList.size()==0||GetCardCount(m_nHandCardList)==0)
	{
		m_pCardManage->SetGameOver(m_nChairID);
	}
}
void ActorBase::RemoveCard(BYTE Card)
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
			delete (*iter);
			m_pCardImageList.erase(iter);
			return;
		}
	}
}
void ActorBase::Show()
{

}
void ActorBase::GameBegin()
{
	Clear();

	m_nOperationType = eOT_None;
}
void ActorBase::Begin()
{
	Hide();

	m_pFarmerImage->SetIsVisual(true);
	m_pBankerImage->SetIsVisual(true);
}
void ActorBase::End()
{
	Hide();

	m_pFarmerImage->SetIsVisual(false);
	m_pBankerImage->SetIsVisual(false);
}
void ActorBase::Hide()
{
	m_nOperationType = eOT_None;
	ClearOut();
}
int ActorBase::GetBanker(int LastScore)
{
	m_bIsCallBanker = true;

	BYTE Temp[20];
	memset(Temp, '\0', 20);
	memcpy(Temp, m_nHandCardList, 20);
	for (int i = 0;
			i < GetCardCount(m_pCardManage->BankerCard->m_nHandCardList); i++)
			{
		Temp[GetCardCount(m_nHandCardList) + i] =
				m_pCardManage->BankerCard->m_nHandCardList[i];
	}
	m_pAI->SetLandScoreCardData(Temp, 20);

	m_nBankerScore = m_pAI->AnalyseLandScore(m_nChairID, LastScore);

	if (m_nBankerScore == 255)
		m_nBankerScore = LastScore;

	if (m_nBankerScore > LastScore)
	{
		m_nOperationType = eOT_CallBanker;
	}
	else
	{
		m_nOperationType = eOT_NotCallBanker;
	}

	return m_nBankerScore;
}
void ActorBase::SetBanker(ActorBase* Banker)
{
	//m_nOperationType = eOT_None;

	m_nBankerID = Banker->GetChairID();

	m_pAI->SetBanker(Banker->GetChairID());

	m_pAI->SetBackCard(
			Banker->GetChairID(),
			m_pCardManage->BankerCard->m_nHandCardList,
			m_pCardManage->BankerCard->GetCardCount(
					m_pCardManage->BankerCard->m_nHandCardList));
}
void ActorBase::RenderOutCard()
{
	for (int i = 0; i < m_pOutCardImageList.size(); i++)
	{
		m_pOutCardImageList[i]->pFrontCard->SetPosition(
				(m_pRowTypeOffect * CARD_SPACE * i) + m_pOutCardPos);
		m_pOutCardImageList[i]->pFrontCard->Draw();
	}
}
void ActorBase::Draw()
{
	CardBase::Draw();
	RenderOutCard();

	switch (m_nOperationType)
	{
	case eOT_CallBanker:

		if (m_nBankerScore != 0)
		{
			m_pScoreImage[m_nBankerScore - 1]->SetPosition(m_pCancelImagePos);
			m_pScoreImage[m_nBankerScore - 1]->Draw();
		}

		break;
	case eOT_NotCallBanker:
		m_pCancelBankerImage->SetPosition(m_pCancelImagePos);
		m_pCancelBankerImage->Draw();
		break;
	case eOT_NotOutCard:
		m_pCancelImage->SetPosition(m_pCancelImagePos);
		m_pCancelImage->Draw();
		break;
	default:
		break;
	}

	if (m_nBankerID == m_nChairID)
	{
		m_pBankerImage->SetPosition(m_pBankerPos);
		m_pBankerImage->Draw();
	}
	else
	{
		m_pFarmerImage->SetPosition(m_pBankerPos);
		m_pFarmerImage->Draw();
	}
}
