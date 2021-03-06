#include "npc_lua.h"
#include "util.h"
#include "char.h"
#include "char_base.h"
#include "anim_tbl.h"
#include "object.h"
#include "net.h"
#include "npcutil.h"
#include "npc_eventaction.h"
#include "battle.h"
#include "readmap.h"
#include "sasql.h"
#include "pet_skill.h"

#ifdef _JZ_NEWSCRIPT_LUA

//////////////////////////////////////////////////////////////////////////////
//设置数据的接口
int NPC_Lua_Char_ClrEvtEnd(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, LUA_MINSTACK);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Max = lua_gettop(_NLL);
	int TM_EveNo = -1;
	int i = 0;
	for(i = 2; i <= TM_Max; i++)
	{
		TM_EveNo = (int)lua_tointeger(_NLL, i);
		NPC_EndEventSetFlgCls(TM_Index, TM_EveNo);
	}

	LRetInt(_NLL, TM_EveNo);
}

int NPC_Lua_Char_ClrEvtNow(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, LUA_MINSTACK);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Max = lua_gettop(_NLL);
	int TM_EveNo = -1;
	int i = 0;
	for(i = 2; i <= TM_Max; i++)
	{
		TM_EveNo = (int)lua_tointeger(_NLL, i);
		NPC_NowEventSetFlgCls(TM_Index, TM_EveNo);
	}
	LRetInt(_NLL, TM_EveNo);
}

int NPC_Lua_Char_ClrEvt(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, LUA_MINSTACK);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Max = lua_gettop(_NLL);
	int TM_EveNo = -1;
	int i = 0;
	for(i = 2; i <= TM_Max; i++)
	{
		TM_EveNo = (int)lua_tointeger(_NLL, i);
		NPC_NowEndEventSetFlgCls(TM_Index, TM_EveNo);
	}
	LRetInt(_NLL, TM_EveNo);
}

int NPC_Lua_Char_SetEvtEnd(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, LUA_MINSTACK);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Max = lua_gettop(_NLL);
	int TM_EveNo = -1;
	int i = 0;
	for(i = 2; i <= TM_Max; i++)
	{
		TM_EveNo = (int)lua_tointeger(_NLL, i);
		NPC_EventSetFlg(TM_Index, TM_EveNo);
	}

	LRetInt(_NLL, TM_EveNo);
}

int NPC_Lua_Char_SetEvtNow(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, LUA_MINSTACK);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Max = lua_gettop(_NLL);
	int TM_EveNo = -1;
	int i = 0;
	for(i = 2; i <= TM_Max; i++)
	{
		TM_EveNo = (int)lua_tointeger(_NLL, i);
		NPC_NowEventSetFlg(TM_Index, TM_EveNo);
	}

	LRetInt(_NLL, TM_EveNo);
}

