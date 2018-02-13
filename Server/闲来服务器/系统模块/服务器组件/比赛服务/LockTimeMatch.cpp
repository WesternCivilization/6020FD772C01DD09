#include "StdAfx.h"
#include "LockTimeMatch.h"
#include "..\��Ϸ������\DataBasePacket.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define INVALID_VALUE				0xFFFF								//��Чֵ

//////////////////////////////////////////////////////////////////////////

//ʱ�Ӷ���
#define IDI_DISTRIBUTE_USER		    (IDI_MATCH_MODULE_START+90)			//�����û�	(IDI_MATCH_MODULE_START+100) mChen: must<IDI_MATCH_MODULE_FINISH

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���캯��
CLockTimeMatch::CLockTimeMatch()
{
	//״̬����
	m_MatchStatus=MatchStatus_Free;

	//��������
	m_pMatchOption=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;

	//�ں˽ӿ�
	m_ppITableFrame=NULL;
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngineEvent=NULL;

	//����ӿ�
	m_pIGameServiceFrame=NULL;
	m_pIServerUserManager=NULL;
	m_pAndroidUserManager=NULL;

	//mChen add
	m_wCurRound = 0;
	m_wFinishTable = 0;
	m_pITablePlayCount = NULL;
}

CLockTimeMatch::~CLockTimeMatch(void)
{
	//�ͷ���Դ
	SafeDeleteArray(m_ppITableFrame);

	//�رն�ʱ��
	m_pITimerEngine->KillTimer(IDI_CHECK_END_MATCH);
	m_pITimerEngine->KillTimer(IDI_DISTRIBUTE_USER);

	//�Ƴ�Ԫ��
	m_DistributeManage.RemoveAll();

	//mChen add
	SafeDeleteArray(m_pITablePlayCount);
}

//�ӿڲ�ѯ
VOID* CLockTimeMatch::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{	
	QUERYINTERFACE(IGameMatchItem,Guid,dwQueryVer);
	QUERYINTERFACE(IMatchEventSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerUserItemSink,Guid,dwQueryVer);	
	QUERYINTERFACE_IUNKNOWNEX(IGameMatchItem,Guid,dwQueryVer);
	return NULL;
}

//������
bool CLockTimeMatch::BindTableFrame(ITableFrame * pTableFrame,WORD wTableID)
{
	if(pTableFrame==NULL || wTableID>m_pGameServiceOption->wTableCount)
	{
		ASSERT(false);
		return false;
	}

	//��������
	CTableFrameHook * pTableFrameHook=new CTableFrameHook();
	pTableFrameHook->InitTableFrameHook(QUERY_OBJECT_PTR_INTERFACE(pTableFrame,IUnknownEx));
	pTableFrameHook->SetMatchEventSink(QUERY_OBJECT_PTR_INTERFACE(this,IUnknownEx));

	//���ýӿ�
	pTableFrame->SetTableFrameHook(QUERY_OBJECT_PTR_INTERFACE(pTableFrameHook,IUnknownEx));
	m_ppITableFrame[wTableID]=pTableFrame;

	return true;
}

//��ʼ���ӿ�
bool CLockTimeMatch::InitMatchInterface(tagMatchManagerParameter & MatchManagerParameter)
{
	//��������
	m_pMatchOption=MatchManagerParameter.pGameMatchOption;
	m_pGameServiceOption=MatchManagerParameter.pGameServiceOption;
	m_pGameServiceAttrib=MatchManagerParameter.pGameServiceAttrib;

	//�ں����
	m_pITimerEngine=MatchManagerParameter.pITimerEngine;
	m_pIDataBaseEngine=MatchManagerParameter.pICorrespondManager;
	m_pITCPNetworkEngineEvent=MatchManagerParameter.pTCPNetworkEngine;

	//�������		
	m_pIGameServiceFrame=MatchManagerParameter.pIMainServiceFrame;		
	m_pIServerUserManager=MatchManagerParameter.pIServerUserManager;
	m_pAndroidUserManager=MatchManagerParameter.pIAndroidUserManager;
	m_pIServerUserItemSink=MatchManagerParameter.pIServerUserItemSink;

	//��������
	m_DistributeManage.SetDistributeRule(m_pGameServiceOption->cbDistributeRule);

	//��������
	if (m_ppITableFrame==NULL)
	{
		m_ppITableFrame=new ITableFrame*[m_pGameServiceOption->wTableCount];
	}

	//mChen add
	if (m_pITablePlayCount == NULL)
	{
		m_pITablePlayCount = new DWORD[m_pGameServiceOption->wTableCount];
	}
	
	////mChen add
	//DWORD dwSignupStartInterval = GetSignupStartInterval();
	//if (dwSignupStartInterval > 0)
	//{
	//	//��ʼ���	
	//	m_pITimerEngine->SetTimer(IDI_CHECK_START_SIGNUP, dwSignupStartInterval * 1000L, TIMES_INFINITY, NULL);
	//}

	//mChen comment
	//��ʼ���
	DWORD dwStartInterval = GetMatchStartInterval();
	if(dwStartInterval>0)
	{
		//��ʼ���	
		m_pITimerEngine->SetTimer(IDI_CHECK_START_MATCH,dwStartInterval*1000L,TIMES_INFINITY,NULL);
	}

	return true;
}

