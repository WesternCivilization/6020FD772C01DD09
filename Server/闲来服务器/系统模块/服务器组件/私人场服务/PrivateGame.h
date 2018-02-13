#ifndef PRIVATE_HEAD_FILE
#define PRIVATE_HEAD_FILE

#pragma once

//�����ļ�
#include "CTableFramePrivate.h"
#include "PrivateServiceHead.h"
#include "PrivateTableInfo.h"

///////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////

//ʱ�Ӷ���
#define IDI_DISMISS_WAITE_END		(IDI_PRIVATE_MODULE_START+1)					//�����ɢ����

//mChen add, for HideSeek
#define IDI_PLAY_AGAIN_WAIT_END		(IDI_PRIVATE_MODULE_START+2)

#define DISMISS_WAITE_TIME		30					//�����ɢʱ��

#define AGAIN_WAITE_TIME		30					//���¿�ʼ�ȴ�ʱ��

struct DBR_GR_Create_Private;

//��ʱ��
class PriaveteGame 
	:public IGamePrivateItem
	,public IPrivateEventSink
	,public IServerUserItemSink
{
	//��������
protected:
	tagGameServiceOption *				m_pGameServiceOption;			//��������
	tagGameServiceAttrib *				m_pGameServiceAttrib;			//��������

	CMD_GR_Private_Info					m_kPrivateInfo;
	//�ں˽ӿ�
protected:
	PrivateTableInfo*					m_pTableInfo;				//
	ITimerEngine *						m_pITimerEngine;				//ʱ������
	IDBCorrespondManager *				m_pIDataBaseEngine;				//��������	
	ITCPNetworkEngineEvent *			m_pITCPNetworkEngineEvent;		//��������

	//����ӿ�
protected:
	IMainServiceFrame *					m_pIGameServiceFrame;			//���ܽӿ�
	IServerUserManager *				m_pIServerUserManager;			//�û�����
	IAndroidUserManager	*				m_pAndroidUserManager;			//��������
	IServerUserItemSink *				m_pIServerUserItemSink;			//�û��ص�

	//��������
public:
	//���캯��
	PriaveteGame();
	//��������
	virtual ~PriaveteGame(void);

	bool SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, DWORD wDataSize);

	bool SendTableData(ITableFrame*	pITableFrame, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, DWORD wDataSize);

	void CreatePrivateCost(PrivateTableInfo* pTableInfo);

	//mChen add, for HideSeek
	void CreateUserCost(DWORD dwUserID, DWORD dwCostType, DWORD dwCost);

	//mChen add, for ������:ÿ��������һ�����һ����ʯ
	void CreatePrivateMatchAward(DWORD dwUserID, DWORD dwAwardScore);

	bool joinPrivateRoom(IServerUserItem * pIServerUserItem,ITableFrame * pICurrTableFrame);

	bool OnEventCreatePrivate(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel);

	void sendPrivateRoomInfo(IServerUserItem * pIServerUserItem,PrivateTableInfo* pTableInfo);

	PrivateTableInfo* getTableInfoByRoomID(DWORD dwRoomID);

	//mChen add
	PrivateTableInfo* getTableInfoByRoomIDAndTypeIdex(DWORD dwRoomID, WORD cbGameTypeIdex);

	PrivateTableInfo* getTableInfoByCreaterID(DWORD dwUserID);

	PrivateTableInfo* getTableInfoByTableID(DWORD dwRoomID);

	PrivateTableInfo* getTableInfoByTableFrame(ITableFrame* pTableFrame);

	void DismissRoom(PrivateTableInfo* pTableInfo, BYTE cbReason);

	void ClearRoom(PrivateTableInfo* pTableInfo);

	void DBR_CreatePrivate(DBR_GR_Create_Private* kInfo,DWORD dwSocketID,IServerUserItem* pIServerUserItem,std::string kHttpChannel);
	//�����ӿ�
public:
 	//�ͷŶ���
 	virtual VOID Release(){ delete this; }
 	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//����֪ͨ
	virtual void OnStartService();

	//mChen add, for HideSeek
	virtual void OnStopService();

	//����ӿ�
public:
	//������
	virtual bool BindTableFrame(ITableFrame * pTableFrame,WORD wTableID);
	//��ʼ���ӿ�
	virtual bool InitPrivateInterface(tagPrivateManagerParameter & MatchManagerParameter);	

	//ϵͳ�¼�
public:
	//ʱ���¼�
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//���ݿ��¼�
	virtual bool OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//˽�˳���Ϣ
	virtual bool OnEventSocketPrivate(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);	
	//����˽�˳�
	bool OnTCPNetworkSubCreatePrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//���¼���˽�˳�
	bool OnTCPNetworkSubAgainEnter(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//����˽�˳�
	bool OnTCPNetworkSubJoinPrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//��ɢ
	bool OnTCPNetworkSubDismissPrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	
	//�û��ӿ�
public:
	//�û�����
	virtual bool OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason);
	//�û�״̬
	virtual bool OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID=INVALID_TABLE, WORD wOldChairID=INVALID_CHAIR);
	//�û�Ȩ��
	virtual bool OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight,bool bGameRight=true);	

	//˽�˳��û��¼�
	virtual bool AddPrivateAction(ITableFrame* pTbableFrame,DWORD dwChairID, BYTE	bActionIdex);

	//�¼��ӿ�
public:
	//�û���¼
	virtual bool OnEventUserLogon(IServerUserItem * pIServerUserItem);
	//�û��ǳ�
	virtual bool OnEventUserLogout(IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnEventEnterPrivate(DWORD dwSocketID ,VOID* pData,DWORD dwUserIP, bool bIsMobile);	
	//�û�����
	virtual bool OnEventUserJoinPrivate(IServerUserItem * pIServerUserItem, BYTE cbReason,DWORD dwSocketID);
	//�û�����
	virtual bool OnEventUserQuitPrivate(IServerUserItem * pIServerUserItem, BYTE cbReason, WORD *pBestRank=NULL, DWORD dwContextID=INVALID_WORD);

	 //���ܺ���
public:
	 //��Ϸ��ʼ
	 virtual bool OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount);
	 //��Ϸ����
	 virtual bool OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	 //�û���ȥ��Ϸ
	 virtual bool OnEventClientReady(WORD wChairID,IServerUserItem * pIServerUserItem);

	 //mChen add, for HideSeek
	 virtual void RandomUserTeamTypeAndModelIndex(IServerUserItem * pIServerUserItem, BYTE cbMapIndexRand, BYTE cbChoosedMapIndex, bool bIsXuFang);
	 virtual void ForceDismissAndClearRoom(WORD wTableId);

	 //�û��¼�
public:
	 //�û�����
	 virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem, char* pFunc=NULL, char *pFile = __FILE__, int nLine = __LINE__);
	 //�û�����
	 virtual bool OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	 //�û�����
	 virtual bool OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	 //�û�ͬ��
	 virtual bool OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	
public:
	 //�û�����
	virtual bool OnEventReqStandUP(IServerUserItem * pIServerUserItem, BYTE cbForceStandUP);

	virtual bool WriteTableScore(ITableFrame* pITableFrame,tagScoreInfo ScoreInfoArray[], WORD wScoreCount,datastream& kData);	
	
	//��������
protected:

	//mChen add, for HideSeek
	//ɾ�������¼
	void ClearGameScoreLocker();
};

#endif