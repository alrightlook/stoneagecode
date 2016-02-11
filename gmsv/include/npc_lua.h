#ifndef __NPC_LUA__
#define __NPC_LUA__

#include "npc_lua_interface.h"
#include "../lua/lua.h"
#include "../lua/lauxlib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define LUA_DATALINE0 0
#define LUA_DATALINE1 2000
#define LUA_DATALINE2 4000
#define LUA_DATALINE3 6000
#define LUA_DATALINE4 8000
#define LUA_DATALINE5 10000

typedef struct __CREATEENEMY
{
	int EnemyId;
	int BaseLevel;
	int SkillType;
}CREATEENEMY,*PCREATEENEMY;

typedef struct __SCRIPTREGLIB
{
	const char *LibName;				//��������
	luaL_reg *FuncRegList;			//��Ҫע��ĺ����⺯���б�
}SCRIPTREGLIB,*PSCRIPTREGLIB;

typedef struct __SCRIPTREGCLASS
{
	const char *ClassName;			//������
	PSCRIPTREGLIB NewLib;				//����������ʵ���� SCRIPTREGLIB
	luaL_reg *FuncRegList;			//��Ҫע����ຯ���б�
}SCRIPTREGCLASS,*PSCRIPTREGCLASS;

typedef struct __SCRIPTREGARRAY
{
	const char *ArrayName;			//������
	const char **SetMetaTable;	//��������������Ӧ����
	const char **SetFuncName;		//��������������Ӧ�����ĺ�������
	PSCRIPTREGLIB FuncList;			//������Ӧ��Ӧ SetMetaTable �Ĵ��������б�
}SCRIPTREGARRAY,*PSCRIPTREGARRAY;

typedef struct __ARRAY_NTINT
{
	size_t Size;
	int Num[1];
}ARRAY_NTINT,*PARRAY_NTINT;

//����NPC-����NPCΨһ����
int NPC_Lua_Create(const char *_DoFile, const char *_InitFuncName, char *_seek, BOOL _IsFly);

//ɾ��NPC ����NPCΨһ����
int NPC_Lua_Del(int _index);

//����ս��
int NPC_Lua_CreateVsEnemy(lua_State *_NLL, int _CharaIndex, int _NpcIndex, const char *_DoFunc, PCREATEENEMY _CreateEnemy, int _ARLen, int _Flg);

const char *NPC_Lua_SetErrorStr(const char *ErrStr);

//ע����չ���ýӿ�
int NPC_Lua_RegCallEx(lua_State * _NLL, luaL_Reg *_RegList);
//ע����չ������ӿ�
int NPC_Lua_RegLibEx(lua_State * _NLL, PSCRIPTREGLIB _RegLib);
//ע����չ��ӿ�
int NPC_Lua_RegClassEx(lua_State * _NLL, PSCRIPTREGCLASS _RegClass);
//ע����չ����ӿ�
int NPC_Lua_RegArrayEx(lua_State * _NLL, PSCRIPTREGARRAY _RegArray);

//��չ���ͽӿں���
//NTInt
int NPC_Lua_NTInt_New(lua_State *_NLL);
int NPC_Lua_NTInt_Get(lua_State *_NLL);
int NPC_Lua_NTInt_Set(lua_State *_NLL);
int NPC_Lua_NTInt_Len(lua_State *_NLL);

//�ӿں���
////////////////////////////////////////////////NL/////////////////////////////////////////////////
int NPC_Lua_NL_GetErrorStr(lua_State *_NLL);						//���ش�����Ϣ
int NPC_Lua_NL_CreateNpc(lua_State *_NLL);							//����NPC
int NPC_Lua_NL_DelNpc(lua_State *_NLL);									//ɾ��NPC

////////////////////////////////////////////////NLG////////////////////////////////////////////////
//���ܽӿ�
int NPC_Lua_NLG_CheckInFront(lua_State *_NLL);					//���ڼ��ĳ�������Ƿ���ĳ��������ǰ
int NPC_Lua_NLG_CheckObj(lua_State *_NLL);							//���ĳ����ͼλ���Ƿ��ж������
int NPC_Lua_NLG_CharLook(lua_State *_NLL);							//�������﷽��(���͸��·��)
int NPC_Lua_NLG_CreateBattle(lua_State *_NLL);					//����ս��

int NPC_Lua_NLG_CreateBattlePvP(lua_State *_NLL);

int NPC_Lua_NLG_SearchWatchBattleRandIndex(lua_State *_NLL);

int NPC_Lua_NLG_DelPet(lua_State *_NLL);								//ɾ��ָ����ҳ�����һ����������
int NPC_Lua_NLG_DelHaveIndexPet(lua_State *_NLL);
int NPC_Lua_NLG_DelItem(lua_State *_NLL);								//ɾ��ָ����ҵ�����һ����������
int NPC_Lua_NLG_DischargeParty(lua_State *_NLL);				//��ɢ �Ŷ�

int NPC_Lua_NLG_GivePet(lua_State *_NLL);								//��ָ�����һ����������
int NPC_Lua_NLG_GiveItem(lua_State *_NLL);							//��ָ�����һ����������
int NPC_Lua_NLG_GiveOneItem(lua_State *_NLL);							//��ָ�����һ������
int NPC_Lua_NLG_GiveRandItem(lua_State *_NLL);
int NPC_Lua_Char_GetOnLinePlayer(lua_State *_NLL);			//��ȡ��ǰ��������

int NPC_Lua_NLG_ShowWindowTalked(lua_State *_NLL);			//��ָ����ҿͻ�����ʾָ�����ݡ����͵ĶԻ���
int NPC_Lua_NLG_SetAction(lua_State *_NLL);							//���ö���Ķ���

int NPC_Lua_NLG_TalkToCli(lua_State *_NLL);							//��ͨ˵�������Զ�ȫ��������˵
int NPC_Lua_NLG_TalkToFloor(lua_State *_NLL);						//����ĳһָ����ͼ�ڵ����˵��

int NPC_Lua_NLG_UpChar(lua_State *_NLL);								//��NPC��Χ����ҷ���NPC�����ݸ��·��
int NPC_Lua_NLG_UpStateBySecond(lua_State *_NLL);				//��ָ����ҷ��͵�2��״̬���·��
int NPC_Lua_NLG_UpStateByThird(lua_State *_NLL);				//��ָ����ҷ��͵�3��״̬���·��
int NPC_Lua_NLG_Update_Party(lua_State *_NLL);					//��������ڵ��Ŷӳ�Ա������ҵ�״̬���·��

int NPC_Lua_NLG_Warp(lua_State *_NLL);									//����һ������
int NPC_Lua_NLG_WalkMove(lua_State *_NLL);							//�ƶ�һ������
int NPC_Lua_NLG_WatchEntry(lua_State *_NLL);						//��ս

int NPC_Lua_NLG_GetMaxPlayNum(lua_State *_NLL);					//��ȡ���������������
int NPC_Lua_NLG_CheckPlayIndex(lua_State *_NLL);				//�����������Ƿ�����

int NPC_Lua_NLG_Save(lua_State *_NLL);

///////////////////////////////////////////////Char////////////////////////////////////////////////
//�������ݵĽӿ�
int NPC_Lua_Char_ClrEvtEnd(lua_State *_NLL);						//����������������־
int NPC_Lua_Char_ClrEvtNow(lua_State *_NLL);						//������������������־
int NPC_Lua_Char_ClrEvt(lua_State *_NLL);								//������������־

int NPC_Lua_Char_SetEvtEnd(lua_State *_NLL);						//���������־Ϊ����״̬
int NPC_Lua_Char_SetEvtNow(lua_State *_NLL);						//���������־Ϊ������״̬
int NPC_Lua_Char_SetData(lua_State *_NLL);							//����Char����

//�¼����õĽӿ�
int NPC_Lua_Char_SetWalkPreEvent(lua_State *_NLL);			//����WalkPre�¼���Ӧ
int NPC_Lua_Char_SetWalkPostEvent(lua_State *_NLL);			//����WalkPost�¼���Ӧ
int NPC_Lua_Char_SetPreOverEvent(lua_State *_NLL);			//����PreOver�¼���Ӧ
int NPC_Lua_Char_SetPostOverEvent(lua_State *_NLL);			//����PostOver�¼���Ӧ
int NPC_Lua_Char_SetWatchEvent(lua_State *_NLL);				//����Watch�¼���Ӧ
int NPC_Lua_Char_SetLoopEvent(lua_State *_NLL);					//����Loop�¼���Ӧ
int NPC_Lua_Char_SetTalkedEvent(lua_State *_NLL);				//����Talked�¼���Ӧ
int NPC_Lua_Char_SetOFFEvent(lua_State *_NLL);					//����OFF�¼���Ӧ
int NPC_Lua_Char_SetLookedEvent(lua_State *_NLL);				//����Looked�¼���Ӧ
int NPC_Lua_Char_SetItemPutEvent(lua_State *_NLL);			//����ItemPut�¼���Ӧ
int NPC_Lua_Char_SetWindowTalkedEvent(lua_State *_NLL);	//����WindowTalked�¼���Ӧ
#ifdef _USER_CHARLOOPS
int NPC_Lua_Char_SetCharLoopsEvent(lua_State *_NLL);		//����CharLoops�¼���Ӧ
int NPC_Lua_Char_SetBattleProPertyEvent(lua_State *_NLL);//����BattleProPerty�¼���Ӧ
#endif

