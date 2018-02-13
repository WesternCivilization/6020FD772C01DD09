----------------------------------------------------------------------
-- ��Ȩ��2017
-- ʱ�䣺2017-08-18
-- ��;���û���ֵ
----------------------------------------------------------------------

USE [QPTreasureDB_HideSeek]
GO

-- ��ҵ����
IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].GSP_GP_AddEnterprisePayment') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].GSP_GP_AddEnterprisePayment
GO

-- ��ҵ���ֽ��
IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].GSP_GP_AddEnterprisePaymentResult') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].GSP_GP_AddEnterprisePaymentResult
GO

-- �û���ֵ
IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].GSP_GP_AddPayment') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].GSP_GP_AddPayment
GO

-- ��ѯ�����û�������Ϣ
IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].GSP_GP_QueryChildrenPaymentInfo') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].GSP_GP_QueryChildrenPaymentInfo
GO

-- ��Ʒ�����¼ WQ
IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].GSP_GP_ShopItemInfo') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].GSP_GP_ShopItemInfo
GO
-- �������� WQ
IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].GSP_GP_InventoryConsumptionInfo') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].GSP_GP_InventoryConsumptionInfo
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

-------��Ʒ���� WQ
CREATE PROCEDURE GSP_GP_ShopItemInfo
	@dwUserID		INT,					        -- �û���ʶ
	@szUID			NVARCHAR(49),				    -- �û�UID
	@szOrderID   	NVARCHAR(31),					-- ������
	@wItemID        smallint,                       ---��ƷID
	@wAmount        smallint,                       ---�ܽ�Ԫ��
	@wCount         smallint,                       ---����
	@strErrorDescribe		NVARCHAR(255) OUTPUT	-- �����Ϣ
WITH ENCRYPTION AS

SET NOCOUNT ON
---������Ϣ
--DECLARE @UserID INT
--DECLARE @_UID NVARCHAR(49)
DECLARE @OrderID NVARCHAR(31)
--DECIMAL @ItemID smallint
--DECIMAL @Amount smallint
--DECIMAL @_Count smallint

---ִ���߼�
BEGIN

---��֤����
     SELECT @OrderID=OrderID
	 FROM RecordAddShopItemInfo
	 WHERE OrderID=@szOrderID
	 
	 IF @OrderID IS NULL
	 BEGIN
	    ---��ֵ��ʯ
		UPDATE GameScoreInfo SET InsureScore=InsureScore+@wCount WHERE UserID=@dwUserID
		IF @@ROWCOUNT=0	
		BEGIN
		INSERT GameScoreInfo(UserID,InsureScore)
		VALUES (@dwUserID,@wCount)
		END
		---��Ӽ�¼
		BEGIN
			DECLARE @RecordID INT
			INSERT INTO RecordAddShopItemInfo(UserID,_UID,OrderID,ItemID,Amount,_Count)
			VALUES (@dwUserID,@szUID,@szOrderID,@wItemID,@wAmount,@wCount)
			SET @RecordID=SCOPE_IDENTITY()
		END
		SET @strErrorDescribe =N'����֧���ɹ��������ĵȴ���'
		
	END
	 
	ELSE
	BEGIN
		SET @strErrorDescribe =N'��Ǹ���ö����Ѵ��ڣ������ظ��ύ��'
		RETURN 1
	END

    DECLARE @dwFinalInsureScore INT
	SELECT @dwFinalInsureScore=InsureScore FROM GameScoreInfo WHERE UserID=@dwUserID
	SELECT @dwFinalInsureScore AS FinalInsureScore
END 

RETURN 0
GO


---------------------------------------------------------------------------------------

-------�������� WQ
CREATE PROCEDURE GSP_GP_InventoryConsumptionInfo
	@dwUserID		INT,					        -- �û���ʶ
	@cbItemID       tinyint,                        ---��ƷID
	@wAmount        smallint,                       ---���ѽ��
	@cbCostType         tinyint,                        ---��������
	@strErrorDescribe		NVARCHAR(255) OUTPUT	-- �����Ϣ
WITH ENCRYPTION AS

SET NOCOUNT ON

