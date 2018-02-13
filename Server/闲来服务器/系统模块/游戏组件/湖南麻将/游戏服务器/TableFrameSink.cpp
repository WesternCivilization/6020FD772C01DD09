#include "StdAfx.h"
#include "TableFrameSink.h"
#include "FvMask.h"
#include "..\..\..\�������\�������\WHDataLocker.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

#define IDI_TIMER_XIAO_HU			1 //С��

#define TIME_XIAO_HU  8

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wBankerUser = INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex, sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee, sizeof(m_bTrustee));
	ZeroMemory(m_lGameScore, sizeof(m_lGameScore));
	ZeroMemory(m_tGangResult, sizeof(m_tGangResult));
	//������Ϣ
	m_cbOutCardData = 0;
	m_cbOutCardCount = 0;
	m_wOutCardUser = INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard, sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount, sizeof(m_cbDiscardCount));

	//������Ϣ
	m_cbSendCardData = 0;
	m_cbSendCardCount = 0;
	m_cbLeftCardCount = 0;
	ZeroMemory(m_cbRepertoryCard, sizeof(m_cbRepertoryCard));
	ZeroMemory(m_cbRepertoryCard_HZ, sizeof(m_cbRepertoryCard_HZ));

	//���б���
	m_cbProvideCard = 0;
	m_wResumeUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	m_wProvideUser = INVALID_CHAIR;

	//״̬����
	m_bSendStatus = false;
	m_bGangStatus = false;
	m_bGangOutStatus = false;


	//�û�״̬
	ZeroMemory(m_bResponse, sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction, sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard, sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction, sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray, sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount, sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard = 0;
	ZeroMemory(m_dwChiHuKind, sizeof(m_dwChiHuKind));
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		m_ChiHuRight[i].SetEmpty();
	}
	memset(m_wProvider, INVALID_CHAIR, sizeof(m_wProvider));

	//�������
	m_pITableFrame = NULL;
	m_pGameServiceOption = NULL;

	//ZY add
	ZeroMemory(TotalScore_MJ, sizeof(TotalScore_MJ));
	m_bPlayCoutIdex = 0;
	playercount = 0;
	nowcount = 0;

	//��������
	ZeroMemory(m_bCanPiaoStatus, sizeof(m_bCanPiaoStatus));
	ZeroMemory(m_bPiaoStatus, sizeof(m_bPiaoStatus));
	ZeroMemory(m_cbPiaoCount, sizeof(m_cbPiaoCount));
	ZeroMemory(m_bGangPiao, sizeof(m_bGangPiao));
	ZeroMemory(m_bPiaoGang, sizeof(m_bPiaoGang));

	m_bOperateStatus = false;
	m_wPiaoChengBaoUser = INVALID_CHAIR;
	ZeroMemory(m_lPiaoScore, sizeof(m_lPiaoScore));
	ZeroMemory(m_lBasePiaoScore, sizeof(m_lBasePiaoScore));

	m_bBuGang = false;

	ZeroMemory(m_cbChiPengCount, sizeof(m_cbChiPengCount));
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		for (BYTE j = 0; j < MAX_WEAVE; j++)
			m_wChiPengUser[i][j] = INVALID_CHAIR;
	}
	ZeroMemory(m_bQingYiSeChengBao, sizeof(m_bQingYiSeChengBao));

	m_cbChiCard = 0;

	ZeroMemory(m_bLouPengStatus, sizeof(m_bLouPengStatus));
	ZeroMemory(m_cbLouPengCard, sizeof(m_cbLouPengCard));

	m_wChiHuUser = INVALID_CHAIR;

	ZeroMemory(m_cbGangCount, sizeof(m_cbGangCount));


	//mChen add, test
	m_cbGameTypeIdex = GAME_TYPE_ZZ;
	FvMask::Add(m_dwGameRuleIdex, _MASK_((DWORD)GAME_TYPE_ZZ_HONGZHONG));
	m_lBaseScore = 1;

	//mChen add, for HideSeek
	//for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	//{
	//	m_sClientsPlayersInfo[wChairID].Reset();
	//}
	m_sClientsPlayersInfos.Reset();
	//ZeroMemory(m_bIsHumanDead, sizeof(m_bIsHumanDead));
	//ZeroMemory(m_bIsAIDead, sizeof(m_bIsAIDead));
	ZeroMemory(m_cbAINum, sizeof(m_cbAINum));
	ZeroMemory(m_cbAICreateInfoItems, sizeof(m_cbAICreateInfoItems));
	ResetInventoryList();

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
void *CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink, Guid, dwQueryVer);
	QUERYINTERFACE(ITableUserAction, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink, Guid, dwQueryVer);
	return NULL;
}

//��ʼ��
bool CTableFrameSink::Initialization(IUnknownEx *pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx != NULL);
	m_pITableFrame = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITableFrame);
	if (m_pITableFrame == NULL)
		return false;

	//��ȡ����
	m_pGameServiceOption = m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption != NULL);

	//mChen edit, for HideSeek:: �����޵�ʱ��֮ǰ�������ң���������Ӫ��ֻ������֮�����Ĳ����Թ���:fix��ͬ��bug
	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL); 
	//m_pITableFrame->SetStartMode(START_MODE_FULL_READY); 

	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	ZeroMemory(m_cbCardIndex, sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee, sizeof(m_bTrustee));

	ZeroMemory(m_lGameScore, sizeof(m_lGameScore));
	ZeroMemory(m_tGangResult, sizeof(m_tGangResult));
	//ZY add
	//ZeroMemory(TotalScore_MJ, sizeof(TotalScore_MJ));

	//������Ϣ
	m_cbOutCardData = 0;
	m_cbOutCardCount = 0;
	m_wOutCardUser = INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard, sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount, sizeof(m_cbDiscardCount));

	//������Ϣ
	m_cbSendCardData = 0;
	m_cbSendCardCount = 0;
	m_cbLeftCardCount = 0;
	ZeroMemory(m_cbRepertoryCard, sizeof(m_cbRepertoryCard));
	ZeroMemory(m_cbRepertoryCard_HZ, sizeof(m_cbRepertoryCard_HZ));

	//���б���
	m_cbProvideCard = 0;
	m_wResumeUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	m_wProvideUser = INVALID_CHAIR;

	//״̬����
	m_bSendStatus = false;
	m_bGangStatus = false;
	m_bGangOutStatus = false;


	//�û�״̬
	ZeroMemory(m_bResponse, sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction, sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard, sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction, sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray, sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount, sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard = 0;
	ZeroMemory(m_dwChiHuKind, sizeof(m_dwChiHuKind));
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		m_ChiHuRight[i].SetEmpty();
	}
	memset(m_wProvider, INVALID_CHAIR, sizeof(m_wProvider));



	//��������
	ZeroMemory(m_bCanPiaoStatus, sizeof(m_bCanPiaoStatus));
	ZeroMemory(m_bPiaoStatus, sizeof(m_bPiaoStatus));
	ZeroMemory(m_cbPiaoCount, sizeof(m_cbPiaoCount));
	ZeroMemory(m_bGangPiao, sizeof(m_bGangPiao));
	ZeroMemory(m_bPiaoGang, sizeof(m_bPiaoGang));

	m_bOperateStatus = false;
	m_wPiaoChengBaoUser = INVALID_CHAIR;
	ZeroMemory(m_lPiaoScore, sizeof(m_lPiaoScore));
	ZeroMemory(m_lBasePiaoScore, sizeof(m_lBasePiaoScore));

	m_bBuGang = false;

	ZeroMemory(m_cbChiPengCount, sizeof(m_cbChiPengCount));
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		for (BYTE j = 0; j < MAX_WEAVE; j++)
			m_wChiPengUser[i][j] = INVALID_CHAIR;
	}
	ZeroMemory(m_bQingYiSeChengBao, sizeof(m_bQingYiSeChengBao));

	m_cbChiCard = 0;

	ZeroMemory(m_bLouPengStatus, sizeof(m_bLouPengStatus));
	ZeroMemory(m_cbLouPengCard, sizeof(m_cbLouPengCard));

	m_wChiHuUser = INVALID_CHAIR;

	ZeroMemory(m_cbGangCount, sizeof(m_cbGangCount));

	//mChen add, for HideSeek
	//for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	//{
	//	m_sClientsPlayersInfo[wChairID].Reset();
	//}
	m_sClientsPlayersInfos.Reset();
	//ZeroMemory(m_bIsHumanDead, sizeof(m_bIsHumanDead));
	//ZeroMemory(m_bIsAIDead, sizeof(m_bIsAIDead));
	ZeroMemory(m_cbAINum, sizeof(m_cbAINum));
	ZeroMemory(m_cbAICreateInfoItems, sizeof(m_cbAICreateInfoItems));
	ZeroMemory(m_sStealth, sizeof(m_sStealth));

	return;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	return true;

	CTraceService::TraceString(TEXT("==========OnEventGameStart========="), TraceLevel_Normal);
	if (hasRule(GAME_TYPE_ZZ_HONGZHONG))
	{
		Shuffle(m_cbRepertoryCard_HZ, MAX_REPERTORY_HZ);
	}
	else
	{
		Shuffle(m_cbRepertoryCard, MAX_REPERTORY);
	}

	if (m_cbGameTypeIdex == GAME_TYPE_ZZ)
	{
		GameStart_ZZ();
	}
	//m_wBankerUser = 0;

	ZeroMemory(m_sStealth, sizeof(m_sStealth));
	return true;
}

void CTableFrameSink::Shuffle(BYTE *pRepertoryCard, int nCardCount)
{
	m_cbLeftCardCount = nCardCount;
	m_GameLogic.RandCardData(pRepertoryCard, nCardCount);


	//test
	/*BYTE byTest[] = {
	0x01,0x05,0x06,
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
	0x01,0x02,0x03,0x04,0x05,0x08,


	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x19,
	0x11,0x13,0x14,0x15,0x16,0x17,0x18,0x19,
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x19,
	0x11,0x12,0x15,0x16,0x17,0x18,0x19,0x34,
	0x18,0x34,0x33,0x33,
	
	0x23,0x23,0x23,0x32,
	0x21,0x24,0x05,0x18,

	0x12,0x31,0x14,0x25,0x22,0x24,0x26,0x07,0x08,0x09,0x28,



	0x01,0x21,0x22,0x26,0x27,0x28,0x29,0x29,0x24,0x25,0x26,0x27,0x29,0x21,0x25,0x37,

	0x27,0x31,0x32,0x13,0x33,0x25,0x35,0x36,0x21,0x22,0x23,0x24,0x26,0x27,0x33,0x32,

	0x07,0x06,0x36,0x31,0x32,0x34,0x35,0x36,0x31,0x34,0x35,0x36,0x35,0x37,0x22,0x29,

	0x01,0x01,0x01,0x02,0x03,0x04,0x37,0x37,0x06,0x07,0x08,0x09,0x09,0x09,0x28,0x28,
	};
	CopyMemory(pRepertoryCard, byTest, sizeof(byTest));*/
	//�ַ��˿�
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if (m_pITableFrame->GetTableUserItem(i) != NULL)
		{
			m_cbLeftCardCount -= (MAX_COUNT - 1);
			m_GameLogic.SwitchToCardIndex(&pRepertoryCard[m_cbLeftCardCount], MAX_COUNT - 1, m_cbCardIndex[i]);
		}
	}
}

