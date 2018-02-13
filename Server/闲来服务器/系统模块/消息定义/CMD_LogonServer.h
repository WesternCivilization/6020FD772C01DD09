#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#include "..\..\ȫ�ֶ���\datastream.h"


#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//��¼����

#define MDM_GP_LOGON				1									//�㳡��¼

//��¼ģʽ
#define SUB_GP_LOGON_GAMEID			1									//I D ��¼
#define SUB_GP_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define SUB_GP_LOGON_VISITOR		5									//�ο͵�¼

//��¼���
#define SUB_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GP_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_GP_LOGON_FINISH			102									//��¼���
#define SUB_GP_VALIDATE_MBCARD      103                                 //��¼ʧ��
#define SUB_GP_MATCH_SIGNUPINFO		106									//������Ϣ

//������ʾ
#define SUB_GP_UPDATE_NOTIFY		200									//������ʾ

//////////////////////////////////////////////////////////////////////////////////
//

#define MB_VALIDATE_FLAGS           0x01                                //Ч���ܱ�
#define LOW_VER_VALIDATE_FLAGS      0x02                                //Ч��Ͱ汾

//I D ��¼
struct CMD_GP_LogonGameID
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	BYTE							cbValidateFlags;			        //У���ʶ
};

//�ʺŵ�¼
struct CMD_GP_LogonAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szUid[LEN_ADDRANK];
	TCHAR							szOpenid[LEN_ACCOUNTS];

	//mChen add, for Match Time
	WORD							wKindID;

	BYTE							cbValidateFlags;			        //У���ʶ
};

//�ο͵�¼
struct CMD_GP_VisitorLogon
{
	WORD								wFaceID;						//ͷ���ʶ
	BYTE								cbGender;						//�û��Ա�
	DWORD								dwPlazaVersion;					//�㳡�汾
	BYTE								cbValidateFlags;			    //У���ʶ
	TCHAR								szNickName[LEN_NICKNAME];		//�û��ǳ�
	DWORD								dwSpreaderID;					//mChen edit,TCHAR	szSpreader[LEN_NICKNAME];		//�ƹ�����
	TCHAR								szPassWord[LEN_PASSWORD];		//��¼����
	TCHAR								szPassWordBank[LEN_PASSWORD];	//��¼����
};

//ע���ʺ�
struct CMD_GP_RegisterAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	WORD                            wChannleID;                         //����ID
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szUid[LEN_ADDRANK];
	TCHAR							szOpenid[LEN_ACCOUNTS];
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	DWORD							dwSpreaderID;						//mChen edit,TCHAR	szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����


	//mChen add, for Match Time
	WORD							wKindID;

	BYTE							cbValidateFlags;			        //У���ʶ
};

//��¼�ɹ�
struct CMD_GP_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	DWORD							dwSpreaderID;						//�ƹ�ID
	BYTE							cbInsureEnabled;					//���п�ͨ
	
	//�û��ɼ�
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//�û�����

	//�û���Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//������Ϣ
	BYTE                            cbShowServerStatus;                 //��ʾ������״̬

	//mChen add, for Match Time
	SYSTEMTIME						MatchStartTime;
	SYSTEMTIME						MatchEndTime;

	//mChen add, forǩ��
	WORD wSeriesDate;

	//mChen add, �Ѵ򳡴�,for�齱
	DWORD dwPlayCount;

	//mChen add,�齱��¼
	DWORD dwRaffleCount;
	DWORD dwPlayCountPerRaffle;

	//mChen add, ����
	INT iSpreaderLevel;	//����ȼ��� -1:���Ǵ�����

	//mChen add, for HideSeek:��ѯ����ģ�Ϳ�
	SCORE	lModelIndex0;

	BYTE							cbGPIsForAppleReview;

	//WQ ͷ��Http
	TCHAR           szHeadHttp[LEN_USER_NOTE];

	//mChen add,������Ϣ
	TCHAR							szPublicNotice[LEN_USER_NOTE];
};

//��¼ʧ��
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��½���
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//�ж�ʱ��
	WORD							wOnLineCountTime;					//����ʱ��
};

//��¼ʧ��
struct CMD_GP_ValidateMBCard
{
	UINT								uMBCardID;						//��������
};

//������ʾ
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//������Ϣ
#define DTP_GP_MEMBER_INFO			2									//��Ա��Ϣ
#define	DTP_GP_UNDER_WRITE			3									//����ǩ��
#define DTP_GP_STATION_URL			4									//��ҳ��Ϣ

//������Ϣ
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//��������
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
};

//��Ա��Ϣ
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��
};

//////////////////////////////////////////////////////////////////////////////////
//�б�����

#define MDM_GP_SERVER_LIST			2									//�б���Ϣ

