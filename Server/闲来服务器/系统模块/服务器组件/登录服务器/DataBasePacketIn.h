#ifndef DATA_BASE_PACKETIN_HEAD_FILE
#define DATA_BASE_PACKETIN_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�������ݰ�

//��¼����
#define	DBR_GP_LOGON_GAMEID			1									//I D ��¼
#define	DBR_GP_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define DBR_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�

//�˺ŷ���
#define DBR_GP_MODIFY_MACHINE		10									//�޸Ļ���
#define DBR_GP_MODIFY_LOGON_PASS	11									//�޸�����
#define DBR_GP_MODIFY_INSURE_PASS	12									//�޸�����
#define DBR_GP_MODIFY_UNDER_WRITE	13									//�޸�ǩ��
#define DBR_GP_MODIFY_INDIVIDUAL	14									//�޸�����
#define DBR_GP_MODIFY_ACCOUNTS		15									//�޸��ʺ�
#define DBR_GP_MODIFY_SPREADER		16									//�޸��Ƽ���
#define DBR_GP_QUERY_SPREADER		17									//��ѯ�Ƽ����ǳ�
#define DBR_GP_ADD_SPREADER			18									//����Ƽ���

//mChen add
#define DBR_GP_ADDDEL_SPREADER		19									//����/ɾ���Ƽ������
#define DBR_GP_QUERY_SPREADERS_INFO	22									//��ѯ�������б�
#define DBR_GP_ADD_PAYMENT			25									//�ڹ�
#define DBR_GP_QUERY_CHILDREN_PAYMENT_INFO		26							//�����û�������Ϣ
#define DBR_GP_ADD_ENTERPRISE_PAYMENT			27							//��ҵ����
#define DBR_GP_ADD_ENTERPRISE_PAYMENT_RESULT	28							//��ҵ���ֽ��

//mChen add, for HideSeek
#define DBR_GP_BOUGHT_TAGGER_MODEL				29	

//ͷ������
#define DBR_GP_MODIFY_SYSTEM_FACE	20									//�޸�ͷ��
#define DBR_GP_MODIFY_CUSTOM_FACE	21									//�޸�ͷ��

//��������
#define DBR_GP_USER_SAVE_SCORE		30									//������Ϸ��
#define DBR_GP_USER_TAKE_SCORE		31									//��ȡ��Ϸ��
#define DBR_GP_USER_TRANSFER_SCORE	32									//ת����Ϸ��

//��ѯ����
#define DBR_GP_QUERY_INDIVIDUAL		40									//��ѯ����
#define DBR_GP_QUERY_INSURE_INFO	41									//��ѯ����
#define DBR_GP_QUERY_USER_INFO	    42									//��ѯ�û�
#define DBR_GP_QUERY_ACCOUNTINFO	44									//��ѯ����
#define DBR_GP_USER_INGAME_SERVERID	45									//��Ϸ״̬


//ϵͳ����
#define DBR_GP_LOAD_GAME_LIST		50									//�����б�
#define DBR_GP_ONLINE_COUNT_INFO	51									//������Ϣ

//��������
#define DBR_GP_MATCH_SIGNUP			55									//��������
#define DBR_GP_MATCH_UNSIGNUP		56									//ȡ������
#define DBR_GP_MATCH_AWARD			57									//��������

//ǩ������
#define DBR_GP_LOAD_CHECKIN_REWARD	60									//ǩ������
#define DBR_GP_CHECKIN_DONE			61									//ִ��ǩ��
#define DBR_GP_CHECKIN_QUERY_INFO	62									//��ѯ��Ϣ
#define DBR_GP_CHECKIN_AWARD		64									//��ѯ��Ϣ

//mChen add
#define DBR_GP_RAFFLE_DONE			65									//ִ�г齱

#define DBR_GP_LOAD_BASEENSURE		70									//���صͱ�

#define DBR_GP_LOAD_BEGINNER		80									//������������
#define DBR_GP_BEGINNER_QUERY_INFO	81									//��ѯ��������
#define DBR_GP_BEGINNER_DONE		82									//��ȡ��������

//�ͱ�����
#define DBR_GP_BASEENSURE_TAKE		100									//��ȡ�ͱ�

//׬������
#define DBR_GP_LOAD_ADDRANK_CONFIG	110									//����
#define DBO_GP_ADDRANK_UP_AWARD		111									//�Ƿ���ȡ���н���		
#define DBR_GP_GET_ADDRANK			112									//�������

//��Ϸ��¼
#define DBR_GP_GAME_RECORD_LIST		120									//��Ϸ��¼
#define DBR_GP_GAME_TOTAL_RECORD	121									//��Ϸ�ط�

//�һ�����	
#define DBR_GP_GET_EXCHANGE_HUAFEI			130									//��öһ������б�

//�̳�	
#define DBR_GP_GET_SHOPINFO			140									//����̳��б�

//�Զ����ֶβ�ѯ
#define DBR_GP_PUBLIC_NOTIC			200									//�Զ����ֶβ�ѯ

#define DBR_GP_QUERY_TOP_PLAYERS	201									//lin add ����ѯ�������ڱ����������а�

#define DBR_GP_TRANSFER_DIAMOND		202									//lin add  (ת����)
#define DBR_GP_QUERY_NICKNAME		203									//lin add  (��ѯ�û��ǳ�)

#define DBR_GP_ADD_SHOPITEM         205                                 //��Ʒ���� WQ 
#define DBR_GP_ADD_EXCHANGSCOREINFO                207                   //��ʯ��Ҷһ�
//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct DBR_GP_LogonGameID
{
	//��¼��Ϣ
	DWORD							dwGameID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_GP_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szUid[LEN_ADDRANK];
	TCHAR							szOpenid[LEN_ACCOUNTS];
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���

	//mChen add, for Match Time
	WORD							wKindID;
};

//�ο͵�¼
struct DBR_GP_LogonVisitor
{
	DWORD							dwClientIP;							//���ӵ�ַ
	TCHAR							szComputerID[LEN_MACHINE_ID];		//��������
	TCHAR							szNickName[LEN_ACCOUNTS];			//����
	TCHAR							szPassWord[LEN_MD5];				//��¼����
	TCHAR							szPassWordBank[LEN_MD5];			//��¼����
	DWORD							dwSpreaderID;						//mChen edit,TCHAR	szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	WORD							wFaceID;							//ע��ͷ��
	BYTE							cbGender;							//�û��Ա�
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺ�ע��
struct DBR_GP_RegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	WORD                            wChannleID;                         //����ID
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szUid[LEN_ADDRANK];
	TCHAR							szOpenid[LEN_ACCOUNTS];
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	DWORD							dwSpreaderID;						//mChen edit,TCHAR	szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���


	//mChen add, for Match Time
	WORD							wKindID;
};

//�޸Ļ���
struct DBR_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct DBR_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct DBR_GP_ModifyAccounts
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szDesAccount[LEN_ACCOUNTS];			//�û��ʺ�
};

//�޸��Ƽ���
struct DBR_GP_ModifySpreader
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	DWORD							dwSpreaderID;						//mChen edit, TCHAR szSpreader[LEN_ACCOUNTS];			//�Ƽ���ID
};

//mChen add
//����/ɾ���Ƽ������
struct DBR_GP_AddDelSpreader
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����

	DWORD							dwSpreaderID;					//�Ƽ���ID
	DWORD							dwParentSpreaderID;				//�ϼ�����ID
	TCHAR							szSpreaderRealName[32];			//�Ƽ�������
	///TCHAR							szSpreaderIDCardNo[32];			//�Ƽ������֤��

	TCHAR							szSpreaderTelNum[32];
	TCHAR							szSpreaderWeiXinAccount[32];

	WORD							wSpreaderLevel;					//�Ƽ��˵ȼ�

	BOOL							bIsAddOperate;						//����/ɾ��������
};
//��ѯ�������б�
struct DBR_GP_QuerySpreadersInfo
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//mChen add
//�ڹ�
struct DBR_GP_AddPayment
{
	DWORD							dwUserID;						//�û� I D

	DWORD							dwPayment;						//�����Ԫ��
	DWORD							dwBoughtDiamond;				//���򵽵���ʯ��

	TCHAR							szPassword[LEN_PASSWORD];       //�û�����
};
//��Ʒ������Ϣ   WQ
struct  DBR_GP_ShopItemInfo
{
	DWORD               dwUserID;                 //�û���ʶ
	TCHAR               szUID[LEN_ADDRANK];       //�û�UID��SDK��
	TCHAR               szOrderID[LEN_USERNOTE];   //������
	WORD                wItemID;                  //��ƷID
	WORD                wAmount;                  //��Ʒ���
	WORD                wCount;                   //��Ʒ����
};
//��ʯ��Ҷһ�   WQ
struct  DBR_GP_ExchangScoreInfo
{
	DWORD               dwUserID;                 //�û���ʶ
	BYTE                cbItemID;                  //����ID
	WORD                wAmount;                  //���߽��
	BYTE                cbExchangeType;             //��������
};
//�����û�������Ϣ
struct DBR_GP_QueryChildrenPaymentInfo
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//�޸�ǩ��
struct DBR_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

