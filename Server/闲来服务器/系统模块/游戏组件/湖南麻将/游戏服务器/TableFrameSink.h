#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//ö�ٶ���

//Ч������
enum enEstimatKind
{
	EstimatKind_OutCard,			//����Ч��
	EstimatKind_GangCard,			//����Ч��
};

//mChen add, for HideSeek
//AIͬ��
struct AIInfoItem
{
	BYTE							cbAINum;
	AICreateInfoItem				cbAICreateInfoItem[GAME_PLAYER]; //AI��
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	WORD							m_wBankerUser;							//ׯ���û�
	LONGLONG						m_lGameScore[GAME_PLAYER];				//��Ϸ�÷�
	BYTE							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	//�û��˿�
	bool							m_bTrustee[GAME_PLAYER];				//�Ƿ��й�



	//������Ϣ
protected:
	WORD							m_wOutCardUser;							//�����û�
	BYTE							m_cbOutCardData;						//�����˿�
	BYTE							m_cbOutCardCount;						//������Ŀ
	BYTE							m_cbDiscardCount[GAME_PLAYER];			//������Ŀ
	BYTE							m_cbDiscardCard[GAME_PLAYER][55];		//������¼

	//������Ϣ
protected:
	BYTE							m_cbSendCardData;						//�����˿�
	BYTE							m_cbSendCardCount;						//������Ŀ
	BYTE							m_cbLeftCardCount;						//ʣ����Ŀ
	BYTE							m_cbRepertoryCard[MAX_REPERTORY];		//����˿�
	BYTE							m_cbRepertoryCard_HZ[MAX_REPERTORY_HZ];	//����˿�


	//���б���
protected:
	WORD							m_wResumeUser;							//��ԭ�û�
	WORD							m_wCurrentUser;							//��ǰ�û�
	WORD							m_wProvideUser;							//��Ӧ�û�
	BYTE							m_cbProvideCard;						//��Ӧ�˿�

	//״̬����
protected:
	bool							m_bSendStatus;							//����״̬
	bool							m_bGangStatus;							//����״̬������״̬�»ᷢ�����ܺ����ܿ�
	bool							m_bGangOutStatus;						//�ܺ�״̬������״̬�»ᷢ��������

	//�û�״̬
public:
	bool							m_bResponse[GAME_PLAYER];				//��Ӧ��־
	BYTE							m_cbUserAction[GAME_PLAYER];			//�û�����
	BYTE							m_cbOperateCard[GAME_PLAYER];			//�����˿�
	BYTE							m_cbPerformAction[GAME_PLAYER];			//ִ�ж���

	//mChen add, for HideSeek
	CMD_S_HideSeek_ClientPlayersInfo m_sClientsPlayersInfos;
	//CMD_C_HideSeek_ClientPlayersInfo m_sClientsPlayersInfo[GAME_PLAYER];
	CCriticalSection				m_CriticalSection;					//ͬ������
	//bool							m_bIsHumanDead[PlayerTeamType::MaxTeamNum][GAME_PLAYER];
	//bool							m_bIsAIDead[PlayerTeamType::MaxTeamNum][GAME_PLAYER];
	//AIͬ��
	BYTE							m_cbAINum[PlayerTeamType::MaxTeamNum];
	AIInfoItem						m_cbAICreateInfoItems[GAME_PLAYER];		//�±꣺wChairID
	//����ͬ��
	InventoryItem					m_sInventoryList[MAX_INVENTORY_NUM];
	//����״̬
	StealthEffect                   m_sStealth[GAME_PLAYER];

	//Add begin
	bool							m_bCanPiaoStatus[GAME_PLAYER];					//����Ʈ��״̬
	bool                            m_bPiaoStatus[GAME_PLAYER];						//Ʈ��״̬
	BYTE							m_cbPiaoCount[GAME_PLAYER];						//Ʈ�ƴ���
	bool							m_bGangPiao[GAME_PLAYER];						//��Ʈ
	bool							m_bPiaoGang[GAME_PLAYER];						//Ʈ��

	bool							m_bOperateStatus;								//����״̬
	WORD							m_wPiaoChengBaoUser;							//Ʈ�Ƴа��û�
	SCORE							m_lPiaoScore[GAME_PLAYER][GAME_PLAYER];			//Ʈ�Ƴа���Ҫ�а��ķ���
	SCORE							m_lBasePiaoScore[GAME_PLAYER];					//�а�ʱ�Ļ����֣������а��ķ�����

	bool							m_bBuGang;										//����

	BYTE							m_cbChiPengCount[GAME_PLAYER];
	WORD							m_wChiPengUser[GAME_PLAYER][MAX_WEAVE];			//��¼����ҵĳ�������
	bool							m_bQingYiSeChengBao[GAME_PLAYER][GAME_PLAYER];	//��һɫ�а�״̬

	BYTE							m_cbChiCard;									//����

	bool							m_bLouPengStatus[GAME_PLAYER];					//©��״̬
	BYTE							m_cbLouPengCard[GAME_PLAYER];					//©����

	WORD							m_wChiHuUser;

	BYTE							m_cbGangCount[GAME_PLAYER];						//�������ƴ���

	tagGangCardResult				m_tGangResult[GAME_PLAYER];						//���Ʒ���������֣���
	//Add end

	//����˿�
protected:
	BYTE							m_cbWeaveItemCount[GAME_PLAYER];			//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];	//����˿�

	//������Ϣ
protected:
	BYTE							m_cbChiHuCard;							//�Ժ��˿�
	DWORD							m_dwChiHuKind[GAME_PLAYER];				//�Ժ����
	CChiHuRight						m_ChiHuRight[GAME_PLAYER];				//
	WORD							m_wProvider[GAME_PLAYER];				//

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���
	BYTE							m_cbGameTypeIdex;						//��Ϸ����
	DWORD							m_dwGameRuleIdex;						//��Ϸ����

	//mChen add
	SCORE							m_lBaseScore;

	//ZY add
	int								TotalScore_MJ[GAME_PLAYER];							//�齫�ܷ�
	int								m_bPlayCoutIdex;
	BYTE							playercount;
	BYTE							nowcount;
	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { }
	//�ӿڲ�ѯ
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){  return 0; };
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){ return 0; };
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);
	//��Ϸ���� ZY add
	virtual void ResetPlayerTotalScore();
	void Shuffle(BYTE* RepertoryCard,int nCardCount); //ϴ��

	void GameStart_ZZ();


	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize) { return false; }
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason) { return false; }

	//mChen add, for HideSeek
	virtual void HideSeek_SendHeartBeat();
	virtual void GenerateAICreateInfo();
	virtual void HideSeek_SendAICreateInfo(IServerUserItem *pIServerUserItem = NULL, bool bOnlySendToLookonUser=false);
	virtual WORD GetDeadHumanNumOfTeam(PlayerTeamType teamType);
	virtual WORD GetDeadAINumOfTeam(PlayerTeamType teamType);
	virtual bool AreAllPlayersOfTeamDead(PlayerTeamType teamType);
	virtual BYTE GetHumanHP(WORD wChairID);
	//����ͬ��
	virtual InventoryItem* GetInventoryList();
	virtual void ResetInventoryList();
	virtual void SetResurrection(WORD wChairID);
	virtual void SetStealth(DWORD dwTime, WORD wChairID);
	virtual void StealthUpate();

	//����ӿ�
public:
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem/*, IMainServiceFrame * m_pIMainServiceFram*/);
	//�����Ϣ����
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem, char *pFunc = NULL, char *pFile = __FILE__, int nLine = __LINE__) { return true; }
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize) { return true; }

	virtual void SetPrivateInfo(BYTE bGameTypeIdex,DWORD bGameRuleIdex,SCORE lBaseScore, BYTE bPlayCoutIdex, BYTE PlayerCount);

	//��Ϸ�¼�
protected:
	//mChen add, for HideSeek
	bool HideSeek_OnClientPlayersInfo(VOID *pDataBuffer, WORD wDataSize, IServerUserItem *pIServerUserItem);

	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
	//�û�����
	bool OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard);

public:
	bool hasRule(BYTE cbRule);
	BYTE AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight);
	BYTE AnalyseChiHuCardZZ(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,bool bSelfSendCard);



	//��������
protected:
	//���Ͳ���
	bool SendOperateNotify();
	//�ɷ��˿�
	bool DispatchCardData(WORD wCurrentUser,bool bTail=false);
	//��Ӧ�ж�
	bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind);

	//
	void ProcessChiHuUser( WORD wChairId, bool bGiveUp );
	//
	void FiltrateRight( WORD wChairId,CChiHuRight &chr );

};

//////////////////////////////////////////////////////////////////////////

#endif
