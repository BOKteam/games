/*
 * HomePageCursor.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: root
 */
#include "HomePageCursor.h"
#include "HomePage.h"
#include "HomePageControl.h"
#include "HelpInfo.h"
int CursorCount=0;
HomePageCursor::HomePageCursor()
{
	InitState();
	//DelegateCursor();
	ECEventDispatcher::Singleton()->SetIsResponseKeyboard(true);
}
HomePageCursor::~HomePageCursor()
{

		//ECEventDispatcher::Singleton()->SetIsResponseKeyboard(false);
		//delete m_pCursor;
		//printf("Remove HomePageCursor\n");
}
void HomePageCursor::DelegateCursor()
{
	CursorCount = CursorCount+1;
	//printf("CursorCount = %d \n",CursorCount);
	ECEventDispatcher::Singleton()->addTargetedDelegate(this, 1, true);
	//printf("Add CursorDelegate!!\n");
}
void HomePageCursor::RemoveDelegate()
{
	CursorCount = CursorCount-1;
	//printf("CursorCount = %d \n",CursorCount);
	ECEventDispatcher::Singleton()->removeDelegate(this);
	//printf("Remove CursorDelegate!!!!\n");
}
void HomePageCursor::InitState()
{
	m_Status = Start;
	m_Levle = None;
	m_Sound = None1;
	m_Help = None2;
	m_Quit = None3;
}
void HomePageCursor::SetHomePage(HomePage* pHomePage)
{
	m_pHomePage = pHomePage;
}
void HomePageCursor::SetHelpInfo(HelpInfo* pHelpInfo)
{
	m_pHelpInfo = pHelpInfo;
}
bool HomePageCursor::OnTouchBegan(ECEvent* event)
{
	return true;
}
void HomePageCursor::OnTouchMove(ECEvent* event)
{

}
void HomePageCursor::OnTouchEnd(ECEvent* event)
{

}
void HomePageCursor::OnTouchCancel(ECEvent* event)
{

}

