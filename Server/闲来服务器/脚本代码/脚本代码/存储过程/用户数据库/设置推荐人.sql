
----------------------------------------------------------------------------------------------------

USE QPAccountsDB_HideSeek
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_ModifySpreader]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_ModifySpreader]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_AddDelSpreader]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_AddDelSpreader]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_QuerySpreadersInfo]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_QuerySpreadersInfo]
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- ��ѯ����
CREATE PROC GSP_GP_ModifySpreader
	@dwUserID INT,								-- �û� I D
	@strPassword NCHAR(32),						-- �û�����
	@dwSpreaderId INT,							-- mChen edit, @strSpreader NVARCHAR(32),					-- �Ƽ���
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ��������
	DECLARE @LogonPass AS NCHAR(32)
	DECLARE @OldSpreaderID AS NCHAR(32)

	SET @strErrorDescribe=N'Start��'
	
	-- ��ѯ�û�
	SELECT @OldSpreaderID=SpreaderID ,@LogonPass=LogonPass FROM AccountsInfo(NOLOCK) WHERE UserID=@dwUserID

	-- �����ж�
	IF @LogonPass<>@strPassword
	BEGIN
		SET @strErrorDescribe=N'�����û����벻��ȷ��������Ϣ��ѯʧ�ܣ�'
		RETURN 1
	END
	
	-- �Ƽ���
	IF @OldSpreaderID<>0
	BEGIN
		SET @strErrorDescribe=N'���Ѿ��������Ƽ��ˣ�'
		RETURN 2
	END
	
	-- �Ƽ���
	IF @dwSpreaderId=0	--mChen edit, IF @strSpreader=N''
	BEGIN
		SET @strErrorDescribe=N'�Ƽ���Ϊ�գ�'
		RETURN 3
	END
	
	IF @dwSpreaderId = @dwUserID
	BEGIN
		SET @strErrorDescribe=N'�Ƽ��˲������Լ���'
		RETURN 4
	END
	

	-- ��ѯ�Ƽ���
	--mChen edit
	/*
	DECLARE @SpreaderUserID AS INT
	SELECT @SpreaderUserID=UserID FROM AccountsInfo WHERE UserID=@dwSpreaderId AND IsSpreader<>0
	--SELECT @SpreaderUserID=UserID FROM AccountsInfo WHERE Accounts=@strSpreader

	-- �������
	IF @SpreaderUserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'��������Ƽ�����Ч'
		--SET @strErrorDescribe=N'��������Ƽ���'+convert(varchar,@dwSpreaderId)+N'�����ڻ��߲�����Ч���Ƽ��ˣ�'
		RETURN 6
	END
	*/
	DECLARE @ResultCout AS INT
	SELECT @ResultCout=count(*) FROM SpreadersInfo WHERE SpreaderID=@dwSpreaderId
	IF @ResultCout=0--IF @@ROWCOUNT=0
	BEGIN
		SET @strErrorDescribe=N'��������Ƽ�����Ч'
		--SET @strErrorDescribe=N'��������Ƽ���'+convert(varchar,@dwSpreaderId)+N'�����ڻ��߲�����Ч���Ƽ��ˣ�'
		RETURN 6
	END

	-- �ƹ����
	DECLARE @RegisterGrantScore INT
	DECLARE @Note NVARCHAR(512)
	SET @Note = N'��'--mChen edit,N'ע��'
	SELECT @RegisterGrantScore = RegisterGrantScore FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GlobalSpreadInfo
	IF @RegisterGrantScore IS NULL
	BEGIN
		SET @RegisterGrantScore=8	--5000 --mChen edit
	END
	
	--mChen comment��To do
	INSERT INTO QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.RecordSpreadInfo(
		UserID,InsureScore,TypeID,ChildrenID,CollectNote)
	VALUES(@dwSpreaderId,@RegisterGrantScore,1,@dwUserID,@Note)		
	
	UPDATE QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo SET InsureScore=InsureScore+@RegisterGrantScore	--mChen edit, SET Score=Score+@RegisterGrantScore
	WHERE UserID=@dwUserID
	
	--����AccountsInfo
	UPDATE AccountsInfo SET SpreaderID=@dwSpreaderId WHERE UserID=@dwUserID
	
	--mChen comment
	/*
	UPDATE QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo SET Score=Score+@RegisterGrantScore
	WHERE UserID=@dwSpreaderId
	*/
	
	DECLARE @DestScore BIGINT
	--mChen edit
	SELECT @DestScore=InsureScore FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo WHERE UserID=@dwUserID
	--SELECT @DestScore=Score FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo WHERE UserID=@dwUserID
	IF @DestScore IS NULL
	BEGIN
		SET @DestScore = 0
	END
	
	--mChen edit
	SET @strErrorDescribe=N'��������ɹ�����ϲ���� '+convert(varchar,@RegisterGrantScore)+N' ����ʯ��'
	--SET @strErrorDescribe=N'�����Ƽ��˳ɹ�����ϲ���� '+convert(varchar,@RegisterGrantScore)+N' �Ľ�ң�'
	
	SELECT @dwSpreaderId AS SpreaderID, @DestScore AS DestScore
		