//ʱ���¼�
bool CLockTimeMatch::OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{	
	switch(dwTimerID)
	{
	case IDI_DISTRIBUTE_USER:				//�����û�
		{
			//ִ�з���
			PerformDistribute();

			return true;
		}

		//mChen add
	case IDI_CHECK_START_SIGNUP:				//��ʼ����
		{
			//����״̬
			m_MatchStatus = MatchStatus_Signingup;

			//�رն�ʱ��
			m_pITimerEngine->KillTimer(IDI_CHECK_START_SIGNUP);

			//��������ʼ
			CTime CurTime = CTime::GetCurrentTime();//���ֵӦ����SignupStartTime��SignupEndTime֮��
			CTime SignupStartTime(m_pMatchOption->MatchType.LockTime.SignupStartTime);
			CTime SignupEndTime(m_pMatchOption->MatchType.LockTime.SignupEndTime);
			CTime MatchStartTime(m_pMatchOption->MatchType.LockTime.MatchStartTime);
			DWORD dwCurTimeStamp = CurTime.GetHour() * 3600 + CurTime.GetMinute() * 60 + CurTime.GetSecond();
			DWORD dwSignupStartStamp = SignupStartTime.GetHour() * 3600 + SignupStartTime.GetMinute() * 60 + SignupStartTime.GetSecond();
			DWORD dwSignupEndStamp = SignupEndTime.GetHour() * 3600 + SignupEndTime.GetMinute() * 60 + SignupEndTime.GetSecond();
			DWORD dwMatchStartStamp = MatchStartTime.GetHour() * 3600 + MatchStartTime.GetMinute() * 60 + MatchStartTime.GetSecond();
			CTimeSpan TimeSpan = SignupEndTime - SignupStartTime;
			DWORD dwTimeInterval = dwSignupEndStamp - dwCurTimeStamp;//TimeSpan.GetTotalSeconds(); //60��
			m_pITimerEngine->SetTimer(IDI_CHECK_START_MATCH, dwTimeInterval * 1000L, TIMES_INFINITY, NULL);

			return true;
		}

	case IDI_CHECK_START_MATCH:				//��ʼ����
		{
			////mChen add
			//WORD wDistributeCount = m_DistributeManage.GetCount();
			//if (wDistributeCount < m_pGameServiceOption->wMinDistributeUser)
			//{
			//	//���ͱ�������δ��Ҫ��ȡ��������Ϣ
			//	TCHAR szMessage[128] = TEXT("��Ϊ��������δ��Ҫ�󣬽���ı���ȡ�����������������μӣ�");
			//	m_pIGameServiceFrame->SendGameMessage(szMessage, SMT_CHAT | SMT_EJECT | SMT_GLOBAL | SMT_CLOSE_GAME);

			//	//�رն�ʱ��
			//	m_pITimerEngine->KillTimer(IDI_CHECK_START_MATCH);

			//	//ǿ�Ʊ�������
			//	m_wCurRound = 3;
			//	m_pITimerEngine->SetTimer(IDI_CHECK_END_MATCH, 1000, TIMES_INFINITY, 0);

			//	return true;
			//}

			//����״̬
			m_MatchStatus=MatchStatus_Playing;

			//����״̬
			BYTE cbMatchStatus=MS_MATCHING;
			m_pIGameServiceFrame->SendData(BG_ALL_CLIENT,MDM_GR_MATCH,SUB_GR_MATCH_STATUS,&cbMatchStatus,sizeof(cbMatchStatus));

			//�رն�ʱ��
			m_pITimerEngine->KillTimer(IDI_CHECK_START_MATCH);

			//����������
			DWORD dwEndInterval = GetMatchEndInterval();
			m_pITimerEngine->SetTimer(IDI_CHECK_END_MATCH, dwEndInterval*1000L, TIMES_INFINITY, 0);//60*2*1000L

			//ִ�з���
			m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_USER,m_pGameServiceOption->wDistributeTimeSpace*1000L,TIMES_INFINITY,NULL);

			SendMatchInfo(NULL);

			//mChen add
			m_wCurRound = 1;

			return true;
		}
	case IDI_CHECK_END_MATCH:				//��������
		{
			//��ȡʱ��
			CTime CurTime=CTime::GetCurrentTime();
			SYSTEMTIME MatchEndTime=m_pMatchOption->MatchType.LockTime.MatchEndTime;

			//�������
			if(m_wCurRound >= 1)//3//if (CurTime.GetHour()*3600+CurTime.GetMinute()*60+CurTime.GetSecond() >= MatchEndTime.wHour*3600+MatchEndTime.wMinute*60+MatchEndTime.wSecond)//if(CurTime >= MatchEndTime) //mChen edit 
			{
				//�رն�ʱ��
				m_pITimerEngine->KillTimer(IDI_CHECK_END_MATCH);
				m_pITimerEngine->KillTimer(IDI_DISTRIBUTE_USER);

				//����״̬
				BYTE cbMatchStatus=MS_MATCHEND;
				m_pIGameServiceFrame->SendData(BG_ALL_CLIENT,MDM_GR_MATCH,SUB_GR_MATCH_STATUS,&cbMatchStatus,sizeof(cbMatchStatus));

				//��ɢ��Ϸ
				for(INT i=0; i<m_pGameServiceOption->wTableCount; i++)
				{
					//��ȡ����
					ITableFrame * pITableFrame=m_ppITableFrame[i];					
					ASSERT(pITableFrame!=NULL);
					if (pITableFrame==NULL) continue;

					//��ɢ��Ϸ
					if(pITableFrame->IsGameStarted())
					{
						pITableFrame->DismissGame();
					}
				}

				//��������
				if (m_MatchStatus == MatchStatus_Signingup)
				{
					//mChen add
					//������δ����ȡ������
				}
				else if (m_MatchStatus == MatchStatus_Playing)//if(CurTime >= MatchEndTime)//mChen edit
				{
					//��������
					DBR_GR_MatchOver MatchOver;
					ZeroMemory(&MatchOver,sizeof(MatchOver));

					//��������				
					MatchOver.cbMatchType=MATCH_TYPE_LOCKTIME;
					MatchOver.dwMatchID=m_pMatchOption->dwMatchID;
					MatchOver.dwMatchNO=m_pMatchOption->dwMatchNO;
					MatchOver.wServerID=m_pGameServiceOption->wServerID;
					MatchOver.MatchEndTime=m_pMatchOption->MatchType.LockTime.MatchEndTime;
					MatchOver.MatchStartTime=m_pMatchOption->MatchType.LockTime.MatchStartTime;

					//Ͷ������
					m_pIDataBaseEngine->PostDataBaseRequest(0,DBR_GR_MATCH_OVER,0,&MatchOver,sizeof(MatchOver));
				}

				//����״̬
				m_MatchStatus = MatchStatus_End;

				////mChen add
				////��һ�α������
				//DWORD dwSignupStartInterval = GetSignupStartInterval();
				//if (dwSignupStartInterval > 0)
				//{
				//	m_pITimerEngine->SetTimer(IDI_CHECK_START_SIGNUP, dwSignupStartInterval * 1000L, TIMES_INFINITY, NULL);
				//}
				//return true;


				//���ͽ�����Ϣ
				TCHAR szMessage[128]=TEXT("����ı������˽������������������μӣ�");
				m_pIGameServiceFrame->SendGameMessage(szMessage,SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_GAME);

				//�������
				DWORD dwStartInterval = GetMatchStartInterval();
				if(dwStartInterval>0)
				{
					//mChen add, for temp test
					///dwStartInterval = 10;
					m_pITimerEngine->SetTimer(IDI_CHECK_START_MATCH,dwStartInterval*1000L,TIMES_INFINITY,NULL);
				}
			}
			return true;
		}
	}
	return true;
}

//���ݿ��¼�
bool CLockTimeMatch::OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	switch(wRequestID)
	{
	case DBO_GR_MATCH_RANK_LIST:			//��������
		{
			//����У��
			ASSERT(wDataSize<=sizeof(DBO_GR_MatchRankList));
			if(wDataSize>sizeof(DBO_GR_MatchRankList)) return false;

			//��ȡ����
			DBO_GR_MatchRankList * pMatchRankList = (DBO_GR_MatchRankList*)pData;

			//��������
			bool bRewardUser=true;
			tagMatchRankInfo * pMatchRankInfo=NULL;
			IServerUserItem * pIRewardUserItem=NULL;

			//���Ž���
			for(WORD wIndex=0;wIndex<pMatchRankList->wUserCount;wIndex++)
			{
				//�������
				pMatchRankInfo=&pMatchRankList->MatchRankInfo[wIndex];
				pIRewardUserItem=m_pIServerUserManager->SearchUserItem(pMatchRankInfo->dwUserID);
				if (pIRewardUserItem==NULL || pIRewardUserItem->IsAndroidUser()==true) continue;

				//д�뽱��
				WriteUserAward(pIRewardUserItem,pMatchRankInfo);

				//���ñ�ʶ
				pIRewardUserItem->SetMatchData(&bRewardUser);
			}

			//�����û�
			WORD wUserIndex=0;			
			while (true)
			{
				pIRewardUserItem=m_pIServerUserManager->EnumUserItem(wUserIndex++);
				if (pIRewardUserItem==NULL) break;
				if (pIRewardUserItem->IsAndroidUser()==true) continue;
				if (pIRewardUserItem->GetMatchData()==&bRewardUser)
				{
					pIRewardUserItem->SetMatchData(NULL);
					continue;
				}

				//���ͽ�����Ϣ
				TCHAR szMessage[128]=TEXT("���α������˽���������ǰ����վ�鿴ϵͳ�������Ƽ���������������������");
				m_pIGameServiceFrame->SendGameMessage(pIRewardUserItem,szMessage,SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_GAME);	
			}			

			break;
		}
	case DBO_GR_MATCH_REWARD_RESULT:	//�������
		{
			//����Ч��
			if(pIServerUserItem==NULL) return true;
			if(wDataSize>sizeof(DBR_GR_MatchRewardResult)) return false;

			//��ȡ����
			DBR_GR_MatchRewardResult * pMatchRewardResult = (DBR_GR_MatchRewardResult*)pData;

			//�����ɹ�
			if(pMatchRewardResult->bResultCode==true)
			{
				//����ṹ
				CMD_GR_MatchGoldUpdate MatchGoldUpdate;

				//���ñ���
				MatchGoldUpdate.lCurrGold=pMatchRewardResult->lCurrGold;
				MatchGoldUpdate.lCurrIngot=pMatchRewardResult->lCurrIngot;
				MatchGoldUpdate.dwCurrExprience=pMatchRewardResult->dwCurrExperience;

				//��������
				m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_MATCH,SUB_GR_MATCH_GOLDUPDATE,&MatchGoldUpdate,sizeof(MatchGoldUpdate));
			}

			break;
		}
	}
	return true;
}

