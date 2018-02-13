#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��
#pragma pack(1)

#define KIND_ID						311									//��Ϸ I D mChen 310 311 312
//�������
#define GAME_PLAYER					20									//��Ϸ����

//mChen add, for HideSeek WangHu
#define MAX_AI_PER_CLIENT			10									//ÿ���ͻ��˵������Ϸ����==1+���AI��
#define INVALID_AI_ID				255

#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

#define GAME_NAME					TEXT("���Ķ�èè")					//��Ϸ���� mChen �����齫 �����齫-תת �����齫-�۶�
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD|GAME_GENRE_EDUCATE)	//��Ϸ���� mChen (GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)

//��Ϸ״̬
//#define GS_MJ_FREE					GAME_STATUS_FREE								//����״̬
//#define GS_MJ_PLAY					(GAME_STATUS_PLAY+1)							//��Ϸ״̬
#define GS_MJ_XIAOHU				(GAME_STATUS_PLAY+2)							//С��״̬

//��������

#define MAX_WEAVE					5									//������
#define MAX_INDEX					34									//�������
#define MAX_COUNT					17									//�����Ŀ
#define MAX_REPERTORY				108									//�����
#define MAX_REPERTORY_HZ			136									//�����齫�����

#define MAX_NIAO_CARD				6									//���������


#define MAX_RIGHT_COUNT				1									//���ȨλDWORD����	

#define GAME_TYPE_ZZ				0
#define GAME_TYPE_CS				1
#define GAME_TYPE_SD				2	//mChen
#define GAME_TYPE_13Shui			13

#define GAME_TYPE_ZZ_258			0		//ֻ��258����
#define GAME_TYPE_ZZ_ZIMOHU		    1		//ֻ����ģ��
#define GAME_TYPE_ZZ_QIDUI			2		//�ɺ��߶�
#define GAME_TYPE_ZZ_QIANGGANGHU	3		//�����ܺ�
#define GAME_TYPE_ZZ_ZHANIAO2		4		//����2��
#define GAME_TYPE_ZZ_ZHANIAO4		5		//����4��
#define GAME_TYPE_ZZ_ZHANIAO6		6		//����6��
#define GAME_TYPE_ZZ_HONGZHONG		7		//�����淨


#define ZZ_ZHANIAO0		0		//����2��
#define ZZ_ZHANIAO2		2		//����2��
#define ZZ_ZHANIAO4		4		//����4��
#define ZZ_ZHANIAO6		6		//����6��
#pragma pack()
#include "../��Ϸ������//GameLogic.h"
#include "../../../ȫ�ֶ���/Platform.h"
#include "../../../���������/�ں�����/TraceService.h"
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////

//�������
struct CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//��������
#define SUB_S_SEND_CARD				102									//�����˿�
#define SUB_S_OPERATE_NOTIFY		104									//������ʾ
#define SUB_S_OPERATE_RESULT		105									//��������
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_TRUSTEE				107									//�û��й�
#define SUB_S_CHI_HU				108									//
#define SUB_S_GANG_SCORE			110									//

#define SUB_S_CHAT_PLAY             1000                                //���������������ݸ��ͻ���

//mChen add, for HideSeek
#define SUB_S_HideSeek_HeartBeat			115						
#define SUB_S_HideSeek_AICreateInfo			116		

//��������
struct CMD_C_CHAT
{
	WORD ChairId;
	TCHAR UserStatus;
	TCHAR ChatData[100];
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//mChen add, for HideSeek
	BYTE							cbGameStatus;
	BYTE							cbMapIndex;
	//for�������ͬ��
	WORD							wRandseed;
	//��ͼ�����Ʒ����
	WORD							wRandseedForRandomGameObject;
	//����ͬ��
	WORD							wRandseedForInventory;
	InventoryItem					sInventoryList[MAX_INVENTORY_NUM];