DECLARE @tempScore INT
DECLARE @finalScore INT
---ִ���߼�
BEGIN

	-- ��ѯ�û�
	IF not exists(SELECT * FROM QPAccountsDB_HideSeek.dbo.AccountsInfo WHERE UserID=@dwUserID)
	BEGIN
		SET @strErrorDescribe = N'��Ǹ������û���Ϣ�����ڻ������벻��ȷ��'
		return 1
	END
	IF @cbCostType=0
	BEGIN
		SELECT @tempScore=Score
		FROM QPTreasureDB_HideSeek.dbo.GameScoreInfo 
		WHERE UserID=@dwUserID
		IF @tempScore>@wAmount
		BEGIN
			UPDATE GameScoreInfo SET Score=Score-@wAmount WHERE UserID=@dwUserID
			SELECT @finalScore=Score
			FROM QPTreasureDB_HideSeek.dbo.GameScoreInfo 
			WHERE UserID=@dwUserID
		END
		ELSE
		BEGIN
			SET @strErrorDescribe = N'��Ǹ����Ľ�Ҳ��㣡'
			return 2
		END
	END
	ELSE IF @cbCostType=1
	BEGIN
		SELECT @tempScore=InsureScore
		FROM QPTreasureDB_HideSeek.dbo.GameScoreInfo 
		WHERE UserID=@dwUserID
		IF @tempScore>@wAmount
		BEGIN
			UPDATE GameScoreInfo SET InsureScore=InsureScore-@wAmount WHERE UserID=@dwUserID
			SELECT @finalScore=InsureScore
			FROM QPTreasureDB_HideSeek.dbo.GameScoreInfo 
			WHERE UserID=@dwUserID
		END
		ELSE
		BEGIN
			SET @strErrorDescribe = N'��Ǹ�������ʯ���㣡'
			return 2
		END
	END
	SELECT @cbItemID AS ItemID,@finalScore AS FinalScore,@cbCostType AS CostType
END 
	SET @strErrorDescribe = N'�ɹ�ʹ�õ��ߣ�'
RETURN 0
GO


---------------------------------------------------------------------------------------
-- ��ҵ����
CREATE PROCEDURE GSP_GP_AddEnterprisePayment
	@dwUserID				INT,					-- �û���ʶ
	@strPassword			NCHAR(32),				-- �û�����
	@dwEnterprisePayment	INT,					-- ���ֽ��֣�
	@strErrorDescribe		NVARCHAR(255) OUTPUT	-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ������Ϣ
DECLARE @UserID INT
DECLARE @Accounts NVARCHAR(31)
DECLARE @GameID INT
DECLARE @Nullity TINYINT
DECLARE @SpreaderID INT

-- �����Ϣ
DECLARE @Score BIGINT

-- ִ���߼�
BEGIN	
	
	-- ��֤�û�
	DECLARE @Openid NVARCHAR(31)
	SELECT @UserID=UserID,@Openid=Openid,@GameID=GameID,@Accounts=Accounts,@Nullity=Nullity,@SpreaderID=SpreaderID
	FROM QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.AccountsInfo
	WHERE UserID=@dwUserID

	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'��Ǹ����Ҫ���ֵ��û��˺Ų����ڡ�'
		RETURN 1
	END

	IF @Nullity=1
	BEGIN
		SET @strErrorDescribe=N'��Ǹ����Ҫ���ֵ��û��˺���ʱ���ڶ���״̬������ϵ�ͻ����������˽���ϸ�����'
		RETURN 2
	END	
	
	-- ������������
	DECLARE @RealName NVARCHAR(31)
	DECLARE @ExtraCash DECIMAL(16,4)	--��õĶ�����(Ԫ)������ɾ��ֱ���¼������öԷ���ʣ����
	SELECT @RealName=RealName,@ExtraCash=ExtraCash FROM QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.SpreadersInfo WHERE SpreaderID=@dwUserID
	IF @RealName IS NULL
	BEGIN
		SET @strErrorDescribe=N'�����Ǵ����ˣ��޷����֣�'
		RETURN 3
	END
	
	--���������ֽ��(Ԫ)
	DECLARE @CashedOut DECIMAL(16,4)
	SELECT @CashedOut=SUM(Amount) FROM RecordSpreaderCashout WHERE SpreaderID=@dwUserID
	IF @CashedOut IS NULL
	BEGIN
		SET @CashedOut=0
	END	
		
	-- ��֤���ֽ��
	DECLARE @TotalGrant DECIMAL(16,4)					--�ܽ�Ԫ��
	DECLARE @TotalGrantOfChildrenBuy DECIMAL(16,4)		--�����û���ֵ��õ�����ܶԪ��
	DECLARE @TotalLeftCash INT							--ʣ����֣�
	SELECT @TotalGrantOfChildrenBuy=SUM(Payment*PaymentGrantRate) FROM RecordChildrenPayment WHERE RelatedSpreaderID=@dwUserID 
	IF @TotalGrantOfChildrenBuy IS NULL
	BEGIN
		SET @TotalGrantOfChildrenBuy=0
	END
	SET @TotalGrant=@TotalGrantOfChildrenBuy+@ExtraCash
	SET @TotalLeftCash=(@TotalGrant-@CashedOut)*100
	IF @TotalLeftCash<@dwEnterprisePayment
	BEGIN
		SET @strErrorDescribe=N'�������֣�'
		RETURN 4
	END
	
	SET @strErrorDescribe=N'�������ݿ���֤�ɹ�!'
	
	SELECT @UserID AS UserID, @RealName AS RealName, @Openid AS Openid, @TotalLeftCash AS TotalLeftCash
