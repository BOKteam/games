/*
 * Homepage.h
 *
 *  Created on: Nov 13, 2011
 *      Author: root
 */

#ifndef HOMEPAGE_H_
#define HOMEPAGE_H_

#include "EnveeCore.h"

using namespace NSEnveeCore;
extern bool isHelpPage;
extern bool isSettingPage;
extern bool isPopupOn;
class HomePageCursor;
class HomePage : public ECNode
{
public:
	HomePage();
	virtual ~HomePage();
	void Help();
	void QuitGame();
    void SetButtonShow(bool ShowFlag);
    void Simple();
    void Middle();
    void High();
    void InitLevelChooseButton();
    void InitMusicChooseButton();
    void ShowChooseLevelButton();
    void ShowChooseSoundButton();
    void ShowMusicBackImage();
    void ShowEffectBackImage();
    void SetSoundButtonShow(bool flag);
    void MusicSoundMinus();
    void MusicSoundAdd();
    void EffecSoundtMinus();
    void EffecSoundtAdd();

    /* For home page back key */
    void EnableHomePageButton();
    void DisableHomePageButton();
    void InitPopup();
    void HidePopup();
    void ShowPopup();
    void PopupButtonQuitCB();
    void PopupButtonCancelCB();
    ECView* Quit_Back;
	ECButton* Quit_Yes,*Quit_No;

	ECButton* HomePage_PlayButton,*HomePage_Setting,*HomePage_HelpButton,*HomePage_QuitButton;
	//ECButton* simple,*middle,*high;
	ECView* LevelChoiceBg;
	ECView* GetCursor(){return m_pCursor;}
    ECView* SoundPic,*RedLine,*NoSound,*NoSound2;
    ECButton* MusicMinus,*MusicAdd;
    ECButton* EffectMinus,*EffectAdd;
    ECView* EffectVolume,*MusicVolume;
    int MusicPosX,MusicPosY;
    int EffectPosX,EffectPosY;
    ECRect MusicRect;
    ECRect EffectRect;

private:
	virtual bool Init();
    void InitBackground();
    void InitButton();

    ECButton* ButtonStatus;
	//ECView* chose1,*chose2,*m_pBlackline,*m_pBlackline1,*m_pBlackline2;

	HomePageCursor* m_pHomePageCursor;
	ECPoint m_cursorPos;
	ECView* m_pCursor;
//	virtual  void NewGame(ECNode* pSender);
//   virtual  void MenuCloseCallback(ECNode* pSender);
};

#endif /* HOMEPAGE_H_ */