void CTableFrameSink::GameStart_ZZ()
{
	CTraceService::TraceString(TEXT("GameStart_ZZ"), TraceLevel_Normal);
	//mChen edit
	LONG lSiceCount = 0;
	if (m_wBankerUser == INVALID_CHAIR)
	{
		m_wBankerUser = 0;
	}
	else
	{
		////�����˿�
		//LONG lSiceCount = MAKELONG(MAKEWORD(rand() % 6 + 1, rand() % 6 + 1), MAKEWORD(rand() % 6 + 1, rand() % 6 + 1));
		//m_wBankerUser = ((BYTE)(lSiceCount >> 24) + (BYTE)(lSiceCount >> 16) - 1) % GAME_PLAYER;
	}

	//���ñ���
	m_cbProvideCard = 0;
	m_wProvideUser = INVALID_CHAIR;
	m_wCurrentUser = m_wBankerUser;

	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart, sizeof(GameStart));
	GameStart.lSiceCount = lSiceCount;
	GameStart.wBankerUser = m_wBankerUser;
	GameStart.wCurrentUser = m_wCurrentUser;
	GameStart.cbLeftCardCount = m_cbLeftCardCount;

	m_pITableFrame->SetGameStatus(GAME_STATUS_PLAY);
	GameStart.cbXiaoHuTag = 0;
	ZeroMemory(GameStart.cbCardData, sizeof(GameStart.cbCardData));
	//��������
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		//���ñ���
		GameStart.cbUserAction = WIK_NULL;//m_cbUserAction[i];		
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i], &GameStart.cbCardData[i*MAX_COUNT]);

		if (m_pITableFrame->GetTableUserItem(i)->IsAndroidUser())
		{
			BYTE bIndex = 1;
			for (WORD j = 0; j < GAME_PLAYER; j++)
			{
				if (j == i) continue;
				m_GameLogic.SwitchToCardData(m_cbCardIndex[j], &GameStart.cbCardData[MAX_COUNT * bIndex++]);
			}
		}		
	}
	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
	//m_pITableFrame->SendLookonData(i, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
	m_bSendStatus = true;
	DispatchCardData(m_wCurrentUser);
}


//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem *pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
	{
		//��������
		CMD_S_GameEnd GameEnd;
		ZeroMemory(&GameEnd, sizeof(GameEnd));
		//GameEnd.wLeftUser = INVALID_CHAIR;

		//mChen add
		GameEnd.cbEndReason = cbReason;

		tagScoreInfo ScoreInfoArray[GAME_PLAYER];
		ZeroMemory(&ScoreInfoArray, sizeof(ScoreInfoArray));

		int	lGameTaxs[GAME_PLAYER];				//
		ZeroMemory(&lGameTaxs, sizeof(lGameTaxs));
		//ͳ�ƻ���
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			if (NULL == m_pITableFrame->GetTableUserItem(i)) continue;

			//���û���
			if (m_lGameScore[i] > 0L)
			{
				lGameTaxs[i] = m_pITableFrame->CalculateRevenue(i, m_lGameScore[i]);
				m_lGameScore[i] -= lGameTaxs[i];
			}

			BYTE ScoreKind;
			if (m_lGameScore[i] > 0L) ScoreKind = SCORE_TYPE_WIN;
			else if (m_lGameScore[i] < 0L) ScoreKind = SCORE_TYPE_LOSE;
			else ScoreKind = SCORE_TYPE_DRAW;

			ScoreInfoArray[i].lScore = m_lGameScore[i];
			ScoreInfoArray[i].lRevenue = lGameTaxs[i];
			ScoreInfoArray[i].cbType = ScoreKind;

			//ZY add
			TotalScore_MJ[i] += m_lGameScore[i];
		//	GameEnd.lTotalScore[i] = TotalScore_MJ[i];

		}

		//д�����
		m_pITableFrame->WriteTableScore(ScoreInfoArray, CountArray(ScoreInfoArray));

		//���ͽ�����Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

		//�������
		ZeroMemory(m_bCanPiaoStatus, sizeof(m_bCanPiaoStatus));
		ZeroMemory(m_bPiaoStatus, sizeof(m_bPiaoStatus));
		ZeroMemory(m_cbPiaoCount, sizeof(m_cbPiaoCount));
		ZeroMemory(m_bGangPiao, sizeof(m_bGangPiao));
		ZeroMemory(m_bPiaoGang, sizeof(m_bPiaoGang));

		m_bOperateStatus = false;
		m_wPiaoChengBaoUser = INVALID_CHAIR;
		ZeroMemory(m_lPiaoScore, sizeof(m_lPiaoScore));
		ZeroMemory(m_lBasePiaoScore, sizeof(m_lBasePiaoScore));

		m_bBuGang = false;

		ZeroMemory(m_cbChiPengCount, sizeof(m_cbChiPengCount));
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			for (BYTE j = 0; j < MAX_WEAVE; j++)
				m_wChiPengUser[i][j] = INVALID_CHAIR;
		}
		ZeroMemory(m_bQingYiSeChengBao, sizeof(m_bQingYiSeChengBao));

		m_cbChiCard = 0;

		ZeroMemory(m_bLouPengStatus, sizeof(m_bLouPengStatus));
		ZeroMemory(m_cbLouPengCard, sizeof(m_cbLouPengCard));

		m_wChiHuUser = INVALID_CHAIR;

		ZeroMemory(m_cbGangCount, sizeof(m_cbGangCount));

		//ZY add
		nowcount++;
		if (nowcount == playercount) {
			nowcount = 0;
			playercount = 0;
			ZeroMemory(TotalScore_MJ, sizeof(TotalScore_MJ));
		}
		//������Ϸ
		m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,cbReason);

		return true;
	}
	case GER_DISMISS:		//��Ϸ��ɢ
	{
		//��������
		CMD_S_GameEnd GameEnd;
		ZeroMemory(&GameEnd, sizeof(GameEnd));
		//GameEnd.wLeftUser = INVALID_CHAIR;

		//mChen add
		GameEnd.cbEndReason = cbReason;
		//ZY add
		ZeroMemory(TotalScore_MJ, sizeof(TotalScore_MJ));
		nowcount = 0;
		playercount = 0;

		////���ñ���
		//memset(GameEnd.wProvideUser, INVALID_CHAIR, sizeof(GameEnd.wProvideUser));

		////�����˿�
		//for (WORD i = 0; i < GAME_PLAYER; i++)
		//{
		//	GameEnd.cbCardCount[i] = m_GameLogic.SwitchToCardData(m_cbCardIndex[i], GameEnd.cbCardData[i]);
		//}

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

		//������Ϸ
		m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,cbReason);

		return true;
	}
	case GER_NETWORK_ERROR:		//�������
	case GER_USER_LEAVE:		//�û�ǿ��
	{
		m_bTrustee[wChairID] = true;

		return true;
	}
	}

	//�������
	ASSERT(FALSE);
	return false;
}
void CTableFrameSink::ResetPlayerTotalScore() {
	CTraceService::TraceString(TEXT("ResetPlayerTotalScore"), TraceLevel_Normal);
	ZeroMemory(TotalScore_MJ, sizeof(TotalScore_MJ));
	nowcount = 0;
	playercount = 0;
}
//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem *pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:	//����״̬
	case GAME_STATUS_WAIT:
	{
		//��������
		CMD_S_StatusFree StatusFree;
		memset(&StatusFree, 0, sizeof(StatusFree));
		StatusFree.cbGameStatus = cbGameStatus;
		StatusFree.cbMapIndex = m_pITableFrame->GetMapIndexRand();
		StatusFree.wRandseed = m_pITableFrame->GetRandSeed();
		StatusFree.wRandseedForRandomGameObject = m_pITableFrame->GetRandSeedForRandomGameObjec();
		StatusFree.wRandseedForInventory = m_pITableFrame->GetRandSeedForInventory();
		//����ͬ��
		CopyMemory(StatusFree.sInventoryList, m_sInventoryList, sizeof(m_sInventoryList));

		//��������
		//StatusFree.wBankerUser = m_wBankerUser;
		//StatusFree.lCellScore = m_lBaseScore;//m_pGameServiceOption->lCellScore
		//CopyMemory(StatusFree.bTrustee, m_bTrustee, sizeof(m_bTrustee));

		//���ͳ���
		return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusFree, sizeof(StatusFree));
	}
	case GAME_STATUS_HIDE:
	case GAME_STATUS_PLAY:	//��Ϸ״̬
	{
		//�Թ��߻�������

		//��������
		CMD_S_StatusPlay StatusPlay;
		memset(&StatusPlay, 0, sizeof(StatusPlay));
		StatusPlay.cbGameStatus = cbGameStatus;
		StatusPlay.cbMapIndex = m_pITableFrame->GetMapIndexRand();
		StatusPlay.wRandseed = m_pITableFrame->GetRandSeed();
		StatusPlay.wRandseedForRandomGameObject = m_pITableFrame->GetRandSeedForRandomGameObjec();
		StatusPlay.wRandseedForInventory = m_pITableFrame->GetRandSeedForInventory();
		//����ͬ��
		CopyMemory(StatusPlay.sInventoryList, m_sInventoryList, sizeof(m_sInventoryList));

		////Log:��ʾ��Ϣ
		//TCHAR szString[512] = TEXT("");
		//_sntprintf(szString, CountArray(szString), TEXT("OnEventSendGameScene��δʹ�õ����б�"));
		//CTraceService::TraceString(szString, TraceLevel_Normal);
		//for (int i = 0; i < MAX_INVENTORY_NUM; i++)
		//{
		//	if (m_sInventoryList[i].cbUsed == 0)
		//	{
		//		_sntprintf(szString, CountArray(szString), TEXT("���ߣ�i=%d"), i);
		//		CTraceService::TraceString(szString, TraceLevel_Normal);
		//	}
		//}

		////��Ϸ����
		//StatusPlay.wBankerUser = m_wBankerUser;
		//StatusPlay.wCurrentUser = m_wCurrentUser;
		//StatusPlay.lCellScore = m_lBaseScore;//m_pGameServiceOption->lCellScore
		//CopyMemory(StatusPlay.bTrustee, m_bTrustee, sizeof(m_bTrustee));

		////״̬����
		//StatusPlay.cbActionCard = m_cbProvideCard;
		//StatusPlay.cbLeftCardCount = m_cbLeftCardCount;
		//StatusPlay.cbActionMask = (m_bResponse[wChiarID] == false) ? m_cbUserAction[wChiarID] : WIK_NULL;

		////��ʷ��¼
		//StatusPlay.wOutCardUser = m_wOutCardUser;
		//StatusPlay.cbOutCardData = m_cbOutCardData;
		//for (int i = 0; i < GAME_PLAYER; i++)
		//{
		//	CopyMemory(StatusPlay.cbDiscardCard[i], m_cbDiscardCard[i], sizeof(BYTE) * 60);
		//}
		//CopyMemory(StatusPlay.cbDiscardCount, m_cbDiscardCount, sizeof(StatusPlay.cbDiscardCount));

		////����˿�
		//CopyMemory(StatusPlay.WeaveItemArray, m_WeaveItemArray, sizeof(m_WeaveItemArray));
		//CopyMemory(StatusPlay.cbWeaveCount, m_cbWeaveItemCount, sizeof(m_cbWeaveItemCount));
		//
		////�˿�����
		//for(int i = 0; i<GAME_PLAYER;i++)
		//{
		//	if (i == wChiarID)
		//		StatusPlay.cbCardCount = m_GameLogic.SwitchToCardData(m_cbCardIndex[i], &StatusPlay.cbCardData[i * MAX_COUNT]);
		//	else
		//		m_GameLogic.SwitchToCardData(m_cbCardIndex[i], &StatusPlay.cbCardData[i*MAX_COUNT]);
		//}
		////StatusPlay.cbCardCount = m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID], &StatusPlay.cbCardData[wChiarID]);
		//StatusPlay.cbSendCardData = ((m_cbSendCardData != 0) && (m_wProvideUser == wChiarID)) ? m_cbSendCardData : 0x00;
		////ZY add
		//for (int i = 0; i < GAME_PLAYER; i++) {
		//	StatusPlay.TotalScore_MJ[i] = TotalScore_MJ[i];
		//}
		//
		//CopyMemory(&StatusPlay.tGangResult, &m_tGangResult[wChiarID], sizeof(StatusPlay.tGangResult));
	
		//���ͳ���
		bool bResult = m_pITableFrame->SendGameScene(pIServerUserItem, &StatusPlay, sizeof(StatusPlay));

		//mChen add, for HideSeek
		BYTE cbUserStatus = US_NULL;
		if (pIServerUserItem != NULL)
		{
			cbUserStatus = pIServerUserItem->GetUserStatus();
		}
		if (cbUserStatus == US_LOOKON) 
		{
			if (m_cbAINum[PlayerTeamType::TaggerTeam] + m_cbAINum[PlayerTeamType::HideTeam] > 0)
			{
				//GenerateAICreateInfo�Ѿ����ù�

				HideSeek_SendAICreateInfo(pIServerUserItem);
			}

			//��������ͬ��
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_GF_INVENTORY_CREATE, NULL, 0, MDM_GF_FRAME);
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_GF_INVENTORY_CREATE, NULL, 0, MDM_GF_FRAME);
		}
		else
		{
			//fix�����WAIT���ߣ����IDI_HIDESEEK_END_WAIT_GAMEʱ���͵�HideSeek_SendAICreateInfo��Ϣ������û��AI

			if (m_cbAINum[PlayerTeamType::TaggerTeam] + m_cbAINum[PlayerTeamType::HideTeam] > 0)
			{
				//GenerateAICreateInfo�Ѿ����ù�

				HideSeek_SendAICreateInfo(pIServerUserItem);
			}
		}

		return bResult;
	}
	}

	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
		////mChen add, for HideSeek
		//case IDI_TIMER_TABLE_SINK_HIDESEEK_USERS_TICK:
		//{
		//	HideSeek_SendHeartBeat();
		//	return true;
		//}

		case IDI_TIMER_XIAO_HU:  //���ƽ���
		{
			ZeroMemory(m_cbUserAction, sizeof(m_cbUserAction));
			m_pITableFrame->SetGameStatus(GAME_STATUS_PLAY);

			m_bSendStatus = true;
			DispatchCardData(m_wCurrentUser);
			return true;
		}
	}
	return false;
}