END 

RETURN 0
GO



---------------------------------------------------------------------------------------
-- ��ҵ���ֽ��
CREATE PROCEDURE GSP_GP_AddEnterprisePaymentResult
	@dwUserID				INT,					-- �û���ʶ
	@dwEnterprisePayment	INT,					-- ���ֽ��֣�
	@strErrorDescribe		NVARCHAR(255) OUTPUT	-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN	
	
	--���ּ�¼
	INSERT INTO RecordSpreaderCashout(SpreaderID,Amount) VALUES(@dwUserID,@dwEnterprisePayment/100.0)		
	
	/*
	-- ���������ֽ�Ԫ��
	UPDATE QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.SpreadersInfo SET CashedOut=CashedOut+@dwEnterprisePayment/100.0 
	WHERE SpreaderID=@dwUserID
	IF @@ROWCOUNT=0
	BEGIN
		SET @strErrorDescribe=N'���������ֽ�� ʧ��!'
		RETURN 1
	END
	*/
	
	SET @strErrorDescribe=N'�������ּ�¼ �ɹ�!'
	
END 

RETURN 0
GO

	
---------------------------------------------------------------------------------------
-- �û���ֵ
CREATE PROCEDURE GSP_GP_AddPayment
	@dwUserID			INT,					-- �û���ʶ
	@strPassword		NCHAR(32),				-- �û�����
	@dwPayAmount		INT,					-- ������
	@dwBoughtDiamond	INT,					-- ������ʯ
	--@dwFinalInsureScore INT output	,			-- ������ʯ��Ŀ
	@strErrorDescribe	NVARCHAR(255) OUTPUT	-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ������Ϣ
DECLARE @UserID INT
DECLARE @Accounts NVARCHAR(31)
DECLARE @GameID INT
DECLARE @Nullity TINYINT
DECLARE @SpreaderID INT

-- �����Ϣ
DECLARE @Score BIGINT