	//LONGLONG						lCellScore;							//�������
	//WORD							wBankerUser;						//ׯ���û�
	//bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//mChen add, for HideSeek
	BYTE							cbGameStatus;
	BYTE							cbMapIndex;

	//for�������ͬ��
	WORD							wRandseed;
	//��ͼ�����Ʒ����
	WORD							wRandseedForRandomGameObject;
	//����ͬ��
	WORD							wRandseedForInventory;
	InventoryItem					sInventoryList[MAX_INVENTORY_NUM];

	////��Ϸ����
	//LONGLONG						lCellScore;									//��Ԫ����
	//WORD							wBankerUser;								//ׯ���û�
	//WORD							wCurrentUser;								//��ǰ�û�

	////״̬����
	//BYTE							cbActionCard;								//�����˿�
	//BYTE							cbActionMask;								//��������
	//BYTE							cbLeftCardCount;							//ʣ����Ŀ
	//bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�
	//WORD							wWinOrder[GAME_PLAYER];						//

	////������Ϣ
	//WORD							wOutCardUser;								//�����û�
	//BYTE							cbOutCardData;								//�����˿�
	//BYTE							cbDiscardCount[GAME_PLAYER];				//������Ŀ
	//BYTE							cbDiscardCard[GAME_PLAYER][60];				//������¼

	////�˿�����
	//BYTE							cbCardCount;								//�˿���Ŀ
	//BYTE							cbCardData[MAX_COUNT * GAME_PLAYER];		//�˿��б�
	//BYTE							cbSendCardData;								//�����˿�

	////����˿�
	//BYTE							cbWeaveCount[GAME_PLAYER];					//�����Ŀ
	//CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//����˿�

	////ZY add ������Ϣ
	//int								TotalScore_MJ[GAME_PLAYER];					//�齫�ܷ�
	//tagGangCardResult				tGangResult;								//������Ҹ�����Ϣ
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	LONG							lSiceCount;									//���ӵ���
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�
	BYTE							cbUserAction;								//�û�����
	BYTE							cbCardData[MAX_COUNT*GAME_PLAYER];			//�˿��б�
	BYTE							cbLeftCardCount;							//
	BYTE							cbXiaoHuTag;                           //С����� 0 ûС�� 1 ��С����

};

//��������
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
	bool							bIsPiao;
};

//�����˿�
struct CMD_S_SendCard
{
	BYTE							cbSendCardData;						//�˿�����
	BYTE							cbActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�
	bool							bTail;								//ĩβ����

																		//mChen add
	//BYTE							cbActionCard;						//�����˿�
	tagGangCardResult				tGangCard;							//�����ݣ������4���ܣ�
	bool							bKaiGangYaoShaiZi;					//�Ƿ񿪸�ҡ����
};


//������ʾ
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	BYTE							cbActionMask;						//��������
	BYTE							cbActionCard;						//�����˿�
};

//��������
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//BYTE							cbCardCount[GAME_PLAYER];			//
	//BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//
	////������Ϣ
	//WORD							wProvideUser[GAME_PLAYER];			//��Ӧ�û�
	//DWORD							dwChiHuRight[GAME_PLAYER];			//��������
	//DWORD							dwStartHuRight[GAME_PLAYER];			//���ֺ�������
	//LONGLONG						lStartHuScore[GAME_PLAYER];			//���ֺ��Ʒ���

	////������Ϣ
	//LONGLONG						lGameScore[GAME_PLAYER];			//��Ϸ����
	//LONGLONG						lTotalScore[GAME_PLAYER];			//��Ϸ�ܷ�
	//int								lGameTax[GAME_PLAYER];				//

	//WORD							wWinOrder[GAME_PLAYER];				//��������

	//LONGLONG						lGangScore[GAME_PLAYER];//��ϸ�÷�
	//BYTE							cbGenCount[GAME_PLAYER];			//
	//WORD							wLostFanShu[GAME_PLAYER][GAME_PLAYER];
	//WORD							wLeftUser;	//

	////����˿�
	//BYTE							cbWeaveCount[GAME_PLAYER];					//�����Ŀ
	//CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//����˿�


	//BYTE							cbCardDataNiao[MAX_NIAO_CARD];	// ����
	//BYTE							cbNiaoCount;	//���Ƹ���
	//BYTE							cbNiaoPick;	//�������

	//mChen add
	BYTE							cbEndReason;

	////mChen add��ʣ�����˿�
	//BYTE							cbLeftCardCount;
	//BYTE							cbRepertoryLeftCard[MAX_REPERTORY_HZ];
};