//�����¼�
bool CLockTimeMatch::OnEventSocketMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//mChen add
	switch (wSubCmdID)
	{
		case SUB_GR_MATCH_FEE:	//��������
		{
			//����Ч��
			ASSERT(wDataSize == sizeof(SCORE));
			if (wDataSize != sizeof(SCORE)) return false;

			//��ȡ����
			SCORE lMatchFee = *(SCORE*)pData;
			if (lMatchFee != m_pMatchOption->lMatchFee)return false;

			//��������
			DBR_GR_MatchSignup MatchSignup;
			ZeroMemory(&MatchSignup, sizeof(MatchSignup));

			//����ṹ
			MatchSignup.dwUserID = pIServerUserItem->GetUserID();
			MatchSignup.lMatchFee = m_pMatchOption->lMatchFee;
			MatchSignup.dwInoutIndex = pIServerUserItem->GetInoutIndex();
			MatchSignup.dwClientAddr = pIServerUserItem->GetClientAddr();
			MatchSignup.dwMatchID = m_pMatchOption->dwMatchID;
			MatchSignup.dwMatchNO = m_pMatchOption->dwMatchNO;//5 ;//mChen hack
			lstrcpyn(MatchSignup.szMachineID, pIServerUserItem->GetMachineID(), CountArray(MatchSignup.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(MatchSignup.dwUserID, DBR_GR_MATCH_SIGNUP, dwSocketID, &MatchSignup, sizeof(MatchSignup));

			return true;
		}

		case SUB_GR_LEAVE_MATCH:	//�˳�����
		{
			//����Ч��
			ASSERT(pIServerUserItem != NULL);
			if (pIServerUserItem == NULL) return false;

			OnEventUserQuitMatch(pIServerUserItem, 0, 0, dwSocketID);

			return true;
		}
	}

	return true;
}


//�û�����
bool CLockTimeMatch::OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CMD_GR_UserScore UserScore;
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();

	//��������
	UserScore.dwUserID=pUserInfo->dwUserID;
	UserScore.UserScore.dwWinCount=pUserInfo->dwWinCount;
	UserScore.UserScore.dwLostCount=pUserInfo->dwLostCount;
	UserScore.UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	UserScore.UserScore.dwFleeCount=pUserInfo->dwFleeCount;	
	UserScore.UserScore.dwExperience=pUserInfo->dwExperience;
	UserScore.UserScore.lLoveLiness=pUserInfo->lLoveLiness;

	//�������
	UserScore.UserScore.lGrade=pUserInfo->lGrade;
	UserScore.UserScore.lInsure=pUserInfo->lInsure;

	//�������
	UserScore.UserScore.lScore=pUserInfo->lScore;
	UserScore.UserScore.lScore+=pIServerUserItem->GetTrusteeScore();
	UserScore.UserScore.lScore+=pIServerUserItem->GetFrozenedScore();

	//��������
	m_pIGameServiceFrame->SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_SCORE,&UserScore,sizeof(UserScore));

	//��������
	CMD_GR_MobileUserScore MobileUserScore;

	//��������
	MobileUserScore.dwUserID=pUserInfo->dwUserID;
	MobileUserScore.UserScore.dwWinCount=pUserInfo->dwWinCount;
	MobileUserScore.UserScore.dwLostCount=pUserInfo->dwLostCount;
	MobileUserScore.UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	MobileUserScore.UserScore.dwFleeCount=pUserInfo->dwFleeCount;
	MobileUserScore.UserScore.dwExperience=pUserInfo->dwExperience;

	//�������
	MobileUserScore.UserScore.lScore=pUserInfo->lScore;
	MobileUserScore.UserScore.lScore+=pIServerUserItem->GetTrusteeScore();
	MobileUserScore.UserScore.lScore+=pIServerUserItem->GetFrozenedScore();

	//��������
	m_pIGameServiceFrame->SendDataBatchToMobileUser(pIServerUserItem->GetTableID(),MDM_GR_USER,SUB_GR_USER_SCORE,&MobileUserScore,sizeof(MobileUserScore));

	//��ʱд��
	if ((CServerRule::IsImmediateWriteScore(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->IsVariation()==true))
	{
		//��������
		DBR_GR_WriteGameScore WriteGameScore;
		ZeroMemory(&WriteGameScore,sizeof(WriteGameScore));

		//�û���Ϣ
		WriteGameScore.dwUserID=pIServerUserItem->GetUserID();
		WriteGameScore.dwDBQuestID=pIServerUserItem->GetDBQuestID();
		WriteGameScore.dwClientAddr=pIServerUserItem->GetClientAddr();
		WriteGameScore.dwInoutIndex=pIServerUserItem->GetInoutIndex();

		//��ȡ����
		pIServerUserItem->DistillVariation(WriteGameScore.VariationInfo);

		//������Ϣ
		WriteGameScore.dwMatchID=m_pMatchOption->dwMatchID;
		WriteGameScore.dwMatchNO=m_pMatchOption->dwMatchNO;

		//Ͷ������
		m_pIDataBaseEngine->PostDataBaseRequest(WriteGameScore.dwUserID,DBR_GR_WRITE_GAME_SCORE,0L,&WriteGameScore,sizeof(WriteGameScore), TRUE);
	}

	return true;
}

//�û�״̬
bool CLockTimeMatch::OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID, WORD wOldChairID)
{
	if(m_pIServerUserItemSink!=NULL)
	{
		return m_pIServerUserItemSink->OnEventUserItemStatus(pIServerUserItem,wOldTableID,wOldChairID);
	}

	return true;
}

//�û�Ȩ��
bool CLockTimeMatch::OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight,bool bGameRight)
{
	if(m_pIServerUserItemSink!=NULL)
	{
		return m_pIServerUserItemSink->OnEventUserItemRight(pIServerUserItem,dwAddRight,dwRemoveRight,bGameRight);
	}

	return true;
}

//�û���¼
bool CLockTimeMatch::OnEventUserLogon(IServerUserItem * pIServerUserItem)
{
	//����״̬
	if(m_MatchStatus>=MatchStatus_Free)
	{
		//����״̬
		BYTE cbMatchStatus=(m_MatchStatus==MatchStatus_Playing)?MS_MATCHING:MS_MATCHEND;
		m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_MATCH,SUB_GR_MATCH_STATUS,&cbMatchStatus,sizeof(cbMatchStatus));

		SendMatchInfo(NULL);
	}

	return true;
}

//�û��ǳ�
bool CLockTimeMatch::OnEventUserLogout(IServerUserItem * pIServerUserItem)
{
	//����ṹ
	SendMatchInfo(NULL);
	return true;
}

//�����¼�
bool CLockTimeMatch::OnEventEnterMatch(DWORD dwSocketID ,VOID* pData,DWORD dwUserIP, bool bIsMobile)
{
	//�ж�ʱ��
	TCHAR szDescribe[256];
	WORD wMessageType;
	if (VerifyMatchTime(szDescribe,CountArray(szDescribe),wMessageType)==false)
	{
		m_pIGameServiceFrame->SendRoomMessage(dwSocketID,szDescribe,wMessageType|SMT_GLOBAL,dwUserIP==0);

		////mChen add: ǿ�ƿͻ��˶���
		////fix�ͻ���bug������ʱ��δ���μӵ��µ�mServiceStatus==ServiceStatus_Entering���Ӷ�����֮���κ�ģʽ��½����System_Tips_1
		////Method1
		////����CAttemperEngineSink::SendLogonFailure
		//CMD_GR_LogonFailure LogonFailure;
		//ZeroMemory(&LogonFailure, sizeof(LogonFailure));
		//LogonFailure.lErrorCode = 0;
		//lstrcpyn(LogonFailure.szDescribeString, szDescribe, CountArray(LogonFailure.szDescribeString));
		//WORD wDataSize = CountStringBuffer(LogonFailure.szDescribeString);
		//WORD wHeadSize = sizeof(LogonFailure) - sizeof(LogonFailure.szDescribeString);
		//IServerUserItem* pIServerUserItem = m_pIServerUserManager->SearchUserItem(dwSocketID);
		//m_pIGameServiceFrame->SendData(pIServerUserItem, MDM_GR_LOGON, SUB_GR_LOGON_FAILURE, &LogonFailure, wHeadSize + wDataSize);

		////Method 2
		//m_pITCPNetworkEngineEvent->OnEventTCPNetworkShut(dwSocketID, 0, 0L);

		////Method 3
		//m_pIDataBaseEngine->PostDataBaseRequest(LogonUserID.dwUserID, DBR_GR_LOGON_USERID, dwSocketID, &LogonUserID, sizeof(LogonUserID));

		return true;
	}

	//�ֻ��û�
	if(bIsMobile == true)
	{
		//������Ϣ
		CMD_GR_LogonMobile * pLogonMobile=(CMD_GR_LogonMobile *)pData;
		pLogonMobile->szPassword[CountArray(pLogonMobile->szPassword)-1]=0;
		pLogonMobile->szMachineID[CountArray(pLogonMobile->szMachineID)-1]=0;

		//��������
		DBR_GR_LogonMobile LogonMobile;
		ZeroMemory(&LogonMobile,sizeof(LogonMobile));

		//��������
		LogonMobile.dwUserID=pLogonMobile->dwUserID;
		LogonMobile.dwClientAddr=dwUserIP;		
		LogonMobile.dwMatchID=m_pMatchOption->dwMatchID;	
		LogonMobile.dwMatchNO=m_pMatchOption->dwMatchNO;
		LogonMobile.cbDeviceType=pLogonMobile->cbDeviceType;
		LogonMobile.wBehaviorFlags=pLogonMobile->wBehaviorFlags;
		LogonMobile.wPageTableCount=pLogonMobile->wPageTableCount;		
		lstrcpyn(LogonMobile.szPassword,pLogonMobile->szPassword,CountArray(LogonMobile.szPassword));
		lstrcpyn(LogonMobile.szMachineID,pLogonMobile->szMachineID,CountArray(LogonMobile.szMachineID));

		//Ͷ������
		m_pIDataBaseEngine->PostDataBaseRequest(LogonMobile.dwUserID,DBR_GR_LOGON_MOBILE,dwSocketID,&LogonMobile,sizeof(LogonMobile));
	}
	else
	{
		//������Ϣ
		CMD_GR_LogonUserID * pLogonUserID=(CMD_GR_LogonUserID *)pData;
		pLogonUserID->szPassword[CountArray(pLogonUserID->szPassword)-1]=0;
		pLogonUserID->szMachineID[CountArray(pLogonUserID->szMachineID)-1]=0;

		//��������
		DBR_GR_LogonUserID LogonUserID;
		ZeroMemory(&LogonUserID,sizeof(LogonUserID));

		//��������
		LogonUserID.dwClientAddr=dwUserIP;
		LogonUserID.dwUserID=pLogonUserID->dwUserID;
		LogonUserID.dwMatchID=m_pMatchOption->dwMatchID;	
		LogonUserID.dwMatchNO=m_pMatchOption->dwMatchNO;	
		lstrcpyn(LogonUserID.szPassword,pLogonUserID->szPassword,CountArray(LogonUserID.szPassword));
		lstrcpyn(LogonUserID.szMachineID,pLogonUserID->szMachineID,CountArray(LogonUserID.szMachineID));

		//Ͷ������
		m_pIDataBaseEngine->PostDataBaseRequest(LogonUserID.dwUserID,DBR_GR_LOGON_USERID,dwSocketID,&LogonUserID,sizeof(LogonUserID));
	}

	return true;
}