-- ִ���߼�
BEGIN	
	
	-- ��֤�û�
	SELECT @UserID=UserID,@GameID=GameID,@Accounts=Accounts,@Nullity=Nullity,@SpreaderID=SpreaderID
	FROM QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.AccountsInfo
	WHERE UserID=@dwUserID

	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'��Ǹ����Ҫ��ֵ���û��˺Ų����ڡ�'
		RETURN 2
	END

	IF @Nullity=1
	BEGIN
		SET @strErrorDescribe=N'��Ǹ����Ҫ��ֵ���û��˺���ʱ���ڶ���״̬������ϵ�ͻ����������˽���ϸ�����'
		RETURN 3
	END	
	
	--�ƹ�ϵͳ
	--��ͨ�û��󶨴���󣬳�ֵ���ͱ���
	DECLARE @dwGrantDiamond INT
	IF @SpreaderID<>0
	BEGIN
		DECLARE @PaymentGrantRate DECIMAL(16,4)
		SELECT @PaymentGrantRate=PaymentGrantRate FROM GlobalSpreadInfo
		IF @PaymentGrantRate IS NULL
		BEGIN
			SET @PaymentGrantRate=0.20
		END
		
		SET @dwGrantDiamond = cast(round(@PaymentGrantRate*@dwBoughtDiamond,0) as INT)
	END 
	ELSE
	BEGIN
		SET @dwGrantDiamond=0
	END

	-- ��ֵ��ʯ
	UPDATE GameScoreInfo SET InsureScore=InsureScore+@dwBoughtDiamond+@dwGrantDiamond WHERE UserID=@UserID
	IF @@ROWCOUNT=0	
	BEGIN
		INSERT GameScoreInfo(UserID,InsureScore,RegisterIP,LastLogonIP)
		VALUES (@UserID,@dwBoughtDiamond+@dwGrantDiamond,'0.0.0.0','0.0.0.0')
	END

	-- ��ֵ��¼
	DECLARE @Note NVARCHAR(128)
	DECLARE @RecordID INT
	SET @Note = N'��ֵ'+LTRIM(STR(@dwPayAmount))+'Ԫ������'+LTRIM(STR(@dwBoughtDiamond))+'��ʯ������'+LTRIM(STR(@dwGrantDiamond))+'��ʯ'
	INSERT INTO RecordUserPayment(UserID,BindSpreaderID,Payment,BoughtInsureScore,GrantInsureScore,TypeID,CollectNote)
	VALUES(@UserID,@SpreaderID,@dwPayAmount,@dwBoughtDiamond,@dwGrantDiamond,1,@Note)	
	SET @RecordID=SCOPE_IDENTITY()

	-- ��¼ for��������
	IF @SpreaderID<>0
	BEGIN
	
		--��ȡ��ɱ�������
		DECLARE @FillGrantRateOfL1 DECIMAL(16,4)
		DECLARE @FillGrantRateOfL2 DECIMAL(16,4)
		DECLARE @FillGrantRateOfL3 DECIMAL(16,4)
		DECLARE @FillGrantRateOfSecondhand DECIMAL(16,4)
		DECLARE @FillGrantRateOfThirdhand DECIMAL(16,4)
		SELECT @FillGrantRateOfL1=FillGrantRateOfL1,@FillGrantRateOfL2=FillGrantRateOfL2,@FillGrantRateOfL3=FillGrantRateOfL3, 
		@FillGrantRateOfSecondhand=FillGrantRateOfSecondhand,@FillGrantRateOfThirdhand=FillGrantRateOfThirdhand 
		FROM GlobalSpreadInfo
		IF @FillGrantRateOfL1 IS NULL
		BEGIN
			SET @strErrorDescribe=N'û��������ɱ���GlobalSpreadInfo'
			RETURN 4
		END
		
		DECLARE @SpreaderIDOfSecondhand INT 	--��һ�����
		DECLARE @SpreaderIDOfThirdhand INT		--���������
		
		--ֱ��
		---------------------------------------------------------------------
		DECLARE @dwSpreaderLevel INT			--ֱ���ȼ�
		SELECT @dwSpreaderLevel=SpreaderLevel,@SpreaderIDOfSecondhand=ParentID FROM QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.SpreadersInfo WHERE SpreaderID=@SpreaderID
		IF @dwSpreaderLevel IS NOT NULL AND @dwSpreaderLevel<>0
		BEGIN
		
			--��ȡ��ɱ���
			DECLARE @GrantRate DECIMAL(16,4)
			IF @dwSpreaderLevel=1
			BEGIN
				SET @GrantRate = @FillGrantRateOfL1
			END ELSE
			IF @dwSpreaderLevel=2
			BEGIN
				SET @GrantRate = @FillGrantRateOfL2
			END ELSE
			IF @dwSpreaderLevel=3
			BEGIN
				SET @GrantRate = @FillGrantRateOfL3
			END 
			
			--д��¼
			INSERT INTO RecordChildrenPayment(RecordIDOfUserPayment,UserID,Payment,RelatedSpreaderID,RelatedSpreaderType,PaymentGrantRate,PaymentGrantState)
			VALUES(@RecordID,@UserID,@dwPayAmount,@SpreaderID,0,@GrantRate,0)	
		
		
			--��һ�����
			----------------------------------------------------------------------------------	
			DECLARE @dwSpreaderLevelOfSecondhand INT	--��һ�����ȼ�
			SELECT @dwSpreaderLevelOfSecondhand=SpreaderLevel,@SpreaderIDOfThirdhand=ParentID FROM QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.SpreadersInfo WHERE SpreaderID=@SpreaderIDOfSecondhand
			IF @dwSpreaderLevelOfSecondhand IS NOT NULL AND @dwSpreaderLevelOfSecondhand<>0
			BEGIN
				--��ȡ��ɱ���
				SET @GrantRate = @FillGrantRateOfSecondhand
				
				--д��¼
				INSERT INTO RecordChildrenPayment(RecordIDOfUserPayment,UserID,Payment,RelatedSpreaderID,RelatedSpreaderType,PaymentGrantRate,PaymentGrantState)
				VALUES(@RecordID,@UserID,@dwPayAmount,@SpreaderIDOfSecondhand,1,@GrantRate,0)	
				
				
				--���������
				-----------------------------------------------------------------------------------------
				DECLARE @dwSpreaderLevelOfThirdhand INT	--���������ȼ�
				SELECT @dwSpreaderLevelOfThirdhand=SpreaderLevel FROM QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.SpreadersInfo WHERE SpreaderID=@SpreaderIDOfThirdhand
				IF @dwSpreaderLevelOfThirdhand IS NOT NULL AND @dwSpreaderLevelOfThirdhand<>0
				BEGIN
					--��ȡ��ɱ���
					SET @GrantRate = @FillGrantRateOfThirdhand
					
					--д��¼
					INSERT INTO RecordChildrenPayment(RecordIDOfUserPayment,UserID,Payment,RelatedSpreaderID,RelatedSpreaderType,PaymentGrantRate,PaymentGrantState)
					VALUES(@RecordID,@UserID,@dwPayAmount,@SpreaderIDOfThirdhand,2,@GrantRate,0)	
				END
			END		
		END
		
	END
	
	SET @strErrorDescribe=N'��ֵ�ɹ�:'+@Note
	
	DECLARE @dwFinalInsureScore INT
	SELECT @dwFinalInsureScore=InsureScore FROM GameScoreInfo WHERE UserID=@UserID
	
	SELECT @UserID AS UserID, @dwFinalInsureScore AS FinalInsureScore
	
