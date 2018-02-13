
----------------------------------------------------------------------------------------------------

USE QPAccountsDB_HideSeek
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_EfficacyAccounts]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_EfficacyAccounts]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_MB_EfficacyAccounts]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_MB_EfficacyAccounts]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- �ʺŵ�¼
CREATE PROC GSP_GP_EfficacyAccounts
	@strAccounts NVARCHAR(31),					-- �û��ʺ�
	@strUid NVARCHAR(49),
	@strOpenid NVARCHAR(31),
	@strPassword NCHAR(32),						-- �û�����
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strMachineID NVARCHAR(32),					-- ������ʶ
	@nNeeValidateMBCard BIT,					-- �ܱ�У��
	@wKindID SMALLINT,							-- ��Ϸ I D		mChen add, for Match Time
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ������Ϣ
DECLARE @UserID INT
DECLARE @CustomID INT
DECLARE @FaceID SMALLINT
DECLARE @Accounts NVARCHAR(31)
DECLARE @NickName NVARCHAR(31)
DECLARE @UnderWrite NVARCHAR(63)
DECLARE @SpreaderID INT
DECLARE @PlayTimeCount INT

-- ���ֱ���
DECLARE @Score BIGINT
DECLARE @Insure BIGINT

-- ��չ��Ϣ
DECLARE @GameID INT
DECLARE @Gender TINYINT
DECLARE @UserMedal INT
DECLARE @Experience INT
DECLARE @LoveLiness INT
DECLARE @InsureEnabled INT
DECLARE @MemberOrder SMALLINT
DECLARE @MemberOverDate DATETIME
DECLARE @MemberSwitchDate DATETIME
DECLARE @ProtectID INT
DECLARE @PasswordID INT

-- ��������
DECLARE @EnjoinLogon AS INT