//��ȡ����
#define SUB_GP_GET_LIST				1									//��ȡ�б�
#define SUB_GP_GET_SERVER			2									//��ȡ����
#define SUB_GP_GET_ONLINE			3									//��ȡ����
#define SUB_GP_GET_COLLECTION		4									//��ȡ�ղ�

//�б���Ϣ
#define SUB_GP_LIST_TYPE			100									//�����б�
#define SUB_GP_LIST_KIND			101									//�����б�
#define SUB_GP_LIST_NODE			102									//�ڵ��б�
#define SUB_GP_LIST_PAGE			103									//�����б�
#define SUB_GP_LIST_SERVER			104									//�����б�
#define SUB_GP_LIST_MATCH			105									//�����б�
#define SUB_GP_VIDEO_OPTION			106									//��Ƶ����

//mChen add. for HideSeek
#define SUB_GP_LIST_LOBBY			107									//�����б�

//�����Ϣ
#define SUB_GP_LIST_FINISH			200									//�������
#define SUB_GP_SERVER_FINISH		201									//�������

//������Ϣ
#define SUB_GR_KINE_ONLINE			300									//��������
#define SUB_GR_SERVER_ONLINE		301									//��������

//////////////////////////////////////////////////////////////////////////////////

//��ȡ����
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//������Ŀ
	WORD							wOnLineServerID[MAX_SERVER];		//�����ʶ
};

//��������
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//������Ŀ
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//��������
};

//��������
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//������Ŀ
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//��������
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GP_USER_SERVICE			3									//�û�����

//�˺ŷ���
#define SUB_GP_MODIFY_MACHINE		100									//�޸Ļ���
#define SUB_GP_MODIFY_LOGON_PASS	101									//�޸�����
#define SUB_GP_MODIFY_INSURE_PASS	102									//�޸�����
#define SUB_GP_MODIFY_UNDER_WRITE	103									//�޸�ǩ��
#define SUB_GP_MODIFY_ACCOUNTS		104									//�޸��ʺ�
#define SUB_GP_MODIFY_SPREADER		105									//�޸��Ƽ���

//mChen add
//����
#define SUB_GP_ADDDEL_SPREADER		106									//����/ɾ���Ƽ������
#define SUB_GP_QUERY_SPREADERS_INFO 107									//��ѯ�������б�
//�ڹ�
#define SUB_GP_ADD_PAYMENT			115									//��Ϸ�ڹ�
#define SUB_GP_QUERY_CHILDREN_PAYMENT_INFO 125							//�����û�������Ϣ
#define SUB_GP_QUERY_PREPAYID   126										//��ѯ΢��PrepayID�����Ϣ
#define SUB_GP_UPLOAD_PAY_INFO	127										//�ͻ���΢��֧����Ϣ
//��ҵ����
#define SUB_GP_ADD_ENTERPRISE_PAYMENT  128

//mChen add, for HideSeek
#define SUB_GP_BOUGHT_TAGGER_MODEL  129

#define SUB_GP_QUERY_SPREADER		110									//��ѯ�Ƽ����ǳ�
#define SUB_GP_ADD_SPREADER			111									//����Ƽ���
#define	SUB_GP_QUERY_NICKNAME		116									//��ѯ�û��ǳ�
#define SUB_GP_NICKNAME_INFO		117									//��ѯ�û��ǳƷ�������
#define SUB_GP_TRANSFER_DIAMOND		118									//ת����
#define SUB_GP_TRANSFER_DIAMOND_RESULT	119								//ת������������
//WQ ��Ʒ
#define SUB_GP_ADDSHOPITEM             150                              //������Ʒ
#define SUB_GP_ADDSHOPITEM_RESULT      151                              //������Ʒ��������
//WQ ��ʯ��Ҷһ�
#define SUB_GP_EXCHANGESCORE             154                              //��������
#define SUB_GP_EXCHANGESCORE_RESULT      155                              //�������ѷ�������

//��ѯ�Ƽ����ǳ� //����Ƽ���
struct CMD_GP_QuerySpreader
{
	DWORD							dwUserID;
	DWORD							dwSpreaderID;
};
//�Ƽ����ǳƷ���
struct CMD_GP_QuerySpreader_Reslut		
{
	BYTE							byType;								//Ϊ���ʾ�ɹ�
	DWORD							dwSpreaderID;
	TCHAR							szNickName[LEN_ACCOUNTS];
	TCHAR							szReslut[128];						//ʧ�ܱ�ʾ
};
//����Ƽ��˷���
struct CMD_GP_AddSpreader_Result
{
	BYTE							byType;								//Ϊ���ʾ�ɹ�
	DWORD							dwSpreaderID;						//�ƹ���ID
	TCHAR							szReslut[128];						//ʧ�ܱ�ʾ
};
//�޸�ͷ��
#define SUB_GP_USER_FACE_INFO		120									//ͷ����Ϣ
#define SUB_GP_SYSTEM_FACE_INFO		121									//ϵͳͷ��
#define SUB_GP_CUSTOM_FACE_INFO		122									//�Զ�ͷ��

