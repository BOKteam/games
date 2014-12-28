#ifndef _ANDROIDAI_H_
#define _ANDROIDAI_H_


#include <vector>
#include <queue>
#include <list>
#include "GameLogic.h"
#pragma once


typedef int INT_PTR;
#define INVALID_CHAIR 0xFFFF
//��ϷAI

//�˿���Ϣ
class tagHandCardInfo
{
public:
	BYTE						cbHandCardData[ MAX_COUNT ];				//�˿����
	BYTE						cbHandCardCount;							//�˿���Ŀ
	tagOutCardTypeResult		CardTypeResult[ 12 + 1 ] ;					//�������

	//��ʼ���
	tagHandCardInfo( void )
	{
		memset( cbHandCardData, '\0',sizeof( cbHandCardData ) ) ;
		cbHandCardCount = 0;
		memset( &CardTypeResult,'\0', sizeof( CardTypeResult ) );
	}
};

//���Ͷ���
//std::
typedef std::list< tagHandCardInfo * > tagHandCardInfoArray;

//ջ�ṹ
class tagStackHandCardInfo
{

	//��j����
public:

	//���캯��
	tagStackHandCardInfo( void ) 
	{ 
		m_HandCardInfoFreeArray.clear();
		m_HandCardInfoArray.clear();
	}

	//�����
	~tagStackHandCardInfo( void ) 
	{

		//���ջ
		ClearAll();
	}

	//Ԫ��ѹջ
	void Push( tagHandCardInfo * pHandCardInfo ) 
	{

		//�Ƿ��пռ�
		if ( 0 < m_HandCardInfoFreeArray.size() )
		{
			//��ȡ�ռ�
			tagHandCardInfo * pHandCardInfoFree = m_HandCardInfoFreeArray.front();
			m_HandCardInfoFreeArray.pop_front();

			//Ԫ�ظ�ֵ
			memcpy( pHandCardInfoFree->cbHandCardData, pHandCardInfo->cbHandCardData, sizeof( pHandCardInfoFree->cbHandCardData ) );
			pHandCardInfoFree->cbHandCardCount = pHandCardInfo->cbHandCardCount;
			memcpy( pHandCardInfoFree->CardTypeResult, pHandCardInfo->CardTypeResult, sizeof( pHandCardInfo->CardTypeResult ) );

			//ѹ��ջ��
			//INT_PTR nECount = m_HandCardInfoArray.size() ;
			m_HandCardInfoArray.push_back( pHandCardInfoFree );
		}
		else 
		{
			//����ռ�
			tagHandCardInfo * pNewHandCardInfo = new tagHandCardInfo() ;

			//Ԫ�ظ�ֵ
			memcpy( pNewHandCardInfo->cbHandCardData, pHandCardInfo->cbHandCardData, sizeof( pNewHandCardInfo->cbHandCardData ) );
			pNewHandCardInfo->cbHandCardCount = pHandCardInfo->cbHandCardCount;
			memcpy( pNewHandCardInfo->CardTypeResult, pHandCardInfo->CardTypeResult, sizeof( pHandCardInfo->CardTypeResult ) );

			//ѹ��ջ��
			//INT_PTR nECount = m_HandCardInfoArray.size() ;
			m_HandCardInfoArray.push_back(pNewHandCardInfo);
			//m_HandCardInfoArray.InsertAt( nECount, pNewHandCardInfo );
		}

	}

	//����ջ��
	void Pop()
	{

		//�ǿ��ж�
		if ( IsEmpty() ) return ;

		//��ȡԪ��
		//INT_PTR nECount = m_HandCardInfoArray.size() ;
		tagHandCardInfo * pTopHandCardInfo = m_HandCardInfoArray.back();

		//�Ƴ�Ԫ��
		m_HandCardInfoArray.pop_back();

		//����ռ�
		m_HandCardInfoFreeArray.push_back( pTopHandCardInfo );
	}

	//��ʼջ
	void InitStack() 
	{

		//����ռ�
		while ( 0 < m_HandCardInfoArray.size() )
		{
			tagHandCardInfo *pHandCardInfo = m_HandCardInfoArray.front();
			m_HandCardInfoArray.pop_front();
			m_HandCardInfoFreeArray.push_back( pHandCardInfo );
		}
	}

