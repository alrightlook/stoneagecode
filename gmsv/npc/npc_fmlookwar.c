#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "npc_fmlookwar.h"
#include "lssproto_serv.h"
#include "saacproto_cli.h"
#include "readmap.h"
#include "battle.h"
#include "log.h"
#include "enemy.h"
#include "handletime.h"
#include "npc_eventaction.h"

#ifdef	_FM_NPC_LOOK_WAR
//�صش����ø�н�ڽ�����"����ʦ"
enum {
	WINDOW_START=1,
	WINDOW_SELECT,
};
enum {
	NPC_WORK_CURRENTTIME = CHAR_NPCWORKINT1,
/*
	NPC_WORK_ROUTETOY = CHAR_NPCWORKINT2,
	NPC_WORK_ROUTEPOINT = CHAR_NPCWORKINT3,
	NPC_WORK_ROUNDTRIP = CHAR_NPCWORKINT4,
	NPC_WORK_MODE = CHAR_NPCWORKINT5,
	NPC_WORK_CURRENTROUTE = CHAR_NPCWORKINT6, 
	NPC_WORK_ROUTEMAX = CHAR_NPCWORKINT7,
	NPC_WORK_WAITTIME = CHAR_NPCWORKINT8,
	NPC_WORK_CURRENTTIME = CHAR_NPCWORKINT9,
	NPC_WORK_SEFLG = CHAR_NPCWORKINT10,
*/
};


static void NPC_FmLookWarMan_selectWindow( int meindex, int toindex, int num,int select);
BOOL FmLookWarMan_Run( int meindex, int toindex, int select);

BOOL NPC_FmLookWarManInit( int meindex )
{
	CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);
	return TRUE;
}

void NPC_FmLookWarManTalked( int meindex, int talkerindex, char *msg, int color )
{
	if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER )
    		return;
	if(NPC_Util_isFaceToFace( meindex ,talkerindex , 2) == FALSE) {
		if(NPC_Util_isFaceToChara( talkerindex, meindex, 1) == FALSE)
			return;
	}
// Syu ADD ��Ŵ���ʦ�ж����״̬
	if( CHAR_getWorkInt ( talkerindex , CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE)
	{
		CHAR_talkToCli( talkerindex, meindex, "��������ӣ�",CHAR_COLORYELLOW);
		return;
	}
	CHAR_setWorkInt( talkerindex, CHAR_WORKSHOPRELEVANT, 0);
	NPC_FmLookWarMan_selectWindow( meindex, talkerindex, WINDOW_START, 0);
}

void NPC_FmLookWarManWindowTalked( int meindex, int talkerindex, int seqno,
								int select, char *data)
{
	if( select == WINDOW_BUTTONTYPE_CANCEL || select == WINDOW_BUTTONTYPE_NO)
		return;
	switch( seqno)	{
	case NPC_FMLOOKWARMAN_START:
		break;
	case NPC_FMLOOKWARMAN_SELECT:
		if( select == WINDOW_BUTTONTYPE_CANCEL )
			return;
		NPC_FmLookWarMan_selectWindow( meindex, talkerindex, WINDOW_SELECT, atoi( data));
		break;
	}

}

static void NPC_FmLookWarMan_selectWindow( int meindex, int toindex, int num,int select)
{
	char token[256];

	int buttontype = 0, windowtype = 0, windowno = 0;
	int fd = getfdFromCharaIndex( toindex);

	windowtype = WINDOW_MESSAGETYPE_MESSAGE;

	if(NPC_Util_isFaceToFace( meindex ,toindex , 2) == FALSE) {
		if(NPC_Util_isFaceToChara( toindex, meindex, 1) == FALSE)
			return;
	}

	memset( token, 0, sizeof( token));

	switch( num)	{
	case WINDOW_START:
		sprintf(token,"2\n����Զ�̹�սԱ���ṩ�����侳�Ĺ�ս����\n"
									"������ѡ������Ҫ�ۿ���ս����\n"
									"                ��ķ����ׯ԰��ս\n"
									"                ������˿ׯ԰��ս\n"
									"                �ӼӴ��ׯ԰��ս\n"
									"                ��³����ׯ԰��ս\n"
									"                ����У��ֳ�ֱ��\n");
		CHAR_setWorkInt( toindex, CHAR_WORKSHOPRELEVANT, WINDOW_START);
		buttontype = WINDOW_BUTTONTYPE_CANCEL;
		windowno = NPC_FMLOOKWARMAN_SELECT;
		windowtype = WINDOW_MESSAGETYPE_SELECT;
		break;
	case WINDOW_SELECT:
		FmLookWarMan_Run( meindex, toindex, select);
		return;
	default:
		break;
	}

	lssproto_WN_send( fd, windowtype, buttontype, windowno,
		CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX), token);
}


void NPC_FmLookWarManLoop( int meindex)
{

}

BOOL FmLookWarMan_Run( int meindex, int toindex, int select)
{
	if(select<1 || select>5) return FALSE;
	switch(select){
		case 1:
			if(SearchFmWarRandIndex(toindex,1042)==0){
				return TRUE;
			}else{
				CHAR_talkToCli( toindex, meindex, "���ź����õ�ͼĿǰ��ս�����Թۿ���",CHAR_COLORYELLOW);
				return FALSE;
			}
			break;
		case 2:
			if(SearchFmWarRandIndex(toindex,2032)==0){
				return TRUE;
			}else{
				CHAR_talkToCli( toindex, meindex, "���ź����õ�ͼĿǰ��ս�����Թۿ���",CHAR_COLORYELLOW);
				return FALSE;
			}
			break;
		case 3:
			if(SearchFmWarRandIndex(toindex,3032)==0){
				return TRUE;
			}else{
				CHAR_talkToCli( toindex, meindex, "���ź����õ�ͼĿǰ��ս�����Թۿ���",CHAR_COLORYELLOW);
				return FALSE;
			}
			break;
		case 4:
			if(SearchFmWarRandIndex(toindex,4032)==0){
				return TRUE;
			}else{
				CHAR_talkToCli( toindex, meindex, "���ź����õ�ͼĿǰ��ս�����Թۿ���",CHAR_COLORYELLOW);
				return FALSE;
			}
			break;
		case 5:
			if(SearchFmWarRandIndex(toindex,20000)==0){
				return TRUE;
			}else{
				CHAR_talkToCli( toindex, meindex, "���ź����õ�ͼĿǰ��ս�����Թۿ���",CHAR_COLORYELLOW);
				return FALSE;
			}
			break;
		default:
			break;
	}
	return FALSE;
}
#endif