//��������
#define SUB_GP_MATCH_SIGNUP			200									//��������
#define SUB_GP_MATCH_UNSIGNUP		201									//ȡ������
#define SUB_GP_MATCH_SIGNUP_RESULT	202									//�������
#define SUB_GP_MATCH_AWARD_LIST		203									//��������
//lin : add
#define SUB_GP_MATCH_TOP_LIST		204									//��ѯ������������
#define SUB_GP_TOP_PLAYERS_INFO_RESOULT	205								//�����������з�������

//ǩ������
#define SUB_GP_CHECKIN_QUERY		220									//��ѯǩ��
#define SUB_GP_CHECKIN_INFO			221									//ǩ����Ϣ
#define SUB_GP_CHECKIN_DONE			222									//ִ��ǩ��
#define SUB_GP_CHECKIN_RESULT		223									//ǩ�����
#define SUB_GP_CHECKIN_AWARD		224									//ǩ������

//mChen add
//�齱����
#define SUB_GP_RAFFLE_DONE			230
#define SUB_GP_RAFFLE_RESULT		231

//��������
#define SUB_GP_BEGINNER_QUERY		240									//��������ǩ��
#define SUB_GP_BEGINNER_INFO		241									//����������Ϣ
#define SUB_GP_BEGINNER_DONE		242									//��������ִ��
#define SUB_GP_BEGINNER_RESULT		243									//�����������

//�ͱ�����
#define SUB_GP_BASEENSURE_LOAD			260								//���صͱ�
#define SUB_GP_BASEENSURE_TAKE			261								//��ȡ�ͱ�
#define SUB_GP_BASEENSURE_PARAMETER		262								//�ͱ�����
#define SUB_GP_BASEENSURE_RESULT		263								//�ͱ����

//��������
#define SUB_GP_USER_INDIVIDUAL		301									//��������
#define	SUB_GP_QUERY_INDIVIDUAL		302									//��ѯ��Ϣ
#define SUB_GP_MODIFY_INDIVIDUAL	303									//�޸�����
#define SUB_GP_QUERY_ACCOUNTINFO	304									//������Ϣ
#define SUB_GP_QUERY_INGAME_SEVERID	305									//��Ϸ״̬

//���з���
#define SUB_GP_USER_SAVE_SCORE		400									//������
#define SUB_GP_USER_TAKE_SCORE		401									//ȡ�����
#define SUB_GP_USER_TRANSFER_SCORE	402									//ת�˲���
#define SUB_GP_USER_INSURE_INFO		403									//��������
#define SUB_GP_QUERY_INSURE_INFO	404									//��ѯ����
#define SUB_GP_USER_INSURE_SUCCESS	405									//���гɹ�
#define SUB_GP_USER_INSURE_FAILURE	406									//����ʧ��
#define SUB_GP_QUERY_USER_INFO_REQUEST	407								//��ѯ�û�
#define SUB_GP_QUERY_USER_INFO_RESULT	408								//�û���Ϣ

//�Զ����ֶβ�ѯ ����
#define SUB_GP_QUERY_PUBLIC_NOTICE		500									//�Զ����ֶβ�ѯ
#define	SUB_GP_PUBLIC_NOTICE			501

//�����Ƽ��˽��
#define SUB_GP_SPREADER_RESOULT			520									//�����Ƽ��˽��

//mChen add
//��ѯ�������б���
#define SUB_GP_SPREADERS_INFO_RESOULT	521

//mChen add
//�ڹ�
#define SUB_GP_ADD_PAYMENT_RESULT		530
//�����û�������Ϣ
#define SUB_GP_CHILDREN_PAYMENT_INFO_RESULT		531
//��ҵ����
#define SUB_GP_ADD_ENTERPRISE_PAYMENT_RESULT	532

//mChen add, for HideSeek
#define SUB_GP_BOUGHT_TAGGER_MODEL_RESULT		533

//׬�����а�
#define SUB_GP_ADDRANK_GET_AWARD_INFO	540									//��ý�����Ϣ
#define SUB_GP_ADDRANK_BACK_AWARD_INFO	541									//���ؽ�����Ϣ
#define SUB_GP_ADDRANK_GET_RANK			542									//�������
#define SUB_GP_ADDRANK_BACK_RANK		543									//��������

//��Ϸ��¼
#define SUB_GP_GAME_RECORD_LIST			550			
#define SUB_GP_GAME_RECORD_TOTAL		551									


//�������
#define SUB_GP_OPERATE_SUCCESS			900									//�����ɹ�
#define SUB_GP_OPERATE_FAILURE			901									//����ʧ��

//���Ѷһ�
#define  SUB_GP_EXCHANGEHUAFEI_GET_LIST_INFO    1000                    //��ȡ�һ������б�
#define  SUB_GP_EXCHANGEHUAFEI_BACK				1001					//�һ������б���

