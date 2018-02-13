#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "../../../ȫ�ֶ���/Array.h"

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//�߼�����

#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////
//��������

//������־
#define WIK_NULL					0x00								//û������
#define WIK_LEFT					0x01								//�������
#define WIK_CENTER					0x02								//�г�����
#define WIK_RIGHT					0x04								//�ҳ�����
#define WIK_PENG					0x08								//��������
#define WIK_GANG					0x10								//��������
#define WIK_XIAO_HU					0x20								//С��
#define WIK_CHI_HU					0x40								//�Ժ�����
#define WIK_ZI_MO					0x80								//����

//////////////////////////////////////////////////////////////////////////
//���ƶ���

//����
#define CHK_NULL						0x00										//�Ǻ�����
#define CHK_CHI_HU						0x01										//������

//������
#define CHR_BA_DUI						0x00000001									//�˶�X2
#define CHR_BAO_TOU						0x00000002									//��ͷX2
#define CHR_PIAO_CAI_YI					0x00000004									//Ʈ��X4
#define CHR_PIAO_CAI_ER					0x00000008									//����Ʈ��X8

#define CHR_PIAO_CAI_SAN				0x00000010									//����Ʈ��X16
#define CHR_GANG_KAI					0x00000020									//�ܿ�X2
#define CHR_GANG_BAO					0x00000040									//�ܱ�
#define CHR_PIAO_GANG					0x00000080									//Ʈ��

#define CHR_GANG_PIAO					0x00000100									//��Ʈ
#define CHR_SHI_SAN_BU_DA				0x00000200									//ʮ������X4
#define CHR_QING_YI_SE					0x00000400									//��һɫX10
#define CHR_QING_FENG_ZI				0x00000800                                  //�����X20

#define CHR_QIANG_GANG					0x00001000									//����
#define CHR_SHI_SAN_BU_DA_QIANG_GANG	0x00002000									//ʮ����������
#define CHR_QIANG_PIAO_GANG				0x00004000									//��Ʈ��
#define CHR_BA_DUI_ZI_PIAO_CAI			0x00008000									//�˶���Ʈ��

#define CHR_QING_YI_SE_PIAO_CAI			0x00010000									//��һɫƮ��
#define CHR_QING_BA_DUI_PIAO_CAI		0x00020000									//��˶�Ʈ��
#define CHR_QING_YI_SE_QIANGGANG		0x00040000									//��һɫ����

#define CHR_ZI_MO		0x01000000									//����
#define CHR_SHU_FAN		0x02000000									//�ط�


//////////////////////////////////////////////////////////////////////////

#define ZI_PAI_COUNT	7

//��������
struct tagKindItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbCardIndex[3];						//�˿�����
	BYTE							cbValidIndex[3];					//ʵ���˿�����
};

//�������
struct tagWeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
};

//���ƽ��
struct tagGangCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[MAX_WEAVE];				//�˿�����
};

//��������
struct tagAnalyseItem
{
	BYTE							cbCardEye;							//�����˿�
	bool                            bMagicEye;                          //�����Ƿ�������
	BYTE							cbWeaveKind[MAX_WEAVE];				//�������
	BYTE							cbCenterCard[MAX_WEAVE];			//�����˿�
	BYTE                            cbCardData[MAX_WEAVE][MAX_WEAVE];   //ʵ���˿�
};

//////////////////////////////////////////////////////////////////////////

#define MASK_CHI_HU_RIGHT			0x0fffffff

/*
//	Ȩλ�ࡣ
//  ע�⣬�ڲ�����λʱ���ֻ��������Ȩλ.����
//  CChiHuRight chr;
//  chr |= (chr_zi_mo|chr_peng_peng)������������޶���ġ�
//  ֻ�ܵ�������:
//  chr |= chr_zi_mo;
//  chr |= chr_peng_peng;
*/
class CChiHuRight
{
	//��̬����
private:
	static bool						m_bInit;
	static DWORD					m_dwRightMask[MAX_RIGHT_COUNT];

