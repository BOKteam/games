/*
 * HomePage.cpp
 *
 *  Created on: Nov 13, 2011
 *      Author: root
 */
#include "HomePage.h"
#include "HomePageControl.h"
#include "DataManage.h"
#include "HomePageCursor.h"
#include "Resourse.h"
HomePage::HomePage()
{
	Init();
}
HomePage::~HomePage()
{
//   delete chose3;
}
bool HomePage::Init()
{
	initLoading();

	inLoadingRes(true);
//	m_pHomePageCursor = new HomePageCursor();
	InitBackground();
	InitButton();
	InitLevelChooseButton();
	InitMusicChooseButton();
	InitPopup();
	inLoadingRes(false);
	return true;
}
void HomePage::InitBackground()
{
	   m_cursorPos = ECPoint(835, 190);
	   m_pCursor = ECView::InitWithFile("Resources/GameUI/CurrentCursor.png");
	   this->AddChild(m_pCursor,10);
	   m_pCursor->SetPosition(m_cursorPos);
	   ECView* m_pBackground = ECView::InitBackgroundWithFile("Resources/GameUI/bg01.png");
//	   m_pBackground->Scale((float) (SDL_GetVideoInfo()->current_w )/(float) (m_pBackground->GetView()->w),
//			                (float) (SDL_GetVideoInfo()->current_h )/(float)(m_pBackground->GetView()->h));
	   this->AddChild(m_pBackground, -1);
	   /*chose1 = ECView::InitWithFile("Resources/GameUI/red_point.png");
	   this -> AddChild(chose1, 1);*/
}
void HomePage::ShowChooseLevelButton()
{
	/*chose1->SetPosition((ECPoint)ECPoint(515,185));
	SetButtonShow(true);
	SetSoundButtonShow(false);*/
}
void HomePage::ShowChooseSoundButton()
{
	//chose1->SetPosition((ECPoint)ECPoint(515,265));
	isSettingPage = true;
	SetSoundButtonShow(true);
	SetButtonShow(false);
	if(MusicRect.width <= 0)
	{
		NoSound->SetIsVisual(true);
	}
	if(EffectRect.width<=0)
	{
		NoSound2->SetIsVisual(true);
	}
}
void HomePage::InitButton()
{
	HomePage_PlayButton = ECButton::InitWithFile("Resources/GameUI/start.png", "Resources/GameUI/start02.png",callback_selector(HomePage::Simple),this);
	HomePage_PlayButton->SetPosition((ECPoint)ECPoint(750,160));
	this->AddChild(HomePage_PlayButton, 2);
	/*m_pBlackline = ECView::InitWithFile("Resources/GameUI/blackline.png");
	m_pBlackline->SetPosition((ECPoint)ECPoint(500,170));
	this -> AddChild(m_pBlackline, 0);*/

	HomePage_Setting = ECButton::InitWithFile("Resources/GameUI/options.png","Resources/GameUI/options02.png",callback_selector(HomePage::ShowChooseSoundButton),this);
	HomePage_Setting->SetPosition((ECPoint)ECPoint(750,240));
	this->AddChild(HomePage_Setting,2);
	/*m_pBlackline = ECView::InitWithFile("Resources/GameUI/blackline.png");
	m_pBlackline->SetPosition((ECPoint)ECPoint(500,250));
	this -> AddChild(m_pBlackline, 0);*/

    HomePage_HelpButton = ECButton::InitWithFile("Resources/GameUI/help.png", "Resources/GameUI/help02.png",callback_selector(HomePage::Help),this);
    HomePage_HelpButton->SetPosition((ECPoint)ECPoint(750,320));
    this->AddChild(HomePage_HelpButton,2);
    /*m_pBlackline1 = ECView::InitWithFile("Resources/GameUI/blackline.png");
    m_pBlackline1->SetPosition((ECPoint)ECPoint(500,330));
    this -> AddChild(m_pBlackline1, 0);*/

    HomePage_QuitButton = ECButton::InitWithFile("Resources/GameUI/quit.png", "Resources/GameUI/quit02.png",callback_selector(HomePage::QuitGame),this);
    HomePage_QuitButton->SetPosition((ECPoint)ECPoint(750,400));
    this->AddChild(HomePage_QuitButton,2);
    /*m_pBlackline2 = ECView::InitWithFile("Resources/GameUI/blackline.png");
    m_pBlackline2->SetPosition((ECPoint)ECPoint(500,410));
    this -> AddChild(m_pBlackline2, 0);*/
}
void HomePage::InitLevelChooseButton()
{
	/*chose1->SetPosition((ECPoint)ECPoint(515,185));
	LevelChoiceBg = ECView::InitWithFile("Resources/GameUI/levelbg.png");
	LevelChoiceBg->SetPosition((ECPoint)ECPoint(760,130));
	this->AddChild(LevelChoiceBg,1);

	simple = ECButton::InitWithFile("Resources/GameUI/easy.png",
									"Resources/GameUI/easy02.png",
									 callback_selector(HomePage::Simple),this);
	simple->SetPosition((ECPoint)ECPoint(760,150));
	this->AddChild(simple, 2);

	middle = ECButton::InitWithFile("Resources/GameUI/normal.png",
								    "Resources/GameUI/normal02.png",
									 callback_selector(HomePage::Middle),this);
	middle->SetPosition((ECPoint)ECPoint(760,200));
	this->AddChild(middle, 2);

	 high = ECButton::InitWithFile("Resources/GameUI/hard.png",
								   "Resources/GameUI/hard02.png",
								    callback_selector(HomePage::High),this);
	high->SetPosition((ECPoint)ECPoint(760,250));
	this->AddChild(high, 2);
	 SetButtonShow(false);*/
}