//�̳�����
#define  SUB_GP_SHOPINFO_GET_LIST_INFO		1100                     //��ȡ�̳��б�
#define  SUB_GP_SHOPINFO_BACK				1101						//�̳��б���

//��Ϸ��¼
struct CMD_GP_GetGameRecord_List
{
	DWORD							dwUserID;							
};

//��Ϸ��¼
struct CMD_GP_GetGameTotalRecord
{
	DWORD							dwUserID;			
	DWORD							dwRecordID;						
};



struct CMD_GP_BackAddBankAwardInfo
{
	INT							kRewardGold[3][LEN_ADDRANK];				
	INT							kRewardType[3][LEN_ADDRANK];				
};

struct CMD_GP_GetAddBank
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	INT								iRankIdex;
};


struct CMD_GP_BackAddBank
{
	int iRankIdex;
	std::vector<std::string>		kNickName;
	std::vector<int>				kUserID;
	std::vector<int>				kFaceID;
	std::vector<int>				kCustomID;
	std::vector<int>				kUserPoint;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(iRankIdex);
		Stream_VALUE(kNickName);
		Stream_VALUE(kUserID);
		Stream_VALUE(kFaceID);
		Stream_VALUE(kCustomID);
		Stream_VALUE(kUserPoint);
	}
};

//���Ѷһ�

struct CMD_GP_GetExchangeHuaFei
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

struct CMD_GP_BackExchangeHuaFei
{
	std::vector<int>				kExchangeID;            //�һ�id
	std::vector<int>				kUseType;			   //�һ���������
	std::vector<int>				kUseNum;				   //�һ����߸���
	std::vector<int>				kGetType;			   //�һ���Ʒ����
	std::vector<int>				kGetNum;				   //�һ���Ʒ����
	std::vector<std::string>		kGoodsName;			   //�һ���Ʒ����
	std::vector<std::string>		kExchangeDesc;		   //�һ�����
	std::vector<std::string>		kImgIcon;			   //ͼ������
	std::vector<int>				kFlag;				   //���

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(kExchangeID);
		Stream_VALUE(kUseType);
		Stream_VALUE(kUseNum);
		Stream_VALUE(kGetType);
		Stream_VALUE(kGetNum);
		Stream_VALUE(kGoodsName);
		Stream_VALUE(kExchangeDesc);
		Stream_VALUE(kImgIcon);
		Stream_VALUE(kFlag);
	}
};

// �̳��б�
struct CMD_GP_GetShopInfo
{
	DWORD						dwUserID;						//�û� I D
	char							szPassword[LEN_PASSWORD];			//�û�����
};

struct CMD_GP_BackShopInfo
{
	std::vector<int>				kItemID;					 //��Ʒid
	std::vector<int>				kItemType;				 //��Ʒ����
	std::vector<int>				kOrderID_IOS;				 //��Ʒ������ ƻ��
	std::vector<int>				kOrderID_Android;			 //��Ʒ������ ��׿
	std::vector<int>				kPrice;					 //��Ʒ�۸�
	std::vector<int>				kGoodsNum;				 //��Ʒ����
	std::vector<std::string>		kItemTitle;				 //����
	std::vector<std::string>		kItemDesc;				 //����
	std::vector<std::string>		kItemIcon;				 //ͼ������
	std::vector<std::string>		kItemName;				 //��Ʒ����

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(kItemID);
		Stream_VALUE(kItemType);
		Stream_VALUE(kOrderID_IOS);
		Stream_VALUE(kOrderID_Android);
		Stream_VALUE(kPrice);
		Stream_VALUE(kGoodsNum);
		Stream_VALUE(kItemTitle);
		Stream_VALUE(kItemDesc);
		Stream_VALUE(kItemIcon);
		Stream_VALUE(kItemName);
	}
};

struct CMD_GR_QueryNickName
{
	DWORD dwUserID;                     //��ѯ��ҵ�ID
};

struct CMD_GR_TransferDiamond
{
	DWORD dwLocalID;					//���ID(ת�����)
	DWORD dwUserID;                     //��ת��ҵ�ID
	DWORD dwDiamondNum;                 //��������
};

//////////////////////////////////////////////////////////////////////////////////
//��������
struct CMD_GP_MatchSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ������
struct CMD_GP_MatchUnSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�������
struct CMD_GP_MatchSignupResult
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


//�������
struct CMD_GP_MatchGetAward
{
	DWORD							dwUserID;	
	DWORD							dwMatchID;		//������ʶ
	DWORD							dwMatchNO;		//��������	
};

//������Ϣ
struct tagMatchAwardkInfo
{
	WORD							MatchRank;							//��������
	DWORD							RewardGold;					//�������
	DWORD							RewardMedal;						//����Ԫ��
	DWORD							RewardExperience;					//��������
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(MatchRank);
		Stream_VALUE(RewardGold);
		Stream_VALUE(RewardMedal);
		Stream_VALUE(RewardExperience);
	}
};

