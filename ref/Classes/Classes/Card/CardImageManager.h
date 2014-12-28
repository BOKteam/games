/*
 * CardImageManager.h
 *
 *  Created on: Oct 9, 2012
 *      Author: root
 */

#ifndef CARDIMAGEMANAGER_H_
#define CARDIMAGEMANAGER_H_
#include "EnveeCore.h"
#include <vector>
using namespace NSEnveeCore;
#define BOMB_EFFECT_COUNT 20
struct View
{
	ECView* pImage;
	char path[64];
};
class CardImageManager
{
	std::vector<View*> m_pImageList;

	static CardImageManager* m_Interface;

	CardImageManager();
	~CardImageManager();
	void InitNewImage(const char* path);
	void Clear();
public:
	static CardImageManager* Singleton();

	ECView* GetView(const char* path);

};



#endif /* CARDIMAGEMANAGER_H_ */