//�û��й�
struct CMD_S_Trustee
{
	bool							bTrustee;							//�Ƿ��й�
	WORD							wChairID;							//�й��û�
};

//������Ϣ
struct CMD_S_ChiHu
{
	WORD							wChiHuUser;							//
	WORD							wProviderUser;						//
	BYTE							cbChiHuCard;						//
	BYTE							cbCardCount;						//
	LONGLONG						lGameScore;							//
	BYTE							cbWinOrder;							//
};

//�ܷ���Ϣ
struct CMD_S_GangScore
{
	WORD							wChairId;							//
	BYTE							cbXiaYu;							//
	LONGLONG						lGangScore[GAME_PLAYER];			//
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			3									//�����˿�
#define SUB_C_TRUSTEE				4									//�û��й�
#define SUB_C_XIAOHU				5									//С��

//mChen add, for HideSeek
#define SUB_C_HIDESEEK_PLAYER_INFO			6									//�ͻ������λ�õ���Ϣ
#define SUB_C_HIDESEEK_PLAYERS_INFO			7

#define SUB_C_CHAT_PLAY                     1001                                //�ӿͻ��˽���������������

//mChen add, for HideSeek
#define MAX_PLAYER_HP						4
//�ͻ�������¼���Ϣ
enum PlayerEventKind
{
	Pick = 0,
	Boom,			//ը����ը

	DeadByDecHp,    //�Լ�����Ѫ����
	DeadByPicked,   //���������
	DeadByBoom,     //��ը��ը��

	GetInventory,   //ʰȡ����
	DecHp,
	AddHp,

	MaxEventNum
};
struct PlayerEventItem
{
	BYTE cbTeamType;
	WORD wChairId;
	BYTE cbAIId;

	BYTE cbEventKind;

	INT32 nCustomData0;
	INT32 nCustomData1;
	INT32 nCustomData2;

	void StreamValue(datastream& kData, bool bSend)
	{
		Stream_VALUE(cbTeamType);
		Stream_VALUE(wChairId);
		Stream_VALUE(cbAIId);

		Stream_VALUE(cbEventKind);

		Stream_VALUE(nCustomData0);
		Stream_VALUE(nCustomData1);
		Stream_VALUE(nCustomData2);
	}
};
//�ͻ������λ�õ���Ϣ
struct PlayerInfoItem
{
	BYTE cbTeamType;
	WORD wChairId;
	BYTE cbAIId;

	INT32 posX;
	INT32 posY;
	INT32 posZ;

	INT32 angleX;
	INT32 angleY;
	INT32 angleZ;

	///TCHAR objNamePicked[LEN_NICKNAME];

	//����������
	BYTE cbHP;	
	BYTE cbIsValid;

	////IsPicked:0x80, HasKilledPlayer:0x40, KilledPlayerChairID:0x3f
	//BYTE cbIsPickedAndKilled;

	////HasKilledPlayer:0x80, KilledPlayerIsAI:0x40, KilledPlayerTeamType:0x20, KilledPlayerChairID:0x1f, 
	//BYTE cbKilledPlayer;
	//BYTE cbKilledAIIdx;

