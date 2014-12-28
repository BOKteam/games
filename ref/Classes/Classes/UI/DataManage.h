/*
 * DataManage.h
 *
 *  Created on: Dec 5, 2011
 *      Author: root
 */

#ifndef DATAMANAGE_H_
#define DATAMANAGE_H_

#include "EnveeCore.h"

using namespace NSEnveeCore;

class DataManage
{
	static DataManage* m_pSingleton;
private:
	int m_nHardID;
public:
	static DataManage* Singleton();

	void  SetEffectSound();
	void  SetBackMusicSound();
	float EffectSound,BackMusicSound;
	float MinusEffectSound();
	float AddEffectSound();
	float MinusBackMusicSound();
	float AddBackMusicSound();
	DataManage();
	~DataManage();
    bool Init();

	int  GetCurrentLevel();
	void SetCurrentLevel(int hard);

};

#endif /* DATAMANAGE_H_ */