void HomePage::InitMusicChooseButton()
{
    NoSound = ECView::InitWithFile("Resources/GameUI/mute.png");
    this->AddChild(NoSound,10);
    NoSound2 = ECView::InitWithFile("Resources/GameUI/mute.png");
    this->AddChild(NoSound2,10);
    SoundPic = ECView::InitWithFile("Resources/GameUI/volume_bg.png");
    SoundPic->SetPosition((ECPoint)ECPoint(445,208));
    this->AddChild(SoundPic, 5);

    MusicVolume = ECView::InitWithFile("Resources/GameUI/volume02.png");
    MusicVolume->SetPosition((ECPoint)ECPoint(554,243));
    MusicPosX = MusicVolume->GetRect().width;
    MusicPosY = MusicVolume->GetRect().height;
    MusicRect.x=0;
    MusicRect.y=0;
    MusicRect.width=(int)(MusicPosX*(DataManage::Singleton()->BackMusicSound));
    MusicRect.height=MusicPosY;
    MusicVolume->SetDrawRect(MusicRect);
    DataManage::Singleton()->SetBackMusicSound();
    this->AddChild(MusicVolume,6);

    EffectVolume= ECView::InitWithFile("Resources/GameUI/volume02.png");
    EffectVolume->SetPosition((ECPoint)ECPoint(554,296));
    EffectPosX = EffectVolume->GetRect().width;
    EffectPosY = EffectVolume->GetRect().height;
    EffectRect.x=0;
    EffectRect.y=0;
    EffectRect.width=(int)(EffectPosX*(DataManage::Singleton()->EffectSound));
    EffectRect.height=EffectPosY;
    EffectVolume->SetDrawRect(EffectRect);
    DataManage::Singleton()->SetEffectSound();
    this->AddChild(EffectVolume,6);

    MusicMinus  = ECButton::InitWithFile("Resources/GameUI/off01.png", "Resources/GameUI/off02.png",callback_selector(HomePage::MusicSoundMinus),this);
    MusicMinus->SetPosition((ECPoint)ECPoint(515,233));
    this->AddChild(MusicMinus,5);
    //MusicMinus->SetInputRectScale(1.2f);

    MusicAdd = ECButton::InitWithFile("Resources/GameUI/on01.png", "Resources/GameUI/on02.png",callback_selector(HomePage::MusicSoundAdd),this);
    MusicAdd->SetPosition((ECPoint)ECPoint(682,233));
    this->AddChild(MusicAdd,5);
    //MusicAdd->SetInputRectScale(1.2f);

    EffectMinus = ECButton::InitWithFile("Resources/GameUI/off01.png", "Resources/GameUI/off02.png",callback_selector(HomePage::EffecSoundtMinus),this);
    EffectMinus->SetPosition((ECPoint)ECPoint(515,290));
    this->AddChild(EffectMinus,5);
    //EffectMinus->SetInputRectScale(1.2f);

    EffectAdd = ECButton::InitWithFile("Resources/GameUI/on01.png", "Resources/GameUI/on02.png",callback_selector(HomePage::EffecSoundtAdd),this);
    EffectAdd->SetPosition((ECPoint)ECPoint(682,290));
    this->AddChild(EffectAdd,5);
    //EffectAdd->SetInputRectScale(1.2f);
    SetSoundButtonShow(false);
}
void HomePage::MusicSoundMinus()
{
	ShowMusicBackImage();
	MusicRect.width = (int)(MusicPosX*(DataManage::Singleton()->MinusBackMusicSound()));
	DataManage::Singleton()->SetBackMusicSound();
	if(MusicRect.width <= 0)
	{
		NoSound->SetPosition((ECPoint)ECPoint(479,242));		NoSound->SetIsVisual(true);
		MusicRect.width=0;
	}
	MusicVolume->SetDrawRect(MusicRect);
}
void HomePage::ShowMusicBackImage()
{
	/*SoundPicBg1->SetPosition((ECPoint)ECPoint(750,233));
	SoundPicBg2->SetPosition((ECPoint)ECPoint(750,273));*/
}
void HomePage::ShowEffectBackImage()
{
	/*SoundPicBg1->SetPosition((ECPoint)ECPoint(750,273));
	SoundPicBg2->SetPosition((ECPoint)ECPoint(750,233));*/
}
void HomePage::MusicSoundAdd()
{
	ShowMusicBackImage();
	MusicRect.width = (int)(MusicPosX*(DataManage::Singleton()->AddBackMusicSound()));
	DataManage::Singleton()->SetBackMusicSound();
	if(MusicRect.width > 0)
	{
		NoSound->SetIsVisual(false);
	}
	if(MusicRect.width >= MusicPosX)
	{
		MusicRect.width = MusicPosX;
	}
	MusicVolume->SetDrawRect(MusicRect);
}
void HomePage::EffecSoundtMinus()
{
	ShowEffectBackImage();
	EffectRect.width = (int)(EffectPosX*(DataManage::Singleton()->MinusEffectSound()));
	DataManage::Singleton()->SetEffectSound();
	if(EffectRect.width <= 0)
	{
		NoSound2->SetPosition((ECPoint)ECPoint(479,299));
		NoSound2->SetIsVisual(true);
		EffectRect.width=0;
	}
	EffectVolume->SetDrawRect(EffectRect);
}
void HomePage::EffecSoundtAdd()
{
	ShowEffectBackImage();
	EffectRect.width = (int)(EffectPosX*(DataManage::Singleton()->AddEffectSound()));
	DataManage::Singleton()->SetEffectSound();
	if(EffectRect.width > 0)
	{
			NoSound2->SetIsVisual(false);
	}
	if(EffectRect.width >= EffectPosX)
	{
		EffectRect.width = EffectPosX;
	}
	EffectVolume->SetDrawRect(EffectRect);
}