	void StreamValue(datastream& kData, bool bSend)
	{
		Stream_VALUE(cbTeamType);
		Stream_VALUE(wChairId);
		Stream_VALUE(cbAIId);

		Stream_VALUE(posX);
		Stream_VALUE(posY);
		Stream_VALUE(posZ);

		Stream_VALUE(angleX);
		Stream_VALUE(angleY);
		Stream_VALUE(angleZ);

		Stream_VALUE(cbHP);
		Stream_VALUE(cbIsValid);
	}
};
/*
struct CMD_C_HideSeek_ClientPlayersInfo
{
	bool bIsValid;
	bool bIsLocked;

	WORD wAIItemCount;
	///WORD wEventItemCount;

	PlayerInfoItem HumanInfoItem;
	PlayerInfoItem AIInfoItems[GAME_PLAYER];
	///PlayerEventItem PlayerEventItems[GAME_PLAYER];
	///std::vector<PlayerInfoItem>	AIInfoItems;
	std::vector<PlayerEventItem> PlayerEventItems;

	void Reset()
	{
		bIsValid = false;
		bIsLocked = true;
		wAIItemCount = 0;
		ZeroMemory(&HumanInfoItem, sizeof(PlayerInfoItem));
		ZeroMemory(AIInfoItems, sizeof(AIInfoItems));
		PlayerEventItems.clear();

		//HP
		HumanInfoItem.cbHP = 4;
		for (int i = 0; i < GAME_PLAYER; i++)
		{
			AIInfoItems[i].cbHP = 4;
		}
	}

	void StreamValue(BYTE *pDataBuffer, WORD wDataSize)
	{
		bIsValid = true;

		wAIItemCount = *((WORD*)pDataBuffer);
		pDataBuffer += sizeof(wAIItemCount);

		WORD wEventItemCount = *((WORD*)pDataBuffer);
		pDataBuffer += sizeof(wEventItemCount);

		if (wAIItemCount > GAME_PLAYER || wEventItemCount > GAME_PLAYER)
		{
			////��ʾ��Ϣ
			//TCHAR szString[512] = TEXT("");
			//_sntprintf(szString, CountArray(szString), TEXT("CMD_C_HideSeek_ClientPlayersInfo StreamValue error: wAIItemCount=%d, wEventItemCount=%d ]"), wAIItemCount, wEventItemCount);
			//CTraceService::TraceString(szString, TraceLevel_Warning);

			wAIItemCount = 0;
			wEventItemCount = 0;

			return;
		}

		// Human Item
		BYTE cbHPOld = HumanInfoItem.cbHP;
		CopyMemory(&HumanInfoItem, pDataBuffer, sizeof(HumanInfoItem)-sizeof(HumanInfoItem.cbHP));
		pDataBuffer += sizeof(HumanInfoItem);
		//if (bIsLocked)
		//{
		//	//��һ�λ�ȡ���ͻ��˵�����
		//}
		//else
		//{
		//	//ֻ���ڵ�һ�ν��տͻ��˵�HP���ݣ��Ժ�ֻ�÷���˵�HP����

		//	//�ָ��ɷ����ԭ����HP����
		//	HumanInfoItem.cbHP = cbHPOld;
		//}

		// AI Items
		for (int i = 0; i < wAIItemCount; i++)
		{
			cbHPOld = AIInfoItems[i].cbHP;

			CopyMemory(&AIInfoItems[i], pDataBuffer, sizeof(PlayerInfoItem)-sizeof(AIInfoItems[i].cbHP));
			///AIInfoItems[i] = *((PlayerInfoItem*)pDataBuffer);
			pDataBuffer += sizeof(PlayerInfoItem);

			//if (!bIsLocked)
			//{
			//	AIInfoItems[i].cbH = cbHPOld;
			//}
		}

		bIsLocked = false;

		//for (int i = 0; i < wEventItemCount; i++)
		//{
		//	CopyMemory(&PlayerEventItems[i], pDataBuffer, sizeof(PlayerEventItem));
		//	///PlayerEventItems[i] = *((PlayerEventItem*)pDataBuffer);
		//	pDataBuffer += sizeof(PlayerEventItem);
		//}

		//AIInfoItems.clear();
		//for (int i = 0; i < wAIItemCount; i++)
		//{
		//	PlayerInfoItem pItem = *((PlayerInfoItem*)pDataBuffer);
		//	AIInfoItems.push_back(pItem);
		//	pDataBuffer += sizeof(PlayerInfoItem);
		//}

		// Event Items
		//PlayerEventItems.clear();
		for (int i = 0; i < wEventItemCount; i++)
		{
			PlayerEventItem pItem = *((PlayerEventItem*)pDataBuffer);
			PlayerEventItems.push_back(pItem);
			pDataBuffer += sizeof(PlayerEventItem);
		}
	}
};
*/
struct CMD_S_HideSeek_ClientPlayersInfo
{
	PlayerInfoItem HumanInfoItems[GAME_PLAYER];
	PlayerInfoItem AIInfoItems[GAME_PLAYER];
	std::vector<PlayerEventItem> PlayerEventItems;