//��������
struct CMD_GR_MatchAwardList
{
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������
	std::vector<tagMatchAwardkInfo> kAwards;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwMatchID);
		Stream_VALUE(dwMatchNO);
		StructVecotrMember(tagMatchAwardkInfo,kAwards);
	}
};

//��ѯ��������ǰn������
struct CMD_GR_QueryTopNum
{
	DWORD							dwTopCount;							//��ѯ����
};
//////////////////////////////////////////////////////////////////////////////////


//�޸�����
struct CMD_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸��Ƽ���
struct CMD_GP_ModifySpreader
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];		//�û�����
	DWORD							dwSpreaderID;					//mChen edit,�Ƽ���ID // TCHAR	szSpreader[LEN_NICKNAME];		//�û�����
};


//mChen add
//����/ɾ���Ƽ������
struct CMD_GP_AddDelSpreader
{
	DWORD							dwUserID;						//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];		//�û�����

	DWORD							dwSpreaderID;					//�Ƽ���ID
	DWORD							dwParentSpreaderID;				//�ϼ�����ID
	TCHAR							szSpreaderRealName[32];			//�Ƽ�������
	///TCHAR							szSpreaderIDCardNo[32];			//�Ƽ������֤��

	TCHAR							szSpreaderTelNum[32];
	TCHAR							szSpreaderWeiXinAccount[32];

	WORD							wSpreaderLevel;					//�Ƽ��˵ȼ�

	bool							bIsAddOperate;					//����/ɾ����
};
//��ѯ�������б�
struct CMD_GP_QuerySpreadersInfo
{
	DWORD							dwUserID;						//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];		//�û�����
};

//mChen add
//�ڹ�
struct CMD_GP_AddPayment
{
	DWORD							dwUserID;						//�û� I D

	DWORD							dwPayment;						//�����Ԫ��
	DWORD							dwBoughtDiamond;				//���򵽵���ʯ��

	TCHAR							szPassword[LEN_PASSWORD];       //�û�����
};
//��Ʒ������Ϣ   WQ
struct  CMD_GP_ShopItemInfo
{
	DWORD               dwUserID;                 //�û���ʶ
	TCHAR               szUID[LEN_ADDRANK];       //�û�UID��SDK��
	TCHAR               szOrderID[LEN_USERNOTE];   //������
	WORD                wItemID;                  //��ƷID
	WORD                wAmount;                  //��Ʒ���
	WORD                wCount;                   //��Ʒ����
};
//����������XML
struct  ShopItemInfoXML
{
	BYTE                cbTest;                    //�ж��Ƿ���Զ���
	WORD                wChannel;                  //������
	TCHAR               szUID[LEN_ADDRANK];        //����UID��SDK��
	TCHAR               szOrderID[LEN_USERNOTE];   //������
	TCHAR               szSDKOrderID[LEN_USERNOTE];//�����ţ�SDK��
	TCHAR               szPayTime[LEN_USERNOTE];   //֧��ʱ��
	FLOAT               wAmount;                   //��Ʒ���
	WORD                wPayStatus;                //֧��״̬
	WORD                wItemID;                  //��ƷID��͸��������
	WORD                wCount;                    //��Ʒ������͸��������
	DWORD               dwUserID;                  //�û���ʶ��͸��������
};
struct CMD_GP_ShopItemInfoResult    //WQ
{
	BYTE                    cbSuccess;                     //����ɹ�
	WORD                    wItemID;                       //��ƷID
	DWORD                   dwFinalInsureScore;            //��ֵ��ӵ����ʯ
	TCHAR                   szDescribeString[256];				//������Ϣ
};
//��ʯ��Ҷһ�   WQ
struct  CMD_GP_ExchangScoreInfo
{
	DWORD               dwUserID;                 //�û���ʶ
	BYTE                cbItemID;                  //����ID
	WORD                wAmount;                  //���߽��
	BYTE                cbExchangeType;             //��������   0-���ת��ʯ��1-��ʯת���
};
struct CMD_GP_ExchangScoreInfoResult    //WQ
{
	BYTE                    cbSuccess;                     //����ɹ�
	BYTE                    cbItemID;                       //��ƷID
	DWORD                   dwFinalScore;                   //��ǰ���
	DWORD                   dwFinalInsure;                   //��ǰ��ʯ
	TCHAR                   szDescribeString[256];		   //������Ϣ
};
//�����û�������Ϣ
struct CMD_GP_QueryChildrenPaymentInfo
{
	DWORD							dwUserID;						//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];		//�û�����
};
//����΢��PREPayID
struct CMD_GP_QueryPrePayID
{
	DWORD dwUserID;													//�û� I D
	DWORD dwShopItemID;												//��Ʒ I D
};
//΢��PrepayID������Ϣ
struct CMD_GP_PrePayIDInfo
{
	BYTE							cbStatusCode;					//��ȡ�ɹ�������Ϣ��1�� �ɹ���0�� ʧ�ܣ�
	TCHAR							szPrePayID[64];					//΢��֧�����ص�PrePayID (wx2017082214271068e0d93b9d0885872954���ǲ��Է��ص�id������32Ϊλ����Ϊ64��
																	//΢�ŵ���֧���ӿ��ĵ���д����ΪString��32��,��ͳһ�µ��ӿڷ��ص��ĵ���PrePayID������ΪString��64��)
	TCHAR							szNonceStr[32];					//΢��֧���µ��õ�����ַ���
	TCHAR							szTimeStamp[10];				//΢��֧���µ��õ�ʱ���
	TCHAR							szSign[32];						//΢��֧���µ��õ�ǩ���ֶ�
	TCHAR							szTradeNo[32];					//΢��֧�� �̻������ţ� �ͻ�����������֧���ɹ���ѯ��
};
//��ҵ����
struct CMD_GP_AddEnterprisePayment
{
	DWORD							dwUserID;						//�û� I D