//��ȡ���ݵĽӿ�
int NPC_Lua_Char_IsEventEnd(lua_State *_NLL);						//����Ƿ�����������־
int NPC_Lua_Char_IsEventNow(lua_State *_NLL);						//����Ƿ��������������־

int NPC_Lua_Char_FindItemId(lua_State *_NLL);						//����ָ�����������Ƿ�ӵ��ĳ����ID
int NPC_Lua_Char_FindPetEnemyId(lua_State *_NLL);				//����ָ�����������Ƿ�ӵ�г���Enemy�ļ��Ķ�ӦID

int NPC_Lua_Char_GetData(lua_State *_NLL);							//��ȡChar����
int NPC_Lua_Char_GetItemId(lua_State *_NLL);						//��ȡָ����������ָ��λ�õĵ���ID
int NPC_Lua_Char_GetPetEnemyId(lua_State *_NLL);				//��ȡָ����������ָ��λ�õĳ���Enemy�ļ��Ķ�ӦID
int NPC_Lua_Char_GetItemIndex(lua_State *_NLL);					//��ȡָ����������ָ��λ�õĵ�������
int NPC_Lua_Char_GetPetIndex(lua_State *_NLL);					//��ȡָ����������ָ��λ�õĳ����������
int NPC_Lua_Char_GetTeamIndex(lua_State *_NLL);					//��ȡ��Ա�Ķ�������
int NPC_Lua_Char_VipPoint(lua_State *_NLL);
int NPC_Lua_Char_HealAll(lua_State *_NLL);
int NPC_Lua_Char_GetPetSkillId(lua_State *_NLL);
int NPC_Lua_Char_GetPetSkillName(lua_State *_NLL);
int NPC_Lua_Char_GetPetSkillMsg(lua_State *_NLL);
int NPC_Lua_Char_SetPetSkill(lua_State *_NLL);
///////////////////////////////////////////////Item////////////////////////////////////////////////
//��ȡ���ݵĽӿ�
int NPC_Lua_Item_GetData(lua_State *_NLL);							//��ȡ��������

//�������ݵĽӿ�
int NPC_Lua_Item_SetData(lua_State *_NLL);							//��ȡ��������

//�¼����õĽӿ�
int NPC_Lua_Item_SetPreOverEvent(lua_State *_NLL);			//����PreOver�¼���Ӧ
int NPC_Lua_Item_SetPostOverEvent(lua_State *_NLL);			//����PostOver�¼���Ӧ
int NPC_Lua_Item_SetWatchEvent(lua_State *_NLL);				//����Watch�¼���Ӧ
int NPC_Lua_Item_SetUseEvent(lua_State *_NLL);					//����Use�¼���Ӧ
int NPC_Lua_Item_SetAttachEvent(lua_State *_NLL);				//����Attach�¼���Ӧ
int NPC_Lua_Item_SetDetachEvent(lua_State *_NLL);				//����Detach�¼���Ӧ
int NPC_Lua_Item_SetDropEvent(lua_State *_NLL);					//����Drop�¼���Ӧ
int NPC_Lua_Item_SetPickUPEvent(lua_State *_NLL);				//����PickUP�¼���Ӧ
#ifdef _Item_ReLifeAct
int NPC_Lua_Item_SetDieReLifeEvent(lua_State *_NLL);		//����DieReLife�¼���Ӧ
#endif

///////////////////////////////////////////////Obj////////////////////////////////////////////////
//��ȡ���ݵĽӿ�
int NPC_Lua_Obj_GetType(lua_State *_NLL);								//��ȡOBJ������
/*
int NPC_Lua_Obj_GetName(lua_State *_NLL);								//��ȡOBJ������
int NPC_Lua_Obj_GetDIR(lua_State *_NLL);								//��ȡOBJ�ķ���
int NPC_Lua_Obj_GetImageNum(lua_State *_NLL);						//��ȡOBJ�������
*/
int NPC_Lua_Obj_GetCharType(lua_State *_NLL);						//��ȡOBJ��Ӧ��Char���ݽṹ������
int NPC_Lua_Obj_GetCharIndex(lua_State *_NLL);					//��ȡOBJ��Ӧ��Char���ݽṹ������
int NPC_Lua_Obj_GetX(lua_State *_NLL);									//��ȡOBJ��X����
int NPC_Lua_Obj_GetY(lua_State *_NLL);									//��ȡOBJ��Y����
int NPC_Lua_Obj_GetFloor(lua_State *_NLL);							//��ȡOBJ���ڵĵ�ͼ���
//int NPC_Lua_Obj_GetDelTime(lua_State *_NLL);						//��ȡOBJ��ɾ��ʱ��

int NPC_Lua_Obj_SetType(lua_State *_NLL);								//��ȡOBJ������
/*
int NPC_Lua_Obj_SetName(lua_State *_NLL);								//��ȡOBJ������
int NPC_Lua_Obj_SetDIR(lua_State *_NLL);								//��ȡOBJ�ķ���
int NPC_Lua_Obj_SetImageNum(lua_State *_NLL);						//��ȡOBJ�������
*/
int NPC_Lua_Obj_SetCharType(lua_State *_NLL);						//��ȡOBJ��Ӧ��Char���ݽṹ������
int NPC_Lua_Obj_SetX(lua_State *_NLL);									//��ȡOBJ��X����
int NPC_Lua_Obj_SetY(lua_State *_NLL);									//��ȡOBJ��Y����
int NPC_Lua_Obj_SetFloor(lua_State *_NLL);							//��ȡOBJ���ڵĵ�ͼ���
//int NPC_Lua_Obj_SetDelTime(lua_State *_NLL);						//��ȡOBJ��ɾ��ʱ��

//////////////////////////////////////////////Battle///////////////////////////////////////////////
//��ȡ���ݵĽӿ�
int NPC_Lua_Battle_GetPlayIndex(lua_State *_NLL);				//��ȡս���е��������
//�������ݵĽӿ�
int NPC_Lua_Battle_SetNORisk(lua_State *_NLL);					//�����Ƿ����޷���ģʽ
int NPC_Lua_Battle_SetMod(lua_State *_NLL);							//����ս��ģʽ��־
int NPC_Lua_Battle_SetType(lua_State *_NLL);						//����ս������
//�¼����õĽӿ�
int NPC_Lua_Battle_SetWinEvent(lua_State *_NLL);				//����Win�¼���Ӧ

///////////////////////////////////////////////Other///////////////////////////////////////////////
int NPC_Lua_GetFuncPoint(lua_State *_NLL);							//��ȡ����ָ��

//////////////////////////////////////////////////////////////////////
#define LRet(r) \
{ \
	NPC_Lua_SetErrorStr(NULL); \
	return r; \
}

#define LRetErr(c,r) \
{ \
	NPC_Lua_SetErrorStr(c); \
	return r; \
}

//////////////////////////////////////////////////////////////////////
//����һ��nil
#define LRetNull(L) \
{ \
	lua_pushnil(L); \
	LRet(1); \
}

//����һ��BOOL��LUA����
#define LRetBool(L, b) \
{ \
	lua_pushboolean(L, b); \
	LRet(1); \
}

//����һ��int��LUA����
#define LRetInt(L, i) \
{ \
	lua_pushinteger(L, i); \
	LRet(1); \
}

#define LRetMsg(L, c) \
{ \
	lua_pushstring(L, c); \
	LRet(1); \
}
////////////////////////////////////////////////////////////////////
//����һ������״̬��һ������Ϣ
#define LRetErrInt(L, i, c) \
{ \
	lua_pushinteger(L, i); \
	LRetErr(c,1); \
}

//����һ���ַ�����LUA����
#define LRetErrNull(L, c) \
{ \
	lua_pushnil(L); \
	LRetErr(c,1); \
}

//����һ���ַ�����LUA����
#define LRetErrMsg(L, c) \
{ \
	lua_pushstring(L, c); \
	LRetErr(c,1); \
}

//���ڼ������Ƿ��㹻
#define CheckEx(L, n) \
{	\
	if(lua_gettop(L) != n) \
	{ \
		luaL_argerror(L, 1, "������������"); \
	} \
}

#define CheckEx2(L, t, n) \
{	\
	if(lua_gettop(L) < (t) || lua_gettop(L) > n) \
	{ \
		luaL_argerror(L, 1, "������������"); \
	} \
}

#define CheckIndexNull(L, n) \
{ \
	if(lua_isnil((L), (n))) \
	{ \
		luaL_argerror((L), 1, "������������Ϊnil"); \
	} \
}

#define CheckBattleIndexNull(L, n) \
{ \
	if(lua_isnil((L), (n))) \
	{ \
		luaL_argerror((L), 1, "ս����������Ϊnil"); \
	} \
}

#define CheckItemIndexNull(L, n) \
{ \
	if(lua_isnil((L), (n))) \
	{ \
		luaL_argerror((L), 1, "������������Ϊnil"); \
	} \
}

#define CheckObjIndexNull(L, n) \
{ \
	if(lua_isnil((L), (n))) \
	{ \
		luaL_argerror((L), 1, "�����������Ϊnil"); \
	} \
}
#endif //#ifndef __NPC_LUA__