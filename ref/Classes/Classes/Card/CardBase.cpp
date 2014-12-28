/*
 * CardBase.cpp
 *
 *  Created on: Oct 11, 2012
 *      Author: root
 */
#include "CardBase.h"
#include "Tool.h"
#include "CardImageManager.h"
void CardBase::RenderCard()
{
	for(int i=0;i<m_pCardImageList.size();i++)
	{
		if(m_pCardImageList[i]->bIsShowBack)
		{
			m_pCardImageList[i]->pBackCard->SetPosition((m_pRowTypeOffect*36*i)+m_pCardPos);
			m_pCardImageList[i]->pBackCard->Draw();
		}
		else
		{
			m_pCardImageList[i]->pFrontCard->SetPosition((m_pRowTypeOffect*36*i)+m_pCardPos);
			m_pCardImageList[i]->pFrontCard->Draw();
		}
	}
}
CardBase::~CardBase()
{
	Clear();
}
void CardBase::Clear()
{
	ClearHand();
}
void CardBase::ClearHand()
{
	memset(m_nHandCardList,'\0',sizeof(m_nHandCardList));

	for(int j=0;j<m_pCardImageList.size();j++)
	{
		delete m_pCardImageList[j];
	}
	m_pCardImageList.clear();

}
void CardBase::Init()
{
	InitCard();
}
void CardBase::InitCard()
{
	for(int i=0;i<20;i++)
	{
		if(m_nHandCardList[i]=='\0')
			return;

		InitOneCard(m_pCardImageList,m_nHandCardList[i],i);

	}
	//printf("\n");
}
int CardBase::GetCardCount(BYTE Card[])
{
	//BYTE Temp[sizeof(Card)];
	//memcpy(Temp,Card,sizeof(Card));
	for(m_nCardCount = 0;m_nCardCount<20;m_nCardCount++)
	{
		if(Card[m_nCardCount]=='\0')
		{
			break;
		}
	}
	return m_nCardCount;
}
void CardBase::ShowCard(bool IsShow)
{
	for(int i=0;i<m_pCardImageList.size();i++)
	{
		m_pCardImageList[i]->bIsShowBack = !IsShow;
	}
}
void CardBase::InitOneCard(std::vector<CardInfo*>& pList,BYTE nCardNum,int CurrentID)
{
		char path[64];
		sprintf(path,"Resources/GameUI/c%d.png",Tool::Singleton()->GetCardID(nCardNum)+1);
		//printf("%d  ",Tool::Singleton()->GetCardID(nCardNum)+1);
		CardInfo* Temp = new CardInfo();

		Temp->pFrontCard = CardImageManager::Singleton()->GetView(path);
		Temp->bIsShowBack = true;
		Temp->nCardID = nCardNum;
		Temp->pBackCard = CardImageManager::Singleton()->GetView("Resources/GameUI/back.png");

		pList.push_back(Temp);

}
void CardBase::Draw()
{
	RenderCard();
}
void CardBase::GameBegin()
{
	Clear();
}