void HomePageCursor::OnKeyUp(ECEvent* event)
{

}
void HomePageCursor::LeftKey()
{
	switch(m_Status)
			{
			  case Start:
				  break;
			  case Setting:
			  	  {
			  		  switch(m_Sound)
			  		  {
			  		  	  case None1:
			  		  		  break;
			  		  	  case SoundMusic:
			  		  	  {
			  		  		  m_pHomePage->MusicSoundMinus();
			  		  		  break;
			  		  	  }
			  		  	  case SoundEffect:
			  		  	  {
			  		  		  m_pHomePage->EffecSoundtMinus();
			  		  		  break;
			  		  	  }
			  		  }
			  		  break;
			  	  }
			  case Help:
			  	  {
			  		switch(m_Help)
			  	    {
			  		   case None2:
			  			 break;
			  		   case NOnone2:
			  		    {
			  			 m_pHelpInfo->ShowPrePicture();
			  			 break;
			  			}
			  		}
			  		break;
			  	  }
			  case Quit:
			 	  {
			 		  break;
			 	  }
			}
}
void HomePageCursor::RightKey()
{
	switch(m_Status)
		{
			case Start:
				break;
			case Setting:
			{
				switch(m_Sound)
				  {
					case None1:
					  break;
				    case SoundMusic:
					  {
						  m_pHomePage->MusicSoundAdd();
						  break;
					  }
				     case SoundEffect:
					 {
						 m_pHomePage->EffecSoundtAdd();
						   break;
					 }
				  }
				break;
			}
			case Help:
			{
			  switch(m_Help)
			  {
			  	  case None2:
			  		  break;
			  	  case NOnone2:
			  	  {
			  		  m_pHelpInfo->ShowNextPicture();
			  	      break;
			  	  }
			   }
				break;
			}

			case Quit:
				break;
		}
}
void HomePageCursor::UpKey()
{
	switch(m_Status)
			{
				case Quit:
				{
					switch(m_Quit)
					{
						case None3:
						{
							m_Status = Help;
							m_pTepCursor->SetPosition((ECPoint)ECPoint(835,350));
							break;
						}
						case Nonone3:
						{
							break;
						}
					}
					break;
				}
				case Help:
				{
					switch(m_Help)
					{
						case None2:
						{
							m_Status = Setting;
							m_pTepCursor->SetPosition((ECPoint)ECPoint(835,270));
							break;
						}
						case NOnone2:
						{
							break;
						}
					}
					break;
				}
				case Setting:
				{
					switch(m_Sound)
					{
						case None1:
						{
							m_Status = Start;

							m_pTepCursor->SetPosition((ECPoint)ECPoint(835,190));
							break;
						}
						case SoundEffect:
						{
							m_pHomePage->ShowMusicBackImage();
							m_pTepCursor->SetPosition((ECPoint)ECPoint(525,250));
							m_Sound =SoundMusic;
							break;
						}
						case SoundMusic:
						{
							break;
						}
					}
					break;
				}
				case Start:
				/*{
					switch(m_Levle)
					{
						case None:
						{
							break;
						}
						case Level3:
						{
							m_pTepCursor->SetPosition((ECPoint)ECPoint(815,230));
							m_Levle = Level2;
							break;
						}
						case Level2:
						{
							m_pTepCursor->SetPosition((ECPoint)ECPoint(815,180));
					    	m_Levle = Level1;
							break;
						}
						case Level1:
						{
							break;
						}
						break;
					}
				}*/
				break;
			}
}
void HomePageCursor::DownKey()
{
	switch(m_Status)
			{
				case Start:
				 {
					 switch(m_Levle)
					  {
						case None:
						{
							m_pTepCursor->SetPosition((ECPoint) ECPoint(835,270));
							m_Status = Setting;
							break;
						}
						/*case Level1:
						{
							m_pTepCursor->SetPosition((ECPoint)ECPoint(815,230));
							m_Levle = Level2;
							break;
						}
						case Level2:
						{
							m_pTepCursor->SetPosition((ECPoint)ECPoint(815,280));
							m_Levle = Level3;
							break;
						}
						case Level3:
						{
							break;
						}*/
					  }
					 break;
				 }
				case Setting:
				{
					switch(m_Sound)
					{
						case None1:
						{
							m_Status = Help;
							m_pTepCursor->SetPosition((ECPoint)ECPoint(835,350));
							break;
						}
						case SoundMusic:
						{
							m_pHomePage->ShowEffectBackImage();
							m_pTepCursor->SetPosition((ECPoint)ECPoint(525,310));
							m_Sound = SoundEffect;
							break;
						}
						case SoundEffect:
						{
							break;
						}
					}
				    break;
				}
				case Help:
				{
					switch(m_Help)
					{
						case None2:
						{
							m_Status = Quit;
							m_pTepCursor->SetPosition((ECPoint)ECPoint(835,430));
							break;
						}
						case NOnone2:
						{
							break;
						}
					}
					 break;
				}
				case Quit:
				{
					break;
				}
			}
}
void HomePageCursor::JFuncKey()
{
	switch(m_Status)
			{
				case Start:
				{
					m_pHomePage->Simple();
					/*switch(m_Levle)
					{
						case None:
						{
							m_Levle = Level1;
							m_pTepCursor->SetPosition((ECPoint)ECPoint(815,180));
							m_pHomePage->ShowChooseLevelButton();
							break;
						}
						case Level1:
						{
							m_pHomePage->Simple();
							break;
						}
						case Level2:
						{
							m_pHomePage->Simple();
							break;
						}
						case Level3:
						{
							m_pHomePage->Simple();
							break;
						}
						break;
					}*/
					break;
				}
				case Setting:
				{
					switch(m_Sound)
					{
						case None1:
						{
							m_Sound = SoundMusic;
							m_pHomePage->ShowMusicBackImage();
							m_pTepCursor->SetPosition((ECPoint)ECPoint(525,250));
							m_pHomePage->ShowChooseSoundButton();
							break;
						}
						case SoundMusic:
						{
							break;
						}
						case SoundEffect:
						{
							break;
						}
					}
					break;
				}
				case Help:
				{
					switch(m_Help)
					{
						case None2:
						{
							m_pHomePage->Help();
							m_Help = NOnone2;
							break;
						}
						case NOnone2:
						{
							break;
						}
					}
					break;
				}
				case Quit:
				{
					m_pHomePage->QuitGame();
					break;
				}
			}
}
void HomePageCursor::KFuncKey()
{

	if (isHelpPage)
	{
		// TODO: back home page
		m_pHelpInfo->BackMainMenu();
		isHelpPage = false;
		//SDL_Log("m_pHelpInfo->BackMainMenu();");
	}
	else if (isSettingPage)
	{
		m_pHomePage->SetSoundButtonShow(false);
		m_pTepCursor->SetPosition((ECPoint)ECPoint(835,270));
		m_Status = Setting;
		m_Sound = None1;
		isSettingPage = false;
	}
	else
	{
		// TODO: Call void HomePage::ShowPopup()
		m_pHomePage->ShowPopup();
	}

	switch(m_Status)
			{
				case Start:
				{
					switch(m_Levle)
					{
						case None:
							break;
						case Level1:
						{
							m_pHomePage->SetButtonShow(false);
							m_pTepCursor->SetPosition((ECPoint)ECPoint(835,190));
							InitState();
							break;
						}
						case Level2:
						{
							m_pHomePage->SetButtonShow(false);
							m_pTepCursor->SetPosition((ECPoint)ECPoint(835,190));
							InitState();
							break;
						}
						case Level3:
						{
							m_pHomePage->SetButtonShow(false);
							m_pTepCursor->SetPosition((ECPoint)ECPoint(835,190));
							InitState();
							break;
						}
					}
					break;
				}
				case Setting:
				{
					switch(m_Sound)
					{
						case None1:
						{
							break;
						}/*
						case SoundEffect:
						{
							m_pHomePage->SetSoundButtonShow(false);
							m_pTepCursor->SetPosition((ECPoint)ECPoint(835,270));
							m_Status = Setting;
							m_Sound = None1;
							break;
						}
						case SoundMusic:
						{
							m_pHomePage->SetSoundButtonShow(false);
							m_pTepCursor->SetPosition((ECPoint)ECPoint(835,270));
							m_Sound = None1;
							break;
						}*/
					}
					break;
				}
				case Help:
				{
					switch(m_Help)
					{
					   case None2:
						   break;
					   case NOnone2:
					   {
						  //m_pHelpInfo->BackMainMenu();
						  InitState();
						  break;
					   }
					}
					break;
				}
				case Quit:
				{
					break;
				}
			}
}
void HomePageCursor::LFuncKey()
{
	switch(m_Status)
	{
	case Help:
	{
		switch(m_Help)
		{
			case None2:break;
			case NOnone2:
			{
				m_pHelpInfo->BackMainMenu();
				InitState();
				break;
			}
		}
	break;
	}
	}
}
void HomePageCursor::OnKeyDown(ECEvent* event)
{
    m_pTepCursor = m_pHomePage->GetCursor();
    if (isPopupOn)
    {
    	if (FUNCTION_J_KEY == event->m_key)
    	{
			SDL_Quit();
			exit(0);
    	}
    	else if (FUNCTION_L_KEY == event->m_key)
    	{
    		m_pHomePage->HidePopup();
    	}
    	return;
    }

	switch (event->m_key)
 {
case LEFT_KEY:
	 {
		LeftKey();
		break;
	 }
case RIGHT_KEY:
	{
		RightKey();
		break;
	}
case UP_KEY:
	{
		UpKey();
		break;
	}
case DOWN_KEY:
	{
		DownKey();
		break;
	}
case  FUNCTION_J_KEY:
	{
		JFuncKey();
		break;
	}
case  FUNCTION_K_KEY:
	{

		break;
	}
case  FUNCTION_L_KEY:
{
	KFuncKey();
	//LFuncKey();
	break;
}
default:
		break;
}

	AdjustCursorPosition();

//	ECPoint cursorPos = ECPoint(m_cursorPosition.x - m_pCursor->GetView()->w / 2,
//								m_cursorPosition.y - m_pCursor->GetView()->h / 2);
//	m_pCursor->SetPosition(cursorPos);
}

void HomePageCursor::AdjustCursorPosition()
{

}


