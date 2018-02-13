
----------------------------------------------------------------------------------------------------

USE QPPlatformDB_HideSeek
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_LoadPrivateInfo]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_LoadPrivateInfo]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_CreatePrivate]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_CreatePrivate]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_BackCreatePrivateCost]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_BackCreatePrivateCost]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_InsertUserRoom]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_InsertUserRoom]
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO


----------------------------------------------------------------------------------------------------

--���ؽڵ�
CREATE  PROCEDURE dbo.GSP_GR_LoadPrivateInfo 
	@wKindID INT								-- ���ͱ�ʶ
WITH ENCRYPTION AS

--��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN
--��ѯ�ڵ�
	DECLARE @KindID INT
	DECLARE @PlayCout1 INT
	DECLARE @PlayCout2 INT
	DECLARE @PlayCout3 INT
	DECLARE @PlayCout4 INT
	DECLARE @PlayCost1 INT
	DECLARE @PlayCost2 INT
	DECLARE @PlayCost3 INT
	DECLARE @PlayCost4 INT
	
	DECLARE @PlayAvgCost1 INT
	DECLARE @PlayAVgCost2 INT
	DECLARE @PlayAvgCost3 INT
	DECLARE @PlayAvgCost4 INT
	
	DECLARE @CostGold INT
	--mChen add
	DECLARE @MatchPlayCout INT
	DECLARE @MatchStartTime datetime
	DECLARE	@MatchEndTime datetime
	
	SELECT @KindID = KindID,@PlayCout1 = PlayCout1,@PlayCout2 = PlayCout2,@PlayCout3 = PlayCout3,@PlayCout4 = PlayCout4,
		@PlayCost1 = PlayCost1,@PlayCost2 = PlayCost2,@PlayCost3 = PlayCost3,@PlayCost4 = PlayCost4,@PlayAvgCost1 = PlayCostAvg1,
		@PlayAvgCost2 = PlayCostAvg2,@PlayAvgCost3 = PlayCostAvg3,@PlayAvgCost4 = PlayCostAvg4,@CostGold = CostGold,
		@MatchPlayCout=MatchPlayCout, @MatchStartTime = MatchStartTime, @MatchEndTime = MatchEndTime	--mChen add
	FROM PrivateInfo WHERE KindID=@wKindID
	
	if @KindID = 0
	BEGIN
		return 1;
	END
	
	SELECT @KindID AS KindID,@PlayCout1 AS PlayCout1,@PlayCout2 AS PlayCout2,@PlayCout3 AS PlayCout3,@PlayCout4 AS PlayCout4,
	@PlayCost1 AS PlayCost1,@PlayCost2 AS PlayCost2,@PlayCost3 AS PlayCost3,@PlayCost4 AS PlayCost4,@CostGold AS CostGold,
	@PlayAvgCost1 AS PlayCostAvg1, @PlayAvgCost2 AS PlayCostAvg2, @PlayAvgCost3 AS PlayCostAvg3, @PlayAvgCost4 AS PlayCostAvg4,
	@MatchPlayCout AS MatchPlayCout, @MatchStartTime AS MatchStartTime, @MatchEndTime AS MatchEndTime	--mChen add
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------

--���ؽڵ�
CREATE  PROCEDURE dbo.GSP_GR_CreatePrivate 
	@dwUserID INT,
	@wKindID INT,								-- ���ͱ�ʶ
	@wCostScore INT,							
	@wCostType INT,							
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

--��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	DECLARE @InsureScore BIGINT
	DECLARE @Score BIGINT
	SELECT @InsureScore=InsureScore,@Score=Score
	FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo WHERE UserID=@dwUserID
	IF  @InsureScore IS NULL
	BEGIN
		set @InsureScore = 0
		set @strErrorDescribe = N'δ�ҵ����'
		return 1;
	END
	if @wCostType = 0
	BEGIN
		IF @InsureScore < @wCostScore
		BEGIN
			set @strErrorDescribe = N'��ʯ������������,InsureScore='+convert(varchar,@InsureScore)+N'CostScore='++convert(varchar,@wCostScore)
			return 2;
		END
	END
	if @wCostType = 1
	BEGIN
		IF @Score < @wCostScore
		BEGIN
			set @strErrorDescribe = N'��Ҳ�����������'
			return 2;
		END
	END
	set @strErrorDescribe = N'��������ɹ�'
	SELECT @InsureScore AS CurSocre
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------

--���ؽڵ�
CREATE  PROCEDURE dbo.GSP_GR_BackCreatePrivateCost 
	@dwUserID INT,
	@wCostScore INT,								
	@wCostType INT						
WITH ENCRYPTION AS

--��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	DECLARE @InsureScore BIGINT
	DECLARE @Score BIGINT
	SELECT @InsureScore=InsureScore,@Score=Score
	FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo WHERE UserID=@dwUserID
	IF  @InsureScore IS NULL
	BEGIN
		set @InsureScore = 0
		return 1;
	END
	
	if @wCostType = 0 	--mChen edit,Type_Private
	BEGIN
		set @InsureScore = @InsureScore-@wCostScore
	END 
	ELSE if @wCostType = 1 	
	BEGIN
		set @Score = @Score-@wCostScore
	END
	
	UPDATE QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo 
	SET InsureScore=@InsureScore,Score=@Score
	WHERE UserID=@dwUserID
	
	SELECT @InsureScore AS CurSocre
END

RETURN 0

GO


----------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------

--���뷿���¼
CREATE  PROCEDURE dbo.GSP_GR_InsertUserRoom 
	@dwUserID INT,
	@dwRoomNum INT,
	@dwBaseScore INT,							
	@dwPlayCout INT,							
	@dwChairCount INT,							
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

--��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- �����¼
	INSERT QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.RecordUserRoomInfo (UserID, RoomNum, BaseScore, PlayCout, ChairCount)
	VALUES (@dwUserID, @dwRoomNum, @dwBaseScore, @dwPlayCout, @dwChairCount)

	IF @@ERROR<>0
	BEGIN
		-- ������Ϣ
		SET @strErrorDescribe=N'���뷿���¼ʧ�ܣ�'
		RETURN 1
	END
	
	SET @strErrorDescribe=N'���뷿���¼�ɹ�'
END

RETURN 0

GO
