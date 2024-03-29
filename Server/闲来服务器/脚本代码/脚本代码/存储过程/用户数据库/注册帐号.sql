
----------------------------------------------------------------------------------------------------

USE QPAccountsDB_HideSeek
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_RegisterAccounts]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_RegisterAccounts]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_MB_RegisterAccounts]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_MB_RegisterAccounts]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- 帐号注册
CREATE PROC GSP_GP_RegisterAccounts
	@strAccounts NVARCHAR(31),					-- 用户帐号
	@strUid NVARCHAR(49),
	@strOpenid NVARCHAR(31),
	@strNickName NVARCHAR(31),					-- 用户昵称
	@dwSpreaderId INT,							-- mChen edit, @strSpreader NVARCHAR(31),					-- 推荐帐号
	@strLogonPass NCHAR(32),					-- 登录密码
	@strInsurePass NCHAR(32),					-- 银行密码
	@wFaceID SMALLINT,							-- 头像标识
	@wChannleID SMALLINT,                       --渠道ID
	@cbGender TINYINT,							-- 用户性别
	@strPassPortID NVARCHAR(18),				-- 身份证号
	@strCompellation NVARCHAR(16),				-- 真实名字
	@strClientIP NVARCHAR(15),					-- 连接地址
	@strMachineID NCHAR(32),					-- 机器标识
	@wKindID SMALLINT,							-- 游戏 I D		mChen add, for Match Time
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 基本信息
DECLARE @UserID INT
DECLARE @Gender TINYINT
DECLARE @FaceID SMALLINT
DECLARE @ChannleID SMALLINT
DECLARE @CustomID INT
DECLARE @MoorMachine TINYINT
DECLARE @Accounts NVARCHAR(31)
DECLARE @NickName NVARCHAR(31)
DECLARE @UnderWrite NVARCHAR(63)

-- 积分变量
DECLARE @Score BIGINT
DECLARE @Insure BIGINT

-- 附加信息
DECLARE @GameID INT
DECLARE @UserMedal INT
DECLARE @Experience INT
DECLARE @LoveLiness INT
DECLARE @SpreaderID INT
DECLARE @InsureEnabled INT
DECLARE @MemberOrder SMALLINT
DECLARE @MemberOverDate DATETIME

-- 辅助变量
DECLARE @EnjoinLogon AS INT
DECLARE @EnjoinRegister AS INT

