//#include "EVApplicationInit.h"

//#include "SDL/SDL.h"
#include "ECSplashScreen.h"

#include "EnveeCore.h"
using namespace NSEnveeCore;

int SDL_main(int argc,char* argv[])
{
/*
	EVApplicationInit* application = new EVApplicationInit();
	application->ApplicationInit();
	application->ApplicationStartLoop();
	//pressESCtoQuitPlus();//clock()
	delete application;
*/
//	printf("ssssssssssssssssssssss");

	/*bool IsFullScreen = false;
	if(argc>1)
	{
		printf("Argv----%s\n",argv[1]);
		if(!strcmp(argv[1],"1"))
			IsFullScreen = true;
		else if(!strcmp(argv[1],"0"))
			IsFullScreen = false;
	}*/
//	if(argc>1)
//	{
//		printf("Argv----%s\n",argv[1]);
//		if(!strcmp(argv[1],"1"))
//			ECDirector::Singleton()->SetLanguage(English);
//		else if(!strcmp(argv[1],"2"))
//			ECDirector::Singleton()->SetLanguage(Chinese);
//	}
	ECDirector::Singleton()->SetLanguage(Chinese);
	ECDirector::Singleton()->InitApplication(EC_SCREEN_W,EC_SCREEN_H,20,false);
	ECNode* pInitScene = ECSplashScreen::CreateSplashScreenWithFile("Resources/LoadingImage.png");
	ECDirector::Singleton()->SetRunScene(pInitScene);
	ECDirector::Singleton()->StartLoop();
	return 0;
}
