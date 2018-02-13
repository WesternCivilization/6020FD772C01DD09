
----------------------------------------------------------------------------------------------------

USE QPAccountsDB_HideSeek
GO


IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_RaffleDone]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_RaffleDone]
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO


----------------------------------------------------------------------------------------------------

-- ��ȡ����
CREATE PROC GSP_GR_RaffleDone
	@dwUserID INT,								-- �û� I D
	@dwRaffleGold INT,							-- �鵽����ʯ
	@strPassword NCHAR(32),						-- �û�����
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strMachineID NVARCHAR(32),					-- ������ʶ
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ��ѯ�û�
	IF not exists(SELECT * FROM AccountsInfo WHERE UserID=@dwUserID AND LogonPass=@strPassword)
	BEGIN
		SET @strErrorDescribe = N'��Ǹ������û���Ϣ�����ڻ������벻��ȷ��'
		return 1
	END

	-- ÿ������ٳ����Գ齱һ��
	DECLARE @PlayCountPerRaffle AS INT
	SELECT @PlayCountPerRaffle=StatusValue FROM SystemStatusInfo WHERE StatusName=N'PlayCountPerRaffle'
	IF @PlayCountPerRaffle IS NULL 
	BEGIN
		SET @strErrorDescribe = N'�齱δ����PlayCountPerRaffle��'
		return 2		
	END
	
	-- �齱��¼��ѯ
	DECLARE @RaffleCount INT	
	SELECT @RaffleCount=RaffleCount FROM AccountsRaffle WHERE UserID=@dwUserID
	IF @RaffleCount IS NULL
	BEGIN
		SET @RaffleCount = 0
		
		--��������
		INSERT INTO AccountsRaffle VALUES(@dwUserID,GetDate(),@RaffleCount,0,@PlayCountPerRaffle)		
	END

	SET @RaffleCount = @RaffleCount+1
		
	--��ѯ�û��Ѵ򳡴�
	DECLARE @PlayCount INT
	SELECT  @PlayCount=COUNT(*) FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.PrivateGameRecordUserRecordID WHERE UserID = @dwUserID
	
	--�Ƿ�����ɳ齱����
	DECLARE @MinPlayCountToRaffle INT
	SET @MinPlayCountToRaffle = @RaffleCount * @PlayCountPerRaffle
	IF @PlayCount < @MinPlayCountToRaffle
	BEGIN
		--SET @strErrorDescribe = N'δ����'+convert(varchar,@MinPlayCountToRaffle)+N'�����ܳ齱��'
		SET @strErrorDescribe = N'δ����'+convert(varchar,@PlayCountPerRaffle)+N'�����ܳ齱��'
		return 3		
	END
	
	-- ������ʯ	
	UPDATE QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo SET InsureScore = InsureScore + @dwRaffleGold WHERE UserID = @dwUserID
	IF @@rowcount = 0
	BEGIN
		-- ��������
		INSERT INTO QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo (UserID,InsureScore, LastLogonIP, LastLogonMachine, RegisterIP, RegisterMachine)
		VALUES (@dwUserID, @dwRaffleGold, @strClientIP, @strMachineID, @strClientIP, @strMachineID)
	END

	-- ���¼�¼
	UPDATE AccountsRaffle SET LastDateTime = GetDate(),RaffleCount = @RaffleCount, RewardGold = RewardGold + @dwRaffleGold  WHERE UserID = @dwUserID
	
	SET @strErrorDescribe = N'�齱�ɹ�!��� '+convert(varchar,@dwRaffleGold)+N' ����ʯ��'

	-- ��ѯ��ʯ
	DECLARE @lScore BIGINT	
	SELECT @lScore=InsureScore FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo WHERE UserID = @dwUserID 	
	IF @lScore IS NULL SET @lScore = 0
	
	-- �׳�����
	SELECT @lScore AS Score, @RaffleCount As RaffleCount, @PlayCount As PlayCount
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------