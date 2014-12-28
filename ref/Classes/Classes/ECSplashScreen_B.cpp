/*
 * ECSplashScreen_B.cpp
 *
 *  Created on: Nov 4, 2011
 *      Author: root
 */

#include "ECSplashScreen_B.h"
//#include "UI/ECView.h"
//#include "GameScene.h"
//#include "UI/ECButton.h"
//#include "UI/ECDirector.h"
// TODO Need to specify for each Game.
//#include "MainGameScene.h"
#include "HomePageControl.h"
namespace NSEnveeCore
{
ECSplashScreen_B::ECSplashScreen_B()
{

}

ECSplashScreen_B::ECSplashScreen_B(const char* path)
{
	m_pSplashView = ECView::InitWithFile(path);
	m_pSplashView->Scale((float)EC_SCREEN_W / (float)m_pSplashView->GetView()->w,
	    (float)EC_SCREEN_H / (float)m_pSplashView->GetView()->h);

	//SDL_SetAlpha(ECDirector::Singleton()->GetScreen(), SDL_SRCALPHA, 96);

	this->AddChild(m_pSplashView, 0);
}

void ECSplashScreen_B::ButtonClicked()
{
	ECDirector::Singleton()->SetRunScene(0);
}

ECSplashScreen_B::~ECSplashScreen_B()
{

}

ECSplashScreen_B* ECSplashScreen_B::CreateSplashScreenWithFile(const char* fileName)
{
	ECSplashScreen_B* pSplash = new ECSplashScreen_B(fileName);

	pSplash->Schedule(schedule_selector(ECSplashScreen_B::UpdateAlpha), 2.0f);

	return pSplash;
}

void ECSplashScreen_B::UpdateAlpha(float dt)
{
	//
	//GameChessManage* pGameScene = new GameChessManage();
   // GameScene* pGameSceneUI = new GameScene();
	ECView* pHomePageControl = HomePageControl::GetInterface()->MainMenuSence();
    ECDirector::Singleton()->SetRunScene(pHomePageControl);
	//ECDirector::Singleton()->SetRunScene(GameScene::Singleton());
	//SDL_SetAlpha(ECDirector::Singleton()->GetScreen(), SDL_SRCALPHA, 255);

	//    ECView* pHomePageControl = HomePageControl::GetInterface()->MainMenuSence();

	//	ECDirector::Singleton()->SetRunScene(pHomePageControl);


}

}
