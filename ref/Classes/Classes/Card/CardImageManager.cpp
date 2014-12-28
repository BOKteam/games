/*
 * CardImageManager.cpp
 *
 *  Created on: Oct 9, 2012
 *      Author: root
 */
#include "CardImageManager.h"
CardImageManager* CardImageManager::m_Interface = NULL;
CardImageManager* CardImageManager::Singleton()
{
	if(m_Interface==NULL)
	{
		m_Interface = new CardImageManager();
	}
	return m_Interface;
}
CardImageManager::CardImageManager()
{
	//m_pImageList

	InitNewImage("Resources/GameUI/back.png");
	InitNewImage("Resources/GameUI/Lord_L.png");
	InitNewImage("Resources/GameUI/Farmer_L.png");
	InitNewImage("Resources/GameUI/Pass_Back.png");
	InitNewImage("Resources/GameUI/PassLord_Back.png");
	InitNewImage("Resources/GameUI/Game_BtnDown_CallScore1.png");
	InitNewImage("Resources/GameUI/Game_BtnDown_CallScore2.png");
	InitNewImage("Resources/GameUI/Game_BtnDown_CallScore3.png");

	char path[64];
	for(int i=1;i<=54;i++)
	{
		sprintf(path,"Resources/GameUI/c%d.png",i);
		InitNewImage(path);
	}
	for(int i=1;i<=BOMB_EFFECT_COUNT;i++)
	{
		sprintf(path,"Resources/Effect/%d.png",i);
		InitNewImage(path);
	}
}
CardImageManager::~CardImageManager()
{
	Clear();
}
ECView* CardImageManager::GetView(const char* path)
{
	for(int i=0;i<m_pImageList.size();i++)
	{
		if(!strcmp(path,m_pImageList[i]->path))
		{
			//printf("GetView----%s\n",path);
			return m_pImageList[i]->pImage;
		}
	}
	return NULL;
}
void CardImageManager::InitNewImage(const char* path)
{
	View* Temp = new View();
	Temp->pImage = ECView::InitWithFile(path);
	strcpy(Temp->path,path);

	m_pImageList.push_back(Temp);
}
void CardImageManager::Clear()
{
	while(m_pImageList.size()>0)
	{
		delete m_pImageList[m_pImageList.size()-1]->pImage;
		delete m_pImageList[m_pImageList.size()-1];
		m_pImageList.pop_back();
	}
}

