----------------------------------------------------------------------
-- ��Ȩ��2017
-- ʱ�䣺2017-08-18
-- ��;���û���ֵ
----------------------------------------------------------------------

USE [QPTreasureDB_HideSeek]
GO


-- �û���ֵ
IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].GSP_GP_BoughtTaggerModel') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].GSP_GP_BoughtTaggerModel
GO

-- ��ʯ��Ҷһ�
IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].GSP_GP_ExchangScoreInfo') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].GSP_GP_ExchangScoreInfo
GO

--  ִ�н��� WQ
IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_AwardDone]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_AwardDone]
GO
	
---------------------------------------------------------------------------------------
-- �û���ֵ
CREATE PROCEDURE GSP_GP_BoughtTaggerModel
	@dwUserID			INT,					-- �û���ʶ
	@strPassword		NCHAR(32),				-- �û�����
	@wBoughtModelIndex	SMALLINT,				-- ����ľ���ģ��
	@dwPayment			INT,					-- ������
	@cbPaymentType		TINYINT,				-- �������ͣ�0-��ң�1-��ʯ��2-�ֽ�
	@lModelIndexToStore	BIGINT,					-- �����洢��AccountsTaggerModel��ֵ
	@strErrorDescribe	NVARCHAR(255) OUTPUT	-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ������Ϣ
DECLARE @UserID INT
DECLARE @Nullity TINYINT
DECLARE @SpreaderID INT

-- ִ���߼�
BEGIN	
	
	-- ��֤�û�
	SELECT @UserID=UserID,@Nullity=Nullity
	FROM QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.AccountsInfo
	WHERE UserID=@dwUserID

	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'��Ǹ�������û��˺Ų����ڡ�'
		RETURN 1
	END

	IF @Nullity=1
	BEGIN
		SET @strErrorDescribe=N'��Ǹ�������û��˺���ʱ���ڶ���״̬������ϵ�ͻ����������˽���ϸ�����'
		RETURN 2
	END	
	
	
	-- ���ô���
	DECLARE @Score BIGINT
	DECLARE @Insure BIGINT
	SELECT @Score=Score,@Insure=InsureScore FROM GameScoreInfo(NOLOCK) WHERE UserID=@UserID
	IF @cbPaymentType=0
	BEGIN
		--����ж�

		IF @Score IS NULL
		BEGIN
			SET @strErrorDescribe=N'��Ǹ�������ʽ��˺Ų����ڡ�'
			RETURN 3
		END
		ELSE IF @Score<@dwPayment
		BEGIN
			-- ������Ϣ
			SET @strErrorDescribe=N'���Ľ�Ҳ��㣡'
			RETURN 4
		END
		
		-- �۳�����
		SET @Score = @Score-@dwPayment
		UPDATE GameScoreInfo SET Score=@Score WHERE UserID=@UserID
	END
	ELSE IF @cbPaymentType=1
	BEGIN
		--����ж�

		IF @Insure IS NULL
		BEGIN
			SET @strErrorDescribe=N'��Ǹ�������ʽ��˺Ų����ڡ�'
			RETURN 5
		END
		ELSE IF @Insure<@dwPayment
		BEGIN
			-- ������Ϣ
			SET @strErrorDescribe=N'������ʯ���㣡'
			RETURN 6
		END
		
		-- �۳�����
		SET @Insure = @Insure-@dwPayment
		UPDATE GameScoreInfo SET InsureScore=@Insure WHERE UserID=@UserID
	END
	ELSE IF @cbPaymentType<>2
	BEGIN
			SET @strErrorDescribe=N'�������Ͳ��ԣ�'
			RETURN 7
	END
		

	-- �����¼
	DECLARE @Note NVARCHAR(128)
	DECLARE @RecordID INT
	IF @cbPaymentType=0
	BEGIN
		SET @Note = N'����'+LTRIM(STR(@dwPayment))+'��ң�����'+LTRIM(STR(@wBoughtModelIndex))+'�ž���'
	END 
	ELSE IF @cbPaymentType=1
	BEGIN 
		SET @Note = N'����'+LTRIM(STR(@dwPayment))+'��ʯ������'+LTRIM(STR(@wBoughtModelIndex))+'�ž���'
	END 
	ELSE IF @cbPaymentType=2
	BEGIN
		SET @Note = N'����'+LTRIM(STR(@dwPayment))+'Ԫ������'+LTRIM(STR(@wBoughtModelIndex))+'�ž���'
	END
	INSERT INTO RecordBoughtTaggerModel(UserID,BoughtModelIndex,Payment,PaymentType,CollectNote)
	VALUES(@UserID,@wBoughtModelIndex,@dwPayment,@cbPaymentType,@Note)	
	SET @RecordID=SCOPE_IDENTITY()
	
	-- �����û�ģ�ͱ�
	IF @wBoughtModelIndex < 64
	BEGIN
		DECLARE @ModelIndex0 BIGINT
		SELECT @ModelIndex0=ModelIndex0 FROM QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.AccountsTaggerModel WHERE UserID=@UserID
		IF @ModelIndex0 IS NULL
		BEGIN
			INSERT INTO QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.AccountsTaggerModel(UserID,ModelIndex0)
			VALUES(@UserID,@lModelIndexToStore)	
		END
		ELSE
		BEGIN
			UPDATE QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.AccountsTaggerModel SET ModelIndex0=ModelIndex0 | @lModelIndexToStore WHERE UserID=@UserID 
		END
	END
	ELSE IF @wBoughtModelIndex < 128
	BEGIN
		DECLARE @ModelIndex1 BIGINT
		SELECT @ModelIndex1=ModelIndex1 FROM QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.AccountsTaggerModel WHERE UserID=@UserID
		IF @ModelIndex1 IS NULL
		BEGIN
			INSERT INTO QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.AccountsTaggerModel(UserID,ModelIndex1)
			VALUES(@UserID,@lModelIndexToStore)	
		END
		ELSE
		BEGIN
			UPDATE QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.AccountsTaggerModel SET ModelIndex1=ModelIndex1 | @lModelIndexToStore WHERE UserID=@UserID 
		END
	END
	ELSE IF @wBoughtModelIndex < 192
	BEGIN
		DECLARE @ModelIndex2 BIGINT
		SELECT @ModelIndex2=ModelIndex2 FROM QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.AccountsTaggerModel WHERE UserID=@UserID
		IF @ModelIndex2 IS NULL
		BEGIN
			INSERT INTO QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.AccountsTaggerModel(UserID,ModelIndex2)
			VALUES(@UserID,@lModelIndexToStore)	
		END
		ELSE
		BEGIN
			UPDATE QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.AccountsTaggerModel SET ModelIndex2=ModelIndex2 | @lModelIndexToStore WHERE UserID=@UserID 
		END
	END
	
	SET @strErrorDescribe=N'����ɹ�:'+@Note

	SELECT @UserID AS UserID, @Score AS Score, @Insure AS Insure 
	
