/*
 * GameSceneUI.h
 *
 *  Created on: Nov 16, 2011
 *      Author: root
 */

#ifndef GAMESCENEUI_H_
#define GAMESCENEUI_H_
//#include "GameActor.h"
#include "EnveeCore.h"
#include "CardImageManager.h"
using namespace NSEnveeCore;
class GameScene;
class GameSceneUI:public ECNode
{
private:
	virtual void Init();
	void InitScoreBoard();
	void InitSceneBackground();
	void InitResultBoard();
	void UpdateScoreBoard(float dt);

	void UpdateAnimation(evTime dt);

	void SetButtonIsEnable(bool IsEnable);
	void WaitRestart();
private:
	//GameActor* m_pActorPlayer;
	ECLable* m_pCurrentScore;
	ECLable* m_pPlayerScore;
	ECLable* m_pComputerScore_A;
	ECLable* m_pComputerScore_B;
	ECView* m_pWinView;
	ECView* m_pLoseView;

	int m_nCurrentFrameID;
	float m_nFrameTime;
	float m_nCountTime;
	bool m_bIsPlay;
	ECView* m_pBombList[BOMB_EFFECT_COUNT];

	GameScene* m_pGameScene;
public:
	GameSceneUI(GameScene* pGameScene);

	void PlayBombAnimtion(float nFrameTime);
	virtual void Update(evTime dt);

	void Win();
	void Lose();
	void HideResult();
	bool GetIsShowResult();
};


#endif /* GAMESCENEUI_H_ */