int NPC_Lua_Char_SetData(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Flg = (int)lua_tointeger(_NLL, 2);

	if(TM_Flg >= 0)
	{
		if(TM_Flg < LUA_DATALINE1)
		{
			TM_Flg -= LUA_DATALINE0;
			if(TM_Flg == CHAR_WHICHTYPE)
			{
				LRetInt(_NLL, 0);
			}
			int TM_IntVal = (int)lua_tointeger(_NLL, 3);
#ifdef _JZ_BILLING_SYSTEM
			if(TM_Flg == CHAR_VIPPOINT)
			{
				saacproto_IncreaseMPoint_send(acfd, -1, CHAR_getChar( TM_Index, CHAR_CDKEY ), "增加会员点", TM_IntVal - CHAR_getInt(TM_Index, TM_Flg));
			}
#endif
			int TM_RetInt = CHAR_setInt(TM_Index, TM_Flg, TM_IntVal);
#ifdef _JZ_SAVE
			if(TM_Flg == CHAR_MEMBERORDER)
			{
				CHAR_charSaveFromConnect(TM_Index, FALSE);
			}
#endif
			LRetInt(_NLL, TM_RetInt);
		}else if(TM_Flg < LUA_DATALINE2)
		{
			TM_Flg -= LUA_DATALINE1;
			char *TM_CharPoint = (char *)lua_tostring(_NLL, 3);
			BOOL TM_RetBOOL = CHAR_setChar(TM_Index, TM_Flg, TM_CharPoint);
			LRetBool(_NLL, TM_RetBOOL);
		}else if(TM_Flg < LUA_DATALINE3)
		{
			TM_Flg -= LUA_DATALINE2;
			if(TM_Flg == CHAR_WORKOBJINDEX || TM_Flg == CHAR_WORKACTION)
			{
				LRetInt(_NLL, 0);
			}
			int TM_WorkIntVal = (int)lua_tointeger(_NLL, 3);
			int TM_RetInt = CHAR_setWorkInt(TM_Index, TM_Flg, TM_WorkIntVal);
			LRetInt(_NLL, TM_RetInt);
		}else if(TM_Flg < LUA_DATALINE4)
		{
			TM_Flg -= LUA_DATALINE3;
			char *TM_WorkCharPoint = (char *)lua_tostring(_NLL, 3);
			BOOL TM_RetBOOL = CHAR_setWorkChar(TM_Index, TM_Flg, TM_WorkCharPoint);
			LRetBool(_NLL, TM_RetBOOL);
		}else if(TM_Flg < LUA_DATALINE5)
		{
			TM_Flg -= LUA_DATALINE4;
			BOOL TM_BOOLVal = (BOOL)lua_toboolean(_NLL, 3);
			int TM_RetInt = CHAR_setFlg(TM_Index, TM_Flg, TM_BOOLVal);
			LRetInt(_NLL, TM_RetInt);
		}else
		{
			LRetErrNull(_NLL, "传入的标志是错误的。");
		}
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}

//////////////////////////////////////////////////////////////////////////////
//事件设置的接口
int NPC_Lua_Char_SetWalkPreEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_WALKPREFUNC] != (void *)NPC_Lua_WalkPreCallBack)
		{
			if(TM_char->functable[CHAR_WALKPREFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_WALKPREFUNC] = TM_char->functable[CHAR_WALKPREFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_WALKPREFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_WALKPREFUNC].string), TM_char->charfunctable[CHAR_WALKPREFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_WALKPREFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_WALKPREFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_WALKPREFUNC] = (void *)NPC_Lua_WalkPreCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_WALKPREFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_WALKPREFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_WALKPREFUNC].string, sizeof(TM_char->charfunctable[CHAR_WALKPREFUNC].string), FUNCNAME_WALKPRECALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_WALKPREFUNC] == (void *)NPC_Lua_WalkPreCallBack)
		{
			TM_char->lua_charfunctable[CHAR_WALKPREFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_WALKPREFUNC] == NULL)
			{
				TM_char->functable[CHAR_WALKPREFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_WALKPREFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_WALKPREFUNC] = TM_char->sur_functable[CHAR_WALKPREFUNC];
				TM_char->sur_functable[CHAR_WALKPREFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_WALKPREFUNC].string, sizeof(TM_char->charfunctable[CHAR_WALKPREFUNC].string), TM_char->sur_charfunctable[CHAR_WALKPREFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetWalkPostEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_WALKPOSTFUNC] != (void *)NPC_Lua_WalkPostCallBack)
		{
			if(TM_char->functable[CHAR_WALKPOSTFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_WALKPOSTFUNC] = TM_char->functable[CHAR_WALKPOSTFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_WALKPOSTFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_WALKPOSTFUNC].string), TM_char->charfunctable[CHAR_WALKPOSTFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_WALKPOSTFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_WALKPOSTFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_WALKPOSTFUNC] = (void *)NPC_Lua_WalkPostCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_WALKPOSTFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_WALKPOSTFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_WALKPOSTFUNC].string, sizeof(TM_char->charfunctable[CHAR_WALKPOSTFUNC].string), FUNCNAME_WALKPOSTCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_WALKPOSTFUNC] == (void *)NPC_Lua_WalkPostCallBack)
		{
			TM_char->lua_charfunctable[CHAR_WALKPOSTFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_WALKPOSTFUNC] == NULL)
			{
				TM_char->functable[CHAR_WALKPOSTFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_WALKPOSTFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_WALKPOSTFUNC] = TM_char->sur_functable[CHAR_WALKPOSTFUNC];
				TM_char->sur_functable[CHAR_WALKPOSTFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_WALKPOSTFUNC].string, sizeof(TM_char->charfunctable[CHAR_WALKPOSTFUNC].string), TM_char->sur_charfunctable[CHAR_WALKPOSTFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetPreOverEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_PREOVERFUNC] != (void *)NPC_Lua_PreOverCallBack)
		{
			if(TM_char->functable[CHAR_PREOVERFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_PREOVERFUNC] = TM_char->functable[CHAR_PREOVERFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_PREOVERFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_PREOVERFUNC].string), TM_char->charfunctable[CHAR_PREOVERFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_PREOVERFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_PREOVERFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_PREOVERFUNC] = (void *)NPC_Lua_PreOverCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_PREOVERFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_PREOVERFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_PREOVERFUNC].string, sizeof(TM_char->charfunctable[CHAR_PREOVERFUNC].string), FUNCNAME_PREOVERCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_PREOVERFUNC] == (void *)NPC_Lua_PreOverCallBack)
		{
			TM_char->lua_charfunctable[CHAR_PREOVERFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_PREOVERFUNC] == NULL)
			{
				TM_char->functable[CHAR_PREOVERFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_PREOVERFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_PREOVERFUNC] = TM_char->sur_functable[CHAR_PREOVERFUNC];
				TM_char->sur_functable[CHAR_PREOVERFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_PREOVERFUNC].string, sizeof(TM_char->charfunctable[CHAR_PREOVERFUNC].string), TM_char->sur_charfunctable[CHAR_PREOVERFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetPostOverEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_POSTOVERFUNC] != (void *)NPC_Lua_PostOverCallBack)
		{
			if(TM_char->functable[CHAR_POSTOVERFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_POSTOVERFUNC] = TM_char->functable[CHAR_POSTOVERFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_POSTOVERFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_POSTOVERFUNC].string), TM_char->charfunctable[CHAR_POSTOVERFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_POSTOVERFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_POSTOVERFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_POSTOVERFUNC] = (void *)NPC_Lua_PostOverCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_POSTOVERFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_POSTOVERFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_POSTOVERFUNC].string, sizeof(TM_char->charfunctable[CHAR_POSTOVERFUNC].string), FUNCNAME_POSTOVERCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_POSTOVERFUNC] == (void *)NPC_Lua_PostOverCallBack)
		{
			TM_char->lua_charfunctable[CHAR_POSTOVERFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_POSTOVERFUNC] == NULL)
			{
				TM_char->functable[CHAR_POSTOVERFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_POSTOVERFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_POSTOVERFUNC] = TM_char->sur_functable[CHAR_POSTOVERFUNC];
				TM_char->sur_functable[CHAR_POSTOVERFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_POSTOVERFUNC].string, sizeof(TM_char->charfunctable[CHAR_POSTOVERFUNC].string), TM_char->sur_charfunctable[CHAR_POSTOVERFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetWatchEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_WATCHFUNC] != (void *)NPC_Lua_WatchCallBack)
		{
			if(TM_char->functable[CHAR_WATCHFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_WATCHFUNC] = TM_char->functable[CHAR_WATCHFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_WATCHFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_WATCHFUNC].string), TM_char->charfunctable[CHAR_WATCHFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_WATCHFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_WATCHFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_WATCHFUNC] = (void *)NPC_Lua_WatchCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_WATCHFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_WATCHFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_WATCHFUNC].string, sizeof(TM_char->charfunctable[CHAR_WATCHFUNC].string), FUNCNAME_WATCHCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_WATCHFUNC] == (void *)NPC_Lua_WatchCallBack)
		{
			TM_char->lua_charfunctable[CHAR_WATCHFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_WATCHFUNC] == NULL)
			{
				TM_char->functable[CHAR_WATCHFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_WATCHFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_WATCHFUNC] = TM_char->sur_functable[CHAR_WATCHFUNC];
				TM_char->sur_functable[CHAR_WATCHFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_WATCHFUNC].string, sizeof(TM_char->charfunctable[CHAR_WATCHFUNC].string), TM_char->sur_charfunctable[CHAR_WATCHFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetLoopEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 4);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);
	int TM_LoopInterval = (int)lua_tointeger(_NLL, 4);
	int TM_RetLoopInterval = -1;

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_LOOPFUNC] != (void *)NPC_Lua_LoopCallBack)
		{
			if(TM_char->functable[CHAR_LOOPFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_LOOPFUNC] = TM_char->functable[CHAR_LOOPFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_LOOPFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_LOOPFUNC].string), TM_char->charfunctable[CHAR_LOOPFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_LOOPFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_LOOPFUNC].string[0] = '\0';
			}
			TM_RetLoopInterval = CHAR_getInt(TM_index, CHAR_LOOPINTERVAL);
			CHAR_setInt(TM_index, CHAR_LOOPINTERVAL, TM_LoopInterval);
			TM_char->functable[CHAR_LOOPFUNC] = (void *)NPC_Lua_LoopCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_LOOPFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_LOOPFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_LOOPFUNC].string, sizeof(TM_char->charfunctable[CHAR_LOOPFUNC].string), FUNCNAME_LOOPCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_LOOPFUNC] == (void *)NPC_Lua_LoopCallBack)
		{
			TM_char->lua_charfunctable[CHAR_LOOPFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_LOOPFUNC] == NULL)
			{
				CHAR_setInt(TM_index, CHAR_LOOPINTERVAL, -1);
				TM_char->functable[CHAR_LOOPFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_LOOPFUNC].string[0] = '\0';
			}else
			{
				CHAR_setInt(TM_index, CHAR_LOOPINTERVAL, TM_LoopInterval);
				TM_char->functable[CHAR_LOOPFUNC] = TM_char->sur_functable[CHAR_LOOPFUNC];
				TM_char->sur_functable[CHAR_LOOPFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_LOOPFUNC].string, sizeof(TM_char->charfunctable[CHAR_LOOPFUNC].string), TM_char->sur_charfunctable[CHAR_LOOPFUNC].string);
			}
		}
	}
	lua_pushinteger(_NLL, TM_RetLoopInterval);
	LRet(TM_Ret + 1);
}

