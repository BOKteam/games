/*
 * GameScene.h
 *
 *  Created on: Nov 15, 2011
 *      Author: root
 */

#ifndef GAMESCENE_H_
#define GAMESCENE_H_
#include "GameCardManage.h"
#include "GameSceneUI.h"
using namespace NSEnveeCore;
class GameScene:public ECView
{
public:
	//static GameScene* Singleton();
	//static GameScene* m_pSingleton;

	GameScene();
	virtual ~GameScene();
	virtual void Init();
	GameSceneUI* GetGameSceneUI(){return m_pGameSceneUI;}
	GameCardManage* GetGameCardManage(){return m_pGameCardManage;}
	ECNode* GetGameFrontUI(){return m_pFrontUI;}
private:

	virtual void Update(evTime dt);
private:

	GameSceneUI* m_pGameSceneUI;
	GameCardManage* m_pGameCardManage;
	ECNode* m_pFrontUI;
};


#endif /* GAMESCENE_H_ */