//��Ϸ��Ϣ����
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID *pDataBuffer, WORD wDataSize, IServerUserItem *pIServerUserItem/*, IMainServiceFrame * pIMainServiceFrame*/)
{
	switch (wSubCmdID)
	{
		case SUB_C_CHAT_PLAY:
		{
			//��֤��Ϣ
			ASSERT(wDataSize == sizeof(CMD_C_CHAT));
			if (wDataSize != sizeof(CMD_C_CHAT)) return false;
			CMD_C_CHAT *pData = (CMD_C_CHAT*)pDataBuffer;
			pData->ChairId = pIServerUserItem->GetChairID();

			//CMD_C_CHAT Chat;
			//ZeroMemory(&Chat, sizeof(Chat));
			//pIMainServiceFrame->SensitiveWordFilter(pData->ChatData, Chat.ChatData, CountArray(Chat.ChatData));
			//Chat.ChairId = pIServerUserItem->GetChairID();
			//Chat.UserStatus = Data->UserStatus;


			TCHAR szString1[512] = TEXT("");
			_sntprintf(szString1, CountArray(szString1), TEXT("��������ID=% d"), pData->ChairId);
			CTraceService::TraceString(szString1, TraceLevel_Normal);
			//ת�����������
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CHAT_PLAY, pData, sizeof(CMD_C_CHAT));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CHAT_PLAY, pData, sizeof(CMD_C_CHAT));
			return true;
		}
	//mChen add, for HideSeek
	case SUB_C_HIDESEEK_PLAYERS_INFO:
	{
		return HideSeek_OnClientPlayersInfo(pDataBuffer, wDataSize, pIServerUserItem);
	}

	case SUB_C_OUT_CARD:		//������Ϣ
	{
		//Ч����Ϣ
		ASSERT(wDataSize == sizeof(CMD_C_OutCard));
		if (wDataSize != sizeof(CMD_C_OutCard)) return false;

		//�û�Ч��
		if (pIServerUserItem->GetUserStatus() != US_PLAYING) return true;

		//��Ϣ����
		CMD_C_OutCard *pOutCard = (CMD_C_OutCard *)pDataBuffer;
		return OnUserOutCard(pIServerUserItem->GetChairID(), pOutCard->cbCardData);
	}
	case SUB_C_OPERATE_CARD:	//������Ϣ
	{
		//Ч����Ϣ
		ASSERT(wDataSize == sizeof(CMD_C_OperateCard));
		if (wDataSize != sizeof(CMD_C_OperateCard)) return false;

		//�û�Ч��
		if (pIServerUserItem->GetUserStatus() != US_PLAYING) return true;

		//��Ϣ����
		CMD_C_OperateCard *pOperateCard = (CMD_C_OperateCard *)pDataBuffer;
		return OnUserOperateCard(pIServerUserItem->GetChairID(), pOperateCard->cbOperateCode, pOperateCard->cbOperateCard);
	}
	case SUB_C_TRUSTEE:
	{
		CMD_C_Trustee *pTrustee = (CMD_C_Trustee *)pDataBuffer;
		if (wDataSize != sizeof(CMD_C_Trustee)) return false;


		m_bTrustee[pIServerUserItem->GetChairID()] = pTrustee->bTrustee;
		CMD_S_Trustee Trustee;
		ZeroMemory(&Trustee, sizeof(Trustee));
		Trustee.bTrustee = pTrustee->bTrustee;
		Trustee.wChairID = pIServerUserItem->GetChairID();
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_TRUSTEE, &Trustee, sizeof(Trustee));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_TRUSTEE, &Trustee, sizeof(Trustee));

		return true;
	}

	}

	return false;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID *pDataBuffer, WORD wDataSize, IServerUserItem *pIServerUserItem)
{
	return false;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem *pIServerUserItem, bool bLookonUser)
{
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem *pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if (bLookonUser == false)
	{
		m_bTrustee[wChairID] = false;
		CMD_S_Trustee Trustee;
		ZeroMemory(&Trustee, sizeof(Trustee));
		Trustee.bTrustee = false;
		Trustee.wChairID = wChairID;
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_TRUSTEE, &Trustee, sizeof(Trustee));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_TRUSTEE, &Trustee, sizeof(Trustee));
	}

	return true;
}

//������Ϸ����
void CTableFrameSink::SetPrivateInfo(BYTE bGameTypeIdex, DWORD	bGameRuleIdex, SCORE lBaseScore, BYTE PlayCout, BYTE PlayerCount)
{
	ResetPlayerTotalScore();
	m_cbGameTypeIdex = bGameTypeIdex;
	m_dwGameRuleIdex = bGameRuleIdex;
	m_lBaseScore = lBaseScore;
	m_bPlayCoutIdex = PlayCout;
	/*TCHAR szString[512] = TEXT("");
	_sntprintf(szString, CountArray(szString), TEXT("�ܾ���=%d"), PlayCout);
	CTraceService::TraceString(szString, TraceLevel_Normal);*/
	playercount = PlayCout;
}

//mChen add, for HideSeek
InventoryItem* CTableFrameSink::GetInventoryList()
{
	return m_sInventoryList;
}
void CTableFrameSink::ResetInventoryList()
{
	for (int i = 0; i < MAX_INVENTORY_NUM; i++)
	{
		m_sInventoryList[i].cbId = i;
		m_sInventoryList[i].cbType = (InventoryType)(rand() % (int)InventoryType::Inventory_Type_Num);
		m_sInventoryList[i].cbUsed = 0;
	}
}
void CTableFrameSink::SetResurrection(WORD wChairID)
{
	if (wChairID < GAME_PLAYER)
	{
		m_sClientsPlayersInfos.HumanInfoItems[wChairID].cbHP = 1;
	}
}

void CTableFrameSink::SetStealth(DWORD dwTime, WORD wChairID)
{
	if (wChairID < GAME_PLAYER)
	{
		m_sStealth[wChairID].cbStealTimeLeft = dwTime;
		m_sStealth[wChairID].cbStealStatus = 1;

		//CMD_S_InventoryConsumptionEvent InventoryConsumptionEvent;
		//ZeroMemory(&InventoryConsumptionEvent, sizeof(InventoryConsumptionEvent));
		//InventoryConsumptionEvent.cbChairID = wChairID;
		//InventoryConsumptionEvent.cbItemID = 1;  //�������ID
		//InventoryConsumptionEvent.cbStealStatus = 1;
		//InventoryConsumptionEvent.cbStealTimeLeft = dwTime;
		//m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CONSUMPTION_INVENTORY_EVENT, &InventoryConsumptionEvent, sizeof(InventoryConsumptionEvent));
		//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CONSUMPTION_INVENTORY_EVENT, &InventoryConsumptionEvent, sizeof(InventoryConsumptionEvent));
	}
}

void CTableFrameSink::StealthUpate()
{
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		if (m_sStealth[i].cbStealStatus == 1 && m_sStealth[i].cbStealTimeLeft != 0)
		{
			m_sStealth[i].cbStealTimeLeft--;
			CMD_S_InventoryConsumptionEvent InventoryConsumptionEvent;
			ZeroMemory(&InventoryConsumptionEvent, sizeof(InventoryConsumptionEvent));
			InventoryConsumptionEvent.cbChairID = i;
			InventoryConsumptionEvent.cbItemID = 1;  //�������ID
			InventoryConsumptionEvent.cbStealStatus = 1;
			InventoryConsumptionEvent.cbStealTimeLeft = m_sStealth[i].cbStealTimeLeft;
			if (m_sStealth[i].cbStealTimeLeft == 0)
			{
				InventoryConsumptionEvent.cbStealStatus = 0;
				m_sStealth[i].cbStealStatus == 0;
			}
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CONSUMPTION_INVENTORY_EVENT, &InventoryConsumptionEvent, sizeof(InventoryConsumptionEvent));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CONSUMPTION_INVENTORY_EVENT, &InventoryConsumptionEvent, sizeof(InventoryConsumptionEvent));
		}
	}
}