int NPC_Lua_Char_SetTalkedEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_TALKEDFUNC] != (void *)NPC_Lua_TalkedCallBack)
		{
			if(TM_char->functable[CHAR_TALKEDFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_TALKEDFUNC] = TM_char->functable[CHAR_TALKEDFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_TALKEDFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_TALKEDFUNC].string), TM_char->charfunctable[CHAR_TALKEDFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_TALKEDFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_TALKEDFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_TALKEDFUNC] = (void *)NPC_Lua_TalkedCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_TALKEDFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_TALKEDFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_TALKEDFUNC].string, sizeof(TM_char->charfunctable[CHAR_TALKEDFUNC].string), FUNCNAME_TALKEDCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_TALKEDFUNC] == (void *)NPC_Lua_TalkedCallBack)
		{
			TM_char->lua_charfunctable[CHAR_TALKEDFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_TALKEDFUNC] == NULL)
			{
				TM_char->functable[CHAR_TALKEDFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_TALKEDFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_TALKEDFUNC] = TM_char->sur_functable[CHAR_TALKEDFUNC];
				TM_char->sur_functable[CHAR_TALKEDFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_TALKEDFUNC].string, sizeof(TM_char->charfunctable[CHAR_TALKEDFUNC].string), TM_char->sur_charfunctable[CHAR_TALKEDFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetOFFEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_OFFFUNC] != (void *)NPC_Lua_OFFCallBack)
		{
			if(TM_char->functable[CHAR_OFFFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_OFFFUNC] = TM_char->functable[CHAR_OFFFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_OFFFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_OFFFUNC].string), TM_char->charfunctable[CHAR_OFFFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_OFFFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_OFFFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_OFFFUNC] = (void *)NPC_Lua_OFFCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_OFFFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_OFFFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_OFFFUNC].string, sizeof(TM_char->charfunctable[CHAR_OFFFUNC].string), FUNCNAME_OFFCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_OFFFUNC] == (void *)NPC_Lua_OFFCallBack)
		{
			TM_char->lua_charfunctable[CHAR_OFFFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_OFFFUNC] == NULL)
			{
				TM_char->functable[CHAR_OFFFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_OFFFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_OFFFUNC] = TM_char->sur_functable[CHAR_OFFFUNC];
				TM_char->sur_functable[CHAR_OFFFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_OFFFUNC].string, sizeof(TM_char->charfunctable[CHAR_OFFFUNC].string), TM_char->sur_charfunctable[CHAR_OFFFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetLookedEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_LOOKEDFUNC] != (void *)NPC_Lua_LookedCallBack)
		{
			if(TM_char->functable[CHAR_LOOKEDFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_LOOKEDFUNC] = TM_char->functable[CHAR_LOOKEDFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_LOOKEDFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_LOOKEDFUNC].string), TM_char->charfunctable[CHAR_LOOKEDFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_LOOKEDFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_LOOKEDFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_LOOKEDFUNC] = (void *)NPC_Lua_LookedCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_LOOKEDFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_LOOKEDFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_LOOKEDFUNC].string, sizeof(TM_char->charfunctable[CHAR_LOOKEDFUNC].string), FUNCNAME_LOOKEDCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_LOOKEDFUNC] == (void *)NPC_Lua_LookedCallBack)
		{
			TM_char->lua_charfunctable[CHAR_LOOKEDFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_LOOKEDFUNC] == NULL)
			{
				TM_char->functable[CHAR_LOOKEDFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_LOOKEDFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_LOOKEDFUNC] = TM_char->sur_functable[CHAR_LOOKEDFUNC];
				TM_char->sur_functable[CHAR_LOOKEDFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_LOOKEDFUNC].string, sizeof(TM_char->charfunctable[CHAR_LOOKEDFUNC].string), TM_char->sur_charfunctable[CHAR_LOOKEDFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetItemPutEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_ITEMPUTFUNC] != (void *)NPC_Lua_ItemPutCallBack)
		{
			if(TM_char->functable[CHAR_ITEMPUTFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_ITEMPUTFUNC] = TM_char->functable[CHAR_ITEMPUTFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_ITEMPUTFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_ITEMPUTFUNC].string), TM_char->charfunctable[CHAR_ITEMPUTFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_ITEMPUTFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_ITEMPUTFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_ITEMPUTFUNC] = (void *)NPC_Lua_ItemPutCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_ITEMPUTFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_ITEMPUTFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_ITEMPUTFUNC].string, sizeof(TM_char->charfunctable[CHAR_ITEMPUTFUNC].string), FUNCNAME_ITEMPUTCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_ITEMPUTFUNC] == (void *)NPC_Lua_ItemPutCallBack)
		{
			TM_char->lua_charfunctable[CHAR_ITEMPUTFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_ITEMPUTFUNC] == NULL)
			{
				TM_char->functable[CHAR_ITEMPUTFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_ITEMPUTFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_ITEMPUTFUNC] = TM_char->sur_functable[CHAR_ITEMPUTFUNC];
				TM_char->sur_functable[CHAR_ITEMPUTFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_ITEMPUTFUNC].string, sizeof(TM_char->charfunctable[CHAR_ITEMPUTFUNC].string), TM_char->sur_charfunctable[CHAR_ITEMPUTFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetWindowTalkedEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_WINDOWTALKEDFUNC] != (void *)NPC_Lua_WindowTalkedCallBack)
		{
			if(TM_char->functable[CHAR_WINDOWTALKEDFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_WINDOWTALKEDFUNC] = TM_char->functable[CHAR_WINDOWTALKEDFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_WINDOWTALKEDFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_WINDOWTALKEDFUNC].string), TM_char->charfunctable[CHAR_WINDOWTALKEDFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_WINDOWTALKEDFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_WINDOWTALKEDFUNC].string[0] = '\0';
			}

			TM_char->functable[CHAR_WINDOWTALKEDFUNC] = (void *)NPC_Lua_WindowTalkedCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_WINDOWTALKEDFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_WINDOWTALKEDFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_WINDOWTALKEDFUNC].string, sizeof(TM_char->charfunctable[CHAR_WINDOWTALKEDFUNC].string), FUNCNAME_WINDOWTALKEDCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_WINDOWTALKEDFUNC] == (void *)NPC_Lua_WindowTalkedCallBack)
		{
			TM_char->lua_charfunctable[CHAR_WINDOWTALKEDFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_WINDOWTALKEDFUNC] == NULL)
			{
				TM_char->functable[CHAR_WINDOWTALKEDFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_WINDOWTALKEDFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_WINDOWTALKEDFUNC] = TM_char->sur_functable[CHAR_WINDOWTALKEDFUNC];
				TM_char->sur_functable[CHAR_WINDOWTALKEDFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_WINDOWTALKEDFUNC].string, sizeof(TM_char->charfunctable[CHAR_WINDOWTALKEDFUNC].string), TM_char->sur_charfunctable[CHAR_WINDOWTALKEDFUNC].string);
			}
		}
	}
	return TM_Ret;
}