	DWORD							dwEnterprisePayment;			//���ֽ��֣�

	TCHAR							szPassword[LEN_PASSWORD];       //�û�����
};

//mChen add, for HideSeek
struct CMD_GP_BoughtTaggerModel
{
	DWORD							dwUserID;						//�û� I D

	DWORD							dwPayment;						//������
	BYTE							cbPaymentType;					//�������ͣ�0-��ң�1-��ʯ��2-�ֽ�
	WORD							wBoughtModelIndex;				

	TCHAR							szPassword[LEN_PASSWORD];       //�û�����
};

struct CMD_GP_ClientPayInfo
{
	TCHAR							szTradeNo[32];					//΢��֧��  (�̻�)�����š����ͻ����ò���΢�Ŷ����ţ�ֻ���õ��ɹ���Ϣ
	DWORD							dwUserID;						//�û�ID
	BYTE							cbSuccessState;					//֧��״̬���ɹ�orʧ�ܣ�
};

struct CMD_GP_ClientPayInfoResoult
{
	BYTE							cbSuccessState;					//֧��״̬���ɹ�orʧ�ܣ�
	DWORD							dwInsureGold;					//֧��֮����ʯ��
	TCHAR							szMsg[64];						//֧��������Ϣ
};

//�޸��ʺ�
struct CMD_GP_ModifyAccounts
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szDesAccount[LEN_ACCOUNTS];			//�û��ʺ�
};

//�޸�ǩ��
struct CMD_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

//////////////////////////////////////////////////////////////////////////////////

//�û�ͷ��
struct CMD_GP_UserFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
};

//�޸�ͷ��
struct CMD_GP_SystemFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�ͷ��
struct CMD_GP_CustomFaceInfo
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

//�󶨻���
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserIndividual
{
	DWORD							dwUserID;							//�û� I D
};

//������Ϣ
struct CMD_GP_UserAccountInfo
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ�����
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	DWORD							dwSpreaderID;						//�ƹ�ID
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
	TCHAR							szLogonIp[LEN_ACCOUNTS];			//��¼IP

	//�û��ɼ�
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//�û�����

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//��Ա����
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��
};

//�����ɹ�
struct CMD_GP_InGameSeverID
{
	DWORD							LockKindID;			
	DWORD							LockServerID;			
};

//��ѯ��Ϣ
struct CMD_GP_QueryIndividual
{
	DWORD							dwUserID;							//�û� I D
};

//��Ϸ״̬
struct CMD_GP_UserInGameServerID
{
	DWORD							dwUserID;							//�û� I D
};


//��ѯ��Ϣ
struct CMD_GP_QueryAccountInfo
{
	DWORD							dwUserID;							//�û� I D
};


//�޸�����
struct CMD_GP_ModifyIndividual
{
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	WORD                            wModCost;                           //�޸Ļ���
	BYTE							cbModCosttType;						//�������ͣ�0-��ң�1-��ʯ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};
//////////////////////////////////////////////////////////////////////////////////
//��ѯǩ��
struct CMD_GP_CheckInQueryInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};

//ǩ����Ϣ
struct CMD_GP_CheckInInfo
{	
	WORD							wSeriesDate;						//��������
	WORD							wAwardDate;							//��Ʒ����
	bool							bTodayChecked;						//ǩ����ʶ
	SCORE							lRewardGold[LEN_SIGIN];				//�������
	BYTE							lRewardType[LEN_SIGIN];				//�������� 1��� 2����
	BYTE							lRewardDay[LEN_SIGIN];				//�������� 
};