-- ִ���߼�
BEGIN
	-- ϵͳ��ͣ
	SELECT @EnjoinLogon=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinLogon'
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SELECT @strErrorDescribe=StatusString FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinLogon'
		RETURN 2
	END

	-- Ч���ַ
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineAddress(NOLOCK) WHERE AddrString=@strClientIP AND (EnjoinOverDate>GETDATE() OR EnjoinOverDate IS NULL)
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SET @strErrorDescribe=N'��Ǹ��֪ͨ����ϵͳ��ֹ�������ڵ� IP ��ַ�ĵ�¼���ܣ�����ϵ�ͻ����������˽���ϸ�����'
		RETURN 4
	END
	
	-- Ч�����
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineMachine(NOLOCK) WHERE MachineSerial=@strMachineID AND (EnjoinOverDate>GETDATE() OR EnjoinOverDate IS NULL)
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SET @strErrorDescribe=N'��Ǹ��֪ͨ����ϵͳ��ֹ�����Ļ����ĵ�¼���ܣ�����ϵ�ͻ����������˽���ϸ�����'
		RETURN 7
	END
	
	-- mChen add
	-- ͳһʹ��Uid------------------------
	DECLARE @UserIDOfUid INT
	DECLARE @UserIDOfOpenid INT
	DECLARE @strDescribe NVARCHAR(128)
	SELECT @UserIDOfUid=UserID FROM AccountsInfo(NOLOCK) WHERE Accounts=@strUid
	SELECT @UserIDOfOpenid=UserID FROM AccountsInfo(NOLOCK) WHERE Accounts=@strOpenid
	
	-- ����Unionid��Openid��
	IF @UserIDOfUid IS NOT NULL
	BEGIN
		UPDATE AccountsInfo SET Unionid=@strUid,Openid=@strOpenid WHERE UserID=@UserIDOfUid
	END
	IF @UserIDOfOpenid IS NOT NULL
	BEGIN
		UPDATE AccountsInfo SET Unionid=@strUid,Openid=@strOpenid WHERE UserID=@UserIDOfOpenid
	END
	
	IF @UserIDOfUid IS NOT NULL AND @UserIDOfOpenid IS NOT NULL
	BEGIN
		-- 2��Accounts�˺ţ�iOS, Android��
		
		DECLARE @SpreaderLevelOfUid INT
		DECLARE @SpreaderLevelOfOpenid INT
		SELECT @SpreaderLevelOfUid=SpreaderLevel FROM SpreadersInfo WHERE SpreaderID=@UserIDOfUid
		SELECT @SpreaderLevelOfOpenid=SpreaderLevel FROM SpreadersInfo WHERE SpreaderID=@UserIDOfOpenid
		IF @SpreaderLevelOfUid IS NOT NULL AND @SpreaderLevelOfOpenid IS NOT NULL
		BEGIN
			-- 2�����Ǵ�����
			-- ������
			SET @strDescribe=N'2�����Ǵ�����'
		END ELSE
		IF @SpreaderLevelOfUid IS NOT NULL 
		BEGIN
			-- Uid�Ǵ����ˣ�Openid���Ǵ�����
			-- ֱ��ɾ��Openid���˾���
			DELETE AccountsInfo WHERE UserID=@UserIDOfOpenid
		END ELSE
		IF @SpreaderLevelOfOpenid IS NOT NULL 
		BEGIN
			-- Openid�Ǵ����ˣ�Uid���Ǵ�����
			-- ����Openid���˵�AccountsΪUid��ɾ��Uid����
			UPDATE AccountsInfo SET Accounts=@strUid,RegAccounts=@strUid,Unionid=@strUid,Openid=@strOpenid WHERE UserID=@UserIDOfOpenid
			DELETE AccountsInfo WHERE UserID=@UserIDOfUid
		END ELSE
		BEGIN
			-- ���������Ǵ�����
			-- ֱ��ɾ��Openid���˾���
			DELETE AccountsInfo WHERE UserID=@UserIDOfOpenid
		END
		
	END ELSE
	IF @UserIDOfOpenid IS NOT NULL
	BEGIN
		-- ֻ��1��Openid��Accounts�˺ţ�Android��
		-- ������û�д����˺ţ�ֻҪ����AccountsΪUid������
		UPDATE AccountsInfo SET Accounts=@strUid,RegAccounts=@strUid,Unionid=@strUid,Openid=@strOpenid WHERE UserID=@UserIDOfOpenid
	END
	/*
	ELSE
	IF @UserIDOfUid IS NOT NULL
	BEGIN
		-- ֻ��1��Uid��Accounts�˺ţ�iOS��
		-- ���ù�
	END ELSE
	BEGIN
		-- û���˺ţ����û���
		-- ���ù�
	END
	*/
	---------------------------
 
	-- ��ѯ�û�
	DECLARE @Nullity TINYINT
	DECLARE @StunDown TINYINT
	DECLARE @LogonPass AS NCHAR(32)
	DECLARE	@MachineSerial NCHAR(32)
	DECLARE	@strInsurePass NCHAR(32)
	DECLARE @MoorMachine AS TINYINT
	SELECT @UserID=UserID, @GameID=GameID, @Accounts=Accounts, @NickName=NickName, @UnderWrite=UnderWrite, @LogonPass=LogonPass,@strInsurePass=InsurePass,
		@FaceID=FaceID, @CustomID=CustomID, @Gender=Gender, @Nullity=Nullity, @StunDown=StunDown, @UserMedal=UserMedal, @Experience=Experience,
		@LoveLiness=LoveLiness, @MemberOrder=MemberOrder, @MemberOverDate=MemberOverDate, @MemberSwitchDate=MemberSwitchDate,
		@MoorMachine=MoorMachine, @MachineSerial=LastLogonMachine,@SpreaderID=SpreaderID,@PlayTimeCount=PlayTimeCount,@PasswordID=PasswordID,@ProtectID=ProtectID
	FROM AccountsInfo(NOLOCK) WHERE Accounts=@strAccounts

	-- ��ѯ�û�
	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'�����ʺŲ����ڻ������������������֤���ٴγ��Ե�¼��'
		RETURN 1
	END	

	-- �ʺŽ�ֹ
	IF @Nullity<>0
	BEGIN
		SET @strErrorDescribe=N'�����ʺ���ʱ���ڶ���״̬������ϵ�ͻ����������˽���ϸ�����'
		RETURN 2
	END	

	-- �ʺŹر�
	IF @StunDown<>0
	BEGIN
		SET @strErrorDescribe=N'�����ʺ�ʹ���˰�ȫ�رչ��ܣ��������¿�ͨ����ܼ���ʹ�ã�'
		RETURN 2
	END	
	
	-- �̶�����
	IF @MoorMachine=1
	BEGIN
		IF @MachineSerial<>@strMachineID
		BEGIN
			SET @strErrorDescribe=N'�����ʺ�ʹ�ù̶�������¼���ܣ�������ʹ�õĻ���������ָ���Ļ�����'
			RETURN 1
		END
	END

	-- �����ж�
	IF @LogonPass<>@strPassword
	BEGIN
		SET @strErrorDescribe=N'�����ʺŲ����ڻ������������������֤���ٴγ��Ե�¼��'
		RETURN 3
	END

	-- �ܱ�У��
	IF @nNeeValidateMBCard=1 AND @PasswordID<>0
	BEGIN
		SELECT @PasswordID AS PasswordID
		RETURN 18
	END

	-- �̶�����
	IF @MoorMachine=2
	BEGIN
		SET @MoorMachine=1
		SET @strErrorDescribe=N'�����ʺųɹ�ʹ���˹̶�������¼���ܣ�'
		UPDATE AccountsInfo SET MoorMachine=@MoorMachine, LastLogonMachine=@strMachineID WHERE UserID=@UserID
	END

	-- �ƹ�Ա���
	IF @SpreaderID<>0 
	BEGIN
		DECLARE @GrantTime	INT
		DECLARE @GrantScore	BIGINT
		DECLARE @Note NVARCHAR(512)
		SET @Note = N'��Ϸʱ�����һ���Խ���'

		SELECT @GrantTime=PlayTimeCount,@GrantScore=PlayTimeGrantScore FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GlobalSpreadInfo
		WHERE ID=1
		IF @GrantTime IS NULL OR @GrantTime=0
		BEGIN
			SET @GrantTime = 108000 -- 30Сʱ
			SET @GrantScore = 200000
		END			
		IF @PlayTimeCount>=@GrantTime
		BEGIN
			-- ��ȡ�����Ϣ
			DECLARE @RecordID INT
			SELECT @RecordID=RecordID FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.RecordSpreadInfo
			WHERE UserID = @SpreaderID AND ChildrenID = @UserID AND TypeID = 2
			
			IF @RecordID IS NULL
			BEGIN
				INSERT INTO QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.RecordSpreadInfo(
					UserID,Score,TypeID,ChildrenID,CollectNote)
				VALUES(@SpreaderID,@GrantScore,2,@UserID,@Note)	
			END		
		END
	END

	-- ��ѯ���
	SELECT @Score=Score, @Insure=InsureScore FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo WHERE UserID=@UserID

	-- ���ݵ���
	IF @Score IS NULL SET @Score=0
	IF @Insure IS NULL SET @Insure=0

	-- ��Ա�ȼ�
	IF @MemberOrder<>0 AND GETDATE()>@MemberSwitchDate
	BEGIN
		DECLARE @UserRight INT	
		SET @UserRight=0
		
		-- ɾ����Ա
		DELETE AccountsMember WHERE UserID=@UserID AND MemberOverDate<=GETDATE()

		-- ������Ա
		SELECT @MemberOverDate=MAX(MemberOverDate), @MemberOrder=MAX(MemberOrder), @MemberSwitchDate=MIN(MemberOverDate)
			FROM AccountsMember(NOLOCK) WHERE UserID=@UserID

		-- ���ݵ���
		IF @MemberOrder IS NULL 
		BEGIN
			SET @MemberOrder=0
			SET @UserRight=512
		END
		IF @MemberOverDate IS NULL SET @MemberOverDate='1980-1-1'
		IF @MemberSwitchDate IS NULL SET @MemberSwitchDate='1980-1-1'

		-- ��������
		UPDATE AccountsInfo SET MemberOrder=@MemberOrder, MemberOverDate=@MemberOverDate, MemberSwitchDate=@MemberSwitchDate,
			UserRight=UserRight&~@UserRight WHERE UserID=@UserID
	END

	-- ������Ϣ
	UPDATE AccountsInfo SET GameLogonTimes=GameLogonTimes+1,LastLogonDate=GETDATE(), LastLogonIP=@strClientIP,
		LastLogonMachine=@strMachineID WHERE UserID=@UserID
		
	SET @InsureEnabled=1
	IF @strInsurePass = N'' SET @InsureEnabled=0

	-- ��¼��־
	DECLARE @DateID INT
	SET @DateID=CAST(CAST(GETDATE() AS FLOAT) AS INT)
	UPDATE SystemStreamInfo SET GameLogonSuccess=GameLogonSuccess+1 WHERE DateID=@DateID
	IF @@ROWCOUNT=0 INSERT SystemStreamInfo (DateID, GameLogonSuccess) VALUES (@DateID, 1)

	--mChen add, for Match Time
	DECLARE @MatchStartTime datetime
	DECLARE	@MatchEndTime datetime
	SELECT @MatchStartTime = MatchStartTime, @MatchEndTime = MatchEndTime FROM QPPlatformDB_HideSeekLink.QPPlatformDB_HideSeek.dbo.PrivateInfo WHERE KindID=@wKindID
	
	--mChen add, forǩ����¼
	DECLARE @wSeriesDate INT	
	SELECT @wSeriesDate=SeriesDate FROM AccountsSignin WHERE UserID=@UserID
	IF @wSeriesDate IS NULL
	BEGIN
		SET @wSeriesDate = 0	
	END
	
	--mChen add:��ѯ�û��Ѵ򳡴�,for�齱
	DECLARE @PlayCount INT
	SELECT  @PlayCount=COUNT(*) FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.PrivateGameRecordUserRecordID WHERE UserID = @UserID
	
	--mChen add:��ѯ�齱��¼
	DECLARE @RaffleCount INT	
	SELECT @RaffleCount=RaffleCount FROM AccountsRaffle WHERE UserID=@UserID
	IF @RaffleCount IS NULL
	BEGIN
		SET @RaffleCount = 0
	END
	-- ÿ������ٳ����Գ齱һ��
	DECLARE @PlayCountPerRaffle INT
	SELECT @PlayCountPerRaffle=StatusValue FROM SystemStatusInfo WHERE StatusName=N'PlayCountPerRaffle'
	IF @PlayCountPerRaffle IS NULL
	BEGIN
		SET @PlayCountPerRaffle = 20
	END
	
	--mChen add:��ѯ����ȼ�
	DECLARE @SpreaderLevel AS INT
	SELECT @SpreaderLevel=SpreaderLevel FROM SpreadersInfo WHERE SpreaderID=@UserID
	IF @SpreaderLevel IS NULL
	BEGIN
		SET @SpreaderLevel=-1
	END
	
	--mChen add:��ѯ������Ϣ
	DECLARE @strPublicNotice NVARCHAR(256)
	SELECT @strPublicNotice=StatusString FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'PublicNotice'
	IF @strPublicNotice IS NULL
	BEGIN
		SET @strPublicNotice=N''
	END
	
	--mChen add, for HideSeek:��ѯ����ģ�Ϳ�
	DECLARE @lModelIndex0 BIGINT
	SELECT @lModelIndex0=ModelIndex0 FROM AccountsTaggerModel(NOLOCK) WHERE UserID=@UserID
	IF @lModelIndex0 IS NULL
	BEGIN
		SET @lModelIndex0=1
		INSERT INTO AccountsTaggerModel(UserID,ModelIndex0)
		VALUES(@UserID,@lModelIndex0)	
	END
	
	--WQ ��ѯ�û�ͷ��
	DECLARE @strHeadHttp NVARCHAR(256)
	SELECT @strHeadHttp=HeadHttp FROM IndividualDatum WHERE UserID=@UserID
	IF @strHeadHttp IS NULL
	BEGIN
		SET @strHeadHttp=N'0'
	END
	
	-- �������
	SELECT @UserID AS UserID, @MatchStartTime AS MatchStartTime, @MatchEndTime AS MatchEndTime, 
		@wSeriesDate AS SeriesDate, @PlayCount AS PlayCount, 
		@RaffleCount AS RaffleCount, @PlayCountPerRaffle AS PlayCountPerRaffle,
		@SpreaderLevel AS SpreaderLevel,
		@strPublicNotice AS PublicNotice,
		@lModelIndex0 AS ModelIndex0,
		@GameID AS GameID, @Accounts AS Accounts, @NickName AS NickName, @UnderWrite AS UnderWrite,
		@FaceID AS FaceID, @CustomID AS CustomID, @Gender AS Gender, @UserMedal AS UserMedal, @Experience AS Experience,
		@Score AS Score, @Insure AS Insure, @LoveLiness AS LoveLiness, @MemberOrder AS MemberOrder, @MemberOverDate AS MemberOverDate,
		@MoorMachine AS MoorMachine,@PasswordID as PasswordID,@SpreaderID as SpreaderID,@InsureEnabled as InsureEnabled,
		@strHeadHttp AS HeadHttp

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- �ʺŵ�¼
CREATE PROC GSP_MB_EfficacyAccounts
	@strAccounts NVARCHAR(31),					-- �û��ʺ�
	@strPassword NCHAR(32),						-- �û�����
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strMachineID NVARCHAR(32),					-- ������ʶ
	@strMobilePhone NVARCHAR(11),				-- �ֻ�����
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ������Ϣ
DECLARE @UserID INT
DECLARE @CustomID INT
DECLARE @FaceID SMALLINT
DECLARE @Accounts NVARCHAR(31)
DECLARE @NickName NVARCHAR(31)
DECLARE @UnderWrite NVARCHAR(63)
DECLARE @SpreaderID INT
DECLARE @PlayTimeCount INT

