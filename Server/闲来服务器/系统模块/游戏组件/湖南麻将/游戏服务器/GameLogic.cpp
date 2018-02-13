#include "StdAfx.h"
#include "GameLogic.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
bool		CChiHuRight::m_bInit = false;
DWORD		CChiHuRight::m_dwRightMask[MAX_RIGHT_COUNT];

//���캯��
CChiHuRight::CChiHuRight()
{
	ZeroMemory(m_dwRight, sizeof(m_dwRight));

	if (!m_bInit)
	{
		m_bInit = true;
		for (BYTE i = 0; i < CountArray(m_dwRightMask); i++)
		{
			if (0 == i)
				m_dwRightMask[i] = 0;
			else
				m_dwRightMask[i] = (DWORD(pow((float)2, (float)(i - 1)))) << 28;
		}
	}
}

//��ֵ������
CChiHuRight & CChiHuRight::operator = (DWORD dwRight)
{
	DWORD dwOtherRight = 0;
	//��֤Ȩλ
	if (!IsValidRight(dwRight))
	{
		//��֤ȡ��Ȩλ
		ASSERT(IsValidRight(~dwRight));
		if (!IsValidRight(~dwRight)) return *this;
		dwRight = ~dwRight;
		dwOtherRight = MASK_CHI_HU_RIGHT;
	}

	for (BYTE i = 0; i < CountArray(m_dwRightMask); i++)
	{
		if ((dwRight&m_dwRightMask[i]) || (i == 0 && dwRight < 0x10000000))
			m_dwRight[i] = dwRight&MASK_CHI_HU_RIGHT;
		else m_dwRight[i] = dwOtherRight;
	}

	return *this;
}

//�����
CChiHuRight & CChiHuRight::operator &= (DWORD dwRight)
{
	bool bNavigate = false;
	//��֤Ȩλ
	if (!IsValidRight(dwRight))
	{
		//��֤ȡ��Ȩλ
		ASSERT(IsValidRight(~dwRight));
		if (!IsValidRight(~dwRight)) return *this;
		//����Ȩλ
		DWORD dwHeadRight = (~dwRight) & 0xF0000000;
		DWORD dwTailRight = dwRight&MASK_CHI_HU_RIGHT;
		dwRight = dwHeadRight | dwTailRight;
		bNavigate = true;
	}

	for (BYTE i = 0; i < CountArray(m_dwRightMask); i++)
	{
		if ((dwRight&m_dwRightMask[i]) || (i == 0 && dwRight < 0x10000000))
		{
			m_dwRight[i] &= (dwRight&MASK_CHI_HU_RIGHT);
		}
		else if (!bNavigate)
			m_dwRight[i] = 0;
	}

	return *this;
}

//�����
CChiHuRight & CChiHuRight::operator |= (DWORD dwRight)
{
	//��֤Ȩλ
	if (!IsValidRight(dwRight)) return *this;

	for (BYTE i = 0; i < CountArray(m_dwRightMask); i++)
	{
		if ((dwRight&m_dwRightMask[i]) || (i == 0 && dwRight < 0x10000000))
			m_dwRight[i] |= (dwRight&MASK_CHI_HU_RIGHT);
	}

	return *this;
}

//��
CChiHuRight CChiHuRight::operator & (DWORD dwRight)
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//��
CChiHuRight CChiHuRight::operator & (DWORD dwRight) const
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//��
CChiHuRight CChiHuRight::operator | (DWORD dwRight)
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//��
CChiHuRight CChiHuRight::operator | (DWORD dwRight) const
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//�Ƿ�ȨλΪ��
bool CChiHuRight::IsEmpty()
{
	for (BYTE i = 0; i < CountArray(m_dwRight); i++)
		if (m_dwRight[i]) return false;
	return true;
}

//����ȨλΪ��
void CChiHuRight::SetEmpty()
{
	ZeroMemory(m_dwRight, sizeof(m_dwRight));
	return;
}

//��ȡȨλ��ֵ
BYTE CChiHuRight::GetRightData(DWORD dwRight[], BYTE cbMaxCount)
{
	ASSERT(cbMaxCount >= CountArray(m_dwRight));
	if (cbMaxCount < CountArray(m_dwRight)) return 0;

	CopyMemory(dwRight, m_dwRight, sizeof(DWORD)*CountArray(m_dwRight));
	return CountArray(m_dwRight);
}

//����Ȩλ��ֵ
bool CChiHuRight::SetRightData(const DWORD dwRight[], BYTE cbRightCount)
{
	ASSERT(cbRightCount <= CountArray(m_dwRight));
	if (cbRightCount > CountArray(m_dwRight)) return false;

	ZeroMemory(m_dwRight, sizeof(m_dwRight));
	CopyMemory(m_dwRight, dwRight, sizeof(DWORD)*cbRightCount);

	return true;
}

//����λ�Ƿ���ȷ
bool CChiHuRight::IsValidRight(DWORD dwRight)
{
	DWORD dwRightHead = dwRight & 0xF0000000;
	for (BYTE i = 0; i < CountArray(m_dwRightMask); i++)
		if (m_dwRightMask[i] == dwRightHead) return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//��̬����

//�˿�����
const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY] =
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
};


//�˿�����
const BYTE CGameLogic::m_cbCardDataArray_HZ[MAX_REPERTORY_HZ] =
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��

	0x31,0x31,0x31,0x31,												//��
	0x32,0x32,0x32,0x32,												//��
	0x33,0x33,0x33,0x33,												//��
	0x34,0x34,0x34,0x34,												//��
	0x35,0x35,0x35,0x35,												//��
	0x36,0x36,0x36,0x36,												//��
	0x37,0x37,0x37,0x37,												//��
};
//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
	m_cbMagicIndex = MAX_INDEX;
}

//��������
CGameLogic::~CGameLogic()
{
}

