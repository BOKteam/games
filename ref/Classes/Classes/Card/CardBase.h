/*
 * CardBase.h
 *
 *  Created on: Oct 11, 2012
 *      Author: root
 */

#ifndef CARDBASE_H_
#define CARDBASE_H_
#include "EnveeCore.h"
#include "AndroidAI.h"
using namespace NSEnveeCore;

#define CARD_SPACE 15
enum ERowType
{
	eRT_Horizontal = 0,
	eRT_Vertical,
};
enum EActorOperationType
{
	eOT_None = 0,
	eOT_CallBanker,
	eOT_NotCallBanker,
	eOT_NotOutCard,
};
enum ETurnSequence
{
	eTS_Begin = 0,
	eTS_RandCard,
	eTS_Sort,
	eTS_CallBanker,
	eTS_SetBanker,
	eTS_OutBegin,
	eTS_OutCard,
	eTS_End,
};
enum EActorSequence
{
	eAS_First = 0,
	eAS_Second,
	eAS_Third,

	eAS_Count,
};
class GameCardManage;
class CardInfo : public ECNode
{
public:
	ECNode* pFrontCard;
	ECNode* pBackCard;
	bool bIsShowBack;
	bool bIsSelect;
	BYTE nCardID;

	CardInfo()
	{
		bIsSelect = false;
		bIsShowBack = true;
	}
	void SelectCard()
	{
		printf("Card Click--%d\n",nCardID);
		bIsSelect = !bIsSelect;
	}
};
class CardBase
{
public:
	BYTE m_nHandCardList[20];

	std::vector<CardInfo*> m_pCardImageList;
protected:

	ECPoint m_pRowTypeOffect;
	ECPoint m_pCardPos;

	GameCardManage* m_pCardManage;
	int m_nCardCount;
protected:
	virtual void RenderCard();
	virtual void ClearHand();

	virtual void InitOneCard(std::vector<CardInfo*>& pList,BYTE nCardNum,int CurrentID);
public:
	std::vector<CardInfo*>& GetCardList(){return m_pCardImageList;}
	ECPoint GetCardPos(){return m_pCardPos;}
	~CardBase();

	GameCardManage* GetCardManage(){return m_pCardManage;}

	int GetCardCount(BYTE Card[]);

	virtual void GameBegin();

	void InitRowType(ERowType Type)
	{
		if(Type==eRT_Horizontal)
		{
			m_pRowTypeOffect = ECPoint(2,0);
		}
		else
		{
			m_pRowTypeOffect = ECPoint(0,1);
		}
	}
	void InitCardPos(ECPoint Pos){m_pCardPos = Pos;}

	virtual void Init();
	virtual void InitCard();
	virtual void Draw();
	virtual void Clear();

	void ShowCard(bool IsShow);

	void SetCardManager(GameCardManage* pManage){m_pCardManage = pManage;}
};

#endif /* CARDBASE_H_ */
