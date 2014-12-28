/*
 * Gameinput.cpp
 *
 *  Created on: Nov 1, 2012
 *      Author: root
 */




/*
 * GameInput.cpp
 *
 *  Created on: Oct 24, 2012
 *      Author: root
 */


#include "GameInput.h"
#include "ActorPlayerUI.h"
#include "ActorPlayer.h"
#include "GameScene.h"
#include "GameCardManage.h"
#include "GameSceneUI.h"
GameInput::GameInput(ActorPlayerUI* pGameSceneUI,ActorPlayer* pActorPlayer)
{
	ECEventDispatcher::Singleton()->addTargetedDelegate(this,0,true);

	m_pGameSceneUI = pGameSceneUI;
	m_pActorPlayer = pActorPlayer;

	CursorMoveRow* TempMoveGame = new CursorMoveRow();
	CursorMoveRow* TempMoveGameMenu = new CursorMoveRow();
	TempMoveGameMenu->RowList.push_back(ECPoint(290,440));
	TempMoveGameMenu->RowList.push_back(ECPoint(400,440));
	TempMoveGameMenu->RowList.push_back(ECPoint(510,440));
	m_pGameWay.push_back(TempMoveGame);
	m_pGameWay.push_back(TempMoveGameMenu);

	CursorMoveRow* TempMoveBanker = new CursorMoveRow();
	m_pCallBankerWay.push_back(TempMoveBanker);

	ECEventDispatcher::Singleton()->SetIsResponseKeyboard(true);
}

void GameInput::SetGameCursor()
{
	m_pGameWay[0]->RowList.clear();

	for(int i=0;i<m_pActorPlayer->GetCardList().size();i++)
	{
		ECPoint Temp = m_pActorPlayer->GetCardPos()+ECPoint(i*30+10,40);
		m_pGameWay[0]->RowList.push_back(Temp);
	}
}

GameInput::~GameInput()
{
	ECEventDispatcher::Singleton()->removeDelegate(this);
	ECEventDispatcher::Singleton()->SetIsResponseKeyboard(true);

	m_pGameWay[0]->RowList.clear();
	m_pGameWay[1]->RowList.clear();

	delete m_pGameWay[0];
	delete m_pGameWay[1];

	m_pGameWay.clear();
	//m_pCallBankerWay.clear();
}

void GameInput::SetFocusCallBanker(int nScore)
{
	m_pCallBankerWay[0]->RowList.clear();
	if(nScore<1)
		m_pCallBankerWay[0]->RowList.push_back(ECPoint(210,445));
	if(nScore<2)
		m_pCallBankerWay[0]->RowList.push_back(ECPoint(320,445));
	if(nScore<3)
		m_pCallBankerWay[0]->RowList.push_back(ECPoint(430,445));
	m_pCallBankerWay[0]->RowList.push_back(ECPoint(550,445));

	ECEventDispatcher::Singleton()->SetIsResponseKeyboard(false);
	ECEventDispatcher::Singleton()->SetCursorMoveWay(m_pCallBankerWay);
}
void GameInput::SetFocusGame()
{
	SetGameCursor();
	ECEventDispatcher::Singleton()->SetIsResponseKeyboard(false);
	ECEventDispatcher::Singleton()->SetCursorMoveWay(m_pGameWay);
}

void GameInput::DisableFocus()
{
	ECEventDispatcher::Singleton()->SetIsResponseKeyboard(true);
}
void GameInput::OnKeyUp(ECEvent* event)
{
    switch (event->m_key)
    {
        case FUNCTION_L_KEY:
        {
        	if(m_pActorPlayer->GetCardManage()->GetGameScene()->GetGameSceneUI()->GetIsShowResult())
        		return;

    		if(!m_pGameSceneUI->GetIsQuitShow())
    		{
    			m_pGameSceneUI->ShowInquiQuit();
    		}
    		else
    		{
    			m_pGameSceneUI->ShowNotInquiQuit();
    		}
            break;
        }
  		case FUNCTION_J_KEY:
  		{

  			if(m_pGameSceneUI->GetIsQuitShow())
  			{
  				m_pGameSceneUI->ClickExit();
  			}
  			break;
  		}
        default:
          break;
     }
}