void HomePage::Help()
{
//	SoundPic->SetIsDisable(true);
	inLoadingRes(true);
	isHelpPage = true;
	SetButtonShow(false);
	SetSoundButtonShow(false);
	m_pCursor->SetIsVisual(false);
    //chose1->SetPosition((ECPoint)ECPoint(550,320));
    ECView *sc = HomePageControl::GetInterface()->HelpInfoData();
    ECDirector::Singleton()->SetRunScene(sc);
    inLoadingRes(false);
}
void HomePage::QuitGame()
{
//	SetButtonShow(false);
    //chose1->SetPosition((ECPoint)ECPoint(550,400));
//	SDL_Quit();
//    exit(0);
	ShowPopup();
}
void HomePage::Simple()
{
//	chose2->SetPosition((ECPoint)ECPoint(800,240));
//	DataManage::Singleton()->SetCurrentLevel(2);
	inLoadingRes(true);
	ECView *sc = HomePageControl::GetInterface()->NewGame();
	ECDirector::Singleton()->SetRunScene(sc);
	inLoadingRes(false);
}
/*
void HomePage::Middle()
{
	DataManage::Singleton()->SetCurrentLevel(3);
	ECView *sc = HomePageControl::GetInterface()->NewGame();
	ECDirector::Singleton()->SetRunScene(sc);
}
void HomePage::High()
{
	DataManage::Singleton()->SetCurrentLevel(4);
	ECView *sc = HomePageControl::GetInterface()->NewGame();
	ECDirector::Singleton()->SetRunScene(sc);
}*/