	//Ȩλ����
private:
	DWORD							m_dwRight[MAX_RIGHT_COUNT];

public:
	//���캯��
	CChiHuRight();

	//���������
public:
	//��ֵ��
	CChiHuRight & operator = (DWORD dwRight);

	//�����
	CChiHuRight & operator &= (DWORD dwRight);
	//�����
	CChiHuRight & operator |= (DWORD dwRight);

	//��
	CChiHuRight operator & (DWORD dwRight);
	CChiHuRight operator & (DWORD dwRight) const;

	//��
	CChiHuRight operator | (DWORD dwRight);
	CChiHuRight operator | (DWORD dwRight) const;

	//���ܺ���
public:
	//�Ƿ�ȨλΪ��
	bool IsEmpty();

	//����ȨλΪ��
	void SetEmpty();

	//��ȡȨλ��ֵ
	BYTE GetRightData(DWORD dwRight[], BYTE cbMaxCount);

	//����Ȩλ��ֵ
	bool SetRightData(const DWORD dwRight[], BYTE cbRightCount);

private:
	//���Ȩλ�Ƿ���ȷ
	bool IsValidRight(DWORD dwRight);
};


//////////////////////////////////////////////////////////////////////////

//����˵��
typedef CWHArray<tagAnalyseItem, tagAnalyseItem &> CAnalyseItemArray;

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_cbCardDataArray[MAX_REPERTORY];		//�˿�����
	static const BYTE				m_cbCardDataArray_HZ[MAX_REPERTORY_HZ];	//�˿�����
	BYTE							m_cbMagicIndex;							//��������
	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ƺ���
public:
	//�����˿�
	void RandCardData(BYTE cbCardData[], BYTE cbMaxCount);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//��������
	void SetMagicIndex(BYTE cbMagicIndex) { m_cbMagicIndex = cbMagicIndex; }
	//����
	bool IsMagicCard(BYTE cbCardData);

	//��ȡ��������Ŀ
	BYTE GetMagicCount(const BYTE cbCardIndex[]);

	//��������
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�˿���Ŀ
	BYTE GetCardCount(const BYTE cbCardIndex[]);
	//����˿�
	BYTE GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4]);

	//�ȼ�����
public:
	//�����ȼ�
	BYTE GetUserActionRank(BYTE cbUserAction);
	//���Ƶȼ�

	WORD GetChiHuActionRank_ZZ(const CChiHuRight & ChiHuRight);


	//�����ж�
public:
	//�����ж�
	BYTE EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�����ж�
	BYTE EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�����ж�,���ϸ��ƣ��Ÿܡ�����
	BYTE EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�����жϣ�����
	BYTE EstimateGangCard(const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard);

	//�����ж�
public:
	//���Ʒ���
	BYTE AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult,bool bOnlyAnalyseShouPai);
	

	//ת������
public:
	//�˿�ת��
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//�˿�ת��
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//�˿�ת��
	BYTE SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT]);
	//�˿�ת��
	BYTE SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);

	//��������
public:
	

	//�����˿�
	bool AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray);
	//����,������ֵ����
	bool SortCardList(BYTE cbCardData[MAX_COUNT], BYTE cbCardCount);


	//�����齫
	//�˶�
	bool IsBaDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE cbCurrentCard, bool &bBaoTou, bool &bCanPiao);
	//��һɫ��
	bool IsQingYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE cbCurrentCard);
	//�����
	bool IsQingFengZi(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE cbCurrentCard);
	//��ͷ
	bool IsBaoTou(const tagAnalyseItem *pAnalyseItem, const BYTE cbCurrentCard);
	bool IsBaoTou(const BYTE cbCardIndex[], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE cbCurrentCard);
	//Ʈ��
	bool CanPiaoCai(const BYTE cbCardIndex[], const BYTE cbCurrentCard);
	//ʮ������
	bool IsShiSanBuDa(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE cbCurrentCard);
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