BYTE CTableFrameSink::GetHumanHP(WORD wChairID)
{
	BYTE cbHP = 1;

	if (wChairID < GAME_PLAYER)
	{
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		const PlayerInfoItem *pHumanInfoItem = &m_sClientsPlayersInfos.HumanInfoItems[wChairID];
		if (pIServerUserItem != NULL && pHumanInfoItem->cbIsValid)
		{
			cbHP = pHumanInfoItem->cbHP;
		}
	}

	return cbHP;
}
WORD CTableFrameSink::GetDeadHumanNumOfTeam(PlayerTeamType teamType)
{
	if (teamType >= PlayerTeamType::MaxTeamNum)
	{
		return 0;
	}

	WORD wDeadHumanNum = 0;
	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		if (m_sClientsPlayersInfos.HumanInfoItems[wChairID].cbHP==0 && m_sClientsPlayersInfos.HumanInfoItems[wChairID].cbTeamType==teamType)
		{
			wDeadHumanNum++;
		}
	}

	return wDeadHumanNum;
}
WORD CTableFrameSink::GetDeadAINumOfTeam(PlayerTeamType teamType)
{
	if (teamType >= PlayerTeamType::MaxTeamNum)
	{
		return 0;
	}

	WORD wDeadNum = 0;
	for (BYTE cbAIId = 0; cbAIId < GAME_PLAYER; cbAIId++)
	{
		if(m_sClientsPlayersInfos.AIInfoItems[cbAIId].cbHP==0 && m_sClientsPlayersInfos.AIInfoItems[cbAIId].cbTeamType==teamType)
		{
			wDeadNum++;
		}
	}

	return wDeadNum;
}
bool CTableFrameSink::AreAllPlayersOfTeamDead(PlayerTeamType teamType)
{
	WORD wTableUserCountOfTeam = m_pITableFrame->GetSitUserCountOfTeam(teamType);
	WORD wDeadHumanNumOfTeam = GetDeadHumanNumOfTeam(teamType);
	bool bAreAllHumanOfTeamDead = (wDeadHumanNumOfTeam == wTableUserCountOfTeam);

	WORD wDeadAINumOfTeam = GetDeadAINumOfTeam(teamType);
	bool bAreAllAIsOfTeamDead = (wDeadAINumOfTeam == m_cbAINum[teamType]);
	
	bool bAreAllPlayersOfTeamDead = (bAreAllHumanOfTeamDead && bAreAllAIsOfTeamDead && (wDeadHumanNumOfTeam + wDeadAINumOfTeam) > 0);
	return bAreAllPlayersOfTeamDead;
}
bool CTableFrameSink::HideSeek_OnClientPlayersInfo(VOID *pDataBuffer, WORD wDataSize, IServerUserItem *pIServerUserItem)
{
	WORD wChairID = pIServerUserItem->GetChairID();
	if (wChairID >= GAME_PLAYER)
	{
		//Log
		TCHAR szString[256] = TEXT("");
		_sntprintf(szString, CountArray(szString), TEXT("HideSeek_OnClientPlayersInfo��incorrect wChairID=%d"), wChairID);
		CTraceService::TraceString(szString, TraceLevel_Warning);

		return true;
	}

	if (m_pITableFrame->GetGameStatus() != GAME_STATUS_HIDE && m_pITableFrame->GetGameStatus() != GAME_STATUS_PLAY)
	{
		return true;
	}

	CWHDataLocker DataLocker(m_CriticalSection);

	//m_sClientsPlayersInfo[wChairID].StreamValue((BYTE *)pDataBuffer, wDataSize);
	m_sClientsPlayersInfos.StreamValue((BYTE *)pDataBuffer, wDataSize);

	return true;
}
void CTableFrameSink::HideSeek_SendHeartBeat()
{
	if (m_pITableFrame->GetGameStatus() != GAME_STATUS_HIDE && m_pITableFrame->GetGameStatus() != GAME_STATUS_PLAY)  //GAME_STATUS_WAIT
	{
		return;
	}

	CMD_S_HideSeek_HeartBeat HeartBeatMsg;
	//ZeroMemory(&HeartBeatMsg, sizeof(HeartBeatMsg));

	CWHDataLocker DataLocker(m_CriticalSection);

	// Human Items
	const PlayerInfoItem *pHumanInfoItem = NULL;
	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		pHumanInfoItem = &m_sClientsPlayersInfos.HumanInfoItems[wChairID];

		if (!pHumanInfoItem->cbIsValid)
		{
			continue;
		}

		HeartBeatMsg.PlayerInfoItems.push_back(*pHumanInfoItem);
	}

	// AI Items
	const PlayerInfoItem *pAIInfoItem = NULL;
	for (BYTE cbAIId = 0; cbAIId < GAME_PLAYER; cbAIId++)
	{
		pAIInfoItem = &m_sClientsPlayersInfos.AIInfoItems[cbAIId];

		if (!pAIInfoItem->cbIsValid)
		{
			continue;
		}

		HeartBeatMsg.PlayerInfoItems.push_back(*pAIInfoItem);
	}

	// Event Items
	int nEventSize = m_sClientsPlayersInfos.PlayerEventItems.size();
	if (nEventSize > 0)
	{
		const PlayerEventItem *pEventItem = NULL;
		for (int i = 0; i < nEventSize; i++)
		{
			pEventItem = &m_sClientsPlayersInfos.PlayerEventItems[i];
			HeartBeatMsg.PlayerEventItems.push_back(*pEventItem);

			// �����¼�
			BYTE cbTeamType = pEventItem->cbTeamType;
			WORD wChairId = pEventItem->wChairId;
			BYTE cbAIId = pEventItem->cbAIId;
			bool bIsHuman = (cbAIId == INVALID_AI_ID);
			if (wChairId >= GAME_PLAYER)
			{
				//Log
				TCHAR szString[128] = TEXT("");
				_sntprintf(szString, CountArray(szString), TEXT("HideSeek_SendHeartBeat: incorrect pEventItem->wChairId = %d"), wChairId);
				CTraceService::TraceString(szString, TraceLevel_Warning);

				continue;
			}

			PlayerInfoItem *pPlayerOfEvent = NULL;
			if (bIsHuman)
			{
				pPlayerOfEvent = &m_sClientsPlayersInfos.HumanInfoItems[wChairId];
			}
			else
			{
				if (cbAIId >= GAME_PLAYER)
				{
					//Log
					TCHAR szString[128] = TEXT("");
					_sntprintf(szString, CountArray(szString), TEXT("HideSeek_SendHeartBeat: incorrect pEventItem->cbAIId = %d"), cbAIId);
					CTraceService::TraceString(szString, TraceLevel_Warning);

					continue;
				}
				pPlayerOfEvent = &m_sClientsPlayersInfos.AIInfoItems[cbAIId];
			}

			switch (pEventItem->cbEventKind)
			{
				//�����¼�
				//fix AI�����ڼ�,B��A��ɱ,A������������,�����Լ�������
			case PlayerEventKind::DeadByDecHp:
			{
				pPlayerOfEvent->cbHP = 0;
			}
			case PlayerEventKind::DeadByPicked:
			case PlayerEventKind::DeadByBoom:
			{
				pPlayerOfEvent->cbHP = 0;

				//Heal Killer,HP++

				// killer
				BYTE cbKillerTeamType = pEventItem->nCustomData0;
				WORD wKillerChairId = pEventItem->nCustomData1;
				BYTE cbKillerAIId = pEventItem->nCustomData2;
				bool bKillerIsHuman = (cbKillerAIId == INVALID_AI_ID);
				if (wKillerChairId >= GAME_PLAYER)
				{
					//Log
					TCHAR szString[128] = TEXT("");
					_sntprintf(szString, CountArray(szString), TEXT("HideSeek_SendHeartBeat: incorrect wKillerChairId = %d"), wKillerChairId);
					CTraceService::TraceString(szString, TraceLevel_Warning);

					break;
				}

				PlayerInfoItem *pKillerOfEvent = NULL;
				if (bKillerIsHuman)
				{
					pKillerOfEvent = &m_sClientsPlayersInfos.HumanInfoItems[wKillerChairId];
				}
				else
				{
					if (cbKillerAIId >= GAME_PLAYER)
					{
						//Log
						TCHAR szString[128] = TEXT("");
						_sntprintf(szString, CountArray(szString), TEXT("HideSeek_SendHeartBeat: incorrect cbKillerAIId = %d"), cbKillerAIId);
						CTraceService::TraceString(szString, TraceLevel_Warning);

						break;
					}
					pKillerOfEvent = &m_sClientsPlayersInfos.AIInfoItems[cbKillerAIId];
				}

				pKillerOfEvent->cbHP++;
				if (pKillerOfEvent->cbHP > MAX_PLAYER_HP)
				{
					pKillerOfEvent->cbHP = MAX_PLAYER_HP;
				}
			}
			break;

			//HP++
			//��Ѫ����ʹ��ͬ��
			case PlayerEventKind::AddHp:
			{
				pPlayerOfEvent->cbHP++;
				if (pPlayerOfEvent->cbHP > MAX_PLAYER_HP)
				{
					pPlayerOfEvent->cbHP = MAX_PLAYER_HP;
				}
			}
			break;

			//HP--
			case PlayerEventKind::DecHp:
			{
				if (pPlayerOfEvent->cbHP > 0)
				{
					pPlayerOfEvent->cbHP--;
				}
			}
			break;

			//����m_sInventoryList[i].cbUsed
			case PlayerEventKind::GetInventory:
			{
				int nInventoryId = (int)pEventItem->nCustomData0;
				InventoryType eInventoryType = (InventoryType)pEventItem->nCustomData1;
				for (int i = 0; i < MAX_INVENTORY_NUM; i++)
				{
					if (m_sInventoryList[i].cbId == nInventoryId)
					{
						m_sInventoryList[i].cbUsed = 1;

						//Log:��ʾ��Ϣ
						TCHAR szString[128] = TEXT("");
						_sntprintf(szString, CountArray(szString), TEXT("User %d ʹ���˵��ߣ�Type=%d, Index=%d, Id=%d"), wChairId, eInventoryType, i, nInventoryId);
						CTraceService::TraceString(szString, TraceLevel_Normal);
					}
				}
			}
			break;
			}
		}

		//�ͷ�m_sClientsPlayersInfos.PlayerEventItemsռ�õĿռ�
		std::vector<PlayerEventItem> tmpPlayerEventItems;
		tmpPlayerEventItems.swap(m_sClientsPlayersInfos.PlayerEventItems);
		//m_sClientsPlayersInfos.PlayerEventItems.clear();
	}

	datastream kDataStream;
	HeartBeatMsg.StreamValue(kDataStream, true);

	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_HideSeek_HeartBeat, &kDataStream[0], kDataStream.size());
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_HideSeek_HeartBeat, &kDataStream[0], kDataStream.size());
}

