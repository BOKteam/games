/*
 * HomePageControl.h
 *
 *  Created on: Nov 13, 2011
 *      Author: root
 */

#ifndef HOMEPAGECONTROL_H_
#define HOMEPAGECONTROL_H_

#include "EnveeCore.h"

class HomePageCursor;
class HelpInfo;
class ChessGameController;
using namespace NSEnveeCore;

class HomePageControl : public ECNode,public ECDelegate
{

private:
	HomePageControl();
	static HomePageControl* m_Interface;
	HomePageCursor* m_pHomePageCursor;
	HelpInfo* m_pHelpInfo;
//	ChessGameController* m_pChessGameController;
public:
	virtual ~HomePageControl();
	void Init();
	void InitCursorData();
	//void RemoveDelegateCursor();
	void DelegateCursor();
	void SetNewHealth();
	void DeleteHelpCursor();
	static HomePageControl * GetInterface();

	//void SetHomePageCursor(HomePageCursor* pHomePageCursor);

public:
	//ECView* GameScene();
	//ECView* FirstScene();
	//ECView* ScoreScene();
	//ECView* LoadingScene();
	ECView* MainMenuSence();
	ECView* NewGame();
	ECView* HelpInfoData();
	//ECView* HelpInfoSence();
	//ECView* GameOverSence();
};




#endif /* HOMEPAGECONTROL_H_ */
