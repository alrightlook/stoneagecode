#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <net/if.h>
#include "util.h"
#include "version.h"

char massage[256];
int servertime=646404;
static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

char *encode_str( const char *src, char *des )
{
    char *tmp = des;
    int len, i, j;
    if( src == NULL || des == NULL )
        return NULL;

    if( src == des ) return NULL;

    len = strlen( src );
    for( i=0; i<len/3; i++ ){
        /* ��һ���ֽڵ�ǰ6λ, ����2λ */
        *des = base64_chars[ (*src & 0xFC) >> 2 ];
        /* ��һ���ֽڵĺ�2λ, ����4λ, �ڶ����ֽڵ�ǰ4λ, ����4λ */
        *++des = base64_chars[ ((*src & 0x03) << 4) | ((*++src & 0xF0) >> 4) ];
        /* �ڶ����ֽڵĺ�4λ, ����2λ���������ֽڵ�ǰ2λ������6λ */
        *++des = base64_chars[ ((*src & 0x0F) << 2) | ((*++src & 0xC0) >> 6) ];
        /* �������ֽڵĺ�6λ */
        *++des   = base64_chars[ *src & 0x3F ];
        des++;
        src++;
    }

    j = len % 3;
    if( j == 1 ){
        *des = base64_chars[ (*src & 0xFC) >> 2 ];
        *++des = base64_chars[ (*src & 0x03) << 4 ];
    }
    if( j == 2 ){
        *des = base64_chars[ (*src & 0xFC) >> 2 ];
        *++des = base64_chars[ ((*src & 0x03) << 4) | ((*++src & 0xF0) >> 4) ];
        *++des = base64_chars[ (*src & 0x0F) << 2 ];
    }

    if( j > 0 ) for(; j<3; j++ ) *++des = '=';

    *++des = '\0';

    return tmp;
}

char *decode_str( const char *src, char *des )
{
    char *tmp = des;
    char srcbuf[4], *p1;
    const char *p;

    int len, i, j;
    if( src == NULL || des == NULL )
        return NULL;

    if( src == des ) return NULL;

    len = strlen( src );
    for( i=0; i<len/4; i++ ){
        /* ÿ�δ�src��ȡ4���ֽڣ���������Ӧ�ֽ���base64_chars
         * ��ƫ�������õ����ֽڵ�ASCIIֵ */
        p = base64_chars;
        bzero( srcbuf, sizeof(srcbuf) );
        for( j=0; j<4; ){
            if( *src == *p ){
                srcbuf[j] = ( *p == '=' ? '\0' : (p - base64_chars));
                src++;
                j++;
                p = base64_chars;
            }
            else p++;
        }

        p1 = srcbuf;
        /* ȡ��1���ֽڵĺ�6λ������2λ�� ȡ��2���ֽڵĵ�3-4λ������4λ*/
        *des = ((*p1 & 0x3F) << 2)  | ((*++p1 & 0x30) >> 4);
        /* ȡ��2���ֽڵĺ�4λ������4λ�� ȡ��3���ֽڵĵ�3-6λ������2λ*/
        *++des = ((*p1 & 0x0F) << 4)  | ((*++p1 & 0x3C) >> 2);
        /* ȡ��3���ֽڵĺ�2λ������6λ��ȡ��4���ֽڵĺ�6λ������λ*/
        *++des = ((*p1 & 0x03) << 6)  | (*++p1 & 0x3F);
        des++;
    }

    *des='\0';

    return tmp;
}


void readpasswd(char *name, char *passwd)
{
	char	line[256];
	FILE* fp = fopen("./pass.txt", "r");
	if (fp == NULL)
	{
		return;
	}

	while(1){		
		line[0]='\0';	
		if (fgets(line, sizeof(line), fp) == NULL)	break;
		chop(line);
		getStringFromIndexWithDelim(line, ":", 1, name, 32);
		getStringFromIndexWithDelim(line, ":", 2, passwd, 32);
	}

}

int attestation( void )
{
	char name[32]="";
	char passwd[32]="";
	readpasswd(name, passwd);
	if(strlen(name)==0){
		printf("�������û�����");
		scanf("%32s",name); 
	}
	if(strlen(passwd)==0){
		printf("���������룺");
		scanf("%32s",passwd); 
	}
	int rnd=0, nowTime=0, id=0;
	int svfd = connectHost( "regserver.17csa.com", 5600);
	if(svfd == -1){
		return 0;
	}
	
	fd_set rfds, wfds , efds;  
 	struct timeval tmv;
	FD_ZERO( &rfds );
	FD_ZERO( &wfds );
	FD_ZERO( &efds );
	FD_SET( svfd , &rfds );
	FD_SET( svfd , &wfds );
	FD_SET( svfd , &efds );
	tmv.tv_sec = tmv.tv_usec = 0;
	int ret = select( svfd + 1 , &rfds,&wfds,&efds,&tmv );
	if( ret > 0 && svfd > 0) {
		if( FD_ISSET( svfd , &wfds ) ){
		char mess[255];
		time_t timep;
		struct tm *p;
		time(&timep);
		p=localtime(&timep);
		timep = mktime(p);
		servertime = timep;
		char des1[256];
		memset( des1, 0, sizeof( des1 ) );
		sprintf(mess, "%s;%s;%s;%d", name, passwd,_17CSA_VERSION,servertime);
		encode_str(mess, des1);
		send(svfd,des1,strlen(des1)+1,0);
		}
	}
	
	char buf[256];
	char des2[256];
	memset( buf, 0, sizeof( buf ) );
	memset( des2, 0, sizeof( des2 ) );
	ret = read( svfd, buf, sizeof( buf ) );
	decode_str(buf, des2);
	if(getStringFromIndexWithDelim(des2,";", 1, buf, sizeof(buf)) == FALSE) return 0;
	if( ret > 0 ) {
		if(strcmp(buf,"YES") == 0){
			if(getStringFromIndexWithDelim(des2,";", 2, buf, sizeof(buf)) == FALSE) return 0;
			if(servertime != atoi(buf)) return 0;
			close(svfd);
			return 1;
		}
	}
	return 0;
}
