/*
 * GameScene.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: root
 */
#include "GameScene.h"
GameScene::GameScene()
{
	//Init();
	m_pFrontUI = new ECNode();

	m_pGameCardManage = new GameCardManage(this);


	m_pGameSceneUI = new GameSceneUI(this);
}
GameScene::~GameScene()
{
	delete m_pGameCardManage;

	m_pGameSceneUI->RemoveAllChild();
	delete m_pGameSceneUI;

	delete m_pFrontUI;
}
void GameScene::Init()
{
	//m_pGameCardManage->NewGame();
	//m_pGameCardManage->CardDispatcher();
}
void GameScene::Update(evTime dt)
{
	m_pGameSceneUI ->Update(dt);
	m_pGameCardManage->Update(dt);
	m_pFrontUI->Update(dt);

}