//�û�����
bool CLockTimeMatch::OnEventUserJoinMatch(IServerUserItem * pIServerUserItem, BYTE cbReason,DWORD dwSocketID)
{
	//����У��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem == NULL) return false;

	//�ж�״̬
	if(pIServerUserItem->GetUserStatus()!=US_FREE || pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		return false;
	}

	////����δ��ʼ
	//if(m_MatchStatus==MatchStatus_Free)
	//{
	//	//������Ϣ
	//	m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("������δ��ʼ�����Ժ�������"),SMT_EJECT);

	//	return false;
	//}

	//��������
	if(m_MatchStatus==MatchStatus_End)
	{
		//������Ϣ
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("�����Ѿ������������Ƽ�����������������������"),SMT_EJECT);
		return true;
	}

	//�������
	if(InsertDistribute(pIServerUserItem)==false) return false;
	
	//���ͳɼ�
	SendMatchUserScore(pIServerUserItem);

	//������Ϣ
	m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("ϵͳ����Ŭ��Ϊ��������λ�����Ժ󡣡���"),SMT_CHAT);

	SendMatchInfo(NULL);

	return true;
}

//�û�����
bool CLockTimeMatch::OnEventUserQuitMatch(IServerUserItem * pIServerUserItem, BYTE cbReason, WORD *pBestRank, DWORD dwContextID)
{
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�Ƴ�����
	m_DistributeManage.RemoveDistributeNode(pIServerUserItem);

	//�Ƴ�ͬ����Ϣ
	m_DistributeManage.RemoveUserSameTableInfo(pIServerUserItem->GetUserID());

	return true;
}

//��Ϸ��ʼ
bool CLockTimeMatch::OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount)
{
	//���ͱ�����Ϣ
	SendTableUserMatchInfo(pITableFrame, INVALID_CHAIR);

	//�������ݰ�
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//��������
	IServerUserItem *pIServerUserItem=NULL;
	IServerUserItem *pITableUserItem=NULL;	

	for (INT i=0; i<pITableFrame->GetChairCount(); i++)
	{
		pIServerUserItem=pITableFrame->GetTableUserItem(i);
		ASSERT(pIServerUserItem!=NULL);
		if (pIServerUserItem==NULL) continue;

		//������ʾ��Ϣ
		TCHAR szMessage[64]=TEXT("");
		_sntprintf(szMessage, CountArray(szMessage),TEXT("���ֱ��������ĵ� %d �֡�"),pIServerUserItem->GetUserPlayCount()+1);
		lstrcpyn(SystemMessage.szString,szMessage,CountArray(SystemMessage.szString));
		SystemMessage.wLength=CountStringBuffer(SystemMessage.szString);
		SystemMessage.wType=SMT_CHAT;

		//������ʾ����
		DWORD wSendSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString)+SystemMessage.wLength*sizeof(TCHAR);
		pITableFrame->SendTableData(pIServerUserItem->GetChairID(),SUB_GF_SYSTEM_MESSAGE, &SystemMessage,wSendSize,MDM_GF_FRAME);

		//���ͬ��
		ClearSameTableUser(pIServerUserItem->GetUserID());

		for (INT j=0; j<pITableFrame->GetChairCount(); j++)
		{
			//��ȡ�û�
			pITableUserItem=pITableFrame->GetTableUserItem(j);
			if(pITableUserItem==NULL) continue;
			if(pITableUserItem==pIServerUserItem) continue;

			//����ͬ���û�
			InserSameTableUser(pIServerUserItem->GetUserID(),pITableUserItem->GetUserID());
		}
	}

	return true;
}

//mChen add
//��������
struct tagMatchScore
{
	DWORD					dwUserID;						//�û�I D
	SCORE					lScore;							//�û��ɼ�	
	IServerUserItem *		pUserItem;						//�û��ӿ�
};
//���������
WORD SortTableUser(ITableFrame *pITableFrame, tagMatchScore score[])
{
	INT_PTR nCount = pITableFrame->GetChairCount();
	for (INT i = 0; i < pITableFrame->GetChairCount(); i++)
	{
		IServerUserItem *pIUserItem = pITableFrame->GetTableUserItem(i);
		score[i].dwUserID = pIUserItem->GetUserID();
		score[i].pUserItem = pIUserItem;
		score[i].lScore = pIUserItem->GetUserScore();
	}

	for (INT_PTR i = 1; i < nCount; i++)
	{
		INT_PTR left = 0, right = i - 1;
		tagMatchScore  Temp = score[i];
		while (left <= right)
		{
			INT_PTR mid = (left + right) / 2;
			if (score[i].lScore > score[mid].lScore)right = mid - 1;
			else left = mid + 1;
		}
		for (INT_PTR j = i - 1; j >= left; j--)
			score[j + 1] = score[j];
		score[left] = Temp;
	}
	return (WORD)nCount;
}
//���������û��뿪
void AllUserExitTable(ITableFrame *pITableFrame)
{
	ASSERT(pITableFrame != NULL);
	if (pITableFrame)
	{
		//�����û�ȫ���뿪
		WORD wChairCount = pITableFrame->GetChairCount();
		for (int i = 0; i < wChairCount; i++)
		{
			IServerUserItem* pUserItem = pITableFrame->GetTableUserItem(i);
			if (pUserItem&&pUserItem->GetTableID() != INVALID_TABLE)pITableFrame->PerformStandUpAction(pUserItem);
		}
	}
}

//mChen add
void CLockTimeMatch::SelectPromotionUserOfTable(ITableFrame *pITableFrame)
{
	//��������
	INT_PTR nCount = pITableFrame->GetChairCount();
	tagMatchScore *pScore = new tagMatchScore[nCount];
	ZeroMemory(pScore, sizeof(tagMatchScore)*nCount);
	WORD wCount = SortTableUser(pITableFrame, pScore);

	//ʹ�����û�ȫ���뿪
	//ע�����������InsertDistributeǰ���ã���ΪPerformStandUpAction�����RemoveDistribute(pIServerUserItem)
	AllUserExitTable(pITableFrame);

	FLOAT fPromotionRate = 0.0;// (m_wCurRound == 1 ? 0.5 : 0.5);
	CString str;
	for (INT_PTR i = 0; i < wCount; i++)
	{
		if(true)///if (i >= (INT_PTR)wCount*fPromotionRate)//m_pMatchRule->dwSecRoundUserCount//��̭һ����ˡ�Note:�����Ϊif(false)����ÿ�������󶼻��Զ���ʼ��һ������
		{
			//��̭��

			//����ṹ
			DBR_GR_MatchEliminate MatchEliminate;
			MatchEliminate.cbMatchType = MATCH_TYPE_LOCKTIME;
			MatchEliminate.dwMatchID = m_pMatchOption->dwMatchID;
			MatchEliminate.dwMatchNO = m_pMatchOption->dwMatchNO;
			MatchEliminate.dwUserID = pScore[i].dwUserID;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(0, DBR_GR_MATCH_ELIMINATE, 0, &MatchEliminate, sizeof(MatchEliminate));

			//��������
			m_pIGameServiceFrame->SendData(pScore[i].pUserItem, MDM_GR_MATCH, SUB_GR_MATCH_ELIMINATE, NULL, 0);

			//������Ϣ
			str.Format(TEXT("�ڱ�������������õ�%d��"), i + 1);
			///str.Format(TEXT("��Ǹ����δ�ܳɹ��������ڱ��ֱ���������õ�%d��"), i + 1);
			m_pIGameServiceFrame->SendGameMessage(pScore[i].pUserItem, str, SMT_EJECT | SMT_CHAT);

			////�û�����
			//if (pScore[i].pUserItem->GetTableID() != INVALID_TABLE)
			//{
			//	WORD wTableID = pScore[i].pUserItem->GetTableID();
			//	ITableFrame * pITableFrame = m_ppITableFrame[wTableID];
			//	pITableFrame->PerformStandUpAction(pScore[i].pUserItem);
			//}
			////RemoveDistribute(pScore[i].pUserItem);
		}
		else
		{
			//������

			//�������
			if (InsertDistribute(pScore[i].pUserItem) == true)
			{
				//������Ϣ
				str.Format(TEXT("��ϲ�������ɹ����ڱ�������������õ�%d�������Ժ���һ�����Ͽ�ʼ"), i + 1);
				m_pIGameServiceFrame->SendGameMessage(pScore[i].pUserItem, str, SMT_CHAT | SMT_TABLE_ROLL);
			}
			else
			{
				ASSERT(false);
			}
		}
	}
}