-- 执行逻辑
BEGIN
	-- 注册暂停
	SELECT @EnjoinRegister=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinRegister'
	IF @EnjoinRegister IS NOT NULL AND @EnjoinRegister<>0
	BEGIN
		SELECT @strErrorDescribe=StatusString FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinRegister'
		RETURN 1
	END

	-- 登录暂停
	SELECT @EnjoinLogon=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinLogon'
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SELECT @strErrorDescribe=StatusString FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinLogon'
		RETURN 2
	END

	-- 效验名字
	IF EXISTS (SELECT [String] FROM ConfineContent(NOLOCK) WHERE CHARINDEX(String,@strAccounts)>0 AND (EnjoinOverDate>GETDATE() OR EnjoinOverDate IS NULL))
	BEGIN
		SET @strErrorDescribe=N'抱歉地通知您，您所输入的登录帐号名含有限制字符串，请更换帐号名后再次申请帐号！'
		RETURN 4
	END

	-- 效验昵称
	IF EXISTS (SELECT [String] FROM ConfineContent(NOLOCK) WHERE CHARINDEX(String,@strNickname)>0 AND (EnjoinOverDate>GETDATE() OR EnjoinOverDate IS NULL))
	BEGIN
		SET @strErrorDescribe=N'抱歉地通知您，您所输入的游戏昵称名含有限制字符串，请更换昵称名后再次申请帐号！'
		RETURN 4
	END

	-- 效验地址
	SELECT @EnjoinRegister=EnjoinRegister FROM ConfineAddress(NOLOCK) WHERE AddrString=@strClientIP AND (EnjoinOverDate>GETDATE() OR EnjoinOverDate IS NULL)
	IF @EnjoinRegister IS NOT NULL AND @EnjoinRegister<>0
	BEGIN
		SET @strErrorDescribe=N'抱歉地通知您，系统禁止了您所在的 IP 地址的注册功能，请联系客户服务中心了解详细情况！'
		RETURN 5
	END
	
	-- 效验机器
	SELECT @EnjoinRegister=EnjoinRegister FROM ConfineMachine(NOLOCK) WHERE MachineSerial=@strMachineID AND (EnjoinOverDate>GETDATE() OR EnjoinOverDate IS NULL)
	IF @EnjoinRegister IS NOT NULL AND @EnjoinRegister<>0
	BEGIN
		SET @strErrorDescribe=N'抱歉地通知您，系统禁止了您的机器的注册功能，请联系客户服务中心了解详细情况！'
		RETURN 6
	END
 
	-- 查询帐号
	IF EXISTS (SELECT UserID FROM AccountsInfo(NOLOCK) WHERE Accounts=@strAccounts)
	BEGIN
		SET @strErrorDescribe=N'此帐号已被注册，请换另一帐号尝试再次注册！'
		RETURN 7
	END

	-- 查询昵称
	 IF EXISTS (SELECT UserID FROM AccountsInfo(NOLOCK) WHERE NickName=@strNickName)
	 BEGIN
	 	SET @strErrorDescribe=N'此昵称已被注册，请换另一昵称尝试再次注册！'
	 	RETURN 7
	 END

	-- 查推广员
	IF @dwSpreaderId<>0	--mChen edit,IF @strSpreader<>''
	BEGIN
		-- 查推广员
		SELECT @SpreaderID=UserID FROM AccountsInfo(NOLOCK) WHERE UserID=@dwSpreaderId
		--SELECT @SpreaderID=UserID FROM AccountsInfo(NOLOCK) WHERE Accounts=@strSpreader

		-- 结果处理
		IF @SpreaderID IS NULL
		BEGIN
			SET @strErrorDescribe=N'您所填写的推荐人不存在或者填写错误，请检查后再次注册！'
			RETURN 8
		END
	END
	ELSE SET @SpreaderID=0

	-- 注册用户
	INSERT AccountsInfo (Accounts,Unionid,Openid,NickName,RegAccounts,LogonPass,InsurePass,SpreaderID,PassPortID,Compellation,Gender,FaceID,ChannleID,
		GameLogonTimes,LastLogonIP,LastLogonMachine,RegisterIP,RegisterMachine)
	VALUES (@strAccounts,@strUid,@strOpenid,@strNickName,@strAccounts,@strLogonPass,@strInsurePass,@SpreaderID,@strPassPortID,@strCompellation,@cbGender,@wFaceID,@wChannleID,
		1,@strClientIP,@strMachineID,@strClientIP,@strMachineID)

	-- 错误判断
	IF @@ERROR<>0
	BEGIN
		SET @strErrorDescribe=N'帐号已存在，请换另一帐号名字尝试再次注册！'
		RETURN 8
	END

	-- 查询用户
	SELECT @UserID=UserID, @GameID=GameID, @Accounts=Accounts, @NickName=NickName, @UnderWrite=UnderWrite, @FaceID=FaceID,@ChannleID=ChannleID,
		@CustomID=CustomID, @Gender=Gender, @UserMedal=UserMedal, @Experience=Experience, @LoveLiness=LoveLiness, @MemberOrder=MemberOrder,
		@MemberOverDate=MemberOverDate, @MoorMachine=MoorMachine
	FROM AccountsInfo(NOLOCK) WHERE Accounts=@strAccounts

	-- 分配标识
	SELECT @GameID=GameID FROM GameIdentifier(NOLOCK) WHERE UserID=@UserID
	IF @GameID IS NULL 
	BEGIN
		SET @GameID=0
		SET @strErrorDescribe=N'用户注册成功，但未成功获取游戏 ID 号码，系统稍后将给您分配！'
	END
	ELSE UPDATE AccountsInfo SET GameID=@GameID WHERE UserID=@UserID

	-- 推广提成
	IF @SpreaderID<>0
	BEGIN
		DECLARE @RegisterGrantScore INT
		DECLARE @Note NVARCHAR(512)
		SET @Note = N'注册'
		SELECT @RegisterGrantScore = RegisterGrantScore FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GlobalSpreadInfo
		IF @RegisterGrantScore IS NULL
		BEGIN
			SET @RegisterGrantScore=5000
		END
		INSERT INTO QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.RecordSpreadInfo(
			UserID,Score,TypeID,ChildrenID,CollectNote)
		VALUES(@SpreaderID,@RegisterGrantScore,1,@UserID,@Note)		
	END

	-- 记录日志
	DECLARE @DateID INT
	SET @DateID=CAST(CAST(GETDATE() AS FLOAT) AS INT)
	UPDATE SystemStreamInfo SET GameRegisterSuccess=GameRegisterSuccess+1 WHERE DateID=@DateID
	IF @@ROWCOUNT=0 INSERT SystemStreamInfo (DateID, GameRegisterSuccess) VALUES (@DateID, 1)

	----------------------------------------------------------------------------------------------------
	----------------------------------------------------------------------------------------------------
	-- 注册赠送

	-- 读取变量
	-- DECLARE @GrantIPCount AS BIGINT
	-- DECLARE @GrantScoreCount AS BIGINT
	-- SELECT @GrantIPCount=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'GrantIPCount'
	-- SELECT @GrantScoreCount=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'GrantScoreCount'

	-- 赠送限制
	--IF @GrantScoreCount IS NOT NULL AND @GrantScoreCount>0 AND @GrantIPCount IS NOT NULL AND @GrantIPCount>0
	--BEGIN
		-- 赠送次数
	--	DECLARE @GrantCount AS BIGINT
	--	DECLARE @GrantMachineCount AS BIGINT
	--	SELECT @GrantCount=GrantCount FROM SystemGrantCount(NOLOCK) WHERE DateID=@DateID AND RegisterIP=@strClientIP
	--	SELECT @GrantMachineCount=GrantCount FROM SystemMachineGrantCount(NOLOCK) WHERE DateID=@DateID AND RegisterMachine=@strMachineID
	
		-- 次数判断
	--	IF (@GrantCount IS NOT NULL AND @GrantCount>=@GrantIPCount) OR (@GrantMachineCount IS NOT NULL AND @GrantMachineCount>=@GrantIPCount)
	--	BEGIN
	--		SET @GrantScoreCount=0
	--	END
	--END

	-- 赠送金币
	--IF @GrantScoreCount IS NOT NULL AND @GrantScoreCount>0
	--BEGIN
		-- 更新记录
		--UPDATE SystemGrantCount SET GrantScore=GrantScore+@GrantScoreCount, GrantCount=GrantCount+1 WHERE DateID=@DateID AND RegisterIP=@strClientIP

		-- 插入记录
		--IF @@ROWCOUNT=0
		--BEGIN
		--	INSERT SystemGrantCount (DateID, RegisterIP, RegisterMachine, GrantScore, GrantCount) VALUES (@DateID, @strClientIP, @strMachineID, @GrantScoreCount, 1)
		--END

		-- 更新记录
		--UPDATE SystemMachineGrantCount SET GrantScore=GrantScore+@GrantScoreCount, GrantCount=GrantCount+1 WHERE DateID=@DateID AND RegisterMachine=@strMachineID

		-- 插入记录
		--IF @@ROWCOUNT=0
		--BEGIN
		--	INSERT SystemMachineGrantCount (DateID, RegisterIP, RegisterMachine, GrantScore, GrantCount) VALUES (@DateID, @strClientIP, @strMachineID, @GrantScoreCount, 1)
		--END

		-- 赠送金币
		--INSERT QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo (UserID, Score, RegisterIP, LastLogonIP) VALUES (@UserID, @GrantScoreCount, @strClientIP, @strClientIP) 
	--END

	-- 注册赠送
	DECLARE @GrantScoreCount AS BIGINT
	DECLARE @GrantScoreInsure AS BIGINT
	SELECT @GrantScoreCount=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'GrantScoreCount'
	SELECT @GrantScoreInsure=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'GrantScoreInsure'
	IF @GrantScoreCount IS NULL SET @GrantScoreCount=0	--mChen 1983
	IF @GrantScoreInsure IS NULL SET @GrantScoreInsure=0	--mChen 1983
	INSERT QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo (UserID, Score,InsureScore, RegisterIP, LastLogonIP) VALUES (@UserID, @GrantScoreCount,@GrantScoreInsure, @strClientIP, @strClientIP) 
	----------------------------------------------------------------------------------------------------
	----------------------------------------------------------------------------------------------------

	-- 查询金币
	SELECT @Score=Score, @Insure=InsureScore FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo WHERE UserID=@UserID

	-- 数据调整
	IF @Score IS NULL SET @Score=0
	IF @Insure IS NULL SET @Insure=0
	
	SET @InsureEnabled=1
	IF @strInsurePass = N'' SET @InsureEnabled=0

	--mChen add, for Match Time
	DECLARE @MatchStartTime datetime
	DECLARE	@MatchEndTime datetime
	SELECT @MatchStartTime = MatchStartTime, @MatchEndTime = MatchEndTime FROM QPPlatformDB_HideSeekLink.QPPlatformDB_HideSeek.dbo.PrivateInfo WHERE KindID=@wKindID
	
	--mChen add, for签到记录
	DECLARE @wSeriesDate INT	
	SELECT @wSeriesDate=SeriesDate FROM AccountsSignin WHERE UserID=@UserID
	IF @wSeriesDate IS NULL
	BEGIN
		SET @wSeriesDate = 0	
	END
	
	--mChen add:查询用户已打场次,for抽奖
	DECLARE @PlayCount INT
	SELECT  @PlayCount=COUNT(*) FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.PrivateGameRecordUserRecordID WHERE UserID = @UserID
	
	--mChen add:查询抽奖记录
	DECLARE @RaffleCount INT	
	SELECT @RaffleCount=RaffleCount FROM AccountsRaffle WHERE UserID=@UserID
	IF @RaffleCount IS NULL
	BEGIN
		SET @RaffleCount = 0
	END
	-- 每打完多少场可以抽奖一次
	DECLARE @PlayCountPerRaffle INT
	SELECT @PlayCountPerRaffle=StatusValue FROM SystemStatusInfo WHERE StatusName=N'PlayCountPerRaffle'
	IF @PlayCountPerRaffle IS NULL
	BEGIN
		SET @PlayCountPerRaffle = 20
	END
	
	--mChen add:查询代理等级
	DECLARE @SpreaderLevel AS INT
	SELECT @SpreaderLevel=SpreaderLevel FROM SpreadersInfo WHERE SpreaderID=@UserID
	IF @SpreaderLevel IS NULL
	BEGIN
		SET @SpreaderLevel=-1
	END
	
	--mChen add:查询公告信息
	DECLARE @strPublicNotice NVARCHAR(256)
	SELECT @strPublicNotice=StatusString FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'PublicNotice'
	IF @strPublicNotice IS NULL
	BEGIN
		SET @strPublicNotice=N''
	END
	
	--mChen add, for HideSeek:查询警察模型库
	DECLARE @lModelIndex0 BIGINT
	SELECT @lModelIndex0=ModelIndex0 FROM AccountsTaggerModel(NOLOCK) WHERE UserID=@UserID
	IF @lModelIndex0 IS NULL
	BEGIN
		SET @lModelIndex0=1
		INSERT INTO AccountsTaggerModel(UserID,ModelIndex0)
		VALUES(@UserID,@lModelIndex0)	
	END
	
	--WQ 查询用户头像	
	DECLARE @strHeadHttp NVARCHAR(256)				-- 头像HTTP				
	IF @strHeadHttp IS NULL
	BEGIN
		SET @strHeadHttp=N'0'		
		INSERT IndividualDatum (UserID, Compellation, QQ, EMail, SeatPhone, MobilePhone, DwellingPlace, HeadHttp,UserChannel)
		VALUES (@UserID, N'', N'', N'', N'', N'', N'', @strHeadHttp, N'')
	END
	
	-- 输出变量
	SELECT @UserID AS UserID, @MatchStartTime AS MatchStartTime, @MatchEndTime AS MatchEndTime, 
		@wSeriesDate AS SeriesDate, @PlayCount AS PlayCount, 
		@RaffleCount AS RaffleCount, @PlayCountPerRaffle AS PlayCountPerRaffle,
		@SpreaderLevel AS SpreaderLevel,
		@strPublicNotice AS PublicNotice,
		@lModelIndex0 AS ModelIndex0,
		@GameID AS GameID, @Accounts AS Accounts, @NickName AS NickName, @UnderWrite AS UnderWrite,
		@FaceID AS FaceID, @ChannleID AS ChannleID,@CustomID AS CustomID, @Gender AS Gender, @UserMedal AS UserMedal, @Experience AS Experience,
		@Score AS Score, @Insure AS Insure, @LoveLiness AS LoveLiness, @MemberOrder AS MemberOrder, @MemberOverDate AS MemberOverDate,
		@MoorMachine AS MoorMachine,@SpreaderID AS SpreaderID,@InsureEnabled AS InsureEnabled,
		@strHeadHttp AS HeadHttp

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- 帐号注册
CREATE PROC GSP_MB_RegisterAccounts
	@strAccounts NVARCHAR(31),					-- 用户帐号
	@strNickName NVARCHAR(31),					-- 用户昵称
	@strLogonPass NCHAR(32),					-- 登录密码
	@strInsurePass NCHAR(32),					-- 银行密码
	@wFaceID SMALLINT,							-- 头像标识
	@cbGender TINYINT,							-- 用户性别
	@strClientIP NVARCHAR(15),					-- 连接地址
	@strMobilePhone NVARCHAR(11),				-- 手机号码
	@strMachineID NCHAR(32),					-- 机器标识
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 基本信息
DECLARE @UserID INT
DECLARE @Gender TINYINT
DECLARE @FaceID SMALLINT
DECLARE @CustomID INT
DECLARE @MoorMachine TINYINT
DECLARE @Accounts NVARCHAR(31)
DECLARE @NickName NVARCHAR(31)
DECLARE @UnderWrite NVARCHAR(63)