/*
void CTableFrameSink::HideSeek_SendHeartBeat()
{
	if (m_pITableFrame->GetGameStatus() != GAME_STATUS_HIDE && m_pITableFrame->GetGameStatus() != GAME_STATUS_PLAY)  //GAME_STATUS_WAIT
	{
		return;
	}

	CMD_S_HideSeek_HeartBeat HeartBeatMsg;
	//ZeroMemory(&HeartBeatMsg, sizeof(HeartBeatMsg));

	CWHDataLocker DataLocker(m_CriticalSection);

	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		CMD_C_HideSeek_ClientPlayersInfo *pClientsPlayersInfo = &m_sClientsPlayersInfo[wChairID];

		if (!pClientsPlayersInfo->bIsValid)
		{
			continue;
		}

		//pClientsPlayersInfo->bIsValid = false;

		// Event Items
		int nEventSize = pClientsPlayersInfo->PlayerEventItems.size();
		if (nEventSize > 0)
		{
			//�����¼�
			for (int i = 0; i < nEventSize; i++)
			{
				//�����¼�,����m_bIsHumanDead,m_bIsAIDead
				//fix AI�����ڼ�,B��A��ɱ,A������������,�����Լ�������
				const PlayerEventItem *pEventItem = &pClientsPlayersInfo->PlayerEventItems[i];
				if (pEventItem->cbEventKind == PlayerEventKind::DeadByDecHp || pEventItem->cbEventKind == PlayerEventKind::DeadByPicked || pEventItem->cbEventKind == PlayerEventKind::DeadByBoom)
				{
					if (pEventItem->wChairId < GAME_PLAYER && pEventItem->cbTeamType < PlayerTeamType::MaxTeamNum)
					{
						if (pEventItem->cbAIId == INVALID_AI_ID)
						{
							m_bIsHumanDead[pEventItem->cbTeamType][pEventItem->wChairId] = true;
						}
						else
						{
							m_bIsAIDead[pEventItem->cbTeamType][pEventItem->cbAIId] = true;
						}
					}

					//�����¼�,HP++
					//Heal Killer
					if (pEventItem->cbEventKind == PlayerEventKind::DeadByPicked || pEventItem->cbEventKind == PlayerEventKind::DeadByBoom)
					{
						// killer
						BYTE cbTeamType = pEventItem->nCustomData0;
						WORD wChairId = pEventItem->nCustomData1;
						BYTE cbAIId = pEventItem->nCustomData2;

						if (wChairId < GAME_PLAYER && cbTeamType < PlayerTeamType::MaxTeamNum)
						{
							if (cbAIId == INVALID_AI_ID)
							{
								m_sClientsPlayersInfo[wChairId].HumanInfoItem.cbHP++;
								if (m_sClientsPlayersInfo[wChairId].HumanInfoItem.cbHP > 4)
								{
									m_sClientsPlayersInfo[wChairId].HumanInfoItem.cbHP = 4;
								}
							}
							else
							{
								// AI
								for (int j = 0; j < m_sClientsPlayersInfo[wChairId].wAIItemCount; j++)
								{
									PlayerInfoItem *pAIInfoItem = &m_sClientsPlayersInfo[wChairId].AIInfoItems[j];
									if (pAIInfoItem->cbAIId == cbAIId)
									{
										pAIInfoItem->cbHP++;
										if (pAIInfoItem->cbHP > 4)
										{
											pAIInfoItem->cbHP = 4;
										}
									}
								}
							}
						}
					}
				}

				//�����¼�,HP++ 
				//��Ѫ����ʹ��ͬ��
				if (pEventItem->cbEventKind == PlayerEventKind::AddHp)
				{
					BYTE cbTeamType = pEventItem->cbTeamType;
					WORD wChairId = pEventItem->wChairId;
					BYTE cbAIId = pEventItem->cbAIId;

					if (wChairId < GAME_PLAYER && cbTeamType < PlayerTeamType::MaxTeamNum)
					{
						if (cbAIId == INVALID_AI_ID)
						{
							m_sClientsPlayersInfo[wChairId].HumanInfoItem.cbHP++;
							if (m_sClientsPlayersInfo[wChairId].HumanInfoItem.cbHP > 4)
							{
								m_sClientsPlayersInfo[wChairId].HumanInfoItem.cbHP = 4;
							}
						}
						else
						{
							// AI
							for (int j = 0; j < m_sClientsPlayersInfo[wChairId].wAIItemCount; j++)
							{
								PlayerInfoItem *pAIInfoItem = &m_sClientsPlayersInfo[wChairId].AIInfoItems[j];
								if (pAIInfoItem->cbAIId == cbAIId)
								{
									pAIInfoItem->cbHP++;
									if (pAIInfoItem->cbHP > 4)
									{
										pAIInfoItem->cbHP = 4;;
									}
								}
							}
						}
					}
				}

				//�����¼�,HP--
				if (pEventItem->cbEventKind == PlayerEventKind::DecHp)
				{
					BYTE cbTeamType = pEventItem->cbTeamType;
					WORD wChairId = pEventItem->wChairId;
					BYTE cbAIId = pEventItem->cbAIId;

					if (wChairId < GAME_PLAYER && cbTeamType < PlayerTeamType::MaxTeamNum)
					{
						if (cbAIId == INVALID_AI_ID)
						{
							if (m_sClientsPlayersInfo[wChairId].HumanInfoItem.cbHP > 0)
							{
								m_sClientsPlayersInfo[wChairId].HumanInfoItem.cbHP--;
							}
						}
						else
						{
							// AI
							for (int j = 0; j < m_sClientsPlayersInfo[wChairId].wAIItemCount; j++)
							{
								PlayerInfoItem *pAIInfoItem = &m_sClientsPlayersInfo[wChairId].AIInfoItems[j];
								if (pAIInfoItem->cbAIId == cbAIId)
								{
									if (pAIInfoItem->cbHP > 0) 
									{
										pAIInfoItem->cbHP--;
									}
								}
							}
						}
					}
				}

				//�����¼�,����m_sInventoryList
				if (pEventItem->cbEventKind == PlayerEventKind::GetInventory)
				{
					int nInventoryId = (int)pEventItem->nCustomData0;
					for (int i = 0; i < MAX_INVENTORY_NUM; i++)
					{
						if (m_sInventoryList[i].cbId == nInventoryId)
						{
							m_sInventoryList[i].cbUsed = 1;

							//Log:��ʾ��Ϣ
							TCHAR szString[512] = TEXT("");
							_sntprintf(szString, CountArray(szString), TEXT("ʹ���˵��ߣ�InventoryIndex=%d, Id=%d"), i, nInventoryId);
							CTraceService::TraceString(szString, TraceLevel_Normal);
						}
					}
				}

				HeartBeatMsg.PlayerEventItems.push_back(*pEventItem);
			}

			//�ͷ�pClientsPlayersInfo->PlayerEventItemsռ�õĿռ�
			std::vector<PlayerEventItem> tmpPlayerEventItems;
			tmpPlayerEventItems.swap(pClientsPlayersInfo->PlayerEventItems);
			//pClientsPlayersInfo->PlayerEventItems.clear();
		}

		// Human Item
		{
			//����m_bIsHumanDead
			PlayerInfoItem *pHumanInfoItem = &pClientsPlayersInfo->HumanInfoItem;
			if (pHumanInfoItem->cbHP == 0 && pHumanInfoItem->wChairId < GAME_PLAYER)
			{
				m_bIsHumanDead[pHumanInfoItem->cbTeamType][pHumanInfoItem->wChairId] = true;
			}

			// fix �Լ����˺���ߣ������Լ����ߺ����ˣ��������ָ����bug
			if (m_bIsHumanDead[pHumanInfoItem->cbTeamType][pHumanInfoItem->wChairId] && pHumanInfoItem->cbHP > 0)
			{
				// �Ѿ���m_bIsHumanDead�б��Ϊ���������յ�������cbHP>0

				pHumanInfoItem->cbHP = 0;
			}

			HeartBeatMsg.PlayerInfoItems.push_back(pClientsPlayersInfo->HumanInfoItem);
		}

		// AI Items
		for (WORD i = 0; i < pClientsPlayersInfo->wAIItemCount; i++)
		{
			//����m_bIsAIDead
			PlayerInfoItem *pAIInfoItem = &pClientsPlayersInfo->AIInfoItems[i];
			if (pAIInfoItem->cbHP == 0 && pAIInfoItem->cbAIId < GAME_PLAYER)
			{
				m_bIsAIDead[pAIInfoItem->cbTeamType][pAIInfoItem->cbAIId] = true;
			}

			// fix �Լ�AI���˺���ߣ������Լ����ߺ��Լ���AI�����˵����ˣ��������ָ����bug
			if (pAIInfoItem->cbAIId < GAME_PLAYER)
			{
				if (m_bIsAIDead[pAIInfoItem->cbTeamType][pAIInfoItem->cbAIId] && pAIInfoItem->cbHP > 0)
				{
					// �Ѿ���m_bIsAIDead�б��Ϊ���������յ�������cbHP>0

					pAIInfoItem->cbHP = 0;
				}
			}

			HeartBeatMsg.PlayerInfoItems.push_back(pClientsPlayersInfo->AIInfoItems[i]);
		}
		pClientsPlayersInfo->wAIItemCount = 0;
	}

	datastream kDataStream;
	HeartBeatMsg.StreamValue(kDataStream, true);

	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_HideSeek_HeartBeat, &kDataStream[0], kDataStream.size());
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_HideSeek_HeartBeat, &kDataStream[0], kDataStream.size());
}
*/
void CTableFrameSink::GenerateAICreateInfo()
{
	//ZeroMemory(m_cbAICreateInfoItems, sizeof(m_cbAICreateInfoItems));

	WORD wTotalAICount = 0;
	for (PlayerTeamType teamType = PlayerTeamType::TaggerTeam; teamType < PlayerTeamType::MaxTeamNum; teamType = (PlayerTeamType)(teamType + 1))
	{
		//����AI����
		WORD wMinUserNumOfTeam = 0;
		if (teamType == PlayerTeamType::TaggerTeam)
		{
			wMinUserNumOfTeam = 3;
		}
		else
		{
			wMinUserNumOfTeam = 7;
		}
		//wMinUserNumOfTeam = 0;

		WORD wTableUserCountOfTeam = m_pITableFrame->GetSitUserCountOfTeam(teamType);
		WORD wAINumOfTeam = 0;
		if (wTableUserCountOfTeam < wMinUserNumOfTeam)
		{
			//��������
			wAINumOfTeam = wMinUserNumOfTeam - wTableUserCountOfTeam;
		}
		m_cbAINum[teamType] = wAINumOfTeam;

		//����AI
		WORD wTableUserCount = m_pITableFrame->GetSitUserCount();
		WORD wCreatedAINumOfTeam = 0;
		if (wAINumOfTeam > 0)
		{
			for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
			{
				IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
				if (pIServerUserItem != NULL)
				{
					WORD wAINumOfUser = wAINumOfTeam / wTableUserCount;
					WORD wAINumLeft = wAINumOfTeam % wTableUserCount;

					if (wChairID < wAINumLeft)
					{
						wAINumOfUser++;
					}

					for (WORD wAIIdx = 0; wAIIdx < wAINumOfUser; wAIIdx++)
					{
						tagUserInfo * pCueUserInfo = pIServerUserItem->GetUserInfo();

						BYTE cbAIIdxOfUser = m_cbAICreateInfoItems[wChairID].cbAINum;
						AICreateInfoItem* pInfoItem = &m_cbAICreateInfoItems[wChairID].cbAICreateInfoItem[cbAIIdxOfUser];
						pInfoItem->cbTeamType = teamType;
						pInfoItem->wChairId = pIServerUserItem->GetChairID();
						pInfoItem->cbModelIdx = rand() % 255;
						pInfoItem->cbAIId = wTotalAICount;

						m_cbAICreateInfoItems[wChairID].cbAINum++;
						wTotalAICount++;
						wCreatedAINumOfTeam++;
					}

					if (wCreatedAINumOfTeam >= wAINumOfTeam)
					{
						break;
					}
				}
			}
		}
	}
}
void CTableFrameSink::HideSeek_SendAICreateInfo(IServerUserItem *pIServerUserItem, bool bOnlySendToLookonUser)
{
	CMD_GF_S_AICreateInfoItems AICreateInfoItems;
	ZeroMemory(&AICreateInfoItems, sizeof(AICreateInfoItems));

	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		for (WORD wAIIdxOfUser = 0; wAIIdxOfUser < m_cbAICreateInfoItems[wChairID].cbAINum; wAIIdxOfUser++)
		{
			AICreateInfoItems.InfoItems[AICreateInfoItems.wItemCount] = m_cbAICreateInfoItems[wChairID].cbAICreateInfoItem[wAIIdxOfUser];
			AICreateInfoItems.wItemCount++;
		}
	}

	//������Ϣ
	DWORD wHeadSize = sizeof(AICreateInfoItems) - sizeof(AICreateInfoItems.InfoItems);
	DWORD wItemDataSize = sizeof(AICreateInfoItem)*AICreateInfoItems.wItemCount;
	DWORD wDataSize = wHeadSize + wItemDataSize;

	//Log:��ʾ��Ϣ
	TCHAR szString[512] = TEXT("");
	_sntprintf(szString, CountArray(szString), TEXT("HideSeek_SendAICreateInfo:wItemCount=%d, wDataSize=%d"), AICreateInfoItems.wItemCount, wDataSize);
	CTraceService::TraceString(szString, TraceLevel_Normal);

	if (pIServerUserItem != NULL)
	{
		m_pITableFrame->SendUserItemData(pIServerUserItem, SUB_S_HideSeek_AICreateInfo, &AICreateInfoItems, wDataSize);
	}
	else
	{
		if (bOnlySendToLookonUser)
		{
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_HideSeek_AICreateInfo, &AICreateInfoItems, wDataSize);
		}
		else
		{
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_HideSeek_AICreateInfo, &AICreateInfoItems, wDataSize);
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_HideSeek_AICreateInfo, &AICreateInfoItems, wDataSize);
		}
	}
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus() == GAME_STATUS_PLAY);
	if (m_pITableFrame->GetGameStatus() != GAME_STATUS_PLAY) return true;

	//�������
	ASSERT(wChairID == m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData) == true);

	//Ч�����
	if (wChairID != m_wCurrentUser) return true;
	if (m_GameLogic.IsValidCard(cbCardData) == false) return true;

	if (cbCardData == m_cbChiCard)
	{

	}
	else
	{
		//ɾ���˿�
		if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID], cbCardData) == false)
		{
			ASSERT(FALSE);
			return true;
		}

		//���ñ���
		m_bSendStatus = true;
		
		m_cbUserAction[wChairID] = WIK_NULL;
		m_cbPerformAction[wChairID] = WIK_NULL;

		//���Ƽ�¼
		m_cbOutCardCount++;
		m_wOutCardUser = wChairID;
		m_cbOutCardData = cbCardData;



		//����Ƿ����Ʈ��
		bool bBaoTou = false;
		bool bCanPiao = false;
		m_GameLogic.IsBaDui(m_cbCardIndex[wChairID], m_WeaveItemArray[wChairID], m_cbWeaveItemCount[wChairID], m_cbOutCardData, bBaoTou, bCanPiao);

		if (m_GameLogic.CanPiaoCai(m_cbCardIndex[wChairID], m_cbOutCardData))
			m_bCanPiaoStatus[wChairID] = true;
		else if (bCanPiao)
			m_bCanPiaoStatus[wChairID] = true;
		else
			m_bCanPiaoStatus[wChairID] = false;


		//�����������ƣ�����Ƿ���Ʈ��
		if (m_GameLogic.IsMagicCard(cbCardData))
		{
			if (!m_bPiaoStatus[wChairID] && m_bGangStatus)
				m_bGangPiao[wChairID] = true;


			m_bPiaoStatus[wChairID] = true;

			//m_bCanPiaoStatus[wChairID]Ϊtrue������Ʈ��Ʈ��ֻ��m_cbPiaoCount[wChairID]����Ŀ
			if (m_bCanPiaoStatus[wChairID])
				m_cbPiaoCount[wChairID]++;
			else
				m_cbPiaoCount[wChairID] = 0;
			

			//���￪ʼ�����Ƿ�Ʈ�Ƴа�
			if (m_cbPiaoCount[wChairID] == 1)
				m_lBasePiaoScore[wChairID] = pow((LONG)2, (LONG)(m_cbPiaoCount[wChairID] + 1 + m_cbGangCount[wChairID]));

			if (m_cbPiaoCount[wChairID] >= 2 && m_bOperateStatus)
			{
				//��Ϊ�����ܣ�������������Ʈ��
				SCORE lPiaoScore = pow((LONG)2, (LONG)(m_cbPiaoCount[wChairID] + 1 + m_cbGangCount[wChairID]));
				if (lPiaoScore > 20)
					lPiaoScore = 20;
				m_lPiaoScore[wChairID][m_wPiaoChengBaoUser] += lPiaoScore * 3 / 2;
			}
		}
		else
		{
			m_bPiaoStatus[wChairID] = false;
			m_cbPiaoCount[wChairID] = 0;
			ZeroMemory(m_lPiaoScore[wChairID], sizeof(m_lPiaoScore[wChairID]));
			m_lBasePiaoScore[wChairID] = 0;

			m_bGangPiao[wChairID] = false;						//�����Ʈ�ƣ����ͷ����״̬�������Ʈ
			m_bPiaoGang[wChairID] = false;						//�����Ʈ�ƣ����ͷ����״̬������Ʈ��

			m_cbGangCount[wChairID] = 0;						//������Ǵ�����񣬾���ո���ͳ��
		}

		m_bOperateStatus = false;								//����״̬�����ƺ�����
		m_wPiaoChengBaoUser = INVALID_CHAIR;

		m_bBuGang = false;

		m_cbChiCard = 0;										//���ó���

		if (m_bGangStatus)
		{
			m_bGangStatus = false;
			m_bGangOutStatus = true;
		}

		//�����Լ������©����¼
		m_bLouPengStatus[wChairID] = false;
		m_cbLouPengCard[wChairID] = 0;



		//��������
		CMD_S_OutCard OutCard;
		ZeroMemory(&OutCard, sizeof(OutCard));
		OutCard.wOutCardUser = wChairID;
		OutCard.cbOutCardData = cbCardData;
		OutCard.bIsPiao = m_bPiaoStatus[wChairID];

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));

		m_wProvideUser = wChairID;
		m_cbProvideCard = cbCardData;

		//�û��л�
		m_wCurrentUser = (wChairID + GAME_PLAYER - 1) % GAME_PLAYER;

		//��Ӧ�ж�
		bool bAroseAction = EstimateUserRespond(wChairID, cbCardData, EstimatKind_OutCard);

		//�����˿�(��ǰ���붪���ƶѣ��������ʱҪ�����ó���)
		if ((m_wOutCardUser != INVALID_CHAIR) && (m_cbOutCardData != 0))
		{
			m_cbDiscardCount[m_wOutCardUser]++;
			m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser] - 1] = m_cbOutCardData;
		}

		//�ɷ��˿�
		if (bAroseAction == false) DispatchCardData(m_wCurrentUser);

		return true;
	}
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus() != GAME_STATUS_FREE);
	if (m_pITableFrame->GetGameStatus() == GAME_STATUS_FREE)
		return true;

	//Ч���û�	ע�⣺�������п��ܷ����˶���
	//ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID != m_wCurrentUser) && (m_wCurrentUser != INVALID_CHAIR))
		return true;

	//��������
	if (m_wCurrentUser == INVALID_CHAIR)
	{
		//Ч��״̬
		if (m_bResponse[wChairID] == true)
			return true;
		if ((cbOperateCode != WIK_NULL) && ((m_cbUserAction[wChairID] & cbOperateCode) == 0))
			return true;

		//��������
		WORD wTargetUser = wChairID;
		BYTE cbTargetAction = cbOperateCode;

		//���ñ���
		m_bResponse[wChairID] = true;
		m_cbPerformAction[wChairID] = cbOperateCode;
		m_cbOperateCard[wChairID] = m_cbProvideCard;

		//©����¼
		if (cbOperateCode == WIK_NULL && (m_cbUserAction[wChairID] & WIK_PENG) != 0)
		{
			//�ͻ��˶�����ѡ�ѡ���˹�
			m_bLouPengStatus[wChairID] = true;
			m_cbLouPengCard[wChairID] = m_cbProvideCard;
		}

		//ִ���ж�
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			//��ȡ����
			BYTE cbUserAction = (m_bResponse[i] == false) ? m_cbUserAction[i] : m_cbPerformAction[i];

			//���ȼ���
			BYTE cbUserActionRank = m_GameLogic.GetUserActionRank(cbUserAction);
			BYTE cbTargetActionRank = m_GameLogic.GetUserActionRank(cbTargetAction);

			//�����ж�
			if (cbUserActionRank > cbTargetActionRank)
			{
				wTargetUser = i;
				cbTargetAction = cbUserAction;
			}
		}
		if (m_bResponse[wTargetUser] == false)
			return true;

		//�Ժ��ȴ�
		if (cbTargetAction == WIK_CHI_HU)
		{
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if ((m_bResponse[i] == false) && (m_cbUserAction[i] & WIK_CHI_HU))
					return true;
			}
		}

		//��������
		if (cbTargetAction == WIK_NULL)
		{
			//�û�״̬
			ZeroMemory(m_bResponse, sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction, sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard, sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction, sizeof(m_cbPerformAction));

			//�����˿�
			DispatchCardData(m_wResumeUser);

			return true;
		}

		//�����˿�(����ʱҪ�����ó���)
		m_cbDiscardCount[m_wProvideUser]--;
		m_cbDiscardCard[m_wProvideUser][m_cbDiscardCount[m_wProvideUser]] = 0;

		//��������
		BYTE cbTargetCard = m_cbOperateCard[wTargetUser];

		//���Ʊ���
		m_cbOutCardData = 0;
		m_bSendStatus = true;
		m_wOutCardUser = INVALID_CHAIR;

		//���Ʋ���
		if (cbTargetAction == WIK_CHI_HU)
		{
			//������Ϣ
			m_cbChiHuCard = cbTargetCard;

			for (WORD i = (m_wProvideUser + GAME_PLAYER - 1) % GAME_PLAYER; i != m_wProvideUser; i = (i + GAME_PLAYER - 1) % GAME_PLAYER)
			{
				//�����ж�
				if ((m_cbPerformAction[i] & WIK_CHI_HU) == 0)
					continue;

				//�����ж�,��ֵȨλ
				BYTE cbWeaveItemCount = m_cbWeaveItemCount[i];
				tagWeaveItem *pWeaveItem = m_WeaveItemArray[i];
				m_dwChiHuKind[i] = AnalyseChiHuCard(m_cbCardIndex[i], pWeaveItem, cbWeaveItemCount, m_cbChiHuCard, m_ChiHuRight[i]);

				//�����˿�
				if (m_dwChiHuKind[i] != WIK_NULL)
				{
					m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
					ProcessChiHuUser(i, false);

					//if ((m_ChiHuRight[i] & CHR_QIANG_GANG).IsEmpty())
					//{
					m_wBankerUser = i;	//���Ʒ�Ϊ��һ��ׯ��
					//}
					//else
					//{
					//	m_wBankerUser = m_wProvideUser; //���ܺ�����������Ϊ��һ��ׯ��
					//}
				}
			}

			OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);

			return true;
		}

		//�û�״̬
		ZeroMemory(m_bResponse, sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction, sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard, sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction, sizeof(m_cbPerformAction));

		//����˿�
		ASSERT(m_cbWeaveItemCount[wTargetUser] < MAX_WEAVE);
		WORD wIndex = m_cbWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard = TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard = cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind = cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser = (m_wProvideUser == INVALID_CHAIR) ? wTargetUser : m_wProvideUser;

		//ɾ���˿ˣ��ԡ�������
		switch (cbTargetAction)
		{
		case WIK_LEFT:		//���Ʋ���
		{
			//ɾ���˿�
			BYTE cbRemoveCard[3];
			m_GameLogic.GetWeaveCard(WIK_LEFT, cbTargetCard, cbRemoveCard);
			VERIFY(m_GameLogic.RemoveCard(cbRemoveCard, 3, &cbTargetCard, 1));
			VERIFY(m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser], cbRemoveCard, 2));

			m_cbChiCard = cbTargetCard;
			break;
		}
		case WIK_RIGHT:		//���Ʋ���
		{
			//ɾ���˿�
			BYTE cbRemoveCard[3];
			m_GameLogic.GetWeaveCard(WIK_RIGHT, cbTargetCard, cbRemoveCard);
			VERIFY(m_GameLogic.RemoveCard(cbRemoveCard, 3, &cbTargetCard, 1));
			VERIFY(m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser], cbRemoveCard, 2));

			m_cbChiCard = cbTargetCard;
			break;
		}
		case WIK_CENTER:	//���Ʋ���
		{
			//ɾ���˿�
			BYTE cbRemoveCard[3];
			m_GameLogic.GetWeaveCard(WIK_CENTER, cbTargetCard, cbRemoveCard);
			VERIFY(m_GameLogic.RemoveCard(cbRemoveCard, 3, &cbTargetCard, 1));
			VERIFY(m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser], cbRemoveCard, 2));

			m_cbChiCard = cbTargetCard;
			break;
		}
		case WIK_PENG:		//���Ʋ���
		{
			//ɾ���˿�
			BYTE cbRemoveCard[] = { cbTargetCard,cbTargetCard };
			VERIFY(m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser], cbRemoveCard, 2));
			break;
		}
		case WIK_GANG:		//�Ÿܲ���
		{
			BYTE cbRemoveCard[] = { cbTargetCard,cbTargetCard,cbTargetCard };
			VERIFY(m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser], cbRemoveCard, CountArray(cbRemoveCard)));
			break;
		}
		default:
			ASSERT(FALSE);
			return false;
		}


		//Ʈ�Ƴа�
		m_bOperateStatus = true;
		m_wPiaoChengBaoUser = m_wProvideUser;
		//��һɫ�а�
		m_wChiPengUser[wTargetUser][m_cbChiPengCount[wTargetUser]++] = m_wProvideUser;



		WORD wClearUser = (wTargetUser + GAME_PLAYER - 1) % GAME_PLAYER;
		//���©����¼
		if ((cbTargetAction & WIK_PENG) != 0 || (cbTargetAction & WIK_GANG) != 0)
		{
			//������
			if (m_wProvideUser != INVALID_CHAIR)
			{
				//���ù����û��Ͳ����û��м���Щ�˵�©����¼
				for (WORD wNextUser = (m_wProvideUser + GAME_PLAYER - 1) % GAME_PLAYER; wNextUser != wClearUser; wNextUser = (wNextUser + GAME_PLAYER - 1) % GAME_PLAYER)
				{
					m_bLouPengStatus[wNextUser] = false;
					m_cbLouPengCard[wNextUser] = 0;
				}
			}
		}



		WORD first = INVALID_CHAIR;
		WORD second = INVALID_CHAIR;
		WORD third = INVALID_CHAIR;
		BYTE count_1 = 0;
		BYTE count_2 = 0;
		BYTE count_3 = 0;
		//BYTE index = 0;
		bool setOnce = false;

		for (BYTE i = 0; i < m_cbChiPengCount[wTargetUser]; i++)
		{
			if (first == INVALID_CHAIR)
			{
				first = m_wChiPengUser[wTargetUser][i];
				count_1++;
			}
			else if (first == m_wChiPengUser[wTargetUser][i])
				count_1++;
			else if (first != INVALID_CHAIR && first != m_wChiPengUser[wTargetUser][i] && second == INVALID_CHAIR)
			{
				second = m_wChiPengUser[wTargetUser][i];
				count_2++;
			}
			else if (second == m_wChiPengUser[wTargetUser][i])
				count_2++;
			else if (first != INVALID_CHAIR && first != m_wChiPengUser[wTargetUser][i] && second != INVALID_CHAIR && second != m_wChiPengUser[wTargetUser][i] && third == INVALID_CHAIR)
			{
				third = m_wChiPengUser[wTargetUser][i];
				count_3++;
			}
			else if (third == m_wChiPengUser[wTargetUser][i])
				count_3++;

			//if (count_1 == 3 || count_2 == 3 || count_3 == 3)
			//{
			//	if (setOnce == false)
			//	{
			//		index = i;
			//		setOnce = true;
			//	}
			//}
		}

		if (count_1 >= 3)
			m_bQingYiSeChengBao[wTargetUser][first] = true;
		else if (count_2 >= 3)
			m_bQingYiSeChengBao[wTargetUser][second] = true;
		else if (count_3 >= 3)
			m_bQingYiSeChengBao[wTargetUser][third] = true;

		/*if (index == 2 && m_cbChiPengCount[wTargetUser] > 3)
		{
			ZeroMemory(m_bQingYiSeChengBao[wTargetUser], sizeof(m_bQingYiSeChengBao[wTargetUser]));
			m_bQingYiSeChengBao[wTargetUser][m_wChiPengUser[wTargetUser][3]] = true;
		}
		else if (index == 3 && m_cbChiPengCount[wTargetUser] > 4)
		{
			ZeroMemory(m_bQingYiSeChengBao[wTargetUser], sizeof(m_bQingYiSeChengBao[wTargetUser]));
			m_bQingYiSeChengBao[wTargetUser][m_wChiPengUser[wTargetUser][4]] = true;
		}*/

		if (m_cbWeaveItemCount[wTargetUser] > 3)
		{
			ZeroMemory(m_bQingYiSeChengBao[wTargetUser], sizeof(m_bQingYiSeChengBao[wTargetUser]));
			m_bQingYiSeChengBao[wTargetUser][m_wChiPengUser[wTargetUser][3]] = true;
		}

		if (m_cbWeaveItemCount[wTargetUser] > 2)
		{
			BYTE cbColor = m_WeaveItemArray[wTargetUser][0].cbCenterCard & MASK_COLOR;
			for (BYTE i = 0; i < m_cbWeaveItemCount[wTargetUser]; i++)
			{
				BYTE cbWeaveColor = m_WeaveItemArray[wTargetUser][i].cbCenterCard & MASK_COLOR;
				if (cbColor != cbWeaveColor)
					ZeroMemory(m_bQingYiSeChengBao[wTargetUser], sizeof(m_bQingYiSeChengBao[wTargetUser]));
			}
		}



		//������
		CMD_S_OperateResult OperateResult;
		ZeroMemory(&OperateResult, sizeof(OperateResult));
		OperateResult.wOperateUser = wTargetUser;
		OperateResult.cbOperateCard = cbTargetCard;
		OperateResult.cbOperateCode = cbTargetAction;
		OperateResult.wProvideUser = (m_wProvideUser == INVALID_CHAIR) ? wTargetUser : m_wProvideUser;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_OPERATE_RESULT, &OperateResult, sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_OPERATE_RESULT, &OperateResult, sizeof(OperateResult));

		//�����û�
		m_wCurrentUser = wTargetUser;

		//���������ܷ����
		if (cbTargetAction == WIK_LEFT || cbTargetAction == WIK_CENTER || cbTargetAction == WIK_RIGHT || cbTargetAction == WIK_PENG)
		{
			//�����ж�
			if (m_cbLeftCardCount > 18 && (m_cbUserAction[wTargetUser] & WIK_GANG) == 0)
			{
				tagGangCardResult GangCardResult;
				m_cbUserAction[wTargetUser] |= m_GameLogic.AnalyseGangCard(m_cbCardIndex[wTargetUser], m_WeaveItemArray[wTargetUser], m_cbWeaveItemCount[wTargetUser], GangCardResult, false);

				if ((m_cbUserAction[wTargetUser] & WIK_GANG) != 0)
				{
					CMD_S_OperateNotify OperateNotify;
					ZeroMemory(&OperateNotify, sizeof(OperateNotify));
					OperateNotify.wResumeUser = wTargetUser;
					OperateNotify.cbActionCard = GangCardResult.cbCardData[0];
					OperateNotify.cbActionMask = m_cbUserAction[wTargetUser];

					//��������
					m_pITableFrame->SendTableData(wTargetUser, SUB_S_OPERATE_NOTIFY, &OperateNotify, sizeof(OperateNotify));
					m_pITableFrame->SendLookonData(wTargetUser, SUB_S_OPERATE_NOTIFY, &OperateNotify, sizeof(OperateNotify));
				}
			}
		}

		//���ƴ����ּܷ���
		if (cbTargetAction == WIK_GANG)
		{
			//������������
			if (m_cbGangCount[wTargetUser] == 0 && m_bGangStatus == false)
				m_cbGangCount[wTargetUser] = 1;
			else
				m_cbGangCount[wTargetUser]++;

			m_bGangStatus = true;

			DispatchCardData(wTargetUser, true);
		}

		return true;
	}

	//��������
	if (m_wCurrentUser == wChairID)
	{
		//Ч�����
		if ((cbOperateCode == WIK_NULL) || ((m_cbUserAction[wChairID] & cbOperateCode) == 0))
			return true;

		//�˿�Ч��
		ASSERT((cbOperateCode == WIK_NULL)
			|| (cbOperateCode == WIK_CHI_HU)
			|| (m_GameLogic.IsValidCard(cbOperateCard) == true));
		if ((cbOperateCode != WIK_NULL)
			&& (cbOperateCode != WIK_CHI_HU)
			&& (m_GameLogic.IsValidCard(cbOperateCard) == false))
			return true;

		//���ñ���
		m_bSendStatus = true;
		m_cbUserAction[m_wCurrentUser] = WIK_NULL;
		m_cbPerformAction[m_wCurrentUser] = WIK_NULL;

		//ִ�ж���
		switch (cbOperateCode)
		{
		case WIK_GANG:			//���Ʋ���
		{
			//��������
			BYTE cbWeaveIndex = 0xFF;
			BYTE cbCardIndex = m_GameLogic.SwitchToCardIndex(cbOperateCard);

			//����
			if (m_cbCardIndex[wChairID][cbCardIndex] == 1)
			{
				//Ѱ�����
				for (BYTE i = 0; i < m_cbWeaveItemCount[wChairID]; i++)
				{
					BYTE cbWeaveKind = m_WeaveItemArray[wChairID][i].cbWeaveKind;
					BYTE cbCenterCard = m_WeaveItemArray[wChairID][i].cbCenterCard;
					if ((cbCenterCard == cbOperateCard) && (cbWeaveKind == WIK_PENG))
					{
						m_bBuGang = true;
						cbWeaveIndex = i;
						break;
					}
				}

				//Ч�鶯��
				ASSERT(cbWeaveIndex != 0xFF);
				if (cbWeaveIndex == 0xFF) return false;

				//����˿�
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard = TRUE;
				m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser = wChairID;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind = cbOperateCode;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard = cbOperateCard;
			}
			else
			{
				//����
				ASSERT(m_cbCardIndex[wChairID][cbCardIndex] == 4);
				if (m_cbCardIndex[wChairID][cbCardIndex] != 4)
					return false;

				m_bBuGang = false;
				//���ñ���
				cbWeaveIndex = m_cbWeaveItemCount[wChairID]++;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard = FALSE;
				m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser = wChairID;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind = cbOperateCode;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard = cbOperateCard;
			}

			//ɾ���˿�
			m_cbCardIndex[wChairID][cbCardIndex] = 0;

			//������������
			if (m_cbGangCount[wChairID] == 0 && m_bGangStatus == false)
				m_cbGangCount[wChairID] = 1;
			else
				m_cbGangCount[wChairID]++;

			m_bGangStatus = true;



			//������
			CMD_S_OperateResult OperateResult;
			ZeroMemory(&OperateResult, sizeof(OperateResult));
			OperateResult.wOperateUser = wChairID;
			OperateResult.wProvideUser = wChairID;
			OperateResult.cbOperateCode = cbOperateCode;
			OperateResult.cbOperateCard = cbOperateCard;

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_OPERATE_RESULT, &OperateResult, sizeof(OperateResult));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_OPERATE_RESULT, &OperateResult, sizeof(OperateResult));


			//���ñ���
			m_cbProvideCard = cbOperateCard;
			m_wProvideUser = wChairID;
			m_wCurrentUser = wChairID;


			//Ч�鶯��
			bool bAroseAction = false;

			if (m_bBuGang == true)
				bAroseAction = EstimateUserRespond(wChairID, cbOperateCard, EstimatKind_GangCard);

			//�����˿�
			if (bAroseAction == false)
			{
				DispatchCardData(wChairID, true);
			}

			return true;
		}
		case WIK_CHI_HU:		//�Ժ�����
		{
			//����Ȩλ
			if (m_cbOutCardCount == 0)
			{
				m_wProvideUser = m_wCurrentUser;
				m_cbProvideCard = m_cbSendCardData;
			}

			//�����ж�,��ֵȨλ
			BYTE cbWeaveItemCount = m_cbWeaveItemCount[wChairID];
			tagWeaveItem *pWeaveItem = m_WeaveItemArray[wChairID];
			m_GameLogic.RemoveCard(m_cbCardIndex[wChairID], &m_cbProvideCard, 1);
			m_dwChiHuKind[wChairID] = AnalyseChiHuCard(m_cbCardIndex[wChairID], pWeaveItem, cbWeaveItemCount, m_cbProvideCard, m_ChiHuRight[wChairID]);


			m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
			ProcessChiHuUser(wChairID, false);


			//������Ϣ
			m_cbChiHuCard = m_cbProvideCard;


			m_wBankerUser = wChairID;	//���Ʒ�Ϊ��һ��ׯ��

			OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);

			return true;
		}
		}

		return true;
	}

	return false;
}