END 

RETURN 0
GO

-------------------------------------------------------------------------------------------
-- ��ʯ��Ҷһ�
CREATE PROCEDURE GSP_GP_ExchangScoreInfo
	@dwUserID			INT,					-- �û���ʶ
	@cbItemID           TINYINT,                -- ��ƷID
	@wAmount            SMALLINT,               -- ���
	@cbExchangeType     TINYINT,                -- �һ�����
	@strErrorDescribe	NVARCHAR(255) OUTPUT	-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ������Ϣ
DECLARE @UserID INT
DECLARE @Nullity TINYINT

-- ִ���߼�
BEGIN	
	
	-- ��֤�û�
	SELECT @UserID=UserID,@Nullity=Nullity
	FROM QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.AccountsInfo
	WHERE UserID=@dwUserID

	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'��Ǹ�������û��˺Ų����ڡ�'
		RETURN 1
	END

	IF @Nullity=1
	BEGIN
		SET @strErrorDescribe=N'��Ǹ�������û��˺���ʱ���ڶ���״̬������ϵ�ͻ����������˽���ϸ�����'
		RETURN 2
	END	
	
	
	-- ���ô���
	DECLARE @Score BIGINT
	DECLARE @Insure BIGINT
	SELECT @Score=Score,@Insure=InsureScore FROM GameScoreInfo(NOLOCK) WHERE UserID=@UserID
	IF @cbExchangeType=0 --���ת��ʯ
	BEGIN
		--����ж�

		IF @Score IS NULL
		BEGIN
			SET @strErrorDescribe=N'��Ǹ�������ʽ��˺Ų����ڡ�'
			RETURN 3
		END
		ELSE IF @Score<@wAmount
		BEGIN
			-- ������Ϣ
			SET @strErrorDescribe=N'���Ľ�Ҳ��㣡'
			RETURN 4
		END
		
		-- �۳�����
		SET @Score = @Score-@wAmount
		UPDATE GameScoreInfo SET Score=@Score,InsureScore=InsureScore+@wAmount*100 WHERE UserID=@UserID
	END
	ELSE IF @cbExchangeType=1 --��ʯת���
	BEGIN
		--����ж�

		IF @Insure IS NULL
		BEGIN
			SET @strErrorDescribe=N'��Ǹ�������ʽ��˺Ų����ڡ�'
			RETURN 5
		END
		ELSE IF @Insure<@wAmount
		BEGIN
			-- ������Ϣ
			SET @strErrorDescribe=N'������ʯ���㣡'
			RETURN 6
		END
		
		-- �۳�����
		SET @Insure = @Insure-@wAmount
		UPDATE GameScoreInfo SET InsureScore=@Insure,Score=Score+@wAmount*100 WHERE UserID=@UserID
	END
		

	-- �����¼
	DECLARE @Note NVARCHAR(128)
	DECLARE @RecordID INT
	IF @cbExchangeType=0
	BEGIN
		SET @Note = N'����'+LTRIM(STR(@wAmount))+'��ң�������ʯ'
	END 
	ELSE IF @cbExchangeType=1
	BEGIN 
		SET @Note = N'����'+LTRIM(STR(@wAmount))+'��ʯ��������'
	END 
	INSERT INTO RecordExchangeScore(UserID,ItemID,ExchangeType,CollectNote)
	VALUES(@UserID,@cbItemID,@cbExchangeType,@Note)	
	SET @RecordID=SCOPE_IDENTITY()

	SET @strErrorDescribe=N'����ɹ�:'+@Note

	SELECT @Score=Score,@Insure=InsureScore
	FROM GameScoreInfo
	WHERE @UserID=UserID
	SELECT @cbItemID AS ItemID, @Score AS FinalScore, @Insure AS FinalInsure 
	