#ifdef _USER_CHARLOOPS
int NPC_Lua_Char_SetCharLoopsEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 4);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);
	int TM_LoopInterval = (int)lua_tointeger(_NLL, 4);
	int TM_RetLoopInterval = -1;

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_LOOPFUNCTEMP1] != (void *)NPC_Lua_CharLoopsCallBack)
		{
			if(TM_char->functable[CHAR_LOOPFUNCTEMP1] != NULL)
			{
				TM_char->sur_functable[CHAR_LOOPFUNCTEMP1] = TM_char->functable[CHAR_LOOPFUNCTEMP1];
				strcpy_s(TM_char->sur_charfunctable[CHAR_LOOPFUNCTEMP1].string, sizeof(TM_char->sur_charfunctable[CHAR_LOOPFUNCTEMP1].string), TM_char->charfunctable[CHAR_LOOPFUNCTEMP1].string);
			}else
			{
				TM_char->sur_functable[CHAR_LOOPFUNCTEMP1] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_LOOPFUNCTEMP1].string[0] = '\0';
			}
			TM_RetLoopInterval = CHAR_getInt(TM_index, CHAR_LOOPINTERVAL);
			CHAR_setInt(TM_index, CHAR_LOOPINTERVAL, TM_LoopInterval);
			TM_char->functable[CHAR_LOOPFUNCTEMP1] = (void *)NPC_Lua_CharLoopsCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_LOOPFUNCTEMP1].string, sizeof(TM_char->lua_charfunctable[CHAR_LOOPFUNCTEMP1].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_LOOPFUNCTEMP1].string, sizeof(TM_char->charfunctable[CHAR_LOOPFUNCTEMP1].string), FUNCNAME_CHARLOOPSCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_LOOPFUNCTEMP1] == (void *)NPC_Lua_CharLoopsCallBack)
		{
			TM_char->lua_charfunctable[CHAR_LOOPFUNCTEMP1].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_LOOPFUNCTEMP1] == NULL)
			{
				CHAR_setInt(TM_index, CHAR_LOOPINTERVAL, -1);
				TM_char->functable[CHAR_LOOPFUNCTEMP1] = (void *)NULL;
				TM_char->charfunctable[CHAR_LOOPFUNCTEMP1].string[0] = '\0';
			}else
			{
				CHAR_setInt(TM_index, CHAR_LOOPINTERVAL, TM_LoopInterval);
				TM_char->functable[CHAR_LOOPFUNCTEMP1] = TM_char->sur_functable[CHAR_LOOPFUNCTEMP1];
				TM_char->sur_functable[CHAR_LOOPFUNCTEMP1] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_LOOPFUNCTEMP1].string, sizeof(TM_char->charfunctable[CHAR_LOOPFUNCTEMP1].string), TM_char->sur_charfunctable[CHAR_LOOPFUNCTEMP1].string);
			}
		}
	}
	lua_pushinteger(_NLL, TM_RetLoopInterval);
	LRet(TM_Ret + 1);
}