//��Ϸ����
bool CLockTimeMatch::OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	ASSERT(pITableFrame!=NULL);
	if (pITableFrame==NULL) return false;

	//mChen add
	//��������
	if (cbReason != REMOVE_BY_ENDMATCH)
	{
		WORD wTableID = pITableFrame->GetTableID();
		m_pITablePlayCount[wTableID]++;

		if (pIServerUserItem)
		{
			WORD wTableID2 = pIServerUserItem->GetTableID();
		}

		//��������
		//IServerUserItem *pIUserItem = pITableFrame->GetTableUserItem(0);
		//DWORD dwUserPlayedCount = pIUserItem->GetUserPlayCount()-pIUserItem->GetUserInfo()->dwFleeCount;
		//WORD dwUserPlayIdx = dwUserPlayedCount % m_pMatchOption->MatchType.LockTime.wMatchPlayCount;
		int nCurTablePlayCount = m_pITablePlayCount[wTableID];
		if (nCurTablePlayCount >= m_pMatchOption->MatchType.LockTime.wMatchPlayCount)//if(dwUserPlayIdx >= m_pMatchOption->MatchType.LockTime.wMatchPlayCount)
		{
			//�������һ��

			m_pITablePlayCount[wTableID] = 0;

			CreateMatchCost(pITableFrame);

			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("������ɱ������������Լ���������ȴ����α����������������"));
			for (WORD i = 0; i < pITableFrame->GetChairCount(); i++)
			{
				//��ȡ�û�
				IServerUserItem *pTmpServerUserItem = pITableFrame->GetTableUserItem(i);

				//������Ϣ
				m_pIGameServiceFrame->SendGameMessage(pTmpServerUserItem, szMessage, SMT_CHAT|SMT_TABLE_ROLL);//SMT_CLOSE_ROOM

				tagUserInfo *pUserScore = pTmpServerUserItem->GetUserInfo();
				pUserScore->dwWinCount = 0L;
				pUserScore->dwLostCount = 0L;
				pUserScore->dwFleeCount = 0L;
				pUserScore->dwDrawCount = 0L;
			}

			SelectPromotionUserOfTable(pITableFrame);

			m_wFinishTable++;
			//if (m_wFinishTable >= m_pGameServiceOption->wMinDistributeUser / m_pGameServiceAttrib->wChairCount)
			//{
			//	//���������һ��

			//	_sntprintf(szMessage, CountArray(szMessage), TEXT("��������ɵ�%d�ֱ�����"), m_wCurRound);

			//	m_wFinishTable = 0;
			//	m_pGameServiceOption->wMinDistributeUser *= 0.5;
			//	m_wCurRound++;

			//	//������Ϣ
			//	for (WORD i = 0; i < pITableFrame->GetChairCount(); i++)
			//	{
			//		//��ȡ�û�
			//		IServerUserItem *pTmpServerUserItem = pITableFrame->GetTableUserItem(i);
			//		m_pIGameServiceFrame->SendGameMessage(pTmpServerUserItem, szMessage, SMT_CHAT | SMT_TABLE_ROLL);
			//	}
			//}

			return true;
		}
		else
		{
			//������Ϣ
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("�������%d/%d������"), m_pITablePlayCount[wTableID], m_pMatchOption->MatchType.LockTime.wMatchPlayCount);
			for (WORD i = 0; i < pITableFrame->GetChairCount(); i++)
			{
				//��ȡ�û�
				IServerUserItem *pTmpServerUserItem = pITableFrame->GetTableUserItem(i);
				m_pIGameServiceFrame->SendGameMessage(pTmpServerUserItem, szMessage, SMT_CHAT | SMT_TABLE_ROLL);
			}
		}
	}

	//��������
	for (INT i=0; i<pITableFrame->GetChairCount(); i++)
	{
		IServerUserItem *pIUserItem=pITableFrame->GetTableUserItem(i);
		ASSERT(pIUserItem!=NULL);
		if (pIUserItem==NULL) continue;

		//��������
		if(cbReason!=REMOVE_BY_ENDMATCH)
		{
			//�ﵽ��̭��
			if(false)//if (m_pMatchOption->MatchType.LockTime.lMatchCullScore>0 && pIUserItem->GetUserScore()<m_pMatchOption->MatchType.LockTime.lMatchCullScore)//mChen edit
			{
				//��������
				TCHAR szMessage[128]=TEXT("");
				_sntprintf(szMessage,CountArray(szMessage),TEXT("�������ı����ҵ��� %d���������ܼ�����Ϸ��"),m_pMatchOption->MatchType.LockTime.lMatchCullScore);

				//������Ϣ
				m_pIGameServiceFrame->SendGameMessage(pIUserItem,szMessage,SMT_EJECT|SMT_CHAT);

				//����ṹ
				DBR_GR_MatchEliminate MatchEliminate;
				MatchEliminate.cbMatchType=MATCH_TYPE_LOCKTIME;
				MatchEliminate.dwMatchID=m_pMatchOption->dwMatchID;
				MatchEliminate.dwMatchNO=m_pMatchOption->dwMatchNO;
				MatchEliminate.dwUserID=pIUserItem->GetUserID();

				//Ͷ������
				m_pIDataBaseEngine->PostDataBaseRequest(0,DBR_GR_MATCH_ELIMINATE,0,&MatchEliminate,sizeof(MatchEliminate));

				//��������
				m_pIGameServiceFrame->SendData(pIUserItem,MDM_GR_MATCH,SUB_GR_MATCH_ELIMINATE,NULL,0);

				continue;
			}

			//�������
			if(InsertDistribute(pIUserItem)==true)
			{
				///m_pIGameServiceFrame->SendGameMessage(pIUserItem,TEXT("���ֱ���������ϵͳ����Ŭ��Ϊ��������λ�����Ժ�..."),SMT_CHAT|SMT_TABLE_ROLL);
			}

			////��ɾ���
			//if(pIUserItem->GetUserPlayCount()-pIUserItem->GetUserInfo()->dwFleeCount==m_pMatchOption->MatchType.LockTime.wMatchPlayCount)
			//{
			//	TCHAR szMessage[128]=TEXT("");
			//	_sntprintf(szMessage,CountArray(szMessage),TEXT("�������%d�ֱ��������Լ�����ǰ������ѡ���˳��ȴ����α����������������"),m_pMatchOption->MatchType.LockTime.wMatchPlayCount);
			//	m_pIGameServiceFrame->SendGameMessage(pIUserItem,szMessage,SMT_CHAT|SMT_TABLE_ROLL);
			//}
		}
	}

	return true;
}


//mChen add
void CLockTimeMatch::CreateMatchCost(ITableFrame* pITableFrame)
{
	for (int i = 0; i < pITableFrame->GetSitUserCount(); i++)
	{
		IServerUserItem *pIUserItem = pITableFrame->GetTableUserItem(i);
		if (!pIUserItem)
		{
			continue;
		}
		DBR_GR_Create_Private_Cost kNetInfo;
		kNetInfo.dwUserID = pITableFrame->GetTableUserItem(i)->GetUserID();
		kNetInfo.dwCost = m_pMatchOption->lMatchFee;
		kNetInfo.dwCostType = 0;// Type_Private;
		m_pIDataBaseEngine->PostDataBaseRequest(0L, DBR_GR_CREATE_PRIVAT_COST, 0L, &kNetInfo, sizeof(kNetInfo));
	}

	////����ṹ
	//CMD_GR_Match_Fee MatchFee;
	//MatchFee.lMatchFee = m_pMatchOption->lMatchFee;
	////������ʾ
	//if (MatchFee.lMatchFee > 0)
	//{
	//	//��������
	//	LPCTSTR pszMatchFeeType[] = { TEXT("��Ϸ��"),TEXT("Ԫ��") };
	//	if (m_pMatchOption->cbMatchFeeType < CountArray(pszMatchFeeType))
	//	{
	//		_sntprintf(MatchFee.szNotifyContent, CountArray(MatchFee.szNotifyContent), TEXT("�������۳������� %I64d %s��ȷ��Ҫ������"), m_pMatchOption->lMatchFee, pszMatchFeeType[m_pMatchOption->cbMatchFeeType]);
	//	}
	//}
	//for (int i = 0; i < pITableFrame->GetSitUserCount(); i++)
	//{
	//	IServerUserItem *pIUserItem = pITableFrame->GetTableUserItem(i);
	//	if (!pIUserItem)
	//	{
	//		continue;
	//	}

	//	//������Ϣ
	//	WORD wDataSize = sizeof(MatchFee) - sizeof(MatchFee.szNotifyContent);
	//	wDataSize += CountStringBuffer(MatchFee.szNotifyContent);
	//	m_pIGameServiceFrame->SendData(pIUserItem, MDM_GR_MATCH, SUB_GR_MATCH_FEE, &MatchFee, wDataSize);
	//	///OnEventSocketMatch(SUB_GR_MATCH_FEE, &m_pMatchOption->lMatchFee, sizeof(SCORE), pIUserItem, dwSocketID);
	//}
}

//�û�����
bool CLockTimeMatch::OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{ 
	return true; 
}

//�û�����
bool CLockTimeMatch::OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//�Ƴ�����
	return RemoveDistribute(pIServerUserItem);
}

 //�û�ͬ��
bool CLockTimeMatch::OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{ 
	return true; 
}

//����Ȩ��
bool CLockTimeMatch::VerifyUserEnterMatch(IServerUserItem * pIServerUserItem)
{
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�����˲�������
	if (pIServerUserItem->IsAndroidUser()==true)
		return true;

	//��������
	WORD wMessageType;
	TCHAR szMessage[128]=TEXT("");

	//����ʱ���ѵ�	
	if (VerifyMatchTime(szMessage,CountArray(szMessage),wMessageType)==false)
	{
		//������Ϣ
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,szMessage,wMessageType);

		return false;
	}

	//�ﵽ��̭��
	if (m_pMatchOption->MatchType.LockTime.lMatchCullScore>0 && pIServerUserItem->GetUserScore()<m_pMatchOption->MatchType.LockTime.lMatchCullScore)
	{
		_sntprintf(szMessage,CountArray(szMessage),TEXT("�������ı����ҵ��� %d���������ܼ�����Ϸ��"),m_pMatchOption->MatchType.LockTime.lMatchCullScore);

		//������Ϣ
		if(pIServerUserItem->GetTableID()==INVALID_TABLE)
		{
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,szMessage,SMT_EJECT|SMT_CHAT);
		}
		else
		{
			m_pIGameServiceFrame->SendGameMessage(pIServerUserItem,szMessage,SMT_EJECT|SMT_CHAT);
		}

		return false;
	}

	return true;
}

//mChen add
//������ʼ���
DWORD CLockTimeMatch::GetSignupStartInterval()
{
	//��ȡʱ��
	CTime CurTime = CTime::GetCurrentTime();
	CTime SignupStartTime(m_pMatchOption->MatchType.LockTime.SignupStartTime);
	///CTime SignupEndTime(m_pMatchOption->MatchType.LockTime.SignupEndTime);

	//��������
	LONGLONG dwTimeInterval = 0;
	DWORD dwCurrStamp, dwStartStamp, dwEndStamp;

	int deltaDay = SignupStartTime.GetDayOfWeek() - CurTime.GetDayOfWeek();
	if (deltaDay > 0)
	{
		//δ��������
		dwTimeInterval += 24 * 3600 * deltaDay;
	}
	else if (deltaDay < 0)
	{
		//�ѹ�������
		dwTimeInterval += 24 * 3600 * (7+deltaDay);
	}

	//����ʱ��
	dwCurrStamp = CurTime.GetHour() * 3600 + CurTime.GetMinute() * 60 + CurTime.GetSecond();
	dwStartStamp = m_pMatchOption->MatchType.LockTime.SignupStartTime.wHour * 3600 + m_pMatchOption->MatchType.LockTime.SignupStartTime.wMinute * 60 + m_pMatchOption->MatchType.LockTime.SignupStartTime.wSecond;
	dwEndStamp = m_pMatchOption->MatchType.LockTime.SignupEndTime.wHour * 3600 + m_pMatchOption->MatchType.LockTime.SignupEndTime.wMinute * 60 + m_pMatchOption->MatchType.LockTime.SignupEndTime.wSecond;

	//����ʱ�䲹��
	if (deltaDay == 0)
	{
		if (dwCurrStamp <= dwStartStamp)
		{
			//δ��ʼ
			dwTimeInterval += (dwStartStamp - dwCurrStamp);
		}
		else if (dwCurrStamp < dwEndStamp)
		{
			//��ʱ�����
			dwTimeInterval += 1;//1���������ʼ
		}
		else// if (dwCurrStamp > dwEndStamp)
		{
			//�ѽ���
			dwTimeInterval += (24 * 3600 * 7-(dwCurrStamp - dwStartStamp));
		}
	}
	else
	{
		dwTimeInterval += (dwStartStamp - dwCurrStamp);
	}

	if (m_wCurRound >= 1)//3
	{
		m_wCurRound = 0;
	}

	return (DWORD)dwTimeInterval;
}

//��ʼ���
DWORD CLockTimeMatch::GetMatchStartInterval()
{
	//��ȡʱ��
	CTime CurTime=CTime::GetCurrentTime();
	CTime MatchStartTime(m_pMatchOption->MatchType.LockTime.MatchStartTime);
	CTime MatchEndTime(m_pMatchOption->MatchType.LockTime.MatchEndTime);

	//��������
	LONGLONG dwTimeInterval=1;
	DWORD dwCurrStamp,dwStartStamp,dwEndStamp;

	//����δ��ʼ
	if(CurTime<MatchStartTime)
	{
		CTimeSpan TimeSpan = MatchStartTime-CurTime;
		dwTimeInterval = TimeSpan.GetTotalSeconds();
	}

	//�����ѽ���
	if(CurTime>=MatchEndTime) dwTimeInterval=0;

	//����ʱ��
	dwCurrStamp = CurTime.GetHour()*3600+CurTime.GetMinute()*60+CurTime.GetSecond();
	dwStartStamp = m_pMatchOption->MatchType.LockTime.MatchStartTime.wHour*3600+m_pMatchOption->MatchType.LockTime.MatchStartTime.wMinute*60+m_pMatchOption->MatchType.LockTime.MatchStartTime.wSecond;
	dwEndStamp = m_pMatchOption->MatchType.LockTime.MatchEndTime.wHour*3600+m_pMatchOption->MatchType.LockTime.MatchEndTime.wMinute*60+m_pMatchOption->MatchType.LockTime.MatchEndTime.wSecond;

	//����δ��ʼ
	if(dwCurrStamp<dwStartStamp) 
	{
		dwTimeInterval = dwStartStamp-dwCurrStamp;
	}

	//�����ѽ���
	if(dwCurrStamp>=dwEndStamp)
	{
		dwTimeInterval = 24*3600-(dwCurrStamp-dwStartStamp);	

		//������ʾ
		TCHAR szString[128] = TEXT("");
		_sntprintf(szString, CountArray(szString), TEXT("LockTimeMatch::GetMatchStartInterval�������ѽ���!dwTimeInterval=[ %d ]"), dwTimeInterval);
		//��ʾ��Ϣ
		CTraceService::TraceString(szString, TraceLevel_Normal);
	}

	//mChen add
	if (m_wCurRound >= 1)//3
	{
		m_wCurRound = 0;
		dwTimeInterval = 24*3600 - (dwCurrStamp - dwStartStamp);//24*3600*7
	}

	return (DWORD)dwTimeInterval;
}


//��ʼ���
DWORD CLockTimeMatch::GetMatchEndInterval()
{
	//��ȡʱ��
	CTime CurTime = CTime::GetCurrentTime();
	///CTime MatchStartTime(m_pMatchOption->MatchType.LockTime.MatchStartTime);
	CTime MatchEndTime(m_pMatchOption->MatchType.LockTime.MatchEndTime);

	//��������
	LONGLONG dwTimeInterval = 1;
	DWORD dwCurrStamp, dwStartStamp, dwEndStamp;

	////����δ��ʼ
	//if (CurTime < MatchStartTime)
	//{
	//	CTimeSpan TimeSpan = MatchStartTime - CurTime;
	//	dwTimeInterval = TimeSpan.GetTotalSeconds();
	//}

	//�����ѽ���
	if (CurTime >= MatchEndTime) dwTimeInterval = 0;

	//����ʱ��
	dwCurrStamp = CurTime.GetHour() * 3600 + CurTime.GetMinute() * 60 + CurTime.GetSecond();
	///dwStartStamp = m_pMatchOption->MatchType.LockTime.MatchStartTime.wHour * 3600 + m_pMatchOption->MatchType.LockTime.MatchStartTime.wMinute * 60 + m_pMatchOption->MatchType.LockTime.MatchStartTime.wSecond;
	dwEndStamp = m_pMatchOption->MatchType.LockTime.MatchEndTime.wHour * 3600 + m_pMatchOption->MatchType.LockTime.MatchEndTime.wMinute * 60 + m_pMatchOption->MatchType.LockTime.MatchEndTime.wSecond;

	////����δ��ʼ
	//if (dwCurrStamp < dwStartStamp)
	//{
	//	dwTimeInterval = dwStartStamp - dwCurrStamp;
	//}

	//����δ����
	if (dwCurrStamp < dwEndStamp)
	{
		dwTimeInterval = dwEndStamp - dwCurrStamp;
	}

	return (DWORD)dwTimeInterval;
}

