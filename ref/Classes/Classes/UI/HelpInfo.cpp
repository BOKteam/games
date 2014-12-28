/*
 * HelpInfo.cpp
 *
 *  Created on: Jan 16, 2012
 *      Author: root
 */

#include "HelpInfo.h"
#include "HomePageControl.h"

HelpInfo::HelpInfo()
{
	Init();
}
HelpInfo::~HelpInfo()
{

}

void HelpInfo::Init()
{
	HelpSenceCount = 0;
	ECView* HelpPic = ECView::InitBackgroundWithFile("Resources/GameUI/help_bg.png");
	this->AddChild(HelpPic, 1);
	PrePic =ECButton::InitWithFile("Resources/GameUI/pre.png", "Resources/GameUI/pre.png",callback_selector(HelpInfo::ShowPrePicture),this);
    PrePic->SetPosition((ECPoint)ECPoint(6,300));
    this->AddChild(PrePic,6);
	PrePic->SetIsVisual(false);

	NextPic =ECButton::InitWithFile("Resources/GameUI/next.png", "Resources/GameUI/next.png",callback_selector(HelpInfo::ShowNextPicture),this);
    NextPic->SetPosition((ECPoint)ECPoint(935,300));
    this->AddChild(NextPic,6);

	HelpPic1 = ECView::InitWithFile("Resources/GameUI/word01.png");
	HelpPic1->SetPosition((ECPoint)ECPoint(120,105));
	this->AddChild(HelpPic1,5);
    HelpPic1->SetIsVisual(true);

    HelpPic2 = ECView::InitWithFile("Resources/GameUI/word02.png");
    HelpPic2->SetPosition((ECPoint)ECPoint(120,105));
    this->AddChild(HelpPic2,5);
    HelpPic2->SetIsVisual(false);

    HelpPic3 = ECView::InitWithFile("Resources/GameUI/word03.png");
    HelpPic3->SetPosition((ECPoint)ECPoint(120,105));
    this->AddChild(HelpPic3,5);
    HelpPic3->SetIsVisual(false);

    HelpPic4 = ECView::InitWithFile("Resources/GameUI/word04.png");
    HelpPic4->SetPosition((ECPoint)ECPoint(120,105));
    this->AddChild(HelpPic4,5);
    HelpPic4->SetIsVisual(false);

    /*HelpPic3 = ECView::InitWithFile("Resources/GameUI/word03.png");
    HelpPic3->SetPosition((ECPoint)ECPoint(150,240));
    this->AddChild(HelpPic3,5);
	HelpPic3->SetIsVisual(false);



    HelpPic5 = ECView::InitWithFile("Resources/GameUI/word05.png");
    HelpPic5->SetPosition((ECPoint)ECPoint(230,280));
    this->AddChild(HelpPic5,5);
    HelpPic5->SetIsVisual(false);*/

    ECButton* BacktoMainMenuers = ECButton::InitWithFile("Resources/GameUI/back02.png", "Resources/GameUI/back01.png",callback_selector(HelpInfo::BackMainMenu),this);
    BacktoMainMenuers->SetPosition((ECPoint)ECPoint(900,550));
    this->AddChild(BacktoMainMenuers,6);
}

void HelpInfo::BackMainMenu()
{
	HomePageControl::GetInterface()->DeleteHelpCursor();
	ECView *sc = HomePageControl::GetInterface()->MainMenuSence();
	ECDirector::Singleton()->SetRunScene(sc);
}
void HelpInfo::ShowPrePicture()
{
	SetScenceMove(-1);
	IsTheFisrtOrLastPic();
	ShowImage();
}
void HelpInfo::ShowNextPicture()
{
	SetScenceMove(1);
	IsTheFisrtOrLastPic();
	ShowImage();
}

bool HelpInfo::IsTheFisrtOrLastPic()
{
		if(HelpSenceCount <= 0)
			{
			PrePic->SetIsVisual(false);
			PrePic->SetIsDisable(true);

			NextPic->SetIsVisual(true);
			NextPic->SetIsDisable(false);
			HelpSenceCount = 0;
			}
		else if(HelpSenceCount >=3)
			{
			NextPic->SetIsVisual(false);
			NextPic->SetIsDisable(true);

			PrePic->SetIsVisual(true);
			PrePic->SetIsDisable(false);
			HelpSenceCount = 3;
			}
		else
		{
			PrePic->SetIsVisual(true);
			NextPic->SetIsVisual(true);
			PrePic->SetIsDisable(false);
			NextPic->SetIsDisable(false);
		}
		return true;
}
void HelpInfo::SetScenceMove(int i)
{
	HelpSenceCount=HelpSenceCount+i;
	//ShowImage();
}
void HelpInfo::SetHelpPicNotVisual()
{
	HelpPic1->SetIsVisual(false);
	HelpPic2->SetIsVisual(false);
	HelpPic3->SetIsVisual(false);
	HelpPic4->SetIsVisual(false);
	//HelpPic5->SetIsVisual(false);
}
void HelpInfo::ShowImage()
{
	SetHelpPicNotVisual();
	switch(HelpSenceCount)
	{
		case 0:
		{
			HelpPic1->SetIsVisual(true);
			break;
		}
		case 1:
		{
			HelpPic2->SetIsVisual(true);
			break;
		}
		case 2:
		{
			HelpPic3->SetIsVisual(true);
			break;
		}
		case 3:
		{
			HelpPic4->SetIsVisual(true);
			break;
		}
		/*case 4:
		{
			HelpPic5->SetIsVisual(true);
			break;
		}*/
	}
}