	//���ջ
	void ClearAll() 
	{

		//�ͷ��ڴ�
		while ( 0 < m_HandCardInfoArray.size() )
		{
			tagHandCardInfo *pHandCardInfo = m_HandCardInfoArray.front();
			delete pHandCardInfo;
			pHandCardInfo = NULL;
			m_HandCardInfoArray.pop_front();
		}

		//�ͷ��ڴ�
		while ( 0 < m_HandCardInfoFreeArray.size() )
		{
			tagHandCardInfo *pHandCardInfo = m_HandCardInfoFreeArray.front();
			delete pHandCardInfo;
			pHandCardInfo = NULL;
			m_HandCardInfoFreeArray.pop_front();
			//m_HandCardInfoFreeArray.pop_back();
		}
	}

	//��ȡջ��
	void GetTop( tagHandCardInfo * & pHandCardInfo )
	{

		//�ǿ��ж�
		if ( IsEmpty() )
		{
			assert( false );
			return;
		}

		//��ȡԪ��
		INT_PTR nECount = m_HandCardInfoArray.size() ;
		pHandCardInfo = m_HandCardInfoArray.back();
	}

	//���ж�
	bool IsEmpty()
	{
		return m_HandCardInfoArray.empty();
	}

	//��Ա��
private:
	tagHandCardInfoArray			m_HandCardInfoFreeArray;					//�˿���Ϣ
	tagHandCardInfoArray			m_HandCardInfoArray;						//�˿���Ϣ

};

//class CAndroidAIBase
//{
//public:
//	CAndroidAIBase();
//	~CAndroidAIBase();
//
//	//���ܺ���
//public:
//	//�������
//	void SetMagicCard(BYTE cbCardData);
//	//�����˿�
//	void SetUserCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount) ;
//	//���õ���
//	void SetBackCard(WORD wChairID, BYTE cbBackCardData[], BYTE cbCardCount) ;
//	//����ׯ��
//	void SetBanker(WORD wBanker) ;
//	//�����˿�
//	void SetLandScoreCardData(BYTE cbCardData[], BYTE cbCardCount) ;
//	//ɾ���˿�
//	void RemoveUserCardData(WORD wChairID, BYTE cbRemoveCardData[], BYTE cbRemoveCardCount) ;
//
//	//�˿����
//protected:
//	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT]; //���ϵ���
//	BYTE							m_cbCardCount[GAME_PLAYER];				  //�˿���Ŀ
//	BYTE							m_cbBackCard[MAX_COUNT];	              //�����˿�
//	BYTE                            m_cbMagicCard;                            //�����
//	WORD							m_wBankerUser;						      //�������
//	LONG							m_lBankerOutCardCount ;					  //���ƴ���
//};

class CAndroidAI
{
public:
	CAndroidAI(void);
	~CAndroidAI(void);

	//���ú���
public:
	//�����˿�
	void SetUserCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount) ;
	//���õ���
	void SetBackCard(WORD wChairID, BYTE cbBackCardData[], BYTE cbCardCount) ;
	//����ׯ��
	void SetBanker(WORD wBanker) ;
	//�����˿�
	void SetLandScoreCardData(BYTE cbCardData[], BYTE cbCardCount) ;
	//ɾ���˿�
	void RemoveUserCardData(WORD wChairID, BYTE cbRemoveCardData[], BYTE cbRemoveCardCount);
	//������
	void ClearAllCard();

	//�߼�����
public:
	//�Ա��˿�
	bool CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount);
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);
	//ɾ���˿�
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ����
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);

	//������
