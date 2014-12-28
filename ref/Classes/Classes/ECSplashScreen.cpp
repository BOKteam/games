/*
 * ECSplashScreen.cpp
 *
 *  Created on: Nov 4, 2011
 *      Author: root
 */

#include "ECSplashScreen.h"
#include "ECSplashScreen_B.h"
namespace NSEnveeCore
{
ECSplashScreen::ECSplashScreen()
{

}

ECSplashScreen::ECSplashScreen(const char* path)
{
	m_pSplashView = ECView::InitWithFile(path);
	m_pSplashView->Scale((float)EC_SCREEN_W / (float)m_pSplashView->GetView()->w,
	    (float)EC_SCREEN_H / (float)m_pSplashView->GetView()->h);

	//SDL_SetAlpha(ECDirector::Singleton()->GetScreen(), SDL_SRCALPHA, 96);

	this->AddChild(m_pSplashView, 0);
}

void ECSplashScreen::ButtonClicked()
{
	ECDirector::Singleton()->SetRunScene(0);
}

ECSplashScreen::~ECSplashScreen()
{

}

ECSplashScreen* ECSplashScreen::CreateSplashScreenWithFile(const char* fileName)
{
	ECSplashScreen* pSplash = new ECSplashScreen(fileName);

	pSplash->Schedule(schedule_selector(ECSplashScreen::UpdateAlpha), 2.0f);

	return pSplash;
}

void ECSplashScreen::UpdateAlpha(float dt)
{
	//
	//GameChessManage* pGameScene = new GameChessManage();
   // GameScene* pGameSceneUI = new GameScene();
	ECNode* pInitScene = ECSplashScreen_B::CreateSplashScreenWithFile("Resources/LoadingImage_B.png");
	ECDirector::Singleton()->SetRunScene(pInitScene);
	//ECDirector::Singleton()->SetRunScene(GameScene::Singleton());
	//SDL_SetAlpha(ECDirector::Singleton()->GetScreen(), SDL_SRCALPHA, 255);

	//    ECView* pHomePageControl = HomePageControl::GetInterface()->MainMenuSence();

	//	ECDirector::Singleton()->SetRunScene(pHomePageControl);


}

}