//���Ͳ���
bool CTableFrameSink::SendOperateNotify()
{
	//������ʾ
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if (m_cbUserAction[i] != WIK_NULL)
		{
			//��������
			CMD_S_OperateNotify OperateNotify;
			ZeroMemory(&OperateNotify, sizeof(OperateNotify));
			OperateNotify.wResumeUser = m_wResumeUser;
			OperateNotify.cbActionCard = m_cbProvideCard;
			OperateNotify.cbActionMask = m_cbUserAction[i];

			//��������
			m_pITableFrame->SendTableData(i, SUB_S_OPERATE_NOTIFY, &OperateNotify, sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i, SUB_S_OPERATE_NOTIFY, &OperateNotify, sizeof(OperateNotify));
		}
	}

	return true;
}

//�ɷ��˿�
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser, bool bTail)
{
	//״̬Ч��
	ASSERT(wCurrentUser != INVALID_CHAIR);
	if (wCurrentUser == INVALID_CHAIR)
		return false;

	//��ׯ����
	if (m_cbLeftCardCount < 19)
	{
		//��������£�ץ���һ���Ƶ�Ϊ�¾�ׯ��
		m_wBankerUser = m_wProvideUser;
		m_wResumeUser = INVALID_CHAIR;
		m_wCurrentUser = INVALID_CHAIR;
		m_wProvideUser = INVALID_CHAIR;
		m_cbProvideCard = 0;

		//������Ϣ
		m_cbChiHuCard = 0;
		ZeroMemory(m_dwChiHuKind, sizeof(m_dwChiHuKind));
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			m_ChiHuRight[i].SetEmpty();
		}
		memset(m_wProvider, INVALID_CHAIR, sizeof(m_wProvider));

		OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);

		return true;
	}

	//���ñ���
	m_cbOutCardData = 0;
	m_wOutCardUser = INVALID_CHAIR;

	//�ܺ���
	if (m_bGangOutStatus)
	{
		m_bGangOutStatus = false;
	}



	//Ʈ��
	if (m_bPiaoStatus[wCurrentUser] == true && m_bGangStatus)
		m_bPiaoGang[wCurrentUser] = true;

	//��Ϊ���˶��ܣ�Ʈ�Ƴа�
	if (m_bPiaoGang[wCurrentUser] && m_bOperateStatus)
	{
		SCORE lPiaoScore = pow((LONG)2, (LONG)(m_cbPiaoCount[wCurrentUser] + 1 + m_cbGangCount[wCurrentUser]));
		if (lPiaoScore > 20)
			lPiaoScore = 20;
		m_lPiaoScore[wCurrentUser][m_wPiaoChengBaoUser] += lPiaoScore * 3 / 2;
	}

	//��������
	CMD_S_SendCard SendCard;
	ZeroMemory(&SendCard, sizeof(SendCard));

	//���ƴ���
	if (m_bSendStatus == true)
	{
		//�����˿�
		m_cbSendCardCount++;
		if (hasRule(GAME_TYPE_ZZ_HONGZHONG))
		{
			m_cbSendCardData = m_cbRepertoryCard_HZ[--m_cbLeftCardCount];
		}
		else
		{
			m_cbSendCardData = m_cbRepertoryCard[--m_cbLeftCardCount];
		}

		//���ñ���
		m_cbProvideCard = m_cbSendCardData;
		m_wProvideUser = wCurrentUser;
		m_wCurrentUser = wCurrentUser;

		//�����ж�,��ֵȨλ
		BYTE cbWeaveItemCount = m_cbWeaveItemCount[wCurrentUser];
		tagWeaveItem *pWeaveItem = m_WeaveItemArray[wCurrentUser];
		m_cbUserAction[wCurrentUser] |= AnalyseChiHuCard(m_cbCardIndex[wCurrentUser], pWeaveItem, cbWeaveItemCount, m_cbSendCardData, m_ChiHuRight[wCurrentUser]);


		

		ZeroMemory(&SendCard.tGangCard, sizeof(tagGangCardResult));
		/*//�����ж�
		if (m_cbLeftCardCount > 18)
		{
			auto actionMask = m_GameLogic.EstimateGangCard(m_cbCardIndex[wCurrentUser], m_cbSendCardData);//����
			if (actionMask == WIK_GANG)
			{
				SendCard.tGangCard.cbCardData[SendCard.tGangCard.cbCardCount++] = m_cbSendCardData;
			}
			else
			{
				actionMask = m_GameLogic.EstimateGangCard(m_WeaveItemArray[wCurrentUser], m_cbWeaveItemCount[wCurrentUser], m_cbSendCardData);//����
				if (actionMask == WIK_GANG)
				{
					SendCard.tGangCard.cbCardData[SendCard.tGangCard.cbCardCount++] = m_cbSendCardData;
				}
			}
		}*/

		//����
		m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;



		//�����ж�
		if (m_cbLeftCardCount > 18)
		{
			m_cbUserAction[wCurrentUser] |= m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser], m_WeaveItemArray[wCurrentUser], m_cbWeaveItemCount[wCurrentUser], SendCard.tGangCard, false);
			if (SendCard.tGangCard.cbCardCount > 0)
			{
				CopyMemory(&m_tGangResult[wCurrentUser], &(SendCard.tGangCard), sizeof(tagGangCardResult));
				
				SendCard.cbActionMask |= WIK_GANG;
			}
		}
	}

	SendCard.wCurrentUser = wCurrentUser;
	SendCard.bTail = bTail;
	SendCard.cbActionMask = m_cbUserAction[wCurrentUser];
	SendCard.cbSendCardData = (m_bSendStatus == true) ? m_cbSendCardData : 0x00;
	SendCard.bKaiGangYaoShaiZi = false;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SEND_CARD, &SendCard, sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_SEND_CARD, &SendCard, sizeof(SendCard));

	return true;
}