protected:
	//����㷨
	void Combination(BYTE cbCombineCardData[], BYTE cbResComLen,  BYTE cbResultCardData[254][5], BYTE &cbResCardLen,BYTE cbSrcCardData[] , BYTE cbCombineLen1, BYTE cbSrcLen, const BYTE cbCombineLen2);
	//�����㷨
	void Permutation(BYTE *list, int m, int n, BYTE result[][4], BYTE &len) ;
	//����ը��
	void GetAllBomCard(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbBomCardData[], BYTE &cbBomCardCount);
	//����˳��
	void GetAllLineCard(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbLineCardData[], BYTE &cbLineCardCount);
	//��������
	void GetAllThreeCard(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbThreeCardData[], BYTE &cbThreeCardCount);
	//�������
	void GetAllDoubleCard(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbDoubleCardData[], BYTE &cbDoubleCardCount);
	//������
	void GetAllSingleCard(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbSingleCardData[], BYTE &cbSingleCardCount);
	//���Ʋ���
	bool _TestOutAllCard(WORD wTestUser, BYTE cbWantOutCardData[], BYTE cbWantOutCardCount, BYTE	cbAllCardData[GAME_PLAYER][MAX_COUNT], BYTE cbUserCardCount[GAME_PLAYER], bool bFirstOutCard) ;
	//���Ʋ���
	bool TestOutAllCard(WORD wTestUser, BYTE cbWantOutCardData[], BYTE cbWantOutCardCount, bool bFirstOutCard) ;
	//�Ĵ�����
	bool AnalyseFourCardType( BYTE const cbHandCardData[MAX_COUNT], BYTE cbHandCardCount, BYTE cbEnemyCardData[MAX_COUNT], BYTE cbEnemyCardCount, tagOutCardResult &CardResult ) ;
	//����ж�
	bool IsLargestCard( WORD wTestUser, BYTE const cbWantOutCardData[], BYTE const cbWantOutCardCount );
	//�Ƿ��ܳ�
	bool VerifyOutCard( WORD wTestUser, BYTE const cbWantOutCardData[], BYTE const cbWantOutCardCount, BYTE const cbCurHandCardData[ MAX_COUNT ], BYTE const cbCurHandCardCount, bool bFirstOutCard ) ;

	//��Ҫ����
protected:
	//�������ͣ�����Ƶ��ã�
	void AnalyseOutCardType(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE const cbTurnCardData[], BYTE const cbTurnCardCount, tagOutCardTypeResult CardTypeResult[12+1]);
	//�������ƣ��ȳ��Ƶ��ã�
	void AnalyseOutCardType(BYTE const cbHandCardData[], BYTE const cbHandCardCount, tagOutCardTypeResult CardTypeResult[12+1]);
	//���Ƹ���
	BYTE AnalyseSinleCardCount(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE const cbWantOutCardData[], BYTE const cbWantOutCardCount, BYTE cbSingleCardData[]=NULL);

	//���ƺ���
public:
	//������ƣ��ȳ��ƣ�
	void BankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, tagOutCardResult & OutCardResult) ;
	//������ƣ�����ƣ�
	void BankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, WORD wOutCardUser, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult) ;
	//�����ϼң��ȳ��ƣ�
	void UpsideOfBankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, WORD wMeChairID,tagOutCardResult & OutCardResult) ;
	//�����ϼң�����ƣ�
	void UpsideOfBankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, WORD wOutCardUser,  const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult) ;
	//�����¼ң��ȳ��ƣ�
	void UndersideOfBankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, WORD wMeChairID,tagOutCardResult & OutCardResult) ;
	//�����¼ң�����ƣ�
	void UndersideOfBankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, WORD wOutCardUser, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult) ;
	//��������
	bool SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, WORD wOutCardUser, WORD wMeChairID, tagOutCardResult & OutCardResult);
	//��������
	bool SearchAutoPlayOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult);


	//�зֺ���
public:
	//�з��ж�
	BYTE AnalyseLandScore(WORD wMeChairID, BYTE cbCurrentLandScore);

	//�ڲ�����
private:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&MASK_COLOR; }
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�����˿�
	bool AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);

	//�˿����
private:
	BYTE							m_cbAllCardData[GAME_PLAYER][MAX_COUNT];//�����˿�
	BYTE							m_cbLandScoreCardData[MAX_COUNT];	//�����˿�
	BYTE							m_cbUserCardCount[GAME_PLAYER];		//�˿���Ŀ
	WORD							m_wBankerUser;						//�������
	LONG							m_lBankerOutCardCount ;				//���ƴ���
	tagStackHandCardInfo			m_StackHandCardInfo;				//ջ��
};


#endif
