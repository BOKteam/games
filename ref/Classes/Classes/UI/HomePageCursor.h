/*
 * HomePageCursor.h
 *
 *  Created on: Jan 9, 2012
 *      Author: root
 */

#ifndef HOMEPAGECURSOR_H_
#define HOMEPAGECURSOR_H_
#include "EnveeCore.h"
//#include "ChessGameController.h"
class HomePage;
class HelpInfo;
using namespace NSEnveeCore;

class HomePageCursor: public ECDelegate
{
	enum status{
		Start,
		Setting,
		Help,
		Quit
	};
	enum Level{
		None,
		Level1,
		Level2,
		Level3
	};
	enum Sound{
		None1,
		SoundEffect,
		SoundMusic
	};
	enum HelpType
	{
		None2,
		NOnone2
	};
	enum QuitType
	{
		None3,
		Nonone3
	};
public:
		HomePageCursor();
		virtual ~HomePageCursor();
		void DelegateCursor();
		void RemoveDelegate();
		virtual bool OnTouchBegan(NSEnveeCore::ECEvent* event);
		virtual void OnTouchMove(NSEnveeCore::ECEvent* event);
		virtual void OnTouchEnd(NSEnveeCore::ECEvent* event);
		virtual void OnTouchCancel(NSEnveeCore::ECEvent* event);

		virtual void OnKeyDown(NSEnveeCore::ECEvent* event);
		virtual void OnKeyUp(NSEnveeCore::ECEvent* event);
		void LeftKey();
		void RightKey();
		void UpKey();
		void DownKey();
		void JFuncKey();
		void KFuncKey();
		void LFuncKey();
		HomePage* GetHomepage(){return m_pHomePage;}
		void SetHomePage(HomePage* pHomePage);
		void SetHelpInfo(HelpInfo* pHelpInfo);
		void InitState();
private:
	void AdjustCursorPosition();

private:
	ECPoint m_cursorPosition;
//	ECView* m_pCursor;
	HomePage* m_pHomePage;
	HelpInfo* m_pHelpInfo;
	NSEnveeCore::ECView* m_pTepCursor;
	std::vector<ECPoint> m_pMoveWay;
	int m_Status;
	int m_Levle;
	int m_Sound;
	int m_Help;
	int m_Quit;
};

#endif /* HOMEPAGECURSOR_H_ */