END

RETURN 0

GO


--mChen add,����/ɾ���Ƽ������
----------------------------------------------------------------------------------------------------

-- ��ѯ����
CREATE PROC GSP_GP_AddDelSpreader
	@dwUserID INT,								-- ִ�в������û� I D
	@strPassword NCHAR(32),						-- �û�����
	
	@dwSpreaderId INT,							-- �Ƽ���ID����������
	
	@strSpreaderRealName NVARCHAR(32),			-- �Ƽ�������
	--@strSpreaderIDCardNo NCHAR(32),			-- �Ƽ������֤��
	@strSpreaderTelNum NVARCHAR(32),			
	@strSpreaderWeiXinAccount NVARCHAR(32),			
	
	@wSpreaderLevel SMALLINT,					-- �Ƽ��˵ȼ�
	@dwParentSpreaderID INT,					-- �ϼ�����ID
		
	@bIsAddOperate BIT,							-- ����/ɾ��������
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

		-- ��������
	DECLARE @LogonPass AS NCHAR(32)

	-- ��ѯ�û�
	SELECT @LogonPass=LogonPass FROM AccountsInfo(NOLOCK) WHERE UserID=@dwUserID

	-- �����ж�
	IF @LogonPass<>@strPassword
	BEGIN
		SET @strErrorDescribe=N'�����û����벻��ȷ��'
		RETURN 1
	END
	
	-- ������Ч�Լ��
	IF @dwSpreaderId=0
	BEGIN
		SET @strErrorDescribe=N'������ID����Ϊ�գ�'
		RETURN 2
	END
	IF @strSpreaderRealName=N''
	BEGIN
		SET @strErrorDescribe=N'��������������Ϊ�գ�'
		RETURN 2
	END
	IF @strSpreaderTelNum=N''
	BEGIN
		SET @strErrorDescribe=N'�����˵绰����Ϊ�գ�'
		RETURN 2
	END
	IF @strSpreaderWeiXinAccount=N''
	BEGIN
		SET @strErrorDescribe=N'������΢�źŲ���Ϊ�գ�'
		RETURN 2
	END
	/*
	IF @strSpreaderIDCardNo=N''
	BEGIN
		SET @strErrorDescribe=N'���������֤�Ų���Ϊ�գ�'
		RETURN 2
	END
	*/
	IF @dwParentSpreaderID=0
	BEGIN
		SET @strErrorDescribe=N'�ϼ�������ID����Ϊ�գ�'
		RETURN 2
	END
	
	
	-- ���������
	DECLARE @MySpreaderLevel AS INT
	SELECT @MySpreaderLevel=SpreaderLevel FROM SpreadersInfo WHERE SpreaderID=@dwUserID
	IF @MySpreaderLevel IS NULL
	BEGIN
		SET @strErrorDescribe=N'�����Ǵ����ˣ�û��Ȩ��ִ�д˲�����strSpreaderRealName='+@strSpreaderRealName
		RETURN 3
	END
	IF @MySpreaderLevel>=@wSpreaderLevel
	BEGIN
		SET @strErrorDescribe=N'���Ĵ���ȼ��������ָ���Ĵ���ȼ���������Ȩ��ִ�д˲�����'
		RETURN 3
	END
	
	
	-- ���������
	DECLARE @ResultCout AS INT
	--DECLARE @IsSpreader AS BIT
	SELECT @ResultCout=count(*) FROM AccountsInfo WHERE UserID=@dwSpreaderId
	-- �������
	IF @ResultCout=0 --IF @@ROWCOUNT=0
	BEGIN
		SET @strErrorDescribe=N'�˴�����'+convert(varchar,@dwSpreaderId)+N'�����ڣ�'
		RETURN 4
	END
	
	-- �����ϼ��Ƽ���
	DECLARE @ParentSpreaderLevel AS INT
	SELECT @ParentSpreaderLevel=SpreaderLevel FROM SpreadersInfo WHERE SpreaderID=@dwParentSpreaderID
	IF @ParentSpreaderLevel IS NULL
	BEGIN
		SET @strErrorDescribe=N'���ϼ������Ǵ����ˣ�'
		RETURN 5
	END
	IF @MySpreaderLevel>=@ParentSpreaderLevel AND @dwUserID<>@dwParentSpreaderID	--���Ը��Լ�������Ĵ���
	BEGIN
		SET @strErrorDescribe=N'���Ĵ���ȼ�������ڴ��ϼ�����ȼ���������Ȩ��ִ�д˲�����'
		RETURN 5
	END
	IF @ParentSpreaderLevel>=@wSpreaderLevel
	BEGIN
		SET @strErrorDescribe=N'ָ���Ĵ���ȼ���������ϼ�����ȼ���'
		RETURN 5
	END


	-- ����/ɾ���Ƽ���
	IF @bIsAddOperate<>0--bIsAddOperate
	BEGIN
		-- �����Ƽ���
		
		--��ѯ�Ƽ���
		SELECT @ResultCout=count(*) FROM SpreadersInfo WHERE SpreaderID=@dwSpreaderId
		IF @ResultCout<>0--IF @@ROWCOUNT<>0
		BEGIN
			SET @strErrorDescribe=N'��Ҫ���ӵĴ����Ѿ��Ǵ����ˣ�'
			RETURN 6
		END
		
		-- �������д��������Ƿ�ﵽ����
		DECLARE @ResultSpreaderCout AS INT
		--DECLARE @IsSpreader AS BIT
		SELECT @ResultSpreaderCout=count(*) FROM SpreadersInfo
		-- �������
		IF @ResultSpreaderCout>=10000 
		BEGIN
			SET @strErrorDescribe=N'���������Ѵ�����'+convert(varchar,@ResultSpreaderCout)+N'�ˣ��޷������ӣ�'
			RETURN 6
		END
		
		--ִ�����Ӳ���
		INSERT INTO QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.SpreadersInfo(
			SpreaderID,RealName,TelNum,WeiXinAccount,SpreaderLevel,ParentID)
		VALUES(@dwSpreaderId,@strSpreaderRealName,@strSpreaderTelNum,@strSpreaderWeiXinAccount,@wSpreaderLevel,@dwParentSpreaderID)
		
		--UPDATE AccountsInfo SET IsSpreader=1 WHERE UserID=@dwSpreaderId
			
		SET @strErrorDescribe=N'���Ӵ����˳ɹ�!'
		
	END 
	ELSE
	BEGIN
		-- ɾ���Ƽ���
		
		--��ѯ�Ƽ���
		DECLARE @SpreaderLevelToDel AS INT
		DECLARE @ParentSpreaderId AS INT
		DECLARE @ExtraCash DECIMAL(16,4)	--��õĶ�����(Ԫ)������ɾ��ֱ���¼������öԷ���ʣ����
		SELECT @SpreaderLevelToDel=SpreaderLevel,@ParentSpreaderId=ParentID,@ExtraCash=ExtraCash FROM SpreadersInfo 
		WHERE SpreaderID=@dwSpreaderId AND RealName=@strSpreaderRealName
		
		IF @SpreaderLevelToDel IS NULL
		BEGIN
			SET @strErrorDescribe=N'Ҫɾ���Ĵ������Ѿ������ڣ�'
			RETURN 7
		END	
		
		-- ���������
		IF @MySpreaderLevel>=@SpreaderLevelToDel
		BEGIN
			SET @strErrorDescribe=N'���Ĵ���ȼ�����ɾ���ô����ˣ�'
			RETURN 7
		END
		
		--����ô��������ֽ��(Ԫ)
		DECLARE @CashedOut DECIMAL(16,4)
		SELECT @CashedOut=SUM(Amount) FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.RecordSpreaderCashout WHERE SpreaderID=@dwSpreaderId
		IF @CashedOut IS NULL
		BEGIN
			SET @CashedOut=0
		END	
	
		--���˴����ʣ��������ת������������
		DECLARE @TotalGrant DECIMAL(16,4)					--�ܽ�Ԫ��
		DECLARE @TotalGrantOfChildrenBuy DECIMAL(16,4)		--�����û���ֵ��õ�����ܶԪ��
		DECLARE @TotalLeftCash DECIMAL(16,4)				--ʣ���Ԫ��
		SELECT @TotalGrantOfChildrenBuy=SUM(Payment*PaymentGrantRate) FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.RecordChildrenPayment WHERE RelatedSpreaderID=@dwSpreaderId 
		IF @TotalGrantOfChildrenBuy IS NULL
		BEGIN
			SET @TotalGrantOfChildrenBuy=0
		END
		SET @TotalGrant=@TotalGrantOfChildrenBuy+@ExtraCash
		SET @TotalLeftCash=@TotalGrant-@CashedOut
		-- ���¸������@ExtraCash
		UPDATE SpreadersInfo SET ExtraCash=ExtraCash+@TotalLeftCash
		WHERE SpreaderID=@ParentSpreaderId
		IF @@ROWCOUNT=0
		BEGIN
			SET @strErrorDescribe=N'ʣ��������ת�������� ʧ��!'
			RETURN 8
		END
		
		
		--�޸Ĵ˴������Щֱ�������ParentIdΪ�˴����ParentId
		UPDATE SpreadersInfo SET ParentId=@ParentSpreaderId WHERE ParentId=@dwSpreaderId
		
		--�޸����а󶨴˴�����û��İ󶨴���Ϊ�˴����ParentId
		UPDATE AccountsInfo SET SpreaderID=@ParentSpreaderId WHERE SpreaderID=@dwSpreaderId
		--UPDATE AccountsInfo SET IsSpreader=0 WHERE UserID=@dwSpreaderId
				
		--���ɾ����¼
		INSERT INTO QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.RecordDelSpreader(OperatorUserID,SpreaderID,RealName,TelNum,WeiXinAccount,SpreaderLevel,ParentID,ExtraCash)
		VALUES(@dwUserID,@dwSpreaderId,@strSpreaderRealName,@strSpreaderTelNum,@strSpreaderWeiXinAccount,@SpreaderLevelToDel,@ParentSpreaderId,@ExtraCash)
		
		--ִ��ɾ������
		--ɾ�����ּ�¼
		DELETE QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.RecordSpreaderCashout WHERE SpreaderID=@dwSpreaderId
		--ɾ�������Ϣ
		DELETE QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.RecordChildrenPayment WHERE RelatedSpreaderID=@dwSpreaderId
		DELETE SpreadersInfo WHERE SpreaderID=@dwSpreaderId
		
		SET @strErrorDescribe=N'ɾ�������˳ɹ�!'
		
	END
	
	
	DECLARE @DestScore BIGINT
	SELECT @DestScore=InsureScore FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo WHERE UserID=@dwUserID
	IF @DestScore IS NULL
	BEGIN
		SET @DestScore = 0
	END
		
	SELECT @DestScore AS DestScore
		
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------