//�����˿�
void CGameLogic::RandCardData(BYTE cbCardData[], BYTE cbMaxCount)
{
	//����׼��
#define RAND_CARD(CardDataArry)         \
	BYTE cbCardDataTemp[CountArray(CardDataArry)];\
	CopyMemory(cbCardDataTemp,CardDataArry,sizeof(CardDataArry));\
	BYTE cbRandCount=0,cbPosition=0;\
	do\
	{\
		cbPosition=rand()%(cbMaxCount-cbRandCount);\
		cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];\
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];\
	} while (cbRandCount<cbMaxCount);\


	if (cbMaxCount == MAX_REPERTORY)
	{
		RAND_CARD(m_cbCardDataArray);
		SetMagicIndex(MAX_INDEX);
	}
	else if (cbMaxCount == MAX_REPERTORY_HZ)
	{
		RAND_CARD(m_cbCardDataArray_HZ);
		SetMagicIndex(SwitchToCardIndex(0x37));//0x35
	}

	return;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//Ч���˿�
	ASSERT(IsValidCard(cbRemoveCard));
	ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)] > 0);

	//ɾ���˿�
	BYTE cbRemoveIndex = SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex] > 0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//ʧ��Ч��
	ASSERT(FALSE);

	return false;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//ɾ���˿�
	for (BYTE i = 0; i < cbRemoveCount; i++)
	{
		//Ч���˿�
		ASSERT(IsValidCard(cbRemoveCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])] > 0);

		//ɾ���˿�
		BYTE cbRemoveIndex = SwitchToCardIndex(cbRemoveCard[i]);
		if (cbCardIndex[cbRemoveIndex] == 0)
		{
			//�������
			ASSERT(FALSE);

			//��ԭɾ��
			for (BYTE j = 0; j < i; j++)
			{
				ASSERT(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else
		{
			//ɾ���˿�
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//��������
	ASSERT(cbCardCount <= MAX_COUNT);
	ASSERT(cbRemoveCount <= cbCardCount);

	//�������
	BYTE cbDeleteCount = 0, cbTempCardData[MAX_COUNT];
	if (cbCardCount > CountArray(cbTempCardData))
		return false;
	CopyMemory(cbTempCardData, cbCardData, cbCardCount * sizeof(cbCardData[0]));

	//�����˿�
	for (BYTE i = 0; i < cbRemoveCount; i++)
	{
		for (BYTE j = 0; j < cbCardCount; j++)
		{
			if (cbRemoveCard[i] == cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j] = 0;
				break;
			}
		}
	}

	//�ɹ��ж�
	if (cbDeleteCount != cbRemoveCount)
	{
		ASSERT(FALSE);
		return false;
	}

	//�����˿�
	BYTE cbCardPos = 0;
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		if (cbTempCardData[i] != 0)
			cbCardData[cbCardPos++] = cbTempCardData[i];
	}

	return true;
}

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue = (cbCardData&MASK_VALUE);
	BYTE cbColor = (cbCardData&MASK_COLOR) >> 4;
	return (((cbValue >= 1) && (cbValue <= 9) && (cbColor <= 2)) || ((cbValue >= 1) && (cbValue <= 7) && (cbColor == 3)));
}

//�˿���Ŀ
BYTE CGameLogic::GetCardCount(const BYTE cbCardIndex[MAX_INDEX])
{
	//��Ŀͳ��
	BYTE cbCardCount = 0;
	for (BYTE i = 0; i < MAX_INDEX; i++)
		cbCardCount += cbCardIndex[i];

	return cbCardCount;
}

//��ȡ���
BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4])
{
	//����˿�
	switch (cbWeaveKind)
	{
	case WIK_LEFT:		//���Ʋ���
	{
		//���ñ���
		cbCardBuffer[0] = cbCenterCard;
		cbCardBuffer[1] = cbCenterCard + 1;
		cbCardBuffer[2] = cbCenterCard + 2;

		return 3;
	}
	case WIK_RIGHT:		//���Ʋ���
	{
		//���ñ���
		cbCardBuffer[0] = cbCenterCard;
		cbCardBuffer[1] = cbCenterCard - 1;
		cbCardBuffer[2] = cbCenterCard - 2;

		return 3;
	}
	case WIK_CENTER:	//���Ʋ���
	{
		//���ñ���
		cbCardBuffer[0] = cbCenterCard;
		cbCardBuffer[1] = cbCenterCard - 1;
		cbCardBuffer[2] = cbCenterCard + 1;

		return 3;
	}
	case WIK_PENG:		//���Ʋ���
	{
		//���ñ���
		cbCardBuffer[0] = cbCenterCard;
		cbCardBuffer[1] = cbCenterCard;
		cbCardBuffer[2] = cbCenterCard;

		return 3;
	}
	case WIK_GANG:		//���Ʋ���
	{
		//���ñ���
		cbCardBuffer[0] = cbCenterCard;
		cbCardBuffer[1] = cbCenterCard;
		cbCardBuffer[2] = cbCenterCard;
		cbCardBuffer[3] = cbCenterCard;

		return 4;
	}
	default:
	{
		ASSERT(FALSE);
	}
	}

	return 0;
}

//�����ȼ�
BYTE CGameLogic::GetUserActionRank(BYTE cbUserAction)
{
	//���Ƶȼ�
	if (cbUserAction&WIK_CHI_HU) { return 4; }

	//���Ƶȼ�
	if (cbUserAction&WIK_GANG) { return 3; }

	//���Ƶȼ�
	if (cbUserAction&WIK_PENG) { return 2; }

	//���Ƶȼ�
	if (cbUserAction&(WIK_RIGHT | WIK_CENTER | WIK_LEFT)) { return 1; }

	return 0;
}



WORD CGameLogic::GetChiHuActionRank_ZZ(const CChiHuRight & ChiHuRight)
{
	WORD wFanShu = 0;

	//����
	if (!(ChiHuRight&CHR_ZI_MO).IsEmpty())
		wFanShu = 1;
	if (!(ChiHuRight&CHR_BA_DUI).IsEmpty())
		wFanShu *= 2;
	if (!(ChiHuRight&CHR_BAO_TOU).IsEmpty())
		wFanShu *= 2;
	if (!(ChiHuRight&CHR_PIAO_CAI_YI).IsEmpty())
		wFanShu *= 2;
	if (!(ChiHuRight&CHR_PIAO_CAI_ER).IsEmpty())
		wFanShu *= 4;
	if (!(ChiHuRight&CHR_PIAO_CAI_SAN).IsEmpty())
		wFanShu *= 8;
	if (!(ChiHuRight&CHR_GANG_KAI).IsEmpty())
		wFanShu *= 2;
	if (!(ChiHuRight&CHR_SHI_SAN_BU_DA).IsEmpty())
		wFanShu *= 4;
	if (!(ChiHuRight&CHR_QING_YI_SE).IsEmpty())
		wFanShu *= 10;
	if (!(ChiHuRight&CHR_QING_FENG_ZI).IsEmpty())
		wFanShu *= 20;
	if (!(ChiHuRight&CHR_GANG_PIAO).IsEmpty())
		wFanShu *= 2;
	if (!(ChiHuRight&CHR_PIAO_GANG).IsEmpty())
		wFanShu *= 2;

	//����
	if (!(ChiHuRight&CHR_QIANG_GANG).IsEmpty())
		wFanShu = 12;
	if (!(ChiHuRight&CHR_SHI_SAN_BU_DA_QIANG_GANG).IsEmpty())
		wFanShu = 12;
	if (!(ChiHuRight&CHR_QING_YI_SE_QIANGGANG).IsEmpty())
		wFanShu = 60;


	ASSERT(wFanShu > 0);
	if (!(ChiHuRight&CHR_ZI_MO).IsEmpty())
		if (wFanShu > 20)
			wFanShu = 20;
	return wFanShu;
}

