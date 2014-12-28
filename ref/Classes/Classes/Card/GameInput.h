/*
 * GameInput.h
 *
 *  Created on: Nov 1, 2012
 *      Author: root
 */

#ifndef GAMEINPUT_H_
#define GAMEINPUT_H_
#include "EnveeCore.h"
using namespace NSEnveeCore;
class ActorPlayerUI;
class ActorPlayer;
class GameInput:public ECView
{
	CursorMoveInfo m_pCallBankerInfo;
	std::vector<CursorMoveRow*> m_pCallBankerWay;

	CursorMoveInfo m_pGameInfo;
	std::vector<CursorMoveRow*> m_pGameWay;

	//CursorMoveInfo m_pDoubleInfo;
	//std::vector<ECPoint> m_pDoubleWay;

	ActorPlayerUI *m_pGameSceneUI;
	ActorPlayer* m_pActorPlayer;
private:
	void SetGameCursor();
public:
	GameInput(ActorPlayerUI* pGameSceneUI,ActorPlayer* pActorPlayer);
	~GameInput();
public:
	virtual void OnKeyUp(ECEvent* event);

	void SetFocusCallBanker(int nScore);
	void SetFocusGame();

	void DisableFocus();
};



#endif /* GAMEINPUT_H_ */
