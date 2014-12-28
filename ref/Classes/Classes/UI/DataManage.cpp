/*
 * DataManage.cpp
 *
 *  Created on: Dec 5, 2011
 *      Author: root
 */

#include "DataManage.h"
#include "EnveeCore.h"
DataManage::DataManage()
{
	Init();
}
DataManage::~DataManage()
{
	delete m_pSingleton;
	m_pSingleton=NULL;
}
DataManage* DataManage::m_pSingleton=NULL;
DataManage* DataManage::Singleton()
{
	if(m_pSingleton == NULL)
	{
		m_pSingleton = new DataManage();
	}
	return m_pSingleton;
}

bool DataManage::Init()
{
	EffectSound = 0.6;
	BackMusicSound = 0.6;
	return true;
}

void DataManage::SetEffectSound()
{
	SoundManager::shareSoundManager()->setEffectsVolume(EffectSound);
}
void DataManage::SetBackMusicSound()
{
	SoundManager::shareSoundManager()->setBackgroundMusicVolume(BackMusicSound);
}
float DataManage::MinusEffectSound()
{
	EffectSound = EffectSound-0.1;
	if(EffectSound<0)
	{
		EffectSound=0;
	}
	return EffectSound;
}
float DataManage::AddEffectSound()
{
	EffectSound = EffectSound+0.1;
	if(EffectSound>1)
	{
		EffectSound = 1;
	}
	return EffectSound;
}
float DataManage::MinusBackMusicSound()
{
	BackMusicSound = BackMusicSound-0.1;
	if(BackMusicSound<0)
	{
		BackMusicSound = 0 ;
	}
	return BackMusicSound;
}
float DataManage::AddBackMusicSound()
{
	BackMusicSound = BackMusicSound+0.1;
	if(BackMusicSound>1)
	{
		BackMusicSound = 1;
	}
	return BackMusicSound;
}
void DataManage::SetCurrentLevel(int hard)
{
	m_nHardID = hard;
}
int DataManage::GetCurrentLevel()
{
	return m_nHardID;
}