BYTE CGameLogic::EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	if (cbCurrentCard >= 0x31 || IsMagicCard(cbCurrentCard))
		return WIK_NULL;

	//��������
	BYTE cbExcursion[3] = { 0,1,2 };
	BYTE cbItemKind[3] = { WIK_LEFT,WIK_CENTER,WIK_RIGHT };

	//�����ж�
	BYTE cbEatKind = 0, cbFirstIndex = 0;
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	for (BYTE i = 0; i < CountArray(cbItemKind); i++)
	{
		BYTE cbValueIndex = cbCurrentIndex % 9;
		if ((cbValueIndex >= cbExcursion[i]) && ((cbValueIndex - cbExcursion[i]) <= 6))
		{
			//�����ж�
			cbFirstIndex = cbCurrentIndex - cbExcursion[i];

			//���Ʋ��ܰ���������
			if (m_cbMagicIndex != MAX_INDEX &&
				m_cbMagicIndex >= cbFirstIndex && m_cbMagicIndex <= cbFirstIndex + 2) continue;

			if ((cbCurrentIndex != cbFirstIndex) && (cbCardIndex[cbFirstIndex] == 0))
				continue;
			if ((cbCurrentIndex != (cbFirstIndex + 1)) && (cbCardIndex[cbFirstIndex + 1] == 0))
				continue;
			if ((cbCurrentIndex != (cbFirstIndex + 2)) && (cbCardIndex[cbFirstIndex + 2] == 0))
				continue;

			//��������
			cbEatKind |= cbItemKind[i];
		}
	}

	return cbEatKind;
}

//�����ж�
BYTE CGameLogic::EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	if (IsMagicCard(cbCurrentCard))
		return WIK_NULL;

	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)] >= 2) ? WIK_PENG : WIK_NULL;
}

//�����ж�
BYTE CGameLogic::EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	if (IsMagicCard(cbCurrentCard))
		return WIK_NULL;

	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)] == 3) ? WIK_GANG : WIK_NULL;
}

BYTE CGameLogic::EstimateGangCard(const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	if (IsMagicCard(cbCurrentCard))
		return WIK_NULL;

	for (BYTE i = 0; i < cbWeaveCount; i++)
	{
		if (WeaveItem[i].cbWeaveKind == WIK_PENG)
		{
			if (WeaveItem[i].cbCenterCard == cbCurrentCard)
				return WIK_GANG;
		}
	}

	return WIK_NULL;
}

//���Ʒ���
BYTE CGameLogic::AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult, bool bOnlyAnalyseShouPai)
{
	//���ñ���
	BYTE cbActionMask = WIK_NULL;
	ZeroMemory(&GangCardResult, sizeof(GangCardResult));

	//���ϸ���
	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		if (i == m_cbMagicIndex) continue;
		if (cbCardIndex[i] == 4)
		{
			cbActionMask |= WIK_GANG;
			GangCardResult.cbCardData[GangCardResult.cbCardCount++] = SwitchToCardData(i);
		}
	}

	//��ϸ���
	if (!bOnlyAnalyseShouPai)
	{
		for (BYTE i = 0; i < cbWeaveCount; i++)
		{
			if (WeaveItem[i].cbWeaveKind == WIK_PENG)
			{
				if (cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)] == 1)
				{
					cbActionMask |= WIK_GANG;
					GangCardResult.cbCardData[GangCardResult.cbCardCount++] = WeaveItem[i].cbCenterCard;
				}
			}
		}
	}

	return cbActionMask;
}



//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	ASSERT(cbCardIndex < 34);
	return ((cbCardIndex / 9) << 4) | (cbCardIndex % 9 + 1);
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));
	return ((cbCardData&MASK_COLOR) >> 4) * 9 + (cbCardData&MASK_VALUE) - 1;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT])
{
	//ת���˿�
	BYTE cbPosition = 0;
	//����
	if (m_cbMagicIndex != MAX_INDEX)
	{
		for (BYTE i = 0; i < cbCardIndex[m_cbMagicIndex]; i++)
			cbCardData[cbPosition++] = SwitchToCardData(m_cbMagicIndex);
	}
	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		if (i == m_cbMagicIndex) continue;
		if (cbCardIndex[i] != 0)
		{
			for (BYTE j = 0; j < cbCardIndex[i]; j++)
			{
				ASSERT(cbPosition < MAX_COUNT);
				cbCardData[cbPosition++] = SwitchToCardData(i);
			}
		}
	}

	return cbPosition;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//���ñ���
	ZeroMemory(cbCardIndex, sizeof(BYTE)*MAX_INDEX);

	//ת���˿�
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

