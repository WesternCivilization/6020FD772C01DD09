USE QPTreasureDB_HideSeek
GO

-- ��ʼ���ƹ�����
TRUNCATE TABLE GlobalSpreadInfo
GO

INSERT INTO GlobalSpreadInfo(RegisterGrantScore,PlayTimeCount,PlayTimeGrantScore,FillGrantRate,BalanceRate,MinBalanceScore) 
VALUES(8,108000,100000,0.1,0.1,200000)	--mChen edit 1888

GO