-- ���ֱ���
DECLARE @Score BIGINT
DECLARE @Insure BIGINT

-- ��չ��Ϣ
DECLARE @GameID INT
DECLARE @Gender TINYINT
DECLARE @UserMedal INT
DECLARE @Experience INT
DECLARE @LoveLiness INT
DECLARE @InsureEnabled INT
DECLARE @MemberOrder SMALLINT
DECLARE @MemberOverDate DATETIME
DECLARE @MemberSwitchDate DATETIME

-- ��������
DECLARE @EnjoinLogon AS INT

-- ִ���߼�
BEGIN
	-- ϵͳ��ͣ
	SELECT @EnjoinLogon=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinLogon'
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SELECT @strErrorDescribe=StatusString FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinLogon'
		RETURN 2
	END

	-- Ч���ַ
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineAddress(NOLOCK) WHERE AddrString=@strClientIP AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SET @strErrorDescribe=N'��Ǹ��֪ͨ����ϵͳ��ֹ�������ڵ� IP ��ַ�ĵ�¼���ܣ�����ϵ�ͻ����������˽���ϸ�����'
		RETURN 4
	END
	
	-- Ч�����
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineMachine(NOLOCK) WHERE MachineSerial=@strMachineID AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SET @strErrorDescribe=N'��Ǹ��֪ͨ����ϵͳ��ֹ�����Ļ����ĵ�¼���ܣ�����ϵ�ͻ����������˽���ϸ�����'
		RETURN 7
	END

	-- ��ѯ�û�
	DECLARE @Nullity TINYINT
	DECLARE @StunDown TINYINT
	DECLARE @LogonPass AS NCHAR(32)
	DECLARE @strInsurePass AS NCHAR(32)
	DECLARE @MoorMachine AS TINYINT
	SELECT @UserID=UserID, @GameID=GameID, @Accounts=Accounts, @NickName=NickName, @UnderWrite=UnderWrite, @LogonPass=LogonPass,@strInsurePass=InsurePass,
		@FaceID=FaceID, @CustomID=CustomID, @Gender=Gender, @Nullity=Nullity, @StunDown=StunDown, @UserMedal=UserMedal, @Experience=Experience,
		@LoveLiness=LoveLiness, @MemberOrder=MemberOrder, @MemberOverDate=MemberOverDate, @MemberSwitchDate=MemberSwitchDate,
		@MoorMachine=MoorMachine,@SpreaderID=SpreaderID,@PlayTimeCount=PlayTimeCount
	FROM AccountsInfo(NOLOCK) WHERE Accounts=@strAccounts

	-- ��ѯ�û�
	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'�����ʺŲ����ڻ������������������֤���ٴγ��Ե�¼��'
		RETURN 1
	END	

	-- �ʺŽ�ֹ
	IF @Nullity<>0
	BEGIN
		SET @strErrorDescribe=N'�����ʺ���ʱ���ڶ���״̬������ϵ�ͻ����������˽���ϸ�����'
		RETURN 2
	END	

	-- �ʺŹر�
	IF @StunDown<>0
	BEGIN
		SET @strErrorDescribe=N'�����ʺ�ʹ���˰�ȫ�رչ��ܣ��������¿�ͨ����ܼ���ʹ�ã�'
		RETURN 2
	END	
	
	-- �̶�����
	IF @MoorMachine <> 0
	BEGIN
		SET @strErrorDescribe=N'�����ʺ�ʹ�ù̶�������¼���ܣ�������ʹ���ֻ��ն˽��е�¼��'
		RETURN 1
	END

	-- �����ж�
	IF @LogonPass<>@strPassword
	BEGIN
		SET @strErrorDescribe=N'�����ʺŲ����ڻ������������������֤���ٴγ��Ե�¼��'
		RETURN 3
	END

	-- �ƹ�Ա���
	IF @SpreaderID<>0 
	BEGIN
		DECLARE @GrantTime	INT
		DECLARE @GrantScore	BIGINT
		DECLARE @Note NVARCHAR(512)
		SET @Note = N'��Ϸʱ�����һ���Խ���'

		SELECT @GrantTime=PlayTimeCount,@GrantScore=PlayTimeGrantScore FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GlobalSpreadInfo
		WHERE ID=1
		IF @GrantTime IS NULL OR @GrantTime=0
		BEGIN
			SET @GrantTime = 108000 -- 30Сʱ
			SET @GrantScore = 200000
		END			
		IF @PlayTimeCount>=@GrantTime
		BEGIN
			-- ��ȡ�����Ϣ
			DECLARE @RecordID INT
			SELECT @RecordID=RecordID FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.RecordSpreadInfo
			WHERE UserID = @SpreaderID AND ChildrenID = @UserID AND TypeID = 2
			IF @RecordID IS NULL
			BEGIN
				INSERT INTO QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.RecordSpreadInfo(
					UserID,Score,TypeID,ChildrenID,CollectNote)
				VALUES(@SpreaderID,@GrantScore,2,@UserID,@Note)	
			END		
		END
	END

	-- ��ѯ���
	SELECT @Score=Score, @Insure=InsureScore FROM QPTreasureDB_HideSeekLink.QPTreasureDB_HideSeek.dbo.GameScoreInfo WHERE UserID=@UserID

	-- ���ݵ���
	IF @Score IS NULL SET @Score=0
	IF @Insure IS NULL SET @Insure=0

	-- ��Ա�ȼ�
	IF @MemberOrder<>0 AND GETDATE()>@MemberSwitchDate
	BEGIN
		DECLARE @UserRight INT	
		SET @UserRight=0
		
		-- ɾ����Ա
		DELETE AccountsMember WHERE UserID=@UserID AND MemberOverDate<=GETDATE()

		-- ������Ա
		SELECT @MemberOverDate=MAX(MemberOverDate), @MemberOrder=MAX(MemberOrder), @MemberSwitchDate=MIN(MemberOverDate)
			FROM AccountsMember(NOLOCK) WHERE UserID=@UserID

		-- ���ݵ���
		IF @MemberOrder IS NULL 
		BEGIN
			SET @MemberOrder=0
			SET @UserRight=512
		END
		IF @MemberOverDate IS NULL SET @MemberOverDate='1980-1-1'
		IF @MemberSwitchDate IS NULL SET @MemberSwitchDate='1980-1-1'

		-- ��������
		UPDATE AccountsInfo SET MemberOrder=@MemberOrder, MemberOverDate=@MemberOverDate, MemberSwitchDate=@MemberSwitchDate,
			UserRight=UserRight&~@UserRight WHERE UserID=@UserID
	END
	
	SET @InsureEnabled=1
	IF @strInsurePass = N'' SET @InsureEnabled=0

	-- ������Ϣ
	UPDATE AccountsInfo SET GameLogonTimes=GameLogonTimes+1, LastLogonDate=GETDATE(), LastLogonIP=@strClientIP,
		LastLogonMachine=@strMachineID, LastLogonMobile=@strMobilePhone WHERE UserID=@UserID

	-- ��¼��־
	DECLARE @DateID INT
	SET @DateID=CAST(CAST(GETDATE() AS FLOAT) AS INT)
	UPDATE SystemStreamInfo SET GameLogonSuccess=GameLogonSuccess+1 WHERE DateID=@DateID
	IF @@ROWCOUNT=0 INSERT SystemStreamInfo (DateID, GameLogonSuccess) VALUES (@DateID, 1)

	-- �������
	SELECT @UserID AS UserID, @GameID AS GameID, @Accounts AS Accounts, @NickName AS NickName, @UnderWrite AS UnderWrite,
		@FaceID AS FaceID, @CustomID AS CustomID, @Gender AS Gender, @UserMedal AS UserMedal, @Experience AS Experience,
		@Score AS Score, @Insure AS Insure, @LoveLiness AS LoveLiness, @MemberOrder AS MemberOrder, @MemberOverDate AS MemberOverDate,
		@MoorMachine AS MoorMachine,@SpreaderID AS SpreaderID,@InsureEnabled AS InsureEnabled

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------