-- 积分变量
DECLARE @Score BIGINT
DECLARE @Insure BIGINT

-- 扩展信息
DECLARE @GameID INT
DECLARE @UserMedal INT
DECLARE @Experience INT
DECLARE @LoveLiness INT
DECLARE @SpreaderID INT
DECLARE @InsureEnabled INT
DECLARE @MemberOrder SMALLINT
DECLARE @MemberOverDate DATETIME

-- 辅助变量
DECLARE @EnjoinLogon AS INT
DECLARE @EnjoinRegister AS INT

-- 执行逻辑
BEGIN
	-- 注册暂停
	SELECT @EnjoinRegister=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinRegister'
	IF @EnjoinRegister IS NOT NULL AND @EnjoinRegister<>0
	BEGIN
		SELECT @strErrorDescribe=StatusString FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinRegister'
		RETURN 1
	END

	-- 登录暂停
	SELECT @EnjoinLogon=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinLogon'
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SELECT @strErrorDescribe=StatusString FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinLogon'
		RETURN 2
	END

	-- 效验名字
	IF (SELECT COUNT(*) FROM ConfineContent(NOLOCK) WHERE CHARINDEX(String,@strAccounts)>0)>0
	BEGIN
		SET @strErrorDescribe=N'抱歉地通知您，您所输入的登录帐号名含有限制字符串，请更换帐号名后再次申请帐号！'
		RETURN 4
	END

	-- 效验昵称
	IF (SELECT COUNT(*) FROM ConfineContent(NOLOCK) WHERE CHARINDEX(String,@strNickName)>0)>0
	BEGIN
		SET @strErrorDescribe=N'抱歉地通知您，您所输入的游戏昵称名含有限制字符串，请更换昵称名后再次申请帐号！'
		RETURN 4
	END

	-- 效验地址
	SELECT @EnjoinRegister=EnjoinRegister FROM ConfineAddress(NOLOCK) WHERE AddrString=@strClientIP AND GETDATE()<EnjoinOverDate
	IF @EnjoinRegister IS NOT NULL AND @EnjoinRegister<>0
	BEGIN
		SET @strErrorDescribe=N'抱歉地通知您，系统禁止了您所在的 IP 地址的注册功能，请联系客户服务中心了解详细情况！'
		RETURN 5
	END
	
	-- 效验机器
	SELECT @EnjoinRegister=EnjoinRegister FROM ConfineMachine(NOLOCK) WHERE MachineSerial=@strMachineID AND GETDATE()<EnjoinOverDate
	IF @EnjoinRegister IS NOT NULL AND @EnjoinRegister<>0
	BEGIN
		SET @strErrorDescribe=N'抱歉地通知您，系统禁止了您的机器的注册功能，请联系客户服务中心了解详细情况！'
		RETURN 6
	END

	-- 查询帐号
	IF EXISTS (SELECT UserID FROM AccountsInfo(NOLOCK) WHERE Accounts=@strAccounts)
	BEGIN
		SET @strErrorDescribe=N'此帐号已被注册，请换另一帐号尝试再次注册！'
		RETURN 7
	END

	-- 查询昵称
	--IF EXISTS (SELECT UserID FROM AccountsInfo(NOLOCK) WHERE NickName=@strNickName)
	--BEGIN
	--	SET @strErrorDescribe=N'此昵称已被注册，请换另一昵称尝试再次注册！'
	--	RETURN 7
	--END

	-- 注册用户
	INSERT AccountsInfo (Accounts,NickName,RegAccounts,LogonPass,InsurePass,Gender,FaceID,GameLogonTimes,LastLogonIP,LastLogonMobile,LastLogonMachine,RegisterIP,RegisterMobile,RegisterMachine)
	VALUES (@strAccounts,@strNickName,@strAccounts,@strLogonPass,@strInsurePass,@cbGender,@wFaceID,1,@strClientIP,@strMobilePhone,@strMachineID,@strClientIP,@strMobilePhone,@strMachineID)

	-- 错误判断
	IF @@ERROR<>0
	BEGIN
		SET @strErrorDescribe=N'由于意外的原因，帐号注册失败，请尝试再次注册！'
		RETURN 8
	END

	-- 查询用户
	SELECT @UserID=UserID, @GameID=GameID, @Accounts=Accounts, @NickName=NickName, @UnderWrite=UnderWrite, @FaceID=FaceID,
		@CustomID=CustomID, @Gender=Gender, @UserMedal=UserMedal, @Experience=Experience, @LoveLiness=LoveLiness, @MemberOrder=MemberOrder,
		@MemberOverDate=MemberOverDate, @MoorMachine=MoorMachine
	FROM AccountsInfo(NOLOCK) WHERE Accounts=@strAccounts

	-- 分配标识
	SELECT @GameID=GameID FROM GameIdentifier(NOLOCK) WHERE UserID=@UserID
	IF @GameID IS NULL 
	BEGIN
		SET @GameID=0
		SET @strErrorDescribe=N'用户注册成功，但未成功获取游戏 ID 号码，系统稍后将给您分配！'
	END
	ELSE UPDATE AccountsInfo SET GameID=@GameID WHERE UserID=@UserID

	-- 推广提成
	IF @SpreaderID<>0
	BEGIN
		DECLARE @RegisterGrantScore INT
		DECLARE @Note NVARCHAR(512)
		SET @Note = N'注册'
		SELECT @RegisterGrantScore = RegisterGrantScore FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GlobalSpreadInfo
		IF @RegisterGrantScore IS NULL
		BEGIN
			SET @RegisterGrantScore=5000
		END
		INSERT INTO QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.RecordSpreadInfo(
			UserID,Score,TypeID,ChildrenID,CollectNote)
		VALUES(@SpreaderID,@RegisterGrantScore,1,@UserID,@Note)		
	END

	-- 记录日志
	DECLARE @DateID INT
	SET @DateID=CAST(CAST(GETDATE() AS FLOAT) AS INT)
	UPDATE SystemStreamInfo SET GameRegisterSuccess=GameRegisterSuccess+1 WHERE DateID=@DateID
	IF @@ROWCOUNT=0 INSERT SystemStreamInfo (DateID, GameRegisterSuccess) VALUES (@DateID, 1)

	----------------------------------------------------------------------------------------------------
	----------------------------------------------------------------------------------------------------
	-- 注册赠送

	-- 读取变量
	DECLARE @GrantIPCount AS BIGINT
	DECLARE @GrantScoreCount AS BIGINT
	SELECT @GrantIPCount=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'GrantIPCount'
	SELECT @GrantScoreCount=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'GrantScoreCount'

	-- 赠送限制
	IF @GrantScoreCount IS NOT NULL AND @GrantScoreCount>0 AND @GrantIPCount IS NOT NULL AND @GrantIPCount>0
	BEGIN
		-- 赠送次数
		DECLARE @GrantCount AS BIGINT
		DECLARE @GrantMachineCount AS BIGINT
		SELECT @GrantCount=GrantCount FROM SystemGrantCount(NOLOCK) WHERE DateID=@DateID AND RegisterIP=@strClientIP
		SELECT @GrantMachineCount=GrantCount FROM SystemMachineGrantCount(NOLOCK) WHERE DateID=@DateID AND RegisterMachine=@strMachineID
	
		-- 次数判断
		IF (@GrantCount IS NOT NULL AND @GrantCount>=@GrantIPCount) OR (@GrantMachineCount IS NOT NULL AND @GrantMachineCount>=@GrantIPCount)
		BEGIN
			SET @GrantScoreCount=0
		END
	END

	-- 赠送金币
	IF @GrantScoreCount IS NOT NULL AND @GrantScoreCount>0
	BEGIN
		-- 更新记录
		UPDATE SystemGrantCount SET GrantScore=GrantScore+@GrantScoreCount, GrantCount=GrantCount+1 WHERE DateID=@DateID AND RegisterIP=@strClientIP

		-- 插入记录
		IF @@ROWCOUNT=0
		BEGIN
			INSERT SystemGrantCount (DateID, RegisterIP, RegisterMachine, GrantScore, GrantCount) VALUES (@DateID, @strClientIP, @strMachineID, @GrantScoreCount, 1)
		END

		-- 更新记录
		UPDATE SystemMachineGrantCount SET GrantScore=GrantScore+@GrantScoreCount, GrantCount=GrantCount+1 WHERE DateID=@DateID AND RegisterMachine=@strMachineID

		-- 插入记录
		IF @@ROWCOUNT=0
		BEGIN
			INSERT SystemMachineGrantCount (DateID, RegisterIP, RegisterMachine, GrantScore, GrantCount) VALUES (@DateID, @strClientIP, @strMachineID, @GrantScoreCount, 1)
		END

		-- 赠送金币
		INSERT QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo (UserID, Score, RegisterIP, LastLogonIP) VALUES (@UserID, @GrantScoreCount, @strClientIP, @strClientIP) 
	END

	----------------------------------------------------------------------------------------------------
	----------------------------------------------------------------------------------------------------
	SET @InsureEnabled=1
	IF @strInsurePass = N'' SET @InsureEnabled=0
	
	-- 查询金币
	SELECT @Score=Score, @Insure=InsureScore FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo WHERE UserID=@UserID

	-- 数据调整
	IF @Score IS NULL SET @Score=0
	IF @Insure IS NULL SET @Insure=0

	-- 输出变量
	SELECT @UserID AS UserID, @GameID AS GameID, @Accounts AS Accounts, @NickName AS NickName, @UnderWrite AS UnderWrite,
		@FaceID AS FaceID, @CustomID AS CustomID, @Gender AS Gender, @UserMedal AS UserMedal, @Experience AS Experience,
		@Score AS Score, @Insure AS Insure, @LoveLiness AS LoveLiness, @MemberOrder AS MemberOrder, @MemberOverDate AS MemberOverDate,
		@MoorMachine AS MoorMachine,@SpreaderID AS SpreaderID,@InsureEnabled AS InsureEnabled

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------
