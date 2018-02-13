USE [master]
GO
/****** 对象:  Database [QPAccountsDB_HideSeek]    脚本日期: 12/08/2008 11:31:04 ******/
CREATE DATABASE [QPAccountsDB_HideSeek] ON  PRIMARY 
( NAME = N'QPAccountsDB_HideSeek', FILENAME = N'C:\数据库\QPAccountsDB_HideSeek.mdf' , SIZE = 3072KB , MAXSIZE = UNLIMITED, FILEGROWTH = 1024KB )
 LOG ON 
( NAME = N'QPAccountsDB_HideSeek_log', FILENAME = N'C:\数据库\QPAccountsDB_HideSeek_log.LDF' , SIZE = 1024KB , MAXSIZE = 2048GB , FILEGROWTH = 10%)
 COLLATE Chinese_PRC_CI_AS
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [QPAccountsDB_HideSeek].[dbo].[sp_fulltext_database] @action = 'disable'
end
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET ANSI_NULL_DEFAULT OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET ANSI_NULLS OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET ANSI_PADDING OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET ANSI_WARNINGS OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET ARITHABORT OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET AUTO_CLOSE OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET AUTO_CREATE_STATISTICS ON 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET AUTO_SHRINK OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET AUTO_UPDATE_STATISTICS ON 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET CURSOR_DEFAULT  GLOBAL 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET CONCAT_NULL_YIELDS_NULL OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET NUMERIC_ROUNDABORT OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET QUOTED_IDENTIFIER OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET RECURSIVE_TRIGGERS OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET  ENABLE_BROKER 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET TRUSTWORTHY OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET PARAMETERIZATION SIMPLE 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET  READ_WRITE 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET RECOVERY FULL 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET  MULTI_USER 
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET PAGE_VERIFY CHECKSUM  
GO
ALTER DATABASE [QPAccountsDB_HideSeek] SET DB_CHAINING OFF 

if ( ((@@microsoftversion / power(2, 24) = 8) and (@@microsoftversion & 0xffff >= 760)) or 
		(@@microsoftversion / power(2, 24) >= 9) )begin 
	exec dbo.sp_dboption @dbname =  N'QPAccountsDB_HideSeek', @optname = 'db chaining', @optvalue = 'OFF'
 end