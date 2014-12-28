/*
 * GameActorScoreManage.h
 *
 *  Created on: Nov 23, 2011
 *      Author: root
 */

#ifndef GAMEACTORSCOREMANAGE_H_
#define GAMEACTORSCOREMANAGE_H_
#include <stdio.h>

class GameActorScoreManage
{
	int m_nActorScore[3];
	int m_nCurrentScore;

	static GameActorScoreManage* m_Instance;
public:
	static GameActorScoreManage* Singleton();

	GameActorScoreManage(){m_nActorScore[0] = 0;m_nActorScore[1] = 0;m_nActorScore[2] = 0;m_nCurrentScore = 0;}
	void SetCurrentScore(int TurnScore){m_nCurrentScore =TurnScore;}
	int  GetCurrentScore(){return m_nCurrentScore;}
	int  GetActorScore(int ID){return m_nActorScore[ID];}
	void SetBomb(){m_nCurrentScore *= 2;}
	bool SetActorWin(int nChairID,int nBankerID);
};
#endif /* GAMEACTORSCOREMANAGE_H_ */
