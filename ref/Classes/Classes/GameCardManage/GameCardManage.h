/*
 * GameCardManage.h
 *
 *  Created on: Nov 15, 2011
 *      Author: root
 */

#ifndef GAMECARDMANAGE_H_
#define GAMECARDMANAGE_H_
#include "EnveeCore.h"
//#include "GameScoreManage.h"
//class ActorBase;
#include "ActorPlayer.h"
#include "ActorEnemy.h"
class PowerCard
{
public:
	BYTE nCurrentOutCard[20];
	int  nCurrentActor;
	int  nCurrentCardCount;
};
class GameActorScoreManage;
class GameScene;
class GameCardManage
{
	//std::vector<GameCardBase*> m_pLordCards;
	void ClearLordCardArray();

	void RenderLordCards();
	void ShowLordCard();
	void HideLordCard();

	void SetFirstActor(int ActorID){m_eCurrentActor = (EActorSequence)ActorID;}

	void SetActorCard(CardBase* Actor,int Form,int To);

	void CallBanker(ActorBase* pActor);

	void SetBanker();

	void ActorOutCard(ActorBase* pActor);
private:
	void SetTimer(float SpaceTime);
	void Process();

	int m_nCurrentBankerScore;
	int m_nCurrentBankerID;

	float m_nOperationTime;
	float m_nCurrentTime;

	ETurnSequence m_eCurrentTurn;
	EActorSequence m_eCurrentActor;

	//int m_nWhoCallLord;

	//GameActorScoreManage* m_pScoreManage;

	BYTE m_cbCardData[FULL_COUNT];

	//SCardType m_sCardArray[54];

	//EActorType m_eCurrentActor;

	ECPoint m_sPlayerPos;
	ECPoint m_sEnemyAPos;
	ECPoint m_sEnemyBPos;
	ECPoint m_sPlayerOutCardsPos;
	ECPoint m_sEnemyAOutCardsPos;
	ECPoint m_sEnemyBOutCardsPos;
	ECPoint m_sPlayerLordImagePos;
	ECPoint m_sEnemyALordImagePos;
	ECPoint m_sEnemyBLordImagePos;
	ECPoint CancelPlayerImagePos;
	ECPoint CancelEnemyAImagePos;
	ECPoint CancelEnemyBImagePos;

	//GameTurnScoreManage* m_pTurnScore;

	bool m_bStopProcess;
public:
	ActorBase* Actor[3];
	CardBase* BankerCard;
	PowerCard* m_sCurrentPowerCard;
	GameScene* m_pGameScene;
public:

	GameCardManage(GameScene* pGameScene);
	~GameCardManage();

	void StopProcess(bool bIsStop){m_bStopProcess = bIsStop;}

	PowerCard* GetCurrentPowerCard(){return m_sCurrentPowerCard;}
	GameScene* GetGameScene(){return m_pGameScene;}

	void Update(float dt);

	//GameTurnScoreManage* GetTurnScore(){return m_pTurnScore;}

	void RenderCards();

	void NewGame();

	void SetGameOver(int nWinChairID);

	void SetCurrentTurn(ETurnSequence Type);
	void InitPowerCardActor(int ChairID);
	void SetNextActor();

	void CardDispatcher();
	void ChooseLord();
	void GameBegin();
	void MainLogic();
	void ClearAllCardsOnBoard();
	void GameOver(bool IsPlayerWin);
	void ResetGame();

	void SetBomb();

};
#endif /* GAMECARDMANAGE_H_ */