//�����û�
bool CLockTimeMatch::PerformDistribute()
{
	//״̬�ж�
	if(m_MatchStatus!=MatchStatus_Playing) return true;

	//mChen add
	FLOAT fPromotionRate = 1;
	switch (m_wCurRound)
	{
	case 1:
		fPromotionRate = 1;
		break;
	case 2:
		fPromotionRate = 0.5;
		break;
	case 3:
		fPromotionRate = 0.5*0.5;
		break;
	case 4:
		fPromotionRate = 0.5*0.5*0.5;
		break;
	default:
		fPromotionRate = 1;
		break;
	}

	//mChen comment
	//����У��
	WORD wDistributeCount = m_DistributeManage.GetCount();
	if(wDistributeCount<=0)//if (wDistributeCount < m_pGameServiceAttrib->wChairCount)//m_pGameServiceOption->wMinDistributeUser*fPromotionRate)//mChen
	{
		//������ʾ
		TCHAR szString[128] = TEXT("");
		_sntprintf(szString, CountArray(szString), TEXT("LockTimeMatch::PerformDistribute���������㣬wDistributeCount=[ %d ]"), wDistributeCount);
		//��ʾ��Ϣ
		CTraceService::TraceString(szString, TraceLevel_Normal);
		return false;
	}
		
	//�����û�
	while(true)
	{
		//��������
		CDistributeInfoArray DistributeInfoArray;

        //��ȡ�û�
		//WORD wRandCount = __max(m_pGameServiceOption->wMaxDistributeUser-m_pGameServiceOption->wMinPartakeGameUser+1,1);
		WORD wNeedCount = wDistributeCount;// m_pGameServiceAttrib->wChairCount/*+rand()%wRandCount*/; //mChen edit
		WORD wDistributeCount = m_DistributeManage.PerformDistribute(DistributeInfoArray, wNeedCount);
		
		//mChen comment
		if (wDistributeCount < wNeedCount/*m_pGameServiceOption->wMinPartakeGameUser*/)
		{
			break;
		}

		//Ѱ��λ��
		ITableFrame * pICurrTableFrame=NULL;
		for (WORD i=0;i<m_pGameServiceOption->wTableCount;i++)
		{
			//��ȡ����
			ASSERT(m_ppITableFrame[i]!=NULL);
			ITableFrame * pITableFrame=m_ppITableFrame[i];

			//mChen add
			//״̬�ж�
			//if (pITableFrame->GetGameStatus() >= GAME_STATUS_PLAY)
			//{
			//	continue;
			//}
			if (pITableFrame->IsGameStarted())
			{
				continue;
			}
			if (pITableFrame->GetNullChairCount() >= wNeedCount)
			{
				pICurrTableFrame = pITableFrame;
				break;
			}

			//mChen comment
			////״̬�ж�
			//if (pITableFrame->GetNullChairCount()==pITableFrame->GetChairCount())
			//{
			//	pICurrTableFrame=pITableFrame;
			//	break;
			//}
		}

		//�����ж�
		if(pICurrTableFrame==NULL) break;

		//�������
		bool bSitSuccess=true;
		INT_PTR nSitFailedIndex=INVALID_CHAIR;
		for(INT_PTR nIndex=0;nIndex<DistributeInfoArray.GetCount();nIndex++)
		{
			//��������
			WORD wChairID=INVALID_CHAIR;

			//��������
			for (WORD i=0;i<pICurrTableFrame->GetChairCount();i++)
			{
				if (pICurrTableFrame->GetTableUserItem(i)==NULL)
				{
					wChairID = i;
					break;
				}
			}

			//�����û�
			if (wChairID!=INVALID_CHAIR)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=DistributeInfoArray[nIndex].pIServerUserItem;
				ITableFrame * pITableFrame = NULL;

				//�û�����
				if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
				{
					WORD wTableID=pIServerUserItem->GetTableID();
					pITableFrame = m_ppITableFrame[wTableID];

					//if (pITableFrame != pICurrTableFrame)//mChen add, hack temp
					//{
					//	pITableFrame->PerformStandUpAction(pIServerUserItem);
					//}
					pITableFrame->PerformStandUpAction(pIServerUserItem);
				}

				////mChen add
				//if (pITableFrame == pICurrTableFrame)
				//{
				//	break;
				//}

				//�û�����
				if(pICurrTableFrame->PerformSitDownAction(wChairID,pIServerUserItem)==false)
				{
					bSitSuccess=false;
					nSitFailedIndex=nIndex;
					break;
				}
			}
		}

		//���½��
		if(bSitSuccess)
		{
			//�Ƴ����
			while(DistributeInfoArray.GetCount()>0)
			{
				m_DistributeManage.RemoveDistributeNode((tagDistributeNode *)DistributeInfoArray[0].pPertainNode);
				DistributeInfoArray.RemoveAt(0);
			}
		}
		else
		{
			for(INT_PTR nIndex=0;nIndex<DistributeInfoArray.GetCount();nIndex++)
			{
				if(nSitFailedIndex==nIndex)
				{
					m_DistributeManage.RemoveDistributeNode((tagDistributeNode *)DistributeInfoArray[nIndex].pPertainNode);
				}
				else
				{
					//�û�����
					if (DistributeInfoArray[nIndex].pIServerUserItem->GetTableID()!=INVALID_TABLE)
					{
						WORD wTableID=DistributeInfoArray[nIndex].pIServerUserItem->GetTableID();
						m_ppITableFrame[wTableID]->PerformStandUpAction(DistributeInfoArray[nIndex].pIServerUserItem);
					}
				}
			}
		}		
	}

	return true;
}

//�����û�
bool CLockTimeMatch::InsertDistribute(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//����״̬
	if (m_MatchStatus != MatchStatus_Signingup && m_MatchStatus != MatchStatus_Playing)//if (m_MatchStatus != MatchStatus_Playing)//mChen edit
	{
		return false;
	}

	//״̬�ж�
	ASSERT(pIServerUserItem->GetUserStatus()<US_PLAYING);
	if (pIServerUserItem->GetUserStatus()>=US_PLAYING) return false;

	//����У��
	if(VerifyUserEnterMatch(pIServerUserItem)==false) return false;

	//��������
	tagDistributeInfo DistributeInfo;
	ZeroMemory(&DistributeInfo,sizeof(DistributeInfo));

	//���ñ���
	DistributeInfo.pPertainNode=NULL;	
	DistributeInfo.dwInsertStamp = (DWORD)time(NULL);
	DistributeInfo.pIServerUserItem=pIServerUserItem;   

	//����ڵ�
	if(m_DistributeManage.InsertDistributeNode(DistributeInfo)==false)
	{
		return pIServerUserItem->IsAndroidUser()?false:true; 
	}

	return true;
}

//������
void CLockTimeMatch::ClearSameTableUser(DWORD dwUserID)
{
	//��������
	tagSameTableInfo * pSameTableInfo = m_DistributeManage.GetUserSameTableInfo(dwUserID);
	if(pSameTableInfo!=NULL)
	{
		pSameTableInfo->wPlayerCount=0;
		ZeroMemory(pSameTableInfo->wPlayerIDSet,sizeof(pSameTableInfo->wPlayerIDSet));
	}
}

//�����û�
void CLockTimeMatch::InserSameTableUser(DWORD dwUserID,DWORD dwTableUserID)
{
	//��������
	tagSameTableInfo * pSameTableInfo = m_DistributeManage.GetUserSameTableInfo(dwUserID);
	if(pSameTableInfo!=NULL)
	{
		//����û�
		pSameTableInfo->wPlayerIDSet[pSameTableInfo->wPlayerCount++] = dwTableUserID;
	}

	return;
}

//�¼�У��
bool CLockTimeMatch::VerifyMatchTime(LPTSTR pszMessage,WORD wMaxCount,WORD & wMessageType)
{
	//��������
	CTime CurTime=CTime::GetCurrentTime();
	CTime MatchStartTime(m_pMatchOption->MatchType.LockTime.MatchStartTime);
	CTime MatchEndTime(m_pMatchOption->MatchType.LockTime.MatchEndTime);

	//��������
	wMessageType = SMT_EJECT|SMT_CHAT;

	//mChen comment
	////����δ��ʼ
	//if(CurTime<MatchStartTime)
	//{
	//	wMessageType |= SMT_CLOSE_ROOM;
	//	_sntprintf(pszMessage,wMaxCount,TEXT("������δ��ʼ,������%d��%d��%d��-%dʱ%d��%d��ǰ���μӱ�����"),MatchStartTime.GetYear(),MatchStartTime.GetMonth(),MatchStartTime.GetDay(),MatchStartTime.GetHour(),MatchStartTime.GetMinute(),MatchStartTime.GetSecond());
	//	return false;
	//}

	//mChen comment
	////�����ѽ���
	//if(CurTime>=MatchEndTime)
	//{
	//	wMessageType |= SMT_CLOSE_ROOM|SMT_CLOSE_GAME;
	//	_sntprintf(pszMessage,wMaxCount,TEXT("�����Ѿ�����,�����Ƽ����������������������ɣ�"));
	//	return false;
	//}

	////mChen add
	//int deltaDay = CurTime.GetDay() - MatchStartTime.GetDay();
	//if (deltaDay < 0 || deltaDay % 7 != 0)
	//{
	//	wMessageType |= SMT_CLOSE_ROOM | SMT_CLOSE_GAME;
	//	_sntprintf(pszMessage, wMaxCount, TEXT("����û�б���,��������%d���μӱ�����"), MatchStartTime.GetDayOfWeek()-1);
	//	return false;
	//}

	//mChen comment
	///*
	//����δ��ʼ
	if(CurTime.GetHour()*3600+CurTime.GetMinute()*60+CurTime.GetSecond()<
	  m_pMatchOption->MatchType.LockTime.MatchStartTime.wHour*3600+m_pMatchOption->MatchType.LockTime.MatchStartTime.wMinute*60+m_pMatchOption->MatchType.LockTime.MatchStartTime.wSecond)
	{
		wMessageType |= SMT_CLOSE_ROOM|SMT_CLOSE_GAME;
		_sntprintf(pszMessage,wMaxCount,TEXT("����ı�����δ��ʼ,������%dʱ%d��%d�����μӱ�����"),MatchStartTime.GetHour(),MatchStartTime.GetMinute(),MatchStartTime.GetSecond());
		return false;
	}
	//*/

	//mChen comment
	//�����ѽ���
	if(CurTime.GetHour()*3600+CurTime.GetMinute()*60+CurTime.GetSecond()>= m_pMatchOption->MatchType.LockTime.MatchEndTime.wHour*3600+m_pMatchOption->MatchType.LockTime.MatchEndTime.wMinute*60+m_pMatchOption->MatchType.LockTime.MatchEndTime.wSecond)
	{
		wMessageType |= SMT_CLOSE_ROOM|SMT_CLOSE_GAME;
		_sntprintf(pszMessage,wMaxCount,TEXT("����ı����Ѿ�����,����������%dʱ%d��%d�����μӱ�����"),MatchStartTime.GetHour(),MatchStartTime.GetMinute(),MatchStartTime.GetSecond());
		return false;
	}

	return true;
}

