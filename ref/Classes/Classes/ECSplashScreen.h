/*
 * ECSplashScreen.h
 *
 *  Created on: Nov 4, 2011
 *      Author: BigD
 */

#ifndef ENVEECORE_ECSPLASHSCREEN_H_
#define ENVEECORE_ECSPLASHSCREEN_H_

//#include "UI/ECNode.h"
#include "EnveeCore.h"
namespace NSEnveeCore
{

// Forward Declarations.
class ECView;

class ECSplashScreen: public NSEnveeCore::ECNode
{
public:
	ECSplashScreen();
	ECSplashScreen(const char* path);
	virtual ~ECSplashScreen();

	static ECSplashScreen* CreateSplashScreenWithFile(const char* fileName);

	void ButtonClicked();

private:
	void UpdateAlpha(float dt);

private:
	ECView* m_pSplashView;
};

}
#endif