//�����˿�
bool CGameLogic::AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)
{
	//��������ʣ������Ŀ
	BYTE cbCardCount = GetCardCount(cbCardIndex);

	//Ч����Ŀ
	ASSERT((cbCardCount >= 2) && (cbCardCount <= MAX_COUNT) && ((cbCardCount - 2) % 3 == 0));
	if ((cbCardCount < 2) || (cbCardCount > MAX_COUNT) || ((cbCardCount - 2) % 3 != 0))
		return false;

	//��������
	BYTE cbKindItemCount = 0;
	tagKindItem KindItem[MAX_COUNT * 5];
	ZeroMemory(KindItem, sizeof(KindItem));

	//cbLessKindItem==����ʣ���Ƶ������
	BYTE cbLessKindItem = (cbCardCount - 2) / 3;
	ASSERT((cbLessKindItem + cbWeaveCount) == MAX_WEAVE);

	//�����ж�
	if (cbLessKindItem == 0)
	{
		//Ч�����
		ASSERT((cbCardCount == 2) && (cbWeaveCount == MAX_WEAVE));

		//�����ж�
		for (BYTE i = 0; i < MAX_INDEX; i++)
		{
			if (cbCardIndex[i] == 0) continue;

			if (cbCardIndex[i] == 2
				|| (m_cbMagicIndex != MAX_INDEX && i != m_cbMagicIndex && cbCardIndex[m_cbMagicIndex] + cbCardIndex[i] == 2))
				//��������Ƹպ�Ϊ���
				//����Ϊ����������ͨ�����
			{
				//��������
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem, sizeof(AnalyseItem));

				//���ý��
				for (BYTE j = 0; j < cbWeaveCount; j++)
				{
					AnalyseItem.cbWeaveKind[j] = WeaveItem[j].cbWeaveKind;
					AnalyseItem.cbCenterCard[j] = WeaveItem[j].cbCenterCard;
					GetWeaveCard(WeaveItem[j].cbWeaveKind, WeaveItem[j].cbCenterCard, AnalyseItem.cbCardData[j]);
				}
				AnalyseItem.cbCardEye = SwitchToCardData(i);

				if (cbCardIndex[i] < 2 || i == m_cbMagicIndex)
					AnalyseItem.bMagicEye = true;
				else
					AnalyseItem.bMagicEye = false;


				//������
				AnalyseItemArray.Add(AnalyseItem);

				return true;
			}
		}

		return false;
	}

	//��ַ���
	BYTE cbMagicCardIndex[MAX_INDEX];
	CopyMemory(cbMagicCardIndex, cbCardIndex, sizeof(cbMagicCardIndex));

	BYTE cbMagicCardCount = 0;
	//��������Ŀ
	cbMagicCardCount = cbCardIndex[m_cbMagicIndex];
	

	if (cbCardCount >= 3)
	{
		for (BYTE i = 0; i < MAX_INDEX; i++)
		{
			//add ����������ƣ����Գ��ٴ��⣬����Ҫ��һ��
			if (i == m_cbMagicIndex || cbMagicCardIndex[i] == 0) continue;

			//ͬ���ж�
			if (cbMagicCardIndex[i] + cbMagicCardCount >= 3)
			{
				ASSERT(cbKindItemCount < CountArray(KindItem));
				KindItem[cbKindItemCount].cbCardIndex[0] = i;
				KindItem[cbKindItemCount].cbCardIndex[1] = i;
				KindItem[cbKindItemCount].cbCardIndex[2] = i;
				KindItem[cbKindItemCount].cbWeaveKind = WIK_PENG;
				KindItem[cbKindItemCount].cbCenterCard = SwitchToCardData(i);
				KindItem[cbKindItemCount].cbValidIndex[0] = cbMagicCardIndex[i] > 0 ? i : m_cbMagicIndex;
				KindItem[cbKindItemCount].cbValidIndex[1] = cbMagicCardIndex[i] > 1 ? i : m_cbMagicIndex;
				KindItem[cbKindItemCount].cbValidIndex[2] = cbMagicCardIndex[i] > 2 ? i : m_cbMagicIndex;
				cbKindItemCount++;
				if (cbMagicCardIndex[i] + cbMagicCardCount >= 6 && cbMagicCardIndex[i] > 3)
				{
					ASSERT(cbKindItemCount < CountArray(KindItem));
					KindItem[cbKindItemCount].cbCardIndex[0] = i;
					KindItem[cbKindItemCount].cbCardIndex[1] = i;
					KindItem[cbKindItemCount].cbCardIndex[2] = i;
					KindItem[cbKindItemCount].cbWeaveKind = WIK_PENG;
					KindItem[cbKindItemCount].cbCenterCard = SwitchToCardData(i);
					KindItem[cbKindItemCount].cbValidIndex[0] = cbMagicCardIndex[i] > 3 ? i : m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[1] = m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[2] = m_cbMagicIndex;
					cbKindItemCount++;
				}
			}
		}

		for(BYTE i = 0; i < 27; i++)
		{
			//1-7
			if ((i < 27) && ((i % 9) < 7))
			{
				//ֻҪ�����������Ƶ��������ڵ���3,��������
				if (cbMagicCardCount + cbMagicCardIndex[i] + cbMagicCardIndex[i + 1] >= 3)
				{
					if (cbMagicCardIndex[i] == 0 || cbMagicCardIndex[i + 1] == 0)
					{ }
					else
					{
						BYTE cbIndex[3] = { cbMagicCardIndex[i],cbMagicCardIndex[i + 1],0 };
						int nMagicCountTemp = cbMagicCardCount;
						BYTE cbValidIndex[3];
						while (nMagicCountTemp + cbIndex[0] + cbIndex[1] + cbIndex[2] >= 3)
						{
							for (BYTE j = 0; j < CountArray(cbIndex); j++)
							{
								if (cbIndex[j] > 0)
								{
									cbIndex[j]--;
									cbValidIndex[j] = i + j;
								}
								else
								{
									nMagicCountTemp--;
									cbValidIndex[j] = m_cbMagicIndex;
								}
							}
							if (nMagicCountTemp >= 0)
							{
								ASSERT(cbKindItemCount < CountArray(KindItem));
								KindItem[cbKindItemCount].cbCardIndex[0] = i;
								KindItem[cbKindItemCount].cbCardIndex[1] = i + 1;
								KindItem[cbKindItemCount].cbCardIndex[2] = i + 2;
								KindItem[cbKindItemCount].cbWeaveKind = WIK_LEFT;
								KindItem[cbKindItemCount].cbCenterCard = SwitchToCardData(i);
								CopyMemory(KindItem[cbKindItemCount].cbValidIndex, cbValidIndex, sizeof(cbValidIndex));
								cbKindItemCount++;
							}
							else
								break;
						}
					}
				}

				if (cbMagicCardCount + cbMagicCardIndex[i] + cbMagicCardIndex[i + 2] >= 3)
				{
					if (cbMagicCardIndex[i] == 0 || cbMagicCardIndex[i + 2] == 0)
					{ }
					else
					{
						BYTE cbIndex[3] = { cbMagicCardIndex[i],0,cbMagicCardIndex[i + 2] };
						int nMagicCountTemp = cbMagicCardCount;
						BYTE cbValidIndex[3];
						while (nMagicCountTemp + cbIndex[0] + cbIndex[1] + cbIndex[2] >= 3)
						{
							for (BYTE j = 0; j < CountArray(cbIndex); j++)
							{
								if (cbIndex[j] > 0)
								{
									cbIndex[j]--;
									cbValidIndex[j] = i + j;
								}
								else
								{
									nMagicCountTemp--;
									cbValidIndex[j] = m_cbMagicIndex;
								}
							}
							if (nMagicCountTemp >= 0)
							{
								ASSERT(cbKindItemCount < CountArray(KindItem));
								KindItem[cbKindItemCount].cbCardIndex[0] = i;
								KindItem[cbKindItemCount].cbCardIndex[1] = i + 1;
								KindItem[cbKindItemCount].cbCardIndex[2] = i + 2;
								KindItem[cbKindItemCount].cbWeaveKind = WIK_LEFT;
								KindItem[cbKindItemCount].cbCenterCard = SwitchToCardData(i);
								CopyMemory(KindItem[cbKindItemCount].cbValidIndex, cbValidIndex, sizeof(cbValidIndex));
								cbKindItemCount++;
							}
							else
								break;
						}
					}
				}

				if (cbMagicCardCount + cbMagicCardIndex[i + 1] + cbMagicCardIndex[i + 2] >= 3)
				{
					if (cbMagicCardIndex[i + 1] == 0 || cbMagicCardIndex[i + 2] == 0)
					{ }
					else
					{
						BYTE cbIndex[3] = { 0,cbMagicCardIndex[i + 1],cbMagicCardIndex[i + 2] };
						int nMagicCountTemp = cbMagicCardCount;
						BYTE cbValidIndex[3];
						while (nMagicCountTemp + cbIndex[0] + cbIndex[1] + cbIndex[2] >= 3)
						{
							for (BYTE j = 0; j < CountArray(cbIndex); j++)
							{
								if (cbIndex[j] > 0)
								{
									cbIndex[j]--;
									cbValidIndex[j] = i + j;
								}
								else
								{
									nMagicCountTemp--;
									cbValidIndex[j] = m_cbMagicIndex;
								}
							}
							if (nMagicCountTemp >= 0)
							{
								ASSERT(cbKindItemCount < CountArray(KindItem));
								KindItem[cbKindItemCount].cbCardIndex[0] = i;
								KindItem[cbKindItemCount].cbCardIndex[1] = i + 1;
								KindItem[cbKindItemCount].cbCardIndex[2] = i + 2;
								KindItem[cbKindItemCount].cbWeaveKind = WIK_LEFT;
								KindItem[cbKindItemCount].cbCenterCard = SwitchToCardData(i);
								CopyMemory(KindItem[cbKindItemCount].cbValidIndex, cbValidIndex, sizeof(cbValidIndex));
								cbKindItemCount++;
							}
							else
								break;
						}
					}
				}

				if (cbMagicCardCount + cbMagicCardIndex[i] + cbMagicCardIndex[i + 1] + cbMagicCardIndex[i + 2] >= 3)
				{
					if (cbMagicCardIndex[i] == 0 || cbMagicCardIndex[i + 1] == 0 || cbMagicCardIndex[i + 2] == 0)
					{ }
					else
					{
						BYTE cbIndex[3] = { cbMagicCardIndex[i],cbMagicCardIndex[i + 1],cbMagicCardIndex[i + 2] };
						int nMagicCountTemp = cbMagicCardCount;
						BYTE cbValidIndex[3];
						while (nMagicCountTemp + cbIndex[0] + cbIndex[1] + cbIndex[2] >= 3)
						{
							for (BYTE j = 0; j < CountArray(cbIndex); j++)
							{
								if (cbIndex[j] > 0)
								{
									cbIndex[j]--;
									cbValidIndex[j] = i + j;
								}
								else
								{
									nMagicCountTemp--;
									cbValidIndex[j] = m_cbMagicIndex;
								}
							}
							if (nMagicCountTemp >= 0)
							{
								ASSERT(cbKindItemCount < CountArray(KindItem));
								KindItem[cbKindItemCount].cbCardIndex[0] = i;
								KindItem[cbKindItemCount].cbCardIndex[1] = i + 1;
								KindItem[cbKindItemCount].cbCardIndex[2] = i + 2;
								KindItem[cbKindItemCount].cbWeaveKind = WIK_LEFT;
								KindItem[cbKindItemCount].cbCenterCard = SwitchToCardData(i);
								CopyMemory(KindItem[cbKindItemCount].cbValidIndex, cbValidIndex, sizeof(cbValidIndex));
								cbKindItemCount++;
							}
							else
								break;
						}
					}
				}
			}
		}
	}

	//��Ϸ���
	if (cbKindItemCount >= cbLessKindItem)
	{
		//��������
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp, sizeof(cbCardIndexTemp));

		//��������
		BYTE cbIndex[MAX_WEAVE];// = { 0,1,2,3 };
		for (int i = 0; i < cbLessKindItem; i++)
		{
			cbIndex[i] = i;
		}
		tagKindItem * pKindItem[MAX_WEAVE];
		ZeroMemory(&pKindItem, sizeof(pKindItem));

		//��ʼ���
		do
		{
			//���ñ���
			CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
			for (BYTE i = 0; i < cbLessKindItem; i++)
				pKindItem[i] = &KindItem[cbIndex[i]];

			//�����ж�
			bool bEnoughCard = true;
			
			for (BYTE i = 0; i < cbLessKindItem * 3; i++)
			{
				//�����ж�
				BYTE cbCardIndex = pKindItem[i / 3]->cbValidIndex[i % 3];
				if (cbCardIndexTemp[cbCardIndex] == 0)
				{
					bEnoughCard = false;
					break;
				}
				else
					cbCardIndexTemp[cbCardIndex]--;
			}

			//�����ж�
			if (bEnoughCard == true)
			{
				//�����ж�
				BYTE cbCardEye = 0;
				bool bMagicEye = false;
				for (BYTE i = 0; i < MAX_INDEX; i++)
				{
					if (cbCardIndexTemp[i] == 2)
					{
						cbCardEye = SwitchToCardData(i);
						if (i == m_cbMagicIndex)
						{
							bMagicEye = true;
						}
						break;
					}
					else if (i != m_cbMagicIndex &&
						m_cbMagicIndex != MAX_INDEX && cbCardIndexTemp[i] + cbCardIndexTemp[m_cbMagicIndex] == 2)
					{
						cbCardEye = SwitchToCardData(i);
						bMagicEye = true;
					}
				}

				//�������
				if (cbCardEye != 0)
				{
					//��������
					tagAnalyseItem AnalyseItem;
					ZeroMemory(&AnalyseItem, sizeof(AnalyseItem));

					//�������
					for (BYTE i = 0; i < cbWeaveCount; i++)
					{
						AnalyseItem.cbWeaveKind[i] = WeaveItem[i].cbWeaveKind;
						AnalyseItem.cbCenterCard[i] = WeaveItem[i].cbCenterCard;
						GetWeaveCard(WeaveItem[i].cbWeaveKind, WeaveItem[i].cbCenterCard, AnalyseItem.cbCardData[i]);
					}

					//��������
					for (BYTE i = 0; i < cbLessKindItem; i++)
					{
						AnalyseItem.cbWeaveKind[i + cbWeaveCount] = pKindItem[i]->cbWeaveKind;
						AnalyseItem.cbCenterCard[i + cbWeaveCount] = pKindItem[i]->cbCenterCard;
						AnalyseItem.cbCardData[cbWeaveCount + i][0] = SwitchToCardData(pKindItem[i]->cbValidIndex[0]);
						AnalyseItem.cbCardData[cbWeaveCount + i][1] = SwitchToCardData(pKindItem[i]->cbValidIndex[1]);
						AnalyseItem.cbCardData[cbWeaveCount + i][2] = SwitchToCardData(pKindItem[i]->cbValidIndex[2]);
					}

					//��������
					AnalyseItem.cbCardEye = cbCardEye;
					AnalyseItem.bMagicEye = bMagicEye;


					//������
					AnalyseItemArray.Add(AnalyseItem);
				}
			}

			//��������
			if (cbIndex[cbLessKindItem - 1] == (cbKindItemCount - 1))
			{
				BYTE i = cbLessKindItem - 1;
				for (; i > 0; i--)
				{
					if ((cbIndex[i - 1] + 1) != cbIndex[i])
					{
						BYTE cbNewIndex = cbIndex[i - 1];
						for (BYTE j = (i - 1); j < cbLessKindItem; j++)
							cbIndex[j] = cbNewIndex + j - i + 2;
						break;
					}
				}
				if (i == 0)
					break;
			}
			else
				cbIndex[cbLessKindItem - 1]++;
		} while (true);

	}

	return (AnalyseItemArray.GetCount() > 0);
}