	void Reset()
	{
		ZeroMemory(HumanInfoItems, sizeof(HumanInfoItems));
		ZeroMemory(AIInfoItems, sizeof(AIInfoItems));

		std::vector<PlayerEventItem> tmpEventItems;
		tmpEventItems.swap(PlayerEventItems);
		//PlayerEventItems.clear();

		//HP
		for (int i = 0; i < GAME_PLAYER; i++)
		{
			HumanInfoItems[i].cbHP = 4;
		}
		for (int i = 0; i < GAME_PLAYER; i++)
		{
			AIInfoItems[i].cbHP = 4;
		}
	}

	void StreamValue(BYTE *pDataBuffer, WORD wDataSize)
	{
		WORD wAIItemCount = *((WORD*)pDataBuffer);
		pDataBuffer += sizeof(wAIItemCount);

		WORD wEventItemCount = *((WORD*)pDataBuffer);
		pDataBuffer += sizeof(wEventItemCount);

		if (wAIItemCount > GAME_PLAYER || wEventItemCount > GAME_PLAYER)
		{
			//Log
			TCHAR szString[128] = TEXT("");
			_sntprintf(szString, CountArray(szString), TEXT("CMD_S_HideSeek_ClientPlayersInfo StreamValue error: wAIItemCount=%d, wEventItemCount=%d"), wAIItemCount, wEventItemCount);
			CTraceService::TraceString(szString, TraceLevel_Warning);

			wAIItemCount = 0;
			wEventItemCount = 0;

			return;
		}

		// Human Item
		PlayerInfoItem *pClientHumanInfoItem = (PlayerInfoItem *)pDataBuffer;
		WORD wHumanChairId = pClientHumanInfoItem->wChairId;
		if (wHumanChairId >= GAME_PLAYER)
		{
			//Log
			TCHAR szString[128] = TEXT("");
			_sntprintf(szString, CountArray(szString), TEXT("CMD_S_HideSeek_ClientPlayersInfo StreamValue error: wHumanChairId=%d"), wHumanChairId);
			CTraceService::TraceString(szString, TraceLevel_Warning);
		}
		PlayerInfoItem *pServerHumanInfoItem = &this->HumanInfoItems[wHumanChairId];
		CopyMemory(pServerHumanInfoItem, pDataBuffer, sizeof(PlayerInfoItem) - sizeof(pClientHumanInfoItem->cbHP) - sizeof(pClientHumanInfoItem->cbIsValid));//ȥ��cbHP��cbIsValid����
		pServerHumanInfoItem->cbIsValid = 1;
		pDataBuffer += sizeof(PlayerInfoItem);
		//if (bIsLocked)
		//{
		//	//��һ�λ�ȡ���ͻ��˵�����
		//}
		//else
		//{
		//	//ֻ���ڵ�һ�ν��տͻ��˵�HP���ݣ��Ժ�ֻ�÷���˵�HP����

		//	//�ָ��ɷ����ԭ����HP����
		//	HumanInfoItem.cbHP = cbHPOld;
		//}

		// AI Items
		PlayerInfoItem *pClientAIInfoItem = NULL;
		PlayerInfoItem *pServerAIInfoItem = NULL;
		BYTE cbAIId = 0;
		for (int i = 0; i < wAIItemCount; i++)
		{
			pClientAIInfoItem = (PlayerInfoItem *)pDataBuffer;
			cbAIId = pClientAIInfoItem->cbAIId;

			if (cbAIId >= GAME_PLAYER)
			{
				//Log
				TCHAR szString[128] = TEXT("");
				_sntprintf(szString, CountArray(szString), TEXT("CMD_S_HideSeek_ClientPlayersInfo StreamValue error: cbAIId=%d"), cbAIId);
				CTraceService::TraceString(szString, TraceLevel_Warning);
			}

			PlayerInfoItem *pServerAIInfoItem = &this->AIInfoItems[cbAIId];
			CopyMemory(pServerAIInfoItem, pDataBuffer, sizeof(PlayerInfoItem) - sizeof(pClientAIInfoItem->cbHP) - sizeof(pClientAIInfoItem->cbIsValid));//ȥ��cbHP��cbIsValid����
			///AIInfoItems[i] = *((PlayerInfoItem*)pDataBuffer);
			pServerAIInfoItem->cbIsValid = 1;
			pDataBuffer += sizeof(PlayerInfoItem);
		}

		//for (int i = 0; i < wEventItemCount; i++)
		//{
		//	CopyMemory(&PlayerEventItems[i], pDataBuffer, sizeof(PlayerEventItem));
		//	///PlayerEventItems[i] = *((PlayerEventItem*)pDataBuffer);
		//	pDataBuffer += sizeof(PlayerEventItem);
		//}

		//AIInfoItems.clear();
		//for (int i = 0; i < wAIItemCount; i++)
		//{
		//	PlayerInfoItem pItem = *((PlayerInfoItem*)pDataBuffer);
		//	AIInfoItems.push_back(pItem);
		//	pDataBuffer += sizeof(PlayerInfoItem);
		//}

		// Event Items
		for (int i = 0; i < wEventItemCount; i++)
		{
			PlayerEventItem pItem = *((PlayerEventItem*)pDataBuffer);
			this->PlayerEventItems.push_back(pItem);
			pDataBuffer += sizeof(PlayerEventItem);
		}
	}
};
struct CMD_S_HideSeek_HeartBeat
{
	//WORD wPlayerItemCount;
	//WORD wEventItemCount;

	///PlayerInfoItem PlayerInfoItems[GAME_PLAYER];

	std::vector<PlayerInfoItem>	PlayerInfoItems;
	std::vector<PlayerEventItem> PlayerEventItems;

	void StreamValue(datastream& kData, bool bSend)
	{
		//Stream_VALUE(wPlayerItemCount);
		//Stream_VALUE(wEventItemCount);
		StructVecotrMember(PlayerInfoItem, PlayerInfoItems);
		StructVecotrMember(PlayerEventItem, PlayerEventItems);
	}
};
//AI������Ϣ
struct AICreateInfoItem
{
	BYTE cbTeamType;
	WORD wChairId;
	BYTE cbModelIdx;

	BYTE cbAIId;
};
struct CMD_GF_S_AICreateInfoItems
{
	WORD wItemCount;

	AICreateInfoItem InfoItems[GAME_PLAYER];
};

//��������
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//�˿�����
};

//��������
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};

//�û��й�
struct CMD_C_Trustee
{
	bool							bTrustee;							//�Ƿ��й�	
};

//����С��
struct CMD_C_XiaoHu
{
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};


//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