void HomePage::SetButtonShow(bool ShowFlag)
{
	/*LevelChoiceBg->SetIsVisual(ShowFlag);
	simple->SetIsDisable(!ShowFlag);
	simple->SetIsVisual(ShowFlag);
	middle->SetIsDisable(!ShowFlag);
	middle->SetIsVisual(ShowFlag);
	high->SetIsDisable(!ShowFlag);
	high->SetIsVisual(ShowFlag);*/
}

void HomePage::SetSoundButtonShow(bool flag)
{
	NoSound->SetIsVisual(false);
	NoSound2->SetIsVisual(false);
	EffectVolume->SetIsVisual(flag);
	MusicVolume->SetIsVisual(flag);
	SoundPic->SetIsVisual(flag);
	/*SoundPicBg1->SetIsVisual(flag);
	SoundPicBg2->SetIsVisual(flag);*/
//	RedLine->SetIsVisual(flag);
	MusicMinus->SetIsVisual(flag);
	MusicMinus->SetIsDisable(!flag);
	MusicAdd->SetIsVisual(flag);
	MusicAdd->SetIsDisable(!flag);
	EffectMinus->SetIsVisual(flag);
	EffectMinus->SetIsDisable(!flag);
	EffectAdd->SetIsVisual(flag);
	EffectAdd->SetIsDisable(!flag);
}


// TODO: For home page back key popup.
bool isHelpPage = false;
bool isSettingPage = false;
void HomePage::EnableHomePageButton()
{
	// HomePage_PlayButton
	HomePage_PlayButton->SetIsDisable(false);
	// HomePage_Setting
	HomePage_Setting->SetIsDisable(false);
	// HomePage_HelpButton
	HomePage_HelpButton->SetIsDisable(false);
	// HomePage_QuitButton
	HomePage_QuitButton->SetIsDisable(false);

	SetSoundButtonShow(false);
}

void HomePage::DisableHomePageButton()
{
	// HomePage_PlayButton
	HomePage_PlayButton->SetIsDisable(true);
	// HomePage_Setting
	HomePage_Setting->SetIsDisable(true);
	// HomePage_HelpButton
	HomePage_HelpButton->SetIsDisable(true);
	// HomePage_QuitButton
	HomePage_QuitButton->SetIsDisable(true);

	SetSoundButtonShow(false);
}

void HomePage::InitPopup()
{
	// TODO:
	// 1. view
	// 2. Two button

	Quit_Back = ECView::InitWithFile("Resources/GameUI/b_sure.png");
	Quit_Back->SetPosition(ECPoint(350,220));
	this->AddChild(Quit_Back,20);

	Quit_Yes = ECButton::InitWithFile("Resources/GameUI/yes.png","Resources/GameUI/yes02.png",
			callback_selector(HomePage::PopupButtonQuitCB),this);
	Quit_Yes->SetPosition(ECPoint(364,295));
	this->AddChild(Quit_Yes,20);


	Quit_No = ECButton::InitWithFile("Resources/GameUI/delete.png","Resources/GameUI/delete02.png",
			callback_selector(HomePage::PopupButtonCancelCB),this);
	Quit_No->SetPosition(ECPoint(494,295));
	this->AddChild(Quit_No,200);

	HidePopup();

}
bool isPopupOn = false;
void HomePage::HidePopup()
{
	// TODO:
	// 1. Hide and disable button
	Quit_Back->SetIsVisual(false);

	Quit_Yes->SetIsDisable(true);
	Quit_Yes->SetIsVisual(false);

	Quit_No->SetIsDisable(true);
	Quit_No->SetIsVisual(false);

	// 2. Enable HomePage button
	EnableHomePageButton();

	isPopupOn = false;
}

void HomePage::ShowPopup()
{
	// Add: popup is on, hide it.
	if (isPopupOn)
	{
		HidePopup();
		return;
	}

	// TODO:
	// 1. Hide and disable button
	Quit_Back->SetIsVisual(true);

	Quit_Yes->SetIsDisable(false);
	Quit_Yes->SetIsVisual(true);

	Quit_No->SetIsDisable(false);
	Quit_No->SetIsVisual(true);

	// 2. Enable HomePage button
	DisableHomePageButton();
	isPopupOn = true;
}

void HomePage::PopupButtonQuitCB()
{
	SDL_Quit();
	exit(0);
}

void HomePage::PopupButtonCancelCB()
{
	HidePopup();
}