END 

RETURN 0
GO

----------------------------------------------------------------------------------------------------

-- ��ȡ����
CREATE PROC GSP_GR_AwardDone
	@dwUserID INT,								-- �û� I D
	@dwAwardGold INT,							-- �������
	@cbCostType TINYINT,                        -- ��������
	@strPassword NCHAR(32),						-- �û�����
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

DECLARE @lScore BIGINT
	
-- ִ���߼�
BEGIN

	-- ��ѯ�û�
	IF not exists(SELECT * FROM QPAccountsDB_HideSeek.dbo.AccountsInfo WHERE UserID=@dwUserID AND LogonPass=@strPassword)
	BEGIN
		SET @strErrorDescribe = N'��Ǹ������û���Ϣ�����ڻ������벻��ȷ��'
		return 1
	END
	
	IF @cbCostType=0
	BEGIN
		-- �������	
		UPDATE GameScoreInfo SET Score = Score + @dwAwardGold WHERE UserID = @dwUserID
		IF @@rowcount = 0
		BEGIN
			-- ��������
			INSERT INTO GameScoreInfo (UserID,Score)
			VALUES (@dwUserID, @dwAwardGold)
		END
		SET @strErrorDescribe = N'������� '+convert(varchar,@dwAwardGold)+N' ö��ң�'

		-- ��ѯ��ʯ
		SELECT @lScore=Score FROM GameScoreInfo WHERE UserID = @dwUserID 	

	END
	ELSE IF @cbCostType=1
	BEGIN
		-- ������ʯ	
		UPDATE GameScoreInfo SET InsureScore = InsureScore + @dwAwardGold WHERE UserID = @dwUserID
		IF @@rowcount = 0
		BEGIN
			-- ��������
			INSERT INTO GameScoreInfo (UserID,InsureScore)
			VALUES (@dwUserID, @dwAwardGold)
		END
		
		SET @strErrorDescribe = N'������� '+convert(varchar,@dwAwardGold)+N' ����ʯ��'

		-- ��ѯ��ʯ
		SELECT @lScore=InsureScore FROM GameScoreInfo WHERE UserID = @dwUserID 	

	END
	IF @lScore IS NULL SET @lScore = 0
	
	-- �׳�����
	SELECT @lScore AS Score, @cbCostType As CostType	
END

RETURN 0

GO


