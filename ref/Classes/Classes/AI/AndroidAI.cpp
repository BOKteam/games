//#include "StdAfx.h"
#include "AndroidAI.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������AIʵ��
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CAndroidAI::CAndroidAI()
{
	memset(m_cbAllCardData,'\0',sizeof(m_cbAllCardData));
	memset(m_cbLandScoreCardData, '\0',sizeof(m_cbLandScoreCardData));
	memset(m_cbUserCardCount, '\0',sizeof(m_cbUserCardCount));
	m_wBankerUser = INVALID_CHAIR;
	m_lBankerOutCardCount = 0;
}

CAndroidAI::~CAndroidAI()
{
}

//��ȡ����
BYTE CAndroidAI::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{
	//������
	switch (cbCardCount)
	{
	case 0:	//����
		{
			return CT_ERROR;
		}
	case 1: //����
		{
			return CT_SINGLE;
		}
	case 2:	//���ƻ��
		{
			//�����ж�
			if ((cbCardData[0]==0x4F)&&(cbCardData[1]==0x4E)) return CT_MISSILE_CARD;
			if (GetCardLogicValue(cbCardData[0])==GetCardLogicValue(cbCardData[1])) return CT_DOUBLE;

			return CT_ERROR;
		}
	}

	//�����˿�
	tagAnalyseResult AnalyseResult;
	if(!AnalysebCardData(cbCardData,cbCardCount,AnalyseResult)) 
		return CT_ERROR ;

	//�����ж�
	if (AnalyseResult.cbFourCount>0)
	{
		//�����ж�
		if ((AnalyseResult.cbFourCount==1)&&(cbCardCount==4)) return CT_BOMB_CARD;
		//	if ((AnalyseResult.cbFourCount==1)&&(AnalyseResult.cbSignedCount==2)&&(cbCardCount==6)) return CT_FOUR_LINE_TAKE_ONE;
		if ((AnalyseResult.cbFourCount==1)&&(AnalyseResult.cbSignedCount==2)&&(cbCardCount==6)) return CT_FOUR_LINE_TAKE_ONE;
		if ((AnalyseResult.cbFourCount==1)&&(AnalyseResult.cbDoubleCount==2)&&(cbCardCount==8)) return CT_FOUR_LINE_TAKE_TWO;

		return CT_ERROR;
	}

	//�����ж�
	if (AnalyseResult.cbThreeCount>0)
	{
		//��������
		if(AnalyseResult.cbThreeCount==1 && cbCardCount==3) return CT_THREE ;

		//l���ж�
		if (AnalyseResult.cbThreeCount>1)
		{
			//������
			BYTE cbCardData=AnalyseResult.cbThreeCardData[0];
			BYTE cbFirstLogicValue=GetCardLogicValue(cbCardData);

			//�������
			if (cbFirstLogicValue>=15) return CT_ERROR;

			//l���ж�
			for (BYTE i=1;i<AnalyseResult.cbThreeCount;i++)
			{
				BYTE cbCardData=AnalyseResult.cbThreeCardData[i*3];
				if (cbFirstLogicValue!=(GetCardLogicValue(cbCardData)+i)) return CT_ERROR;
			}
		}

		//�����ж�
		if (AnalyseResult.cbThreeCount*3==cbCardCount) return CT_THREE_LINE;
		if (AnalyseResult.cbThreeCount*4==cbCardCount) return CT_THREE_LINE_TAKE_ONE;
		if ((AnalyseResult.cbThreeCount*5==cbCardCount)&&(AnalyseResult.cbDoubleCount==AnalyseResult.cbThreeCount)) return CT_THREE_LINE_TAKE_TWO;

		return CT_ERROR;
	}

	//}������
	if (AnalyseResult.cbDoubleCount>=3)
	{
		//������
		BYTE cbCardData=AnalyseResult.cbDoubleCardData[0];
		BYTE cbFirstLogicValue=GetCardLogicValue(cbCardData);

		//�������
		if (cbFirstLogicValue>=15) return CT_ERROR;

		//l���ж�
		for (BYTE i=1;i<AnalyseResult.cbDoubleCount;i++)
		{
			BYTE cbCardData=AnalyseResult.cbDoubleCardData[i*2];
			if (cbFirstLogicValue!=(GetCardLogicValue(cbCardData)+i)) return CT_ERROR;
		}

		//��l�ж�
		if ((AnalyseResult.cbDoubleCount*2)==cbCardCount) return CT_DOUBLE_LINE;

		return CT_ERROR;
	}

	//�����ж�
	if ((AnalyseResult.cbSignedCount>=5)&&(AnalyseResult.cbSignedCount==cbCardCount))
	{
		//������
		BYTE cbCardData=AnalyseResult.cbSignedCardData[0];
		BYTE cbFirstLogicValue=GetCardLogicValue(cbCardData);

		//�������
		if (cbFirstLogicValue>=15) return CT_ERROR;

		//l���ж�
		for (BYTE i=1;i<AnalyseResult.cbSignedCount;i++)
		{
			BYTE cbCardData=AnalyseResult.cbSignedCardData[i];
			if (cbFirstLogicValue!=(GetCardLogicValue(cbCardData)+i)) return CT_ERROR;
		}

		return CT_SINGLE_LINE;
	}

	return CT_ERROR;
}

//�߼���ֵ
BYTE CAndroidAI::GetCardLogicValue(BYTE cbCardData)
{
	//�˿�����
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

//#ifdef _DEBUG
	if(cbCardValue<=0 || cbCardValue>(MASK_VALUE&0x4f))
		return 0 ;
//#endif

	assert(cbCardValue>0 && cbCardValue<=(MASK_VALUE&0x4f));


	//ת����ֵ
	if (cbCardColor==0x40) return cbCardValue+2;
	return (cbCardValue<=2)?(cbCardValue+13):cbCardValue;
}

//��Ч�ж�
bool CAndroidAI::IsValidCard(BYTE cbCardData)
{
	//��ȡ����
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//��Ч�ж�
	if ((cbCardData==0x4E)||(cbCardData==0x4F)) return true;
	if ((cbCardColor<=0x30)&&(cbCardValue>=0x01)&&(cbCardValue<=0x0D)) return true;

	return false;
}

//�Ա��˿�
bool CAndroidAI::CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//��ȡ����
	BYTE cbNextType=GetCardType(cbNextCard,cbNextCount);
	BYTE cbFirstType=GetCardType(cbFirstCard,cbFirstCount);

	//�����ж�
	if (cbNextType==CT_ERROR) return false;
	if (cbNextType==CT_MISSILE_CARD) return true;
	if (cbFirstType==CT_MISSILE_CARD) return false ;

	//ը���ж�
	if ((cbFirstType!=CT_BOMB_CARD)&&(cbNextType==CT_BOMB_CARD)) return true;
	if ((cbFirstType==CT_BOMB_CARD)&&(cbNextType!=CT_BOMB_CARD)) return false;

	//�����ж�
	if ((cbFirstType!=cbNextType)||(cbFirstCount!=cbNextCount)) return false;

	//��ʼ�Ա�
	switch (cbNextType)
	{
	case CT_SINGLE:
	case CT_DOUBLE:
	case CT_THREE:
	case CT_SINGLE_LINE:
	case CT_DOUBLE_LINE:
	case CT_THREE_LINE:
	case CT_BOMB_CARD:
		{
			//��ȡ��ֵ
			BYTE cbNextLogicValue=GetCardLogicValue(cbNextCard[0]);
			BYTE cbFirstLogicValue=GetCardLogicValue(cbFirstCard[0]);

			//�Ա��˿�
			return cbNextLogicValue>cbFirstLogicValue;
		}
	case CT_THREE_LINE_TAKE_ONE:
	case CT_THREE_LINE_TAKE_TWO:
		{
			//�����˿�
			tagAnalyseResult NextResult;
			tagAnalyseResult FirstResult;
			AnalysebCardData(cbNextCard,cbNextCount,NextResult);
			AnalysebCardData(cbFirstCard,cbFirstCount,FirstResult);

			//��ȡ��ֵ
			BYTE cbNextLogicValue=GetCardLogicValue(NextResult.cbThreeCardData[0]);
			BYTE cbFirstLogicValue=GetCardLogicValue(FirstResult.cbThreeCardData[0]);

			//�Ա��˿�
			return cbNextLogicValue>cbFirstLogicValue;
		}
	case CT_FOUR_LINE_TAKE_ONE:
	case CT_FOUR_LINE_TAKE_TWO:
		{
			//�����˿�
			tagAnalyseResult NextResult;
			tagAnalyseResult FirstResult;
			AnalysebCardData(cbNextCard,cbNextCount,NextResult);
			AnalysebCardData(cbFirstCard,cbFirstCount,FirstResult);

			//��ȡ��ֵ
			BYTE cbNextLogicValue=GetCardLogicValue(NextResult.cbFourCardData[0]);
			BYTE cbFirstLogicValue=GetCardLogicValue(FirstResult.cbFourCardData[0]);

			//�Ա��˿�
			return cbNextLogicValue>cbFirstLogicValue;
		}
	}

	return false;
}

//������
void CAndroidAI::ClearAllCard()
{
	memset(m_cbAllCardData,'\0',sizeof(m_cbAllCardData));
	memset(m_cbLandScoreCardData, '\0',sizeof(m_cbLandScoreCardData));
	memset(m_cbUserCardCount, '\0',sizeof(m_cbUserCardCount));
	m_wBankerUser = INVALID_CHAIR;
	m_lBankerOutCardCount = 0;
}

//��������
bool CAndroidAI::SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, WORD wOutCardUser, WORD wMeChairID, tagOutCardResult & OutCardResult)
{
	//����ж�
	WORD wUndersideOfBanker = (m_wBankerUser+1)%GAME_PLAYER ;	//�����¼�
	WORD wUpsideOfBanker = (wUndersideOfBanker+1)%GAME_PLAYER ;	//�����ϼ�

	//��ʼ��
	//printf("OutCardResult Size --- %d\n",sizeof(OutCardResult));
	memset(&OutCardResult, '\0',sizeof(OutCardResult)) ;

	//�ȳ���
	if(cbTurnCardCount==0)
	{
		//�������
		if(wMeChairID==m_wBankerUser) BankerOutCard(cbHandCardData, cbHandCardCount, OutCardResult) ;
		//�����¼�
		else if(wMeChairID==wUndersideOfBanker) UndersideOfBankerOutCard(cbHandCardData, cbHandCardCount,wMeChairID,  OutCardResult) ;
		//�����ϼ�
		else if(wMeChairID==wUpsideOfBanker) UpsideOfBankerOutCard(cbHandCardData, cbHandCardCount, wMeChairID, OutCardResult) ;
		//���� ID
		else assert(false) ;
	}
	//ѹ��
	else
	{	
		//�������
		if(wMeChairID==m_wBankerUser) BankerOutCard(cbHandCardData, cbHandCardCount, wOutCardUser, cbTurnCardData, cbTurnCardCount, OutCardResult) ;
		//�����¼�
		else if(wMeChairID==wUndersideOfBanker) UndersideOfBankerOutCard(cbHandCardData, cbHandCardCount, wOutCardUser, cbTurnCardData, cbTurnCardCount, OutCardResult) ;
		//�����ϼ�
		else if(wMeChairID==wUpsideOfBanker) UpsideOfBankerOutCard(cbHandCardData, cbHandCardCount, wOutCardUser, cbTurnCardData, cbTurnCardCount, OutCardResult) ;
		//���� ID
		else assert(false) ;

		//�������ж�
		if (GetCardType(m_cbAllCardData[wOutCardUser], m_cbUserCardCount[wOutCardUser]) != CT_ERROR && m_lBankerOutCardCount == 1 && (OutCardResult.cbCardCount <= 0 || 
			!CompareCard(cbTurnCardData, OutCardResult.cbResultCard , cbTurnCardCount , OutCardResult.cbCardCount)))
		{
			//���±�û��
			tagOutCardTypeResult CardTypeResult[12+1] ;
			memset(CardTypeResult,'\0', sizeof(CardTypeResult)) ;

			//��������
			AnalyseOutCardType(cbHandCardData,cbHandCardCount,cbTurnCardData,cbTurnCardCount, CardTypeResult) ;

			BYTE cbOutCardType = GetCardType(cbTurnCardData, cbTurnCardCount) ;

			if (CardTypeResult[cbOutCardType].cbCardTypeCount > 0 && cbOutCardType != CT_THREE)
			{
				BYTE cbIndex = CardTypeResult[cbOutCardType].cbCardTypeCount - 1 ;
				BYTE cbCardCount = CardTypeResult[cbOutCardType].cbEachHandCardCount[cbIndex] ;

				memcpy(OutCardResult.cbResultCard, CardTypeResult[cbOutCardType].cbCardData[cbIndex], cbCardCount) ;
				OutCardResult.cbCardCount = cbCardCount ;
			}
			//��ը��
			else if (CardTypeResult[CT_BOMB_CARD].cbCardTypeCount > 0)
			{
				BYTE cbIndex = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount - 1 ;
				BYTE cbCardCount = CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[cbIndex] ;
				memcpy(OutCardResult.cbResultCard, CardTypeResult[CT_BOMB_CARD].cbCardData[cbIndex], cbCardCount) ;
				OutCardResult.cbCardCount = cbCardCount ;
			}
			else if (CardTypeResult[CT_MISSILE_CARD].cbCardTypeCount > 0)
			{
				OutCardResult.cbCardCount = 2 ;
				OutCardResult.cbResultCard[0] = 0x4F ;
				OutCardResult.cbResultCard[1] = 0x4E ;
			}
		}

		//�����ж�
		if (GetCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser]) != CT_ERROR && (OutCardResult.cbCardCount <= 0 || 
			!CompareCard(cbTurnCardData, OutCardResult.cbResultCard , cbTurnCardCount , OutCardResult.cbCardCount)) &&
			m_cbUserCardCount[(1 + m_wBankerUser) % GAME_PLAYER] == NORMAL_COUNT && m_cbUserCardCount[(2 + m_wBankerUser) % GAME_PLAYER] == NORMAL_COUNT)
		{
			//���±�û��
			tagOutCardTypeResult CardTypeResult[12+1] ;
			memset(CardTypeResult, '\0',sizeof(CardTypeResult)) ;

			//��������
			AnalyseOutCardType(cbHandCardData,cbHandCardCount,cbTurnCardData,cbTurnCardCount, CardTypeResult) ;

			BYTE cbOutCardType = GetCardType(cbTurnCardData, cbTurnCardCount) ;

			if (CardTypeResult[cbOutCardType].cbCardTypeCount > 0 && cbOutCardType != CT_THREE)
			{
				BYTE cbIndex = CardTypeResult[cbOutCardType].cbCardTypeCount - 1 ;
				BYTE cbCardCount = CardTypeResult[cbOutCardType].cbEachHandCardCount[cbIndex] ;

				memcpy(OutCardResult.cbResultCard, CardTypeResult[cbOutCardType].cbCardData[cbIndex], cbCardCount) ;
				OutCardResult.cbCardCount = cbCardCount ;
			}
			//��ը��
			else if (CardTypeResult[CT_BOMB_CARD].cbCardTypeCount > 0)
			{
				BYTE cbIndex = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount - 1 ;
				BYTE cbCardCount = CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[cbIndex] ;
				memcpy(OutCardResult.cbResultCard, CardTypeResult[CT_BOMB_CARD].cbCardData[cbIndex], cbCardCount) ;
				OutCardResult.cbCardCount = cbCardCount ;
			}
			else if (CardTypeResult[CT_MISSILE_CARD].cbCardTypeCount > 0)
			{
				OutCardResult.cbCardCount = 2 ;
				OutCardResult.cbResultCard[0] = 0x4F ;
				OutCardResult.cbResultCard[1] = 0x4E ;
			}
		}

	}

	//���ƴ���
	if (m_wBankerUser == wMeChairID && OutCardResult.cbCardCount > 0) ++m_lBankerOutCardCount ;

	return true ;
}

//����ը��
void CAndroidAI::GetAllBomCard(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbBomCardData[], BYTE &cbBomCardCount)
{
	BYTE cbTmpCardData[MAX_COUNT] ;
	memcpy(cbTmpCardData, cbHandCardData, cbHandCardCount) ;

	//��С����
	SortCardList(cbTmpCardData, cbHandCardCount, ST_ORDER);

	cbBomCardCount = 0 ;

	if(cbHandCardCount<2) return ;

	//˫��ը��
	if(0x4F==cbTmpCardData[0] && 0x4E==cbTmpCardData[1])
	{
		cbBomCardData[cbBomCardCount++] = cbTmpCardData[0] ;
		cbBomCardData[cbBomCardCount++] = cbTmpCardData[1] ;
	}

	//�˿˷���
	for (BYTE i=0;i<cbHandCardCount;i++)
	{
		//������
		BYTE cbSameCount=1;
		BYTE cbLogicValue=GetCardLogicValue(cbTmpCardData[i]);

		//����ͬ��
		for (BYTE j=i+1;j<cbHandCardCount;j++)
		{
			//��ȡ�˿�
			if (GetCardLogicValue(cbTmpCardData[j])!=cbLogicValue) break;

			//���ñ�
			cbSameCount++;
		}

		if(4==cbSameCount)
		{
			cbBomCardData[cbBomCardCount++] = cbTmpCardData[i] ;
			cbBomCardData[cbBomCardCount++] = cbTmpCardData[i+1] ;
			cbBomCardData[cbBomCardCount++] = cbTmpCardData[i+2] ;
			cbBomCardData[cbBomCardCount++] = cbTmpCardData[i+3] ;
		}

		//��������
		i+=cbSameCount-1;
	}
}

//����˳��
void CAndroidAI::GetAllLineCard(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbLineCardData[], BYTE &cbLineCardCount)
{
	BYTE cbTmpCard[MAX_COUNT] ;
	memcpy(cbTmpCard, cbHandCardData, cbHandCardCount) ;
	//��С����
	SortCardList(cbTmpCard, cbHandCardCount, ST_ORDER) ;

	cbLineCardCount = 0 ;

	//���У��
	if(cbHandCardCount<5) return ;

	BYTE cbFirstCard = 0 ;
	//ȥ��2����
	for(BYTE i=0 ; i<cbHandCardCount ; ++i)	if(GetCardLogicValue(cbTmpCard[i])<15)	{cbFirstCard = i ; break ;}

	BYTE cbSingleLineCard[12] ;
	BYTE cbSingleLineCount=0 ;
	BYTE cbLeftCardCount = cbHandCardCount ;
	bool bFindSingleLine = true ;

	//l���ж�
	while (cbLeftCardCount>=5 && bFindSingleLine)
	{
		cbSingleLineCount=1 ;
		bFindSingleLine = false ;
		BYTE cbLastCard = cbTmpCard[cbFirstCard] ;
		cbSingleLineCard[cbSingleLineCount-1] = cbTmpCard[cbFirstCard] ;
		for (BYTE i=cbFirstCard+1; i<cbLeftCardCount; i++)
		{
			BYTE cbCardData=cbTmpCard[i];

			//l���ж�
			if (1!=(GetCardLogicValue(cbLastCard)-GetCardLogicValue(cbCardData)) && GetCardValue(cbLastCard)!=GetCardValue(cbCardData)) 
			{
				cbLastCard = cbTmpCard[i] ;
				if(cbSingleLineCount<5) 
				{
					cbSingleLineCount = 1 ;
					cbSingleLineCard[cbSingleLineCount-1] = cbTmpCard[i] ;
					continue ;
				}
				else break ;
			}
			//ͬ���ж�
			else if(GetCardValue(cbLastCard)!=GetCardValue(cbCardData))
			{
				cbLastCard = cbCardData ;
				cbSingleLineCard[cbSingleLineCount] = cbCardData ;
				++cbSingleLineCount ;
			}					
		}

		//�������
		if(cbSingleLineCount>=5)
		{
			RemoveCard(cbSingleLineCard, cbSingleLineCount, cbTmpCard, cbLeftCardCount) ;
			memcpy(cbLineCardData+cbLineCardCount , cbSingleLineCard, sizeof(BYTE)*cbSingleLineCount) ;
			cbLineCardCount += cbSingleLineCount ;
			cbLeftCardCount -= cbSingleLineCount ;
			bFindSingleLine = true ;
		}
	}
}

//��������
void CAndroidAI::GetAllThreeCard(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbThreeCardData[], BYTE &cbThreeCardCount)
{
	BYTE cbTmpCardData[MAX_COUNT] ;
	memcpy(cbTmpCardData, cbHandCardData, cbHandCardCount) ;

	//��С����
	SortCardList(cbTmpCardData, cbHandCardCount, ST_ORDER);

	cbThreeCardCount = 0 ;

	//�˿˷���
	for (BYTE i=0;i<cbHandCardCount;i++)
	{
		//������
		BYTE cbSameCount=1;
		BYTE cbLogicValue=GetCardLogicValue(cbTmpCardData[i]);

		//����ͬ��
		for (BYTE j=i+1;j<cbHandCardCount;j++)
		{
			//��ȡ�˿�
			if (GetCardLogicValue(cbTmpCardData[j])!=cbLogicValue) break;

			//���ñ�
			cbSameCount++;
		}

		if(cbSameCount>=3)
		{
			cbThreeCardData[cbThreeCardCount++] = cbTmpCardData[i] ;
			cbThreeCardData[cbThreeCardCount++] = cbTmpCardData[i+1] ;
			cbThreeCardData[cbThreeCardCount++] = cbTmpCardData[i+2] ;	
		}

		//��������
		i+=cbSameCount-1;
	}
}

//�������
void CAndroidAI::GetAllDoubleCard(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbDoubleCardData[], BYTE &cbDoubleCardCount)
{
	BYTE cbTmpCardData[MAX_COUNT] ;
	memcpy(cbTmpCardData, cbHandCardData, cbHandCardCount) ;

	//��С����
	SortCardList(cbTmpCardData, cbHandCardCount, ST_ORDER);

	cbDoubleCardCount = 0 ;

	//�˿˷���
	for (BYTE i=0;i<cbHandCardCount;i++)
	{
		//������
		BYTE cbSameCount=1;
		BYTE cbLogicValue=GetCardLogicValue(cbTmpCardData[i]);

		//����ͬ��
		for (BYTE j=i+1;j<cbHandCardCount;j++)
		{
			//��ȡ�˿�
			if (GetCardLogicValue(cbTmpCardData[j])!=cbLogicValue) break;

			//���ñ�
			cbSameCount++;
		}

		if(cbSameCount>=2)
		{
			cbDoubleCardData[cbDoubleCardCount++] = cbTmpCardData[i] ;
			cbDoubleCardData[cbDoubleCardCount++] = cbTmpCardData[i+1] ;
		}

		//��������
		i+=cbSameCount-1;
	}
}

//������
void CAndroidAI::GetAllSingleCard(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbSingleCardData[], BYTE &cbSingleCardCount)
{
	cbSingleCardCount =0 ;

	BYTE cbTmpCardData[MAX_COUNT] ;
	memcpy(cbTmpCardData, cbHandCardData, cbHandCardCount) ;

	//��С����
	SortCardList(cbTmpCardData, cbHandCardCount, ST_ORDER);

	//�˿˷���
	for (BYTE i=0;i<cbHandCardCount;i++)
	{
		//������
		BYTE cbSameCount=1;
		BYTE cbLogicValue=GetCardLogicValue(cbTmpCardData[i]);

		//����ͬ��
		for (BYTE j=i+1;j<cbHandCardCount;j++)
		{
			//��ȡ�˿�
			if (GetCardLogicValue(cbTmpCardData[j])!=cbLogicValue) break;

			//���ñ�
			cbSameCount++;
		}

		if(cbSameCount==1)
		{
			cbSingleCardData[cbSingleCardCount++] = cbTmpCardData[i] ;
		}

		//��������
		i+=cbSameCount-1;
	}
}

//�������
void CAndroidAI::AnalyseOutCardType(BYTE const cbHandCardData[], BYTE const cbHandCardCount, tagOutCardTypeResult CardTypeResult[12+1])
{
	/*printf("AnalyseOut\n");
	for(int i=0;i<cbHandCardCount;i++)
	{
		printf("%d ",cbHandCardData[i]);
	}
	printf("\n");*/
	memset(CardTypeResult,'\0', sizeof(CardTypeResult[0])*12) ;
	BYTE cbTmpCardData[MAX_COUNT] ;
	//�����˿ˣ���ֹ����ʱ�ı��˿�
	BYTE cbReserveCardData[MAX_COUNT] ;
	memcpy(cbReserveCardData, cbHandCardData, cbHandCardCount) ;
	SortCardList(cbReserveCardData, cbHandCardCount, ST_ORDER) ;
	memcpy(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

	//��������
	for(BYTE i=0; i<cbHandCardCount; ++i)
	{
		BYTE Index = CardTypeResult[CT_SINGLE].cbCardTypeCount ;
		CardTypeResult[CT_SINGLE].cbCardType = CT_SINGLE ;
		CardTypeResult[CT_SINGLE].cbCardData[Index][0] = cbTmpCardData[i] ;
		CardTypeResult[CT_SINGLE].cbEachHandCardCount[Index] = 1 ;
		CardTypeResult[CT_SINGLE].cbCardTypeCount++ ;	

		assert(CardTypeResult[CT_SINGLE].cbCardTypeCount<MAX_TYPE_COUNT) ;
	}

	//��������
	{
		BYTE cbDoubleCardData[MAX_COUNT] ;
		BYTE cbDoubleCardcount=0; 
		GetAllDoubleCard(cbTmpCardData, cbHandCardCount, cbDoubleCardData, cbDoubleCardcount) ;
		for(BYTE i=0; i<cbDoubleCardcount; i+=2)
		{
			BYTE Index = CardTypeResult[CT_DOUBLE].cbCardTypeCount ;
			CardTypeResult[CT_DOUBLE].cbCardType = CT_DOUBLE ;
			CardTypeResult[CT_DOUBLE].cbCardData[Index][0] = cbDoubleCardData[i] ;
			CardTypeResult[CT_DOUBLE].cbCardData[Index][1] = cbDoubleCardData[i+1] ;
			CardTypeResult[CT_DOUBLE].cbEachHandCardCount[Index] = 2 ;
			CardTypeResult[CT_DOUBLE].cbCardTypeCount++ ;	

			assert(CardTypeResult[CT_DOUBLE].cbCardTypeCount<MAX_TYPE_COUNT) ;
		}
	}

	//��������
	{
		BYTE cbThreeCardData[MAX_COUNT];
		BYTE cbThreeCardCount=0 ;
		GetAllThreeCard(cbTmpCardData, cbHandCardCount, cbThreeCardData, cbThreeCardCount) ;
		for(BYTE i=0; i<cbThreeCardCount; i+=3)
		{
			BYTE Index = CardTypeResult[CT_THREE].cbCardTypeCount ;
			CardTypeResult[CT_THREE].cbCardType = CT_THREE ;
			CardTypeResult[CT_THREE].cbCardData[Index][0] = cbThreeCardData[i] ;
			CardTypeResult[CT_THREE].cbCardData[Index][1] = cbThreeCardData[i+1] ;
			CardTypeResult[CT_THREE].cbCardData[Index][2] = cbThreeCardData[i+2] ;
			CardTypeResult[CT_THREE].cbEachHandCardCount[Index] = 3 ;
			CardTypeResult[CT_THREE].cbCardTypeCount++ ;	

			assert(CardTypeResult[CT_THREE].cbCardTypeCount<MAX_TYPE_COUNT) ;
		}
	}

	//ը������
	{
		BYTE cbFourCardData[MAX_COUNT];
		BYTE cbFourCardCount=0 ;
		if(cbHandCardCount>=2 && 0x4F==cbTmpCardData[0] && 0x4E==cbTmpCardData[1])
		{
			BYTE Index = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount ;
			CardTypeResult[CT_BOMB_CARD].cbCardType = CT_BOMB_CARD ;
			CardTypeResult[CT_BOMB_CARD].cbCardData[Index][0] = cbTmpCardData[0] ;
			CardTypeResult[CT_BOMB_CARD].cbCardData[Index][1] = cbTmpCardData[1] ;
			CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index] = 2 ;
			CardTypeResult[CT_BOMB_CARD].cbCardTypeCount++ ;	
			GetAllBomCard(cbTmpCardData+2, cbHandCardCount-2, cbFourCardData, cbFourCardCount) ;
		}
		else GetAllBomCard(cbTmpCardData, cbHandCardCount, cbFourCardData, cbFourCardCount) ;
		for (BYTE i=0; i<cbFourCardCount; i+=4)
		{
			BYTE Index = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount ;
			CardTypeResult[CT_BOMB_CARD].cbCardType = CT_BOMB_CARD ;
			CardTypeResult[CT_BOMB_CARD].cbCardData[Index][0] = cbFourCardData[i] ;
			CardTypeResult[CT_BOMB_CARD].cbCardData[Index][1] = cbFourCardData[i+1] ;
			CardTypeResult[CT_BOMB_CARD].cbCardData[Index][2] = cbFourCardData[i+2] ;
			CardTypeResult[CT_BOMB_CARD].cbCardData[Index][3] = cbFourCardData[i+3] ;
			CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index] = 4 ;
			CardTypeResult[CT_BOMB_CARD].cbCardTypeCount++ ;	

			assert(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount<MAX_TYPE_COUNT) ;
		}
	}
	//��l����
	{
		//�ָ��˿ˣ���ֹ����ʱ�ı��˿�
		memcpy(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

		BYTE cbFirstCard = 0 ;
		//ȥ��2����
		for(BYTE i=0 ; i<cbHandCardCount ; ++i)
		{
			if(GetCardLogicValue(cbTmpCardData[i])<15)
			{
				cbFirstCard = i ;
				break ;
			}
		}

		BYTE cbSingleLineCard[12] ;
		BYTE cbSingleLineCount=1 ;
		BYTE cbLeftCardCount = cbHandCardCount ;
		bool bFindSingleLine = true ;

		//l���ж�
		while (cbLeftCardCount>=5 && bFindSingleLine)
		{
			cbSingleLineCount=1 ;
			bFindSingleLine = false ;
			BYTE cbLastCard = cbTmpCardData[cbFirstCard] ;
			cbSingleLineCard[cbSingleLineCount-1] = cbTmpCardData[cbFirstCard] ;
			for (BYTE i=cbFirstCard+1; i<cbLeftCardCount; i++)
			{
				BYTE cbCardData=cbTmpCardData[i];

				//l���ж�
				if (1!=(GetCardLogicValue(cbLastCard)-GetCardLogicValue(cbCardData)) && GetCardValue(cbLastCard)!=GetCardValue(cbCardData)) 
				{
					cbLastCard = cbTmpCardData[i] ;
					//�Ƿ�Ϸ�
					if(cbSingleLineCount<5) 
					{
						cbSingleLineCount = 1 ;
						cbSingleLineCard[cbSingleLineCount-1] = cbTmpCardData[i] ;
						continue ;
					}
					else break ;
				}
				//ͬ���ж�
				else if(GetCardValue(cbLastCard)!=GetCardValue(cbCardData))
				{
					cbLastCard = cbCardData ;
					cbSingleLineCard[cbSingleLineCount] = cbCardData ;
					++cbSingleLineCount ;
				}
			}

			//�������
			if(cbSingleLineCount>=5)
			{
				BYTE Index ;
				//����l��

				int nStart = 0 ;
				
				//�Ӵ�С
				nStart = cbSingleLineCount - 5 ;
				while ( 0 < nStart )
				{
					Index = CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount ;
					BYTE cbThisLineCount = cbSingleLineCount-nStart ;
					CardTypeResult[CT_SINGLE_LINE].cbCardType = CT_SINGLE_LINE ;
					memcpy(CardTypeResult[CT_SINGLE_LINE].cbCardData[Index], cbSingleLineCard, sizeof(BYTE)*(cbThisLineCount));
					CardTypeResult[CT_SINGLE_LINE].cbEachHandCardCount[Index] = cbThisLineCount;
					CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount++ ;

					assert(CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount<MAX_TYPE_COUNT) ;
					nStart-- ;
				}

				//��С����
				nStart = cbSingleLineCount - 5;
				while ( 0 <= nStart )
				{
					Index = CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount ;
					BYTE cbThisLineCount = cbSingleLineCount-nStart ;
					CardTypeResult[CT_SINGLE_LINE].cbCardType = CT_SINGLE_LINE ;
					memcpy(CardTypeResult[CT_SINGLE_LINE].cbCardData[Index], cbSingleLineCard+nStart, sizeof(BYTE)*(cbThisLineCount));
					CardTypeResult[CT_SINGLE_LINE].cbEachHandCardCount[Index] = cbThisLineCount;
					CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount++ ;

					assert(CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount<MAX_TYPE_COUNT) ;
					nStart-- ;
				}

				RemoveCard(cbSingleLineCard, cbSingleLineCount, cbTmpCardData, cbLeftCardCount) ;
				cbLeftCardCount -= cbSingleLineCount ;
				bFindSingleLine = true ;
			}
		}

	}

	//��l����
	{
		//�ָ��˿ˣ���ֹ����ʱ�ı��˿�
		memcpy(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

		//l���ж�
		BYTE cbFirstCard = 0 ;
		//ȥ��2����
		for(BYTE i=0 ; i<cbHandCardCount ; ++i)	if(GetCardLogicValue(cbTmpCardData[i])<15)	{cbFirstCard = i ; break ;}

		BYTE cbLeftCardCount = cbHandCardCount-cbFirstCard ;
		bool bFindDoubleLine = true ;
		BYTE cbDoubleLineCount = 0 ;
		BYTE cbDoubleLineCard[24] ;
		//��ʼ�ж�
		while (cbLeftCardCount>=6 && bFindDoubleLine)
		{
			BYTE cbLastCard = cbTmpCardData[cbFirstCard] ;
			BYTE cbSameCount = 1 ;
			cbDoubleLineCount = 0 ;
			bFindDoubleLine=false ;
			for(BYTE i=cbFirstCard+1 ; i<cbLeftCardCount+cbFirstCard ; ++i)
			{
				//����ͬ��
				while ((i<(cbLeftCardCount+cbFirstCard))&&GetCardLogicValue(cbLastCard)==GetCardLogicValue(cbTmpCardData[i]))
				{
					++cbSameCount;
					++i ;
				}

				BYTE cbLastDoubleCardValue ;
				if(cbDoubleLineCount>0) cbLastDoubleCardValue = GetCardLogicValue(cbDoubleLineCard[cbDoubleLineCount-1]) ;
				//���¿�ʼ
				if((cbSameCount<2 || (cbDoubleLineCount>0 && (cbLastDoubleCardValue-GetCardLogicValue(cbLastCard))!=1)) && i<=cbLeftCardCount+cbFirstCard)
				{
					if(cbDoubleLineCount>=6) break ;
					//����
					if(cbSameCount>=2) i-=cbSameCount ;
					cbLastCard = cbTmpCardData[i] ;
					cbDoubleLineCount = 0 ;
				}
				//�������
				else if(cbSameCount>=2)
				{
					cbDoubleLineCard[cbDoubleLineCount] = cbTmpCardData[i-cbSameCount] ;
					cbDoubleLineCard[cbDoubleLineCount+1] = cbTmpCardData[i-cbSameCount+1] ;
					cbDoubleLineCount += 2 ;

					//��β�ж�
					if(i==(cbLeftCardCount+cbFirstCard-2))
						if((GetCardLogicValue(cbLastCard)-GetCardLogicValue(cbTmpCardData[i]))==1 && (GetCardLogicValue(cbTmpCardData[i])==GetCardLogicValue(cbTmpCardData[i+1])))
						{
							cbDoubleLineCard[cbDoubleLineCount] = cbTmpCardData[i] ;
							cbDoubleLineCard[cbDoubleLineCount+1] = cbTmpCardData[i+1] ;
							cbDoubleLineCount += 2 ;
							break ;
						}

				}

				cbLastCard = cbTmpCardData[i] ;
				cbSameCount = 1 ;
			}

			//�������
			if(cbDoubleLineCount>=6)
			{
				BYTE Index ;

				//����l��
				BYTE cbCurrentDoubleLineCount = 6 ;
				while ( cbCurrentDoubleLineCount < cbDoubleLineCount )
				{
					Index = CardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount ;
					CardTypeResult[CT_DOUBLE_LINE].cbCardType = CT_DOUBLE_LINE ;
					memcpy(CardTypeResult[CT_DOUBLE_LINE].cbCardData[Index], cbDoubleLineCard, sizeof(BYTE)*cbCurrentDoubleLineCount);
					CardTypeResult[CT_DOUBLE_LINE].cbEachHandCardCount[Index] = cbCurrentDoubleLineCount;
					CardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount++ ;

					cbCurrentDoubleLineCount += 2 ;

					assert(CardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount<MAX_TYPE_COUNT) ;
				}

				//��С����
				if ( cbDoubleLineCount >= 6 )
				{
					//����l��
					int cbLeftLen = cbDoubleLineCount - 6 ;
					while ( cbLeftLen >= 0 )
					{
						Index = CardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount ;
						CardTypeResult[CT_DOUBLE_LINE].cbCardType = CT_DOUBLE_LINE ;
						memcpy(CardTypeResult[CT_DOUBLE_LINE].cbCardData[Index], cbDoubleLineCard + cbLeftLen, sizeof( BYTE ) * ( cbDoubleLineCount - cbLeftLen ));
						CardTypeResult[CT_DOUBLE_LINE].cbEachHandCardCount[Index] = cbDoubleLineCount - cbLeftLen;
						CardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount++ ;

						cbLeftLen -= 2 ;

						assert(CardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount<MAX_TYPE_COUNT) ;
					}
				}

				RemoveCard(cbDoubleLineCard, cbDoubleLineCount, cbTmpCardData, cbFirstCard+cbLeftCardCount) ;				
				bFindDoubleLine=true ;
				cbLeftCardCount -= cbDoubleLineCount ;
			}
		}
	}

	//��l����
	{
		//�ָ��˿ˣ���ֹ����ʱ�ı��˿�
		memcpy(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

		//l���ж�
		BYTE cbFirstCard = 0 ;
		//ȥ��2����
		for(BYTE i=0 ; i<cbHandCardCount ; ++i)	if(GetCardLogicValue(cbTmpCardData[i])<15)	{cbFirstCard = i ; break ;}

		BYTE cbLeftCardCount = cbHandCardCount-cbFirstCard ;
		bool bFindThreeLine = true ;
		BYTE cbThreeLineCount = 0 ;
		BYTE cbThreeLineCard[20] ;
		//��ʼ�ж�
		while (cbLeftCardCount>=6 && bFindThreeLine)
		{
			BYTE cbLastCard = cbTmpCardData[cbFirstCard] ;
			BYTE cbSameCount = 1 ;
			cbThreeLineCount = 0 ;
			bFindThreeLine = false ;
			for(BYTE i=(cbFirstCard+1) ; i<(cbLeftCardCount+cbFirstCard) ; ++i)
			{
				//����ͬ��
				//printf("Current I +%d\n",i);
				while ((i<(cbLeftCardCount+cbFirstCard))&&GetCardLogicValue(cbLastCard)==GetCardLogicValue(cbTmpCardData[i]))
				{
					++cbSameCount;
					++i;
				}

				BYTE cbLastThreeCardValue ;
				if(cbThreeLineCount>0) cbLastThreeCardValue = GetCardLogicValue(cbThreeLineCard[cbThreeLineCount-1]) ;

				//���¿�ʼ
				if((cbSameCount<3 || (cbThreeLineCount>0&&(cbLastThreeCardValue-GetCardLogicValue(cbLastCard))!=1)) && i<=cbLeftCardCount+cbFirstCard)
				{
					if(cbThreeLineCount>=6) break ;

					if(cbSameCount>=3) i-=cbSameCount ;
					cbLastCard = cbTmpCardData[i] ;
					cbThreeLineCount = 0 ;
				}
				//�������
				else if(cbSameCount>=3)
				{
					cbThreeLineCard[cbThreeLineCount] = cbTmpCardData[i-cbSameCount] ;
					cbThreeLineCard[cbThreeLineCount+1] = cbTmpCardData[i-cbSameCount+1] ;
					cbThreeLineCard[cbThreeLineCount+2] = cbTmpCardData[i-cbSameCount+2] ;
					cbThreeLineCount += 3 ;

					//��β�ж�
					if(i==(cbLeftCardCount+cbFirstCard-3))
						if((GetCardLogicValue(cbLastCard)-GetCardLogicValue(cbTmpCardData[i]))==1 && (GetCardLogicValue(cbTmpCardData[i])==GetCardLogicValue(cbTmpCardData[i+1])) && (GetCardLogicValue(cbTmpCardData[i])==GetCardLogicValue(cbTmpCardData[i+2])))
						{
							cbThreeLineCard[cbThreeLineCount] = cbTmpCardData[i] ;
							cbThreeLineCard[cbThreeLineCount+1] = cbTmpCardData[i+1] ;
							cbThreeLineCard[cbThreeLineCount+2] = cbTmpCardData[i+2] ;
							cbThreeLineCount += 3 ;
							break ;
						}

				}

				cbLastCard = cbTmpCardData[i] ;
				cbSameCount = 1 ;
			}

			//�������
			if(cbThreeLineCount>=6)
			{
				BYTE Index ;

				Index = CardTypeResult[CT_THREE_LINE].cbCardTypeCount ;
				CardTypeResult[CT_THREE_LINE].cbCardType = CT_THREE_LINE ;
				memcpy(CardTypeResult[CT_THREE_LINE].cbCardData[Index], cbThreeLineCard, sizeof(BYTE)*cbThreeLineCount);
				CardTypeResult[CT_THREE_LINE].cbEachHandCardCount[Index] = cbThreeLineCount;
				CardTypeResult[CT_THREE_LINE].cbCardTypeCount++ ;

				assert(CardTypeResult[CT_THREE_LINE].cbCardTypeCount<MAX_TYPE_COUNT) ;

				RemoveCard(cbThreeLineCard, cbThreeLineCount, cbTmpCardData, cbFirstCard+cbLeftCardCount) ;
				bFindThreeLine=true ;
				cbLeftCardCount -= cbThreeLineCount ;
			}
		}

	}
	//���һ��
	{
		//�ָ��˿ˣ���ֹ����ʱ�ı��˿�
		memcpy(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

		BYTE cbHandThreeCard[MAX_COUNT];
		BYTE cbHandThreeCount=0 ;

		//�Ƴ�ը��
		BYTE cbAllBomCardData[MAX_COUNT] ;
		BYTE cbAllBomCardCount=0 ;
		GetAllBomCard(cbTmpCardData, cbHandCardCount, cbAllBomCardData, cbAllBomCardCount) ;
		RemoveCard(cbAllBomCardData, cbAllBomCardCount, cbTmpCardData, cbHandCardCount) ;

		GetAllThreeCard(cbTmpCardData, cbHandCardCount-cbAllBomCardCount, cbHandThreeCard, cbHandThreeCount) ;

		{
			BYTE Index ;
			//ȥ������
			BYTE cbRemainCardData[MAX_COUNT] ;
			memcpy(cbRemainCardData, cbTmpCardData, cbHandCardCount-cbAllBomCardCount) ;
			BYTE cbRemainCardCount=cbHandCardCount-cbAllBomCardCount-cbHandThreeCount ;
			RemoveCard(cbHandThreeCard, cbHandThreeCount, cbRemainCardData, cbHandCardCount-cbAllBomCardCount) ;
			//�����һ��
			for(BYTE i=0; i<cbHandThreeCount; i+=3)
			{
				//�����һ��
				for(BYTE j=0; j<cbRemainCardCount; ++j)
				{
					Index = CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardTypeCount ;
					CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardType = CT_THREE_LINE_TAKE_ONE ;
					CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardData[Index][0] = cbHandThreeCard[i] ;
					CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardData[Index][1] = cbHandThreeCard[i+1] ;
					CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardData[Index][2] = cbHandThreeCard[i+2] ;
					CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardData[Index][3] = cbRemainCardData[j] ;
					CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbEachHandCardCount[Index] = 4 ;
					CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardTypeCount++ ;
				}			
			}
		}

		//��l��
		BYTE cbLeftThreeCardCount=cbHandThreeCount ;
		bool bFindThreeLine=true ;
		BYTE cbLastIndex=0 ;
		if(GetCardLogicValue(cbHandThreeCard[0])==15) cbLastIndex=3 ;
		while ((cbLeftThreeCardCount>=6) && bFindThreeLine)
		{
			BYTE cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[cbLastIndex]);
			BYTE cbThreeLineCard[MAX_COUNT];
			BYTE cbThreeLineCardCount=3;
			cbThreeLineCard[0]=cbHandThreeCard[cbLastIndex];
			cbThreeLineCard[1]=cbHandThreeCard[cbLastIndex+1];
			cbThreeLineCard[2]=cbHandThreeCard[cbLastIndex+2];

			bFindThreeLine = false ;
			for(BYTE j=3+cbLastIndex; j<cbLeftThreeCardCount; j+=3)
			{
				//l���ж�
				if(1!=(cbLastLogicCard-(GetCardLogicValue(cbHandThreeCard[j]))))
				{
					cbLastIndex = j ;
					if(cbLeftThreeCardCount-j>=6) bFindThreeLine = true ;

					break;
				}

				cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[j]);
				cbThreeLineCard[cbThreeLineCardCount]=cbHandThreeCard[j];
				cbThreeLineCard[cbThreeLineCardCount+1]=cbHandThreeCard[j+1];
				cbThreeLineCard[cbThreeLineCardCount+2]=cbHandThreeCard[j+2];
				cbThreeLineCardCount += 3;
			}
			if(cbThreeLineCardCount>3)
			{
				BYTE Index ;

				BYTE cbRemainCard[MAX_COUNT];
				BYTE cbRemainCardCount=cbHandCardCount-cbAllBomCardCount-cbHandThreeCount ;


				//�Ƴ������Ӧ���Ƴ�ը����ȣ�
				memcpy(cbRemainCard, cbTmpCardData, (cbHandCardCount-cbAllBomCardCount)*sizeof(BYTE));
				RemoveCard(cbHandThreeCard, cbHandThreeCount, cbRemainCard, cbHandCardCount-cbAllBomCardCount) ;

				for(BYTE start=0; start<cbThreeLineCardCount-3; start+=3)
				{
					//��˳��Ŀ
					BYTE cbThisTreeLineCardCount = cbThreeLineCardCount-start ;
					//���Ƹ���
					BYTE cbSingleCardCount=(cbThisTreeLineCardCount)/3;

					//���Ʋ���
					if(cbRemainCardCount<cbSingleCardCount) continue ;

					//�������
					BYTE cbComCard[5];
					BYTE cbComResCard[254][5] ;
					BYTE cbComResLen=0 ;

					Combination(cbComCard, 0, cbComResCard, cbComResLen, cbRemainCard, cbSingleCardCount, cbRemainCardCount, cbSingleCardCount);
					for(BYTE i=0; i<cbComResLen; ++i)
					{
						Index = CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardTypeCount ;
						CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardType = CT_THREE_LINE_TAKE_ONE ;
						//��������
						memcpy(CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardData[Index], cbThreeLineCard+start, sizeof(BYTE)*cbThisTreeLineCardCount);
						//���浥��
						memcpy(CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardData[Index]+cbThisTreeLineCardCount, cbComResCard[i], cbSingleCardCount) ;


						CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbEachHandCardCount[Index] = cbThisTreeLineCardCount+cbSingleCardCount ;
						CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardTypeCount++ ;

						assert(CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardTypeCount<MAX_TYPE_COUNT) ;
					}

				}

				//�Ƴ���l
				bFindThreeLine = true ;
				RemoveCard(cbThreeLineCard, cbThreeLineCardCount, cbHandThreeCard, cbLeftThreeCardCount) ;
				cbLeftThreeCardCount -= cbThreeLineCardCount ;
			}
		}
	}

	//���һ��
	{
		//�ָ��˿ˣ���ֹ����ʱ�ı��˿�
		memcpy(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

		BYTE cbHandThreeCard[MAX_COUNT];
		BYTE cbHandThreeCount=0 ;
		BYTE cbRemainCarData[MAX_COUNT] ;
		BYTE cbRemainCardCount=0 ;

		//��ȡ����
		GetAllThreeCard(cbTmpCardData, cbHandCardCount, cbHandThreeCard, cbHandThreeCount) ;

		//�Ƴ������Ӧ���Ƴ�ը����ȣ�
		memcpy(cbRemainCarData, cbTmpCardData, cbHandCardCount) ;
		RemoveCard(cbHandThreeCard, cbHandThreeCount, cbRemainCarData, cbHandCardCount) ;
		cbRemainCardCount = cbHandCardCount-cbHandThreeCount ;

		//��ȡ����
		BYTE cbAllDoubleCardData[MAX_COUNT] ;
		BYTE cbAllDoubleCardCount=0 ;
		GetAllDoubleCard(cbRemainCarData, cbRemainCardCount, cbAllDoubleCardData, cbAllDoubleCardCount) ;

		//�����һ��
		for(BYTE i=0; i<cbHandThreeCount; i+=3)
		{
			BYTE Index ;

			//�����һ��
			for(BYTE j=0; j<cbAllDoubleCardCount; j+=2)
			{
				Index = CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardTypeCount ;
				CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardType = CT_THREE_LINE_TAKE_TWO ;
				CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardData[Index][0] = cbHandThreeCard[i] ;
				CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardData[Index][1] = cbHandThreeCard[i+1] ;
				CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardData[Index][2] = cbHandThreeCard[i+2] ;
				CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardData[Index][3] = cbAllDoubleCardData[j] ;
				CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardData[Index][4] = cbAllDoubleCardData[j+1] ;
				CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbEachHandCardCount[Index] = 5 ;
				CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardTypeCount++ ;
			}	
		}

		//��l���
		BYTE cbLeftThreeCardCount=cbHandThreeCount ;
		bool bFindThreeLine=true ;
		BYTE cbLastIndex=0 ;
		if(GetCardLogicValue(cbHandThreeCard[0])==15) cbLastIndex=3 ;
		while ((cbLeftThreeCardCount>=6) && bFindThreeLine)
		{
			BYTE cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[cbLastIndex]);
			BYTE cbThreeLineCard[MAX_COUNT];
			BYTE cbThreeLineCardCount=3;
			cbThreeLineCard[0]=cbHandThreeCard[cbLastIndex];
			cbThreeLineCard[1]=cbHandThreeCard[cbLastIndex+1];
			cbThreeLineCard[2]=cbHandThreeCard[cbLastIndex+2];

			bFindThreeLine=false ;
			for(BYTE j=3+cbLastIndex; j<cbLeftThreeCardCount; j+=3)
			{
				//l���ж�
				if(1!=(cbLastLogicCard-(GetCardLogicValue(cbHandThreeCard[j]))))
				{
					cbLastIndex = j ;
					if(cbLeftThreeCardCount-j>=6) bFindThreeLine = true ;

					break;
				}

				cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[j]);
				cbThreeLineCard[cbThreeLineCardCount]=cbHandThreeCard[j];
				cbThreeLineCard[cbThreeLineCardCount+1]=cbHandThreeCard[j+1];
				cbThreeLineCard[cbThreeLineCardCount+2]=cbHandThreeCard[j+2];
				cbThreeLineCardCount += 3;
			}
			if(cbThreeLineCardCount>3)
			{
				BYTE Index ;

				for(BYTE start=0; start<cbThreeLineCardCount-3; start+=3)
				{
					//��˳��Ŀ
					BYTE cbThisTreeLineCardCount = cbThreeLineCardCount-start ;
					//��������
					BYTE cbDoubleCardCount=((cbThisTreeLineCardCount)/3);

					//���Ʋ���
					if(cbRemainCardCount<cbDoubleCardCount) continue ;

					BYTE cbDoubleCardIndex[10]; //�����±�
					for(BYTE i=0, j=0; i<cbAllDoubleCardCount; i+=2, ++j)
						cbDoubleCardIndex[j]=i ;

					//�������
					BYTE cbComCard[5];
					BYTE cbComResCard[254][5] ;
					BYTE cbComResLen=0 ;

					//���ö��Ƶ��±�����ϣ��ٸ���±���ȡ�����
					Combination(cbComCard, 0, cbComResCard, cbComResLen, cbDoubleCardIndex, cbDoubleCardCount, cbAllDoubleCardCount/2, cbDoubleCardCount);

					assert(cbComResLen<=254) ;

					for(BYTE i=0; i<cbComResLen; ++i)
					{
						Index = CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardTypeCount ;
						CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardType = CT_THREE_LINE_TAKE_TWO ;
						//��������
						memcpy(CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardData[Index], cbThreeLineCard+start, sizeof(BYTE)*cbThisTreeLineCardCount);
						//�������
						for(BYTE j=0, k=0; j<cbDoubleCardCount; ++j, k+=2)
						{
							CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardData[Index][cbThisTreeLineCardCount+k] = cbAllDoubleCardData[cbComResCard[i][j]];
							CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardData[Index][cbThisTreeLineCardCount+k+1] = cbAllDoubleCardData[cbComResCard[i][j]+1];
						}

						CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbEachHandCardCount[Index] = cbThisTreeLineCardCount+2*cbDoubleCardCount ;
						CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardTypeCount++ ;

						assert(CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardTypeCount<MAX_TYPE_COUNT) ;
					}

				}
				//�Ƴ���l
				bFindThreeLine = true ;				
				RemoveCard(cbThreeLineCard, cbThreeLineCardCount, cbHandThreeCard, cbLeftThreeCardCount) ;
				cbLeftThreeCardCount -= cbThreeLineCardCount ;
			}
		}
	}
	//�Ĵ�}��
	/*
	{
	//�ָ��˿ˣ���ֹ����ʱ�ı��˿�
	memcpy(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

	BYTE cbFirstCard = 0 ;
	//ȥ������
	for(BYTE i=0 ; i<cbHandCardCount ; ++i)	if(GetCardColor(cbTmpCardData[i])!=0x40)	{cbFirstCard = i ; break ;}

	BYTE cbHandAllFourCardData[MAX_COUNT] ;
	BYTE cbHandAllFourCardCount=0;
	//��ȡ����
	GetAllBomCard(cbTmpCardData+cbFirstCard, cbHandCardCount-cbFirstCard, cbHandAllFourCardData, cbHandAllFourCardCount) ;

	//�Ƴ�����
	BYTE cbRemainCard[MAX_COUNT];
	BYTE cbRemainCardCount=cbHandCardCount-cbHandAllFourCardCount ;
	memcpy(cbRemainCard, cbTmpCardData, cbHandCardCount*sizeof(BYTE));
	RemoveCard(cbHandAllFourCardData, cbHandAllFourCardCount, cbRemainCard, cbHandCardCount) ;

	for(BYTE Start=0; Start<cbHandAllFourCardCount; Start += 4)
	{
	BYTE Index ;
	//�������
	BYTE cbComCard[5];
	BYTE cbComResCard[254][5] ;
	BYTE cbComResLen=0 ;
	//�������
	Combination(cbComCard, 0, cbComResCard, cbComResLen, cbRemainCard, 2, cbRemainCardCount, 2);
	for(BYTE i=0; i<cbComResLen; ++i)
	{
	//���ܴ��
	if(GetCardValue(cbComResCard[i][0])==GetCardValue(cbComResCard[i][1])) continue ;

	Index=CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbCardTypeCount ;
	CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbCardType = CT_FOUR_LINE_TAKE_ONE ;
	memcpy(CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbCardData[Index], cbHandAllFourCardData+Start, 4) ;
	CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbCardData[Index][4] = cbComResCard[i][0] ;
	CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbCardData[Index][4+1] = cbComResCard[i][1] ;
	CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbEachHandCardCount[Index] = 6 ;
	CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbCardTypeCount++ ;

	assert(CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbCardTypeCount<MAX_TYPE_COUNT) ;
	}
	}
	}*/


	//�Ĵ�}��
	/*
	{
	//�ָ��˿ˣ���ֹ����ʱ�ı��˿�
	memcpy(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

	BYTE cbFirstCard = 0 ;
	//ȥ������
	for(BYTE i=0 ; i<cbHandCardCount ; ++i)	if(GetCardColor(cbTmpCardData[i])!=0x40)	{cbFirstCard = i ; break ;}

	BYTE cbHandAllFourCardData[MAX_COUNT] ;
	BYTE cbHandAllFourCardCount=0;

	//��ȡ����
	GetAllBomCard(cbTmpCardData+cbFirstCard, cbHandCardCount-cbFirstCard, cbHandAllFourCardData, cbHandAllFourCardCount) ;

	//�Ƴ�����
	BYTE cbRemainCard[MAX_COUNT];
	BYTE cbRemainCardCount=cbHandCardCount-cbHandAllFourCardCount ;
	memcpy(cbRemainCard, cbTmpCardData, cbHandCardCount*sizeof(BYTE));
	RemoveCard(cbHandAllFourCardData, cbHandAllFourCardCount, cbRemainCard, cbHandCardCount) ;

	for(BYTE Start=0; Start<cbHandAllFourCardCount; Start += 4)
	{
	//��ȡ����
	BYTE cbAllDoubleCardData[MAX_COUNT] ;
	BYTE cbAllDoubleCardCount=0 ;
	GetAllDoubleCard(cbRemainCard, cbRemainCardCount, cbAllDoubleCardData, cbAllDoubleCardCount) ;

	BYTE cbDoubleCardIndex[10]; //�����±�
	for(BYTE i=0, j=0; i<cbAllDoubleCardCount; i+=2, ++j)
	cbDoubleCardIndex[j]=i ;

	//�������
	BYTE cbComCard[5];
	BYTE cbComResCard[255][5] ;
	BYTE cbComResLen=0 ;

	//���ö��Ƶ��±�����ϣ��ٸ���±���ȡ�����
	Combination(cbComCard, 0, cbComResCard, cbComResLen, cbDoubleCardIndex, 2, cbAllDoubleCardCount/2, 2);
	for(BYTE i=0; i<cbComResLen; ++i)
	{
	BYTE Index = CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbCardTypeCount ;
	CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbCardType = CT_FOUR_LINE_TAKE_TWO ;
	memcpy(CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbCardData[Index], cbHandAllFourCardData+Start, 4) ;

	//�������
	for(BYTE j=0, k=0; j<4; ++j, k+=2)
	{
	CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbCardData[Index][4+k] = cbAllDoubleCardData[cbComResCard[i][j]];
	CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbCardData[Index][4+k+1] = cbAllDoubleCardData[cbComResCard[i][j]+1];
	}

	CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbEachHandCardCount[Index] = 8 ;
	CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbCardTypeCount++ ;

	assert(CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbCardTypeCount<MAX_TYPE_COUNT) ;
	}
	}
	}*/

}

//��������
void CAndroidAI::AnalyseOutCardType(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE const cbTurnCardData[], BYTE const cbTurnCardCount,tagOutCardTypeResult CardTypeResult[12+1])
{
	memset(CardTypeResult, '\0',sizeof(CardTypeResult[0])*12) ;
	
	BYTE cbTmpCard[MAX_COUNT];
	memcpy(cbTmpCard, cbHandCardData, cbHandCardCount) ;
	SortCardList(cbTmpCard, cbHandCardCount, ST_ORDER) ;
	//	SortCardList(cbTurnCardData, cbTurnCardCount, ST_ORDER) ;

	BYTE cbTurnCardType = GetCardType(cbTurnCardData, cbTurnCardCount) ;

	assert(cbTurnCardType!=CT_ERROR) ;

	if(cbTurnCardType==CT_ERROR)
		return ;

	if(cbTurnCardType!=CT_MISSILE_CARD && cbTurnCardType!=CT_BOMB_CARD)
	{
		//˫��ը��
		if(cbHandCardCount>=2 && 0x4F==cbTmpCard[0] && 0x4E==cbTmpCard[1])
		{
			BYTE Index = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount;
			CardTypeResult[CT_BOMB_CARD].cbCardType = CT_BOMB_CARD ;
			CardTypeResult[CT_BOMB_CARD].cbCardData[Index][0] = cbTmpCard[0] ;
			CardTypeResult[CT_BOMB_CARD].cbCardData[Index][1] = cbTmpCard[1] ;
			CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index] = 2 ;
			CardTypeResult[CT_BOMB_CARD].cbCardTypeCount++;

			BYTE cbBomCardData[MAX_COUNT];
			BYTE cbBomCardCount=0;
			GetAllBomCard(cbTmpCard+2, cbHandCardCount-2, cbBomCardData, cbBomCardCount) ;
			for(BYTE i=0; i<cbBomCardCount/4; ++i)
			{
				Index = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount;
				CardTypeResult[CT_BOMB_CARD].cbCardType = CT_BOMB_CARD ;
				memcpy(CardTypeResult[CT_BOMB_CARD].cbCardData[Index], cbBomCardData+4*i, 4) ;
				CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index] = 4;
				CardTypeResult[CT_BOMB_CARD].cbCardTypeCount++;

				assert(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount<=MAX_TYPE_COUNT) ;
			}
		}
		//ը������
		else
		{
			BYTE cbBomCardData[MAX_COUNT];
			BYTE cbBomCardCount=0;
			GetAllBomCard(cbTmpCard, cbHandCardCount, cbBomCardData, cbBomCardCount) ;
			for(BYTE i=0; i<cbBomCardCount/4; ++i)
			{
				BYTE Index = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount;
				CardTypeResult[CT_BOMB_CARD].cbCardType = CT_BOMB_CARD ;
				memcpy(CardTypeResult[CT_BOMB_CARD].cbCardData[Index], cbBomCardData+4*i, 4) ;
				CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index] = 4;
				CardTypeResult[CT_BOMB_CARD].cbCardTypeCount++;

				assert(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount<=MAX_TYPE_COUNT) ;
			}
		}
	}

	switch(cbTurnCardType)
	{
	case CT_SINGLE:				//��������
		{			
			for(BYTE i=0; i<cbHandCardCount; ++i) 
				if(GetCardLogicValue(cbTmpCard[i])>GetCardLogicValue(cbTurnCardData[0])) 
				{
					BYTE Index = CardTypeResult[CT_SINGLE].cbCardTypeCount ;
					CardTypeResult[CT_SINGLE].cbCardType = CT_SINGLE ;
					CardTypeResult[CT_SINGLE].cbCardData[Index][0] = cbTmpCard[i];
					CardTypeResult[CT_SINGLE].cbEachHandCardCount[Index] = 1;
					CardTypeResult[CT_SINGLE].cbCardTypeCount++ ;

					assert(CardTypeResult[CT_SINGLE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
				}
				break ;
		}
	case CT_DOUBLE:				//��������
		{
			//�˿˷���
			for (BYTE i=0;i<cbHandCardCount;i++)
			{
				//������
				BYTE cbSameCount=1;
				BYTE cbLogicValue=GetCardLogicValue(cbTmpCard[i]);

				//����ͬ��
				for (BYTE j=i+1;j<cbHandCardCount;j++)
				{
					//��ȡ�˿�
					if (GetCardLogicValue(cbTmpCard[j])!=cbLogicValue) break;

					//���ñ�
					cbSameCount++;
				}

				if(cbSameCount>=2 && GetCardLogicValue(cbTmpCard[i])>GetCardLogicValue(cbTurnCardData[0]))
				{
					BYTE Index = CardTypeResult[CT_DOUBLE].cbCardTypeCount ;
					CardTypeResult[CT_DOUBLE].cbCardType = CT_DOUBLE ;
					CardTypeResult[CT_DOUBLE].cbCardData[Index][0] = cbTmpCard[i];
					CardTypeResult[CT_DOUBLE].cbCardData[Index][1] = cbTmpCard[i+1];
					CardTypeResult[CT_DOUBLE].cbEachHandCardCount[Index] = 2;
					CardTypeResult[CT_DOUBLE].cbCardTypeCount++ ;

					assert(CardTypeResult[CT_DOUBLE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
				}
				//��������
				i+=cbSameCount-1;
			}
			break ;
		}
	case CT_THREE:				//��������
		{
			//�˿˷���
			for (BYTE i=0;i<cbHandCardCount;i++)
			{
				//������
				BYTE cbSameCount=1;
				BYTE cbLogicValue=GetCardLogicValue(cbTmpCard[i]);

				//����ͬ��
				for (BYTE j=i+1;j<cbHandCardCount;j++)
				{
					//��ȡ�˿�
					if (GetCardLogicValue(cbTmpCard[j])!=cbLogicValue) break;

					//���ñ�
					cbSameCount++;
				}

				if(cbSameCount>=3 && GetCardLogicValue(cbTmpCard[i])>GetCardLogicValue(cbTurnCardData[0]))
				{
					BYTE Index = CardTypeResult[CT_THREE].cbCardTypeCount ;
					CardTypeResult[CT_THREE].cbCardType = CT_THREE ;
					CardTypeResult[CT_THREE].cbCardData[Index][0] = cbTmpCard[i];
					CardTypeResult[CT_THREE].cbCardData[Index][1] = cbTmpCard[i+1];
					CardTypeResult[CT_THREE].cbCardData[Index][2] = cbTmpCard[i+2];
					CardTypeResult[CT_THREE].cbEachHandCardCount[Index] = 3;
					CardTypeResult[CT_THREE].cbCardTypeCount++ ;

					assert(CardTypeResult[CT_THREE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
				}
				//��������
				i+=cbSameCount-1;
			}
			break ;
		}
	case CT_SINGLE_LINE:		//��l����
		{
			BYTE cbFirstCard = 0 ;
			//ȥ��2����
			for(BYTE i=0 ; i<cbHandCardCount ; ++i)	if(GetCardLogicValue(cbTmpCard[i])<15)	{cbFirstCard = i ; break ;}

			BYTE cbSingleLineCard[12] ;
			BYTE cbSingleLineCount=1 ;
			BYTE cbLeftCardCount = cbHandCardCount ;
			bool bFindSingleLine = true ;

			//l���ж�
			while (cbLeftCardCount>=cbTurnCardCount && bFindSingleLine)
			{
				cbSingleLineCount=1 ;
				bFindSingleLine = false ;
				BYTE cbLastCard = cbTmpCard[cbFirstCard] ;
				cbSingleLineCard[cbSingleLineCount-1] = cbTmpCard[cbFirstCard] ;
				for (BYTE i=cbFirstCard+1; i<cbLeftCardCount; i++)
				{
					BYTE cbCardData=cbTmpCard[i];

					//l���ж�
					if (1!=(GetCardLogicValue(cbLastCard)-GetCardLogicValue(cbCardData)) && GetCardValue(cbLastCard)!=GetCardValue(cbCardData)) 
					{
						cbLastCard = cbTmpCard[i] ;
						//�Ƿ�Ϸ�
						if(cbSingleLineCount<cbTurnCardCount) 
						{
							cbSingleLineCount = 1 ;
							cbSingleLineCard[cbSingleLineCount-1] = cbTmpCard[i] ;
							continue ;
						}
						else break ;
					}
					//ͬ���ж�
					else if(GetCardValue(cbLastCard)!=GetCardValue(cbCardData))
					{
						cbLastCard = cbCardData ;
						cbSingleLineCard[cbSingleLineCount] = cbCardData ;
						++cbSingleLineCount ;
					}
				}

				//�������
				if(cbSingleLineCount>=cbTurnCardCount && GetCardLogicValue(cbSingleLineCard[0])>GetCardLogicValue(cbTurnCardData[0]))
				{
					BYTE Index ;
					BYTE cbStart=0 ;
					//����l��
					while (GetCardLogicValue(cbSingleLineCard[cbStart])>GetCardLogicValue(cbTurnCardData[0]) && ((cbSingleLineCount-cbStart)>=cbTurnCardCount))
					{
						Index = CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount ;
						CardTypeResult[CT_SINGLE_LINE].cbCardType = CT_SINGLE_LINE ;
						memcpy(CardTypeResult[CT_SINGLE_LINE].cbCardData[Index], cbSingleLineCard+cbStart, sizeof(BYTE)*cbTurnCardCount);
						CardTypeResult[CT_SINGLE_LINE].cbEachHandCardCount[Index] = cbTurnCardCount;
						CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount++ ;
						cbStart++;

						assert(CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
					}

					RemoveCard(cbSingleLineCard, cbSingleLineCount, cbTmpCard, cbLeftCardCount) ;
					cbLeftCardCount -= cbSingleLineCount ;
					bFindSingleLine = true ;
				}
			}

			break ;
		}
	case CT_DOUBLE_LINE:		//��l����
		{
			//l���ж�
			BYTE cbFirstCard = 0 ;
			//ȥ��2����
			for(BYTE i=0 ; i<cbHandCardCount ; ++i)	if(GetCardLogicValue(cbTmpCard[i])<15)	{cbFirstCard = i ; break ;}

			BYTE cbLeftCardCount = cbHandCardCount-cbFirstCard ;
			bool bFindDoubleLine = true ;
			BYTE cbDoubleLineCount = 0 ;
			BYTE cbDoubleLineCard[24] ;
			//��ʼ�ж�
			while (cbLeftCardCount>=cbTurnCardCount && bFindDoubleLine)
			{
				BYTE cbLastCard = cbTmpCard[cbFirstCard] ;
				BYTE cbSameCount = 1 ;
				cbDoubleLineCount = 0 ;
				bFindDoubleLine=false ;
				for(BYTE i=cbFirstCard+1 ; i<cbLeftCardCount+cbFirstCard ; ++i)
				{
					//����ͬ��
					while (GetCardValue(cbLastCard)==GetCardValue(cbTmpCard[i]) && i<cbLeftCardCount+cbFirstCard)
					{
						++cbSameCount;
						++i ;
					}

					BYTE cbLastDoubleCardValue ;
					if(cbDoubleLineCount>0) cbLastDoubleCardValue = GetCardLogicValue(cbDoubleLineCard[cbDoubleLineCount-1]) ;
					//���¿�ʼ
					if((cbSameCount<2 || (cbDoubleLineCount>0 && (cbLastDoubleCardValue-GetCardLogicValue(cbLastCard))!=1)) && i<=cbLeftCardCount+cbFirstCard)
					{
						if(cbDoubleLineCount>=cbTurnCardCount) break ;

						if(cbSameCount>=2) i-=cbSameCount ;

						cbLastCard = cbTmpCard[i] ;
						cbDoubleLineCount = 0 ;
					}
					//�������
					else if(cbSameCount>=2)
					{
						cbDoubleLineCard[cbDoubleLineCount] = cbTmpCard[i-cbSameCount] ;
						cbDoubleLineCard[cbDoubleLineCount+1] = cbTmpCard[i-cbSameCount+1] ;
						cbDoubleLineCount += 2 ;

						//��β�ж�
						if(i==(cbLeftCardCount+cbFirstCard-2))
							if((GetCardLogicValue(cbLastCard)-GetCardLogicValue(cbTmpCard[i]))==1 && (GetCardLogicValue(cbTmpCard[i])==GetCardLogicValue(cbTmpCard[i+1])))
							{
								cbDoubleLineCard[cbDoubleLineCount] = cbTmpCard[i] ;
								cbDoubleLineCard[cbDoubleLineCount+1] = cbTmpCard[i+1] ;
								cbDoubleLineCount += 2 ;
								break ;
							}

					}

					cbLastCard = cbTmpCard[i] ;
					cbSameCount = 1 ;
				}

				//�������
				if(cbDoubleLineCount>=cbTurnCardCount)
				{
					BYTE Index ;
					BYTE cbStart=0 ;
					//����l��
					while (GetCardLogicValue(cbDoubleLineCard[cbStart])>GetCardLogicValue(cbTurnCardData[0]) && ((cbDoubleLineCount-cbStart)>=cbTurnCardCount))
					{
						Index = CardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount ;
						CardTypeResult[CT_DOUBLE_LINE].cbCardType = CT_DOUBLE_LINE ;
						memcpy(CardTypeResult[CT_DOUBLE_LINE].cbCardData[Index], cbDoubleLineCard+cbStart, sizeof(BYTE)*cbTurnCardCount);
						CardTypeResult[CT_DOUBLE_LINE].cbEachHandCardCount[Index] = cbTurnCardCount;
						CardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount++ ;
						cbStart += 2;

						assert(CardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
					}
					RemoveCard(cbDoubleLineCard, cbDoubleLineCount, cbTmpCard, cbFirstCard+cbLeftCardCount) ;				
					bFindDoubleLine=true ;
					cbLeftCardCount -= cbDoubleLineCount ;
				}
			}

			break;
		}
	case CT_THREE_LINE:			//��l����
		{
			//l���ж�
			BYTE cbFirstCard = 0 ;
			//ȥ��2����
			for(BYTE i=0 ; i<cbHandCardCount ; ++i)	if(GetCardLogicValue(cbTmpCard[i])<15)	{cbFirstCard = i ; break ;}

			BYTE cbLeftCardCount = cbHandCardCount-cbFirstCard ;
			bool bFindThreeLine = true ;
			BYTE cbThreeLineCount = 0 ;
			BYTE cbThreeLineCard[20] ;
			//��ʼ�ж�
			while (cbLeftCardCount>=cbTurnCardCount && bFindThreeLine)
			{
				BYTE cbLastCard = cbTmpCard[cbFirstCard] ;
				BYTE cbSameCount = 1 ;
				cbThreeLineCount = 0 ;
				bFindThreeLine = false ;
				for(BYTE i=cbFirstCard+1 ; i<cbLeftCardCount+cbFirstCard ; ++i)
				{
					//����ͬ��
					while (GetCardValue(cbLastCard)==GetCardValue(cbTmpCard[i]) && i<cbLeftCardCount+cbFirstCard)
					{
						++cbSameCount;
						++i ;
					}

					BYTE cbLastThreeCardValue ;
					if(cbThreeLineCount>0) cbLastThreeCardValue = GetCardLogicValue(cbThreeLineCard[cbThreeLineCount-1]) ;

					//���¿�ʼ
					if((cbSameCount<3 || (cbThreeLineCount>0&&(cbLastThreeCardValue-GetCardLogicValue(cbLastCard))!=1)) && i<=cbLeftCardCount+cbFirstCard)
					{
						if(cbThreeLineCount>=cbTurnCardCount) break ;

						if(cbSameCount>=3) i-= 3 ;
						cbLastCard = cbTmpCard[i] ;
						cbThreeLineCount = 0 ;
					}
					//�������
					else if(cbSameCount>=3)
					{
						cbThreeLineCard[cbThreeLineCount] = cbTmpCard[i-cbSameCount] ;
						cbThreeLineCard[cbThreeLineCount+1] = cbTmpCard[i-cbSameCount+1] ;
						cbThreeLineCard[cbThreeLineCount+2] = cbTmpCard[i-cbSameCount+2] ;
						cbThreeLineCount += 3 ;

						//��β�ж�
						if(i==(cbLeftCardCount+cbFirstCard-3))
							if((GetCardLogicValue(cbLastCard)-GetCardLogicValue(cbTmpCard[i]))==1 && (GetCardLogicValue(cbTmpCard[i])==GetCardLogicValue(cbTmpCard[i+1])) && (GetCardLogicValue(cbTmpCard[i])==GetCardLogicValue(cbTmpCard[i+2])))
							{
								cbThreeLineCard[cbThreeLineCount] = cbTmpCard[i] ;
								cbThreeLineCard[cbThreeLineCount+1] = cbTmpCard[i+1] ;
								cbThreeLineCard[cbThreeLineCount+2] = cbTmpCard[i+2] ;
								cbThreeLineCount += 3 ;
								break ;
							}

					}

					cbLastCard = cbTmpCard[i] ;
					cbSameCount = 1 ;
				}

				//�������
				if(cbThreeLineCount>=cbTurnCardCount)
				{
					BYTE Index ;
					BYTE cbStart=0 ;
					//����l��
					while (GetCardLogicValue(cbThreeLineCard[cbStart])>GetCardLogicValue(cbTurnCardData[0]) && ((cbThreeLineCount-cbStart)>=cbTurnCardCount))
					{
						Index = CardTypeResult[CT_THREE_LINE].cbCardTypeCount ;
						CardTypeResult[CT_THREE_LINE].cbCardType = CT_THREE_LINE ;
						memcpy(CardTypeResult[CT_THREE_LINE].cbCardData[Index], cbThreeLineCard+cbStart, sizeof(BYTE)*cbTurnCardCount);
						CardTypeResult[CT_THREE_LINE].cbEachHandCardCount[Index] = cbTurnCardCount;
						CardTypeResult[CT_THREE_LINE].cbCardTypeCount++ ;
						cbStart += 3;

						assert(CardTypeResult[CT_THREE_LINE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
					}

					RemoveCard(cbThreeLineCard, cbThreeLineCount, cbTmpCard, cbFirstCard+cbLeftCardCount) ;
					bFindThreeLine=true ;
					cbLeftCardCount -= cbThreeLineCount ;
				}
			}

			break;
		}
	case CT_THREE_LINE_TAKE_ONE://���һ��
		{
			BYTE cbTurnThreeCard[MAX_COUNT]; 
			BYTE cbTurnThreeCount=0;
			BYTE cbHandThreeCard[MAX_COUNT];
			BYTE cbHandThreeCount=0 ;
			BYTE cbSingleCardCount=cbTurnCardCount/4;

			//�Ƴ�ը��
			BYTE cbAllBomCardData[MAX_COUNT] ;
			BYTE cbAllBomCardCount=0 ;
			GetAllBomCard(cbTmpCard, cbHandCardCount, cbAllBomCardData, cbAllBomCardCount) ;
			RemoveCard(cbAllBomCardData, cbAllBomCardCount, cbTmpCard, cbHandCardCount) ;

			//�����˿�
			GetAllThreeCard(cbTurnCardData, cbTurnCardCount, cbTurnThreeCard, cbTurnThreeCount) ;

			BYTE cbFirstCard = 0 ;

			//ȥ��2����
			if(cbTurnThreeCount>3)
				for(BYTE i=0 ; i<cbHandCardCount-cbAllBomCardCount ; ++i)	
					if(GetCardLogicValue(cbTmpCard[i])<15)	
					{
						cbFirstCard = i ; 
						break ;
					}

					GetAllThreeCard(cbTmpCard+cbFirstCard, cbHandCardCount-cbFirstCard-cbAllBomCardCount, cbHandThreeCard, cbHandThreeCount) ;

					if(cbHandThreeCount<cbTurnThreeCount || (cbHandThreeCount>0&&GetCardLogicValue(cbHandThreeCard[0])<GetCardLogicValue(cbTurnThreeCard[0]))) return ;

					for(BYTE i=0; i<cbHandThreeCount; i+=3)
					{
						BYTE cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[i]);
						BYTE cbThreeLineCard[MAX_COUNT];
						BYTE cbThreeLineCardCount=3;
						cbThreeLineCard[0]=cbHandThreeCard[i];
						cbThreeLineCard[1]=cbHandThreeCard[i+1];
						cbThreeLineCard[2]=cbHandThreeCard[i+2];
						for(BYTE j=i+3; j<cbHandThreeCount; j+=3)
						{
							//l���ж�
							if(1!=(cbLastLogicCard-(GetCardLogicValue(cbHandThreeCard[j]))) || cbThreeLineCardCount==cbTurnThreeCount) break;

							cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[j]);
							cbThreeLineCard[cbThreeLineCardCount]=cbHandThreeCard[j];
							cbThreeLineCard[cbThreeLineCardCount+1]=cbHandThreeCard[j+1];
							cbThreeLineCard[cbThreeLineCardCount+2]=cbHandThreeCard[j+2];
							cbThreeLineCardCount += 3;
						}
						if(cbThreeLineCardCount==cbTurnThreeCount && GetCardLogicValue(cbThreeLineCard[0])>GetCardLogicValue(cbTurnThreeCard[0]))
						{
							BYTE Index ;

							BYTE cbRemainCard[MAX_COUNT];
							memcpy(cbRemainCard, cbTmpCard, (cbHandCardCount-cbAllBomCardCount)*sizeof(BYTE));
							RemoveCard(cbThreeLineCard, cbTurnThreeCount, cbRemainCard, (cbHandCardCount-cbAllBomCardCount)) ;

							//�������
							BYTE cbComCard[5];
							BYTE cbComResCard[254][5] ;
							BYTE cbComResLen=0 ;
							Combination(cbComCard, 0, cbComResCard, cbComResLen, cbRemainCard, cbSingleCardCount, (cbHandCardCount-cbAllBomCardCount)-cbTurnThreeCount, cbSingleCardCount);
							for(BYTE i=0; i<cbComResLen; ++i)
							{
								Index = CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardTypeCount ;
								CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardType = CT_THREE_LINE_TAKE_ONE;
								//��������
								memcpy(CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardData[Index], cbThreeLineCard, sizeof(BYTE)*cbTurnThreeCount);
								//���浥��
								memcpy(CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardData[Index]+cbTurnThreeCount, cbComResCard[i], cbSingleCardCount) ;

								assert(cbTurnThreeCount+cbSingleCardCount==cbTurnCardCount) ;
								CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbEachHandCardCount[Index] = cbTurnCardCount ;
								CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardTypeCount++ ;

								assert(CardTypeResult[CT_THREE_LINE_TAKE_ONE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
							}

						}
					}

					break;
		}
	case CT_THREE_LINE_TAKE_TWO://���һ��
		{
			BYTE cbTurnThreeCard[MAX_COUNT]; 
			BYTE cbTurnThreeCount=0;
			BYTE cbHandThreeCard[MAX_COUNT];
			BYTE cbHandThreeCount=0 ;
			BYTE cbDoubleCardCount=cbTurnCardCount/5;

			//�����˿�
			GetAllThreeCard(cbTurnCardData, cbTurnCardCount, cbTurnThreeCard, cbTurnThreeCount) ;

			BYTE cbFirstCard = 0 ;

			//ȥ��2����
			if(cbTurnThreeCount>3)
				for(BYTE i=0 ; i<cbHandCardCount ; ++i)	if(GetCardLogicValue(cbTmpCard[i])<15)	{cbFirstCard = i ; break ;}

				GetAllThreeCard(cbTmpCard+cbFirstCard, cbHandCardCount-cbFirstCard, cbHandThreeCard, cbHandThreeCount) ;

				if(cbHandThreeCount<cbTurnThreeCount || (cbHandThreeCount>0&&GetCardLogicValue(cbHandThreeCard[0])<GetCardLogicValue(cbTurnThreeCard[0]))) return ;

				for(BYTE i=0; i<cbHandThreeCount; i+=3)
				{
					BYTE cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[i]);
					BYTE cbThreeLineCard[MAX_COUNT];
					BYTE cbThreeLineCardCount=3;
					cbThreeLineCard[0]=cbHandThreeCard[i];
					cbThreeLineCard[1]=cbHandThreeCard[i+1];
					cbThreeLineCard[2]=cbHandThreeCard[i+2];
					for(BYTE j=i+3; j<cbHandThreeCount; j+=3)
					{
						//l���ж�
						if(1!=(cbLastLogicCard-(GetCardLogicValue(cbHandThreeCard[j]))) || cbThreeLineCardCount==cbTurnThreeCount) break;

						cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[j]);
						cbThreeLineCard[cbThreeLineCardCount]=cbHandThreeCard[j];
						cbThreeLineCard[cbThreeLineCardCount+1]=cbHandThreeCard[j+1];
						cbThreeLineCard[cbThreeLineCardCount+2]=cbHandThreeCard[j+2];
						cbThreeLineCardCount += 3;
					}
					if(cbThreeLineCardCount==cbTurnThreeCount && GetCardLogicValue(cbThreeLineCard[0])>GetCardLogicValue(cbTurnThreeCard[0]))
					{
						BYTE Index ;

						BYTE cbRemainCard[MAX_COUNT];
						memcpy(cbRemainCard, cbTmpCard, cbHandCardCount*sizeof(BYTE));
						RemoveCard(cbThreeLineCard, cbTurnThreeCount, cbRemainCard, cbHandCardCount) ;

						BYTE cbAllDoubleCardData[MAX_COUNT] ;
						BYTE cbAllDoubleCardCount=0 ;
						GetAllDoubleCard(cbRemainCard, cbHandCardCount-cbTurnThreeCount, cbAllDoubleCardData, cbAllDoubleCardCount) ;


						BYTE cbDoubleCardIndex[10]; //�����±�
						for(BYTE i=0, j=0; i<cbAllDoubleCardCount; i+=2, ++j)
							cbDoubleCardIndex[j]=i ;

						//�������
						BYTE cbComCard[5];
						BYTE cbComResCard[254][5] ;
						BYTE cbComResLen=0 ;

						//���ö��Ƶ��±�����ϣ��ٸ���±���ȡ�����
						Combination(cbComCard, 0, cbComResCard, cbComResLen, cbDoubleCardIndex, cbDoubleCardCount, cbAllDoubleCardCount/2, cbDoubleCardCount);
						for(BYTE i=0; i<cbComResLen; ++i)
						{
							Index = CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardTypeCount ;
							CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardType = CT_THREE_LINE_TAKE_TWO ;
							//��������
							memcpy(CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardData[Index], cbThreeLineCard, sizeof(BYTE)*cbTurnThreeCount);
							//�������
							for(BYTE j=0, k=0; j<cbDoubleCardCount; ++j, k+=2)
							{
								CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardData[Index][cbTurnThreeCount+k] = cbAllDoubleCardData[cbComResCard[i][j]];
								CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardData[Index][cbTurnThreeCount+k+1] = cbAllDoubleCardData[cbComResCard[i][j]+1];
							}

							assert(cbTurnThreeCount+cbDoubleCardCount*2==cbTurnCardCount) ;
							CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbEachHandCardCount[Index] = cbTurnCardCount ;

							CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardTypeCount++ ;

							assert(CardTypeResult[CT_THREE_LINE_TAKE_TWO].cbCardTypeCount<=MAX_TYPE_COUNT) ;
						}		

					}
				}

				break;
		}
	case CT_FOUR_LINE_TAKE_ONE://�Ĵ�}��
		{
			BYTE cbFirstCard = 0 ;
			//ȥ������
			for(BYTE i=0 ; i<cbHandCardCount ; ++i)	if(GetCardColor(cbTmpCard[i])!=0x40)	{cbFirstCard = i ; break ;}

			BYTE cbHandAllFourCardData[MAX_COUNT] ;
			BYTE cbHandAllFourCardCount=0;
			BYTE cbTurnAllFourCardData[MAX_COUNT];
			BYTE cbTurnAllFourCardCount=0;
			//��ȡ����
			GetAllBomCard(cbTmpCard+cbFirstCard, cbHandCardCount-cbFirstCard, cbHandAllFourCardData, cbHandAllFourCardCount) ;
			GetAllBomCard(cbTurnCardData, cbTurnCardCount, cbTurnAllFourCardData, cbTurnAllFourCardCount) ;

			if(cbHandAllFourCardCount>0 && GetCardLogicValue(cbHandAllFourCardData[0])<GetCardLogicValue(cbTurnAllFourCardData[0])) return ;


			BYTE cbCanOutFourCardData[MAX_COUNT] ;
			BYTE cbCanOutFourCardCount=0 ;
			//�ɳ����
			for(BYTE i=0; i<cbHandAllFourCardCount; i+=4)
			{
				if(GetCardLogicValue(cbHandAllFourCardData[i])>GetCardLogicValue(cbTurnAllFourCardData[0]))
				{
					cbCanOutFourCardData[cbCanOutFourCardCount] = cbHandAllFourCardData[i] ;
					cbCanOutFourCardData[cbCanOutFourCardCount+1] = cbHandAllFourCardData[i+1] ;
					cbCanOutFourCardData[cbCanOutFourCardCount+2] = cbHandAllFourCardData[i+2] ;
					cbCanOutFourCardData[cbCanOutFourCardCount+3] = cbHandAllFourCardData[i+3] ;
					cbCanOutFourCardCount += 4 ;
				}
			}

			if((cbHandCardCount-cbCanOutFourCardCount) < (cbTurnCardCount-cbTurnAllFourCardCount)) return ;

			BYTE cbRemainCard[MAX_COUNT];
			memcpy(cbRemainCard, cbTmpCard, cbHandCardCount*sizeof(BYTE));
			RemoveCard(cbCanOutFourCardData, cbCanOutFourCardCount, cbRemainCard, cbHandCardCount) ;
			for(BYTE Start=0; Start<cbCanOutFourCardCount; Start += 4)
			{
				BYTE Index ;
				//�������
				BYTE cbComCard[5];
				BYTE cbComResCard[254][5] ;
				BYTE cbComResLen=0 ;
				//�������
				Combination(cbComCard, 0, cbComResCard, cbComResLen, cbRemainCard, 2, cbHandCardCount-cbCanOutFourCardCount, 2);
				for(BYTE i=0; i<cbComResLen; ++i)
				{
					//���ܴ��
					if(GetCardValue(cbComResCard[i][0])==GetCardValue(cbComResCard[i][1])) continue ;

					Index=CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbCardTypeCount ;
					CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbCardType = CT_FOUR_LINE_TAKE_ONE ;
					memcpy(CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbCardData[Index], cbCanOutFourCardData+Start, 4) ;
					CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbCardData[Index][4] = cbComResCard[i][0] ;
					CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbCardData[Index][4+1] = cbComResCard[i][1] ;
					CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbEachHandCardCount[Index] = 6 ;
					CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbCardTypeCount++ ;

					assert(CardTypeResult[CT_FOUR_LINE_TAKE_ONE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
				}
			}


			break;
		}
	case CT_FOUR_LINE_TAKE_TWO://�Ĵ�}��
		{
			BYTE cbFirstCard = 0 ;
			//ȥ������
			for(BYTE i=0 ; i<cbHandCardCount ; ++i)	if(GetCardColor(cbTmpCard[i])!=0x40)	{cbFirstCard = i ; break ;}

			BYTE cbHandAllFourCardData[MAX_COUNT] ;
			BYTE cbHandAllFourCardCount=0;
			BYTE cbTurnAllFourCardData[MAX_COUNT];
			BYTE cbTurnAllFourCardCount=0;
			//��ȡ����
			GetAllBomCard(cbTmpCard+cbFirstCard, cbHandCardCount-cbFirstCard, cbHandAllFourCardData, cbHandAllFourCardCount) ;
			GetAllBomCard(cbTurnCardData, cbTurnCardCount, cbTurnAllFourCardData, cbTurnAllFourCardCount) ;

			if(cbHandAllFourCardCount>0 && GetCardLogicValue(cbHandAllFourCardData[0])<GetCardLogicValue(cbTurnAllFourCardData[0])) return ;


			BYTE cbCanOutFourCardData[MAX_COUNT] ;
			BYTE cbCanOutFourCardCount=0 ;
			//�ɳ����
			for(BYTE i=0; i<cbHandAllFourCardCount; i+=4)
			{
				if(GetCardLogicValue(cbHandAllFourCardData[i])>GetCardLogicValue(cbTurnAllFourCardData[0]))
				{
					cbCanOutFourCardData[cbCanOutFourCardCount] = cbHandAllFourCardData[i] ;
					cbCanOutFourCardData[cbCanOutFourCardCount+1] = cbHandAllFourCardData[i+1] ;
					cbCanOutFourCardData[cbCanOutFourCardCount+2] = cbHandAllFourCardData[i+2] ;
					cbCanOutFourCardData[cbCanOutFourCardCount+3] = cbHandAllFourCardData[i+3] ;
					cbCanOutFourCardCount += 4 ;
				}
			}

			if((cbHandCardCount-cbCanOutFourCardCount) < (cbTurnCardCount-cbTurnAllFourCardCount)) return ;

			BYTE cbRemainCard[MAX_COUNT];
			memcpy(cbRemainCard, cbTmpCard, cbHandCardCount*sizeof(BYTE));
			RemoveCard(cbCanOutFourCardData, cbCanOutFourCardCount, cbRemainCard, cbHandCardCount) ;
			for(BYTE Start=0; Start<cbCanOutFourCardCount; Start += 4)
			{
				BYTE cbAllDoubleCardData[MAX_COUNT] ;
				BYTE cbAllDoubleCardCount=0 ;
				GetAllDoubleCard(cbRemainCard, cbHandCardCount-cbCanOutFourCardCount, cbAllDoubleCardData, cbAllDoubleCardCount) ;


				BYTE cbDoubleCardIndex[10]; //�����±�
				for(BYTE i=0, j=0; i<cbAllDoubleCardCount; i+=2, ++j)
					cbDoubleCardIndex[j]=i ;

				//�������
				BYTE cbComCard[5];
				BYTE cbComResCard[254][5] ;
				BYTE cbComResLen=0 ;

				//���ö��Ƶ��±�����ϣ��ٸ���±���ȡ�����
				Combination(cbComCard, 0, cbComResCard, cbComResLen, cbDoubleCardIndex, 2, cbAllDoubleCardCount/2, 2);
				for(BYTE i=0; i<cbComResLen; ++i)
				{
					BYTE Index = CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbCardTypeCount ;
					CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbCardType = CT_FOUR_LINE_TAKE_TWO ;
					memcpy(CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbCardData[Index], cbCanOutFourCardData+Start, 4) ;

					//�������
					for(BYTE j=0, k=0; j<4; ++j, k+=2)
					{
						CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbCardData[Index][4+k] = cbAllDoubleCardData[cbComResCard[i][j]];
						CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbCardData[Index][4+k+1] = cbAllDoubleCardData[cbComResCard[i][j]+1];
					}

					CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbEachHandCardCount[Index] = 8 ;
					CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbCardTypeCount++ ;

					assert(CardTypeResult[CT_FOUR_LINE_TAKE_TWO].cbCardTypeCount<=MAX_TYPE_COUNT) ;
				}
			}
			break;
		}
	case CT_BOMB_CARD:			//ը������
		{
			BYTE cbAllBomCardData[MAX_COUNT] ;
			BYTE cbAllBomCardCount=0 ; 
			GetAllBomCard(cbTmpCard, cbHandCardCount, cbAllBomCardData, cbAllBomCardCount) ;
			BYTE cbFirstBom=0 ;
			BYTE Index ;
			if(cbAllBomCardCount>0 && cbAllBomCardData[0]==0x4F)
			{
				Index = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount ;
				CardTypeResult[CT_BOMB_CARD].cbCardType = CT_BOMB_CARD ;
				CardTypeResult[CT_BOMB_CARD].cbCardData[Index][0] = 0x4F ;
				CardTypeResult[CT_BOMB_CARD].cbCardData[Index][1] = 0x4E ;
				CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index] = 2 ;
				CardTypeResult[CT_BOMB_CARD].cbCardTypeCount++ ;

				assert(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount<=MAX_TYPE_COUNT) ;
				cbFirstBom=2;
			}
			for(BYTE i=cbFirstBom; i<cbAllBomCardCount; i+=4)
			{
				if(GetCardLogicValue(cbAllBomCardData[i])>GetCardLogicValue(cbTurnCardData[0]))
				{
					Index = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount ;
					CardTypeResult[CT_BOMB_CARD].cbCardType = CT_BOMB_CARD ;
					memcpy(CardTypeResult[CT_BOMB_CARD].cbCardData[Index], cbAllBomCardData+i, 4) ;
					CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index] = 4 ;
					CardTypeResult[CT_BOMB_CARD].cbCardTypeCount++ ;

					assert(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount<=MAX_TYPE_COUNT) ;
				}
			}
			break;
		}
	case CT_MISSILE_CARD:		//�������
		{
			//û�бȻ���������
			break;
		}
	default:
		assert(false) ;
		break;
	}

}


/********************************************************************
������Combination
����
cbCombineCardData���洢�������Ͻ��
cbResComLen���ѵõ�����ϳ��ȣ���ʼ����ʱ�˲���Ϊ0
cbResultCardData���洢���е���Ͻ��
cbResCardLen��cbResultCardData�ĵ�һ�±�ĳ��ȣ���Ͻ��ĸ���
cbSrcCardData����Ҫ����ϵ����
cbSrcLen��cbSrcCardData�������Ŀ
cbCombineLen2��cbCombineLen1����ϵĳ��ȣ���ʼ����ʱ}����ȡ�
*********************************************************************/
//����㷨
void CAndroidAI::Combination(BYTE cbCombineCardData[], BYTE cbResComLen,  BYTE cbResultCardData[100][5], BYTE &cbResCardLen,BYTE cbSrcCardData[] , BYTE cbCombineLen1, BYTE cbSrcLen, const BYTE cbCombineLen2)
{

	if( cbResComLen == cbCombineLen2 )
	{
		memcpy(&cbResultCardData[cbResCardLen], cbCombineCardData, cbResComLen) ;
		++cbResCardLen ;

		assert(cbResCardLen<255) ;

	}
	else
	{ 
		if(cbCombineLen1 >= 1 && cbSrcLen > 0 && (cbSrcLen+1) >= 0 ){
			cbCombineCardData[cbCombineLen2-cbCombineLen1] =  cbSrcCardData[0];
			++cbResComLen ;
			Combination(cbCombineCardData,cbResComLen, cbResultCardData, cbResCardLen, cbSrcCardData+1,cbCombineLen1-1, cbSrcLen-1, cbCombineLen2);

			--cbResComLen;
			Combination(cbCombineCardData,cbResComLen, cbResultCardData, cbResCardLen, cbSrcCardData+1,cbCombineLen1, cbSrcLen-1, cbCombineLen2);
		}
	}
}

//�����㷨
void CAndroidAI::Permutation(BYTE *list, int m, int n, BYTE result[][4], BYTE &len)
{ 
	int j,temp; 
	if(m == n){ 
		for(j = 0; j < n; j++) 
			result[len][j]=list[j]; 
		len++ ;
	} 
	else{ 
		for(j = m; j < n; j++){ 
			temp = list[m] ;
			list[m] = list[j];
			list[j] = temp ;
			Permutation(list,m+1,n,result,len); 
			temp = list[m] ;
			list[m] = list[j];
			list[j] = temp ;
		} 
	} 
} 

//���Ƹ���
BYTE CAndroidAI::AnalyseSinleCardCount(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE const cbWantOutCardData[], BYTE const cbWantOutCardCount, BYTE cbSingleCardData[])
{
	//�����ж�
	assert(cbHandCardCount>0) ;
	if(cbHandCardCount<=0) return MAX_COUNT + 5 ;

	BYTE cbRemainCard[MAX_COUNT] ;
	BYTE cbRemainCardCount=0 ;
	memcpy(cbRemainCard, cbHandCardData, cbHandCardCount) ;
	SortCardList(cbRemainCard, cbHandCardCount, ST_ORDER) ;

	//�Ƴ��˿�
	if(cbWantOutCardCount!=0) RemoveCard(cbWantOutCardData, cbWantOutCardCount, cbRemainCard, cbHandCardCount) ;
	cbRemainCardCount = cbHandCardCount-cbWantOutCardCount ;

	//����ָ��
	typedef void (CAndroidAI::*pGetAllCardFun)(BYTE const [], BYTE const , BYTE[], BYTE &);

	//ָ������
	pGetAllCardFun GetAllCardFunArray[4] ;
	//void (*p)(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbBomCardData[], BYTE &cbBomCardCount) = GetAllBomCard;
	GetAllCardFunArray[0] = &CAndroidAI::GetAllBomCard;		//ը������
	GetAllCardFunArray[1] = &CAndroidAI::GetAllLineCard ;	//˳�Ӻ���
	GetAllCardFunArray[2] = &CAndroidAI::GetAllThreeCard ;	//������
	GetAllCardFunArray[3] = &CAndroidAI::GetAllDoubleCard ;	//���Ӻ���

	//ָ�������±�
	BYTE cbIndexArray[4] = {0,1,2,3} ;
	//���н��
	BYTE cbPermutationRes[24][4] ;
	BYTE cbLen=0 ;
	//��������
	Permutation(cbIndexArray, 0, 4, cbPermutationRes, cbLen) ;
	assert(cbLen==24) ;
	if(cbLen!=24) return MAX_COUNT + 5 ;

	//������Ŀ
	BYTE cbMinSingleCardCount = MAX_COUNT + 5 ;
	//������Сֵ
	for(BYTE i=0; i<24; ++i)
	{
		//�������
		BYTE cbTmpCardData[MAX_COUNT] ;
		BYTE cbTmpCardCount = cbRemainCardCount ;
		memcpy(cbTmpCardData, cbRemainCard, cbRemainCardCount) ;

		for(BYTE j=0; j<4; ++j)
		{
			BYTE Index = cbPermutationRes[i][j] ;

			//У���±�
			assert(Index>=0 && Index<4) ;
			if(Index<0 || Index>=4) return MAX_COUNT + 5 ;

			pGetAllCardFun pTmpGetAllCardFun = GetAllCardFunArray[Index] ;

			//��ȡ�˿�
			BYTE cbGetCardData[MAX_COUNT] ;
			BYTE cbGetCardCount=0 ;
			//��Ա����
			((*this).*pTmpGetAllCardFun)(cbTmpCardData, cbTmpCardCount, cbGetCardData, cbGetCardCount) ;

			//ɾ���˿�
			if(cbGetCardCount!=0) RemoveCard(cbGetCardData, cbGetCardCount, cbTmpCardData, cbTmpCardCount) ;
			cbTmpCardCount -= cbGetCardCount ;
		}

		//���㵥��
		BYTE cbSingleCard[MAX_COUNT] ;
		BYTE cbSingleCardCount=0 ;
		GetAllSingleCard(cbTmpCardData, cbTmpCardCount, cbSingleCard, cbSingleCardCount) ;
		if(cbMinSingleCardCount> cbSingleCardCount)
		{
			cbMinSingleCardCount = cbSingleCardCount ;

			//���浥��
			if(cbSingleCardData!=NULL)
			{
				memcpy(cbSingleCardData, cbSingleCard, cbSingleCardCount) ;
			}
		}
	}

	//������ж�
	if (cbWantOutCardCount > 0)
	{
		//��������
		BYTE cbCardType = GetCardType(cbWantOutCardData, cbWantOutCardCount) ;
		if (cbCardType == CT_THREE_LINE_TAKE_ONE || cbCardType == CT_THREE_LINE_TAKE_TWO)

		for (BYTE i = 3; i < cbWantOutCardCount; ++i)
		{
			if (GetCardLogicValue(cbWantOutCardData[i]) >= 14) cbMinSingleCardCount += 3 ;
		}
	}

	//�������ж�
	if ( GetCardType(cbWantOutCardData, cbWantOutCardCount) == CT_DOUBLE )
	{
		BYTE cbAllThreeCardData[MAX_COUNT], cbAllThreeCount ;
		BYTE cbAllLineCardData[MAX_COUNT], cbAllLineCount ;

		GetAllThreeCard(cbHandCardData, cbHandCardCount, cbAllThreeCardData, cbAllThreeCount) ;
		GetAllLineCard( cbHandCardData, cbHandCardCount, cbAllLineCardData, cbAllLineCount ) ;

		BYTE cbThreeCardValue = 0 ;
		for ( BYTE i = 0; i < cbAllThreeCount; ++i)
			for ( BYTE j = 0; j < cbWantOutCardCount; ++j)
				if ( GetCardLogicValue(cbWantOutCardData[j]) == GetCardLogicValue(cbAllThreeCardData[i]) )
				{
					cbThreeCardValue = GetCardValue( cbWantOutCardData[j] ) ;
					break ;
				}

		//�Ƿ���l��
		bool bInLineCard = false ;
		for ( BYTE cbLineCardIndex = 0; cbLineCardIndex < cbAllLineCount; ++cbLineCardIndex )
			if ( GetCardValue( cbAllLineCardData[cbLineCardIndex] ) == cbThreeCardValue )
			{
				bInLineCard = true ;
				break ;
			}
		if ( !bInLineCard && cbThreeCardValue != 0 ) cbMinSingleCardCount += 2 ;
	}

	//��ը�ж�
	if(cbWantOutCardCount!=0)
	{
		//ը���˿�
		BYTE cbBombCard[20] ;
		BYTE cbBombCardCount=0;
		GetAllBomCard(cbHandCardData, cbHandCardCount, cbBombCard, cbBombCardCount) ;

		//��������
		BYTE cbCardType = GetCardType(cbWantOutCardData, cbWantOutCardCount) ;

		if(cbBombCardCount>0 && cbCardType<CT_BOMB_CARD)
		{
			//Ѱ����ͬ
			for(BYTE i = GetCardColor(cbBombCard[0]) == 4 ? 2 : 0; i<cbBombCardCount; i += 4)
				for(BYTE j=0; j<cbWantOutCardCount; ++j)
				{
					if(GetCardValue( cbBombCard[i] ) == GetCardValue( cbWantOutCardData[j] ) && GetCardLogicValue(cbWantOutCardData[j])<15 && 
						cbCardType!=CT_SINGLE_LINE && cbCardType!=CT_DOUBLE_LINE) 
						return MAX_COUNT + 5 ; //����ը��

					else if ( GetCardValue( cbBombCard[i] ) == GetCardValue( cbWantOutCardData[j] ) && GetCardLogicValue(cbWantOutCardData[j])<15 && 
						( cbCardType == CT_SINGLE_LINE || cbCardType == CT_DOUBLE_LINE) ) 
						cbMinSingleCardCount += 2 ; //����ը��
				}

			//���ը���ж�
			if (cbCardType == CT_SINGLE_LINE)
			{
				BYTE cbBombCount = 0 ;

				for (BYTE i = GetCardColor(cbBombCard[0]) == 4 ? 2 : 0; i < cbBombCardCount; i += 4)
					for (BYTE j=0; j < cbWantOutCardCount; ++j)
						if ( GetCardValue( cbBombCard[i] ) == GetCardValue( cbWantOutCardData[j] ) ) ++cbBombCount ;

				if (cbBombCount >= 2) return  MAX_COUNT ; //����ը��

				//�������
				tagAnalyseResult AnalyseResult;
				AnalysebCardData(cbHandCardData, cbHandCardCount,AnalyseResult) ;

                BYTE cbThreeCount = 0 ;

				for (BYTE i = 0; i < AnalyseResult.cbThreeCount; ++i)
					for (BYTE j = 0; j < cbWantOutCardCount; ++j)
						if (GetCardValue(cbWantOutCardData[j]) == GetCardValue(AnalyseResult.cbThreeCardData[3 * i])) ++cbThreeCount ;

				if (cbThreeCount + cbBombCount >= 2) return  MAX_COUNT + 5 ;

			}
		}
	}

	return cbMinSingleCardCount ;
}

//�����˿�
void CAndroidAI::SetUserCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount)
{
	memcpy(m_cbAllCardData[wChairID], cbCardData, cbCardCount*sizeof(BYTE)) ;
	m_cbUserCardCount[wChairID] = cbCardCount ;

	//�����˿�
	SortCardList(m_cbAllCardData[wChairID], cbCardCount, ST_ORDER) ;
}

//���õ���
void CAndroidAI::SetBackCard(WORD wChairID, BYTE cbBackCardData[], BYTE cbCardCount)
{
	BYTE cbTmpCount = m_cbUserCardCount[wChairID] ;
	memcpy(m_cbAllCardData[wChairID]+cbTmpCount, cbBackCardData, cbCardCount*sizeof(BYTE)) ;
	m_cbUserCardCount[wChairID] += cbCardCount ;

	//�����˿�
	SortCardList(m_cbAllCardData[wChairID], m_cbUserCardCount[wChairID], ST_ORDER) ;
}

//����ׯ��
void CAndroidAI::SetBanker(WORD wBanker) 
{
	m_wBankerUser = wBanker ;
	m_lBankerOutCardCount = 0 ;
}
//�����˿�
void CAndroidAI::SetLandScoreCardData(BYTE cbCardData[], BYTE cbCardCount) 
{
	assert(cbCardCount==MAX_COUNT) ;
	if(cbCardCount!=MAX_COUNT) return ;

	memcpy(m_cbLandScoreCardData, cbCardData, cbCardCount*sizeof(BYTE)) ;
	//�����˿�
	SortCardList(m_cbLandScoreCardData, cbCardCount, ST_ORDER) ;
}
//ɾ���˿�
void CAndroidAI::RemoveUserCardData(WORD wChairID, BYTE cbRemoveCardData[], BYTE cbRemoveCardCount) 
{
	bool bSuccess = RemoveCard(cbRemoveCardData, cbRemoveCardCount, m_cbAllCardData[wChairID], m_cbUserCardCount[wChairID]) ;
	assert(bSuccess) ;
	m_cbUserCardCount[wChairID] -= cbRemoveCardCount ;

}

//������ƣ��ȳ��ƣ�
void CAndroidAI::BankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, tagOutCardResult & OutCardResult) 
{
	//���±�û��
	tagOutCardTypeResult CardTypeResult[12+1] ;
	memset(CardTypeResult, '\0',sizeof(CardTypeResult)) ;

	//��ʼ��
	memset(&OutCardResult, '\0',sizeof(OutCardResult)) ;

	BYTE cbLineCard[MAX_COUNT] ;
	BYTE cbThreeLineCard[MAX_COUNT] ;
	BYTE cbDoubleLineCard[MAX_COUNT] ;
	BYTE cbLineCardCount;
	BYTE cbThreeLineCardCount ;
	BYTE cbDoubleLineCount ;
	GetAllLineCard(cbHandCardData, cbHandCardCount, cbLineCard, cbLineCardCount) ;
	GetAllThreeCard(cbHandCardData, cbHandCardCount, cbThreeLineCard, cbThreeLineCardCount) ;
	GetAllDoubleCard(cbHandCardData, cbHandCardCount, cbDoubleLineCard, cbDoubleLineCount) ;

	WORD wUndersideOfBanker = (m_wBankerUser+1)%GAME_PLAYER ;	//�����¼�
	WORD wUpsideOfBanker = (wUndersideOfBanker+1)%GAME_PLAYER ;	//�����ϼ�

	//�жϿɷ����
	BYTE cbSingleCardCount = MAX_COUNT+CT_MISSILE_CARD ;
	bool bFindBestCard = false ;
	AnalyseOutCardType(cbHandCardData, cbHandCardCount, CardTypeResult) ;
	for(BYTE cbCardType=CT_SINGLE; cbCardType<=CT_MISSILE_CARD; ++cbCardType)
		if(CardTypeResult[cbCardType].cbCardTypeCount>0)
		{
			for(LONG lIndex=0; lIndex<CardTypeResult[cbCardType].cbCardTypeCount; ++lIndex)
			{
				if(TestOutAllCard(m_wBankerUser, CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex], true))
				{
					//���㵥��
					BYTE cbTmpSingleCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CardTypeResult[cbCardType].cbCardData[lIndex], 
						CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]) ;

					//����ж�
					if (cbTmpSingleCount >= MAX_COUNT) continue ;

					//ը�����ȼ��ź�
					BYTE cbBombCardType = GetCardType(CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]) ;
					if (cbBombCardType == CT_BOMB_CARD) cbTmpSingleCount += 4 ;
					else if (cbBombCardType == CT_MISSILE_CARD) cbTmpSingleCount += 5 ;
					else if ( 15 == GetCardLogicValue( CardTypeResult[ cbCardType ].cbCardData[ lIndex ][ 0 ] ) ) cbTmpSingleCount += 2;
					else if ( 15 < GetCardLogicValue( CardTypeResult[ cbCardType ].cbCardData[ lIndex ][ 0 ] ) ) cbTmpSingleCount += 3;


					////�ı�Ȩֵ
					//if (cbBombCardType != CT_ERROR) cbTmpSingleCount += cbBombCardType ;
					
					if (cbTmpSingleCount <= cbSingleCardCount)
					{
						//���ñ�
						OutCardResult.cbCardCount=CardTypeResult[cbCardType].cbEachHandCardCount[lIndex];
						memcpy(OutCardResult.cbResultCard, CardTypeResult[cbCardType].cbCardData[lIndex],
						CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]*sizeof(BYTE));
						cbSingleCardCount = cbTmpSingleCount ;
						bFindBestCard = true ;
					}

				}
			}
		}
		//ֱ�ӷ���
		if (bFindBestCard) return ;

	//�����}��
	if ( cbHandCardCount == 4 && GetCardLogicValue(cbHandCardData[1]) == 16 && m_cbUserCardCount[wUndersideOfBanker] == 1 && 
		GetCardLogicValue(cbHandCardData[2]) < GetCardLogicValue(m_cbAllCardData[wUndersideOfBanker][0]))
	{
		OutCardResult.cbCardCount = 1 ;
		OutCardResult.cbResultCard[0] = cbHandCardData[2] ;
		return ;
	}

	//�Ĵ������ж�
	if ( AnalyseFourCardType(cbHandCardData, cbHandCardCount, m_cbAllCardData[wUndersideOfBanker], m_cbUserCardCount[wUndersideOfBanker], OutCardResult ) )
	{
		return ;
	}

	//�����}��
	if ( cbHandCardCount == 4 && GetCardLogicValue(cbHandCardData[1]) == 16 && m_cbUserCardCount[wUpsideOfBanker] == 1 && 
		GetCardLogicValue(cbHandCardData[2]) < GetCardLogicValue(m_cbAllCardData[wUpsideOfBanker][0]))
	{
		OutCardResult.cbCardCount = 1 ;
		OutCardResult.cbResultCard[0] = cbHandCardData[2] ;
		return ;
	}

	//�Ĵ������ж�
	if ( AnalyseFourCardType(cbHandCardData, cbHandCardCount, m_cbAllCardData[wUpsideOfBanker], m_cbUserCardCount[wUpsideOfBanker], OutCardResult ) )
	{
		return ;
	}

	//���ֻʣ˳�ƺ͵�ֻ�����ȳ�˳
	{
		if(cbLineCardCount+1==cbHandCardCount && CT_SINGLE==GetCardType(cbLineCard, cbLineCardCount))
		{
			OutCardResult.cbCardCount = cbLineCardCount ;
			memcpy(OutCardResult.cbResultCard, cbLineCard, cbLineCardCount) ;
		}
		else if(cbThreeLineCardCount+1==cbHandCardCount && CT_THREE_LINE==GetCardType(cbThreeLineCard, cbThreeLineCardCount))
		{
			OutCardResult.cbCardCount = cbThreeLineCardCount ;
			memcpy(OutCardResult.cbResultCard, cbThreeLineCard, cbThreeLineCardCount) ;
		}
		else if(cbDoubleLineCount+1==cbHandCardCount && CT_DOUBLE_LINE==GetCardType(cbDoubleLineCard, cbDoubleLineCount))
		{
			OutCardResult.cbCardCount = cbDoubleLineCount ;
			memcpy(OutCardResult.cbResultCard, cbDoubleLineCard, cbDoubleLineCount) ;
		}
		//˫��ը����һ��
		else if(cbHandCardCount>2 && cbHandCardData[0]==0x4f && cbHandCardData[1]==0x4e && CT_ERROR!=GetCardType(cbHandCardData+2, cbHandCardCount-2) &&
			GetCardType(cbHandCardData+2, cbHandCardCount-2) !=CT_FOUR_LINE_TAKE_ONE && 
			GetCardType(cbHandCardData+2, cbHandCardCount-2) !=CT_FOUR_LINE_TAKE_TWO)
		{
			OutCardResult.cbCardCount = 2 ;
			OutCardResult.cbResultCard[0] = 0x4f ;
			OutCardResult.cbResultCard[1] = 0x4e ;
		}

		if(OutCardResult.cbCardCount>0)
		{
			return ;
		}
	}

	//�����һֻ
	if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
	{
		OutCardResult.cbCardCount = 2 ;
		OutCardResult.cbResultCard[0] = 0x4f ;
		OutCardResult.cbResultCard[1] = 0x4e ;
		return ;
	}
	//����
	else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
	{
		OutCardResult.cbCardCount = 2 ;
		OutCardResult.cbResultCard[0] = 0x4f ;
		OutCardResult.cbResultCard[1] = 0x4e ;
		return ;
	}
	//ֻʣһ����
	else if(CT_ERROR!=GetCardType(cbHandCardData, cbHandCardCount) && CT_FOUR_LINE_TAKE_ONE!=GetCardType(cbHandCardData, cbHandCardCount) && CT_FOUR_LINE_TAKE_TWO!=GetCardType(cbHandCardData, cbHandCardCount))
	{
		OutCardResult.cbCardCount = cbHandCardCount ;
		memcpy(OutCardResult.cbResultCard, cbHandCardData, cbHandCardCount) ;
		return ;
	}

	//ֻʣһ�ź�һ��
	if(cbHandCardCount>=2)
	{
		//�ϼ��˿�
		tagOutCardTypeResult UpsideCanOutCardType1[13] ;
		memset(UpsideCanOutCardType1,'\0', sizeof(UpsideCanOutCardType1)) ;
		tagOutCardTypeResult UpsideCanOutCardType2[13] ;
		memset(UpsideCanOutCardType2, '\0',sizeof(UpsideCanOutCardType2)) ;

		//�¼��˿�
		tagOutCardTypeResult UndersideCanOutCardType1[13] ;
		memset(UndersideCanOutCardType1, '\0',sizeof(UndersideCanOutCardType1)) ;
		tagOutCardTypeResult UndersideCanOutCardType2[13] ;
		memset(UndersideCanOutCardType2,'\0', sizeof(UndersideCanOutCardType2)) ;

		BYTE cbFirstHandCardType = GetCardType(cbHandCardData, cbHandCardCount-1) ;
		BYTE cbSecondHandCardType = GetCardType(cbHandCardData+1, cbHandCardCount-1) ;

		//�Ƿ���ը
		BYTE cbAllBombCardData[MAX_COUNT], cbAllBombCount=0 ;
		GetAllBomCard(cbHandCardData, cbHandCardCount, cbAllBombCardData, cbAllBombCount) ;

		//û��ը��
		if (cbAllBombCount <= 0 && cbFirstHandCardType!=CT_THREE_LINE_TAKE_ONE && cbFirstHandCardType!=CT_THREE_LINE_TAKE_TWO)
		{
		if(CT_ERROR!=cbFirstHandCardType && cbFirstHandCardType!=CT_FOUR_LINE_TAKE_ONE && cbFirstHandCardType!= CT_FOUR_LINE_TAKE_TWO)
		{
			AnalyseOutCardType(m_cbAllCardData[wUpsideOfBanker], m_cbUserCardCount[wUpsideOfBanker], cbHandCardData, cbHandCardCount-1, UpsideCanOutCardType1) ;
			AnalyseOutCardType(m_cbAllCardData[wUndersideOfBanker], m_cbUserCardCount[wUndersideOfBanker], cbHandCardData, cbHandCardCount-1, UndersideCanOutCardType1) ;
		}
		if(CT_ERROR!=cbSecondHandCardType && cbSecondHandCardType!=CT_FOUR_LINE_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_LINE_TAKE_TWO)
		{
			AnalyseOutCardType(m_cbAllCardData[wUpsideOfBanker], m_cbUserCardCount[wUpsideOfBanker], cbHandCardData+1, cbHandCardCount-1, UpsideCanOutCardType2) ;
			AnalyseOutCardType(m_cbAllCardData[wUndersideOfBanker], m_cbUserCardCount[wUndersideOfBanker], cbHandCardData+1, cbHandCardCount-1, UndersideCanOutCardType2) ;
		}

		if(cbSecondHandCardType!=CT_ERROR && cbSecondHandCardType!=CT_FOUR_LINE_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_LINE_TAKE_TWO && UpsideCanOutCardType2[cbSecondHandCardType].cbCardTypeCount==0 && UndersideCanOutCardType2[cbSecondHandCardType].cbCardTypeCount==0 && 
			UpsideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0 && UndersideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
		{
			OutCardResult.cbCardCount = cbHandCardCount-1 ;
			memcpy(OutCardResult.cbResultCard, cbHandCardData+1, cbHandCardCount-1) ;
			return ;
		}

		if(cbFirstHandCardType!=CT_ERROR && cbFirstHandCardType!=CT_FOUR_LINE_TAKE_ONE && cbFirstHandCardType!= CT_FOUR_LINE_TAKE_TWO && UpsideCanOutCardType1[cbFirstHandCardType].cbCardTypeCount==0 && UndersideCanOutCardType1[cbFirstHandCardType].cbCardTypeCount==0 &&
			UpsideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0 && UndersideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
		{
			OutCardResult.cbCardCount = cbHandCardCount-1 ;
			memcpy(OutCardResult.cbResultCard, cbHandCardData, cbHandCardCount-1) ;
			return ;
		}

		if(GetCardLogicValue(cbHandCardData[0])>=GetCardLogicValue(m_cbAllCardData[wUpsideOfBanker][0]) &&
			GetCardLogicValue(cbHandCardData[0])>=GetCardLogicValue(m_cbAllCardData[wUndersideOfBanker][0]) &&
			CT_ERROR!=cbSecondHandCardType && cbSecondHandCardType!=CT_FOUR_LINE_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_LINE_TAKE_TWO &&
			UpsideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0 && UndersideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
		{
			OutCardResult.cbCardCount = 1 ;
			OutCardResult.cbResultCard[0] = cbHandCardData[0] ;
			return ;
		}

		if(CT_ERROR!=cbSecondHandCardType && cbSecondHandCardType!=CT_FOUR_LINE_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_LINE_TAKE_TWO &&
			UpsideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0 && UndersideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
		{
			OutCardResult.cbCardCount = cbHandCardCount-1 ;
			memcpy(OutCardResult.cbResultCard, cbHandCardData+1, cbHandCardCount-1) ;
			return ;
		}
		}
		//����ը��
		else
		{
			//��ȥը�����
			BYTE cbRemainCard[MAX_COUNT], cbRemainCount=0 ;
			memcpy(cbRemainCard, cbHandCardData, cbHandCardCount) ;
			cbRemainCount = cbHandCardCount ;
			RemoveCard(cbAllBombCardData, cbAllBombCount, cbRemainCard, cbRemainCount) ;
			cbRemainCount -= cbAllBombCount ;
			if (GetCardType(cbRemainCard, cbRemainCount) != CT_ERROR)
			{
				OutCardResult.cbCardCount = cbRemainCount ;
				memcpy(OutCardResult.cbResultCard, cbRemainCard, cbRemainCount) ;
				return ;
			}
		}
	}

	{
		{
			//�����˿�
			tagOutCardTypeResult MeOutCardTypeResult[13] ;
			memset(MeOutCardTypeResult, '\0',sizeof(MeOutCardTypeResult)) ;
			AnalyseOutCardType(cbHandCardData, cbHandCardCount, MeOutCardTypeResult) ;

			//���㵥��
			BYTE cbMinSingleCardCount[4] ;
			cbMinSingleCardCount[0]=MAX_COUNT ;
			cbMinSingleCardCount[1]=MAX_COUNT ;
			cbMinSingleCardCount[2]=MAX_COUNT ;
			cbMinSingleCardCount[3]=MAX_COUNT ;
			BYTE cbIndex[4]={0} ;
			BYTE cbOutcardType[4]={CT_ERROR} ;
			BYTE cbMinValue=MAX_COUNT ; 
			BYTE cbMinSingleCountInFour=MAX_COUNT ;
			BYTE cbMinCardType=CT_ERROR ;
			BYTE cbMinIndex=0 ;

			//��ը�������
			for(BYTE cbCardType=CT_DOUBLE; cbCardType<CT_BOMB_CARD; ++cbCardType)
			{

				tagOutCardTypeResult const &tmpCardResult = MeOutCardTypeResult[cbCardType] ;

				//��ͬ���ͣ���ͬ���ȣ���l����l����ͬ���Ϳ��ܳ��Ȳ�һ��
				BYTE cbThisHandCardCount = MAX_COUNT ;

				//�ϼ��˿�
				tagOutCardTypeResult UpsideOutCardTypeResult[13] ;
				memset(UpsideOutCardTypeResult, '\0',sizeof(UpsideOutCardTypeResult)) ;

				//�¼��˿�
				tagOutCardTypeResult UndersideOutCardTypeResult[13] ;
				memset(UndersideOutCardTypeResult,'\0', sizeof(UndersideOutCardTypeResult)) ;


				for(BYTE i=0; i<tmpCardResult.cbCardTypeCount; ++i)
				{
					//�������ж�
					if ( cbCardType == CT_DOUBLE )
					{
						tagAnalyseResult AnalyseResult ;
						memset( &AnalyseResult, '\0', sizeof( AnalyseResult ) ) ;
						AnalysebCardData( cbHandCardData, cbHandCardCount, AnalyseResult ) ;
						if ( AnalyseResult.cbSignedCount + AnalyseResult.cbThreeCount * 3 == cbHandCardCount )
						{
							bool bContinue = false ;
							for ( BYTE cbThreeIndex = 0; cbThreeIndex < AnalyseResult.cbThreeCount; ++cbThreeIndex )
								if ( GetCardValue(  tmpCardResult.cbCardData[i][0] ) == GetCardValue( AnalyseResult.cbThreeCardData[3 * cbThreeIndex] ) )
								{
									bContinue = true ;
									break ;
								}
							if ( bContinue ) continue ;
						}
					}

					BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i]) ;

					//���·���
					if(tmpCardResult.cbEachHandCardCount[i]!=cbThisHandCardCount)
					{
						cbThisHandCardCount = tmpCardResult.cbEachHandCardCount[i] ;
						AnalyseOutCardType(m_cbAllCardData[wUpsideOfBanker], m_cbUserCardCount[wUpsideOfBanker], 
							tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i] ,UpsideOutCardTypeResult) ;
						AnalyseOutCardType(m_cbAllCardData[wUndersideOfBanker], m_cbUserCardCount[wUndersideOfBanker], 
							tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i] ,UndersideOutCardTypeResult) ;
					}
					BYTE cbMaxValue=0 ; 
					BYTE Index = 0 ;

					//���˳�ӣ�����Ĵ���
					BYTE cbCurrentCardType = GetCardType(tmpCardResult.cbCardData[i], cbThisHandCardCount) ;
					if (cbThisHandCardCount != cbHandCardCount && cbCurrentCardType >= CT_SINGLE_LINE && cbCurrentCardType <= CT_THREE_LINE_TAKE_TWO &&
						( GetCardLogicValue(tmpCardResult.cbCardData[i][cbThisHandCardCount-1]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-2]) || 
						GetCardLogicValue(tmpCardResult.cbCardData[i][0]) >= 11 ) )
					{
						BYTE cbRemainCardData[MAX_COUNT], cbRemainCardCount ;
						memcpy(cbRemainCardData, cbHandCardData, cbHandCardCount) ;
						cbRemainCardCount = cbHandCardCount ;

						//�Ƴ��˿�
						RemoveCard(tmpCardResult.cbCardData[i], cbThisHandCardCount, cbRemainCardData, cbRemainCardCount) ;
						cbRemainCardCount -= cbThisHandCardCount ;

						//����˿�
						BYTE cbCurrentLargestLogicCard = GetCardLogicValue(tmpCardResult.cbCardData[i][0]) ;

						if (GetCardType(cbRemainCardData, cbRemainCardCount) == CT_ERROR && (cbCurrentCardType >= CT_THREE_LINE_TAKE_ONE && 
							cbCurrentCardType <= CT_THREE_LINE_TAKE_TWO && cbCurrentLargestLogicCard >= 11 && cbThisHandCardCount <=5 || 
							cbCurrentCardType == CT_SINGLE_LINE && cbThisHandCardCount <= 6 && cbCurrentLargestLogicCard >= 12 || 
							cbCurrentCardType >= CT_DOUBLE_LINE && cbCurrentCardType <= CT_THREE_LINE && cbCurrentLargestLogicCard >= 12 && cbThisHandCardCount <= 8))
						{
							//��ʱ����
							if ( cbCurrentCardType >= CT_SINGLE_LINE && cbCurrentCardType <= CT_THREE_LINE &&
								GetCardLogicValue(tmpCardResult.cbCardData[i][cbThisHandCardCount - 1]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-3]) )
								continue ;

							if ( cbCurrentCardType >= CT_THREE_LINE_TAKE_ONE && cbCurrentCardType <= CT_THREE_LINE_TAKE_TWO &&
								GetCardLogicValue(tmpCardResult.cbCardData[i][0]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-3]) )
								continue ;
						}
					}

					//��Դ�ԣ������ȳ�
					if (cbCardType == CT_DOUBLE && GetCardLogicValue(tmpCardResult.cbCardData[i][0]) >= 11)
					{
						BYTE cbAllSingleCardData[MAX_COUNT], cbAllSingleCount ;
						cbAllSingleCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, NULL, 0, cbAllSingleCardData) ;
						if (cbAllSingleCount >= 2 && GetCardLogicValue(cbAllSingleCardData[cbAllSingleCount-2]) < 10) continue ;
					}

					//�з�����ѹס��
					if(UpsideOutCardTypeResult[cbCardType].cbCardTypeCount>0 || UndersideOutCardTypeResult[cbCardType].cbCardTypeCount>0)
					{
						//�ϼ��ܵ�
						if(UpsideOutCardTypeResult[cbCardType].cbEachHandCardCount[0] > 0 && m_cbUserCardCount[wUpsideOfBanker]<=UpsideOutCardTypeResult[cbCardType].cbEachHandCardCount[0]+1)
							continue ;

						//�¼��ܵ�
						if(UndersideOutCardTypeResult[cbCardType].cbEachHandCardCount[0] > 0 && m_cbUserCardCount[wUndersideOfBanker]<=UndersideOutCardTypeResult[cbCardType].cbEachHandCardCount[0]+1)
							continue ;

						//�Լ����������û���Ȩ
						//if(UpsideOutCardTypeResult[cbCardType].cbCardTypeCount > 0 && GetCardLogicValue(tmpCardResult.cbCardData[0][0]) < GetCardLogicValue(UpsideOutCardTypeResult[cbCardType].cbCardData[0][0]) ||
						//	UpsideOutCardTypeResult[cbCardType].cbCardTypeCount > 0 && GetCardLogicValue(tmpCardResult.cbCardData[0][0]) < GetCardLogicValue(UpsideOutCardTypeResult[cbCardType].cbCardData[0][0])) 
						//	continue ;
					}
					//�Ƿ��д��� 
					if(tmpCardResult.cbEachHandCardCount[i] != cbHandCardCount)
					{
						bool bHaveLargeCard=false ;
						for(BYTE j=0; j<tmpCardResult.cbEachHandCardCount[i]; ++j)
						{
							if(GetCardLogicValue(tmpCardResult.cbCardData[i][j])>=15) bHaveLargeCard=true ;
							if(cbCardType!=CT_SINGLE_LINE && cbCardType!=CT_DOUBLE_LINE  && GetCardLogicValue(tmpCardResult.cbCardData[i][0])==14) bHaveLargeCard=true ; 
						}

						if(bHaveLargeCard)
							continue ;
					}

					//����cbMinSingleCardCount[4]�����ֵ
					for(BYTE j=0; j<4; ++j)
					{
						if(cbMinSingleCardCount[j]>=cbTmpCount)
						{
							cbMinSingleCardCount[j] = cbTmpCount ;
							cbIndex[j] = i ;
							cbOutcardType[j] = cbCardType ;
							break ;
						}
					}

					//������Сֵ
					if(cbMinSingleCountInFour>=cbTmpCount)
					{
						//��С����
						cbMinCardType = cbCardType ;
						//��С�����е���С����
						cbMinSingleCountInFour=cbTmpCount ;						
						//��С�����е���С��
						cbMinIndex=i ;
					}
				}
			}

			if(cbMinSingleCountInFour>=AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, NULL, 0)+3 && 
				(m_cbUserCardCount[wUndersideOfBanker]>=4 && m_cbUserCardCount[wUpsideOfBanker]>=4))
				cbMinSingleCountInFour=MAX_COUNT ;

			if(cbMinSingleCountInFour!=MAX_COUNT)
			{
				BYTE Index = cbMinIndex ;

				//ѡ����С��
				for(BYTE i=0; i<4; ++i)
				{
					if(cbOutcardType[i]==cbMinCardType && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
						GetCardLogicValue(MeOutCardTypeResult[cbMinCardType].cbCardData[cbIndex[i]][0])<GetCardLogicValue(MeOutCardTypeResult[cbMinCardType].cbCardData[Index][0]))
						Index = cbIndex[i] ;
				}

				//�����һֻ
				if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
				{
					OutCardResult.cbCardCount = 2 ;
					OutCardResult.cbResultCard[0] = 0x4f ;
					OutCardResult.cbResultCard[1] = 0x4e ;
					return ;
				}
				//����
				else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
				{
					OutCardResult.cbCardCount = 2 ;
					OutCardResult.cbResultCard[0] = 0x4f ;
					OutCardResult.cbResultCard[1] = 0x4e ;
					return ;
				}
				else
				{
					//���ñ�
					OutCardResult.cbCardCount=MeOutCardTypeResult[cbMinCardType].cbEachHandCardCount[Index];
					memcpy(OutCardResult.cbResultCard,MeOutCardTypeResult[cbMinCardType].cbCardData[Index],MeOutCardTypeResult[cbMinCardType].cbEachHandCardCount[Index]*sizeof(BYTE));
					return ;
				}

				assert(OutCardResult.cbCardCount>0) ;

				return ;
			}

			//��û���ҵ��ʺϵ�����Ӵ��С
			if(OutCardResult.cbCardCount<=0 && (m_cbUserCardCount[wUndersideOfBanker] == 1 || m_cbUserCardCount[wUpsideOfBanker] == 1))
			{
				//ֻ��һ����ʱ���ܷ���
				if(MeOutCardTypeResult[CT_SINGLE].cbCardTypeCount>0)
				{
					BYTE Index=MAX_COUNT ;
					for(BYTE i=0; i<MeOutCardTypeResult[CT_SINGLE].cbCardTypeCount; ++i)
					{
						if((m_cbUserCardCount[wUndersideOfBanker] == 1 && GetCardLogicValue(MeOutCardTypeResult[CT_SINGLE].cbCardData[i][0])>=GetCardLogicValue(m_cbAllCardData[wUndersideOfBanker][0])) ||
							(m_cbUserCardCount[wUpsideOfBanker] == 1 && GetCardLogicValue(MeOutCardTypeResult[CT_SINGLE].cbCardData[i][0])>=GetCardLogicValue(m_cbAllCardData[wUpsideOfBanker][0])))
						{
							Index=i ;
						}
						else break ;
					}

					if(MAX_COUNT!=Index)
					{
						OutCardResult.cbCardCount = MeOutCardTypeResult[CT_SINGLE].cbEachHandCardCount[Index] ;
						memcpy(OutCardResult.cbResultCard, MeOutCardTypeResult[CT_SINGLE].cbCardData[Index], OutCardResult.cbCardCount) ;
						return ;
					}
				}
			}
		}
	}
	BYTE cbFirstCard=0 ;
	//�������2
	for(BYTE i=0; i<cbHandCardCount; ++i) 
		if(GetCardLogicValue(cbHandCardData[i])<15)
		{
			cbFirstCard = i ;
			break ;
		}

		if(cbFirstCard<cbHandCardCount-1)
			AnalyseOutCardType(cbHandCardData+cbFirstCard, cbHandCardCount-cbFirstCard, CardTypeResult) ;
		else
			AnalyseOutCardType(cbHandCardData, cbHandCardCount, CardTypeResult) ;

		//���㵥��
		BYTE cbMinSingleCardCount[4] ;
		cbMinSingleCardCount[0]=MAX_COUNT ;
		cbMinSingleCardCount[1]=MAX_COUNT ;
		cbMinSingleCardCount[2]=MAX_COUNT ;
		cbMinSingleCardCount[3]=MAX_COUNT ;
		BYTE cbIndex[4]={0} ;
		BYTE cbOutcardType[4]={CT_ERROR} ;
		BYTE cbMinValue=MAX_COUNT ; 
		BYTE cbMinSingleCountInFour=MAX_COUNT ;
		BYTE cbMinCardType=CT_ERROR ;
		BYTE cbMinIndex=0 ;

		//��ը�������
		for(BYTE cbCardType=CT_SINGLE; cbCardType<CT_BOMB_CARD; ++cbCardType)
		{
			tagOutCardTypeResult const &tmpCardResult = CardTypeResult[cbCardType] ;
			for(BYTE i=0; i<tmpCardResult.cbCardTypeCount; ++i)
			{
				//�мҿ����ߵ�
				if ( CompareCard(tmpCardResult.cbCardData[i], m_cbAllCardData[wUndersideOfBanker], tmpCardResult.cbEachHandCardCount[i], m_cbUserCardCount[wUndersideOfBanker]) ||
					CompareCard(tmpCardResult.cbCardData[i],  m_cbAllCardData[wUpsideOfBanker], tmpCardResult.cbEachHandCardCount[i],m_cbUserCardCount[wUpsideOfBanker]))
					continue ;

				//���˳�ӣ�����Ĵ���
				if ( tmpCardResult.cbEachHandCardCount[i] != cbHandCardCount && cbCardType >= CT_SINGLE_LINE && cbCardType <= CT_THREE_LINE_TAKE_TWO &&
					( GetCardLogicValue(tmpCardResult.cbCardData[i][tmpCardResult.cbEachHandCardCount[i]-1]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-2]) || 
						GetCardLogicValue(tmpCardResult.cbCardData[i][0]) >= 11 ))
				{
					BYTE cbRemainCardData[MAX_COUNT], cbRemainCardCount ;
					memcpy(cbRemainCardData, cbHandCardData, cbHandCardCount) ;
					cbRemainCardCount = cbHandCardCount ;

					//�Ƴ��˿�
					RemoveCard(tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i], cbRemainCardData, cbRemainCardCount) ;
					cbRemainCardCount -= tmpCardResult.cbEachHandCardCount[i] ;

					//����˿�
					BYTE cbCurrentLargestLogicCard = GetCardLogicValue(tmpCardResult.cbCardData[i][0]) ;

					if (GetCardType(cbRemainCardData, cbRemainCardCount) == CT_ERROR && (cbCardType >= CT_THREE_LINE_TAKE_ONE && 
						cbCardType <= CT_THREE_LINE_TAKE_TWO && cbCurrentLargestLogicCard >= 11 && tmpCardResult.cbEachHandCardCount[i] <=5 || 
						cbCardType == CT_SINGLE_LINE && tmpCardResult.cbEachHandCardCount[i] <= 6 && cbCurrentLargestLogicCard >= 12 || 
						cbCardType >= CT_DOUBLE_LINE && cbCardType <= CT_THREE_LINE && cbCurrentLargestLogicCard >= 12 && tmpCardResult.cbEachHandCardCount[i] <= 8))
					{
						//��ʱ����
						if ( cbCardType >= CT_SINGLE_LINE && cbCardType <= CT_THREE_LINE &&
							GetCardLogicValue(tmpCardResult.cbCardData[i][tmpCardResult.cbEachHandCardCount[i] - 1]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-3]) )
							continue ;

						if ( cbCardType >= CT_THREE_LINE_TAKE_ONE && cbCardType <= CT_THREE_LINE_TAKE_TWO &&
							GetCardLogicValue(tmpCardResult.cbCardData[i][0]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-3]) )
							continue ;
					}
				}

				BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i]) ;

				BYTE cbMaxValue=0 ; 
				BYTE Index = 0 ;

				//����cbMinSingleCardCount[4]�����ֵ
				for(BYTE j=0; j<4; ++j)
				{
					if(cbMinSingleCardCount[j]>=cbTmpCount)
					{
						cbMinSingleCardCount[j] = cbTmpCount ;
						cbIndex[j] = i ;
						cbOutcardType[j] = cbCardType ;
						break ;
					}
				}

				//������Сֵ
				if(cbMinSingleCountInFour>=cbTmpCount)
				{
					//��С����
					cbMinCardType = cbCardType ;
					//��С�����е���С����
					cbMinSingleCountInFour=cbTmpCount ;						
					//��С�����е���С��
					cbMinIndex=i ;
				}
			}
		}

		if(cbMinSingleCountInFour!=MAX_COUNT)
		{
			BYTE Index = cbMinIndex ;

			//ѡ����С��
			for(BYTE i=0; i<4; ++i)
			{
				if(cbOutcardType[i]==cbMinCardType && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
					GetCardLogicValue(CardTypeResult[cbMinCardType].cbCardData[cbIndex[i]][0])<GetCardLogicValue(CardTypeResult[cbMinCardType].cbCardData[Index][0]))
					Index = cbIndex[i] ;
			}

			//�����һֻ
			if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
			{
				OutCardResult.cbCardCount = 2 ;
				OutCardResult.cbResultCard[0] = 0x4f ;
				OutCardResult.cbResultCard[1] = 0x4e ;
				return ;
			}
			//����
			else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
			{
				OutCardResult.cbCardCount = 2 ;
				OutCardResult.cbResultCard[0] = 0x4f ;
				OutCardResult.cbResultCard[1] = 0x4e ;
				return ;
			}
			else
			{
				//���ñ�
				OutCardResult.cbCardCount=CardTypeResult[cbMinCardType].cbEachHandCardCount[Index];
				memcpy(OutCardResult.cbResultCard,CardTypeResult[cbMinCardType].cbCardData[Index],CardTypeResult[cbMinCardType].cbEachHandCardCount[Index]*sizeof(BYTE));
				return ;
			}

			assert(OutCardResult.cbCardCount>0) ;

			return ;
		}
		//���ֻʣը��
		else if (CardTypeResult[CT_BOMB_CARD].cbCardTypeCount > 0) 
		{
			//BYTE Index=0 ;
			//BYTE cbLogicCardValue = GetCardLogicValue(0x4F)+1 ;
			////��Сը��
			//for(BYTE i=0; i<CardTypeResult[CT_BOMB_CARD].cbCardTypeCount; ++i)
			//	if(cbLogicCardValue>GetCardLogicValue(CardTypeResult[CT_BOMB_CARD].cbCardData[i][0]))
			//	{
			//		cbLogicCardValue = GetCardLogicValue(CardTypeResult[CT_BOMB_CARD].cbCardData[i][0]) ;
			//		Index = i ;
			//	}

			//	//���ñ�
			//	OutCardResult.cbCardCount=CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index];
			//	memcpy(OutCardResult.cbResultCard,CardTypeResult[CT_BOMB_CARD].cbCardData[Index],CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index]*sizeof(BYTE));

			//	return ;
		}

		BYTE cbAllSingleCardData[MAX_COUNT], cbAllSingleCardCount ;
		cbAllSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, NULL, 0, cbAllSingleCardData) ;

		if ( cbAllSingleCardCount > 0 )
		{
			//���û������ͳ���С��һ��
			if ( ( 1 == m_cbUserCardCount[wUndersideOfBanker] || 1 == m_cbUserCardCount[wUpsideOfBanker] ) && cbAllSingleCardCount >= 2 ) 
			{
				OutCardResult.cbCardCount = 1 ;
				OutCardResult.cbResultCard[0] = cbAllSingleCardData[cbAllSingleCardCount-2] ;
			}
			else 
			{
				OutCardResult.cbCardCount = 1 ;
				OutCardResult.cbResultCard[0] = cbAllSingleCardData[cbAllSingleCardCount-1] ;
			}
			return ;
		}

		//���û������ͳ���С��һ��
		OutCardResult.cbCardCount = 1 ;
		OutCardResult.cbResultCard[0] = cbHandCardData[cbHandCardCount-1] ;

		return ;
}

//������ƣ�����ƣ�
void CAndroidAI::BankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, WORD wOutCardUser, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult) 
{
	//��ʼ��
	memset(&OutCardResult, '\0',sizeof(OutCardResult)) ;

	//���±�û��
	tagOutCardTypeResult CardTypeResult[12+1] ;
	memset(&CardTypeResult,'\0', sizeof(CardTypeResult)) ;

	//��������
	BYTE cbOutCardType = GetCardType(cbTurnCardData,cbTurnCardCount) ;
	AnalyseOutCardType(cbHandCardData,cbHandCardCount,cbTurnCardData,cbTurnCardCount, CardTypeResult) ;

	WORD wUndersideUser = (m_wBankerUser+1)%GAME_PLAYER ;
	WORD wUpsideUser = (wUndersideUser+1)%GAME_PLAYER ;

	//ֻʣը��
	if(cbHandCardCount==CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[0] && (cbOutCardType<CT_BOMB_CARD || 
		GetCardLogicValue(CardTypeResult[CT_BOMB_CARD].cbCardData[0][0])>GetCardLogicValue(cbTurnCardData[0])))
	{
		OutCardResult.cbCardCount = CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[0] ;
		memcpy(OutCardResult.cbResultCard,  CardTypeResult[CT_BOMB_CARD].cbCardData, OutCardResult.cbCardCount) ;

		return ;
	}
	//˫��ը����һ��
	else if(cbHandCardCount>2 && cbHandCardData[0]==0x4f && cbHandCardData[1]==0x4e && CT_ERROR!=GetCardType(cbHandCardData+2, cbHandCardCount-2) &&
		CT_FOUR_LINE_TAKE_ONE != GetCardType(cbHandCardData+2, cbHandCardCount-2) && CT_FOUR_LINE_TAKE_TWO != GetCardType(cbHandCardData+2, cbHandCardCount-2))
	{
		OutCardResult.cbCardCount = 2 ;
		OutCardResult.cbResultCard[0] = 0x4f ;
		OutCardResult.cbResultCard[1] = 0x4e ;
		return  ;
	}

	//ը����һ��
	BYTE cbRemainCard[MAX_COUNT], cbRemainCount=0;
	BYTE cbAllBombCard[MAX_COUNT], cbAllBombCount=0 ;
	GetAllBomCard(cbHandCardData, cbHandCardCount, cbAllBombCard, cbAllBombCount) ;

	//��ը�ж�
	if(cbAllBombCount>0)
	{
		//ʣ���˿�
		memcpy(cbRemainCard, cbHandCardData, cbHandCardCount) ;
		cbRemainCount = cbHandCardCount ;
		RemoveCard(cbAllBombCard, cbAllBombCount, cbRemainCard, cbRemainCount) ;
		cbRemainCount -= cbAllBombCount ;

		if(CT_ERROR != GetCardType(cbRemainCard, cbRemainCount) || 
			(2==cbRemainCount && GetCardLogicValue(cbRemainCard[0])>GetCardLogicValue(m_cbAllCardData[wUndersideUser][0]) &&
			GetCardLogicValue(cbRemainCard[0])>GetCardLogicValue(m_cbAllCardData[wUpsideUser][0])))
		{
			if((cbOutCardType<CT_BOMB_CARD || GetCardLogicValue(cbAllBombCard[0])>GetCardLogicValue(cbTurnCardData[0])) &&
				 ( CardTypeResult[cbOutCardType].cbCardTypeCount <= 0 || CT_ERROR != GetCardType(cbRemainCard, cbRemainCount) ) )
			{
				//˫��ը��
				if(GetCardColor(cbAllBombCard[0])==0x40)
				{
					OutCardResult.cbCardCount = 2 ;
					OutCardResult.cbResultCard[0] = 0x4f ;
					OutCardResult.cbResultCard[1] = 0x4e ;
					return ;
				}
				else
				{
					//�����м���
					BYTE cbUnderSideBankerAllBombCard[MAX_COUNT], cbUnderSideBankerAllBombCardCount ;
					GetAllBomCard( m_cbAllCardData[wUndersideUser], m_cbUserCardCount[wUndersideUser], cbUnderSideBankerAllBombCard, cbUnderSideBankerAllBombCardCount) ;

					BYTE cbUpSideBankerAllBombCard[MAX_COUNT], cbUpSideBankerAllBombCardCount ;
					GetAllBomCard( m_cbAllCardData[wUpsideUser], m_cbUserCardCount[wUpsideUser], cbUpSideBankerAllBombCard, cbUpSideBankerAllBombCardCount) ;

					if ( !CompareCard( cbTurnCardData, cbRemainCard, cbTurnCardCount, cbRemainCount) ||  ( cbUnderSideBankerAllBombCardCount <= 0 && cbUpSideBankerAllBombCardCount )|| 
						( GetCardLogicValue( cbAllBombCard[0] ) > GetCardLogicValue( cbUnderSideBankerAllBombCard[0] ) && 
						GetCardLogicValue( cbAllBombCard[0] ) > GetCardLogicValue( cbUpSideBankerAllBombCard[0] )) )
					{
						OutCardResult.cbCardCount = 4 ;
						memcpy(OutCardResult.cbResultCard, cbAllBombCard, 4) ;
						return ;
					}
				}
			}
		}
	}

	//�жϿɷ����
	BYTE cbSingleCardCount = MAX_COUNT+CT_MISSILE_CARD ;
	bool bFindBestCard = false ;
	for(BYTE cbCardType=CT_SINGLE; cbCardType<=CT_MISSILE_CARD; ++cbCardType)
		if(CardTypeResult[cbCardType].cbCardTypeCount>0)
		{
			for(LONG lIndex=0; lIndex<CardTypeResult[cbCardType].cbCardTypeCount; ++lIndex)
			{
				if(TestOutAllCard(m_wBankerUser, CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex], false))
				{
					//���㵥��
					BYTE cbTmpSingleCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CardTypeResult[cbCardType].cbCardData[lIndex], 
						CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]) ;

					//����ж�
					if (cbTmpSingleCount >= MAX_COUNT) continue ;

					//ը�����ȼ��ź�
					BYTE cbBombCardType = GetCardType(CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]) ;
					if (cbBombCardType == CT_BOMB_CARD) cbTmpSingleCount += 4 ;
					else if (cbBombCardType == CT_MISSILE_CARD) cbTmpSingleCount += 5 ;

					////�ı�Ȩֵ
					//if (cbBombCardType != CT_ERROR) cbTmpSingleCount += cbBombCardType ;

					//����ը��
					//BYTE cbWantOutCardType = GetCardType(CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]) ;
					//if (CardTypeResult[cbOutCardType].cbCardTypeCount > 0 && cbOutCardType < CT_BOMB_CARD && cbWantOutCardType >= CT_BOMB_CARD) continue ;

					if (cbTmpSingleCount <= cbSingleCardCount)
					{
						//���ñ�
						OutCardResult.cbCardCount=CardTypeResult[cbCardType].cbEachHandCardCount[lIndex];
						memcpy(OutCardResult.cbResultCard, CardTypeResult[cbCardType].cbCardData[lIndex],
						CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]*sizeof(BYTE));
						cbSingleCardCount = cbTmpSingleCount ;
						bFindBestCard = true ;
					}
				}
			}
		}
		//ֱ�ӷ���
		if (bFindBestCard) return ;

	//ȡ���ĸ���С����
	BYTE cbMinSingleCardCount[4] ;
	cbMinSingleCardCount[0]=MAX_COUNT ;
	cbMinSingleCardCount[1]=MAX_COUNT ;
	cbMinSingleCardCount[2]=MAX_COUNT ;
	cbMinSingleCardCount[3]=MAX_COUNT ;
	BYTE cbIndex[4]={0} ;	
	BYTE cbMinSingleCountInFour=MAX_COUNT ;

	//�ɳ��˿ˣ������Ѿ����˵�ը���ˣ�
	tagOutCardTypeResult const &CanOutCard = CardTypeResult[cbOutCardType] ;

	for(BYTE i=0; i<CanOutCard.cbCardTypeCount; ++i)
	{
		//��С����
		BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount,CanOutCard.cbCardData[i], CanOutCard.cbEachHandCardCount[i]) ; 
		BYTE cbMaxValue=0 ; 
		BYTE Index = 0 ;

		//����cbMinSingleCardCount[4]�����ֵ
		for(BYTE j=0; j<4; ++j)
		{
			if(cbMinSingleCardCount[j]>=cbTmpCount)
			{
				cbMinSingleCardCount[j] = cbTmpCount ;
				cbIndex[j] = i ;
				break ;
			}
		}

	}

	for(BYTE i=0; i<4; ++i)
		if(cbMinSingleCountInFour>cbMinSingleCardCount[i]) cbMinSingleCountInFour = cbMinSingleCardCount[i] ;


	//ԭʼ������
	BYTE cbOriginSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount,NULL,0) ;

	if(CanOutCard.cbCardTypeCount>0 && cbMinSingleCountInFour < MAX_COUNT)
	{
		BYTE cbMinLogicCardValue = GetCardLogicValue(0x4F)+1 ;
		bool bFindCard = false ;
		BYTE cbCanOutIndex=0 ;
		for(BYTE i=0; i<4; ++i)
		{
			BYTE Index = cbIndex[i] ;

			if((cbMinSingleCardCount[i]<cbOriginSingleCardCount+3)  &&  (cbMinSingleCardCount[i]<=cbMinSingleCountInFour || cbMinSingleCardCount[i]<=cbMinSingleCountInFour+1 && 
				CanOutCard.cbCardType >= CT_THREE_LINE_TAKE_ONE && CanOutCard.cbCardType <= CT_THREE_LINE_TAKE_TWO ) &&
				cbMinLogicCardValue>GetCardLogicValue(CanOutCard.cbCardData[Index][0]))
			{
				//��Դ���
				bool bNoLargeCard = true ;

				//��������������������4�����ҳ����С��K���ƶ��Ҳ��ǳ������ϵ������ʱ�����ܳ�2ȥ��
				if(m_cbUserCardCount[wOutCardUser]>=4 && cbHandCardCount>=5  && CanOutCard.cbEachHandCardCount[Index]>=2 && 
					GetCardLogicValue(CanOutCard.cbCardData[Index][0])>=15 &&
					GetCardLogicValue(cbTurnCardData[0])<13 &&
					(wOutCardUser==wUndersideUser&&GetCardLogicValue(cbTurnCardData[0])<GetCardLogicValue(m_cbAllCardData[wUndersideUser][0]) || wOutCardUser==wUpsideUser&&GetCardLogicValue(cbTurnCardData[0])<GetCardLogicValue(m_cbAllCardData[wUpsideUser][0])) && 
					CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount)
					bNoLargeCard=false ;

				//������û�д��ƣ���Էɻ�������Ĵ��ƣ�
				for(BYTE k=3; k<CanOutCard.cbEachHandCardCount[Index]; ++k)
				{
					if(GetCardLogicValue(CanOutCard.cbCardData[Index][k])>=15 && 
						CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount)
						bNoLargeCard = false ;
				}
				if(bNoLargeCard)
				{
					bFindCard = true ;
					cbCanOutIndex = Index ; 
					cbMinLogicCardValue = GetCardLogicValue(CanOutCard.cbCardData[Index][0]) ;
				}
			}
		}

		if(bFindCard)
		{
			//�����
			BYTE cbLargestLogicCard ;
			if(wOutCardUser==wUndersideUser) cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[wUndersideUser][0]) ;
			else if(wOutCardUser==wUpsideUser) cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[wUpsideUser][0]) ;
			bool bCanOut=true ;

			//��ֻѹ2
			if(GetCardLogicValue(cbTurnCardData[0])<cbLargestLogicCard)
			{
				if(GetCardColor(CanOutCard.cbCardData[cbCanOutIndex][0])==0x40 && GetCardLogicValue(cbTurnCardData[0])<=14 && cbHandCardCount>5) 								
				{
					bCanOut = false ;
				}
			}

			//˫���ж�
			if(GetCardLogicValue(CanOutCard.cbCardData[cbCanOutIndex][0])>=16 && cbHandCardCount>=2 && cbHandCardData[0]==0x4F && cbHandCardData[1]==0x4E)
			{
				bool bOutMissileCard = false ;
				//һ���ƺ�һ��ը��
				BYTE cbRemainCardData[20], cbRemainCardCount=cbHandCardCount ;
				memcpy(cbRemainCardData, cbHandCardData, cbHandCardCount) ;
				RemoveCard(cbRemainCardData, 2, cbRemainCardData, cbRemainCardCount) ;
				cbRemainCardCount -= 2 ;
				if(CT_ERROR!=GetCardType(cbRemainCardData, cbRemainCardCount)) bOutMissileCard = true;

				//ֻʣ����
				BYTE cbRemainLargestCard = GetCardLogicValue(cbRemainCardData[0]) ;
				if(cbRemainCardCount<5 && cbRemainCardCount>0 && GetCardLogicValue(cbRemainCardData[0])>=14) bOutMissileCard = true;

				//ը������
				BYTE cbSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CanOutCard.cbCardData[cbCanOutIndex],
					CanOutCard.cbEachHandCardCount[cbCanOutIndex]) ;
				if(cbSingleCardCount<=1 && GetCardLogicValue(cbRemainCardData[0])>=11) bOutMissileCard = true;

				//����С��
				if (GetCardLogicValue(cbRemainCardData[0]) <= 10 && CT_ERROR == GetCardType(cbRemainCardData, cbRemainCardCount) && 
					(GetCardLogicValue(m_cbAllCardData[(1+m_wBankerUser) % GAME_PLAYER][0]) > 10 || GetCardLogicValue(m_cbAllCardData[(2+m_wBankerUser) % GAME_PLAYER][0]) > 10))
					bOutMissileCard = false ;

				//����˿�
				if(bOutMissileCard)
				{
					//��������ը��
					BYTE cbIndex = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount - 1 ;

					OutCardResult.cbCardCount = CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[cbIndex] ;
					memcpy(OutCardResult.cbResultCard, CardTypeResult[CT_BOMB_CARD].cbCardData[cbIndex], OutCardResult.cbCardCount) ;
					return ;
				}
			}

			if(bCanOut)
			{
				//���ñ�
				OutCardResult.cbCardCount=CanOutCard.cbEachHandCardCount[cbCanOutIndex];
				memcpy(OutCardResult.cbResultCard,CanOutCard.cbCardData[cbCanOutIndex],CanOutCard.cbEachHandCardCount[cbCanOutIndex]*sizeof(BYTE));

				return ;
			}
		}

		if(cbOutCardType==CT_SINGLE)
		{
			//�мҵ������
			BYTE cbLargestLogicCard ;
			if(wOutCardUser==wUndersideUser) cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[wUndersideUser][0]) ;
			else if(wOutCardUser==wUpsideUser) cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[wUpsideUser][0]) ;

			if(GetCardLogicValue(cbTurnCardData[0])==14 || 
				GetCardLogicValue(cbTurnCardData[0])>=cbLargestLogicCard || 
				(GetCardLogicValue(cbTurnCardData[0])<cbLargestLogicCard-1) ||
				(wOutCardUser==wUndersideUser&&m_cbUserCardCount[wUndersideUser]<=5 || wOutCardUser==wUpsideUser&&m_cbUserCardCount[wUpsideUser]<=5))
			{
				//ȡһ�Ŵ��ڵ���2����Ҫ���мҳ���ƴ���ƣ�
				BYTE cbIndex=MAX_COUNT ;
				for(BYTE i=0; i<cbHandCardCount; ++i)
					if(GetCardLogicValue(cbHandCardData[i])>GetCardLogicValue(cbTurnCardData[0]) &&
						GetCardLogicValue(cbHandCardData[i])>=15)
					{
						cbIndex = i ;
					}
					if(cbIndex!=MAX_COUNT)
					{
						//���ñ�
						OutCardResult.cbCardCount=1;
						OutCardResult.cbResultCard[0] = cbHandCardData[cbIndex] ;

						return ;
					}
			}
		}


		BYTE cbMinSingleCount=MAX_COUNT ;
		BYTE Index=0 ;
		for(BYTE i=0; i<CardTypeResult[cbOutCardType].cbCardTypeCount; ++i)
		{
			BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CardTypeResult[cbOutCardType].cbCardData[i], CardTypeResult[cbOutCardType].cbEachHandCardCount[i]) ;
			if(cbMinSingleCount>=cbTmpCount)
			{
				cbMinSingleCount = cbTmpCount ;
				Index = i ;
			}
		}
		//���ñ�
		OutCardResult.cbCardCount=CardTypeResult[cbOutCardType].cbEachHandCardCount[Index];
		memcpy(OutCardResult.cbResultCard, CardTypeResult[cbOutCardType].cbCardData[Index], OutCardResult.cbCardCount) ;

		return ;
	}

	//��Ҫ����ը��
	if(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount>0)
	{
		tagOutCardTypeResult const &BomCard = CardTypeResult[CT_BOMB_CARD] ;
		BYTE cbMinLogicValue = GetCardLogicValue(BomCard.cbCardData[0][0]) ;
		BYTE Index = 0 ;
		for(BYTE i=0; i<BomCard.cbCardTypeCount; ++i)
		{
			if(cbMinLogicValue>GetCardLogicValue(BomCard.cbCardData[i][0]))
			{
				cbMinLogicValue = GetCardLogicValue(BomCard.cbCardData[i][0]) ;
				Index = i ;
			}
		}

		bool bOutBomb=false ;

		//��һ�м�
		//WORD wOtherUser=INVALID_CHAIR ;
		//for(WORD wUser=0; wUser<GAME_PLAYER; ++wUser)
		//	if(wUser!=wOutCardUser && wUser!=m_wBankerUser) wOtherUser=wOtherUser ;
		
		//һ���ƺ�һ��ը��
		BYTE cbRemainCardData[20], cbRemainCardCount=cbHandCardCount ;
		memcpy(cbRemainCardData, cbHandCardData, cbHandCardCount) ;
		RemoveCard(BomCard.cbCardData[Index], BomCard.cbEachHandCardCount[Index], cbRemainCardData, cbRemainCardCount) ;
		cbRemainCardCount -= BomCard.cbEachHandCardCount[Index] ;
		if(CT_ERROR!=GetCardType(cbRemainCardData, cbRemainCardCount)) bOutBomb = true ;

		//ը������
		BYTE cbSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, BomCard.cbCardData[Index],BomCard.cbEachHandCardCount[Index]) ;
		if(cbSingleCardCount==0 && GetCardLogicValue(cbRemainCardData[0]) > 
			GetCardLogicValue(m_cbAllCardData[wUpsideUser == wOutCardUser ? wUndersideUser : wUpsideUser][0])) bOutBomb = true ;

		//ֻʣһ��
		BYTE cbRemainCardType = GetCardType(m_cbAllCardData[wOutCardUser], m_cbUserCardCount[wOutCardUser]) ;
		if(cbRemainCardType>CT_ERROR && cbRemainCardType<CT_FOUR_LINE_TAKE_ONE && GetCardLogicValue(m_cbAllCardData[wOutCardUser][0])<15 && 
			cbSingleCardCount < 2 && (GetCardLogicValue(cbRemainCardData[0]) >= GetCardLogicValue(m_cbAllCardData[wUndersideUser][0]) && 
			GetCardLogicValue(cbRemainCardData[0]) >= GetCardLogicValue(m_cbAllCardData[wUpsideUser][0]))) bOutBomb = true ;

		//������
		if (cbRemainCardType != CT_ERROR && m_lBankerOutCardCount == 1) bOutBomb = true ;

		//ֻʣ����
		BYTE cbRemainLargestCard = GetCardLogicValue(cbRemainCardData[0]) ;
		if(cbRemainCardCount<5 && cbRemainCardCount>0 && (cbRemainLargestCard!=GetCardLogicValue(BomCard.cbCardData[Index][0])) && 
			cbRemainLargestCard>GetCardLogicValue(m_cbAllCardData[wOutCardUser][0]) && cbRemainLargestCard > 14) bOutBomb = true ;

		//�����˿�
		tagAnalyseResult AnalyseResult ;
		AnalysebCardData(cbRemainCardData, cbRemainCardCount, AnalyseResult) ;

		if (m_cbUserCardCount[wOutCardUser] == 1 && (AnalyseResult.cbDoubleCount * 2 + AnalyseResult.cbThreeCount * 3 + AnalyseResult.cbFourCount * 4 + 1 >= cbRemainCardCount)) bOutBomb = true ;


		//���ñ�
		if(bOutBomb)
		{
			OutCardResult.cbCardCount=BomCard.cbEachHandCardCount[Index];
			memcpy(OutCardResult.cbResultCard,BomCard.cbCardData[Index],BomCard.cbEachHandCardCount[Index]*sizeof(BYTE));
		}
		return ;
	}

	return ;
}

//�����ϼң��ȳ��ƣ�
void CAndroidAI::UpsideOfBankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, WORD wMeChairID, tagOutCardResult & OutCardResult)
{
	//��ʼ��
	memset(&OutCardResult, '\0',sizeof(OutCardResult)) ;

	//����ֻ��һ�ţ��Ӵ�С����
	if (m_cbUserCardCount[m_wBankerUser] == 1)
	{
		BYTE cbSingleCardData[MAX_COUNT] ;
		BYTE cbSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, NULL, 0, cbSingleCardData) ;

		//ֻʣ����
		if (cbSingleCardCount == cbHandCardCount) 
		{
			OutCardResult.cbCardCount = 1 ;
			OutCardResult.cbResultCard[0] = cbHandCardData[0] ;

			return ;
		}
	}

	//���±�û��
	tagOutCardTypeResult CardTypeResult[12+1] ;
	memset(CardTypeResult, '\0',sizeof(CardTypeResult)) ;

	BYTE cbLineCard[MAX_COUNT] ;
	BYTE cbThreeLineCard[MAX_COUNT] ;
	BYTE cbDoubleLineCard[MAX_COUNT] ;
	BYTE cbLineCardCount;
	BYTE cbThreeLineCardCount ;
	BYTE cbDoubleLineCount ;
	GetAllLineCard(cbHandCardData, cbHandCardCount, cbLineCard, cbLineCardCount) ;
	GetAllThreeCard(cbHandCardData, cbHandCardCount, cbThreeLineCard, cbThreeLineCardCount) ;
	GetAllDoubleCard(cbHandCardData, cbHandCardCount, cbDoubleLineCard, cbDoubleLineCount) ;

	//�жϿɷ����
	BYTE cbSingleCardCount = MAX_COUNT+CT_MISSILE_CARD ;
	bool bFindBestCard = false ;
	AnalyseOutCardType(cbHandCardData, cbHandCardCount, CardTypeResult) ;
	for(BYTE cbCardType=CT_SINGLE; cbCardType<=CT_MISSILE_CARD; ++cbCardType)
		if(CardTypeResult[cbCardType].cbCardTypeCount>0)
		{
			for(LONG lIndex=0; lIndex<CardTypeResult[cbCardType].cbCardTypeCount; ++lIndex)
			{
				WORD wMeChairID = (m_wBankerUser+2)%GAME_PLAYER ;
				
				if(TestOutAllCard(wMeChairID, CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex], true))
				{
					//���㵥��
					BYTE cbTmpSingleCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CardTypeResult[cbCardType].cbCardData[lIndex], 
						CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]) ;

					//����ж�
					if (cbTmpSingleCount >= MAX_COUNT) continue ;

					//ը�����ȼ��ź�
					BYTE cbBombCardType = GetCardType(CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]) ;
					if (cbBombCardType == CT_BOMB_CARD) cbTmpSingleCount += 4 ;
					else if (cbBombCardType == CT_MISSILE_CARD) cbTmpSingleCount += 5 ;
					else if ( 15 == GetCardLogicValue( CardTypeResult[ cbCardType ].cbCardData[ lIndex ][ 0 ] ) ) cbTmpSingleCount += 2;
					else if ( 15 < GetCardLogicValue( CardTypeResult[ cbCardType ].cbCardData[ lIndex ][ 0 ] ) ) cbTmpSingleCount += 3;


					////�ı�Ȩֵ
					//if (cbBombCardType != CT_ERROR) cbTmpSingleCount += cbBombCardType ;

					if(cbTmpSingleCount <= cbSingleCardCount)
					{
						//���ñ�
						OutCardResult.cbCardCount=CardTypeResult[cbCardType].cbEachHandCardCount[lIndex];
						memcpy(OutCardResult.cbResultCard, CardTypeResult[cbCardType].cbCardData[lIndex],
						CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]*sizeof(BYTE));
						cbSingleCardCount = cbTmpSingleCount ;

						bFindBestCard = true ;
					}
				}
			}
		}
		//ֱ�ӷ���
		if (bFindBestCard) return ;

		//�����}��
		if ( cbHandCardCount == 4 && GetCardLogicValue(cbHandCardData[1]) == 16 && m_cbUserCardCount[m_wBankerUser] == 1 && 
			GetCardLogicValue(cbHandCardData[2]) < GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]))
		{
			OutCardResult.cbCardCount = 1 ;
			OutCardResult.cbResultCard[0] = cbHandCardData[2] ;
			return ;
		}

		//�Ĵ������ж�
		if ( AnalyseFourCardType(cbHandCardData, cbHandCardCount, m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], OutCardResult ) )
		{
			return ;
		}

	//�����˳�ƺ͵�ֻ��һ�ԣ����ҵ�ֻ��Աȵ����С�����ȳ�˳
	{
		if(cbLineCardCount+1==cbHandCardCount && CT_SINGLE==GetCardType(cbLineCard, cbLineCardCount))
		{
			OutCardResult.cbCardCount = cbLineCardCount ;
			memcpy(OutCardResult.cbResultCard, cbLineCard, cbLineCardCount) ;
		}
		else if(cbThreeLineCardCount+1==cbHandCardCount && CT_THREE_LINE==GetCardType(cbThreeLineCard, cbThreeLineCardCount))
		{
			OutCardResult.cbCardCount = cbThreeLineCardCount ;
			memcpy(OutCardResult.cbResultCard, cbThreeLineCard, cbThreeLineCardCount) ;
		}
		else if(cbDoubleLineCount+1==cbHandCardCount && CT_DOUBLE_LINE==GetCardType(cbDoubleLineCard, cbDoubleLineCount))
		{
			OutCardResult.cbCardCount = cbDoubleLineCount ;
			memcpy(OutCardResult.cbResultCard, cbDoubleLineCard, cbDoubleLineCount) ;
		}
		//˫��ը����һ��
		else if(cbHandCardCount>2 && cbHandCardData[0]==0x4f && cbHandCardData[1]==0x4e && CT_ERROR!=GetCardType(cbHandCardData+2, cbHandCardCount-2) &&
			CT_FOUR_LINE_TAKE_ONE != GetCardType(cbHandCardData+2, cbHandCardCount-2) && CT_FOUR_LINE_TAKE_TWO != GetCardType(cbHandCardData+2, cbHandCardCount-2))
		{
			OutCardResult.cbCardCount = 2 ;
			OutCardResult.cbResultCard[0] = 0x4f ;
			OutCardResult.cbResultCard[1] = 0x4e ;
		}

		if(OutCardResult.cbCardCount>0)
		{
			return ;
		}
	}
	//�����һֻ
	if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
	{
		OutCardResult.cbCardCount = 2 ;
		OutCardResult.cbResultCard[0] = 0x4f ;
		OutCardResult.cbResultCard[1] = 0x4e ;
		return ;
	}
	//����
	else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
	{
		OutCardResult.cbCardCount = 2 ;
		OutCardResult.cbResultCard[0] = 0x4f ;
		OutCardResult.cbResultCard[1] = 0x4e ;
		return ;
	}
	//ֻʣһ����
	else if(CT_ERROR!=GetCardType(cbHandCardData, cbHandCardCount) && CT_FOUR_LINE_TAKE_ONE!=GetCardType(cbHandCardData, cbHandCardCount) && CT_FOUR_LINE_TAKE_TWO!=GetCardType(cbHandCardData, cbHandCardCount))
	{
		OutCardResult.cbCardCount = cbHandCardCount ;
		memcpy(OutCardResult.cbResultCard, cbHandCardData, cbHandCardCount) ;
		return ;
	}

	//ֻʣһ�ź�һ��
	if(cbHandCardCount>=2)
	{
		//�����˿�
		tagOutCardTypeResult BankerCanOutCardType1[13] ;
		memset(BankerCanOutCardType1,'\0', sizeof(BankerCanOutCardType1)) ;
		tagOutCardTypeResult BankerCanOutCardType2[13] ;
		memset(BankerCanOutCardType2,'\0', sizeof(BankerCanOutCardType2)) ;

		BYTE cbFirstHandCardType = GetCardType(cbHandCardData, cbHandCardCount-1) ;
		BYTE cbSecondHandCardType = GetCardType(cbHandCardData+1, cbHandCardCount-1) ;

		//�Ƿ���ը
		BYTE cbAllBombCardData[MAX_COUNT], cbAllBombCount=0 ;
		GetAllBomCard(cbHandCardData, cbHandCardCount, cbAllBombCardData, cbAllBombCount) ;

		//û��ը��
		if (cbAllBombCount <= 0 && cbFirstHandCardType!=CT_THREE_LINE_TAKE_ONE && cbFirstHandCardType!=CT_THREE_LINE_TAKE_TWO)
		{
		//������Գ����
		if(cbFirstHandCardType!=CT_ERROR)
			AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], cbHandCardData, cbHandCardCount-1, BankerCanOutCardType1) ;
		if(cbSecondHandCardType!=CT_ERROR)
			AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], cbHandCardData+1, cbHandCardCount-1, BankerCanOutCardType2) ;

		if(cbSecondHandCardType!=CT_ERROR && cbSecondHandCardType!=CT_FOUR_LINE_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_LINE_TAKE_TWO && 
			BankerCanOutCardType2[cbSecondHandCardType].cbCardTypeCount==0 && BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
		{
			OutCardResult.cbCardCount = cbHandCardCount-1 ;
			memcpy(OutCardResult.cbResultCard, cbHandCardData+1, cbHandCardCount-1) ;
			return ;
		}

		if(cbFirstHandCardType!=CT_ERROR && cbFirstHandCardType!=CT_FOUR_LINE_TAKE_ONE && cbFirstHandCardType!= CT_FOUR_LINE_TAKE_TWO &&
			BankerCanOutCardType1[cbFirstHandCardType].cbCardTypeCount==0 && BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
		{
			OutCardResult.cbCardCount = cbHandCardCount-1 ;
			memcpy(OutCardResult.cbResultCard, cbHandCardData, cbHandCardCount-1) ;
			return ;
		}

		if(GetCardLogicValue(cbHandCardData[0])>=GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) &&
			CT_ERROR!=cbSecondHandCardType && cbSecondHandCardType!=CT_FOUR_LINE_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_LINE_TAKE_TWO &&
			BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
		{
			OutCardResult.cbCardCount = 1 ;
			OutCardResult.cbResultCard[0] = cbHandCardData[0] ;
			return ;
		}

		if(CT_ERROR!=cbSecondHandCardType && cbSecondHandCardType!=CT_FOUR_LINE_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_LINE_TAKE_TWO &&
			BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
		{
			OutCardResult.cbCardCount = cbHandCardCount-1 ;
			memcpy(OutCardResult.cbResultCard, cbHandCardData+1, cbHandCardCount-1) ;
			return ;
		}
		}
		//����ը��
		else
		{
			//��ȥը�����
			BYTE cbRemainCard[MAX_COUNT], cbRemainCount=0 ;
			memcpy(cbRemainCard, cbHandCardData, cbHandCardCount) ;
			cbRemainCount = cbHandCardCount ;
			RemoveCard(cbAllBombCardData, cbAllBombCount, cbRemainCard, cbRemainCount) ;
			cbRemainCount -= cbAllBombCount ;
			if (GetCardType(cbRemainCard, cbRemainCount) != CT_ERROR)
			{
				OutCardResult.cbCardCount = cbRemainCount ;
				memcpy(OutCardResult.cbResultCard, cbRemainCard, cbRemainCount) ;
				return ;
			}
		}
	}

	//���ƽ����ж�
	WORD wFriendID = (m_wBankerUser + 1) % GAME_PLAYER ;
	if (1 == m_cbUserCardCount[m_wBankerUser] && cbHandCardCount >= 2 && m_cbUserCardCount[wFriendID] >= 2)
	{
		tagAnalyseResult MeAnalyseResult ;
		memset(&MeAnalyseResult,'\0', sizeof(MeAnalyseResult)) ;

		//�����˿�
		AnalysebCardData(cbHandCardData, cbHandCardCount, MeAnalyseResult) ;

		tagAnalyseResult FriendAnalyseResult ;
		memset(&FriendAnalyseResult,'\0', sizeof(FriendAnalyseResult)) ;

		//�����˿�
		AnalysebCardData(m_cbAllCardData[wFriendID], m_cbUserCardCount[wFriendID], FriendAnalyseResult) ;

		//�����ж�
		if ((m_cbUserCardCount[wFriendID] == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4) || 
			m_cbUserCardCount[wFriendID] == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4 + 1)) &&
			MeAnalyseResult.cbDoubleCount > 0 && FriendAnalyseResult.cbDoubleCount > 0)
		{
			//��С����
			BYTE cbMeLeastDoubleCardLogic = GetCardLogicValue(MeAnalyseResult.cbDoubleCardData[MeAnalyseResult.cbDoubleCount*2-2]) ;
			//������
			BYTE cbFriendLargestDoublecardLogic = GetCardLogicValue(FriendAnalyseResult.cbDoubleCardData[0]) ;

			//����ж�
			if (cbMeLeastDoubleCardLogic < 14 && cbMeLeastDoubleCardLogic < cbFriendLargestDoublecardLogic)
			{
				OutCardResult.cbCardCount = 2 ;
				OutCardResult.cbResultCard[0] = MeAnalyseResult.cbDoubleCardData[MeAnalyseResult.cbDoubleCount*2-2] ;
				OutCardResult.cbResultCard[1] = MeAnalyseResult.cbDoubleCardData[MeAnalyseResult.cbDoubleCount*2-1] ;
				return ;
			}
          			
		}
	}

	//�¼�Ϊ������ҵ����˿�����5��
	//	if(m_cbUserCardCount[m_wBankerUser]<=5)
	{
		//�����˿�
		tagOutCardTypeResult MeOutCardTypeResult[13] ;
		memset(MeOutCardTypeResult, '\0',sizeof(MeOutCardTypeResult)) ;
		AnalyseOutCardType(cbHandCardData, cbHandCardCount, MeOutCardTypeResult) ;

		//�Լ��˿�
		WORD wFriendID ;
		for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID) 
			if(wChairID!=m_wBankerUser && wMeChairID!=wChairID) wFriendID = wChairID ;

		//���㵥��
		BYTE cbMinSingleCardCount[4] ;
		cbMinSingleCardCount[0]=MAX_COUNT ;
		cbMinSingleCardCount[1]=MAX_COUNT ;
		cbMinSingleCardCount[2]=MAX_COUNT ;
		cbMinSingleCardCount[3]=MAX_COUNT ;
		BYTE cbIndex[4]={0} ;
		BYTE cbOutcardType[4]={CT_ERROR} ;
		BYTE cbMinValue=MAX_COUNT ; 
		BYTE cbMinSingleCountInFour=MAX_COUNT ;
		BYTE cbMinCardType=CT_ERROR ;
		BYTE cbMinIndex=0 ;

		//����������
		BYTE cbBankerDoubleCardData[MAX_COUNT], cbBankerDoubleCardCount ;
		GetAllDoubleCard(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], cbBankerDoubleCardData, cbBankerDoubleCardCount) ;

		//��ը�������
		for(BYTE cbCardType=CT_DOUBLE; cbCardType<CT_BOMB_CARD; ++cbCardType)
		{
			tagOutCardTypeResult const &tmpCardResult = MeOutCardTypeResult[cbCardType] ;

			//��ͬ���ͣ���ͬ���ȣ���l����l����ͬ���Ϳ��ܳ��Ȳ�һ��
			BYTE cbThisHandCardCount = MAX_COUNT ;

			//�����˿�
			tagOutCardTypeResult BankerCanOutCard[13] ;
			memset(BankerCanOutCard,'\0', sizeof(BankerCanOutCard)) ;

			tagOutCardTypeResult FriendOutCardTypeResult[13] ;
			memset(FriendOutCardTypeResult,'\0', sizeof(FriendOutCardTypeResult)) ;

			for(BYTE i=0; i<tmpCardResult.cbCardTypeCount; ++i)
			{
				//�������ж�
				if ( cbCardType == CT_DOUBLE )
				{
					tagAnalyseResult AnalyseResult ;
					memset( &AnalyseResult, '\0', sizeof( AnalyseResult ) ) ;
					AnalysebCardData( cbHandCardData, cbHandCardCount, AnalyseResult ) ;
					if ( AnalyseResult.cbSignedCount + AnalyseResult.cbThreeCount * 3 == cbHandCardCount )
					{
						bool bContinue = false ;
						for ( BYTE cbThreeIndex = 0; cbThreeIndex < AnalyseResult.cbThreeCount; ++cbThreeIndex )
							if ( GetCardValue(  tmpCardResult.cbCardData[i][0] ) == GetCardValue( AnalyseResult.cbThreeCardData[3 * cbThreeIndex] ) )
							{
								bContinue = true ;
								break ;
							}
						if ( bContinue ) continue ;
					}
				}

				BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i]) ;

				//9�ض���
				if (cbCardType == CT_DOUBLE && cbBankerDoubleCardCount > 0 && GetCardLogicValue(cbBankerDoubleCardData[cbBankerDoubleCardCount-1]) < 10 &&
					GetCardLogicValue(tmpCardResult.cbCardData[i][0]) < GetCardLogicValue(cbBankerDoubleCardData[cbBankerDoubleCardCount-1]) &&
					GetCardLogicValue(tmpCardResult.cbCardData[0][0]) >= 10 && GetCardLogicValue(tmpCardResult.cbCardData[0][0]) < 14) continue ;

				//���·���
				if(tmpCardResult.cbEachHandCardCount[i]!=cbThisHandCardCount)
				{
					cbThisHandCardCount = tmpCardResult.cbEachHandCardCount[i] ;
					AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], 
						tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i] ,BankerCanOutCard) ;
					AnalyseOutCardType(m_cbAllCardData[wFriendID], m_cbUserCardCount[wFriendID],
						tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i] ,FriendOutCardTypeResult) ;
				}

				BYTE cbMaxValue=0 ; 
				BYTE Index = 0 ;

				//���˳�ӣ�����Ĵ���
				BYTE cbCurrentCardType = GetCardType(tmpCardResult.cbCardData[i], cbThisHandCardCount) ;
				if (cbThisHandCardCount != cbHandCardCount && cbCurrentCardType >= CT_SINGLE_LINE && cbCurrentCardType <= CT_THREE_LINE_TAKE_TWO &&
					( GetCardLogicValue(tmpCardResult.cbCardData[i][cbThisHandCardCount-1]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-2]) || 
						GetCardLogicValue(tmpCardResult.cbCardData[i][0]) >= 11 ))
				{
					BYTE cbRemainCardData[MAX_COUNT], cbRemainCardCount ;
					memcpy(cbRemainCardData, cbHandCardData, cbHandCardCount) ;
					cbRemainCardCount = cbHandCardCount ;

					//�Ƴ��˿�
					RemoveCard(tmpCardResult.cbCardData[i], cbThisHandCardCount, cbRemainCardData, cbRemainCardCount) ;
					cbRemainCardCount -= cbThisHandCardCount ;

					//����˿�
					BYTE cbCurrentLargestLogicCard = GetCardLogicValue(tmpCardResult.cbCardData[i][0]) ;

					if (GetCardType(cbRemainCardData, cbRemainCardCount) == CT_ERROR && (cbCurrentCardType >= CT_THREE_LINE_TAKE_ONE && 
						cbCurrentCardType <= CT_THREE_LINE_TAKE_TWO && cbCurrentLargestLogicCard >= 11 && cbThisHandCardCount <=5 || 
						cbCurrentCardType == CT_SINGLE_LINE && cbThisHandCardCount <= 6 && cbCurrentLargestLogicCard >= 12 || 
						cbCurrentCardType >= CT_DOUBLE_LINE && cbCurrentCardType <= CT_THREE_LINE && cbCurrentLargestLogicCard >= 12 && cbThisHandCardCount <= 8))
					{
						//��ʱ����
						if ( cbCurrentCardType >= CT_SINGLE_LINE && cbCurrentCardType <= CT_THREE_LINE &&
							GetCardLogicValue(tmpCardResult.cbCardData[i][cbThisHandCardCount - 1]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-3]) )
							continue ;

						if ( cbCurrentCardType >= CT_THREE_LINE_TAKE_ONE && cbCurrentCardType <= CT_THREE_LINE_TAKE_TWO &&
							GetCardLogicValue(tmpCardResult.cbCardData[i][0]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-3]) )
							continue ;
					}
				}

				//�������ѹ�ƣ����Ҷ��Ѳ�����ѹ����
				if((BankerCanOutCard[cbCardType].cbCardTypeCount>0&&FriendOutCardTypeResult[cbCardType].cbCardTypeCount==0) || (BankerCanOutCard[cbCardType].cbCardTypeCount>0 && FriendOutCardTypeResult[cbCardType].cbCardTypeCount>0 &&
					GetCardLogicValue(FriendOutCardTypeResult[cbCardType].cbCardData[0][0])<=GetCardLogicValue(BankerCanOutCard[cbCardType].cbCardData[0][0])))
				{
					//�����ܵ�
					if( BankerCanOutCard[cbCardType].cbEachHandCardCount[0] > 0 && m_cbUserCardCount[m_wBankerUser]<=BankerCanOutCard[cbCardType].cbEachHandCardCount[0]+1)
						continue ;

					//�Լ����������û���Ȩ
					if(GetCardLogicValue(tmpCardResult.cbCardData[0][0]) < GetCardLogicValue(BankerCanOutCard[cbCardType].cbCardData[0][0]) &&
						BankerCanOutCard[cbCardType].cbCardTypeCount > 0) 
						continue ;
				}
				//�Ƿ��д���
				if(tmpCardResult.cbEachHandCardCount[i] != cbHandCardCount)
				{
					bool bHaveLargeCard=false ;
					for(BYTE j=0; j<tmpCardResult.cbEachHandCardCount[i]; ++j)
						if(GetCardLogicValue(tmpCardResult.cbCardData[i][j])>=15) bHaveLargeCard=true ;
					if(cbCardType!=CT_SINGLE_LINE && cbCardType!=CT_DOUBLE_LINE  && GetCardLogicValue(tmpCardResult.cbCardData[i][0])==14) bHaveLargeCard=true ; 

					if(bHaveLargeCard) continue ;
				}

				//�����Ƿ�����ߵ����ﶼû�п���ը��
				if(tmpCardResult.cbEachHandCardCount[i]==m_cbUserCardCount[m_wBankerUser] && cbCardType==GetCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser]) &&
					GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0])>GetCardLogicValue(tmpCardResult.cbCardData[i][0])) continue ;

				//����cbMinSingleCardCount[4]�����ֵ
				for(BYTE j=0; j<4; ++j)
				{
					if(cbMinSingleCardCount[j]>=cbTmpCount)
					{
						cbMinSingleCardCount[j] = cbTmpCount ;
						cbIndex[j] = i ;
						cbOutcardType[j] = cbCardType ;
						break ;
					}
				}

				//������Сֵ
				if(cbMinSingleCountInFour>=cbTmpCount)
				{
					//��С����
					cbMinCardType = cbCardType ;
					//��С�����е���С����
					cbMinSingleCountInFour=cbTmpCount ;						
					//��С�����е���С��
					cbMinIndex=i ;
				}
			}
		}

		if(cbMinSingleCountInFour>=AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, NULL, 0)+3 && 
			m_cbUserCardCount[m_wBankerUser]>4)
			cbMinSingleCountInFour=MAX_COUNT ;

		if(cbMinSingleCountInFour!=MAX_COUNT)
		{
			BYTE Index = cbMinIndex ;

			//ѡ����С��
			for(BYTE i=0; i<4; ++i)
			{
				if(cbOutcardType[i]==cbMinCardType && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
					GetCardLogicValue(MeOutCardTypeResult[cbMinCardType].cbCardData[cbIndex[i]][0])<GetCardLogicValue(MeOutCardTypeResult[cbMinCardType].cbCardData[Index][0]))
					Index = cbIndex[i] ;
			}

			//�����һֻ
			if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
			{
				OutCardResult.cbCardCount = 2 ;
				OutCardResult.cbResultCard[0] = 0x4f ;
				OutCardResult.cbResultCard[1] = 0x4e ;
				return ;
			}
			//����
			else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
			{
				OutCardResult.cbCardCount = 2 ;
				OutCardResult.cbResultCard[0] = 0x4f ;
				OutCardResult.cbResultCard[1] = 0x4e ;
				return ;
			}
			else
			{
				//���ñ�
				OutCardResult.cbCardCount=MeOutCardTypeResult[cbMinCardType].cbEachHandCardCount[Index];
				memcpy(OutCardResult.cbResultCard,MeOutCardTypeResult[cbMinCardType].cbCardData[Index],MeOutCardTypeResult[cbMinCardType].cbEachHandCardCount[Index]*sizeof(BYTE));
				return ;
			}

			assert(OutCardResult.cbCardCount>0) ;

			return ;
		}

		//�������˿�����5����û���ҵ��ʺϵ�����Ӵ��С
		if(OutCardResult.cbCardCount<=0 && m_cbUserCardCount[m_wBankerUser]<=5)
		{
			//ֻ��һ����ʱ���ܷŵ�����
			if(m_cbUserCardCount[m_wBankerUser]==1 && MeOutCardTypeResult[CT_SINGLE].cbCardTypeCount>0)
			{
				BYTE Index=MAX_COUNT ;
				for(BYTE i=0; i<MeOutCardTypeResult[CT_SINGLE].cbCardTypeCount; ++i)
				{
					if(GetCardLogicValue(MeOutCardTypeResult[CT_SINGLE].cbCardData[i][0])>=GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]))
					{
						Index=i ;
					}
					else break ;
				}

				if(MAX_COUNT!=Index)
				{
					OutCardResult.cbCardCount = MeOutCardTypeResult[CT_SINGLE].cbEachHandCardCount[Index] ;
					memcpy(OutCardResult.cbResultCard, MeOutCardTypeResult[CT_SINGLE].cbCardData[Index], OutCardResult.cbCardCount) ;
					return ;
				}
			}
		}
	}

	BYTE cbFirstCard=0 ;
	//�������2
	for(BYTE i=0; i<cbHandCardCount; ++i) 
		if(GetCardLogicValue(cbHandCardData[i])<15)
		{
			cbFirstCard = i ;
			break ;
		}

		if(cbFirstCard<cbHandCardCount-1)
			AnalyseOutCardType(cbHandCardData+cbFirstCard, cbHandCardCount-cbFirstCard, CardTypeResult) ;
		else
			AnalyseOutCardType(cbHandCardData, cbHandCardCount, CardTypeResult) ;

		//���㵥��
		BYTE cbMinSingleCardCount[4] ;
		cbMinSingleCardCount[0]=MAX_COUNT ;
		cbMinSingleCardCount[1]=MAX_COUNT ;
		cbMinSingleCardCount[2]=MAX_COUNT ;
		cbMinSingleCardCount[3]=MAX_COUNT ;
		BYTE cbIndex[4]={0} ;
		BYTE cbOutcardType[4]={CT_ERROR} ;
		BYTE cbMinValue=MAX_COUNT ; 
		BYTE cbMinSingleCountInFour=MAX_COUNT ;
		BYTE cbMinCardType=CT_ERROR ;
		BYTE cbMinIndex=0 ;

		//���������
		BYTE cbBankerSingleCardData[MAX_COUNT] ;
		BYTE cbBankerSingleCardCount=AnalyseSinleCardCount(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], NULL, 0, cbBankerSingleCardData) ;
		BYTE cbBankerSingleCardLogic = 0 ;
		if(cbBankerSingleCardCount>=2 && GetCardLogicValue(cbBankerSingleCardData[cbBankerSingleCardCount-2])<=10) cbBankerSingleCardLogic = GetCardLogicValue(cbBankerSingleCardData[cbBankerSingleCardCount-2]) ;
		else if(cbBankerSingleCardCount>=2 && GetCardLogicValue(cbBankerSingleCardData[cbBankerSingleCardCount-1])<=10) cbBankerSingleCardLogic = GetCardLogicValue(cbBankerSingleCardData[cbBankerSingleCardCount-1]) ;
		else if(cbBankerSingleCardCount>0 && GetCardLogicValue(cbBankerSingleCardData[0])<=10) cbBankerSingleCardLogic = GetCardLogicValue(cbBankerSingleCardData[0]) ;

		//����������
		BYTE cbBankerDoubleCardData[MAX_COUNT], cbBankerDoubleCardCount ;
		GetAllDoubleCard(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], cbBankerDoubleCardData, cbBankerDoubleCardCount) ;

		//��ը�������
		for(BYTE cbCardType=CT_SINGLE; cbCardType<CT_BOMB_CARD; ++cbCardType)
		{
			tagOutCardTypeResult const &tmpCardResult = CardTypeResult[cbCardType] ;
			for(BYTE i=0; i<tmpCardResult.cbCardTypeCount; ++i)
			{
				//���ܷ��ߵ���С��
				if(cbCardType==CT_SINGLE &&  cbBankerSingleCardCount > 0 && GetCardLogicValue(tmpCardResult.cbCardData[i][0])<cbBankerSingleCardLogic) continue ;

				//9�ض���
				if (cbCardType == CT_DOUBLE && cbBankerDoubleCardCount > 0 && GetCardLogicValue(cbBankerDoubleCardData[cbBankerDoubleCardCount-1]) < 10 &&
					GetCardLogicValue(tmpCardResult.cbCardData[i][0]) < GetCardLogicValue(cbBankerDoubleCardData[cbBankerDoubleCardCount-1]) &&
					GetCardLogicValue(tmpCardResult.cbCardData[0][0]) >= 10 && GetCardLogicValue(tmpCardResult.cbCardData[0][0]) < 14) continue ;

				//���˳�ӣ�����Ĵ���
				if ( tmpCardResult.cbEachHandCardCount[i] != cbHandCardCount && cbCardType >= CT_SINGLE_LINE && cbCardType <= CT_THREE_LINE_TAKE_TWO &&
					( GetCardLogicValue(tmpCardResult.cbCardData[i][tmpCardResult.cbEachHandCardCount[i]-1]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-2]) || 
						GetCardLogicValue(tmpCardResult.cbCardData[i][0]) >= 11 ))
				{
					BYTE cbRemainCardData[MAX_COUNT], cbRemainCardCount ;
					memcpy(cbRemainCardData, cbHandCardData, cbHandCardCount) ;
					cbRemainCardCount = cbHandCardCount ;

					//�Ƴ��˿�
					RemoveCard(tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i], cbRemainCardData, cbRemainCardCount) ;
					cbRemainCardCount -= tmpCardResult.cbEachHandCardCount[i] ;

					//����˿�
					BYTE cbCurrentLargestLogicCard = GetCardLogicValue(tmpCardResult.cbCardData[i][0]) ;

					if (GetCardType(cbRemainCardData, cbRemainCardCount) == CT_ERROR && (cbCardType >= CT_THREE_LINE_TAKE_ONE && 
						cbCardType <= CT_THREE_LINE_TAKE_TWO && cbCurrentLargestLogicCard >= 11 && tmpCardResult.cbEachHandCardCount[i] <=5 || 
						cbCardType == CT_SINGLE_LINE && tmpCardResult.cbEachHandCardCount[i] <= 6 && cbCurrentLargestLogicCard >= 12 || 
						cbCardType >= CT_DOUBLE_LINE && cbCardType <= CT_THREE_LINE && cbCurrentLargestLogicCard >= 12 && tmpCardResult.cbEachHandCardCount[i] <= 8))
					{
						//��ʱ����
						if ( cbCardType >= CT_SINGLE_LINE && cbCardType <= CT_THREE_LINE &&
							GetCardLogicValue(tmpCardResult.cbCardData[i][tmpCardResult.cbEachHandCardCount[i] - 1]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-3]) )
							continue ;

						if ( cbCardType >= CT_THREE_LINE_TAKE_ONE && cbCardType <= CT_THREE_LINE_TAKE_TWO &&
							GetCardLogicValue(tmpCardResult.cbCardData[i][0]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-3]) )
							continue ;
					}
				}

				BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i]) ;

				BYTE cbMaxValue=0 ; 
				BYTE Index = 0 ;

				//����cbMinSingleCardCount[4]�����ֵ
				for(BYTE j=0; j<4; ++j)
				{
					if(cbMinSingleCardCount[j]>=cbTmpCount)
					{
						cbMinSingleCardCount[j] = cbTmpCount ;
						cbIndex[j] = i ;
						cbOutcardType[j] = cbCardType ;
						break ;
					}
				}

				//������Сֵ
				if(cbMinSingleCountInFour>=cbTmpCount)
				{
					//��С����
					cbMinCardType = cbCardType ;
					//��С�����е���С����
					cbMinSingleCountInFour=cbTmpCount ;						
					//��С�����е���С��
					cbMinIndex=i ;
				}
			}
		}

		if(cbMinSingleCountInFour!=MAX_COUNT)
		{
			BYTE Index = cbMinIndex ;

			//ѡ����С��
			for(BYTE i=0; i<4; ++i)
			{
				if(cbOutcardType[i]==cbMinCardType && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
					GetCardLogicValue(CardTypeResult[cbMinCardType].cbCardData[cbIndex[i]][0])<GetCardLogicValue(CardTypeResult[cbMinCardType].cbCardData[Index][0]))
					Index = cbIndex[i] ;
			}

			//�����һֻ
			if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
			{
				OutCardResult.cbCardCount = 2 ;
				OutCardResult.cbResultCard[0] = 0x4f ;
				OutCardResult.cbResultCard[1] = 0x4e ;
				return ;
			}
			//����
			else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
			{
				OutCardResult.cbCardCount = 2 ;
				OutCardResult.cbResultCard[0] = 0x4f ;
				OutCardResult.cbResultCard[1] = 0x4e ;
				return ;
			}
			else
			{
				//���ñ�
				OutCardResult.cbCardCount=CardTypeResult[cbMinCardType].cbEachHandCardCount[Index];
				memcpy(OutCardResult.cbResultCard,CardTypeResult[cbMinCardType].cbCardData[Index],CardTypeResult[cbMinCardType].cbEachHandCardCount[Index]*sizeof(BYTE));
				return ;
			}

			assert(OutCardResult.cbCardCount>0) ;

			return ;
		}
		//���ֻʣը��
		else if (CardTypeResult[CT_BOMB_CARD].cbCardTypeCount > 0) 
		{	
			//BYTE Index=0 ;
			//BYTE cbLogicCardValue = GetCardLogicValue(0x4F)+1 ;
			////��Сը��
			//for(BYTE i=0; i<CardTypeResult[CT_BOMB_CARD].cbCardTypeCount; ++i)
			//	if(cbLogicCardValue>GetCardLogicValue(CardTypeResult[CT_BOMB_CARD].cbCardData[i][0]))
			//	{
			//		cbLogicCardValue = GetCardLogicValue(CardTypeResult[CT_BOMB_CARD].cbCardData[i][0]) ;
			//		Index = i ;
			//	}

			//	//���ñ�
			//	OutCardResult.cbCardCount=CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index];
			//	memcpy(OutCardResult.cbResultCard,CardTypeResult[CT_BOMB_CARD].cbCardData[Index],CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index]*sizeof(BYTE));

			//	return ;
		}

		BYTE cbAllSingleCardData[MAX_COUNT], cbAllSingleCardCount ;
		cbAllSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, NULL, 0, cbAllSingleCardData) ;

		if ( cbAllSingleCardCount > 0 )
		{
			//���û������ͳ���С��һ��
			if ( 1 == m_cbUserCardCount[m_wBankerUser] ) 
			{
				OutCardResult.cbCardCount = 1 ;
				OutCardResult.cbResultCard[0] = cbAllSingleCardData[0] ;
			}
			else 
			{
				OutCardResult.cbCardCount = 1 ;
				OutCardResult.cbResultCard[0] = cbAllSingleCardData[cbAllSingleCardCount-1] ;
			}
			return ;
		}

		//���û������ͳ���С��һ��
		OutCardResult.cbCardCount = 1 ;
		OutCardResult.cbResultCard[0] = cbHandCardData[cbHandCardCount-1] ;
		return ;
}

//�����ϼң�����ƣ�
void CAndroidAI::UpsideOfBankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, WORD wOutCardUser, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult) 
{
	//���±�û��
	tagOutCardTypeResult CardTypeResult[12+1] ;
	memset(CardTypeResult, '\0',sizeof(CardTypeResult)) ;

	//��ʼ��
	memset(&OutCardResult,'\0', sizeof(OutCardResult)) ;

	//��������
	BYTE cbOutCardType = GetCardType(cbTurnCardData, cbTurnCardCount) ;

	//����ֻ��һ�ţ��Ӵ�С����
	if (m_cbUserCardCount[m_wBankerUser] == 1 && wOutCardUser != m_wBankerUser && CT_SINGLE == cbOutCardType)
	{
		BYTE cbSingleCardData[MAX_COUNT] ;
		BYTE cbSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, NULL, 0, cbSingleCardData) ;

		WORD wFriendID = (m_wBankerUser+1) % GAME_PLAYER ;
		WORD wMeID = (wFriendID+1) % GAME_PLAYER ;

		BYTE cbFriendLargestLogicCard = GetCardLogicValue(m_cbAllCardData[wFriendID][0]) ;
		BYTE cbMeLargestLogicCard = GetCardLogicValue(cbHandCardData[0]) ;
		BYTE cbTurnLogicCard = GetCardLogicValue(cbTurnCardData[0]) ;

		//ֻʣ����(���Ի�����)
		if (cbSingleCardCount == cbHandCardCount && cbFriendLargestLogicCard > cbTurnLogicCard && cbMeLargestLogicCard > cbTurnLogicCard) 
		{
			OutCardResult.cbCardCount = 1 ;
			OutCardResult.cbResultCard[0] = cbHandCardData[0] ;

			return ;
		}
	}

	//����ɳ���
	tagOutCardTypeResult BankerOutCardTypeResult[13] ;
	memset(BankerOutCardTypeResult,'\0', sizeof(BankerOutCardTypeResult)) ;

	AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], BankerOutCardTypeResult) ;
	AnalyseOutCardType(cbHandCardData,cbHandCardCount,cbTurnCardData,cbTurnCardCount, CardTypeResult) ;

	//ֻʣը��
	if(cbHandCardCount==CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[0] && (cbOutCardType<CT_BOMB_CARD || 
		GetCardLogicValue(CardTypeResult[CT_BOMB_CARD].cbCardData[0][0])>GetCardLogicValue(cbTurnCardData[0])))
	{
		OutCardResult.cbCardCount = CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[0] ;
		memcpy(OutCardResult.cbResultCard,  CardTypeResult[CT_BOMB_CARD].cbCardData, OutCardResult.cbCardCount) ;

		return ;
	}
	//˫��ը����һ��
	else if(cbHandCardCount>2 && cbHandCardData[0]==0x4f && cbHandCardData[1]==0x4e && CT_ERROR!=GetCardType(cbHandCardData+2, cbHandCardCount-2) &&
		CT_FOUR_LINE_TAKE_ONE != GetCardType(cbHandCardData+2, cbHandCardCount-2) && CT_FOUR_LINE_TAKE_TWO != GetCardType(cbHandCardData+2, cbHandCardCount-2))
	{
		OutCardResult.cbCardCount = 2 ;
		OutCardResult.cbResultCard[0] = 0x4f ;
		OutCardResult.cbResultCard[1] = 0x4e ;
		return ;
	}

	//ը����һ��
	BYTE cbRemainCard[MAX_COUNT], cbRemainCount=0;
	BYTE cbAllBombCard[MAX_COUNT], cbAllBombCount=0 ;
	GetAllBomCard(cbHandCardData, cbHandCardCount, cbAllBombCard, cbAllBombCount) ;

	//��ը�ж�
	if(cbAllBombCount>0 && m_wBankerUser==wOutCardUser)
	{
		//ʣ���˿�
		memcpy(cbRemainCard, cbHandCardData, cbHandCardCount) ;
		cbRemainCount = cbHandCardCount ;
		RemoveCard(cbAllBombCard, cbAllBombCount, cbRemainCard, cbRemainCount) ;
		cbRemainCount -= cbAllBombCount ;

		if(CT_ERROR != GetCardType(cbRemainCard, cbRemainCount) || 
			(2==cbRemainCount && GetCardLogicValue(cbRemainCard[0])>GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0])))
		{
			if((cbOutCardType<CT_BOMB_CARD || GetCardLogicValue(cbAllBombCard[0])>GetCardLogicValue(cbTurnCardData[0])) &&
				( CardTypeResult[cbOutCardType].cbCardTypeCount <= 0 || CT_ERROR != GetCardType(cbRemainCard, cbRemainCount)) )
			{
				//˫��ը��
				if(GetCardColor(cbAllBombCard[0])==0x40)
				{
					OutCardResult.cbCardCount = 2 ;
					OutCardResult.cbResultCard[0] = 0x4f ;
					OutCardResult.cbResultCard[1] = 0x4e ;
					return ;
				}
				else
				{
					//���������
					BYTE cbBankerAllBombCard[MAX_COUNT], cbBankerAllBombCardCount ;
					GetAllBomCard( m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], cbBankerAllBombCard, cbBankerAllBombCardCount) ;

					if ( !CompareCard( cbTurnCardData, cbRemainCard, cbTurnCardCount, cbRemainCount) ||  cbBankerAllBombCardCount <= 0 || 
						GetCardLogicValue( cbAllBombCard[0] ) > GetCardLogicValue( cbBankerAllBombCard[0] ) )
					{
						OutCardResult.cbCardCount = 4 ;
						memcpy(OutCardResult.cbResultCard, cbAllBombCard, 4) ;
						return ;
					}
				}
			}
		}
	}

	//ֻ��һ����
	if ( GetCardType(cbHandCardData, cbHandCardCount) != CT_FOUR_LINE_TAKE_ONE && GetCardType(cbHandCardData, cbHandCardCount) != CT_FOUR_LINE_TAKE_TWO &&
		CompareCard(cbTurnCardData, cbHandCardData, cbTurnCardCount, cbHandCardCount) )
	{
		OutCardResult.cbCardCount = cbHandCardCount ;
		memcpy(OutCardResult.cbResultCard, cbHandCardData, cbHandCardCount) ;

		return ;
	}

	//���ƽ����ж�
	if (1 == m_cbUserCardCount[m_wBankerUser] && cbHandCardCount >= 2 && cbOutCardType == CT_DOUBLE)
	{
		tagAnalyseResult AnalyseResult ;
		memset(&AnalyseResult, '\0',sizeof(AnalyseResult)) ;

		//�����˿�
		AnalysebCardData(cbHandCardData, cbHandCardCount, AnalyseResult) ;

		//�����ж�
		if (cbHandCardCount == (AnalyseResult.cbDoubleCount * 2 + AnalyseResult.cbThreeCount * 3 + AnalyseResult.cbFourCount * 4) || 
			cbHandCardCount == (AnalyseResult.cbDoubleCount * 2 + AnalyseResult.cbThreeCount * 3 + AnalyseResult.cbFourCount * 4 + 1))
		{
			//����ж�
			for (int nIndex = AnalyseResult.cbDoubleCount-1; nIndex>=0 ; --nIndex)
			{
				if (GetCardLogicValue(AnalyseResult.cbDoubleCardData[nIndex*2]) > GetCardLogicValue(cbTurnCardData[0]))
				{
					OutCardResult.cbCardCount = 2 ;
					OutCardResult.cbResultCard[0] = AnalyseResult.cbDoubleCardData[nIndex*2] ;
					OutCardResult.cbResultCard[1] = AnalyseResult.cbDoubleCardData[nIndex*2+1] ;
					return ;
				}
			}

			//��ը�ж�
			if (AnalyseResult.cbFourCount > 0) 
			{
				//��Сը��
				BYTE cbLestBombIndex = AnalyseResult.cbFourCount-1 ;

				OutCardResult.cbCardCount = 4 ;
				OutCardResult.cbResultCard[0] = AnalyseResult.cbFourCardData[cbLestBombIndex*4] ;
				OutCardResult.cbResultCard[1] = AnalyseResult.cbFourCardData[cbLestBombIndex*4+1] ;
				OutCardResult.cbResultCard[2] = AnalyseResult.cbFourCardData[cbLestBombIndex*4+2] ;
				OutCardResult.cbResultCard[3] = AnalyseResult.cbFourCardData[cbLestBombIndex*4+3] ;

				return ;
			}
            			
		}

		//����Լ�
		if ( wOutCardUser != m_wBankerUser )
		{
			tagAnalyseResult FriendAnalyseResult ;
			memset( &FriendAnalyseResult,'\0', sizeof( FriendAnalyseResult ) ) ;

			//�����˿�
			AnalysebCardData( m_cbAllCardData[wOutCardUser], m_cbUserCardCount[wOutCardUser], FriendAnalyseResult ) ;

			//�����ж�
			if (m_cbUserCardCount[wOutCardUser] == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4) || 
				m_cbUserCardCount[wOutCardUser] == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4 + 1))
			{
				return ;
			}

			//���±�û��
			tagOutCardTypeResult FriendCardTypeResult[12+1] ;
			memset( FriendCardTypeResult, '\0',sizeof( FriendCardTypeResult ) ) ;

			AnalyseOutCardType( m_cbAllCardData[wOutCardUser], m_cbUserCardCount[wOutCardUser], FriendCardTypeResult ) ;

			for ( BYTE cbLineCardIdx = 0; cbLineCardIdx < FriendCardTypeResult[CT_SINGLE_LINE].cbCardTypeCount; ++cbLineCardIdx )
			{
				//ʣ���˿�
				BYTE cbRemainCardData[MAX_COUNT], cbRemainCardCount ;

				cbRemainCardCount = m_cbUserCardCount[wOutCardUser] ;
				memcpy( cbRemainCardData, m_cbAllCardData[wOutCardUser], cbRemainCardCount ) ;
				RemoveCard( FriendCardTypeResult[CT_SINGLE_LINE].cbCardData[cbLineCardIdx],  FriendCardTypeResult[CT_SINGLE_LINE].cbEachHandCardCount[cbLineCardIdx], 
					cbRemainCardData, cbRemainCardCount ) ;

				cbRemainCardCount -= FriendCardTypeResult[CT_SINGLE_LINE].cbEachHandCardCount[cbLineCardIdx] ;

				//�����˿�
				AnalysebCardData( cbRemainCardData, cbRemainCardCount, FriendAnalyseResult ) ;

				//�����ж�
				if (cbRemainCardCount == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4) || 
					cbRemainCardCount == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4 + 1))
				{
					return ;
				}
			}

			for ( BYTE cbLineCardIdx = 0; cbLineCardIdx < FriendCardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount; ++cbLineCardIdx )
			{
				//ʣ���˿�
				BYTE cbRemainCardData[MAX_COUNT], cbRemainCardCount ;

				cbRemainCardCount = m_cbUserCardCount[wOutCardUser] ;
				memcpy( cbRemainCardData, m_cbAllCardData[wOutCardUser], cbRemainCardCount ) ;
				RemoveCard( FriendCardTypeResult[CT_SINGLE_LINE].cbCardData[cbLineCardIdx],  FriendCardTypeResult[CT_SINGLE_LINE].cbEachHandCardCount[cbLineCardIdx], 
					cbRemainCardData, cbRemainCardCount ) ;

				cbRemainCardCount -= FriendCardTypeResult[CT_SINGLE_LINE].cbEachHandCardCount[cbLineCardIdx] ;

				//�����˿�
				AnalysebCardData( cbRemainCardData, cbRemainCardCount, FriendAnalyseResult ) ;

				//�����ж�
				if (cbRemainCardCount == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4) || 
					cbRemainCardCount == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4 + 1))
				{
					return ;
				}
			}


		}
	}

	//�Լҿɷ����
	if( m_wBankerUser != wOutCardUser && ! CompareCard( cbTurnCardData, m_cbAllCardData[ m_wBankerUser ], cbTurnCardCount, m_cbUserCardCount[ m_wBankerUser ] ) )
	{
		//ׯ���˿�
		bool bBankerCanOut = false ;
		tagOutCardTypeResult BankerOutCardResult[12+1] ;
		memset(BankerOutCardResult, '\0',sizeof(BankerOutCardResult)) ;

        //�����˿�
		AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], cbTurnCardData, cbTurnCardCount, BankerOutCardResult) ;
		for(BYTE cbCardType=CT_SINGLE; cbCardType<=CT_MISSILE_CARD; ++cbCardType) if(BankerOutCardResult[cbCardType].cbCardTypeCount>0) bBankerCanOut = true ; 

		if(!bBankerCanOut)
		{
		//�Լ�ID
		WORD wFriendChairID = (m_wBankerUser+1)%GAME_PLAYER ;

		//�����˿�
		tagOutCardTypeResult FriendCardTypeResult[12+1] ;
		memset(FriendCardTypeResult, '\0',sizeof(FriendCardTypeResult)) ;
		AnalyseOutCardType(m_cbAllCardData[wFriendChairID], m_cbUserCardCount[wFriendChairID], FriendCardTypeResult) ;

		for(BYTE cbCardType=CT_SINGLE; cbCardType<=CT_MISSILE_CARD; ++cbCardType)
			if(FriendCardTypeResult[cbCardType].cbCardTypeCount>0)
			{
				for(LONG lIndex=0; lIndex<FriendCardTypeResult[cbCardType].cbCardTypeCount; ++lIndex)
				{
					if(TestOutAllCard(wFriendChairID, FriendCardTypeResult[cbCardType].cbCardData[lIndex], FriendCardTypeResult[cbCardType].cbEachHandCardCount[lIndex], true))
					{
						//��ѹ�Լ�
						return ;
					}
				}
			}
		}
	}

	//���߶Լ�
	if (GetCardType(m_cbAllCardData[(m_wBankerUser + 1) % GAME_PLAYER], m_cbUserCardCount[(m_wBankerUser + 1) % GAME_PLAYER]) == GetCardType(cbTurnCardData, cbTurnCardCount) &&
		CompareCard(cbTurnCardData, m_cbAllCardData[(m_wBankerUser + 1) % GAME_PLAYER], cbTurnCardCount, m_cbUserCardCount[(m_wBankerUser + 1) % GAME_PLAYER]) &&
		!CompareCard(cbTurnCardData, m_cbAllCardData[m_wBankerUser], cbTurnCardCount, m_cbUserCardCount[m_wBankerUser]))
		return ;

	if (CompareCard(cbTurnCardData, m_cbAllCardData[(m_wBankerUser + 1) % GAME_PLAYER], cbTurnCardCount, m_cbUserCardCount[(m_wBankerUser + 1) % GAME_PLAYER]) &&
		!CompareCard(cbTurnCardData, m_cbAllCardData[m_wBankerUser], cbTurnCardCount, m_cbUserCardCount[m_wBankerUser])) return ;

	//�жϿɷ����
	BYTE cbSingleCardCount = MAX_COUNT+CT_MISSILE_CARD ;
	bool bFindBestCard = false ;
	for(BYTE cbCardType=CT_SINGLE; cbCardType<=CT_MISSILE_CARD; ++cbCardType)
		if(CardTypeResult[cbCardType].cbCardTypeCount>0)
		{
			for(LONG lIndex=0; lIndex<CardTypeResult[cbCardType].cbCardTypeCount; ++lIndex)
			{
				WORD wMeChairID = (m_wBankerUser+2)%GAME_PLAYER ;

				if(TestOutAllCard(wMeChairID, CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex], false))
				{
					//���㵥��
					BYTE cbTmpSingleCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CardTypeResult[cbCardType].cbCardData[lIndex], 
						CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]) ;

					//����ж�
					if (cbTmpSingleCount >= MAX_COUNT) continue ;

					//ը�����ȼ��ź�
					BYTE cbBombCardType = GetCardType(CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]) ;
					if (cbBombCardType == CT_BOMB_CARD) cbTmpSingleCount += 4 ;
					else if (cbBombCardType == CT_MISSILE_CARD) cbTmpSingleCount += 5 ;

					////�ı�Ȩֵ
					//if (cbBombCardType != CT_ERROR) cbTmpSingleCount += cbBombCardType ;
					
					//����ը��
					//BYTE cbWantOutCardType = GetCardType(CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]) ;
					//if (CardTypeResult[cbOutCardType].cbCardTypeCount > 0 && cbOutCardType < CT_BOMB_CARD && cbWantOutCardType >= CT_BOMB_CARD) continue ;


					if(cbTmpSingleCount <= cbSingleCardCount)
					{
						//���ñ�
						OutCardResult.cbCardCount=CardTypeResult[cbCardType].cbEachHandCardCount[lIndex];
						memcpy(OutCardResult.cbResultCard, CardTypeResult[cbCardType].cbCardData[lIndex],
						CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]*sizeof(BYTE));
						cbSingleCardCount = cbTmpSingleCount ;

						bFindBestCard = true ;
					}
				}
			}
		}
		//ֱ�ӷ���
		if (bFindBestCard) return ;

	//���ׯ��û�д���������ѹ�Լ���
	if( m_cbUserCardCount[m_wBankerUser]<=5 && wOutCardUser!=m_wBankerUser &&
		(BankerOutCardTypeResult[cbOutCardType].cbCardTypeCount==0 ||
		GetCardLogicValue(BankerOutCardTypeResult[cbOutCardType].cbCardData[0][0])<=GetCardLogicValue(cbTurnCardData[0])) &&
		CardTypeResult[cbOutCardType].cbEachHandCardCount[0]!=cbHandCardCount)//����һ�γ���
	{
		//�������
		return ;
	}

	//�¼�Ϊ������ҵ����˿�����5��
	if(m_cbUserCardCount[m_wBankerUser]<=5 && CardTypeResult[cbOutCardType].cbCardTypeCount>0 && cbOutCardType!=CT_BOMB_CARD && 
		((GetCardLogicValue(cbTurnCardData[0])<12 && wOutCardUser!=m_wBankerUser && BankerOutCardTypeResult[cbOutCardType].cbCardTypeCount>0) ||//�Լҳ���
		(wOutCardUser==m_wBankerUser)))//�������
	{
		//��ֹ���ȴ���Ƴ�ȥ
		BYTE Index = cbOutCardType == CT_SINGLE ? 0 : CardTypeResult[cbOutCardType].cbCardTypeCount - 1 ;
		//Ѱ�ҿ���ѹס�������Сһ����
		BYTE cbThisOutTypeMinSingleCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CardTypeResult[cbOutCardType].cbCardData[0], CardTypeResult[cbOutCardType].cbEachHandCardCount[0]) ;
		BYTE cbBestIndex = 255 ;
		for(BYTE i=0; i<CardTypeResult[cbOutCardType].cbCardTypeCount; ++i)
		{
			BYTE cbTmpSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CardTypeResult[cbOutCardType].cbCardData[i], CardTypeResult[cbOutCardType].cbEachHandCardCount[i]) ;
			if((BankerOutCardTypeResult[cbOutCardType].cbCardTypeCount>0 && 
				GetCardLogicValue(CardTypeResult[cbOutCardType].cbCardData[i][0])>=GetCardLogicValue(BankerOutCardTypeResult[cbOutCardType].cbCardData[0][0]) ||
				BankerOutCardTypeResult[cbOutCardType].cbCardTypeCount==0) && 
				cbTmpSingleCardCount<=cbThisOutTypeMinSingleCount)
			{
				cbBestIndex = i ;
				cbThisOutTypeMinSingleCount = cbTmpSingleCardCount ;
			}

			if((BankerOutCardTypeResult[cbOutCardType].cbCardTypeCount>0 && 
				GetCardLogicValue(CardTypeResult[cbOutCardType].cbCardData[i][0])>=GetCardLogicValue(BankerOutCardTypeResult[cbOutCardType].cbCardData[0][0]) ||
				BankerOutCardTypeResult[cbOutCardType].cbCardTypeCount==0))
				Index = i ;
			else break ;
		}

		if(cbBestIndex!=255)
		{
			OutCardResult.cbCardCount = CardTypeResult[cbOutCardType].cbEachHandCardCount[cbBestIndex] ;
			memcpy(OutCardResult.cbResultCard, CardTypeResult[cbOutCardType].cbCardData[cbBestIndex], OutCardResult.cbCardCount) ;
		}
		else
		{
			OutCardResult.cbCardCount = CardTypeResult[cbOutCardType].cbEachHandCardCount[Index] ;
			memcpy(OutCardResult.cbResultCard, CardTypeResult[cbOutCardType].cbCardData[Index], OutCardResult.cbCardCount) ;
		}

		return ;
	}

	//���ƶ���
	if (CT_SINGLE == cbOutCardType && CardTypeResult[cbOutCardType].cbCardTypeCount > 0)
	{
		BYTE cbMeSingleCardData[MAX_COUNT], cbMeSingleCardCount ;
		BYTE cbBankerSingleCardData[MAX_COUNT], cbBankerSingleCardCount ;

		//��ȡ����
		cbMeSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, NULL, 0, cbMeSingleCardData) ;
		cbBankerSingleCardCount = AnalyseSinleCardCount(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], NULL, 0, cbBankerSingleCardData) ;

		//������С��
		if (cbBankerSingleCardCount > 0 && cbMeSingleCardCount > 0 && GetCardLogicValue(cbBankerSingleCardData[cbBankerSingleCardCount-1]) <= 10)
		{
			//9��}��
			if (cbBankerSingleCardCount >= 2 && GetCardValue(cbBankerSingleCardData[cbBankerSingleCardCount-2]) <= 10)
			{
				for (int nMeIndex = cbMeSingleCardCount-1; nMeIndex >=0 ; --nMeIndex)
					if (GetCardLogicValue(cbMeSingleCardData[nMeIndex]) > GetCardLogicValue(cbTurnCardData[0]) && 
						GetCardLogicValue(cbMeSingleCardData[nMeIndex]) >= GetCardLogicValue(cbBankerSingleCardData[cbBankerSingleCardCount-2]) &&
						GetCardLogicValue(cbMeSingleCardData[nMeIndex]) <= 15)
					{
						OutCardResult.cbCardCount = 1 ;
						OutCardResult.cbResultCard[0] = cbMeSingleCardData[nMeIndex] ;

						return ;
					}
			}	

			//9��һ��
			for (int nMeIndex = cbMeSingleCardCount-1; nMeIndex >=0 ; --nMeIndex)
				if (GetCardLogicValue(cbMeSingleCardData[nMeIndex]) > GetCardLogicValue(cbTurnCardData[0]) && 
					GetCardLogicValue(cbMeSingleCardData[nMeIndex]) >= GetCardLogicValue(cbBankerSingleCardData[cbBankerSingleCardCount-1]) &&
					GetCardLogicValue(cbMeSingleCardData[nMeIndex]) <= 15)
				{
					OutCardResult.cbCardCount = 1 ;
					OutCardResult.cbResultCard[0] = cbMeSingleCardData[nMeIndex] ;

					return ;
				}
		}
	}

	//ȡ���ĸ���С����
	BYTE cbMinSingleCardCount[4] ;
	cbMinSingleCardCount[0]=MAX_COUNT ;
	cbMinSingleCardCount[1]=MAX_COUNT ;
	cbMinSingleCardCount[2]=MAX_COUNT ;
	cbMinSingleCardCount[3]=MAX_COUNT ;
	BYTE cbIndex[4]={0} ;	
	BYTE cbMinSingleCountInFour=MAX_COUNT ;

	//�ɳ��˿ˣ������Ѿ����˵�ը���ˣ�
	tagOutCardTypeResult const &CanOutCard = CardTypeResult[cbOutCardType] ;

	for(BYTE i=0; i<CanOutCard.cbCardTypeCount; ++i)
	{
		//��С����
		BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount,CanOutCard.cbCardData[i], CanOutCard.cbEachHandCardCount[i]) ; 
		BYTE cbMaxValue=0 ; 
		BYTE Index = 0 ;

		//����cbMinSingleCardCount[4]�����ֵ
		for(BYTE j=0; j<4; ++j)
		{
			if(cbMinSingleCardCount[j]>=cbTmpCount)
			{
				cbMinSingleCardCount[j] = cbTmpCount ;
				cbIndex[j] = i ;
				break ;
			}
		}

	}

	for(BYTE i=0; i<4; ++i)
		if(cbMinSingleCountInFour>cbMinSingleCardCount[i]) cbMinSingleCountInFour = cbMinSingleCardCount[i] ;


	//ԭʼ������
	BYTE cbOriginSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount,NULL,0) ;

	//����������
	BYTE cbBankerDoubleCardData[MAX_COUNT], cbBankerDoubleCardCount ;
	GetAllDoubleCard(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], cbBankerDoubleCardData, cbBankerDoubleCardCount) ;

	//���ѳ���
	bool bFriendOut = m_wBankerUser!=wOutCardUser ;
	if(bFriendOut)
	{
		//��9���������һ����
		if (GetCardType(m_cbAllCardData[(1 + m_wBankerUser) % GAME_PLAYER], m_cbUserCardCount[(1 + m_wBankerUser) % GAME_PLAYER]) != CT_ERROR) return ;

		//�������TestOutAllCard���ѶԿɳ�ը���������
		if(CanOutCard.cbCardTypeCount>0 && CanOutCard.cbCardType < CT_BOMB_CARD)
		{
			//���������
			BYTE cbBankerSingleCardData[MAX_COUNT] ;
			BYTE cbBankerSingleCardCount=AnalyseSinleCardCount(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], NULL, 0, cbBankerSingleCardData) ;
			BYTE cbBankerSingleCardLogic = 0 ;

			if(cbBankerSingleCardCount>=2 && GetCardLogicValue(cbBankerSingleCardData[cbBankerSingleCardCount-2])<=10) cbBankerSingleCardLogic = GetCardLogicValue(cbBankerSingleCardData[cbBankerSingleCardCount-2]) ;
			else if(cbBankerSingleCardCount>=2 && GetCardLogicValue(cbBankerSingleCardData[cbBankerSingleCardCount-1])<=10) cbBankerSingleCardLogic = GetCardLogicValue(cbBankerSingleCardData[cbBankerSingleCardCount-1]) ;
			else if(cbBankerSingleCardCount>0 && GetCardLogicValue(cbBankerSingleCardData[0])<=10) cbBankerSingleCardLogic = GetCardLogicValue(cbBankerSingleCardData[0]) ;

			BYTE cbMinLogicCardValue = GetCardLogicValue(0x4F)+1 ;
			bool bFindCard = false ;
			BYTE cbCanOutIndex=0 ;
			for(BYTE i=0; i<4; ++i)
			{
				BYTE Index = cbIndex[i] ;

				//����l�Ʋ��ӶԼ���
				if ( CanOutCard.cbCardType >= CT_THREE &&  CanOutCard.cbCardType <= CT_MISSILE_CARD && GetCardLogicValue(CanOutCard.cbCardData[Index][0]) >= 7 && 
					CanOutCard.cbEachHandCardCount[Index] <=5 )
					continue ;

				//����9��
				bool bCanOut = false ;
				if(cbOutCardType==CT_SINGLE && cbBankerSingleCardCount > 0 && GetCardLogicValue(CanOutCard.cbCardData[Index][0]) >= cbBankerSingleCardLogic &&
					GetCardLogicValue(cbTurnCardData[0]) < 14 && cbMinSingleCardCount[i] < cbOriginSingleCardCount &&
					GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) > GetCardLogicValue(cbTurnCardData[0]))
					bCanOut = true ;

				//9�ض���
				//if (cbOutCardType == CT_DOUBLE && cbBankerDoubleCardCount > 0 && GetCardLogicValue(cbBankerDoubleCardData[cbBankerDoubleCardCount-1]) < 10 &&
				//	GetCardLogicValue(CanOutCard.cbCardData[Index][0]) < GetCardLogicValue(cbBankerDoubleCardData[cbBankerDoubleCardCount-1]) &&
				//	GetCardLogicValue(CanOutCard.cbCardData[0][0]) >= 10 && GetCardLogicValue(CanOutCard.cbCardData[0][0]) < 14) continue ;

				//С��J���ƣ�����С��K������ɢ��
				if(bCanOut ||
					((cbMinSingleCardCount[i]<cbOriginSingleCardCount+3 &&  (cbMinSingleCardCount[i]<=cbMinSingleCountInFour || cbMinSingleCardCount[i]<=cbMinSingleCountInFour+1 && 
				CanOutCard.cbCardType >= CT_THREE_LINE_TAKE_ONE && CanOutCard.cbCardType <= CT_THREE_LINE_TAKE_TWO ) && 
					(GetCardLogicValue(CanOutCard.cbCardData[Index][0])<=11 || (cbMinSingleCardCount[i]<cbOriginSingleCardCount)&&GetCardLogicValue(CanOutCard.cbCardData[Index][0])<=13)) &&
					cbMinLogicCardValue>GetCardLogicValue(CanOutCard.cbCardData[Index][0]) && cbHandCardCount>5))
				{
					//������û�д��ƣ���Էɻ�������Ĵ��ƣ�
					bool bNoLargeCard = true ;
					for(BYTE k=3; k<CanOutCard.cbEachHandCardCount[Index]; ++k)
					{
						//�д��ƶ��Ҳ���һ�γ���
						if(GetCardLogicValue(CanOutCard.cbCardData[Index][k])>=15 && 
							CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount) bNoLargeCard = false ;
					}
					if(bNoLargeCard)
					{
						bFindCard = true ;
						cbCanOutIndex = Index ; 
						cbMinLogicCardValue = GetCardLogicValue(CanOutCard.cbCardData[Index][0]) ;
					}
				}
				else if(cbHandCardCount<5 && cbMinSingleCardCount[i]<cbOriginSingleCardCount+4 && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
					cbMinLogicCardValue>GetCardLogicValue(CanOutCard.cbCardData[Index][0]))
				{
					//�ܳ�����Լҵ�2
					if ( GetCardLogicValue( CanOutCard.cbCardData[Index][0] ) >= 16 && GetCardLogicValue( cbTurnCardData[0] ) >= 15 )
						continue ;

					bFindCard = true ;
					cbCanOutIndex = Index ; 
					cbMinLogicCardValue = GetCardLogicValue(CanOutCard.cbCardData[Index][0]) ;
				}
			}

			if(bFindCard)
			{

				//���ñ�
				OutCardResult.cbCardCount=CanOutCard.cbEachHandCardCount[cbCanOutIndex];
				memcpy(OutCardResult.cbResultCard,CanOutCard.cbCardData[cbCanOutIndex],CanOutCard.cbEachHandCardCount[cbCanOutIndex]*sizeof(BYTE));

				return ;
			}
			//��������������
			else if(cbHandCardCount<=5)
			{
				BYTE cbMinLogicCard = GetCardLogicValue(0x4f)+1 ;
				BYTE cbCanOutIndex = 0 ;
				for(BYTE i=0; i<4; ++i)
					if(cbMinSingleCardCount[i]<MAX_COUNT && cbMinSingleCardCount[i]<=cbMinSingleCountInFour && cbMinLogicCard>GetCardLogicValue(CanOutCard.cbCardData[cbIndex[i]][0]) && 
						GetCardLogicValue(CanOutCard.cbCardData[cbIndex[i]][0])<=14)
					{
						cbMinLogicCard = GetCardLogicValue(CanOutCard.cbCardData[cbIndex[i]][0]) ;
						cbCanOutIndex = cbIndex[i] ;
					}

					if(cbMinLogicCard != (GetCardLogicValue(0x4f)+1))
					{
						//���ñ�
						OutCardResult.cbCardCount=CanOutCard.cbEachHandCardCount[cbCanOutIndex];
						memcpy(OutCardResult.cbResultCard,CanOutCard.cbCardData[cbCanOutIndex],CanOutCard.cbEachHandCardCount[cbCanOutIndex]*sizeof(BYTE));

						return ;
					}
			}

			return ;
		}
		else
		{
			return ;
		}

	}
	//�������
	else
	{
		if(CanOutCard.cbCardTypeCount>0)
		{
			BYTE cbMinLogicCardValue = GetCardLogicValue(0x4F)+1 ;
			bool bFindCard = false ;
			BYTE cbCanOutIndex=0 ;
			for(BYTE i=0; i<4; ++i)
			{
				BYTE Index = cbIndex[i] ;

				if((cbMinSingleCardCount[i]<cbOriginSingleCardCount+4)  &&  (cbMinSingleCardCount[i]<=cbMinSingleCountInFour || cbMinSingleCardCount[i]<=cbMinSingleCountInFour+1 && 
				CanOutCard.cbCardType >= CT_THREE_LINE_TAKE_ONE && CanOutCard.cbCardType <= CT_THREE_LINE_TAKE_TWO ) &&
					cbMinLogicCardValue>GetCardLogicValue(CanOutCard.cbCardData[Index][0]))
				{
					//��Դ���
					bool bNoLargeCard = true ;

					//�����������������4�����ҵ�������С��K���ƶ��Ҳ��ǵ������ϵ������ʱ�����ܳ�2ȥ��
					if(m_cbUserCardCount[m_wBankerUser]>=4 && cbHandCardCount>=5  && CanOutCard.cbEachHandCardCount[Index]>=2 && 
						GetCardLogicValue(CanOutCard.cbCardData[Index][0])>=15 &&
						GetCardLogicValue(cbTurnCardData[0])<13 &&
						GetCardLogicValue(cbTurnCardData[0])<GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) && 
						CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount)
						bNoLargeCard=false ;

					//������û�д��ƣ���Էɻ�������Ĵ��ƣ�
					for(BYTE k=3; k<CanOutCard.cbEachHandCardCount[Index]; ++k)
					{
						if(GetCardLogicValue(CanOutCard.cbCardData[Index][k])>=15 && 
							CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount)
							bNoLargeCard = false ;
					}
					if(bNoLargeCard)
					{
						bFindCard = true ;
						cbCanOutIndex = Index ; 
						cbMinLogicCardValue = GetCardLogicValue(CanOutCard.cbCardData[Index][0]) ;
					}
				}
			}

			if(bFindCard)
			{
				//����������
				BYTE cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) ;
				bool bCanOut=true ;

				//��ֻѹ2
				if(GetCardLogicValue(cbTurnCardData[0])<cbLargestLogicCard)
				{
					if(GetCardColor(CanOutCard.cbCardData[cbCanOutIndex][0])==0x40 && GetCardLogicValue(cbTurnCardData[0])<=14 && cbHandCardCount>5) 								
					{
						bCanOut = false ;
					}
				}
				 //˫���ж�
				if(GetCardLogicValue(CanOutCard.cbCardData[cbCanOutIndex][0])>=16 && cbHandCardCount>=2 && cbHandCardData[0]==0x4F && cbHandCardData[1]==0x4E)
				{
					bool bOutMissileCard = false ;
					//һ���ƺ�һ��ը��
					BYTE cbRemainCardData[20], cbRemainCardCount=cbHandCardCount ;
					memcpy(cbRemainCardData, cbHandCardData, cbHandCardCount) ;
					RemoveCard(cbRemainCardData, 2, cbRemainCardData, cbRemainCardCount) ;
					cbRemainCardCount -= 2 ;
					if(CT_ERROR!=GetCardType(cbRemainCardData, cbRemainCardCount)) bOutMissileCard = true;

					//ֻʣ����
					BYTE cbRemainLargestCard = GetCardLogicValue(cbRemainCardData[0]) ;
					if(cbRemainCardCount<5 && cbRemainCardCount>0 && GetCardLogicValue(cbRemainCardData[0])>=14) bOutMissileCard = true;

					//ը������
					BYTE cbSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CanOutCard.cbCardData[cbCanOutIndex],
						CanOutCard.cbEachHandCardCount[cbCanOutIndex]) ;
					if(cbSingleCardCount<=1 && GetCardLogicValue(cbRemainCardData[0])>=11) bOutMissileCard = true;

					//����С��
					if (GetCardLogicValue(cbRemainCardData[0]) <= 10 && CT_ERROR == GetCardType(cbRemainCardData, cbRemainCardCount) && 
						GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) > 10)
						bOutMissileCard = false ;

					//����˿�
					if(bOutMissileCard)
					{
						//��������ը��
						BYTE cbIndex = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount - 1 ;

						OutCardResult.cbCardCount = CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[cbIndex] ;
						memcpy(OutCardResult.cbResultCard, CardTypeResult[CT_BOMB_CARD].cbCardData[cbIndex], OutCardResult.cbCardCount) ;
						return ;
					}
				}

				if(bCanOut)
				{
					//���ñ�
					OutCardResult.cbCardCount=CanOutCard.cbEachHandCardCount[cbCanOutIndex];
					memcpy(OutCardResult.cbResultCard,CanOutCard.cbCardData[cbCanOutIndex],CanOutCard.cbEachHandCardCount[cbCanOutIndex]*sizeof(BYTE));

					return ;
				}
			}

			if(cbOutCardType==CT_SINGLE)
			{
				//����������
				BYTE cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) ;

				if(GetCardLogicValue(cbTurnCardData[0])==14 || 
					GetCardLogicValue(cbTurnCardData[0])>=cbLargestLogicCard || 
					(GetCardLogicValue(cbTurnCardData[0])<cbLargestLogicCard-1) ||
					m_cbUserCardCount[m_wBankerUser]<=5)
				{
					//ȡһ�Ŵ��ڵ���2����Ҫ�ȵ������ƴ���ƣ�
					BYTE cbIndex=MAX_COUNT ;
					for(BYTE i=0; i<cbHandCardCount; ++i)
						if(GetCardLogicValue(cbHandCardData[i])>GetCardLogicValue(cbTurnCardData[0]) &&	GetCardLogicValue(cbHandCardData[i])>=15)
						{
							cbIndex = i ;
						}
						if(cbIndex!=MAX_COUNT)
						{
							//���ñ�
							OutCardResult.cbCardCount=1;
							OutCardResult.cbResultCard[0] = cbHandCardData[cbIndex] ;

							return ;
						}
				}
			}
		}

		//��Ҫ����ը��
		if(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount>0 || 
			(NORMAL_COUNT  == cbHandCardCount&& NORMAL_COUNT == m_cbUserCardCount[(m_wBankerUser+1)%GAME_PLAYER]))
		{
			tagOutCardTypeResult const &BomCard = CardTypeResult[CT_BOMB_CARD] ;
			BYTE cbMinLogicValue = GetCardLogicValue(BomCard.cbCardData[0][0]) ;
			BYTE Index = 0 ;
			for(BYTE i=0; i<BomCard.cbCardTypeCount; ++i)
			{
				if(cbMinLogicValue>GetCardLogicValue(BomCard.cbCardData[i][0]))
				{
					cbMinLogicValue = GetCardLogicValue(BomCard.cbCardData[i][0]) ;
					Index = i ;
				}
			}

			bool bOutBomb = false ;

			//�����ж�
			if (NORMAL_COUNT  == cbHandCardCount && NORMAL_COUNT == m_cbUserCardCount[(m_wBankerUser+1)%GAME_PLAYER] && CT_ERROR != GetCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser]))
				bOutBomb = true ;

			//һ���ƺ�һ��ը��
			BYTE cbRemainCardData[20], cbRemainCardCount=cbHandCardCount ;
			memcpy(cbRemainCardData, cbHandCardData, cbHandCardCount) ;
			RemoveCard(BomCard.cbCardData[Index], BomCard.cbEachHandCardCount[Index], cbRemainCardData, cbRemainCardCount) ;
			cbRemainCardCount -= BomCard.cbEachHandCardCount[Index] ;
			if(CT_ERROR!=GetCardType(cbRemainCardData, cbRemainCardCount)) bOutBomb = true ;

			//ը������
			BYTE cbSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, BomCard.cbCardData[Index],BomCard.cbEachHandCardCount[Index]) ;
			if(cbSingleCardCount==0 && GetCardLogicValue(cbRemainCardData[0]) > GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0])) 
				bOutBomb = true ;

			//ֻʣһ��
			BYTE cbRemainCardType = GetCardType(m_cbAllCardData[wOutCardUser], m_cbUserCardCount[wOutCardUser]) ;
			if(cbRemainCardType>CT_ERROR && cbRemainCardType<CT_FOUR_LINE_TAKE_ONE && GetCardLogicValue(m_cbAllCardData[wOutCardUser][0])<15 && 
				cbSingleCardCount < 2 && (GetCardLogicValue(cbRemainCardData[0]) >= GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0])))
				bOutBomb = true ;

			//ֻʣ����
			BYTE cbRemainLargestCard = GetCardLogicValue(cbRemainCardData[0]) ;
			if(cbRemainCardCount<5 && cbRemainCardCount>0 && (cbRemainLargestCard!=GetCardLogicValue(BomCard.cbCardData[Index][0])) && 
				cbRemainLargestCard>GetCardLogicValue(m_cbAllCardData[wOutCardUser][0]) && cbRemainLargestCard > 14) bOutBomb = true ;

			//�����˿�
			tagAnalyseResult AnalyseResult ;
			AnalysebCardData(cbRemainCardData, cbRemainCardCount, AnalyseResult) ;

			if (m_cbUserCardCount[m_wBankerUser] ==1 &&  (AnalyseResult.cbDoubleCount * 2 + AnalyseResult.cbThreeCount * 3 + AnalyseResult.cbFourCount * 4 + 1 >= cbRemainCardCount)) bOutBomb = true ;

			//���ñ�
			if(bOutBomb)
			{
				OutCardResult.cbCardCount=BomCard.cbEachHandCardCount[Index];
				memcpy(OutCardResult.cbResultCard,BomCard.cbCardData[Index],BomCard.cbEachHandCardCount[Index]*sizeof(BYTE));
			}

			return ;
		}

		return ;
	}
	return ;
}

//�����¼ң��ȳ��ƣ�
void CAndroidAI::UndersideOfBankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, WORD wMeChairID,tagOutCardResult & OutCardResult) 
{
	//���±�û��
	tagOutCardTypeResult CardTypeResult[12+1] ;
	memset(CardTypeResult, '\0',sizeof(CardTypeResult)) ;

	//��ʼ��
	memset(&OutCardResult, '\0',sizeof(OutCardResult)) ;

	BYTE cbLineCard[MAX_COUNT] ;
	BYTE cbThreeLineCard[MAX_COUNT] ;
	BYTE cbDoubleLineCard[MAX_COUNT] ;
	BYTE cbLineCardCount;
	BYTE cbThreeLineCardCount ;
	BYTE cbDoubleLineCount ;
	GetAllLineCard(cbHandCardData, cbHandCardCount, cbLineCard, cbLineCardCount) ;
	GetAllThreeCard(cbHandCardData, cbHandCardCount, cbThreeLineCard, cbThreeLineCardCount) ;
	GetAllDoubleCard(cbHandCardData, cbHandCardCount, cbDoubleLineCard, cbDoubleLineCount) ;

	//�жϿɷ����
	BYTE cbSingleCardCount = MAX_COUNT+CT_MISSILE_CARD ;
	bool bFindBestCard = false ;
	AnalyseOutCardType(cbHandCardData, cbHandCardCount, CardTypeResult) ;
	for(BYTE cbCardType=CT_SINGLE; cbCardType<=CT_MISSILE_CARD; ++cbCardType)
		if(CardTypeResult[cbCardType].cbCardTypeCount>0)
		{
			for(LONG lIndex=0; lIndex<CardTypeResult[cbCardType].cbCardTypeCount; ++lIndex)
			{
				WORD wMeChairID = (m_wBankerUser+1)%GAME_PLAYER ;

				if(TestOutAllCard(wMeChairID, CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex], true))
				{
					//���㵥��
					BYTE cbTmpSingleCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CardTypeResult[cbCardType].cbCardData[lIndex], 
						CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]) ;

					//����ж�
					if (cbTmpSingleCount >= MAX_COUNT) continue ;

					//ը�����ȼ��ź�
					BYTE cbBombCardType = GetCardType(CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]) ;
					if (cbBombCardType == CT_BOMB_CARD) cbTmpSingleCount += 4 ;
					else if (cbBombCardType == CT_MISSILE_CARD) cbTmpSingleCount += 5 ;
					else if ( 15 == GetCardLogicValue( CardTypeResult[ cbCardType ].cbCardData[ lIndex ][ 0 ] ) ) cbTmpSingleCount += 2;
					else if ( 15 < GetCardLogicValue( CardTypeResult[ cbCardType ].cbCardData[ lIndex ][ 0 ] ) ) cbTmpSingleCount += 3;


					////�ı�Ȩֵ
					//if (cbBombCardType != CT_ERROR) cbTmpSingleCount += cbBombCardType ;
					
					if(cbTmpSingleCount <= cbSingleCardCount)
					{
						//���ñ�
						OutCardResult.cbCardCount=CardTypeResult[cbCardType].cbEachHandCardCount[lIndex];
						memcpy(OutCardResult.cbResultCard, CardTypeResult[cbCardType].cbCardData[lIndex],
						CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]*sizeof(BYTE));
						cbSingleCardCount = cbTmpSingleCount ;

                        bFindBestCard = true ;
					}
				}
			}
		}
	//ֱ�ӷ���
	if (bFindBestCard) return ;

	//�����}��
	if ( cbHandCardCount == 4 && GetCardLogicValue(cbHandCardData[1]) == 16 && m_cbUserCardCount[m_wBankerUser] == 1 && 
		GetCardLogicValue(cbHandCardData[2]) < GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]))
	{
		OutCardResult.cbCardCount = 1 ;
		OutCardResult.cbResultCard[0] = cbHandCardData[2] ;
		return ;
	}

	//�Ĵ������ж�
	if ( AnalyseFourCardType(cbHandCardData, cbHandCardCount, m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], OutCardResult ) )
	{
		return ;
	}


	//�����˳�ƺ͵�ֻ��һ�ԣ����ҵ�ֻ��Աȵ����С�����ȳ�˳
	{
		if(cbLineCardCount+1==cbHandCardCount && CT_SINGLE==GetCardType(cbLineCard, cbLineCardCount))
		{
			OutCardResult.cbCardCount = cbLineCardCount ;
			memcpy(OutCardResult.cbResultCard, cbLineCard, cbLineCardCount) ;
		}
		else if(cbThreeLineCardCount+1==cbHandCardCount && CT_THREE_LINE==GetCardType(cbThreeLineCard, cbThreeLineCardCount))
		{
			OutCardResult.cbCardCount = cbThreeLineCardCount ;
			memcpy(OutCardResult.cbResultCard, cbThreeLineCard, cbThreeLineCardCount) ;
		}
		else if(cbDoubleLineCount+1==cbHandCardCount && CT_DOUBLE_LINE==GetCardType(cbDoubleLineCard, cbDoubleLineCount))
		{
			OutCardResult.cbCardCount = cbDoubleLineCount ;
			memcpy(OutCardResult.cbResultCard, cbDoubleLineCard, cbDoubleLineCount) ;
		}
		//˫��ը����һ��
		else if(cbHandCardCount>2 && cbHandCardData[0]==0x4f && cbHandCardData[1]==0x4e && CT_ERROR!=GetCardType(cbHandCardData+2, cbHandCardCount-2) &&
			CT_FOUR_LINE_TAKE_ONE != GetCardType(cbHandCardData+2, cbHandCardCount-2) && CT_FOUR_LINE_TAKE_TWO != GetCardType(cbHandCardData+2, cbHandCardCount-2))
		{
			OutCardResult.cbCardCount = 2 ;
			OutCardResult.cbResultCard[0] = 0x4f ;
			OutCardResult.cbResultCard[1] = 0x4e ;
		}

		if(OutCardResult.cbCardCount>0)
		{
			return ;
		}
	}
	//�����һֻ
	if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
	{
		OutCardResult.cbCardCount = 2 ;
		OutCardResult.cbResultCard[0] = 0x4f ;
		OutCardResult.cbResultCard[1] = 0x4e ;
		return ;
	}
	//����
	else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
	{
		OutCardResult.cbCardCount = 2 ;
		OutCardResult.cbResultCard[0] = 0x4f ;
		OutCardResult.cbResultCard[1] = 0x4e ;
		return ;
	}
	//ֻʣһ����
	else if(CT_ERROR!=GetCardType(cbHandCardData, cbHandCardCount) && CT_FOUR_LINE_TAKE_ONE!=GetCardType(cbHandCardData, cbHandCardCount) && CT_FOUR_LINE_TAKE_TWO!=GetCardType(cbHandCardData, cbHandCardCount))
	{
		OutCardResult.cbCardCount = cbHandCardCount ;
		memcpy(OutCardResult.cbResultCard, cbHandCardData, cbHandCardCount) ;
		return ;
	}

	//ֻʣһ�ź�һ��
	if(cbHandCardCount>=2)
	{
		//�����˿�
		tagOutCardTypeResult BankerCanOutCardType1[13] ;
		memset(BankerCanOutCardType1,'\0', sizeof(BankerCanOutCardType1)) ;
		tagOutCardTypeResult BankerCanOutCardType2[13] ;
		memset(BankerCanOutCardType2,'\0', sizeof(BankerCanOutCardType2)) ;

		BYTE cbFirstHandCardType = GetCardType(cbHandCardData, cbHandCardCount-1) ;
		BYTE cbSecondHandCardType = GetCardType(cbHandCardData+1, cbHandCardCount-1) ;

		//�Ƿ���ը
		BYTE cbAllBombCardData[MAX_COUNT], cbAllBombCount=0 ;
		GetAllBomCard(cbHandCardData, cbHandCardCount, cbAllBombCardData, cbAllBombCount) ;

		//û��ը��
		if (cbAllBombCount <= 0 && cbFirstHandCardType!=CT_THREE_LINE_TAKE_ONE && cbFirstHandCardType!=CT_THREE_LINE_TAKE_TWO)
		{
		//������Գ����
		if(cbFirstHandCardType!=CT_ERROR)
			AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], cbHandCardData, cbHandCardCount-1, BankerCanOutCardType1) ;
		if(cbSecondHandCardType!=CT_ERROR)
			AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], cbHandCardData+1, cbHandCardCount-1, BankerCanOutCardType2) ;

		if(cbSecondHandCardType!=CT_ERROR && cbSecondHandCardType!=CT_FOUR_LINE_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_LINE_TAKE_TWO && 
			BankerCanOutCardType2[cbSecondHandCardType].cbCardTypeCount==0 && BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
		{
			OutCardResult.cbCardCount = cbHandCardCount-1 ;
			memcpy(OutCardResult.cbResultCard, cbHandCardData+1, cbHandCardCount-1) ;
			return ;
		}

		if(cbFirstHandCardType!=CT_ERROR && cbFirstHandCardType!=CT_FOUR_LINE_TAKE_ONE && cbFirstHandCardType!= CT_FOUR_LINE_TAKE_TWO &&
			BankerCanOutCardType1[cbFirstHandCardType].cbCardTypeCount==0 && BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
		{
			OutCardResult.cbCardCount = cbHandCardCount-1 ;
			memcpy(OutCardResult.cbResultCard, cbHandCardData, cbHandCardCount-1) ;
			return ;
		}

		if(GetCardLogicValue(cbHandCardData[0])>=GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) &&
			CT_ERROR!=cbSecondHandCardType && cbSecondHandCardType!=CT_FOUR_LINE_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_LINE_TAKE_TWO &&
			BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
		{
			OutCardResult.cbCardCount = 1 ;
			OutCardResult.cbResultCard[0] = cbHandCardData[0] ;
			return ;
		}

		if(CT_ERROR!=cbSecondHandCardType && cbSecondHandCardType!=CT_FOUR_LINE_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_LINE_TAKE_TWO &&
			BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
		{
			OutCardResult.cbCardCount = cbHandCardCount-1 ;
			memcpy(OutCardResult.cbResultCard, cbHandCardData+1, cbHandCardCount-1) ;
			return ;
		}
		}
		//����ը��
		else
		{
			//��ȥը�����
			BYTE cbRemainCard[MAX_COUNT], cbRemainCount=0 ;
			memcpy(cbRemainCard, cbHandCardData, cbHandCardCount) ;
			cbRemainCount = cbHandCardCount ;
			RemoveCard(cbAllBombCardData, cbAllBombCount, cbRemainCard, cbRemainCount) ;
			cbRemainCount -= cbAllBombCount ;
			if (GetCardType(cbRemainCard, cbRemainCount) != CT_ERROR)
			{
				OutCardResult.cbCardCount = cbRemainCount ;
				memcpy(OutCardResult.cbResultCard, cbRemainCard, cbRemainCount) ;
				return ;
			}
		}
	}

	//���߶���
	WORD wFriendID = (m_wBankerUser+2)%GAME_PLAYER ;
	BYTE cbFriendCardType = GetCardType(m_cbAllCardData[wFriendID], m_cbUserCardCount[wFriendID]) ;
	
	//�����˿�
	if(CT_SINGLE==cbFriendCardType)
	{
		//�Ϸ��ж�
		assert(m_cbUserCardCount[wFriendID]==1) ;
		if(m_cbUserCardCount[wFriendID]==1 && GetCardLogicValue(cbHandCardData[cbHandCardCount-1]) < GetCardLogicValue(m_cbAllCardData[wFriendID][0]))
		{
			OutCardResult.cbCardCount = 1 ;
			OutCardResult.cbResultCard[0] = cbHandCardData[cbHandCardCount-1] ;
			return ;
		}
	}
	//һ���˿�
	else if(CT_DOUBLE==cbFriendCardType && cbDoubleLineCount>=2)
	{
		if(GetCardLogicValue(cbDoubleLineCard[cbDoubleLineCount-1]) < GetCardLogicValue(m_cbAllCardData[wFriendID][0]))
		{
			OutCardResult.cbCardCount = 2 ;
			OutCardResult.cbResultCard[0] = cbDoubleLineCard[cbDoubleLineCount-2] ;
			OutCardResult.cbResultCard[1] = cbDoubleLineCard[cbDoubleLineCount-1] ;

			return ;
		}
	}

	//���ƽ����ж�
	if (1 == m_cbUserCardCount[m_wBankerUser] && cbHandCardCount >= 2 && m_cbUserCardCount[wFriendID] >= 2)
	{
		tagAnalyseResult MeAnalyseResult ;
		memset(&MeAnalyseResult,'\0', sizeof(MeAnalyseResult)) ;

		//�����˿�
		AnalysebCardData(cbHandCardData, cbHandCardCount, MeAnalyseResult) ;

		tagAnalyseResult FriendAnalyseResult ;
		memset(&FriendAnalyseResult, '\0',sizeof(FriendAnalyseResult)) ;

		//�����˿�
		AnalysebCardData(m_cbAllCardData[wFriendID], m_cbUserCardCount[wFriendID], FriendAnalyseResult) ;

		//�����ж�
		if ((m_cbUserCardCount[wFriendID] == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4) || 
			m_cbUserCardCount[wFriendID] == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4 + 1)) &&
			MeAnalyseResult.cbDoubleCount > 0 && FriendAnalyseResult.cbDoubleCount > 0)
		{
			//��С����
			BYTE cbMeLeastDoubleCardLogic = GetCardLogicValue(MeAnalyseResult.cbDoubleCardData[MeAnalyseResult.cbDoubleCount*2-2]) ;
			//������
			BYTE cbFriendLargestDoublecardLogic = GetCardLogicValue(FriendAnalyseResult.cbDoubleCardData[0]) ;

			//����ж�
			if (cbMeLeastDoubleCardLogic < 14 && cbMeLeastDoubleCardLogic < cbFriendLargestDoublecardLogic)
			{
				OutCardResult.cbCardCount = 2 ;
				OutCardResult.cbResultCard[0] = MeAnalyseResult.cbDoubleCardData[MeAnalyseResult.cbDoubleCount*2-2] ;
				OutCardResult.cbResultCard[1] = MeAnalyseResult.cbDoubleCardData[MeAnalyseResult.cbDoubleCount*2-1] ;
				return ;
			}
          			
		}
	}

	//�¼�Ϊ������ҵ����˿�����5��
	//	if(m_cbUserCardCount[m_wBankerUser]<=5)
	{
		//�����˿�
		tagOutCardTypeResult MeOutCardTypeResult[13] ;
		memset(MeOutCardTypeResult, '\0',sizeof(MeOutCardTypeResult)) ;
		AnalyseOutCardType(cbHandCardData, cbHandCardCount, MeOutCardTypeResult) ;

		//�Լ��˿�
		WORD wFriendID ;
		for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID) 
			if(wChairID!=m_wBankerUser && wMeChairID!=wChairID) wFriendID = wChairID ;

		//���㵥��
		BYTE cbMinSingleCardCount[4] ;
		cbMinSingleCardCount[0]=MAX_COUNT ;
		cbMinSingleCardCount[1]=MAX_COUNT ;
		cbMinSingleCardCount[2]=MAX_COUNT ;
		cbMinSingleCardCount[3]=MAX_COUNT ;
		BYTE cbIndex[4]={0} ;
		BYTE cbOutcardType[4]={CT_ERROR} ;
		BYTE cbMinValue=MAX_COUNT ; 
		BYTE cbMinSingleCountInFour=MAX_COUNT ;
		BYTE cbMinCardType=CT_ERROR ;
		BYTE cbMinIndex=0 ;

		//��ը�������
		for(BYTE cbCardType=CT_DOUBLE; cbCardType<CT_BOMB_CARD; ++cbCardType)
		{
			tagOutCardTypeResult const &tmpCardResult = MeOutCardTypeResult[cbCardType] ;

			//��ͬ���ͣ���ͬ���ȣ���l����l����ͬ���Ϳ��ܳ��Ȳ�һ��
			BYTE cbThisHandCardCount = MAX_COUNT ;

			//�����˿�
			tagOutCardTypeResult BankerCanOutCard[13] ;
			memset(BankerCanOutCard,'\0', sizeof(BankerCanOutCard)) ;

			tagOutCardTypeResult FriendOutCardTypeResult[13] ;
			memset(FriendOutCardTypeResult,'\0', sizeof(FriendOutCardTypeResult)) ;

			for(BYTE i=0; i<tmpCardResult.cbCardTypeCount; ++i)
			{
				//�������ж�
				if ( cbCardType == CT_DOUBLE )
				{
					tagAnalyseResult AnalyseResult ;
					memset( &AnalyseResult, '\0', sizeof( AnalyseResult ) ) ;
					AnalysebCardData( cbHandCardData, cbHandCardCount, AnalyseResult ) ;
					if ( AnalyseResult.cbSignedCount + AnalyseResult.cbThreeCount * 3 == cbHandCardCount )
					{
						bool bContinue = false ;
						for ( BYTE cbThreeIndex = 0; cbThreeIndex < AnalyseResult.cbThreeCount; ++cbThreeIndex )
							if ( GetCardValue(  tmpCardResult.cbCardData[i][0] ) == GetCardValue( AnalyseResult.cbThreeCardData[3 * cbThreeIndex] ) )
							{
								bContinue = true ;
								break ;
							}
						if ( bContinue ) continue ;
					}
				}

				BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i]) ;

				//���·���
				if(tmpCardResult.cbEachHandCardCount[i]!=cbThisHandCardCount)
				{
					cbThisHandCardCount = tmpCardResult.cbEachHandCardCount[i] ;
					AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], 
						tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i] ,BankerCanOutCard) ;
					AnalyseOutCardType(m_cbAllCardData[wFriendID], m_cbUserCardCount[wFriendID],
						tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i] ,FriendOutCardTypeResult) ;
				}

				BYTE cbMaxValue=0 ; 
				BYTE Index = 0 ;

				
				//���˳�ӣ�����Ĵ���
				BYTE cbCurrentCardType = GetCardType(tmpCardResult.cbCardData[i], cbThisHandCardCount) ;
				if (cbThisHandCardCount != cbHandCardCount && cbCurrentCardType >= CT_SINGLE_LINE && cbCurrentCardType <= CT_THREE_LINE_TAKE_TWO &&
					( GetCardLogicValue(tmpCardResult.cbCardData[i][cbThisHandCardCount-1]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-2]) || 
						GetCardLogicValue(tmpCardResult.cbCardData[i][0]) >= 11 ))
				{
					BYTE cbRemainCardData[MAX_COUNT], cbRemainCardCount ;
					memcpy(cbRemainCardData, cbHandCardData, cbHandCardCount) ;
					cbRemainCardCount = cbHandCardCount ;

					//�Ƴ��˿�
					RemoveCard(tmpCardResult.cbCardData[i], cbThisHandCardCount, cbRemainCardData, cbRemainCardCount) ;
					cbRemainCardCount -= cbThisHandCardCount ;

					//����˿�
					BYTE cbCurrentLargestLogicCard = GetCardLogicValue(tmpCardResult.cbCardData[i][0]) ;

					if (GetCardType(cbRemainCardData, cbRemainCardCount) == CT_ERROR && (cbCurrentCardType >= CT_THREE_LINE_TAKE_ONE && 
						cbCurrentCardType <= CT_THREE_LINE_TAKE_TWO && cbCurrentLargestLogicCard >= 11 && cbThisHandCardCount <=5 || 
						cbCurrentCardType == CT_SINGLE_LINE && cbThisHandCardCount <= 6 && cbCurrentLargestLogicCard >= 12 || 
						cbCurrentCardType >= CT_DOUBLE_LINE && cbCurrentCardType <= CT_THREE_LINE && cbCurrentLargestLogicCard >= 12 && cbThisHandCardCount <= 8))
					{
						//��ʱ����
						if ( cbCurrentCardType >= CT_SINGLE_LINE && cbCurrentCardType <= CT_THREE_LINE &&
							GetCardLogicValue(tmpCardResult.cbCardData[i][cbThisHandCardCount - 1]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-3]) )
							continue ;

						if ( cbCurrentCardType >= CT_THREE_LINE_TAKE_ONE && cbCurrentCardType <= CT_THREE_LINE_TAKE_TWO &&
							GetCardLogicValue(tmpCardResult.cbCardData[i][0]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-3]) )
							continue ;
					}
				}

				//��Դ�ԣ������ȳ�
				if (cbCardType == CT_DOUBLE && GetCardLogicValue(tmpCardResult.cbCardData[i][0]) >= 11)
				{
					BYTE cbAllSingleCardData[MAX_COUNT], cbAllSingleCount ;
					cbAllSingleCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, NULL, 0, cbAllSingleCardData) ;
					if (cbAllSingleCount >= 2 && GetCardLogicValue(cbAllSingleCardData[cbAllSingleCount-2]) < 10) continue ;
				}

				//�������ѹ�ƣ����Ҷ��Ѳ�����ѹ����
				if((BankerCanOutCard[cbCardType].cbCardTypeCount>0&&FriendOutCardTypeResult[cbCardType].cbCardTypeCount==0) || (BankerCanOutCard[cbCardType].cbCardTypeCount>0 && FriendOutCardTypeResult[cbCardType].cbCardTypeCount>0 &&
					GetCardLogicValue(FriendOutCardTypeResult[cbCardType].cbCardData[0][0])<=GetCardLogicValue(BankerCanOutCard[cbCardType].cbCardData[0][0])))
				{
					//�����ܵ�
					if( BankerCanOutCard[cbCardType].cbEachHandCardCount[0] > 0 && m_cbUserCardCount[m_wBankerUser]<=BankerCanOutCard[cbCardType].cbEachHandCardCount[0]+1)
						continue ;

					//�Լ����������û���Ȩ
					//if(GetCardLogicValue(tmpCardResult.cbCardData[0][0]) < GetCardLogicValue(BankerCanOutCard[cbCardType].cbCardData[0][0]) && 
					//	BankerCanOutCard[cbCardType].cbCardTypeCount > 0) 
					//	continue ;
				}
				//�Ƿ��д���
				if(tmpCardResult.cbEachHandCardCount[i] != cbHandCardCount)
				{
					bool bHaveLargeCard=false ;
					for(BYTE j=0; j<tmpCardResult.cbEachHandCardCount[i]; ++j)
						if(GetCardLogicValue(tmpCardResult.cbCardData[i][j])>=15) bHaveLargeCard=true ;

					if(cbCardType!=CT_SINGLE_LINE && cbCardType!=CT_DOUBLE_LINE && GetCardLogicValue(tmpCardResult.cbCardData[i][0])==14) bHaveLargeCard=true ; 

					if(bHaveLargeCard) continue ;
				}

				//�����Ƿ�����ߵ����ﶼû�п���ը��
				if(tmpCardResult.cbEachHandCardCount[i]==m_cbUserCardCount[m_wBankerUser] && cbCardType==GetCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser]) &&
					GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0])>GetCardLogicValue(tmpCardResult.cbCardData[i][0])) continue ;

				//����cbMinSingleCardCount[4]�����ֵ
				for(BYTE j=0; j<4; ++j)
				{
					if(cbMinSingleCardCount[j]>=cbTmpCount)
					{
						cbMinSingleCardCount[j] = cbTmpCount ;
						cbIndex[j] = i ;
						cbOutcardType[j] = cbCardType ;
						break ;
					}
				}

				//������Сֵ
				if(cbMinSingleCountInFour>=cbTmpCount)
				{
					//��С����
					cbMinCardType = cbCardType ;
					//��С�����е���С����
					cbMinSingleCountInFour=cbTmpCount ;						
					//��С�����е���С��
					cbMinIndex=i ;
				}
			}
		}

		if(cbMinSingleCountInFour>=AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, NULL, 0)+3 && 
			m_cbUserCardCount[m_wBankerUser]>4)
			cbMinSingleCountInFour=MAX_COUNT ;

		if(cbMinSingleCountInFour!=MAX_COUNT)
		{
			BYTE Index = cbMinIndex ;

			//ѡ����С��
			for(BYTE i=0; i<4; ++i)
			{
				if(cbOutcardType[i]==cbMinCardType && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
					GetCardLogicValue(MeOutCardTypeResult[cbMinCardType].cbCardData[cbIndex[i]][0])<GetCardLogicValue(MeOutCardTypeResult[cbMinCardType].cbCardData[Index][0]))
					Index = cbIndex[i] ;
			}

			//�����һֻ
			if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
			{
				OutCardResult.cbCardCount = 2 ;
				OutCardResult.cbResultCard[0] = 0x4f ;
				OutCardResult.cbResultCard[1] = 0x4e ;
				return ;
			}
			//����
			else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
			{
				OutCardResult.cbCardCount = 2 ;
				OutCardResult.cbResultCard[0] = 0x4f ;
				OutCardResult.cbResultCard[1] = 0x4e ;
				return ;
			}
			else
			{
				//���ñ�
				OutCardResult.cbCardCount=MeOutCardTypeResult[cbMinCardType].cbEachHandCardCount[Index];
				memcpy(OutCardResult.cbResultCard,MeOutCardTypeResult[cbMinCardType].cbCardData[Index],MeOutCardTypeResult[cbMinCardType].cbEachHandCardCount[Index]*sizeof(BYTE));
				return ;
			}

			assert(OutCardResult.cbCardCount>0) ;

			return ;
		}

		//�������˿�����5����û���ҵ��ʺϵ�����Ӵ��С
		if(OutCardResult.cbCardCount<=0 && m_cbUserCardCount[m_wBankerUser]<=5)
		{
			//ֻ��һ����ʱ���ܷŵ�����
			if(m_cbUserCardCount[m_wBankerUser]==1 && MeOutCardTypeResult[CT_SINGLE].cbCardTypeCount>0)
			{
				//��Сһ��
				BYTE Index=MAX_COUNT ;
				for(BYTE i=0; i<MeOutCardTypeResult[CT_SINGLE].cbCardTypeCount; ++i)
				{
					if(GetCardLogicValue(MeOutCardTypeResult[CT_SINGLE].cbCardData[i][0])>=GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]))
					{
						Index=i ;
					}
					else break ;
				}

				if(MAX_COUNT!=Index)
				{
					OutCardResult.cbCardCount = MeOutCardTypeResult[CT_SINGLE].cbEachHandCardCount[Index] ;
					memcpy(OutCardResult.cbResultCard, MeOutCardTypeResult[CT_SINGLE].cbCardData[Index], OutCardResult.cbCardCount) ;
					return ;
				}
			}
		}
	}

	BYTE cbFirstCard=0 ;
	//�������2
	for(BYTE i=0; i<cbHandCardCount; ++i) 
		if(GetCardLogicValue(cbHandCardData[i])<15)
		{
			cbFirstCard = i ;
			break ;
		}

		if(cbFirstCard<cbHandCardCount-1)
			AnalyseOutCardType(cbHandCardData+cbFirstCard, cbHandCardCount-cbFirstCard, CardTypeResult) ;
		else
			AnalyseOutCardType(cbHandCardData, cbHandCardCount, CardTypeResult) ;

		//���㵥��
		BYTE cbMinSingleCardCount[4] ;
		cbMinSingleCardCount[0]=MAX_COUNT ;
		cbMinSingleCardCount[1]=MAX_COUNT ;
		cbMinSingleCardCount[2]=MAX_COUNT ;
		cbMinSingleCardCount[3]=MAX_COUNT ;
		BYTE cbIndex[4]={0} ;
		BYTE cbOutcardType[4]={CT_ERROR} ;
		BYTE cbMinValue=MAX_COUNT ; 
		BYTE cbMinSingleCountInFour=MAX_COUNT ;
		BYTE cbMinCardType=CT_ERROR ;
		BYTE cbMinIndex=0 ;

		//��ը�������
		for(BYTE cbCardType=CT_SINGLE; cbCardType<CT_BOMB_CARD; ++cbCardType)
		{
			tagOutCardTypeResult const &tmpCardResult = CardTypeResult[cbCardType] ;
			for(BYTE i=0; i<tmpCardResult.cbCardTypeCount; ++i)
			{
				//ׯ�ҿ����ߵ�
				if ( CompareCard(tmpCardResult.cbCardData[i], m_cbAllCardData[m_wBankerUser], tmpCardResult.cbEachHandCardCount[i], m_cbUserCardCount[m_wBankerUser]) )
					continue ;

				//���˳�ӣ�����Ĵ���
				if ( tmpCardResult.cbEachHandCardCount[i] != cbHandCardCount && cbCardType >= CT_SINGLE_LINE && cbCardType <= CT_THREE_LINE_TAKE_TWO &&
					( GetCardLogicValue(tmpCardResult.cbCardData[i][tmpCardResult.cbEachHandCardCount[i]-1]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-2]) || 
						GetCardLogicValue(tmpCardResult.cbCardData[i][0]) >= 11 ))
				{
					BYTE cbRemainCardData[MAX_COUNT], cbRemainCardCount ;
					memcpy(cbRemainCardData, cbHandCardData, cbHandCardCount) ;
					cbRemainCardCount = cbHandCardCount ;

					//�Ƴ��˿�
					RemoveCard(tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i], cbRemainCardData, cbRemainCardCount) ;
					cbRemainCardCount -= tmpCardResult.cbEachHandCardCount[i] ;

					//����˿�
					BYTE cbCurrentLargestLogicCard = GetCardLogicValue(tmpCardResult.cbCardData[i][0]) ;

					if (GetCardType(cbRemainCardData, cbRemainCardCount) == CT_ERROR && (cbCardType >= CT_THREE_LINE_TAKE_ONE && 
						cbCardType <= CT_THREE_LINE_TAKE_TWO && cbCurrentLargestLogicCard >= 11 && tmpCardResult.cbEachHandCardCount[i] <=5 || 
						cbCardType == CT_SINGLE_LINE && tmpCardResult.cbEachHandCardCount[i] <= 6 && cbCurrentLargestLogicCard >= 12 || 
						cbCardType >= CT_DOUBLE_LINE && cbCardType <= CT_THREE_LINE && cbCurrentLargestLogicCard >= 12 && tmpCardResult.cbEachHandCardCount[i] <= 8))
					{
						//��ʱ����
						if ( cbCardType >= CT_SINGLE_LINE && cbCardType <= CT_THREE_LINE &&
							GetCardLogicValue(tmpCardResult.cbCardData[i][tmpCardResult.cbEachHandCardCount[i] - 1]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-3]) )
							continue ;

						if ( cbCardType >= CT_THREE_LINE_TAKE_ONE && cbCardType <= CT_THREE_LINE_TAKE_TWO &&
							GetCardLogicValue(tmpCardResult.cbCardData[i][0]) > GetCardLogicValue(cbHandCardData[cbHandCardCount-3]) )
							continue ;
					}
				}

				BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i]) ;

				BYTE cbMaxValue=0 ; 
				BYTE Index = 0 ;

				//����cbMinSingleCardCount[4]�����ֵ
				for(BYTE j=0; j<4; ++j)
				{
					if(cbMinSingleCardCount[j]>=cbTmpCount)
					{
						cbMinSingleCardCount[j] = cbTmpCount ;
						cbIndex[j] = i ;
						cbOutcardType[j] = cbCardType ;
						break ;
					}
				}

				//������Сֵ
				if(cbMinSingleCountInFour>=cbTmpCount)
				{
					//��С����
					cbMinCardType = cbCardType ;
					//��С�����е���С����
					cbMinSingleCountInFour=cbTmpCount ;						
					//��С�����е���С��
					cbMinIndex=i ;
				}
			}
		}

		if(cbMinSingleCountInFour!=MAX_COUNT)
		{
			BYTE Index = cbMinIndex ;

			//ѡ����С��
			for(BYTE i=0; i<4; ++i)
			{
				if(cbOutcardType[i]==cbMinCardType && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
					GetCardLogicValue(CardTypeResult[cbMinCardType].cbCardData[cbIndex[i]][0])<GetCardLogicValue(CardTypeResult[cbMinCardType].cbCardData[Index][0]))
					Index = cbIndex[i] ;
			}

			//�����һֻ
			if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
			{
				OutCardResult.cbCardCount = 2 ;
				OutCardResult.cbResultCard[0] = 0x4f ;
				OutCardResult.cbResultCard[1] = 0x4e ;
				return ;
			}
			//����
			else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
			{
				OutCardResult.cbCardCount = 2 ;
				OutCardResult.cbResultCard[0] = 0x4f ;
				OutCardResult.cbResultCard[1] = 0x4e ;
				return ;
			}
			else
			{
				//���ñ�
				OutCardResult.cbCardCount=CardTypeResult[cbMinCardType].cbEachHandCardCount[Index];
				memcpy(OutCardResult.cbResultCard,CardTypeResult[cbMinCardType].cbCardData[Index],CardTypeResult[cbMinCardType].cbEachHandCardCount[Index]*sizeof(BYTE));
				return ;
			}

			assert(OutCardResult.cbCardCount>0) ;

			return ;
		}
		//���ֻʣը��
		else if (CardTypeResult[CT_BOMB_CARD].cbCardTypeCount > 0) 
		{	
			//BYTE Index=0 ;
			//BYTE cbLogicCardValue = GetCardLogicValue(0x4F)+1 ;
			////��Сը��
			//for(BYTE i=0; i<CardTypeResult[CT_BOMB_CARD].cbCardTypeCount; ++i)
			//	if(cbLogicCardValue>GetCardLogicValue(CardTypeResult[CT_BOMB_CARD].cbCardData[i][0]))
			//	{
			//		cbLogicCardValue = GetCardLogicValue(CardTypeResult[CT_BOMB_CARD].cbCardData[i][0]) ;
			//		Index = i ;
			//	}

			//	//���ñ�
			//	OutCardResult.cbCardCount=CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index];
			//	memcpy(OutCardResult.cbResultCard,CardTypeResult[CT_BOMB_CARD].cbCardData[Index],CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index]*sizeof(BYTE));

			//	return ;
		}

		BYTE cbAllSingleCardData[MAX_COUNT], cbAllSingleCardCount ;
		cbAllSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, NULL, 0, cbAllSingleCardData) ;

		if ( cbAllSingleCardCount > 0 )
		{
			//���û������ͳ���С��һ��
			OutCardResult.cbCardCount = 1 ;
			OutCardResult.cbResultCard[0] = cbAllSingleCardData[cbAllSingleCardCount-1] ;

			return ;
		}

		//���û������ͳ���С��һ��
		OutCardResult.cbCardCount = 1 ;
		OutCardResult.cbResultCard[0] = cbHandCardData[cbHandCardCount-1] ;
		return ;
}
//�����¼ң�����ƣ�
void CAndroidAI::UndersideOfBankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, WORD wOutCardUser, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult)
{
	//��ʼ��
	memset(&OutCardResult, '\0',sizeof(OutCardResult)) ;

	//���±�û��
	tagOutCardTypeResult CardTypeResult[12+1] ;
	memset(CardTypeResult, '\0',sizeof(CardTypeResult)) ;

	//��������
	BYTE cbOutCardType = GetCardType(cbTurnCardData,cbTurnCardCount) ;

	//����ɳ���
	tagOutCardTypeResult BankerOutCardTypeResult[13] ;
	memset(BankerOutCardTypeResult, '\0',sizeof(BankerOutCardTypeResult)) ;

	AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], BankerOutCardTypeResult) ;
	AnalyseOutCardType(cbHandCardData,cbHandCardCount,cbTurnCardData,cbTurnCardCount, CardTypeResult) ;

	//ֻʣը��
	if(cbHandCardCount==CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[0] && (cbOutCardType<CT_BOMB_CARD || 
		GetCardLogicValue(CardTypeResult[CT_BOMB_CARD].cbCardData[0][0])>GetCardLogicValue(cbTurnCardData[0])))
	{
		OutCardResult.cbCardCount = CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[0] ;
		memcpy(OutCardResult.cbResultCard,  CardTypeResult[CT_BOMB_CARD].cbCardData, OutCardResult.cbCardCount) ;

		return ;
	}
	//˫��ը����һ��
	else if(cbHandCardCount>2 && cbHandCardData[0]==0x4f && cbHandCardData[1]==0x4e && CT_ERROR!=GetCardType(cbHandCardData+2, cbHandCardCount-2) &&
		CT_FOUR_LINE_TAKE_ONE != GetCardType(cbHandCardData+2, cbHandCardCount-2) && CT_FOUR_LINE_TAKE_TWO != GetCardType(cbHandCardData+2, cbHandCardCount-2))
	{
		OutCardResult.cbCardCount = 2 ;
		OutCardResult.cbResultCard[0] = 0x4f ;
		OutCardResult.cbResultCard[1] = 0x4e ;
		return ;
	}

	//ը����һ��
	BYTE cbRemainCard[MAX_COUNT], cbRemainCount=0;
	BYTE cbAllBombCard[MAX_COUNT], cbAllBombCount=0 ;
	GetAllBomCard(cbHandCardData, cbHandCardCount, cbAllBombCard, cbAllBombCount) ;

	//��ը�ж�
	if(cbAllBombCount>0 && wOutCardUser==m_wBankerUser)
	{
		//ʣ���˿�
		memcpy(cbRemainCard, cbHandCardData, cbHandCardCount) ;
		cbRemainCount = cbHandCardCount ;
		RemoveCard(cbAllBombCard, cbAllBombCount, cbRemainCard, cbRemainCount) ;
		cbRemainCount -= cbAllBombCount ;
		
		if(CT_ERROR != GetCardType(cbRemainCard, cbRemainCount) || 
			(2==cbRemainCount && GetCardLogicValue(cbRemainCard[0])>GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0])))
		{
			if((cbOutCardType<CT_BOMB_CARD || GetCardLogicValue(cbAllBombCard[0])>GetCardLogicValue(cbTurnCardData[0])) &&
				( CardTypeResult[cbOutCardType].cbCardTypeCount <= 0 || CT_ERROR != GetCardType(cbRemainCard, cbRemainCount)) )
			{
				//˫��ը��
				if(GetCardColor(cbAllBombCard[0])==0x40)
				{
					OutCardResult.cbCardCount = 2 ;
					OutCardResult.cbResultCard[0] = 0x4f ;
					OutCardResult.cbResultCard[1] = 0x4e ;
					return ;
				}
				else
				{
					//���������
					BYTE cbBankerAllBombCard[MAX_COUNT], cbBankerAllBombCardCount ;
					GetAllBomCard( m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], cbBankerAllBombCard, cbBankerAllBombCardCount) ;

					if ( !CompareCard( cbTurnCardData, cbRemainCard, cbTurnCardCount, cbRemainCount) ||  cbBankerAllBombCardCount <= 0 || 
						GetCardLogicValue( cbAllBombCard[0] ) > GetCardLogicValue( cbBankerAllBombCard[0] ) )
					{
						OutCardResult.cbCardCount = 4 ;
						memcpy(OutCardResult.cbResultCard, cbAllBombCard, 4) ;
						return ;
					}
				}
			}
		}
	}

	//ֻʣһ�ֳ�ը
	if ( CardTypeResult[CT_BOMB_CARD].cbCardTypeCount > 0 && GetCardType(m_cbAllCardData[(m_wBankerUser + 2) % GAME_PLAYER], m_cbUserCardCount[(m_wBankerUser + 2) % GAME_PLAYER]) != CT_ERROR &&
		GetCardType(m_cbAllCardData[(m_wBankerUser + 2) % GAME_PLAYER], m_cbUserCardCount[(m_wBankerUser + 2) % GAME_PLAYER]) <= CT_DOUBLE )
	{
		//ֻʣ����
		if ( GetCardType(m_cbAllCardData[(m_wBankerUser + 2) % GAME_PLAYER], m_cbUserCardCount[(m_wBankerUser + 2) % GAME_PLAYER]) == CT_SINGLE )
		{
			BYTE cbIndex = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount - 1 ;
			BYTE cbCardCount = CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[cbIndex] ;
			memcpy(OutCardResult.cbResultCard, CardTypeResult[CT_BOMB_CARD].cbCardData[cbIndex], cbCardCount) ;
			OutCardResult.cbCardCount = cbCardCount ;

			return ;
		}
		else if ( GetCardType(m_cbAllCardData[(m_wBankerUser + 2) % GAME_PLAYER], m_cbUserCardCount[(m_wBankerUser + 2) % GAME_PLAYER]) == CT_DOUBLE )
		{
			BYTE cbAllDoubleCard[MAX_COUNT], cbAllDoubleCount ;
			GetAllDoubleCard(cbHandCardData, cbHandCardCount, cbAllDoubleCard, cbAllDoubleCount) ;

			if ( cbAllDoubleCount > 0 && GetCardLogicValue(cbAllDoubleCard[cbAllDoubleCount - 1]) <= 10)
			{
				BYTE cbIndex = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount - 1 ;
				BYTE cbCardCount = CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[cbIndex] ;
				memcpy(OutCardResult.cbResultCard, CardTypeResult[CT_BOMB_CARD].cbCardData[cbIndex], cbCardCount) ;
				OutCardResult.cbCardCount = cbCardCount ;

				return ;
			}			
		}

	}

	//ֻ��һ����
	if ( GetCardType(cbHandCardData, cbHandCardCount) != CT_FOUR_LINE_TAKE_ONE && GetCardType(cbHandCardData, cbHandCardCount) != CT_FOUR_LINE_TAKE_TWO &&
		CompareCard(cbTurnCardData, cbHandCardData, cbTurnCardCount, cbHandCardCount) )
	{
		OutCardResult.cbCardCount = cbHandCardCount ;
		memcpy(OutCardResult.cbResultCard, cbHandCardData, cbHandCardCount) ;

		return ;
	}

	//���ƽ����ж�
	if (1 == m_cbUserCardCount[m_wBankerUser] && cbHandCardCount >= 2 && cbOutCardType == CT_DOUBLE)
	{
		tagAnalyseResult AnalyseResult ;
		memset(&AnalyseResult,'\0', sizeof(AnalyseResult)) ;

		//�����˿�
		AnalysebCardData(cbHandCardData, cbHandCardCount, AnalyseResult) ;

		//�����ж�
		if (cbHandCardCount == (AnalyseResult.cbDoubleCount * 2 + AnalyseResult.cbThreeCount * 3 + AnalyseResult.cbFourCount * 4) || 
			cbHandCardCount == (AnalyseResult.cbDoubleCount * 2 + AnalyseResult.cbThreeCount * 3 + AnalyseResult.cbFourCount * 4 + 1))
		{
			//����ж�
			for (int nIndex = AnalyseResult.cbDoubleCount-1; nIndex>=0 ; --nIndex)
			{
				if (GetCardLogicValue(AnalyseResult.cbDoubleCardData[nIndex*2]) > GetCardLogicValue(cbTurnCardData[0]))
				{
					OutCardResult.cbCardCount = 2 ;
					OutCardResult.cbResultCard[0] = AnalyseResult.cbDoubleCardData[nIndex*2] ;
					OutCardResult.cbResultCard[1] = AnalyseResult.cbDoubleCardData[nIndex*2+1] ;
					return ;
				}
			}

			//��ը�ж�
			if (AnalyseResult.cbFourCount > 0) 
			{
				//��Сը��
				BYTE cbLestBombIndex = AnalyseResult.cbFourCount-1 ;

				OutCardResult.cbCardCount = 4 ;
				OutCardResult.cbResultCard[0] = AnalyseResult.cbFourCardData[cbLestBombIndex*4] ;
				OutCardResult.cbResultCard[1] = AnalyseResult.cbFourCardData[cbLestBombIndex*4+1] ;
				OutCardResult.cbResultCard[2] = AnalyseResult.cbFourCardData[cbLestBombIndex*4+2] ;
				OutCardResult.cbResultCard[3] = AnalyseResult.cbFourCardData[cbLestBombIndex*4+3] ;

				return ;
			}
            			
		}

		//����Լ�
		if ( wOutCardUser != m_wBankerUser )
		{
			tagAnalyseResult FriendAnalyseResult ;
			memset( &FriendAnalyseResult, '\0',sizeof( FriendAnalyseResult ) ) ;

			//�����˿�
			AnalysebCardData( m_cbAllCardData[wOutCardUser], m_cbUserCardCount[wOutCardUser], FriendAnalyseResult ) ;

			//�����ж�
			if ( m_cbUserCardCount[wOutCardUser] == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4) || 
				 m_cbUserCardCount[wOutCardUser] == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4 + 1))
			{
				return ;
			}

			//���±�û��
			tagOutCardTypeResult FriendCardTypeResult[12+1] ;
			memset( FriendCardTypeResult, '\0',sizeof( FriendCardTypeResult ) ) ;

			AnalyseOutCardType( m_cbAllCardData[wOutCardUser], m_cbUserCardCount[wOutCardUser], FriendCardTypeResult ) ;

			for ( BYTE cbLineCardIdx = 0; cbLineCardIdx < FriendCardTypeResult[CT_SINGLE_LINE].cbCardTypeCount; ++cbLineCardIdx )
			{
				//ʣ���˿�
				BYTE cbRemainCardData[MAX_COUNT], cbRemainCardCount ;

				cbRemainCardCount = m_cbUserCardCount[wOutCardUser] ;
				memcpy( cbRemainCardData, m_cbAllCardData[wOutCardUser], cbRemainCardCount ) ;
				RemoveCard( FriendCardTypeResult[CT_SINGLE_LINE].cbCardData[cbLineCardIdx],  FriendCardTypeResult[CT_SINGLE_LINE].cbEachHandCardCount[cbLineCardIdx], 
					cbRemainCardData, cbRemainCardCount ) ;

				cbRemainCardCount -= FriendCardTypeResult[CT_SINGLE_LINE].cbEachHandCardCount[cbLineCardIdx] ;

				//�����˿�
				AnalysebCardData( cbRemainCardData, cbRemainCardCount, FriendAnalyseResult ) ;

				//�����ж�
				if (cbRemainCardCount == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4) || 
					cbRemainCardCount == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4 + 1))
				{
					return ;
				}
			}

			for ( BYTE cbLineCardIdx = 0; cbLineCardIdx < FriendCardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount; ++cbLineCardIdx )
			{
				//ʣ���˿�
				BYTE cbRemainCardData[MAX_COUNT], cbRemainCardCount ;

				cbRemainCardCount = m_cbUserCardCount[wOutCardUser] ;
				memcpy( cbRemainCardData, m_cbAllCardData[wOutCardUser], cbRemainCardCount ) ;
				RemoveCard( FriendCardTypeResult[CT_SINGLE_LINE].cbCardData[cbLineCardIdx],  FriendCardTypeResult[CT_SINGLE_LINE].cbEachHandCardCount[cbLineCardIdx], 
					cbRemainCardData, cbRemainCardCount ) ;

				cbRemainCardCount -= FriendCardTypeResult[CT_SINGLE_LINE].cbEachHandCardCount[cbLineCardIdx] ;

				//�����˿�
				AnalysebCardData( cbRemainCardData, cbRemainCardCount, FriendAnalyseResult ) ;

				//�����ж�
				if (cbRemainCardCount == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4) || 
					cbRemainCardCount == (FriendAnalyseResult.cbDoubleCount * 2 + FriendAnalyseResult.cbThreeCount * 3 + FriendAnalyseResult.cbFourCount * 4 + 1))
				{
					return ;
				}
			}


		}
	}


	//�Լҿɷ����
	if( m_wBankerUser != wOutCardUser && ! CompareCard( cbTurnCardData, m_cbAllCardData[ m_wBankerUser ], cbTurnCardCount, m_cbUserCardCount[ m_wBankerUser ] ) )
	{
		//ׯ���˿�
		bool bBankerCanOut = false ;
		tagOutCardTypeResult BankerOutCardResult[12+1] ;
		memset(BankerOutCardResult, '\0',sizeof(BankerOutCardResult)) ;

        //�����˿�
		AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], cbTurnCardData, cbTurnCardCount, BankerOutCardResult) ;
		for(BYTE cbCardType=CT_SINGLE; cbCardType<=CT_MISSILE_CARD; ++cbCardType) if(BankerOutCardResult[cbCardType].cbCardTypeCount>0) bBankerCanOut = true ; 

		if(!bBankerCanOut)
		{
		//�Լ�ID
		WORD wFriendChairID = (m_wBankerUser+2)%GAME_PLAYER ;

		//�����˿�
		tagOutCardTypeResult FriendCardTypeResult[12+1] ;
		memset(FriendCardTypeResult, '\0',sizeof(FriendCardTypeResult)) ;
		AnalyseOutCardType(m_cbAllCardData[wFriendChairID], m_cbUserCardCount[wFriendChairID], FriendCardTypeResult) ;

		for(BYTE cbCardType=CT_SINGLE; cbCardType<=CT_MISSILE_CARD; ++cbCardType)
			if(FriendCardTypeResult[cbCardType].cbCardTypeCount>0)
			{
				for(LONG lIndex=0; lIndex<FriendCardTypeResult[cbCardType].cbCardTypeCount; ++lIndex)
				{

					if(TestOutAllCard(wFriendChairID, FriendCardTypeResult[cbCardType].cbCardData[lIndex], FriendCardTypeResult[cbCardType].cbEachHandCardCount[lIndex], true))
					{
						//��ѹ�Լ�
						return ;
					}
				}
			}
		}
	}

	//���߶Լ�
	if (GetCardType(m_cbAllCardData[(m_wBankerUser + 2) % GAME_PLAYER], m_cbUserCardCount[(m_wBankerUser + 2) % GAME_PLAYER]) == GetCardType(cbTurnCardData, cbTurnCardCount) &&
		CompareCard(cbTurnCardData, m_cbAllCardData[(m_wBankerUser + 2) % GAME_PLAYER], cbTurnCardCount, m_cbUserCardCount[(m_wBankerUser + 2) % GAME_PLAYER]))
		return ;

	if (CompareCard(cbTurnCardData, m_cbAllCardData[(m_wBankerUser + 2) % GAME_PLAYER], cbTurnCardCount, m_cbUserCardCount[(m_wBankerUser + 2) % GAME_PLAYER])) return ;

	//�жϿɷ����
	BYTE cbSingleCardCount = MAX_COUNT+CT_MISSILE_CARD ;
	bool bFindBestCard = false ;
	for(BYTE cbCardType=CT_SINGLE; cbCardType<=CT_MISSILE_CARD; ++cbCardType)
		if(CardTypeResult[cbCardType].cbCardTypeCount>0)
		{
			for(LONG lIndex=0; lIndex<CardTypeResult[cbCardType].cbCardTypeCount; ++lIndex)
			{
				WORD wMeChairID = (m_wBankerUser+1)%GAME_PLAYER ;

				if(TestOutAllCard(wMeChairID, CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex], false))
				{
					//���㵥��
					BYTE cbTmpSingleCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CardTypeResult[cbCardType].cbCardData[lIndex], 
						CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]) ;

					//����ж�
					if (cbTmpSingleCount >= MAX_COUNT) continue ;

					//ը�����ȼ��ź�
					BYTE cbBombCardType = GetCardType(CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]) ;
					if (cbBombCardType == CT_BOMB_CARD) cbTmpSingleCount += 4 ;
					else if (cbBombCardType == CT_MISSILE_CARD) cbTmpSingleCount += 5 ;

					////�ı�Ȩֵ
					//if (cbBombCardType != CT_ERROR) cbTmpSingleCount += cbBombCardType ;

					//����ը��
					//BYTE cbWantOutCardType = GetCardType(CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]) ;
					//if (CardTypeResult[cbOutCardType].cbCardTypeCount > 0 && cbOutCardType < CT_BOMB_CARD && cbWantOutCardType >= CT_BOMB_CARD) continue ;

					if(cbTmpSingleCount <= cbSingleCardCount)
					{
						//���ñ�
						OutCardResult.cbCardCount=CardTypeResult[cbCardType].cbEachHandCardCount[lIndex];
						memcpy(OutCardResult.cbResultCard, CardTypeResult[cbCardType].cbCardData[lIndex],
						CardTypeResult[cbCardType].cbEachHandCardCount[lIndex]*sizeof(BYTE));
						cbSingleCardCount = cbTmpSingleCount ;
						bFindBestCard = true ;
					}
				}
			}
		}
		//ֱ�ӷ���
		if (bFindBestCard) return ;

	//ȡ���ĸ���С����
	BYTE cbMinSingleCardCount[4] ;
	cbMinSingleCardCount[0]=MAX_COUNT ;
	cbMinSingleCardCount[1]=MAX_COUNT ;
	cbMinSingleCardCount[2]=MAX_COUNT ;
	cbMinSingleCardCount[3]=MAX_COUNT ;
	BYTE cbIndex[4]={0} ;	
	BYTE cbMinSingleCountInFour=MAX_COUNT ;

	//�ɳ��˿ˣ������Ѿ����˵�ը���ˣ�
	tagOutCardTypeResult const &CanOutCard = CardTypeResult[cbOutCardType] ;

	for(BYTE i=0; i<CanOutCard.cbCardTypeCount; ++i)
	{
		//��С����
		BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount,CanOutCard.cbCardData[i], CanOutCard.cbEachHandCardCount[i]) ; 
		BYTE cbMaxValue=0 ; 
		BYTE Index = 0 ;

		//����cbMinSingleCardCount[4]�����ֵ
		for(BYTE j=0; j<4; ++j)
		{
			if(cbMinSingleCardCount[j]>=cbTmpCount)
			{
				cbMinSingleCardCount[j] = cbTmpCount ;
				cbIndex[j] = i ;
				break ;
			}
		}

	}

	for(BYTE i=0; i<4; ++i)
		if(cbMinSingleCountInFour>cbMinSingleCardCount[i]) cbMinSingleCountInFour = cbMinSingleCardCount[i] ;


	//ԭʼ������
	BYTE cbOriginSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount,NULL,0) ;

	//���ѳ���
	bool bFriendOut = m_wBankerUser!=wOutCardUser ;
	if(bFriendOut)
	{
		//��9���������һ����
		if (GetCardType(m_cbAllCardData[(2 + m_wBankerUser) % GAME_PLAYER], m_cbUserCardCount[(2 + m_wBankerUser) % GAME_PLAYER]) != CT_ERROR) return ;

		//�������TestOutAllCard���ѶԿɳ�ը���������
		if(CanOutCard.cbCardTypeCount > 0 && CanOutCard.cbCardType < CT_BOMB_CARD &&  cbMinSingleCountInFour < MAX_COUNT)
		{
			BYTE cbMinLogicCardValue = GetCardLogicValue(0x4F)+1 ;
			bool bFindCard = false ;
			BYTE cbCanOutIndex=0 ;
			for(BYTE i=0; i<4; ++i)
			{
				BYTE Index = cbIndex[i] ;
				
				//����l�Ʋ��ӶԼ���
				if ( CanOutCard.cbCardType >= CT_THREE &&  CanOutCard.cbCardType <= CT_MISSILE_CARD && GetCardLogicValue(CanOutCard.cbCardData[Index][0]) >= 7 && 
					CanOutCard.cbEachHandCardCount[Index] <=5 )
					continue ;
				
				//С��J���ƣ�����С��K������ɢ��
				if((cbMinSingleCardCount[i]<cbOriginSingleCardCount+3 &&  (cbMinSingleCardCount[i]<=cbMinSingleCountInFour || cbMinSingleCardCount[i]<=cbMinSingleCountInFour+1 && 
				CanOutCard.cbCardType >= CT_THREE_LINE_TAKE_ONE && CanOutCard.cbCardType <= CT_THREE_LINE_TAKE_TWO ) && 
					(GetCardLogicValue(CanOutCard.cbCardData[Index][0])<=11 || (cbMinSingleCardCount[i]<cbOriginSingleCardCount)&&GetCardLogicValue(CanOutCard.cbCardData[Index][0])<=13)) &&
					cbMinLogicCardValue>GetCardLogicValue(CanOutCard.cbCardData[Index][0]) && cbHandCardCount>5)
				{
					//������û�д��ƣ���Էɻ�������Ĵ��ƣ�
					bool bNoLargeCard = true ;
					for(BYTE k=3; k<CanOutCard.cbEachHandCardCount[Index]; ++k)
					{
						//�д��ƶ��Ҳ���һ�γ���
						if(GetCardLogicValue(CanOutCard.cbCardData[Index][k])>=15 && 
							CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount) bNoLargeCard = false ;
					}
					if(bNoLargeCard)
					{
						bFindCard = true ;
						cbCanOutIndex = Index ; 
						cbMinLogicCardValue = GetCardLogicValue(CanOutCard.cbCardData[Index][0]) ;
					}
				}
				else if(cbHandCardCount<5 && cbMinSingleCardCount[i]<cbOriginSingleCardCount+4 && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
					cbMinLogicCardValue>GetCardLogicValue(CanOutCard.cbCardData[Index][0]))
				{
					//�ܳ�����Լҵ�2
					if ( GetCardLogicValue( CanOutCard.cbCardData[Index][0] ) >= 16 && GetCardLogicValue( cbTurnCardData[0] ) >= 15 )
						continue ;

					bFindCard = true ;
					cbCanOutIndex = Index ; 
					cbMinLogicCardValue = GetCardLogicValue(CanOutCard.cbCardData[Index][0]) ;
				}
			}

			if(bFindCard)
			{

				//���ñ�
				OutCardResult.cbCardCount=CanOutCard.cbEachHandCardCount[cbCanOutIndex];
				memcpy(OutCardResult.cbResultCard,CanOutCard.cbCardData[cbCanOutIndex],CanOutCard.cbEachHandCardCount[cbCanOutIndex]*sizeof(BYTE));

				return ;
			}
			//��������������
			else if(cbHandCardCount<=5)
			{
				BYTE cbMinLogicCard = GetCardLogicValue(0x4f)+1 ;
				BYTE cbCanOutIndex = 0 ;
				for(BYTE i=0; i<4; ++i)
					if(cbMinSingleCardCount[i]<MAX_COUNT && cbMinSingleCardCount[i]<=cbMinSingleCountInFour && cbMinLogicCard>GetCardLogicValue(CanOutCard.cbCardData[cbIndex[i]][0]) && 
						GetCardLogicValue(CanOutCard.cbCardData[cbIndex[i]][0])<=14)
					{
						cbMinLogicCard = GetCardLogicValue(CanOutCard.cbCardData[cbIndex[i]][0]) ;
						cbCanOutIndex = cbIndex[i] ;
					}

					if(cbMinLogicCard != (GetCardLogicValue(0x4f)+1))
					{
						//���ñ�
						OutCardResult.cbCardCount=CanOutCard.cbEachHandCardCount[cbCanOutIndex];
						memcpy(OutCardResult.cbResultCard,CanOutCard.cbCardData[cbCanOutIndex],CanOutCard.cbEachHandCardCount[cbCanOutIndex]*sizeof(BYTE));

						return ;
					}
			}

			return ;
		}
		else
		{
			return ;
		}

	}
	//�������
	else
	{
		if(CanOutCard.cbCardTypeCount>0 && cbMinSingleCountInFour < MAX_COUNT)
		{
			BYTE cbMinLogicCardValue = GetCardLogicValue(0x4F)+1 ;
			bool bFindCard = false ;
			BYTE cbCanOutIndex=0 ;
			for(BYTE i=0; i<4; ++i)
			{
				BYTE Index = cbIndex[i] ;

				if((cbMinSingleCardCount[i]<cbOriginSingleCardCount+4)  &&  (cbMinSingleCardCount[i]<=cbMinSingleCountInFour || cbMinSingleCardCount[i]<=cbMinSingleCountInFour+1 && 
				CanOutCard.cbCardType >= CT_THREE_LINE_TAKE_ONE && CanOutCard.cbCardType <= CT_THREE_LINE_TAKE_TWO ) &&
					cbMinLogicCardValue>GetCardLogicValue(CanOutCard.cbCardData[Index][0]))
				{
					//��Դ���
					bool bNoLargeCard = true ;

					//�����������������4�����ҵ�������С��K���ƶ��Ҳ��ǵ������ϵ������ʱ�����ܳ�2ȥ��
					if(m_cbUserCardCount[m_wBankerUser]>=4 && cbHandCardCount>=5 && CanOutCard.cbEachHandCardCount[Index]>=2 && 
						GetCardLogicValue(CanOutCard.cbCardData[Index][0])>=15 &&
						GetCardLogicValue(cbTurnCardData[0])<13 &&
						GetCardLogicValue(cbTurnCardData[0])<GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) && 
						CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount)
						bNoLargeCard=false ;

					//������û�д��ƣ���Էɻ�������Ĵ��ƣ�
					for(BYTE k=3; k<CanOutCard.cbEachHandCardCount[Index]; ++k)
					{
						if(GetCardLogicValue(CanOutCard.cbCardData[Index][k])>=15 && 
							CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount)
							bNoLargeCard = false ;
					}
					if(bNoLargeCard)
					{
						bFindCard = true ;
						cbCanOutIndex = Index ; 
						cbMinLogicCardValue = GetCardLogicValue(CanOutCard.cbCardData[Index][0]) ;
					}
				}
			}

			if(bFindCard)
			{
				//����������
				BYTE cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) ;
				bool bCanOut=true ;

				//��ֻѹ2
				if(GetCardLogicValue(cbTurnCardData[0])<cbLargestLogicCard)
				{
					if(GetCardColor(CanOutCard.cbCardData[cbCanOutIndex][0])==0x40 && GetCardLogicValue(cbTurnCardData[0])<=14 && cbHandCardCount>5) 								
					{
						bCanOut = false ;
					}
				}

                //˫���ж�
				if(GetCardLogicValue(CanOutCard.cbCardData[cbCanOutIndex][0])>=16 && cbHandCardCount>=2 && cbHandCardData[0]==0x4F && cbHandCardData[1]==0x4E)
				{
					bool bOutMissileCard = false ;
					//һ���ƺ�һ��ը��
					BYTE cbRemainCardData[20], cbRemainCardCount=cbHandCardCount ;
					memcpy(cbRemainCardData, cbHandCardData, cbHandCardCount) ;
					RemoveCard(cbRemainCardData, 2, cbRemainCardData, cbRemainCardCount) ;
					cbRemainCardCount -= 2 ;
					if(CT_ERROR!=GetCardType(cbRemainCardData, cbRemainCardCount)) bOutMissileCard = true;

					//ֻʣ����
					BYTE cbRemainLargestCard = GetCardLogicValue(cbRemainCardData[0]) ;
					if(cbRemainCardCount<5 && cbRemainCardCount>0 && GetCardLogicValue(cbRemainCardData[0])>=14) bOutMissileCard = true;

					//ը������
					BYTE cbSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CanOutCard.cbCardData[cbCanOutIndex],
						CanOutCard.cbEachHandCardCount[cbCanOutIndex]) ;
					if(cbSingleCardCount<=1 && GetCardLogicValue(cbRemainCardData[0])>=11) bOutMissileCard = true;

					//����С��
					if (GetCardLogicValue(cbRemainCardData[0]) <= 10 && CT_ERROR == GetCardType(cbRemainCardData, cbRemainCardCount) && 
						GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) > 10)
						bOutMissileCard = false ;

					//����˿�
					if(bOutMissileCard)
					{
						//��������ը��
						BYTE cbIndex = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount - 1 ;

						OutCardResult.cbCardCount = CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[cbIndex] ;
						memcpy(OutCardResult.cbResultCard, CardTypeResult[CT_BOMB_CARD].cbCardData[cbIndex], OutCardResult.cbCardCount) ;
						return ;
					}
				}
				if(bCanOut)
				{
					//���ñ�
					OutCardResult.cbCardCount=CanOutCard.cbEachHandCardCount[cbCanOutIndex];
					memcpy(OutCardResult.cbResultCard,CanOutCard.cbCardData[cbCanOutIndex],CanOutCard.cbEachHandCardCount[cbCanOutIndex]*sizeof(BYTE));

					return ;
				}
			}

			if(cbOutCardType==CT_SINGLE)
			{
				//����������
				BYTE cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) ;

				if(GetCardLogicValue(cbTurnCardData[0])==14 || 
					GetCardLogicValue(cbTurnCardData[0])>=cbLargestLogicCard || 
					(GetCardLogicValue(cbTurnCardData[0])<cbLargestLogicCard-1) ||
					m_cbUserCardCount[m_wBankerUser]<=5)
				{
					//ȡһ�Ŵ��ڵ���2����Ҫ�ȵ������ƴ���ƣ�
					BYTE cbIndex=MAX_COUNT ;
					for(BYTE i=0; i<cbHandCardCount; ++i)
						if(GetCardLogicValue(cbHandCardData[i])>GetCardLogicValue(cbTurnCardData[0]) &&
							GetCardLogicValue(cbHandCardData[i])>=15)
						{
							cbIndex = i ;
						}
						if(cbIndex!=MAX_COUNT)
						{
							//���ñ�
							OutCardResult.cbCardCount=1;
							OutCardResult.cbResultCard[0] = cbHandCardData[cbIndex] ;

							return ;
						}
				}
			}

			//�����Ѳ���9�ص���ʱ
			WORD wMeChairID = (m_wBankerUser+1)%GAME_PLAYER ;
			WORD wFriendID = (wMeChairID+1)%GAME_PLAYER ;

			tagOutCardTypeResult FriendCardTypeResult[13] ;
			memset(FriendCardTypeResult,'\0', sizeof(FriendCardTypeResult)) ;
			AnalyseOutCardType(m_cbAllCardData[wFriendID], m_cbUserCardCount[wFriendID], cbTurnCardData, cbTurnCardCount, FriendCardTypeResult) ;

			//�����Ѳ���9�ص���ʱ
			if(m_cbUserCardCount[m_wBankerUser]<=4 && FriendCardTypeResult[cbOutCardType].cbCardTypeCount==0 && CardTypeResult[cbOutCardType].cbCardTypeCount>0)
			{
				BYTE cbMinSingleCount=MAX_COUNT ;
				BYTE Index=0 ;
				for(BYTE i=0; i<CardTypeResult[cbOutCardType].cbCardTypeCount; ++i)
				{
					BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CardTypeResult[cbOutCardType].cbCardData[i], CardTypeResult[cbOutCardType].cbEachHandCardCount[i]) ;
					if(cbMinSingleCount>=cbTmpCount)
					{
						cbMinSingleCount = cbTmpCount ;
						Index = i ;
					}
				}
				//���ñ�
				OutCardResult.cbCardCount=CardTypeResult[cbOutCardType].cbEachHandCardCount[Index];
				memcpy(OutCardResult.cbResultCard, CardTypeResult[cbOutCardType].cbCardData[Index], OutCardResult.cbCardCount) ;

				return ;
			}
		}

		//��Ҫ����ը��
		if(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount>0 || 
			(NORMAL_COUNT  == cbHandCardCount && NORMAL_COUNT == m_cbUserCardCount[(m_wBankerUser+2)%GAME_PLAYER]))
		{
			tagOutCardTypeResult const &BomCard = CardTypeResult[CT_BOMB_CARD] ;
			BYTE cbMinLogicValue = GetCardLogicValue(BomCard.cbCardData[0][0]) ;
			BYTE Index = 0 ;
			for(BYTE i=0; i<BomCard.cbCardTypeCount; ++i)
			{
				if(cbMinLogicValue>GetCardLogicValue(BomCard.cbCardData[i][0]))
				{
					cbMinLogicValue = GetCardLogicValue(BomCard.cbCardData[i][0]) ;
					Index = i ;
				}
			}

			bool bOutBomb = false ;

			//�����ж�
			if (NORMAL_COUNT  == cbHandCardCount && NORMAL_COUNT == m_cbUserCardCount[(m_wBankerUser+2)%GAME_PLAYER] && CT_ERROR != GetCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser]))
				bOutBomb = true ;

			//һ���ƺ�һ��ը��
			BYTE cbRemainCardData[20], cbRemainCardCount=cbHandCardCount ;
			memcpy(cbRemainCardData, cbHandCardData, cbHandCardCount) ;
			RemoveCard(BomCard.cbCardData[Index], BomCard.cbEachHandCardCount[Index], cbRemainCardData, cbRemainCardCount) ;
			cbRemainCardCount -= BomCard.cbEachHandCardCount[Index] ;
			if(CT_ERROR!=GetCardType(cbRemainCardData, cbRemainCardCount)) bOutBomb = true ;

			//ը������
			BYTE cbSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, BomCard.cbCardData[Index],BomCard.cbEachHandCardCount[Index]) ;
			if(cbSingleCardCount==0 && GetCardLogicValue(cbRemainCardData[0]) > GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0])) 
				bOutBomb = true ;

			//ֻʣһ��
			BYTE cbRemainCardType = GetCardType(m_cbAllCardData[wOutCardUser], m_cbUserCardCount[wOutCardUser]) ;
			if(cbRemainCardType>CT_ERROR && cbRemainCardType<CT_FOUR_LINE_TAKE_ONE && GetCardLogicValue(m_cbAllCardData[wOutCardUser][0])<15 && 
				cbSingleCardCount < 2 && (GetCardLogicValue(cbRemainCardData[0]) >= GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0])))
				bOutBomb = true ;

			//ֻʣ����
			BYTE cbRemainLargestCard = GetCardLogicValue(cbRemainCardData[0]) ;
			if(cbRemainCardCount<5 && cbRemainCardCount>0 && (cbRemainLargestCard!=GetCardLogicValue(BomCard.cbCardData[Index][0])) && 
				cbRemainLargestCard>GetCardLogicValue(m_cbAllCardData[wOutCardUser][0]) && cbRemainLargestCard > 14) bOutBomb = true ;

			//�����˿�
			tagAnalyseResult AnalyseResult ;
			AnalysebCardData(cbRemainCardData, cbRemainCardCount, AnalyseResult) ;

			if (m_cbUserCardCount[m_wBankerUser] == 1 && (AnalyseResult.cbDoubleCount * 2 + AnalyseResult.cbThreeCount * 3 + AnalyseResult.cbFourCount * 4 + 1 >= cbRemainCardCount)) bOutBomb = true ;


			//���ñ�
			if(bOutBomb)
			{
				OutCardResult.cbCardCount=BomCard.cbEachHandCardCount[Index];
				memcpy(OutCardResult.cbResultCard,BomCard.cbCardData[Index],BomCard.cbEachHandCardCount[Index]*sizeof(BYTE));
			}

			return ;
		}

		return ;
	}
	return ;
}

//�з��ж�
BYTE CAndroidAI::AnalyseLandScore(WORD wMeChairID, BYTE cbCurrentLandScore)
{
//#ifdef _DEBUG
//	//����з�
//	return 255 ;
//#endif

	//������Ŀ
	BYTE cbLargeCardCount = 0 ;
	BYTE Index=0 ;
	while(GetCardLogicValue(m_cbLandScoreCardData[Index++])>=15) ++cbLargeCardCount ;

	//���Ƹ���
	BYTE cbSingleCardCount=AnalyseSinleCardCount(m_cbLandScoreCardData, sizeof(m_cbLandScoreCardData), NULL, 0) ;

	//��}��
	if(cbLargeCardCount>=4 && cbSingleCardCount<=4) return 3 ;

	//����з�
	if(cbLargeCardCount<=2 || cbCurrentLandScore>=1) return 255 ;

	//������
	BYTE cbMinSingleCardCount=MAX_COUNT ;
	for(WORD wChairID=0 , i=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		BYTE cbTmpSingleCardCount = AnalyseSinleCardCount(m_cbAllCardData[wChairID], NORMAL_COUNT, NULL, 0) ;
		if(wChairID!=wMeChairID && cbTmpSingleCardCount<cbMinSingleCardCount) cbTmpSingleCardCount=cbMinSingleCardCount ;
	}

	//��һ��
	if(cbLargeCardCount>=3 && cbSingleCardCount<cbMinSingleCardCount-3) return 2 ;

	//����з�
	return 255 ;
}

//���Ʋ���
bool CAndroidAI::_TestOutAllCard(WORD wTestUser, BYTE cbWantOutCardData[], BYTE cbWantOutCardCount, BYTE	cbAllCardData[GAME_PLAYER][MAX_COUNT], BYTE cbUserCardCount[GAME_PLAYER], bool bFirstOutCard)
{
	//�˵ݹ��ݲ�����
	//return false ;

	//����ID
	WORD wFriendID = GAME_PLAYER ;
	if(wTestUser != m_wBankerUser) wFriendID = (wTestUser==((m_wBankerUser+1)%GAME_PLAYER)) ? (m_wBankerUser+2)%GAME_PLAYER : (m_wBankerUser+1)%GAME_PLAYER ;

	//��ʱֻ����������
//	for(WORD wUser=0; wUser<GAME_PLAYER; ++wUser) if(wFriendID!=wUser && cbUserCardCount[wUser]>10) return false ;

//	m_lRecCount++ ;
	//��ֹ���ݹ�
//	if(m_lRecCount>50) return false ;
	
	BYTE cbWantOutCardType = GetCardType(cbWantOutCardData, cbWantOutCardCount) ;

	//�����ж�
	if ( cbUserCardCount[wTestUser] == cbWantOutCardCount ) return true ;

    //�����ж�
	//try
	{
	for(WORD wUser=0; wUser<GAME_PLAYER; ++wUser)
	{
		if(wUser!=wTestUser && wFriendID!=wUser)
		{
			tagOutCardTypeResult CardTypeResult[12+1] ;
			memset(CardTypeResult, '\0',sizeof(CardTypeResult)) ;

			//���Ʒ���
			//try
			{
			AnalyseOutCardType(cbAllCardData[wUser], cbUserCardCount[wUser], cbWantOutCardData, cbWantOutCardCount, CardTypeResult) ;
			}
			//catch(...)
			{
				//return false ;
			}
			for(BYTE cbCardType=CT_SINGLE; cbCardType<=CT_MISSILE_CARD; ++cbCardType)
				if(CardTypeResult[cbCardType].cbCardTypeCount>0) return false ;
		}
	}
	}
	//catch(...)
	{
		//return false ;
	}

	//��ը�ж�
	BYTE cbAllBombCard[MAX_COUNT], cbAllBombCount=0 ;
	GetAllBomCard(cbAllCardData[wTestUser], cbUserCardCount[wTestUser], cbAllBombCard, cbAllBombCount) ;
	if (cbAllBombCount > 0)
	{
		//ʣ���˿�
		BYTE cbRemainCard[MAX_COUNT], cbRemainCardCount ;

		memcpy(cbRemainCard, cbAllCardData[wTestUser], cbUserCardCount[wTestUser]) ;
		cbRemainCardCount = cbUserCardCount[wTestUser] ;

		RemoveCard(cbWantOutCardData, cbWantOutCardCount, cbRemainCard, cbRemainCardCount) ;
		cbRemainCardCount -= cbWantOutCardCount ;

		//�޴�����
		if (CT_FOUR_LINE_TAKE_ONE == cbWantOutCardType || CT_FOUR_LINE_TAKE_TWO==cbWantOutCardType) return false ;

		//ֻʣը��
		if (cbWantOutCardType >= CT_BOMB_CARD && cbWantOutCardCount == cbUserCardCount[wTestUser]) return true ; 

		//ֻʣը��
		if (cbAllBombCount == cbUserCardCount[wTestUser] && cbWantOutCardType >= CT_BOMB_CARD) return true ;

		//ը����һ��
		if ( cbWantOutCardType >= CT_BOMB_CARD && GetCardType(cbRemainCard, cbRemainCardCount) != CT_ERROR && 
			CT_FOUR_LINE_TAKE_TWO != GetCardType(cbRemainCard, cbRemainCardCount) && CT_FOUR_LINE_TAKE_ONE != GetCardType(cbRemainCard, cbRemainCardCount) )
			return true ;

		//�׳���ʱ����ը��
		if (cbWantOutCardType >= CT_BOMB_CARD && bFirstOutCard)
		{
			//����ֻʣһ����
			if (wTestUser != m_wBankerUser && GetCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser]) == CT_ERROR)
				return false ;

				WORD wUndersideOfBanker = (m_wBankerUser+1)%GAME_PLAYER ;	//�����¼�
				WORD wUpsideOfBanker = (wUndersideOfBanker+1)%GAME_PLAYER ;	//�����ϼ�

			//�м�ֻʣһ����
			if (wTestUser == m_wBankerUser && GetCardType(m_cbAllCardData[wUndersideOfBanker], m_cbUserCardCount[wUndersideOfBanker]) == CT_ERROR &&
				GetCardType(m_cbAllCardData[wUpsideOfBanker], m_cbUserCardCount[wUpsideOfBanker]) == CT_ERROR)
				return false ;
		}

		//��ը�ж�
		if (cbWantOutCardType < CT_FOUR_LINE_TAKE_ONE && cbWantOutCardType != CT_SINGLE_LINE)
		{
			for (BYTE i=0; i < cbWantOutCardCount; ++i)
				for (BYTE j=0; j < cbAllBombCount; ++j)
				{
					if (GetCardValue(cbAllBombCard[j]) == GetCardValue(cbWantOutCardData[i])) return false ;
				}
		}
	}

	if(cbUserCardCount[wTestUser]==0) return true ;

	//�ݹ鴦��
	//try
	{
	assert(cbUserCardCount[wTestUser] >= cbWantOutCardCount) ;
	if (cbUserCardCount[wTestUser] < cbWantOutCardCount ) return false ;
	RemoveCard(cbWantOutCardData, cbWantOutCardCount, cbAllCardData[wTestUser], cbUserCardCount[wTestUser]) ;
	cbUserCardCount[wTestUser] -= cbWantOutCardCount ;

	//�������
	if (cbWantOutCardType >= CT_THREE_LINE_TAKE_ONE && cbWantOutCardType <= CT_FOUR_LINE_TAKE_TWO && !bFirstOutCard)
	{
		bool bHaveLargeCard = false ;
		for (BYTE i = 3; i < cbWantOutCardCount; ++i) 
			if (GetCardLogicValue(cbWantOutCardData[i]) >= 15)
			{
				bHaveLargeCard = true ;
				break ;
			}
		if (bHaveLargeCard)
		{
			for (BYTE i = 0; i < cbUserCardCount[wTestUser]; ++i)
				if (GetCardLogicValue(cbAllCardData[wTestUser][i]) < 15) return false ;
		}
	}
	}
	//catch(...)
	{
		//return false ;
	}
	if(cbUserCardCount[wTestUser]!=0)
	{
		tagOutCardTypeResult CardTypeResult[12+1] ;
		memset(CardTypeResult, '\0',sizeof(CardTypeResult)) ;

		//���Ʒ���
		//try
		{
		AnalyseOutCardType(cbAllCardData[wTestUser], cbUserCardCount[wTestUser], CardTypeResult) ;
		}
		//catch(...)
		{
			//return false ;
		}
		for(BYTE cbCardType=CT_SINGLE; cbCardType<=CT_MISSILE_CARD; ++cbCardType)
			if(CardTypeResult[cbCardType].cbCardTypeCount>0) 
			{
				for(LONG lIndex=0; lIndex<CardTypeResult[cbCardType].cbCardTypeCount; ++lIndex)
				{
					//�������
					BYTE cbTmpCardData[GAME_PLAYER][MAX_COUNT], cbTmpCardCount[GAME_PLAYER] ;
					memcpy(cbTmpCardData, cbAllCardData, sizeof(cbTmpCardData)) ;
					memcpy(cbTmpCardCount, cbUserCardCount, sizeof(cbTmpCardCount)) ;

					//�ݹ����
					//try
					{
					if(_TestOutAllCard(wTestUser, CardTypeResult[cbCardType].cbCardData[lIndex], CardTypeResult[cbCardType].cbEachHandCardCount[lIndex], cbTmpCardData, cbTmpCardCount, bFirstOutCard))
						return true ;
					}
					//catch(...)
					{
						//return false ;
					}
				}
			}
	}
	else
		return true ;

	return false ;
}


//�Ĵ�����
bool CAndroidAI::AnalyseFourCardType( BYTE const cbHandCardData[MAX_COUNT], BYTE cbHandCardCount, BYTE cbEnemyCardData[MAX_COUNT], BYTE cbEnemyCardCount, tagOutCardResult &CardResult )
{
	//��ʼ��
	memset(&CardResult,'\0',sizeof(CardResult)) ;

	//�����ж�
	if ( cbHandCardCount < 5 ) return false ;

	//�Է����ͷ���
	if ( GetCardType(cbEnemyCardData, cbEnemyCardCount) == CT_SINGLE )
	{
		//�����ж�
		if ( GetCardLogicValue(cbHandCardData[1] ) < 16 )
		{
			//�����˿�
			tagAnalyseResult AnalyseResult ;
			AnalysebCardData(cbHandCardData, cbHandCardCount, AnalyseResult) ;

			//��������
			if ( cbHandCardCount != ( AnalyseResult.cbFourCount * 4 + AnalyseResult.cbDoubleCount * 2 + AnalyseResult.cbSignedCount ) || 
				AnalyseResult.cbFourCount <= 0 ) return false ;

			//�Է�����
			if ( GetCardType(cbEnemyCardData, cbEnemyCardCount) == CT_SINGLE )
			{
				//�����ж�
				if ( AnalyseResult.cbSignedCount < 2 ) return false ;

				//��С�ж�
				if ( GetCardLogicValue(AnalyseResult.cbSignedCardData[0]) >= GetCardLogicValue(cbEnemyCardData[0]) ) return false ;

				//���Ŵ�}��
				if ( AnalyseResult.cbFourCount >= 1 && AnalyseResult.cbSignedCount == 2 )
				{
					//ը������
					BYTE cbFourCardIndex = ( AnalyseResult.cbFourCount - 1 ) * 4 ;

					//���ؽ��
					CardResult.cbCardCount = 4 + 2 ;
					memcpy(CardResult.cbResultCard, AnalyseResult.cbFourCardData + cbFourCardIndex, 4 ) ;
					CardResult.cbResultCard[4] = AnalyseResult.cbSignedCardData[0] ;
					CardResult.cbResultCard[5] = AnalyseResult.cbSignedCardData[1] ;

					return true ;
				}
				//���Ŵ���
				else if ( AnalyseResult.cbFourCount >= 1 && AnalyseResult.cbSignedCount == 3 )
				{
					//ը������
					BYTE cbFourCardIndex = ( AnalyseResult.cbFourCount - 1 ) * 4 ;

					//���ؽ��
					CardResult.cbCardCount = 4 + 2 ;
					memcpy(CardResult.cbResultCard, AnalyseResult.cbFourCardData + cbFourCardIndex, 4 ) ;
					CardResult.cbResultCard[4] = AnalyseResult.cbSignedCardData[1] ;
					CardResult.cbResultCard[5] = AnalyseResult.cbSignedCardData[2] ;

					return true ;
				}
			}
			else if ( GetCardType(cbEnemyCardData, cbEnemyCardCount) == CT_DOUBLE )
			{
			}

		}
		//�����ж�
		else if ( GetCardLogicValue(cbHandCardData[1] ) == 16 )
		{
			//�����˿�
			tagAnalyseResult AnalyseResult ;
			AnalysebCardData(cbHandCardData, cbHandCardCount, AnalyseResult) ;

			//��������
			if ( cbHandCardCount != ( AnalyseResult.cbFourCount * 4 + AnalyseResult.cbDoubleCount * 2 + AnalyseResult.cbSignedCount ) || 
				AnalyseResult.cbFourCount <= 0 ) return false ;

			//�Է�����
			if ( GetCardType(cbEnemyCardData, cbEnemyCardCount) == CT_SINGLE )
			{
				//�����ж�
				if ( AnalyseResult.cbSignedCount < 4 ) return false ;

				//��С�ж�
				if ( GetCardLogicValue(AnalyseResult.cbSignedCardData[2]) >= GetCardLogicValue(cbEnemyCardData[0]) ) return false ;

				//���Ŵ�}��
				if ( AnalyseResult.cbFourCount >= 1 && AnalyseResult.cbSignedCount == 4 )
				{
					//ը������
					BYTE cbFourCardIndex = ( AnalyseResult.cbFourCount - 1 ) * 4 ;

					//���ؽ��
					CardResult.cbCardCount = 4 + 2 ;
					memcpy(CardResult.cbResultCard, AnalyseResult.cbFourCardData + cbFourCardIndex, 4 ) ;
					CardResult.cbResultCard[4] = AnalyseResult.cbSignedCardData[2] ;
					CardResult.cbResultCard[5] = AnalyseResult.cbSignedCardData[3] ;

					return true ;
				}
				//���Ŵ���
				else if ( AnalyseResult.cbFourCount >= 1 && AnalyseResult.cbSignedCount == 5 )
				{
					//ը������
					BYTE cbFourCardIndex = ( AnalyseResult.cbFourCount - 1 ) * 4 ;

					//���ؽ��
					CardResult.cbCardCount = 4 + 2 ;
					memcpy(CardResult.cbResultCard, AnalyseResult.cbFourCardData + cbFourCardIndex, 4 ) ;
					CardResult.cbResultCard[4] = AnalyseResult.cbSignedCardData[3] ;
					CardResult.cbResultCard[5] = AnalyseResult.cbSignedCardData[4] ;

					return true ;
				}
			}
		}
	}
	////һ�Ŵ��ƴ�һ��
	//else if (GetCardType(cbEnemyCardData, cbEnemyCardCount) == CT_ERROR && cbEnemyCardCount == 2 )
	//{
	//	//�����ж�
	//	if ( GetCardLogicValue(cbHandCardData[2] ) < 16 )
	//	{
	//		//�����˿�
	//		tagAnalyseResult AnalyseResult ;
	//		AnalysebCardData(cbHandCardData, cbHandCardCount, AnalyseResult) ;

	//		//��������
	//		if ( cbHandCardCount != ( AnalyseResult.cbFourCount * 4 + AnalyseResult.cbDoubleCount * 2 + AnalyseResult.cbSignedCount ) || 
	//			AnalyseResult.cbFourCount <= 0 ) return false ;

	//		//�����ж�
	//		if ( AnalyseResult.cbSignedCount < 2 ) return false ;

	//		//���Ŵ���
	//		if ( AnalyseResult.cbFourCount >= 1 && AnalyseResult.cbSignedCount == 3 )
	//		{
	//			//��С�ж�
	//			if ( GetCardLogicValue(AnalyseResult.cbSignedCardData[0]) > GetCardLogicValue(cbEnemyCardData[0]) ) return false ;

	//			//ը������
	//			BYTE cbFourCardIndex = ( AnalyseResult.cbFourCount - 1 ) * 4 ;

	//			//���ؽ��
	//			CardResult.cbCardCount = 4 + 2 ;
	//			memcpy(CardResult.cbResultCard, AnalyseResult.cbFourCardData + cbFourCardIndex, 4 ) ;
	//			CardResult.cbResultCard[4] = AnalyseResult.cbSignedCardData[1] ;
	//			CardResult.cbResultCard[5] = AnalyseResult.cbSignedCardData[2] ;

	//			return true ;
	//		}
	//	}
	//}
	return false ;
}


//���Ʋ���
bool CAndroidAI::TestOutAllCard( WORD wTestUser, BYTE cbWantOutCardData[], BYTE cbWantOutCardCount, bool bFirstOutCard ) {
/*

		if ( ! VerifyOutCard( wTestUser, cbWantOutCardData, cbWantOutCardCount, m_cbAllCardData[ wTestUser ], m_cbUserCardCount[ wTestUser ], bFirstOutCard ) ) {
			return false;
		}
*/
		//ģ��ݹ鴦��
		if ( cbWantOutCardCount != m_cbUserCardCount[ wTestUser ] ) {

			//��ʼջ
			m_StackHandCardInfo.InitStack();

			//��һ��Ԫ��
			tagHandCardInfo OriginHandCardInfo ;

			//�����˿�
			memcpy( OriginHandCardInfo.cbHandCardData, m_cbAllCardData[ wTestUser ], m_cbUserCardCount[ wTestUser ] );
			OriginHandCardInfo.cbHandCardCount = m_cbUserCardCount[ wTestUser ];

			//�Ƴ��һ����
			RemoveCard( cbWantOutCardData, cbWantOutCardCount, OriginHandCardInfo.cbHandCardData, OriginHandCardInfo.cbHandCardCount );
			OriginHandCardInfo.cbHandCardCount -= cbWantOutCardCount;

			//�����˿�
			//try {
				AnalyseOutCardType( OriginHandCardInfo.cbHandCardData, OriginHandCardInfo.cbHandCardCount, OriginHandCardInfo.CardTypeResult ) ;
			//}
			//catch( ... ) {
				//return false ;
			//}

			//Ԫ��ѹջ
			m_StackHandCardInfo.Push( &OriginHandCardInfo );

			//�������
			LONG lJudgeCount = 0;
		}
		else {
			return true ;
		}


	return false ;
}

//����ж�
bool CAndroidAI::IsLargestCard( WORD wTestUser, BYTE const cbWantOutCardData[], BYTE const cbWantOutCardCount ) {

	//����ID
	WORD wFriendID = GAME_PLAYER ;
	if ( wTestUser != m_wBankerUser ) {
		wFriendID = ( wTestUser == ( ( m_wBankerUser + 1 ) % GAME_PLAYER ) ) ? ( m_wBankerUser + 2 ) % GAME_PLAYER : 
			( m_wBankerUser + 1 ) % GAME_PLAYER ;
	}

	//�����ж�
	//try{
		for ( WORD wUser = 0; wUser < GAME_PLAYER; ++wUser ) {
			if ( wUser != wTestUser && wFriendID != wUser ) {
				tagOutCardTypeResult CardTypeResult[ 12 + 1 ] ;
				memset( CardTypeResult,'\0',sizeof( CardTypeResult ) ) ;

				//���Ʒ���
				//try {
					AnalyseOutCardType( m_cbAllCardData[ wUser ], m_cbUserCardCount[ wUser ], cbWantOutCardData, cbWantOutCardCount, CardTypeResult ) ;
				//}
				//catch(...) {
					//return false ;
				//}

				for ( BYTE cbCardType = CT_SINGLE; cbCardType <= CT_MISSILE_CARD; ++cbCardType ) {
					if ( 0 < CardTypeResult[ cbCardType ].cbCardTypeCount ) {
						return false ;
					}
				}
			}
		}
	//}
	//catch( ... ) {
		//return false ;
	//}

	return true;
}

//�Ƿ��ܳ�
bool CAndroidAI::VerifyOutCard( WORD wTestUser, BYTE const cbWantOutCardData[], BYTE const cbWantOutCardCount, BYTE const cbCurHandCardData[ MAX_COUNT ], BYTE const cbCurHandCardCount, bool bFirstOutCard ) {

	//����ID
	WORD wFriendID = GAME_PLAYER ;
	if ( wTestUser != m_wBankerUser ) {
		wFriendID = ( wTestUser == ( ( m_wBankerUser + 1 ) % GAME_PLAYER ) ) ? ( m_wBankerUser + 2 ) % GAME_PLAYER : 
			( m_wBankerUser + 1 ) % GAME_PLAYER ;
	}

	BYTE cbWantOutCardType = GetCardType( cbWantOutCardData, cbWantOutCardCount ) ;

	//�׳��ж�
	if ( bFirstOutCard && cbWantOutCardCount == cbCurHandCardCount ) {
		return true;
	}

	//�Ƿ����
	if ( ! IsLargestCard( wTestUser, cbWantOutCardData, cbWantOutCardCount ) ) {
		return false;
	}

	//��ը�ж�
	BYTE cbAllBombCard[ MAX_COUNT ], cbAllBombCount = 0 ;
	GetAllBomCard( cbCurHandCardData, cbCurHandCardCount, cbAllBombCard, cbAllBombCount ) ;
	fprintf(stderr, "ddddddddd\n");
	if ( 0 < cbAllBombCount ) {
		//ʣ���˿�
		BYTE cbRemainCard [MAX_COUNT ], cbRemainCardCount ;

		memcpy( cbRemainCard, cbCurHandCardData, cbCurHandCardCount ) ;
		cbRemainCardCount = cbCurHandCardCount ;

		RemoveCard( cbWantOutCardData, cbWantOutCardCount, cbRemainCard, cbRemainCardCount ) ;
		cbRemainCardCount -= cbWantOutCardCount ;

		//�޴�����
		if ( CT_FOUR_LINE_TAKE_ONE == cbWantOutCardType || CT_FOUR_LINE_TAKE_TWO==cbWantOutCardType ) return false ;

		//ֻʣը��
		if ( CT_BOMB_CARD <= cbWantOutCardType && cbWantOutCardCount == cbCurHandCardCount ) return true ; 

		//ֻʣը��
		if ( cbAllBombCount == m_cbUserCardCount[ wTestUser ] && CT_BOMB_CARD <= cbWantOutCardType ) return true ;

		//ը����һ��
		if ( CT_BOMB_CARD <= cbWantOutCardType && GetCardType(cbRemainCard, cbRemainCardCount) != CT_ERROR && 
			CT_FOUR_LINE_TAKE_TWO != GetCardType( cbRemainCard, cbRemainCardCount ) && CT_FOUR_LINE_TAKE_ONE != GetCardType( cbRemainCard, cbRemainCardCount ) ) {
			return true ;
		}

		//�׳���ʱ����ը��
		if (CT_BOMB_CARD <= cbWantOutCardType && bFirstOutCard ) {
			//����ֻʣһ����
			if ( wTestUser != m_wBankerUser && GetCardType( m_cbAllCardData[ m_wBankerUser ], m_cbUserCardCount[ m_wBankerUser ] ) == CT_ERROR ) {
				return false ;
			}

			WORD wUndersideOfBanker = (m_wBankerUser+1)%GAME_PLAYER ;	//�����¼�
			WORD wUpsideOfBanker = (wUndersideOfBanker+1)%GAME_PLAYER ;	//�����ϼ�

			//�м�ֻʣһ����
			if ( wTestUser == m_wBankerUser && GetCardType( m_cbAllCardData[ wUndersideOfBanker ], m_cbUserCardCount[ wUndersideOfBanker ] ) == CT_ERROR &&
				GetCardType( m_cbAllCardData[ wUpsideOfBanker ], m_cbUserCardCount[ wUpsideOfBanker ] ) == CT_ERROR ) {
				return false ;
			}
		}

		//��ը�ж�
		if ( cbWantOutCardType < CT_FOUR_LINE_TAKE_ONE && cbWantOutCardType != CT_SINGLE_LINE ) {
			for ( BYTE i = 0; i < cbWantOutCardCount; ++i )
				for ( BYTE j = 0; j < cbAllBombCount; ++j ) {
					if ( GetCardValue( cbAllBombCard[ j ] ) == GetCardValue( cbWantOutCardData[ i ] ) ) return false ;
				}
		}
	}

	//�����ж�
	if ( cbCurHandCardCount == cbWantOutCardCount ) {
		return true ;
	}
	fprintf(stderr, "ccccc\n");

	if ( cbCurHandCardCount == 0 ) {
		return true ;
	}

		assert( cbWantOutCardCount <= cbCurHandCardCount ) ;
		if ( cbCurHandCardCount < cbWantOutCardCount ) {
			return false ;
		}

		//ʣ���˿�
		BYTE cbRemainCard [MAX_COUNT ], cbRemainCardCount ;

		memcpy( cbRemainCard, cbCurHandCardData, cbCurHandCardCount ) ;
		cbRemainCardCount = cbCurHandCardCount ;

		RemoveCard( cbWantOutCardData, cbWantOutCardCount, cbRemainCard, cbRemainCardCount ) ;
		cbRemainCardCount -= cbWantOutCardCount ;

		if ( cbRemainCardCount == 0 ) {
			return false;
		}
		fprintf(stderr, "fffff\n");
		//�������
		if ( CT_THREE_LINE_TAKE_ONE <= cbWantOutCardType && cbWantOutCardType <= CT_FOUR_LINE_TAKE_TWO && ! bFirstOutCard ) {
			bool bHaveLargeCard = false ;
			for ( BYTE i = 3; i < cbWantOutCardCount; ++i ) 
				if ( 15 <= GetCardLogicValue( cbWantOutCardData[ i ]) ) {
					bHaveLargeCard = true ;
					break ;
				}
				if ( bHaveLargeCard ) {
					for ( BYTE i = 0; i < cbRemainCardCount; ++i ) {
						if ( GetCardLogicValue( cbRemainCard[ i ]) < 15 ) {
							return false ;
						}
					}
				}
		}

	fprintf(stderr, "aaaaaaaaaaa\n");
	return true;
}

//�����˿�
void CAndroidAI::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//��Ŀ����
	if (cbCardCount==0) return;

	//ת����ֵ
	BYTE cbSortValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardLogicValue(cbCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE cbThreeCount,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if ((cbSortValue[i]<cbSortValue[i+1])||
				((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//����λ��
				cbThreeCount=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbThreeCount;
				cbThreeCount=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=cbThreeCount;
				bSorted=false;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	//��Ŀ����
	if (cbSortType==ST_COUNT)
	{
		//�����˿�
		BYTE cbIndex=0;
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

		//��������
		memcpy(&cbCardData[cbIndex],AnalyseResult.cbFourCardData,sizeof(BYTE)*AnalyseResult.cbFourCount*4);
		cbIndex+=AnalyseResult.cbFourCount*4;

		//��������
		memcpy(&cbCardData[cbIndex],AnalyseResult.cbThreeCardData,sizeof(BYTE)*AnalyseResult.cbThreeCount*3);
		cbIndex+=AnalyseResult.cbThreeCount*3;

		//��������
		memcpy(&cbCardData[cbIndex],AnalyseResult.cbDoubleCardData,sizeof(BYTE)*AnalyseResult.cbDoubleCount*2);
		cbIndex+=AnalyseResult.cbDoubleCount*2;

		//��������
		memcpy(&cbCardData[cbIndex],AnalyseResult.cbSignedCardData,sizeof(BYTE)*AnalyseResult.cbSignedCount);
		cbIndex+=AnalyseResult.cbSignedCount;
	}

	return;
}

//ɾ���˿�
bool CAndroidAI::RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//�������
	assert(cbRemoveCount<=cbCardCount);
	if(cbRemoveCount>cbCardCount)
		return false ;

	//�����
	BYTE cbDeleteCount=0,cbTempCardData[MAX_COUNT];
	if (cbCardCount>CountArray(cbTempCardData)) return false;
	memcpy(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//�����˿�
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		for (BYTE j=0;j<cbCardCount;j++)
		{
			if (cbRemoveCard[i]==cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j]=0;
				break;
			}
		}
	}
	if (cbDeleteCount!=cbRemoveCount) return false;

	//�����˿�
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}

//�����˿�
bool CAndroidAI::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
	//���ý��
	memset(&AnalyseResult,0,sizeof(AnalyseResult));

	//�˿˷���
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//������
		BYTE cbSameCount=1;
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);
		if(cbLogicValue<=0) 
			return false;

		//����ͬ��
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			//��ȡ�˿�
			if (GetCardLogicValue(cbCardData[j])!=cbLogicValue) break;

			//���ñ�
			cbSameCount++;
		}

		//���ý��
		switch (cbSameCount)
		{
		case 1:		//����
			{
				BYTE cbIndex=AnalyseResult.cbSignedCount++;
				AnalyseResult.cbSignedCardData[cbIndex*cbSameCount]=cbCardData[i];
				break;
			}
		case 2:		//}��
			{
				BYTE cbIndex=AnalyseResult.cbDoubleCount++;
				AnalyseResult.cbDoubleCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbDoubleCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				break;
			}
		case 3:		//����
			{
				BYTE cbIndex=AnalyseResult.cbThreeCount++;
				AnalyseResult.cbThreeCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbThreeCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbThreeCardData[cbIndex*cbSameCount+2]=cbCardData[i+2];
				break;
			}
		case 4:		//����
			{
				BYTE cbIndex=AnalyseResult.cbFourCount++;
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount+2]=cbCardData[i+2];
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount+3]=cbCardData[i+3];
				break;
			}
		}

		//��������
		i+=cbSameCount-1;
	}

	return true;
}