//ִ��ǩ��
struct CMD_GP_CheckInDone
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ǩ�����
struct CMD_GP_CheckInResult
{
	bool							bType;								//�Ƿ��Ǵﵽ������ȡ��Ʒ
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lScore;								//��ǰ���
	WORD							wSeriesDate;						//��������
	TCHAR							szNotifyContent[128];				//��ʾ����
};


//mChen add
//ִ�г齱
struct CMD_GP_RaffleDone
{
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwRaffleGold;						//�鵽����ʯ

	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};
struct CMD_GP_RaffleResult
{
	bool							bSuccessed;							//�ɹ���ʶ

	SCORE							lScore;								//��ǰ����
	DWORD							dwRaffleCount;						//�ѳ齱����
	DWORD							dwPlayCount;						//�Ѵ򳡴�

	TCHAR							szNotifyContent[128];				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////////////
//���ֻ

struct CMD_GP_BeginnerQueryInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};
struct CMD_GP_BeginnerInfo
{	
	WORD							wSeriesDate;						//��������
	bool							bTodayChecked;						//ǩ����ʶ
	bool							bLastCheckIned;						//ǩ����ʶ
	SCORE							lRewardGold[LEN_BEGINNER];			//�������
	BYTE							lRewardType[LEN_BEGINNER];			//�������� 1��� 2����
};
struct CMD_GP_BeginnerDone
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

struct CMD_GP_BeginnerResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lAwardCout;							//��������
	SCORE							lAwardType;							//��������
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////////////
//�ͱ�����

//��ȡ�ͱ�
struct CMD_GP_BaseEnsureTake
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�ͱ�����
struct CMD_GP_BaseEnsureParamter
{
	SCORE							lScoreCondition;					//��Ϸ������
	SCORE							lScoreAmount;						//��Ϸ������
	BYTE							cbTakeTimes;						//��ȡ����	
};

//�ͱ����
struct CMD_GP_BaseEnsureResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lGameScore;							//��ǰ��Ϸ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};


//�Զ����ֶβ�ѯ ����
struct CMD_GP_QueryNotice
{
	TCHAR							szKeyName[LEN_NICKNAME];			//�ؼ���
};
struct CMD_GP_PublicNotice
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[512];				//�ɹ���Ϣ
};
//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_UserIndividual

#define DTP_GP_UI_NICKNAME			1									//�û��ǳ�
#define DTP_GP_UI_USER_NOTE			2									//�û�˵��
#define DTP_GP_UI_UNDER_WRITE		3									//����ǩ��
#define DTP_GP_UI_QQ				4									//Q Q ����
#define DTP_GP_UI_EMAIL				5									//�����ʼ�
#define DTP_GP_UI_SEAT_PHONE		6									//�̶��绰
#define DTP_GP_UI_MOBILE_PHONE		7									//�ƶ��绰
#define DTP_GP_UI_COMPELLATION		8									//��ʵ����
#define DTP_GP_UI_DWELLING_PLACE	9									//��ϵ��ַ
#define DTP_GP_UI_HEAD_HTTP			10									//ͷ��
#define DTP_GP_UI_IP				11									//IP
#define DTP_GP_UI_CHANNEL			12									//������

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserInsureInfo
{
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	SCORE							lTransferPrerequisite;				//ת������
};

//������
struct CMD_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ȡ���
struct CMD_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ���
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת�˽��
struct CMD_GP_UserTransferScore
{
	DWORD							dwUserID;							//�û� I D
	BYTE                            cbByNickName;                       //�ǳ�����
	SCORE							lTransferScore;						//ת�˽��
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//���гɹ�
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ȡ���
struct CMD_GP_UserTakeResult
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
};

//��ѯ����
struct CMD_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
};

//��ѯ�û�
struct CMD_GP_QueryUserInfoRequest
{
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};

//�û���Ϣ
struct CMD_GP_UserTransferUserInfo
{
	DWORD							dwTargetGameID;						//Ŀ���û�
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};
//////////////////////////////////////////////////////////////////////////////////

//����ʧ��
struct CMD_GP_OperateFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�����ɹ�
struct CMD_GP_OperateSuccess
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//�����ɹ�
struct CMD_GP_SpreaderResoult
{
	//mChen add
	BYTE							cbOperateType;	//�������ͣ�0-�󶨴���1-���Ӵ���2-ɾ������				
	DWORD							dwBindSpreaderId;//�󶨲������󶨵Ĵ���ID

	LONG							lResultCode;						//��������
	SCORE							lScore;
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//mChen add
//��ѯ�������б�
#define Max_Spreaders_Num_PerSend 400	//ÿ�η������������� 
struct SpreaderInfoItem
{
	DWORD SpreaderId;

	TCHAR RealName[32];
	//TCHAR IDCardNo[32];
	TCHAR TelNum[32];
	TCHAR WeiXinAccount[32];