//��Ӧ�ж�
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//��������
	bool bAroseAction = false;

	//�û�״̬
	ZeroMemory(m_bResponse, sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction, sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction, sizeof(m_cbPerformAction));


	//�������Ʈ�ƣ�ֻ��Ʈ�Ƶ���ҿ��Գԡ���
	bool bHasPiao = false;
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if (m_bPiaoStatus[i] == true)
			bHasPiao = true;
	}


	//�����жϣ������Ÿ�
	//��������
	if (EstimatKind == EstimatKind_OutCard)
	{
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			//�û�����,�����û�
			if (wCenterUser == i) continue;

			//û��Ʈ��
			if (!bHasPiao)
			{
				//û��©�����߲���©��������ͬ
				if (m_bLouPengStatus[i] == false || m_cbLouPengCard[i] != cbCenterCard)
					m_cbUserAction[i] |= m_GameLogic.EstimatePengCard(m_cbCardIndex[i], cbCenterCard);

				//�Ÿ��ж�
				if (m_cbLeftCardCount > 18)//Ϊ�˸��ƺ����ƿ��Բ�
				{
					m_cbUserAction[i] |= m_GameLogic.EstimateGangCard(m_cbCardIndex[i], cbCenterCard);
				}
			}
			//����һ����Ʈ��
			else
			{
				if (m_bPiaoStatus[i] == true)
				{
					//û��©�����߲���©��������ͬ
					if (m_bLouPengStatus[i] == false || m_cbLouPengCard[i] != cbCenterCard)
						m_cbUserAction[i] |= m_GameLogic.EstimatePengCard(m_cbCardIndex[i], cbCenterCard);

					//�Ÿ��ж�
					if (m_cbLeftCardCount > 18)//Ϊ�˸��ƺ����ƿ��Բ�
					{
						m_cbUserAction[i] |= m_GameLogic.EstimateGangCard(m_cbCardIndex[i], cbCenterCard);
					}
				}
			}

			//����ж�,��һ����Ϊnull����
			if (m_cbUserAction[i] != WIK_NULL)
				bAroseAction = true;
		}
	}


	//���ܺ�,����
	//��ͷ����²�������
	bool bHasBaoTou = false;
	bool bHasOne = false;
	if (EstimatKind == EstimatKind_GangCard)
	{
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			//�û�����,�����û�
			if (wCenterUser == i) continue;

			//�Ժ��ж�
			BYTE cbWeaveItemCount = m_cbWeaveItemCount[i];
			tagWeaveItem *pWeaveItem = m_WeaveItemArray[i];
			m_cbUserAction[i] |= AnalyseChiHuCard(m_cbCardIndex[i], pWeaveItem, cbWeaveItemCount, cbCenterCard, m_ChiHuRight[i]);

			//����ж�,��һ����Ϊnull����
			if (m_cbUserAction[i] != WIK_NULL)
				bHasOne = true;

			if (!(m_ChiHuRight[i] & CHR_BAO_TOU).IsEmpty())
				bHasBaoTou = true;
		}

		if (bHasOne && !bHasBaoTou)
			bAroseAction = true;
	}
	else
	{
		//ͳ�Ƴ���
		BYTE bChiCount = 0;
		for (BYTE j = 0; j < m_cbWeaveItemCount[m_wCurrentUser]; j++)
		{
			tagWeaveItem m_WeaveItem = m_WeaveItemArray[m_wCurrentUser][j];
			if (m_WeaveItem.cbWeaveKind == WIK_GANG || m_WeaveItem.cbWeaveKind == WIK_PENG) continue;
			bChiCount++;
		}

		//���ֻ�ܳ�����
		if (bChiCount >= 2)
		{

		}
		else
		{
			if (!bHasPiao)
			{
				m_cbUserAction[m_wCurrentUser] |= m_GameLogic.EstimateEatCard(m_cbCardIndex[m_wCurrentUser], cbCenterCard);
			}
			else
			{
				if (m_bPiaoStatus[m_wCurrentUser] == true)
				{
					m_cbUserAction[m_wCurrentUser] |= m_GameLogic.EstimateEatCard(m_cbCardIndex[m_wCurrentUser], cbCenterCard);
				}
			}
		}

		//����ж�
		if (m_cbUserAction[m_wCurrentUser] != WIK_NULL)
			bAroseAction = true;
	}

	//�������
	if (bAroseAction == true)
	{
		//���ñ���
		m_wProvideUser = wCenterUser;
		m_cbProvideCard = cbCenterCard;
		m_wResumeUser = m_wCurrentUser;
		m_wCurrentUser = INVALID_CHAIR;

		//������ʾ
		SendOperateNotify();
		return true;
	}

	return false;
}