//����
bool CGameLogic::IsMagicCard(BYTE cbCardData)
{
	if (m_cbMagicIndex != MAX_INDEX)
		return SwitchToCardIndex(cbCardData) == m_cbMagicIndex;
	return false;
}

BYTE CGameLogic::GetMagicCount(const BYTE cbCardIndex[])
{
	BYTE cbMagicCount = cbCardIndex[m_cbMagicIndex];

	return cbMagicCount;
}

//����,������ֵ����
bool CGameLogic::SortCardList(BYTE cbCardData[MAX_COUNT], BYTE cbCardCount)
{
	//��Ŀ����
	if (cbCardCount == 0 || cbCardCount > MAX_COUNT) return false;

	//�������
	bool bSorted = true;
	BYTE cbSwitchData = 0, cbLast = cbCardCount - 1;
	do
	{
		bSorted = true;
		for (BYTE i = 0; i < cbLast; i++)
		{
			if (cbCardData[i] > cbCardData[i + 1])
			{
				//���ñ�־
				bSorted = false;

				//�˿�����
				cbSwitchData = cbCardData[i];
				cbCardData[i] = cbCardData[i + 1];
				cbCardData[i + 1] = cbSwitchData;
			}
		}
		cbLast--;
	} while (bSorted == false);

	return true;
}




