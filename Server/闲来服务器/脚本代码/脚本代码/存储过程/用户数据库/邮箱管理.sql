
----------------------------------------------------------------------------------------------------

USE QPAccountsDB_HideSeek
GO


IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_AddMail]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_AddMail]
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO


----------------------------------------------------------------------------------------------------

-- д�ʼ�
CREATE PROC GSP_GP_AddMail
	@dwFromUserID INT,							-- �����û� I D��0��ʾϵͳ
	@dwToUserID INT,							-- �ռ��û� I D��0��ʾȺ��
	@strPassword NCHAR(32),						-- �����û�����
	@strMailTitle NVARCHAR(128),				-- �ʼ�����
	@strMailContent NVARCHAR(512),				-- �ʼ�����
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ��ѯ�û�
	IF @dwFromUserID<>0
	BEGIN
		IF not exists(SELECT * FROM AccountsInfo WHERE UserID=@dwFromUserID AND LogonPass=@strPassword)
		BEGIN
			SET @strErrorDescribe = N'��Ǹ�������û���Ϣ�����ڻ������벻��ȷ��'
			return 1
		END
	END

	--��������
	INSERT INTO SystemMailbox VALUES(@dwFromUserID,@dwToUserID,@strMailTitle,@strMailContent,GetDate(),0)		
		
	SET @strErrorDescribe = N'д�ʼ��ɹ�!'

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------