//�������
void CTableFrameSink::ProcessChiHuUser(WORD wChairId, bool bGiveUp)
{
	if (!bGiveUp)
	{
		//Ȩλ����
		FiltrateRight(wChairId, m_ChiHuRight[wChairId]);

		WORD wFanShu = 0;
		if (m_cbGameTypeIdex == GAME_TYPE_ZZ)
		{
			wFanShu = m_GameLogic.GetChiHuActionRank_ZZ(m_ChiHuRight[wChairId]);
		}

		//������θ��Ƽӱ�
		if (m_cbGangCount[wChairId] > 1)
			wFanShu = wFanShu * (m_cbGangCount[wChairId] - 1) * 2;

		if (!(m_ChiHuRight[wChairId] & CHR_ZI_MO).IsEmpty())
		{
			if (wFanShu > 20)
				wFanShu = 20;
		}
		else
		{
			if (wFanShu > 60)
				wFanShu = 60;
		}

		//mChen edit
		LONGLONG lChiHuScore = wFanShu * m_lBaseScore;
		///LONGLONG lChiHuScore = wFanShu * m_pGameServiceOption->lCellScore;


		if (m_wProvideUser == wChairId)
		{
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if (i == wChairId) continue;

				//���Ʒ�
				m_lGameScore[i] -= lChiHuScore;
				m_lGameScore[wChairId] += lChiHuScore;

			}

			//Ʈ�Ƴа�
			bool bHasPiaoScore = false;
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if (i == wChairId) continue;

				if (m_lPiaoScore[wChairId][i] == 0) continue;

				ZeroMemory(m_lGameScore, sizeof(m_lGameScore));
				bHasPiaoScore = true;
			}
			if (bHasPiaoScore)
			{
				for (WORD i = 0; i < GAME_PLAYER; i++)
				{
					if (i == wChairId) continue;

					m_lGameScore[i] -= (m_lBasePiaoScore[wChairId] + m_lPiaoScore[wChairId][i]) * m_lBaseScore;//m_pGameServiceOption->lCellScore
					m_lGameScore[wChairId] += (m_lBasePiaoScore[wChairId] + m_lPiaoScore[wChairId][i]) * m_lBaseScore;;
				}
			}


			//��һɫ�а�
			bool isQing = false;
			bool isBaoTou = false;
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if (i == wChairId) continue;

				if (m_bQingYiSeChengBao[wChairId][i] != true && m_bQingYiSeChengBao[i][wChairId] != true) continue;


				//���а�
				isQing = m_GameLogic.IsQingYiSe(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], m_GameLogic.SwitchToCardData(33));
				isBaoTou = m_GameLogic.IsBaoTou(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], m_GameLogic.SwitchToCardData(33));

				LONGLONG lLoserScore = 0L;
				WORD wLoser = 0;
				if (isQing)
				{
					wLoser = 10;
					if (isBaoTou)
						wLoser = 20;
					lLoserScore = wLoser * m_lBaseScore;//m_pGameServiceOption->lCellScore
				}

				//��ֹ���а�bug,���Ƽ�����һɫ��m_bQingYiSeChengBao == false;��һ������Ʒ���m_bQingYiSeChengBao == true,��������һɫ����ʱ��Ҳ��а���
				//if (m_bQingYiSeChengBao[wChairId][i] == false && m_bQingYiSeChengBao[i][wChairId] == true && !isQing) continue;


				if (!(m_ChiHuRight[wChairId] & CHR_QING_YI_SE).IsEmpty() || isQing)
				{
					if (lLoserScore > lChiHuScore)
						lChiHuScore = lLoserScore;

					ZeroMemory(m_lGameScore, sizeof(m_lGameScore));
					m_lGameScore[i] -= lChiHuScore * 3;
					m_lGameScore[wChairId] += lChiHuScore * 3;
				}
			}

		}
		//����
		else
		{
			bool isQing = false;
			bool isBaoTou = false;
			isQing = m_GameLogic.IsQingYiSe(m_cbCardIndex[m_wProvideUser], m_WeaveItemArray[m_wProvideUser], m_cbWeaveItemCount[m_wProvideUser], m_GameLogic.SwitchToCardData(33));
			isBaoTou = m_GameLogic.IsBaoTou(m_cbCardIndex[m_wProvideUser], m_WeaveItemArray[m_wProvideUser], m_cbWeaveItemCount[m_wProvideUser], m_GameLogic.SwitchToCardData(33));

			LONGLONG lLoserScore = 0L;
			WORD wLoser = 0;
			if (isQing)
			{
				wLoser = 30;
				if (isBaoTou)
					wLoser = 60;
			}

			if (m_cbGangCount[m_wProvideUser] > 1)
				wLoser = 12 * (m_cbGangCount[m_wProvideUser] - 1) * 2;

			if (wLoser > 60)
				wLoser = 60;

			lLoserScore = wLoser * m_lBaseScore;//m_pGameServiceOption->lCellScore


			if (lLoserScore > lChiHuScore)
				lChiHuScore = lLoserScore;


			m_lGameScore[m_wProvideUser] -= lChiHuScore;
			m_lGameScore[wChairId] += lChiHuScore;
		}

		//���ñ���
		m_wProvider[wChairId] = m_wProvideUser;
		m_bGangStatus = false;
		m_bGangOutStatus = false;
		m_wChiHuUser = wChairId;


		//������Ϣ
		CMD_S_ChiHu ChiHu;
		ZeroMemory(&ChiHu, sizeof(ChiHu));
		ChiHu.wChiHuUser = wChairId;
		ChiHu.wProviderUser = m_wProvideUser;
		ChiHu.lGameScore = m_lGameScore[wChairId];
		ChiHu.cbCardCount = m_GameLogic.GetCardCount(m_cbCardIndex[wChairId]);
		ChiHu.cbChiHuCard = m_cbProvideCard;
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CHI_HU, &ChiHu, sizeof(ChiHu));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CHI_HU, &ChiHu, sizeof(ChiHu));

	}

	return;
}





//////////////////////////////////////////////////////////////////////////