/*
//�����齫
��������
*/

//��һɫ
bool CGameLogic::IsQingYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE cbCurrentCard)
{
	if (IsQingFengZi(cbCardIndex, WeaveItem, cbWeaveCount, cbCurrentCard)) return false;

	//��ʱ����
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//��������
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//�����ж�
	BYTE cbCardColor = 0xFF;

	//�������ϵ���
	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		//����������
		if (i == m_cbMagicIndex) continue;

		//������Ϊ0
		if (cbCardIndexTemp[i] != 0)
		{
			//��ɫ�ж�
			if (cbCardColor != 0xFF)
				return false;

			//���û�ɫ
			cbCardColor = (SwitchToCardData(i)&MASK_COLOR);

			//��������
			i = (i / 9 + 1) * 9 - 1;
		}
	}

	//�������ֻ��������
	if (cbCardColor == 0xFF)
	{
		ASSERT(m_cbMagicIndex != MAX_INDEX && cbCardIndexTemp[m_cbMagicIndex] > 0);
		//������
		ASSERT(cbWeaveCount > 0);
		cbCardColor = WeaveItem[0].cbCenterCard&MASK_COLOR;
	}

	//����ж�
	for (BYTE i = 0; i < cbWeaveCount; i++)
	{
		BYTE cbCenterCard = WeaveItem[i].cbCenterCard;
		if ((cbCenterCard&MASK_COLOR) != cbCardColor)	return false;
	}

	return true;
}

//�����
bool CGameLogic::IsQingFengZi(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE cbCurrentCard)
{
	//��ʱ����
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//��������
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	BYTE cbCardColor = SwitchToCardData(m_cbMagicIndex)&MASK_COLOR;

	//�������ϵ���
	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		//����������
		if (i == m_cbMagicIndex) continue;

		if (cbCardIndexTemp[i] != 0)
		{
			BYTE cardColor = SwitchToCardData(i)&MASK_COLOR;
			if (cardColor != cbCardColor)
				return false;
		}
	}

	//�������ֻ������
	if (cbCardColor == SwitchToCardData(m_cbMagicIndex)&MASK_COLOR)
	{
		ASSERT(m_cbMagicIndex != MAX_INDEX && cbCardIndexTemp[m_cbMagicIndex] > 0);
		//������
		ASSERT(cbWeaveCount > 0);
		cbCardColor = WeaveItem[0].cbCenterCard&MASK_COLOR;
	}


	//����ж�
	for (BYTE i = 0; i < cbWeaveCount; i++)
	{
		BYTE cbCenterCard = WeaveItem[i].cbCenterCard;
		if ((cbCenterCard&MASK_COLOR) != cbCardColor)	return false;
	}

	return true;
}

//�˶�
bool CGameLogic::IsBaDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE cbCurrentCard, bool &bBaoTou, bool &bCanPiao)
{
	//�����г���
	if (cbWeaveCount != 0) return false;

	bBaoTou = true;

	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		if (i == m_cbMagicIndex) continue;

		if (cbCardIndex[i] != 0)
		{
			if (cbCardIndex[i] == 2 || cbCardIndex[i] == 4) continue;

			bBaoTou = false;
		}
	}

	if (GetMagicCount(cbCardIndex) != 0)
		bBaoTou = true;


	//��ʱ����
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//��������
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//ͳ�Ƶ����ƣ��������Ƶ���Ŀ���������һ�Ի���ƣ�������
	BYTE cbReplaceCount = 0;

	//�������ϵ���
	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		//ÿ���Ƶ���Ŀ
		BYTE cbCardCount = cbCardIndexTemp[i];

		//���ƹ���
		if (i == m_cbMagicIndex) continue;

		//����ͳ��
		if (cbCardCount == 1 || cbCardCount == 3) 	cbReplaceCount++;
	}

	//ȥ������������ͬ��Ŀ��ʣ�����������Ҫ���ŵ���Ŀ����ĿΪ1������true
	if (cbReplaceCount > cbCardIndexTemp[m_cbMagicIndex] + 1)
		return false;

	bCanPiao = false;
	if (cbCardIndexTemp[m_cbMagicIndex] - cbReplaceCount > 0)
		bCanPiao = true;

	return true;
}

//��ͷ
bool CGameLogic::IsBaoTou(const tagAnalyseItem *pAnalyseItem, const BYTE cbCurrentCard)
{
	//�������û��������
	if (!pAnalyseItem->bMagicEye)
		return false;
	if (IsMagicCard(pAnalyseItem->cbCardEye))
		return true;
	if (SwitchToCardIndex(cbCurrentCard) == m_cbMagicIndex && pAnalyseItem->cbCardEye != SwitchToCardData(m_cbMagicIndex))
		return false;
	if (SwitchToCardIndex(cbCurrentCard) != m_cbMagicIndex && pAnalyseItem->cbCardEye != cbCurrentCard)
		return false;

	return true;
}

bool CGameLogic::IsBaoTou(const BYTE cbCardIndex[], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE cbCurrentCard)
{
	//���ñ���
	CAnalyseItemArray AnalyseItemArray;
	AnalyseItemArray.RemoveAll();

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
	if (cbCurrentCard != 0)
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	AnalyseCard(cbCardIndexTemp, WeaveItem, cbWeaveCount, AnalyseItemArray);

	//��ͷ
	for (INT_PTR i = 0; i < AnalyseItemArray.GetCount(); i++)
	{
		//��������
		tagAnalyseItem *pAnalyseItem = &AnalyseItemArray[i];

		if (IsBaoTou(pAnalyseItem,cbCurrentCard))
			return true;
	}

	return false;
}

//Ʈ��
bool CGameLogic::CanPiaoCai(const BYTE cbCardIndex[], const BYTE cbCurrentCard)
{
	//��ʱ����
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//��������
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;



	//Ʈ��
	bool bCanPiao = false;

	if (GetMagicCount(cbCardIndexTemp) > 1)
		bCanPiao = true;


	return bCanPiao;
}

