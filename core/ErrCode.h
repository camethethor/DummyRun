#pragma once

enum DISCONNECT_ERRCODE
{
	GRACEFUL				= -1, 
	FORCED					= -2, 
	EXCEPTION				= -3, 
	SENDINGPACKETSIZE_FULL	= -4, 
	INVALID_HEADER			= -5,
};

enum LOGIN_ERRCODE
{
	LOGIN_SUCCESS,
	LOGIN_AUTHFAIL,
	LOGIN_INVALIDID,
};

enum CREATEGAMEROOM_ERRORCODE
{
	ERR_NONE,
	ERR_NOTENOUGH_ROOM,
	ERR_DONOTJOIN_CLAN,
	ERR_DONTSETTING_BASICCHACTER,
	ERR_WRONG_USERCOUNT,
};

enum ENTERROOM_ERRORCODE
{
	SUCCESS_ENTER_ROOM,			
	ERR_DEFAULT_CHARACTER,					// 기본 캐릭터 없음
	ERR_NOT_EXIST_ROOM,						// 존재하지 않는 방
	ERR_INVALID_PASSWORD,					// 패스워드가 틀림
	ERR_ISGAMING,							// 게임이 진행중
	ERR_BEOVERSTAFFED,						// 인원이 다 찼음
	ERR_DONTJOIN_CLAN,						// 클랜에 가입하지 않음
	ERR_PROGRESS_CLANMATCHING,				// 클랜매칭 중인 방에 동일한 클랜이 입장하지 않음
};
