#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
typedef unsigned char BYTE;
typedef long LONG;
typedef unsigned short WORD;
//�����궨��
#define KIND_ID						200									//��Ϸ I D

#define GAME_PLAYER					3									//��Ϸ����
#define GAME_NAME					TEXT("������")						//��Ϸ����

//��Ϸ����
#define GAME_GENRE                  (GENRE_ENTER_SITDOWN|GENRE_FULLSIT|GENRE_READY)

//����ģʽ
#define PARAM_MODE_NORMAL           0x00                                //һ��ģʽ
#define PARAM_MODE_LAIZI            0x01                                //���ģʽ
#define PARAM_MODE_QIANG            0x02                                //�5���

//��Ϸ����
#define TABLE_PARAMETER_MODE        0                                   //��Ϸģʽ||0-����Ϸ/1-���/2-�5���
#define TABLE_PARAMETER_GOLD        1                                   //���/����

//��Ŀ����
#define MAX_COUNT					20									//�����Ŀ
#define FULL_COUNT					54									//ȫ����Ŀ
#define GOOD_CARD_COUTN				38									//������Ŀ
#define BACK_COUNT					3									//������Ŀ
#define NORMAL_COUNT				17									//������Ŀ

//��Ϸ״̬
#define GS_WK_FREE					GS_FREE								//�ȴ�ʼ
#define GS_WK_SCORE					GS_PLAYING							//�з�״̬
#define GS_WK_PLAYING				GS_PLAYING+1						//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_SEND_CARD				100									//��������
#define SUB_S_LAND_SCORE			101									//�з�����
#define SUB_S_LAND_QIANG			102									//�5���
#define SUB_S_GAME_START			103									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				104									//�û�����
#define SUB_S_PASS_CARD				105									//�������
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_TRUSTEE				107									//�û��й�
#define SUB_S_MOVIE_END             108                                 //����з�״̬
#define SUB_S_SEND_ALLCARD          110                                 //�����˿�

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lBaseScore;							//����
	BYTE                            cbTableMode;                        //��Ϸģʽ
};

//��Ϸ״̬
struct CMD_S_StatusScore
{
	BYTE							bLandScore;							//�������
	BYTE                            cbTableMode;                        //��Ϸģʽ
	BYTE							cbMulti;							//�5�����
	LONG							lBaseScore;							//����
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbScoreInfo[3];						//�з���Ϣ
	BYTE                            cbCardCount;                        //�˿���
	BYTE							cbCardData[MAX_COUNT];				//�����˿�
	bool							bUserTrustee[GAME_PLAYER];			//����й�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD							wLandUser;							//�������
	BYTE							cbMulti;							//�5�����
	BYTE							cbBombTime;							//ը������
	LONG							lBaseScore;							//����
	BYTE							cbLandScore;						//�������
	BYTE                            cbTableMode;                        //��Ϸģʽ
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbBackCard[BACK_COUNT];				//�����˿�
	BYTE							cbCardData[MAX_COUNT];	            //�����˿�
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbTurnCardCount;					//�����
	BYTE							cbTurnCardData[MAX_COUNT];			//�����б�
	bool							bUserTrustee[GAME_PLAYER];			//����й�
};

//�����˿�
struct CMD_S_SendCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardData[NORMAL_COUNT];			//�˿��б�
};

//�����˿˸������
//�����˿�
struct CMD_S_SendAllCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardData[GAME_PLAYER][20];		//�˿��б�
	BYTE							cbBackCardData[3];					//�����˿�
};

struct CMD_S_MovieEnd
{
	WORD							wCurrentUser;						//��ǰ�û�
};

//�û��з�
struct CMD_S_LandScore
{
	WORD							wLandUser;							//�з����
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbLandScore;						//�ϴνз�
	BYTE							cbCurrentScore;						//��ǰ�з�
	WORD                            wQiangUser;                         //�5����û�
};

struct CMD_S_LandQiang 
{
	WORD							wLandUser;							//�������
	WORD                            wMulti;                             //�ӱ�
	WORD                            wCurrentUser;						//��ǰ���
	BYTE                            cbQiangLand;                        //��һ���5����ʶ
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD				 			wLandUser;							//�������
	BYTE							cbLandScore;						//�������
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wMulti;								//����
	BYTE                            cbMagicCard;                        //�����
	BYTE							cbBackCard[3];						//�����˿�
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�
};

//�������
struct CMD_S_PassCard
{
	BYTE							cbNewTurn;							//һ�ֿ�ʼ
	WORD				 			wPassUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG							lGameTax;							//��Ϸ˰��
	LONG							lGameScore[GAME_PLAYER];			//��Ϸ���
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbCardData[FULL_COUNT];				//�˿��б�
	BYTE							cbMulti;							//�5�����
	BYTE							cbBombTime;							//ը������
	//BYTE                            cbEndReason[GAME_PLAYER];         //����ԭ��
};

//�û��й�
struct CMD_S_Trustee
{
	bool							bTrustee;							//�Ƿ��й�
	WORD							wChairID;							//�й��û�
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_LAND_SCORE			1									//�û��з�
#define SUB_C_LAND_QIANG            2                                   //�5���
#define SUB_C_OUT_CART				3									//�û�����
#define SUB_C_PASS_CARD				4									//�������
#define SUB_C_TRUSTEE				5									//�й���Ϣ
#define SUB_C_SNATCH                6                                   //�5���
#define SUB_C_MOVIE_END             7                                   //���ƽ���

//�йܽṹ
struct CMD_C_Trustee 
{
	bool							bTrustee;							//�йܱ�ʶ
};

//�û��з�
struct CMD_C_LandScore
{
	BYTE							cbLandScore;						//������� 255��ʾ����
};

//������ݰ�
struct CMD_C_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�
};

//////////////////////////////////////////////////////////////////////////


#define EXP_ADD_WIN			5					//Ӯһ����Ӿ���ֵ
#define EXP_ADD_LOST		1					//��һ����Ӿ���ֵ
#define EXP_ADD_DRAW		1					//��һ����Ӿ���ֵ
#define EXP_ADD_BASE		2					//���㹫ʽ�еĻ�ֵ

#endif