//�޸�ͷ��
struct DBR_GP_ModifySystemFace
{
	//�û���Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�ͷ��
struct DBR_GP_ModifyCustomFace
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyIndividual
{
	//��֤����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����

	//�ʺ�����
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//�û���Ϣ
	TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�绰����
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

	//��ϵ����
	TCHAR							szQQ[LEN_QQ];						//Q Q ����
	TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
	
	TCHAR							szHeadHttp[LEN_USER_NOTE];			//ͷ��HTTP
	TCHAR							szUserChannel[LEN_NICKNAME];		//������
	WORD                            wModCost;							//�޸Ļ���
	BYTE							cbModCosttType;						//�������ͣ�0-��ң�1-��ʯ
};

//��ѯ�û���Ϸ״̬
struct DBR_GP_QueryUserInGameServerID
{
	DWORD							dwUserID;							//�û� I D
};

//��ѯ����
struct DBR_GP_QueryIndividual
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��ѯ������Ϣ
struct DBR_GP_QueryAccountInfo
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//������Ϸ��
struct DBR_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ����Ϸ��
struct DBR_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ��Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת����Ϸ��
struct DBR_GP_UserTransferScore
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	BYTE                            cbByNickName;                       //�ǳ�����
	SCORE							lTransferScore;						//ת����Ϸ��
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ����
struct DBR_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��ѯ�û�
struct DBR_GP_QueryInsureUserInfo
{
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};

//�û�����
struct DBO_GP_UserTransferUserInfo
{
	DWORD							dwGameID;							//�û� I D
	TCHAR							szNickName[LEN_NICKNAME];			//�ʺ��ǳ�
};

//������Ϣ
struct DBR_GP_OnLineCountInfo
{
	WORD							wKindCount;							//������Ŀ
	DWORD							dwOnLineCountSum;					//��������
	tagOnLineInfoKind				OnLineCountKind[MAX_KIND];			//��������
};

//��ѯǩ��
struct DBR_GP_CheckInQueryInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};

//ִ��ǩ��
struct DBR_GP_CheckInDone
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//mChen add
//ִ�г齱
struct DBR_GP_RaffleDone
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwRaffleGold;						//�鵽����ʯ

	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
															//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

////////////////////////////////////////////////////////////////////////////////////////////
//����������ѯ
struct DBR_GP_BeginnerQueryInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};
struct DBR_GP_BeginnerDone
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

////////////////////////////////////////////////////////////////////////////////////////////

//�ͱ�����
struct DBO_GP_BaseEnsureParameter
{
	SCORE							lScoreCondition;					//��Ϸ������
	SCORE							lScoreAmount;						//��Ϸ������
	BYTE							cbTakeTimes;						//��ȡ����	
};

//�ͱ����
struct DBO_GP_BaseEnsureResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lGameScore;							//��ǰ��Ϸ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//��ȡ�ͱ�
struct DBR_GP_TakeBaseEnsure
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�������
struct DBO_GP_MatchSignupResult
{
	bool							bSignup;							//������ʶ
	bool							bSuccessed;							//�ɹ���ʶ

	//mChen add
	SYSTEMTIME MatchSignupStartTime;
	SYSTEMTIME MatchSignupEndTime;
	SYSTEMTIME MatchStartTime;
	SYSTEMTIME MatchEndTime;

	TCHAR							szDescribeString[128];				//������Ϣ
};


//���ؽ���
struct DBR_GR_LoadMatchReward
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������	
};

//�޸Ļ���
struct DBR_GP_PublicNotic
{
	TCHAR							szKeyName[LEN_NICKNAME];			//�ؼ���
};

/////////////////////////////////////
//׬���
struct DBR_GP_GetAddBankConfig
{
	INT								iIdex;
};
//������а�
struct DBR_GP_GetAddBank
{
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	INT								iIdex;
};


//��������
struct DBR_GP_MatchSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��������
struct DBR_GP_MatchUnSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};


//��Ϸ��¼
struct DBR_GP_GameRecordList
{
	DWORD							dwUserID;							
};


//��Ϸ�ط�
struct DBR_GP_GetGameTotalRecord
{
	DWORD							dwUserID;		
	DWORD							dwRecordID;							
};

//���Ѷһ�
struct DBR_GP_GetExchangeHuaFei
{
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
};

//���Ѷһ�
struct DBR_GP_GetShopInfo
{
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
};

#endif