int NPC_Lua_Char_SetBattleProPertyEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_BATTLEPROPERTY] != (void *)NPC_Lua_BattleProPertyCallBack)
		{
			if(TM_char->functable[CHAR_BATTLEPROPERTY] != NULL)
			{
				TM_char->sur_functable[CHAR_BATTLEPROPERTY] = TM_char->functable[CHAR_BATTLEPROPERTY];
				strcpy_s(TM_char->sur_charfunctable[CHAR_BATTLEPROPERTY].string, sizeof(TM_char->sur_charfunctable[CHAR_BATTLEPROPERTY].string), TM_char->charfunctable[CHAR_BATTLEPROPERTY].string);
			}else
			{
				TM_char->sur_functable[CHAR_BATTLEPROPERTY] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_BATTLEPROPERTY].string[0] = '\0';
			}

			TM_char->functable[CHAR_BATTLEPROPERTY] = (void *)NPC_Lua_BattleProPertyCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_BATTLEPROPERTY].string, sizeof(TM_char->lua_charfunctable[CHAR_BATTLEPROPERTY].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_BATTLEPROPERTY].string, sizeof(TM_char->charfunctable[CHAR_BATTLEPROPERTY].string), FUNCNAME_BATTLEPROPERTYCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_BATTLEPROPERTY] == (void *)NPC_Lua_BattleProPertyCallBack)
		{
			TM_char->lua_charfunctable[CHAR_BATTLEPROPERTY].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_BATTLEPROPERTY] == NULL)
			{
				TM_char->functable[CHAR_BATTLEPROPERTY] = (void *)NULL;
				TM_char->charfunctable[CHAR_BATTLEPROPERTY].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_BATTLEPROPERTY] = TM_char->sur_functable[CHAR_BATTLEPROPERTY];
				TM_char->sur_functable[CHAR_BATTLEPROPERTY] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_BATTLEPROPERTY].string, sizeof(TM_char->charfunctable[CHAR_BATTLEPROPERTY].string), TM_char->sur_charfunctable[CHAR_BATTLEPROPERTY].string);
			}
		}
	}
	return TM_Ret;
}
#endif
//////////////////////////////////////////////////////////////////////////////
//获取数据的接口
int NPC_Lua_Char_GetData(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Flg = (int)lua_tointeger(_NLL, 2);

	if(TM_Flg >= 0)
	{
		if(TM_Flg < LUA_DATALINE1)
		{
			TM_Flg -= LUA_DATALINE0;
			if(TM_Flg == CHAR_WHICHTYPE)
			{
				LRetInt(_NLL, 0);
			}
			int TM_RetInt = CHAR_getInt(TM_Index, TM_Flg);
			LRetInt(_NLL, TM_RetInt);
		}else if(TM_Flg < LUA_DATALINE2)
		{
			TM_Flg -= LUA_DATALINE1;
			char *TM_RetPoint = CHAR_getChar(TM_Index, TM_Flg);
			LRetMsg(_NLL, TM_RetPoint);
		}else if(TM_Flg < LUA_DATALINE3)
		{
			TM_Flg -= LUA_DATALINE2;
			int TM_RetWorkInt = CHAR_getWorkInt(TM_Index, TM_Flg);
			LRetInt(_NLL, TM_RetWorkInt);
		}else if(TM_Flg < LUA_DATALINE4)
		{
			TM_Flg -= LUA_DATALINE3;
			char *TM_RetWorkPoint = CHAR_getWorkChar(TM_Index, TM_Flg);
			LRetMsg(_NLL, TM_RetWorkPoint);
		}else if(TM_Flg < LUA_DATALINE5)
		{
			TM_Flg -= LUA_DATALINE4;
			BOOL TM_RetBOOL = CHAR_getFlg(TM_Index, TM_Flg);
			LRetBool(_NLL, TM_RetBOOL);
		}else
		{
			LRetErrNull(_NLL, "传入的标志是错误的。");
		}
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}

int NPC_Lua_Char_IsEventEnd(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_EventNo = (int)lua_tointeger(_NLL, 2);
	
	BOOL TM_Ret = NPC_EventCheckFlg(TM_Index, TM_EventNo);

	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_Char_IsEventNow(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_EventNo = (int)lua_tointeger(_NLL, 2);
	
	BOOL TM_Ret = NPC_NowEventCheckFlg(TM_Index, TM_EventNo);

	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_Char_FindItemId(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_ItemId = (int)lua_tointeger(_NLL, 2);
	int TM_ItemIndex = -1;
	int i = 0;
	int j = 0;
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	for( i = 0 ; i<CHAR_MAXITEMHAVE ; i++ )
	{
		TM_ItemIndex = CHAR_getItemIndex(TM_Index, i);
		if( ITEM_CHECKINDEX(TM_ItemIndex) == FALSE )continue;
		if( ITEM_getInt(TM_ItemIndex, ITEM_ID) == TM_ItemId) {
			//LRetInt(_NLL, i);
			j++;
		}
	}
	LRetInt(_NLL, j);
	//LRetErrInt(_NLL, -2, "找不到指定道具。");
}

int NPC_Lua_Char_VipPoint(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Point = (int)lua_tointeger(_NLL, 2);
	int TM_Flag = (int)lua_tointeger(_NLL, 3);
	char *TM_Cdkey;
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	
	TM_Cdkey = CHAR_getChar(TM_Index,CHAR_CDKEY);

	int TM_MyPoint = sasql_vippoint(TM_Cdkey,TM_Point,TM_Flag);
	
	if(TM_MyPoint<0){
		LRetErrInt(_NLL, -2, "数据出错");
	}
	LRetInt(_NLL, TM_MyPoint);
}

int NPC_Lua_Char_FindPetEnemyId(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_PetId = (int)lua_tointeger(_NLL, 2);
	int TM_PetIndex = -1;
	int i = 0;

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	for( i = 0 ; i<CHAR_MAXPETHAVE ; i++ )
	{
		TM_PetIndex = CHAR_getCharPet( TM_Index, i);
		if( CHAR_CHECKINDEX(TM_PetIndex) == FALSE )continue;
		if( CHAR_getInt(TM_PetIndex, CHAR_PETID) == TM_PetId) {
			LRetInt(_NLL, i);
		}
	}
	LRetErrInt(_NLL, -2, "找不到指定宠物。");
}

int NPC_Lua_Char_GetItemId(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Pos = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	if(TM_Pos >= 0 && TM_Pos < CHAR_MAXITEMHAVE)
	{
		int TM_ItemIndex = CHAR_getItemIndex(TM_Index, TM_Pos);
		
		if( ITEM_CHECKINDEX(TM_ItemIndex) == FALSE ) LRetErrInt(_NLL, -2, "该道具栏没有道具。");
		LRetInt(_NLL, ITEM_getInt(TM_ItemIndex, ITEM_ID));
	}
	LRetErrInt(_NLL, -3, "传入的位置超出道具栏范围。");
}

int NPC_Lua_Char_GetItemIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Pos = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	if(TM_Pos >= 0 && TM_Pos < CHAR_MAXITEMHAVE)
	{
		int TM_ItemIndex = CHAR_getItemIndex(TM_Index, TM_Pos);
		
		if( ITEM_CHECKINDEX(TM_ItemIndex) == FALSE ) LRetErrInt(_NLL, -2, "该道具栏没有道具。");
		LRetInt(_NLL, TM_ItemIndex);
	}
	LRetErrInt(_NLL, -3, "传入的位置超出道具栏范围。");
}

int NPC_Lua_Char_GetPetEnemyId(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Pos = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	if(TM_Pos >= 0 && TM_Pos < CHAR_MAXPETHAVE)
	{
		int TM_PetIndex = CHAR_getCharPet(TM_Index, TM_Pos);
		
		if( CHAR_CHECKINDEX(TM_PetIndex) == FALSE ) LRetErrInt(_NLL, -2, "该宠物栏没有宠物。");
		LRetInt(_NLL, CHAR_getInt(TM_PetIndex, CHAR_PETID));
	}
	LRetErrInt(_NLL, -3, "传入的位置超出宠物栏范围。");
}

int NPC_Lua_Char_GetPetIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Pos = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	if(TM_Pos >= 0 && TM_Pos < CHAR_MAXPETHAVE)
	{
		int TM_PetIndex = CHAR_getCharPet(TM_Index, TM_Pos);
		
		if( CHAR_CHECKINDEX(TM_PetIndex) == FALSE ) LRetErrInt(_NLL, -2, "该宠物栏没有宠物。");
		LRetInt(_NLL, TM_PetIndex);
	}
	LRetErrInt(_NLL, -3, "传入的位置超出宠物栏范围。");
}

int NPC_Lua_Char_GetTeamIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Pos = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	if(TM_Pos >= 1 && TM_Pos < CHAR_PARTYMAX)
	{
		int TM_ParIndex = CHAR_getWorkInt( TM_Index, TM_Pos + CHAR_WORKPARTYINDEX1);

		if( CHAR_CHECKINDEX(TM_ParIndex) == FALSE ) LRetErrInt(_NLL, -2, "该位置没有队员。");
		LRetInt(_NLL, TM_ParIndex);
	}
	LRetErrInt(_NLL, -3, "传入的位置超出队员位置范围。");
}

