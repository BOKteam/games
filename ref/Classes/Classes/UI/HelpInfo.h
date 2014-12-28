/*
 * HelpInfo.h
 *
 *  Created on: Jan 16, 2012
 *      Author: root
 */

#ifndef HELPINFO_H_
#define HELPINFO_H_

#include "EnveeCore.h"
#include "HomePageCursor.h"
using namespace NSEnveeCore;

class HelpInfo:public ECNode
{
public:
	void Init();
	HelpInfo();
	virtual ~HelpInfo();
    void SetScenceMove(int i);


    void BackMainMenu();
    void ShowPrePicture();
    void ShowNextPicture();
    void SetHelpPicNotVisual();
    void ShowImage();
    bool IsTheFisrtOrLastPic();

  //  HomePageCursor* pHomePageCursor;

private:
    ECButton* NextPic,*PrePic;
	ECView* HelpPic1,*HelpPic2,*HelpPic3,*HelpPic4;//,*HelpPic5;
    int HelpSenceCount;
    int m_nStats,m_IsNext,m_IsPre;
};


#endif /* HELPINFO_H_ */
