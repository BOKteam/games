/*
 * ActorBase.h
 *
 *  Created on: Oct 8, 2012
 *      Author: root
 */

#ifndef ACTORBASE_H_
#define ACTORBASE_H_


#include "CardBase.h"

class ActorBase : public CardBase
{
public:
	bool m_bIsCallBanker;
	CAndroidAI* m_pAI;
protected:
	int m_nChairID;
	int m_nBankerScore;
	int m_nBankerID;
	EActorOperationType m_nOperationType;


	tagOutCardResult m_nOutCardList;

	std::vector<CardInfo*> m_pOutCardImageList;

	ECPoint m_pOutCardPos;
	ECPoint m_pBankerPos;
	ECPoint m_pCancelImagePos;


	ECView* m_pScoreImage[3];
	ECView* m_pCancelImage;
	ECView* m_pBankerImage;
	ECView* m_pFarmerImage;
	ECView* m_pCancelBankerImage;
protected:


	virtual void InitCard();
	virtual void InitBack();
	virtual void RenderCard();
	//virtual void InitOneCard(BYTE nCardNum,int CurrentID) = 0;
	//virtual void InsertBankerCard();

	void Show();

	void RenderOutCard();
	void RemoveCardArray();
	virtual void RemoveCard(BYTE Card);

	virtual void ClearOut();
public:
	void Begin();
	void End();
	virtual void GameBegin();
	virtual void Clear();

	void InitOutCardPos(ECPoint Pos){m_pOutCardPos = Pos;}

	void InitBankerPos(ECPoint Pos){m_pBankerPos = Pos;}
	void InitCancelImagePos(ECPoint Pos){m_pCancelImagePos = Pos;}


	void SetBankerScore(int Score){m_nBankerScore = Score;}
public:
	ActorBase();
	~ActorBase();

	void SetTurn();

	void SetChair(int ChairID){m_nChairID = ChairID;};
	int  GetChairID(){return m_nChairID;}

	void Hide();

	virtual void SetBanker(ActorBase* Banker);
	virtual int  GetBanker(int LastScore);

	virtual void OutCard();

	virtual void Init();
	void InitAI();
	virtual void Draw();
};


#endif /* ACTORBASE_H_ */