END 

RETURN 0
GO



--��ѯ�����û�������Ϣ
----------------------------------------------------------------------------------------------------

-- ��ѯ����
CREATE PROC GSP_GP_QueryChildrenPaymentInfo
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
	SELECT @LogonPass=LogonPass FROM QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.AccountsInfo WHERE UserID=@dwUserID

	-- �����ж�
	IF @LogonPass<>@strPassword
	BEGIN
		SET @strErrorDescribe=N'�����û����벻��ȷ��'
		RETURN 1
	END
	
	
	-- ���������
	DECLARE @dwMySpreaderLevel INT
	DECLARE @ExtraCash DECIMAL(16,4)	--��õĶ�����(Ԫ)������ɾ��ֱ���¼������öԷ���ʣ����
	SELECT @dwMySpreaderLevel=SpreaderLevel,@ExtraCash=ExtraCash FROM QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.SpreadersInfo WHERE SpreaderID=@dwUserID
	IF @dwMySpreaderLevel IS NULL
	BEGIN
		SET @strErrorDescribe=N'�����Ǵ����ˣ�û��Ȩ��ִ�д˲�����'
		RETURN 2
	END
		
	--���������ֽ��(Ԫ)
	DECLARE @CashedOut DECIMAL(16,4)
	SELECT @CashedOut=SUM(Amount) FROM RecordSpreaderCashout WHERE SpreaderID=@dwUserID
	IF @CashedOut IS NULL
	BEGIN
		SET @CashedOut=0
	END	
		
	-- ��ѯ�����û�������Ϣ
	DECLARE @HasChildrenBuyCount INT
	DECLARE @TotalGrant DECIMAL(16,4)					--�ܽ�Ԫ��
	DECLARE @TotalGrantOfChildrenBuy DECIMAL(16,4)		--�����û���ֵ��õ�����ܶԪ��
	DECLARE @TotalLeftCash DECIMAL(16,4)				--ʣ���Ԫ��
	SELECT @TotalGrantOfChildrenBuy=SUM(Payment*PaymentGrantRate) FROM RecordChildrenPayment WHERE RelatedSpreaderID=@dwUserID 
	IF @TotalGrantOfChildrenBuy IS NULL
	BEGIN
		SET @TotalGrantOfChildrenBuy=0
		SET @TotalGrant=@TotalGrantOfChildrenBuy+@ExtraCash
		SET @TotalLeftCash=@TotalGrant-@CashedOut
		
		SET @strErrorDescribe=N'��ѯ�����û�������Ϣ�ɹ�!'
		SET @HasChildrenBuyCount=0
		SELECT @HasChildrenBuyCount as HasChildrenBuyCount, @TotalGrantOfChildrenBuy as TotalGrantOfChildrenBuy, @ExtraCash as ExtraCash, @CashedOut as CashedOut, @TotalLeftCash as TotalLeftCash 
	END ELSE
	BEGIN
		SET @TotalGrant=@TotalGrantOfChildrenBuy+@ExtraCash
		SET @TotalLeftCash=@TotalGrant-@CashedOut
		
		SET @strErrorDescribe=N'��ѯ�����û�������Ϣ�ɹ�!'
		SET @HasChildrenBuyCount=1
		SELECT *, @HasChildrenBuyCount as HasChildrenBuyCount, @TotalGrantOfChildrenBuy as TotalGrantOfChildrenBuy, @ExtraCash as ExtraCash, @CashedOut as CashedOut, @TotalLeftCash as TotalLeftCash FROM RecordChildrenPayment WHERE RelatedSpreaderID=@dwUserID 
	END
	
	/*
	DECLARE @FillGrantRateOfL1 DECIMAL(16,4)
	DECLARE @FillGrantRateOfL2 DECIMAL(16,4)
	DECLARE @FillGrantRateOfL3 DECIMAL(16,4)
	DECLARE @FillGrantRateOfSecondhand DECIMAL(16,4)
	DECLARE @FillGrantRateOfThirdhand DECIMAL(16,4)
	SELECT @FillGrantRateOfL1=FillGrantRateOfL1,@FillGrantRateOfL2=FillGrantRateOfL2,@FillGrantRateOfL3=FillGrantRateOfL3, 
	@FillGrantRateOfSecondhand=FillGrantRateOfSecondhand,@FillGrantRateOfThirdhand=FillGrantRateOfThirdhand
	FROM GlobalSpreadInfo
	IF @FillGrantRateOfL1 IS NULL
	BEGIN
		SET @strErrorDescribe=N'û��������ɱ���GlobalSpreadInfo'
		RETURN 2
	END
		
	DECLARE @Gain DECIMAL(16,4)
	DECLARE @dwPayAmount INT

	--dwUserID��ֱ��
	
	--��ȡ��ɱ���
	DECLARE @GrantRate DECIMAL(16,4)
	IF @dwMySpreaderLevel=1
	BEGIN
		SET @GrantRate = @FillGrantRateOfL1
	END ELSE
	IF @dwMySpreaderLevel=2
	BEGIN
		SET @GrantRate = @FillGrantRateOfL2
	END ELSE
	IF @dwMySpreaderLevel=3
	BEGIN
		SET @GrantRate = @FillGrantRateOfL3
	END
	
	SELECT *,Payment*@GrantRate AS Gain FROM RecordUserPayment WHERE BindSpreaderID=@dwUserID
	

	--dwUserID�Ǹ������
	SET @GrantRate = @FillGrantRateOfSecondhand
	
	--dwUserID�Ǹ��������
	SET @GrantRate = @FillGrantRateOfThirdhand
	
	SELECT * FROM RecordUserPayment WHERE BindSpreaderID<>0 AND 
		(BindSpreaderID=@dwUserID or BindSpreaderID in
			(
				--dwUserID���Ӵ���
				SELECT SpreaderID FROM QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.SpreadersInfo WHERE ParentID=@dwUserID 
				or ParentID in (SELECT SpreaderID FROM QPAccountsDB_HideSeekLink.QPAccountsDB_HideSeek.dbo.SpreadersInfo WHERE ParentID=@dwUserID)
			)
		)
	*/
		
END

RETURN 0

GO