//�����û�����
bool CLockTimeMatch::SendMatchUserScore(IServerUserItem * pIServerUserItem)
{
	//��������
	CMD_GR_UserScore UserScore;
	ZeroMemory(&UserScore,sizeof(UserScore));

	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();

	//��������
	UserScore.dwUserID=pUserInfo->dwUserID;
	UserScore.UserScore.dwWinCount=pUserInfo->dwWinCount;
	UserScore.UserScore.dwLostCount=pUserInfo->dwLostCount;
	UserScore.UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	UserScore.UserScore.dwFleeCount=pUserInfo->dwFleeCount;	
	UserScore.UserScore.dwExperience=pUserInfo->dwExperience;
	UserScore.UserScore.lLoveLiness=pUserInfo->lLoveLiness;

	//�������
	UserScore.UserScore.lGrade=pUserInfo->lGrade;
	UserScore.UserScore.lInsure=pUserInfo->lInsure;
	UserScore.UserScore.lScore=pUserInfo->lScore;
	//UserScore.UserScore.lIngot=pUserInfo->lIngot;

	return m_pIGameServiceFrame->SendData(BG_ALL_CLIENT,MDM_GR_USER,SUB_GR_USER_SCORE,&UserScore,sizeof(UserScore));
}

//�Ƴ����
bool CLockTimeMatch::RemoveDistribute(IServerUserItem * pIServerUserItem)
{
	//��֤����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�Ƴ��û�
	m_DistributeManage.RemoveDistributeNode(pIServerUserItem);

	return true;
}


//��ȡ����
WORD CLockTimeMatch::GetUserRank(IServerUserItem *pUserItem, ITableFrame *pITableFrame)
{
	//����Ч��
	ASSERT(pUserItem!=NULL);
	if(pUserItem==NULL) return INVALID_WORD;
	if (!pITableFrame)
	{
		return INVALID_WORD;
	}
	//�������
	SCORE lUserScore=pUserItem->GetUserScore();
	WORD wRank=1;
	DWORD dwUserID=0;
	IServerUserItem *pLoopUserItem=NULL;

	ASSERT(pUserItem->GetTableID()==pITableFrame->GetTableID());
	if(pUserItem->GetTableID()!=pITableFrame->GetTableID()) return INVALID_WORD;

	for(WORD i=0; i<m_pGameServiceAttrib->wChairCount; i++)
	{
		pLoopUserItem=pITableFrame->GetTableUserItem(i);
		if(pLoopUserItem==NULL) continue;

		if(pLoopUserItem!=pUserItem && pLoopUserItem->GetUserScore()>lUserScore)
			wRank++;
	}
	return wRank;
}
//���ͱ�����Ϣ
void CLockTimeMatch::SendTableUserMatchInfo(ITableFrame *pITableFrame, WORD wChairID)
{
	//��֤����
	ASSERT(pITableFrame!=NULL);
	if (pITableFrame==NULL) return;

	//��������
	CMD_GR_Match_Info MatchInfo;
	ZeroMemory(&MatchInfo,sizeof(MatchInfo));

	//���ñ���
	_sntprintf(MatchInfo.szTitle[0], CountArray(MatchInfo.szTitle[0]),TEXT("���ַ�ʽ���ۼƻ���"));
	_sntprintf(MatchInfo.szTitle[1], CountArray(MatchInfo.szTitle[1]),TEXT("��Ϸ������%d"), m_pMatchOption->MatchType.LockTime.lMatchInitScore);
	_sntprintf(MatchInfo.szTitle[2], CountArray(MatchInfo.szTitle[2]),TEXT("�������ƣ���ʱ����"));

	for(WORD i=0; i<pITableFrame->GetChairCount(); i++)
	{
		//��ȡ�û�
		if(wChairID!=INVALID_CHAIR && wChairID!=i) continue;
		IServerUserItem *pIServerUserItem=pITableFrame->GetTableUserItem(i);
		if(pIServerUserItem==NULL) continue;

		//���þ���
		MatchInfo.wGameCount=(WORD)pIServerUserItem->GetUserPlayCount()+1;
		MatchInfo.wRank=GetUserRank(pIServerUserItem,pITableFrame);

		//��������
		if (pIServerUserItem->IsAndroidUser()==false)
		{
			pITableFrame->SendTableData(i,SUB_GR_MATCH_INFO,&MatchInfo,sizeof(MatchInfo),MDM_GR_MATCH);
		}
	}

	return;
}

//д�뽱��
bool CLockTimeMatch::WriteUserAward(IServerUserItem *pIServerUserItem,tagMatchRankInfo * pMatchRankInfo)
{
	//����Ч��
	ASSERT(pIServerUserItem!=NULL && pMatchRankInfo!=NULL);
	if(pIServerUserItem==NULL || pMatchRankInfo==NULL) return false;

	//��������
	CMD_GR_MatchResult MatchResult;
	ZeroMemory(&MatchResult,sizeof(MatchResult));

	//��������
	MatchResult.lGold=pMatchRankInfo->lRewardGold;
	MatchResult.dwIngot=pMatchRankInfo->dwRewardIngot;
	MatchResult.dwExperience=pMatchRankInfo->dwRewardExperience;

	//����ʾ
	_sntprintf(MatchResult.szDescribe,CountArray(MatchResult.szDescribe),TEXT("%s, ��ϲ����%s�л�õ�%d��,�������£�"),
	pIServerUserItem->GetNickName(),m_pMatchOption->szMatchName,pMatchRankInfo->wRankID);

	//�����û���Ϣ
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	//pUserInfo->lIngot+=pMatchRankInfo->dwRewardIngot;
	pUserInfo->dwExperience+=pMatchRankInfo->dwRewardExperience;
	OnEventUserItemScore(pIServerUserItem, 0);

	//��������
	if (pIServerUserItem->IsAndroidUser()==false)
	{
		m_pIGameServiceFrame->SendData(pIServerUserItem, MDM_GR_MATCH, SUB_GR_MATCH_RESULT, &MatchResult, sizeof(MatchResult));
	}

	//д���¼
	if(m_pIDataBaseEngine!=NULL)
	{
		//��������
		DBR_GR_MatchReward MatchReward;
		ZeroMemory(&MatchReward,sizeof(MatchReward));
		
		//����ṹ
		MatchReward.dwUserID=pIServerUserItem->GetUserID();	
		MatchReward.lRewardGold=pMatchRankInfo->lRewardGold;
		MatchReward.dwRewardIngot=pMatchRankInfo->dwRewardIngot;
		MatchReward.dwRewardExperience=pMatchRankInfo->dwRewardExperience;
		MatchReward.dwClientAddr=pIServerUserItem->GetClientAddr();

		//Ͷ������
		m_pIDataBaseEngine->PostDataBaseRequest(MatchReward.dwUserID,DBR_GR_MATCH_REWARD,0,&MatchReward,sizeof(MatchReward));
	}

	return true;
}
//��������
bool CLockTimeMatch::SendMatchInfo(IServerUserItem * pIServerUserItem)
{
	CMD_GR_Match_Num MatchNum;
	ZeroMemory(&MatchNum,sizeof(MatchNum));

	MatchNum.dwWaitting=m_pIServerUserManager->GetUserItemCount();
	MatchNum.dwTotal=m_pGameServiceAttrib->wChairCount;

	if(pIServerUserItem==NULL)
	{
		m_pIGameServiceFrame->SendData(BG_ALL_CLIENT,MDM_GR_MATCH,SUB_GR_MATCH_NUM,&MatchNum, sizeof(MatchNum));
		return true;
	}

	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_MATCH,SUB_GR_MATCH_NUM,&MatchNum, sizeof(MatchNum));
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
