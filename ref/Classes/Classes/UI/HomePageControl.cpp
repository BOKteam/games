/*
 * HomePageControl.cpp
 *
 *  Created on: Nov 13, 2011
 *      Author: root
 */
#include "HomePageControl.h"
#include "HomePage.h"
#include "GameScene.h"
#include "HomePageCursor.h"
#include "HelpInfo.h"
#include "Resourse.h"

HomePageControl* HomePageControl::m_Interface = NULL;
HomePageControl* HomePageControl::GetInterface()
{
	if(m_Interface == NULL)
	{
		m_Interface = new HomePageControl();
	}
	return m_Interface;
}
HomePageControl::HomePageControl()
{

	Init();
}

HomePageControl::~HomePageControl()
{
	delete m_Interface;
	m_Interface=NULL;
}

void HomePageControl :: Init()
{
	m_pHomePageCursor = new HomePageCursor();
//	m_pHelpInfo = new HelpInfo();
}
void HomePageControl::InitCursorData()
{
	m_pHomePageCursor->InitState();
}
void HomePageControl::DeleteHelpCursor()
{
	m_pHomePageCursor->RemoveDelegate();
}

ECView* HomePageControl::MainMenuSence()
{
	SoundManager::shareSoundManager()->playBackgroundMusic(STARTBACKMUSIC,true);

	m_pHomePageCursor->DelegateCursor();
	ECView *scene = new ECView();
	HomePage* pHomepage = new HomePage();
	m_pHomePageCursor->SetHomePage(pHomepage);
	scene->AddChild(pHomepage,1);
	//pHomepage->release();
	return scene;
}

ECView* HomePageControl::NewGame()
{
	SoundManager::shareSoundManager()->playBackgroundMusic(GAMEBACKMUSIC,true);

	m_pHomePageCursor->RemoveDelegate();
	ECView *scene1 = new ECView();
	GameScene* pGameScene = new GameScene();
	scene1->AddChild(pGameScene,1,1);
//	printf("RemoveDelegate\n");
	return scene1;
}

ECView* HomePageControl::HelpInfoData()
{
	ECView* scene2 = new ECView();
	HelpInfo* pHelpInfo = new HelpInfo();
	m_pHomePageCursor->SetHelpInfo(pHelpInfo);
	scene2->AddChild(pHelpInfo,1);
	return scene2;
}

