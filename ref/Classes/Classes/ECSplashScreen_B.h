/*
 * ECSplashScreen_B.h
 *
 *  Created on: Nov 4, 2011
 *      Author: BigD
 */

#ifndef ENVEECORE_ECSplashScreen_B_H_
#define ENVEECORE_ECSplashScreen_B_H_

//#include "UI/ECNode.h"
#include "EnveeCore.h"
namespace NSEnveeCore
{

// Forward Declarations.
class ECView;

class ECSplashScreen_B: public NSEnveeCore::ECNode
{
public:
	ECSplashScreen_B();
	ECSplashScreen_B(const char* path);
	virtual ~ECSplashScreen_B();

	static ECSplashScreen_B* CreateSplashScreenWithFile(const char* fileName);

	void ButtonClicked();

private:
	void UpdateAlpha(float dt);

private:
	ECView* m_pSplashView;
};

}
#endif
