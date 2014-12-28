#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "CMD_Land.h"
//////////////////////////////////////////////////////////////////////////
#include "assert.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//��������
#define ST_ORDER					0									//��С����
#define ST_COUNT					1									//��Ŀ����
#define CountArray(array) (sizeof(array) / sizeof(array[0]))
//////////////////////////////////////////////////////////////////////////
//��Ŀ����

//#define MAX_COUNT					20									//�����Ŀ
//#define FULL_COUNT					54									//ȫ����Ŀ
//#define GOOD_CARD_COUTN				38									//������Ŀ
//#define BACK_COUNT					3									//������Ŀ
//#define NORMAL_COUNT				17									//������Ŀ

//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define	MASK_COLOR					0xF0								//��ɫ����0xF0
#define	MASK_VALUE					0x0F								//��ֵ����
//#define MASK_LAIZI                  0x70                                //�������

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//�������� --������ ����� 5
#define CT_DOUBLE					2									//�������� --��ֵ��ͬ��}���� ��÷�� 4+ ���� 4
#define CT_THREE					3									//�������� --��ֵ��ͬ�������� ����� J
#define CT_SINGLE_LINE				4									//��l���� --���Ż����l���ƣ��磺 45678 �� 78910JQK ��������( 2 ���˫��
#define CT_DOUBLE_LINE				5									//��l���� --��Ի����l����ƣ��磺 334455 �� 7788991010JJ ��������( 2 ���˫��
#define CT_THREE_LINE				6									//��l���� --��������l�������ƣ��磺 333444 �� 555666777888 ��������( 2 ���˫��
#define CT_THREE_LINE_TAKE_ONE		7									//���һ�� --��ֵ��ͬ�������� + һ�ŵ��ƻ�һ���ơ����磺 333+6 �� 444+99
#define CT_THREE_LINE_TAKE_TWO		8									//���һ��
#define CT_FOUR_LINE_TAKE_ONE		9									//�Ĵ�}�� --�����ƣ�}���ơ�ע�⣺�Ĵ������ը��
#define CT_FOUR_LINE_TAKE_TWO		10									//�Ĵ�}��
//#define CT_BOMB_SOFT				11									//��ը     --�����Ϊ5�� 333��5�͹�����3333��ը����77��55�͹�����7777��ը��
#define CT_BOMB_CARD				11									//ը������ --����ͬ��ֵ��
//#define CT_BOMB_PURE				13									//�����ը�� --�����������ɵ�ը��
#define CT_MISSILE_CARD				12									//������� --˫������С���������

//////////////////////////////////////////////////////////////////////////

//����ṹ
struct tagAnalyseResult
{
	BYTE 							cbFourCount;						//������Ŀ
	BYTE 							cbThreeCount;						//������Ŀ
	BYTE 							cbDoubleCount;						//}����Ŀ
	BYTE							cbSignedCount;						//������Ŀ
	BYTE                            cbMagicCount;                       //�����Ŀ
	BYTE							cbFourCardData[MAX_COUNT];			//�����˿�
	BYTE							cbThreeCardData[MAX_COUNT];			//�����˿�
	BYTE							cbDoubleCardData[MAX_COUNT];		//}���˿�
	BYTE							cbSignedCardData[MAX_COUNT];		//�����˿�
};

//���ƽ��
struct tagOutCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbResultCard[MAX_COUNT];			//����˿�
};

#define MAX_TYPE_COUNT 254
struct tagOutCardTypeResult 
{
	BYTE							cbCardType;							//�˿�����
	BYTE							cbCardTypeCount;					//������Ŀ
	BYTE							cbEachHandCardCount[MAX_TYPE_COUNT];//ÿ�ָ���
	BYTE							cbCardData[MAX_TYPE_COUNT][MAX_COUNT];//�˿����
};
/*
const CString Card_Name_Array[FULL_COUNT] =
{
	TEXT("����A"),TEXT("����2"), TEXT("����3"), TEXT("����4"), TEXT("����5"), TEXT("����6"), TEXT("����7"), TEXT("����8"), TEXT("����9"), TEXT("����10"), TEXT("����J"), TEXT("����Q"), TEXT("����K"),
	TEXT("÷��A"),TEXT("÷��2"), TEXT("÷��3"), TEXT("÷��4"), TEXT("÷��5"), TEXT("÷��6"), TEXT("÷��7"), TEXT("÷��8"), TEXT("÷��9"), TEXT("÷��10"), TEXT("÷��J"), TEXT("÷��Q"), TEXT("÷��K"),
	TEXT("����A"),TEXT("����2"), TEXT("����3"), TEXT("����4"), TEXT("����5"), TEXT("����6"), TEXT("����7"), TEXT("����8"), TEXT("����9"), TEXT("����10"), TEXT("����J"), TEXT("����Q"), TEXT("����K"),
	TEXT("����A"),TEXT("����2"), TEXT("����3"), TEXT("����4"), TEXT("����5"), TEXT("����6"), TEXT("����7"), TEXT("����8"), TEXT("����9"), TEXT("����10"), TEXT("����J"), TEXT("����Q"), TEXT("����K"),
	TEXT("С��"),TEXT("����") 
};
*/

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//������
protected:
	static const BYTE				m_cbCardData[FULL_COUNT];			//�˿����
	//static const BYTE				m_cbGoodcardData[GOOD_CARD_COUTN];	//�������

	//������
public:
	//���캯��
	CGameLogic();
	//�����
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData & MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData & MASK_COLOR; }
	//�Ƿ����
	//bool IsLaiziCard(BYTE cbCardData) { return (cbCardData & MASK_LAIZI) == 1; }
	//�����
	//CString GetCardName(BYTE cbCardData);

	//���ƺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);
	//ɾ���˿�
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);
	//����˿�
	BYTE GetRandomCard(void);
	//����ɾ���˿�
	bool TryRemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, const BYTE cbCardData[], BYTE cbCardCount);

	//�߼�����
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�Ա��˿�
	bool CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount);
	//�Ƿ�ը��
	bool IsBombCard(BYTE cbCardType);
	//�˿�ת��
	BYTE SwitchToCardIndex(BYTE cbCardData);

	//AI����
	bool SearchAutoPlayOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult);

	//�ڲ�����
public:
	//�����˿�
	bool AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
	//�����˿�
	void AnalyseAutoPlayCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);

//////////////////////////////////////////////////////////////////////////
};


#endif