--mChen add,��ѯ��������Ϣ
----------------------------------------------------------------------------------------------------

-- ��ѯ����
CREATE PROC GSP_GP_QuerySpreadersInfo
	@dwUserID INT,								-- �û�ID
	@strPassword NCHAR(32),						-- �û�����
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ��������
	DECLARE @LogonPass AS NCHAR(32)

	-- ��ѯ�û�
	SELECT @LogonPass=LogonPass FROM AccountsInfo(NOLOCK) WHERE UserID=@dwUserID

	-- �����ж�
	IF @LogonPass<>@strPassword
	BEGIN
		SET @strErrorDescribe=N'�����û����벻��ȷ��'
		RETURN 1
	END
	
	-- ���������
	DECLARE @MySpreaderLevel AS INT
	SELECT @MySpreaderLevel=SpreaderLevel FROM SpreadersInfo WHERE SpreaderID=@dwUserID
	IF @MySpreaderLevel IS NULL
	BEGIN
		SET @strErrorDescribe=N'�����Ǵ����ˣ�û��Ȩ��ִ�д˲�����'
		RETURN 2
	END
	
	SET @strErrorDescribe=N'��ѯ��������Ϣ�ɹ�!'
	
	-- ��ѯ�û�
	IF @MySpreaderLevel=0
	BEGIN
		SELECT * FROM SpreadersInfo
	END ELSE 
	IF @MySpreaderLevel=1
	BEGIN	
		SELECT * FROM SpreadersInfo WHERE ParentID=@dwUserID 
		or ParentID in (SELECT SpreaderID FROM SpreadersInfo WHERE ParentID=@dwUserID)
	END ELSE
	IF @MySpreaderLevel=2
	BEGIN	
		SELECT * FROM SpreadersInfo WHERE ParentID=@dwUserID 
	END ELSE
	IF @MySpreaderLevel=3
	BEGIN	
		SET @strErrorDescribe=N'�������������������Ӵ���'
		RETURN 3
	END ELSE
	BEGIN	
		SET @strErrorDescribe=N'��Ĵ���ȼ����ԣ�'
		RETURN 4
	END
	--SELECT * FROM SpreadersInfo
		
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------