//ʮ������
bool CGameLogic::IsShiSanBuDa(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE cbCurrentCard)
{
	//�����г�����
	if (cbWeaveCount != 0) return false;

	//��ʱ����
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//��������
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	BYTE cbJiangCount = 0;

	//�������ϵ���
	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		if (i == m_cbMagicIndex) continue;

		BYTE cbCardCount = cbCardIndexTemp[i];

		if (cbCardCount > 2)
			return false;

		if (cbCardCount == 2)
			cbJiangCount++;
	}

	if (cbJiangCount > 1)
		return false;
	if (cbJiangCount == 0 && cbCardIndexTemp[m_cbMagicIndex] <= 1)
		return false;

	BYTE cbReplaceCount = 0;
	//�ж�����
	//��ɫ1
	//1.2.3
	if (cbCardIndexTemp[0] == 0 && cbCardIndexTemp[1] == 0 && cbCardIndexTemp[2] == 0)
	{
		if (cbCardIndexTemp[m_cbMagicIndex] > 1)
			cbReplaceCount++;
		else
			return false;
	}
	else
	{
		//1
		if (cbCardIndexTemp[0] != 0)
		{
			if (cbCardIndexTemp[1] != 0 || cbCardIndexTemp[2] != 0)	return false;

			//4.5.6
			if (cbCardIndexTemp[3] == 0 && cbCardIndexTemp[4] == 0 && cbCardIndexTemp[5] == 0)
			{
				if (cbCardIndexTemp[m_cbMagicIndex] > 1)
					cbReplaceCount++;
				else
					return false;
			}
			else
			{
				//147.148.149
				if (cbCardIndexTemp[3] != 0)
				{
					if (cbCardIndexTemp[4] != 0 || cbCardIndexTemp[5] != 0)	return false;

					if (cbCardIndexTemp[6] == 0 && cbCardIndexTemp[7] == 0 && cbCardIndexTemp[8] == 0)
					{
						if (cbCardIndexTemp[m_cbMagicIndex] > 1)
							cbReplaceCount++;
						else
							return false;
					}
					else
					{
						if (cbCardIndexTemp[6] != 0)
						{
							if (cbCardIndexTemp[7] != 0 || cbCardIndexTemp[8] != 0)	return false;
						}
						else if (cbCardIndexTemp[7] != 0)
						{
							if (cbCardIndexTemp[8] != 0) return false;
						}
					}
				}
				//158.159
				else if (cbCardIndexTemp[4] != 0)
				{
					if (cbCardIndexTemp[5] != 0 || cbCardIndexTemp[6] != 0)	return false;

					if (cbCardIndexTemp[7] == 0 && cbCardIndexTemp[8] == 0)
					{
						if (cbCardIndexTemp[m_cbMagicIndex] > 1)
							cbReplaceCount++;
						else
							return false;
					}
					else
					{
						if (cbCardIndexTemp[7] != 0)
						{
							if (cbCardIndexTemp[8] != 0) return false;
						}
					}
				}
				//169
				else
				{
					if (cbCardIndexTemp[6] != 0 || cbCardIndexTemp[7] != 0)	return false;

					if (cbCardIndexTemp[8] == 0)
					{
						if (cbCardIndexTemp[m_cbMagicIndex] > 1)
							cbReplaceCount++;
						else
							return false;
					}
				}
			}
		}
		//2
		else if (cbCardIndexTemp[1] != 0)
		{
			if (cbCardIndexTemp[2] != 0 || cbCardIndexTemp[3] != 0 || cbCardIndexTemp[6] != 0)	return false;

			//5.6
			if (cbCardIndexTemp[4] == 0 && cbCardIndexTemp[5] == 0)
			{
				if (cbCardIndexTemp[m_cbMagicIndex] > 1)
					cbReplaceCount++;
				else
					return false;
			}
			else
			{
				//258.259
				if (cbCardIndexTemp[4] != 0)
				{
					if (cbCardIndexTemp[5] != 0) return false;

					if (cbCardIndexTemp[7] == 0 && cbCardIndexTemp[8] == 0)
					{
						if (cbCardIndexTemp[m_cbMagicIndex] > 1)
							cbReplaceCount++;
						else
							return false;
					}
					else
					{
						if (cbCardIndexTemp[7] != 0)
						{
							if (cbCardIndexTemp[8] != 0) return false;
						}
					}
				}
				//269
				else
				{
					if (cbCardIndexTemp[7] != 0)	return false;

					if (cbCardIndexTemp[8] == 0)
					{
						if (cbCardIndexTemp[m_cbMagicIndex] > 1)
							cbReplaceCount++;
						else
							return false;
					}
				}
			}
		}
		//369
		else
		{
			if (cbCardIndexTemp[3] != 0 || cbCardIndexTemp[4] != 0 || cbCardIndexTemp[6] != 0 || cbCardIndexTemp[7] != 0)	return false;

			if (cbCardIndexTemp[5] == 0)
			{
				if (cbCardIndexTemp[m_cbMagicIndex] > 1)
					cbReplaceCount++;
				else
					return false;
			}
			else
			{
				if (cbCardIndexTemp[8] == 0)
				{
					if (cbCardIndexTemp[m_cbMagicIndex] > 1)
						cbReplaceCount++;
					else
						return false;
				}
			}
		}
	}

	//��ɫ2
	//1.2.3
	if (cbCardIndexTemp[9] == 0 && cbCardIndexTemp[10] == 0 && cbCardIndexTemp[11] == 0)
	{
		if (cbCardIndexTemp[m_cbMagicIndex] > 1)
			cbReplaceCount++;
		else
			return false;
	}
	else
	{
		//1
		if (cbCardIndexTemp[9] != 0)
		{
			if (cbCardIndexTemp[10] != 0 || cbCardIndexTemp[11] != 0)	return false;

			//4.5.6
			if (cbCardIndexTemp[12] == 0 && cbCardIndexTemp[13] == 0 && cbCardIndexTemp[14] == 0)
			{
				if (cbCardIndexTemp[m_cbMagicIndex] > 1)
					cbReplaceCount++;
				else
					return false;
			}
			else
			{
				//147.148.149
				if (cbCardIndexTemp[12] != 0)
				{
					if (cbCardIndexTemp[13] != 0 || cbCardIndexTemp[14] != 0)	return false;

					if (cbCardIndexTemp[15] == 0 && cbCardIndexTemp[16] == 0 && cbCardIndexTemp[17] == 0)
					{
						if (cbCardIndexTemp[m_cbMagicIndex] > 1)
							cbReplaceCount++;
						else
							return false;
					}
					else
					{
						if (cbCardIndexTemp[15] != 0)
						{
							if (cbCardIndexTemp[16] != 0 || cbCardIndexTemp[17] != 0)	return false;
						}
						else if (cbCardIndexTemp[16] != 0)
						{
							if (cbCardIndexTemp[17] != 0) return false;
						}
					}
				}
				//158.159
				else if (cbCardIndexTemp[13] != 0)
				{
					if (cbCardIndexTemp[14] != 0 || cbCardIndexTemp[15] != 0)	return false;

					if (cbCardIndexTemp[16] == 0 && cbCardIndexTemp[17] == 0)
					{
						if (cbCardIndexTemp[m_cbMagicIndex] > 1)
							cbReplaceCount++;
						else
							return false;
					}
					else
					{
						if (cbCardIndexTemp[16] != 0)
						{
							if (cbCardIndexTemp[17] != 0) return false;
						}
					}
				}
				//169
				else
				{
					if (cbCardIndexTemp[15] != 0 || cbCardIndexTemp[16] != 0)	return false;

					if (cbCardIndexTemp[17] == 0)
					{
						if (cbCardIndexTemp[m_cbMagicIndex] > 1)
							cbReplaceCount++;
						else
							return false;
					}
				}
			}
		}
		//2
		else if (cbCardIndexTemp[10] != 0)
		{
			if (cbCardIndexTemp[11] != 0 || cbCardIndexTemp[12] != 0 || cbCardIndexTemp[15] != 0)	return false;

			//5.6
			if (cbCardIndexTemp[13] == 0 && cbCardIndexTemp[14] == 0)
			{
				if (cbCardIndexTemp[m_cbMagicIndex] > 1)
					cbReplaceCount++;
				else
					return false;
			}
			else
			{
				//258.259
				if (cbCardIndexTemp[13] != 0)
				{
					if (cbCardIndexTemp[14] != 0) return false;

					if (cbCardIndexTemp[16] == 0 && cbCardIndexTemp[17] == 0)
					{
						if (cbCardIndexTemp[m_cbMagicIndex] > 1)
							cbReplaceCount++;
						else
							return false;
					}
					else
					{
						if (cbCardIndexTemp[16] != 0)
						{
							if (cbCardIndexTemp[17] != 0) return false;
						}
					}
				}
				//269
				else
				{
					if (cbCardIndexTemp[16] != 0)	return false;

					if (cbCardIndexTemp[17] == 0)
					{
						if (cbCardIndexTemp[m_cbMagicIndex] > 1)
							cbReplaceCount++;
						else
							return false;
					}
				}
			}
		}
		//369
		else
		{
			if (cbCardIndexTemp[12] != 0 || cbCardIndexTemp[13] != 0 || cbCardIndexTemp[15] != 0 || cbCardIndexTemp[16] != 0)	return false;

			if (cbCardIndexTemp[14] == 0)
			{
				if (cbCardIndexTemp[m_cbMagicIndex] > 1)
					cbReplaceCount++;
				else
					return false;
			}
			else
			{
				if (cbCardIndexTemp[17] == 0)
				{
					if (cbCardIndexTemp[m_cbMagicIndex] > 1)
						cbReplaceCount++;
					else
						return false;
				}
			}
		}
	}

	//��ɫ3
	//1.2.3
	if (cbCardIndexTemp[18] == 0 && cbCardIndexTemp[19] == 0 && cbCardIndexTemp[20] == 0)
	{
		if (cbCardIndexTemp[m_cbMagicIndex] > 1)
			cbReplaceCount++;
		else
			return false;
	}
	else
	{
		//1
		if (cbCardIndexTemp[18] != 0)
		{
			if (cbCardIndexTemp[19] != 0 || cbCardIndexTemp[20] != 0)	return false;

			//4.5.6
			if (cbCardIndexTemp[21] == 0 && cbCardIndexTemp[22] == 0 && cbCardIndexTemp[23] == 0)
			{
				if (cbCardIndexTemp[m_cbMagicIndex] > 1)
					cbReplaceCount++;
				else
					return false;
			}
			else
			{
				//147.148.149
				if (cbCardIndexTemp[21] != 0)
				{
					if (cbCardIndexTemp[22] != 0 || cbCardIndexTemp[23] != 0)	return false;

					if (cbCardIndexTemp[24] == 0 && cbCardIndexTemp[25] == 0 && cbCardIndexTemp[26] == 0)
					{
						if (cbCardIndexTemp[m_cbMagicIndex] > 1)
							cbReplaceCount++;
						else
							return false;
					}
					else
					{
						if (cbCardIndexTemp[24] != 0)
						{
							if (cbCardIndexTemp[25] != 0 || cbCardIndexTemp[26] != 0)	return false;
						}
						else if (cbCardIndexTemp[25] != 0)
						{
							if (cbCardIndexTemp[26] != 0) return false;
						}
					}
				}
				//158.159
				else if (cbCardIndexTemp[22] != 0)
				{
					if (cbCardIndexTemp[23] != 0 || cbCardIndexTemp[24] != 0)	return false;

					if (cbCardIndexTemp[25] == 0 && cbCardIndexTemp[26] == 0)
					{
						if (cbCardIndexTemp[m_cbMagicIndex] > 1)
							cbReplaceCount++;
						else
							return false;
					}
					else
					{
						if (cbCardIndexTemp[25] != 0)
						{
							if (cbCardIndexTemp[26] != 0) return false;
						}
					}
				}
				//169
				else
				{
					if (cbCardIndexTemp[24] != 0 || cbCardIndexTemp[25] != 0)	return false;

					if (cbCardIndexTemp[26] == 0)
					{
						if (cbCardIndexTemp[m_cbMagicIndex] > 1)
							cbReplaceCount++;
						else
							return false;
					}
				}
			}
		}
		//2
		else if (cbCardIndexTemp[19] != 0)
		{
			if (cbCardIndexTemp[20] != 0 || cbCardIndexTemp[21] != 0 || cbCardIndexTemp[24] != 0)	return false;

			//5.6
			if (cbCardIndexTemp[22] == 0 && cbCardIndexTemp[23] == 0)
			{
				if (cbCardIndexTemp[m_cbMagicIndex] > 1)
					cbReplaceCount++;
				else
					return false;
			}
			else
			{
				//258.259
				if (cbCardIndexTemp[22] != 0)
				{
					if (cbCardIndexTemp[23] != 0) return false;

					if (cbCardIndexTemp[25] == 0 && cbCardIndexTemp[26] == 0)
					{
						if (cbCardIndexTemp[m_cbMagicIndex] > 1)
							cbReplaceCount++;
						else
							return false;
					}
					else
					{
						if (cbCardIndexTemp[25] != 0)
						{
							if (cbCardIndexTemp[26] != 0) return false;
						}
					}
				}
				//269
				else
				{
					if (cbCardIndexTemp[25] != 0)	return false;

					if (cbCardIndexTemp[26] == 0)
					{
						if (cbCardIndexTemp[m_cbMagicIndex] > 1)
							cbReplaceCount++;
						else
							return false;
					}
				}
			}
		}
		//369
		else
		{
			if (cbCardIndexTemp[21] != 0 || cbCardIndexTemp[22] != 0 || cbCardIndexTemp[24] != 0 || cbCardIndexTemp[25] != 0)	return false;

			if (cbCardIndexTemp[23] == 0)
			{
				if (cbCardIndexTemp[m_cbMagicIndex] > 1)
					cbReplaceCount++;
				else
					return false;
			}
			else
			{
				if (cbCardIndexTemp[26] == 0)
				{
					if (cbCardIndexTemp[m_cbMagicIndex] > 1)
						cbReplaceCount++;
					else
						return false;
				}
			}
		}
	}

	//���Ʒ���
	for (BYTE i = 27; i < MAX_INDEX; i++)
	{
		if (i == m_cbMagicIndex) continue;

		BYTE cbCardCount = cbCardIndexTemp[i];

		if (cbCardCount == 0)
			cbReplaceCount++;
	}

	if (cbReplaceCount + 1 > cbCardIndexTemp[m_cbMagicIndex]) return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
