/*
 * GameScoreManage.h
 *
 *  Created on: Nov 23, 2011
 *      Author: root
 */

#ifndef GAMESCOREMANAGE_H_
#define GAMESCOREMANAGE_H_
class GameTurnScoreManage
{
public:
	GameTurnScoreManage(){InitCurrentScore();}
	void InitCurrentScore(){m_nCurrentTurnCount = 3;}
	void SetScoreDouble(){m_nCurrentTurnCount*=2;}
	int  GetTurnScore(){return m_nCurrentTurnCount;}
private:
	int m_nCurrentTurnCount;
};



#endif /* GAMESCOREMANAGE_H_ */
