/*
 * ActorPlayer.h
 *
 *  Created on: Oct 8, 2012
 *      Author: root
 */

#ifndef ACTORPLAYER_H_
#define ACTORPLAYER_H_
#include "ActorBase.h"
class ActorPlayerUI;
class ActorPlayer : public ActorBase
{
	//virtual void InitCard();
	virtual void InitOneCard(std::vector<CardInfo*>& pList,BYTE nCardNum,int CurrentID);

	virtual void ClearOut();
	virtual void ClearHand();

	virtual void RemoveCard(BYTE Card);

	void GetOutCard();
	bool CheckCouldOutCard();

private:
	ActorPlayerUI* m_pActorPlayerUI;
public:
	ActorPlayer();
	virtual ~ActorPlayer();

	virtual void Init();

	void ClickHint();
	void ClickOutCard();
	void ClickCancelOut();
	bool CancelSelect();

	ActorPlayerUI* GetActorPlayerUI(){return m_pActorPlayerUI;}

	virtual int  GetBanker(int LastScore);
	virtual void OutCard();
	virtual void Draw();
};

#endif /* ACTORPLAYER_H_ */