int NPC_Lua_Char_HealAll(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	NPC_HealerAllHeal(TM_Index);
	LRetInt(_NLL, 1);
}


int NPC_Lua_Char_GetPetSkillId(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_SkillIndex = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	if( !CHAR_CHECKPETSKILLINDEX( TM_SkillIndex)){
	 	LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	
	int TM_SkillId=CHAR_getPetSkill( TM_Index, TM_SkillIndex);
	LRetInt(_NLL, TM_SkillId);
}

int NPC_Lua_Char_GetPetSkillName(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int skillarray = PETSKILL_getPetskillArray( TM_Index);
	if( PETSKILL_CHECKINDEX( skillarray) == FALSE ){
		LRetErrNull(_NLL, "传入的标志是错误的。");
	}
	LRetMsg(_NLL, PETSKILL_getChar( skillarray, PETSKILL_NAME));
}

int NPC_Lua_Char_GetPetSkillMsg(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int skillarray = PETSKILL_getPetskillArray( TM_Index);
	if( PETSKILL_CHECKINDEX( skillarray) == FALSE ){
		LRetErrNull(_NLL, "传入的标志是错误的。");
	}
	LRetMsg(_NLL, PETSKILL_getChar( skillarray, PETSKILL_COMMENT));
}

int NPC_Lua_Char_SetPetSkill(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Slod = (int)lua_tointeger(_NLL, 2);
	int TM_SkillId = (int)lua_tointeger(_NLL, 3);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	if( !CHAR_CHECKPETSKILLINDEX( TM_Slod)){
	 	LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	int skillarray = PETSKILL_getPetskillArray( TM_SkillId);
	if( PETSKILL_CHECKINDEX( skillarray) == FALSE ){
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	CHAR_setPetSkill( TM_Index, TM_Slod, TM_SkillId);
	LRetInt(_NLL, 1);
}
#endif //#ifdef _JZ_NEWSCRIPT_LUA
