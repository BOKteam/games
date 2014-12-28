/*
 * GameActorScoreManage.cpp
 *
 *  Created on: Nov 23, 2011
 *      Author: root
 */

#include "GameActorScoreManage.h"
#include <iostream>
GameActorScoreManage* GameActorScoreManage::m_Instance = NULL;
GameActorScoreManage* GameActorScoreManage::Singleton()
{
	if(m_Instance == NULL)
	{
		m_Instance = new GameActorScoreManage();
	}
	return m_Instance;
}
bool GameActorScoreManage::SetActorWin(int nChairID,int nBankerID)
{
	int TempActor_A = m_nActorScore[0];
	printf("Actor %d Win,and Actor Is Banker? %d\n",nChairID,nBankerID==nChairID?1:0);
	for(int i=0;i<3;i++)
	{
		printf("Actor %d 's Score--%d\n",i,m_nActorScore[nChairID]);
	}

	if(nBankerID == nChairID)
	{
		m_nActorScore[nChairID] += m_nCurrentScore*2;
		m_nActorScore[(nChairID+1)%3] -= m_nCurrentScore;
		m_nActorScore[(nChairID+2)%3] -= m_nCurrentScore;
	}
	else
	{
		m_nActorScore[nBankerID] -= m_nCurrentScore*2;
		m_nActorScore[nChairID] += m_nCurrentScore;

		if((nChairID+1) != nBankerID)
		{
			m_nActorScore[(nChairID+1)%3] += m_nCurrentScore;
		}
		else
		{
			m_nActorScore[(nChairID+2)%3] += m_nCurrentScore;
		}
	}
	m_nCurrentScore = 0;

	for(int i=0;i<3;i++)
	{
		printf("Actor %d 's Score--%d\n",i,m_nActorScore[nChairID]);
	}
	return m_nActorScore[0]>TempActor_A;
}