	WORD SpreaderLevel;
	DWORD ParentID;
};
struct CMD_GP_SpreadersInfoResoult
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[64];				//�ɹ���Ϣ

	WORD							wPacketIdx;							//��ǰ�����
	WORD							wPacketNum;							//�ְ����͵��ܰ���

	WORD							wItemCount;
	SpreaderInfoItem				SpreaderInfoItems[Max_Spreaders_Num_PerSend];//��������Ϣ
};

//mChen add
//�ڹ�
struct CMD_GP_AddPaymentResult
{
	LONG							lResultCode;						//��������
	DWORD							dwFinalInsureScore;

	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};
//�����û�������Ϣ
#define Max_PaymentInfo_Num_PerSend 800	//ÿ�η����������Ϣ�� 
struct PaymentInfoItem
{
	DWORD UserId;
	DWORD Payment;
	DOUBLE PaymentGrantRate;
};
struct CMD_GP_ChildrenPaymentInfoResult
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[64];				//�ɹ���Ϣ

	DOUBLE							dTotalGrantOfChildrenBuy;			//�����û���ֵ��õ�����ܶԪ��
	DOUBLE							dExtraCash;							//��õĶ�����(Ԫ)������ɾ��ֱ���¼������öԷ���ʣ����
	DOUBLE							dCashedOut;							//�����ֽ�Ԫ��
	DOUBLE							dTotalLeftCash;						//ʣ���Ԫ��

	WORD							wPacketIdx;							//��ǰ�����
	WORD							wPacketNum;							//�ְ����͵��ܰ���

	WORD							wItemCount;
	PaymentInfoItem				PaymentInfoItems[Max_PaymentInfo_Num_PerSend];//������Ϣ
};
//��ҵ����
struct CMD_GP_AddEnterprisePaymentResult
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szRealName[LEN_NICKNAME];
	TCHAR							szOpenid[LEN_ACCOUNTS];
	LONG							lResultCode;						//��������
	DWORD							dwEnterprisePayment;				//���ֽ��֣�
	DWORD							dwTotalLeftCash;					//ʣ����֣�

	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//mChen add, for HideSeek
struct CMD_GP_BoughtTaggerModelResult
{
	LONG							lResultCode;						//��������

	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//�û�����
	WORD							wBoughtModelIndex;

	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

#define Max_TopPlayers_Num 50
//lin add
//��ѯ���������б���
struct TopMatchPlayerInfoItem
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							strNickName[32];					//�û��ǳ�
	SCORE							lTotalScore;						//���б�������
	WORD							wWinCount;							//ʤ������
	WORD							wDrawCount;							//ƽ�ֳ���
	WORD							wLoseCount;							//ʧ�ܳ���
};

//�� add
//��ѯ����Top n �б���
struct CMD_GP_TopMatchPlayerInfoResult
{
	LONG							lResultCode;						//��������

	WORD							wItemCount;
	TopMatchPlayerInfoItem				TopInfoItems[Max_TopPlayers_Num];//��������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//Զ�̷���

#define MDM_GP_REMOTE_SERVICE		4									//Զ�̷���

//���ҷ���
#define SUB_GP_C_SEARCH_DATABASE	100									//���ݲ���
#define SUB_GP_C_SEARCH_CORRESPOND	101									//Э������

//���ҷ���
#define SUB_GP_S_SEARCH_DATABASE	200									//���ݲ���
#define SUB_GP_S_SEARCH_CORRESPOND	201									//Э������

//////////////////////////////////////////////////////////////////////////////////

//Э������
struct CMD_GP_C_SearchCorrespond
{
	DWORD							dwGameID;							//��Ϸ��ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//Э������
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//�û���Ŀ
	tagUserRemoteInfo				UserRemoteInfo[16];					//�û���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//��¼����
#define MDM_MB_LOGON				100									//�㳡��¼

//��¼ģʽ
#define SUB_MB_LOGON_GAMEID			1									//I D ��¼
#define SUB_MB_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_MB_REGISTER_ACCOUNTS	3									//ע���ʺ�

//��¼���
#define SUB_MB_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_MB_LOGON_FAILURE		101									//��¼ʧ��

//������ʾ
#define SUB_MB_UPDATE_NOTIFY		200									//������ʾ

//////////////////////////////////////////////////////////////////////////////////

//I D ��¼
struct CMD_MB_LogonGameID
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//�ʺŵ�¼
struct CMD_MB_LogonAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//ע���ʺ�
struct CMD_MB_RegisterAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//��¼�ɹ�
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//��¼ʧ��
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������ʾ
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};


//////////////////////////////////////////////////////////////////////////////////
//�б�����

#define MDM_MB_SERVER_LIST			101									//�б���Ϣ

//�б���Ϣ
#define SUB_MB_LIST_KIND			100									//�����б�
#define SUB_MB_LIST_SERVER			101									//�����б�
#define SUB_MB_LIST_FINISH			200									//�б����

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif