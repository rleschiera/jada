#include "jada.h"
#include "jada_internal.h"
#include "jada_messages.h"

static FixString        myHostName = "";
static FixString        myHostIP = "";
static FixString        myProxyOpenPassword = "";


/******************************************************************************/
/*                                                                            */
/*   T C P     S E R V I C E S                                                */
/*                                                                            */
/******************************************************************************/
DllExport int __stdcall JADA_TcpSetUseProxy(int iUseProxy)
{
    int     iRet;

    iRet = JADA_IniSetInt("proxy", "use", iUseProxy);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

DllExport int __stdcall JADA_TcpGetUseProxy(int *piUseProxy)
{
    int     iRet;
    int     iUseProxy;

    iRet = JADA_IniGetInt("proxy", "use", &iUseProxy);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    *piUseProxy = iUseProxy;

    return(0);
}

DllExport int __stdcall JADA_TcpSetProxyAddress(char *szProxyName, int iPort)
{
    int     iRet;
    ShortString  szPort = "";

    iRet = JADA_IniSetString("proxy", "host", szProxyName);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    if (iPort > 0) {
        sprintf(szPort, "%d", iPort);
    }
    iRet = JADA_IniSetString("proxy", "port", szPort);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

DllExport int __stdcall JADA_TcpSetProxyCredentials(char *szUser, char *szPassword, char *szOpenPassword)
{
    int     iRet;

    iRet = JADA_IniSetString("proxy", "user", szUser);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    iRet = JADA_IniSetString("proxy", "password", szPassword);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    strcpy(myProxyOpenPassword, szOpenPassword);

    return(0);
}

DllExport int __stdcall JADA_TcpGetProxyCredentials(char *szUser, int iUserLength, char *szPassword, int iPasswordLength)
{
    int     iRet;

    iRet = JADA_IniGetString("proxy", "user", szUser, iUserLength);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    iRet = JADA_IniGetString("proxy", "password", szPassword, iPasswordLength);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(0);
}

DllExport int __stdcall JADA_TcpCreateListenPort(char *szHostName, int iPort)
{
    int                 iRet;
    struct hostent     *n;
    struct sockaddr_in  myAddress;
    int                 iSocket;

    memset(&myAddress, 0, sizeof(struct sockaddr_in));
    myAddress.sin_family = AF_INET;
    myAddress.sin_port = htons(iPort);
    myAddress.sin_addr.s_addr = inet_addr(szHostName);
    if (myAddress.sin_addr.s_addr == INADDR_NONE) {
        n = gethostbyname(szHostName);
        CORE_ReturnValIfFail(n != NULL, -1, ;, "Errore generico");
        memcpy((char *) &myAddress.sin_addr, n->h_addr, n->h_length);
    }
    strcpy(myHostName, szHostName);
    strcpy(myHostIP, inet_ntoa(myAddress.sin_addr));

    /* Crea il listener socket */
    iSocket = socket(AF_INET, SOCK_STREAM, 0);
    CORE_ReturnValIfFail(iSocket > 0, -1, ;, "Errore generico");
    iRet = bind(iSocket, (struct sockaddr *)&myAddress, sizeof(myAddress));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
    iRet = listen(iSocket, 5);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(iSocket);
}

static int ConnectToRemotePort(char *szName, int iPort, int *piSocket)
{
    int                 iRet;
    struct hostent     *n;
    struct sockaddr_in  myAddress;
    int                 iLength;
    int                 iSocket;

    /* XXX */
    memset(&myAddress, 0, sizeof(struct sockaddr_in));
    myAddress.sin_family = AF_INET;
    myAddress.sin_port = htons(iPort);
    myAddress.sin_addr.s_addr = inet_addr(szName);
    if (myAddress.sin_addr.s_addr == INADDR_NONE) {
        n = gethostbyname(szName);
        CORE_ReturnValIfFail(n != NULL, -1, ;, "Errore generico");
        memcpy((char *) &myAddress.sin_addr, n->h_addr, n->h_length);
    }

	iSocket = socket(AF_INET, SOCK_STREAM, 0);
    CORE_ReturnValIfFail(iSocket > 0, -1, ;, "Errore generico");
    iRet = connect(iSocket, (struct sockaddr *)&myAddress, sizeof(myAddress));
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    iLength = sizeof(myAddress);  
    iRet = getsockname(iSocket, (struct sockaddr *)&myAddress, &iLength);  
    CORE_ReturnValIfFail(iRet >= 0, -1, ;, "Errore generico");
    strcpy(myHostIP, inet_ntoa(myAddress.sin_addr));

    *piSocket = iSocket;
  
    return(0);
}

//#define BUFF_SIZE 8*1024  
//static const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//
//static void encode_base_64(char* src,char* dest,int max_len)
//{
//	int n,l,i;
// 	l=strlen(src);
//	max_len=(max_len-1)/4;
//	for ( i=0;i<max_len;i++,src+=3,l-=3)
//	{
//		switch (l) {
//		case 0:
//			break;
//		case 1:
//			n=src[0] << 16;
//			*dest++=base64[(n >> 18) & 077];
//			*dest++=base64[(n >> 12) & 077];
//			*dest++='=';
//			*dest++='=';
//			break;
//		case 2:
//			n=src[0] << 16 | src[1] << 8;
//			*dest++=base64[(n >> 18) & 077];
//			*dest++=base64[(n >> 12) & 077];
//			*dest++=base64[(n >> 6) & 077];
//			*dest++='=';
//			break;
//		default:
//			n=src[0] << 16 | src[1] << 8 | src[2];
//			*dest++=base64[(n >> 18) & 077];
//			*dest++=base64[(n >> 12) & 077];
//			*dest++=base64[(n >> 6) & 077];
//			*dest++=base64[n & 077];
//		}
//		if (l<3) break;
//	}
//	*dest++=0;
//}
//
//static int read_n_bytes(int fd,char *buff, size_t size)
//{
//  int i=0,got=0;
//  for(;;)
//  {
//    i=recv(fd,&buff[got],1, 0);
//    if(i<=0)
//         return i;
//    got+=i;
//    if(got==size)
//         return got;
//  }
//}
//
//static int write_n_bytes(int fd,char *buff,size_t size)
//{
//  int i=0,wrote=0;
//  for(;;)
//  {
//    i=send(fd,&buff[wrote],size-wrote, 0);
//    if(i<=0)
//         return i;
//    wrote+=i;
//    if(wrote==size)
//         return wrote;
//  }
//}
//
//static int tunnel_to(int sock, unsigned int ip, unsigned short port, proxy_type pt,char *user,char *pass)
//{
//        int len;
//        char buff[BUFF_SIZE];
//        memset(buff,0,sizeof(buff));
//        switch(pt)
//        {
//        	case HTTP_TYPE:
//         		{
//             		sprintf(buff,"CONNECT %s:%d HTTP/1.0\r\n",
//			        inet_ntoa( * (struct in_addr *) &ip),
//			        ntohs(port));
//           			if (user[0])
//                		{
//					char src[256];
//     					char dst[512];
//					strcpy(src,user);
//					strcat(src,":");
//					strcat(src,pass);
//					encode_base_64(src,dst,512);
//					strcat(buff,"Proxy-Authorization: Basic ");
//					strcat(buff,dst);
//					strcat(buff,"\r\n\r\n");
//				}
//    				else
//					strcat(buff,"\r\n");
//			
//           			len=strlen(buff);
//
//			        if(len!=send(sock,buff,len,0))
//			                return SOCK_ERROR;
//			
//           			memset(buff,0,sizeof(buff));
//                        len=0 ;
//      			 // read header byte by byte.
//			       while(len<BUFF_SIZE)
//			       {
//			                if(1==read_n_bytes(sock,buff+len,1))
//			                        len++;
//			                else
//			                        return SOCK_ERROR;
//			                if (    len > 4     &&
//		                        	buff[len-1]=='\n'  &&
//			                        buff[len-2]=='\r'  &&
//			                        buff[len-3]=='\n'  &&
//			                        buff[len-4]=='\r'  )
//		                        break;
//			       }
//
//			       // if not ok (200) or response greather than BUFF_SIZE return BLOCKED;
//			       if (     (len==BUFF_SIZE)  ||
//			                ! (     buff[9] =='2'         &&
//			                        buff[10]=='0'        &&
//			                        buff[11]=='0'         ))
//                                  return BLOCKED;
//			       return SUCCESS;
//           		}
//            	break;
//            case SOCKS4_TYPE:
//            	{
//               		memset(buff,0,sizeof(buff));
//                 		buff[0]=4; // socks version
//  				buff[1]=1; // connect command
//				memcpy(&buff[2],&port,2); // dest port
//				memcpy(&buff[4],&ip,4); // dest host
//				len=strlen(user)+1; // username
//    				if(len>1)	
//         				strcpy(&buff[8],user);
//				if((len+8)!=write_n_bytes(sock,buff,(8+len)))
//					return SOCK_ERROR;
//
// 				if(8!=read_n_bytes(sock,buff,8))
//					return SOCK_ERROR;
//            	
//				if (buff[0]!=0||buff[1]!=90)
//					return BLOCKED;
//     				
//         			return SUCCESS;
//               	}
//                	break;
//            case SOCKS5_TYPE:
//            	{
//               		if(user)
//                 		{
//                 			buff[0]=5;   //version
//					buff[1]=2;	//nomber of methods
//					buff[2]=0;   // no auth method
//	    				buff[3]=2;  /// auth method -> username / password
//                              if(4!=write_n_bytes(sock,buff,4))
//					 	return SOCK_ERROR;
//       			}
//            		else
//                		{
//            			buff[0]=5;   //version
//					buff[1]=1;	//nomber of methods
//					buff[2]=0;   // no auth method
//                              if(3!=write_n_bytes(sock,buff,3))
//					 	return SOCK_ERROR;
//       			}
//
//				memset(buff,0,sizeof(buff));
//
//				if(2!=read_n_bytes(sock,buff,2))
//			 		return SOCK_ERROR;
//			
//      			if (buff[0]!=5||(buff[1]!=0&&buff[1]!=2))
//         			{
//        				if((buff[0]==0x05)&&(buff[1]==(char)0xFF))
//             						return BLOCKED;
//						else
//							return SOCK_ERROR;
//          			}
//          			
//          			if (buff[1]==2)
//               		{
//					// authentication
//					char in[2];
//     					char out[515]; char* cur=out;
//					int c;
//     					*cur++=1; // version
//					c=strlen(user);
//					*cur++=c;
//					strncpy(cur,user,c);
//					cur+=c;
//					c=strlen(pass);
//					*cur++=c;
//					strncpy(cur,pass,c);
//					cur+=c;
//					
//     					if((cur-out)!=write_n_bytes(sock,out,cur-out))
//					 	return SOCK_ERROR;
//     					
//          				
//					if(2!=read_n_bytes(sock,in,2))
//			 			return SOCK_ERROR;
//					if(in[0]!=1||in[1]!=0)
//       				{
//						if(in[0]!=1)
//      						return SOCK_ERROR;
//						else
//      						return BLOCKED;
//					}
//				}	
//
//     				buff[0]=5;       // version
//				buff[1]=1;       // connect
//				buff[2]=0;       // reserved
//				buff[3]=1;       // ip v4
//
//			 	memcpy(&buff[4],&ip,4); // dest host
//				memcpy(&buff[8],&port,2); // dest port
//				
//
//			      if(10!=write_n_bytes(sock,buff,10))
//					return SOCK_ERROR;
//		
//			      if(4!=read_n_bytes(sock,buff,4))
//					return SOCK_ERROR;
//
//				if (buff[0]!=5||buff[1]!=0)
//			      	return SOCK_ERROR;
//
//			  	switch (buff[3])
//			      {
//					case 1: len=4;  break;
//					case 4: len=16; break;
//					case 3: len=0;
//			  			if(1!=read_n_bytes(sock,(char*)&len,1))
//			 				return SOCK_ERROR;
//        					break;
//					default:
//						return SOCK_ERROR;
//				}
//
//     				if((len+2)!=read_n_bytes(sock,buff,(len+2)))
//					return SOCK_ERROR;
//
//				return SUCCESS;
//                	}
//                	break;	
//
//        }
//
//return SOCK_ERROR;
//}

//static int ConnectToServerViaProxy(char *szHostName, int iPort, int *piSocket)
//{
//    int         iRet; //, iBytes, iBase64Length;
//    int         iSocket;
//    // char        szOutMessage[512], *szPtr;
//    // char        szInMessage[32];
//    ShortString      szProxyName, szProxyUser;
//    // FixString   szProxyCredentials;
//    int         iProxyPort;
//    struct hostent     *n;
//    struct sockaddr_in  myAddress;
//
//
//    CORE_SilentReturnValIfFail(strlen(myProxyOpenPassword) > 0, JADA_PROXY_AUTHENTICATE, ;);
//
//    /* Leggi il settaggio del proxy */
//    iRet = JADA_IniGetString("proxy", "host", szProxyName, sizeof(szProxyName));
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura del proxy IP dal file .ini");
//    iRet = JADA_IniGetInt("proxy", "port", &iProxyPort);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura del proxy PORT dal file .ini");
//    iRet = JADA_IniGetString("proxy", "user", szProxyUser, sizeof(szProxyUser));
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
//
//    iRet = ConnectToRemotePort(szProxyName, iProxyPort, &iSocket);
//    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella connessione al proxy");
//
//
//
//    memset(&myAddress, 0, sizeof(struct sockaddr_in));
//    myAddress.sin_addr.s_addr = inet_addr(szHostName);
//    if (myAddress.sin_addr.s_addr == INADDR_NONE) {
//        n = gethostbyname(szHostName);
//        CORE_ReturnValIfFail(n != NULL, -1, ;, "Errore generico");
//        memcpy((char *) &myAddress.sin_addr, n->h_addr, n->h_length);
//    }
//
//    iRet = tunnel_to(iSocket, myAddress.sin_addr.s_addr, htons(iPort), HTTP_TYPE,"raleschu","Gnappetta96");
//
//    //sprintf(szProxyCredentials, "%s:%s", szProxyUser, myProxyOpenPassword);
//    //szPtr = JADA_Base64Encode(szProxyCredentials, strlen(szProxyCredentials), &iBase64Length);
//    //CORE_ReturnValIfFail(szPtr != NULL, -1, ;, "Errore nella connessione al proxy");
//    //memset(szProxyCredentials, 0, sizeof(szProxyCredentials));
//    //memcpy(szProxyCredentials, szPtr, iBase64Length);
//    //free(szPtr);
//
//    //sprintf(szOutMessage, "CONNECT %s:%d HTTP/1.0 \r\nHOST %s:%d\r\nProxy-Authorization:Basic %s\r\n\r\n", 
//    //        szHostName, iPort, szHostName, iPort, szProxyCredentials);
//    //sprintf(szOutMessage, "CONNECT %s:%d HTTP/1.0 \r\nHOST %s:%d\r\n\r\n", 
//    //        szHostName, iPort, szHostName, iPort, szProxyCredentials);
//    //sprintf(szOutMessage, "CONNECT %s:%d HTTP/1.0 \r\n\r\n", 
//    //        szHostName, iPort, szHostName, iPort, szProxyCredentials);
//    //iBytes = send(iSocket, szOutMessage, strlen(szOutMessage), 0);
//    //CORE_SilentReturnValIfFail(iBytes == strlen(szOutMessage), -1, ;);
//    //JADA_Sleep(250);
//
////     request = "GET {0} HTTP/1.1\r\nHost: {1}\r\nUser-Agent: Mozilla/5.0 (Windows NT 5.1; rv:5.0) Gecko/20100101 Firefox/5.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-us,en;q=0.5\r\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\nConnection: keep-alive\r\nReferer: {0}\r\nCookie: {2}\r\n\r\n";
//// Proxy-Authorization: Basic <BASE64("USER:PASS")>
//// If any of your requests get a response "407 Proxy Authentication Required" you can read the response header field Proxy-Authenticate which will tell you the authentication mode to use when authorizing. Above it's Basic (the most common one) but there are others like Digest and NTLM. You can read up on the other two here
//    //iBytes = recv(iSocket, szInMessage, sizeof(szInMessage), 0);
//    //CORE_SilentReturnValIfFail(iBytes > 0, -1, ;);
//
//    //szPtr = strstr(szInMessage, "407 Proxy Authentication Required");
//    //if (szPtr != NULL) {
//    //    return(JADA_PROXY_AUTHENTICATE);
//    //}
//
//    //szPtr = strstr(szInMessage, " 200");
//    //CORE_SilentReturnValIfFail(szPtr != NULL, -1, ;);
//    //if (szPtr != NULL) {
//    //    do {
//    //        iBytes = recv(iSocket, szInMessage, sizeof(szInMessage), 0);
//    //    } while (iBytes > 0);
//    //}
////4.Wait for a line of response. If it contains HTTP/1.X 200, the connection is successful.
////5.Read further lines of response until you receive an empty line.
////6.Now, you are connected to the outside world through a proxy. Do any data exchange you want.
//
//    *piSocket = iSocket;
//
//    return(0);
//}

int JADA_TcpConnectToPort(char *szHostName, int iPort, JADA_Socket *sokSelf)
{
    int                 iRet, iUseProxy = 0;
    int                 iSocket;

    /* Leggi il settaggio del proxy */
    iRet = JADA_IniGetInt("proxy", "use", &iUseProxy);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella lettura del proxy IP dal file .ini");

    /* Se c'è un proxy da saltare... */
    iUseProxy = 0;
    if (iUseProxy > 0) {
        //iRet = ConnectToServerViaProxy(szHostName, iPort, &iSocket);
        //if (iRet == JADA_PROXY_AUTHENTICATE) {
        //    JADA_LogMessage("%s, %d: il proxy richiede autenticazione\n", __FILE__, __LINE__);
        //    return(iRet);
        //}
        //CORE_ReturnValIfFail(iRet == 0, iRet, ;, "Errore nella connessione alla porta remota (proxy)");
    }
    /* Altrimenti connettiti direttamente */
    else {
        iRet = ConnectToRemotePort(szHostName, iPort, &iSocket);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore nella connessione alla porta remota (direct)");
    }

    iRet = JADA_TcpCreateSocket(sokSelf, iSocket);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
  
    return(0);
}

int JADA_TcpEnqueueMessage(JADA_Socket *sokSelf, JADA_Message msgOut, int iPriority)
{
    int     iRet;

    switch (iPriority) {
    case JADA_MSG_QUEUE_CALL:
        iRet = JADA_LstAdd(sokSelf->lstOutCallMsgs, &msgOut, sizeof(msgOut), NULL);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;
    case JADA_MSG_QUEUE_DESKTOP:
        iRet = JADA_LstAdd(sokSelf->lstOutVncMsgs, &msgOut, sizeof(msgOut), NULL);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;
    case JADA_MSG_QUEUE_FTP:
        iRet = JADA_LstAdd(sokSelf->lstOutFtpMsgs, &msgOut, sizeof(msgOut), NULL);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;
    case JADA_MSG_QUEUE_SESSION:
        iRet = JADA_LstAdd(sokSelf->lstOutSupportMsgs, &msgOut, sizeof(msgOut), NULL);
        CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");
        break;
    default:
        CORE_ReturnValIfFail(0, -1, ;, "Errore generico");
        break;
    }

    iRet = pthread_cond_broadcast(&sokSelf->queueCondVar);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "errore nella cond_broadcast della EnqueueMessage");

    return(0);
}

int JADA_TcpWaitEnqueuedMessage(JADA_Socket *sokSelf, int iMsecsToWait)
{
    int             iRet, iCount;
    struct timespec myTimeSpec;

    iCount = JADA_LstGetCount(sokSelf->lstOutCallMsgs);
    iCount += JADA_LstGetCount(sokSelf->lstOutVncMsgs);
    iCount += JADA_LstGetCount(sokSelf->lstOutFtpMsgs);
    iCount += JADA_LstGetCount(sokSelf->lstOutSupportMsgs);

    if (iCount == 0) {
        iRet = JADA_SetTimeSpec(&myTimeSpec, iMsecsToWait);
        iRet = pthread_mutex_lock(&sokSelf->queueMutex);
        iRet = pthread_cond_timedwait(&sokSelf->queueCondVar, &sokSelf->queueMutex, &myTimeSpec);
        iRet = pthread_mutex_unlock(&sokSelf->queueMutex);

        iCount = JADA_LstGetCount(sokSelf->lstOutCallMsgs);
        iCount += JADA_LstGetCount(sokSelf->lstOutVncMsgs);
        iCount += JADA_LstGetCount(sokSelf->lstOutFtpMsgs);
        iCount += JADA_LstGetCount(sokSelf->lstOutSupportMsgs);
    }

    return(iCount);
}

int JADA_TcpSendMessageZipped(JADA_Socket *sokSelf, JADA_Message msgOut)
{
    int             iRet, iBytes, iLength;
    int             iSent, iRemaining;
    char            *szZippedPayload, *ptrOut;
    JADA_MsgSelf    *msgPtr = (JADA_MsgSelf *) msgOut;
    JADA_MsgFooter  *ftrPtr;

    /* Controlla se il socket e' valido */
    CORE_ReturnValIfFail(sokSelf->iSocket > 0, -1, ;, "Errore generico");

    /* Controlla il footer del messaggio */
    ftrPtr = (JADA_MsgFooter *) &msgPtr->szPayload[msgPtr->szHeader.iLength];
    CORE_ReturnValIfFail(msgPtr->szHeader.iType == ftrPtr->iType, -1, ;, "Errore generico");

    /* Comprimi il payload del messaggio in uscita */
    iLength = msgPtr->szHeader.iLength + sizeof(JADA_MsgFooter);
    szZippedPayload = JADA_DeflateBuffer(msgPtr->szPayload, iLength, &msgPtr->szHeader.iZippedLength); 
    CORE_ReturnValIfFail(szZippedPayload != NULL, -1, ;, "Errore generico");

    /* Blocca il socket */
    iRet = pthread_mutex_lock(&sokSelf->socketMutex);
    CORE_ReturnValIfFail(iRet == 0, -1, free(szZippedPayload);, "Errore generico");

    /* Scrivi l'header del messaggio */
    ptrOut = (char *)&msgPtr->szHeader;
    iRemaining = sizeof(JADA_MsgHeader); iSent = 0;
    do {
        iBytes = send(sokSelf->iSocket, &ptrOut[iSent], iRemaining, 0);
        CORE_SilentReturnValIfFail(iBytes > 0, -1, free(szZippedPayload); pthread_mutex_unlock(&sokSelf->socketMutex););
        iSent += iBytes;
        iRemaining -= iBytes;
    } while (iRemaining > 0);

    /* Scrivi il payload del messaggio */
    ptrOut = (char *)szZippedPayload;
    iRemaining = msgPtr->szHeader.iZippedLength;  iSent = 0;
    do {
        iBytes = send(sokSelf->iSocket, &ptrOut[iSent], iRemaining, 0);
        CORE_SilentReturnValIfFail(iBytes > 0, -1, free(szZippedPayload); pthread_mutex_unlock(&sokSelf->socketMutex););
        iSent += iBytes;
        iRemaining -= iBytes;
    } while (iRemaining > 0);

    iRet = pthread_mutex_unlock(&sokSelf->socketMutex);
    CORE_ReturnValIfFail(iRet == 0, -1, free(szZippedPayload);, "Errore generico");

    free(szZippedPayload);

    return(0);
}

JADA_Message JADA_TcpReceiveMessageZipped(char *szFile, int iLine, JADA_Socket *sokSelf)
{
    int             iBytes, iLength;
    int             iReceived, iRemaining;
    JADA_MsgHeader  myHeader;
    JADA_Message    myMessage;
    JADA_MsgSelf    *msgPtr;
    JADA_MsgFooter  *ftrPtr;
    char            *bufPtr, *szUnzippedPayload;

    /* Controlla se il socket e' valido */
    CORE_ReturnValIfFail(sokSelf->iSocket > 0, NULL, ;, "Errore generico");

    /* Leggi inizialmente l'header del messaggio */
    iReceived = 0; iRemaining = sizeof(myHeader); 
    bufPtr = (char *)&myHeader;
    do {
        iBytes = recv(sokSelf->iSocket, &bufPtr[iReceived], iRemaining, 0);
        CORE_SilentReturnValIfFail(iBytes > 0, NULL, ;);
        iReceived += iBytes;
        iRemaining -= iBytes;
    } while (iRemaining > 0);
    CORE_ReturnValIfFail(iReceived == sizeof(myHeader), NULL, ;, "iReceived == sizeof(myHeader)");
    CORE_ReturnValIfFail(myHeader.iLength > 0, NULL, ;, "myHeader.iLength > 0");

    /* Crea il messaggio di dimensione adeguata */
    iLength = CORE_Max(myHeader.iLength, myHeader.iZippedLength);
    myMessage = JADA_MsgNew(szFile, iLine, myHeader.iType, myHeader.iMsgId, iLength);
    CORE_ReturnValIfFail(myMessage != NULL, NULL, ;, "errore sulla JADA_MsgNew");
    msgPtr = (JADA_MsgSelf *) myMessage;
    msgPtr->szHeader.iLength = myHeader.iLength;

    /* A seguire leggi il payload del messaggio */
    iReceived = 0; iRemaining = myHeader.iZippedLength; 
    bufPtr = (char *)&msgPtr->szPayload[0];
    do {
        iBytes = recv(sokSelf->iSocket, &bufPtr[iReceived], iRemaining, 0);
        CORE_ReturnValIfFail(iBytes > 0, NULL, ;, "errore sulla recv");
        iReceived += iBytes;
        iRemaining -= iBytes;
    } while (iRemaining > 0);

    /* Decomprimi il payload del messaggio in ingresso */
    iLength = msgPtr->szHeader.iLength + sizeof(JADA_MsgFooter);
    szUnzippedPayload = JADA_InflateBuffer(iLength, msgPtr->szPayload, myHeader.iZippedLength);
    CORE_ReturnValIfFail(szUnzippedPayload != NULL, NULL, ;, "Errore generico");
    memcpy(msgPtr->szPayload, szUnzippedPayload, iLength);
    free(szUnzippedPayload);

    /* Controlla il footer del messaggio */
    ftrPtr = (JADA_MsgFooter *) &msgPtr->szPayload[myHeader.iLength];
    CORE_ReturnValIfFail(msgPtr->szHeader.iType == ftrPtr->iType, NULL, ;, "Errore generico");

    return(myMessage);
}

int JADA_TcpSendMessage(JADA_Socket *sokSelf, JADA_Message msgOut)
{
    int     iRet, iResult;

    /* Controlla se il socket e' valido */
    CORE_ReturnValIfFail(sokSelf->iSocket > 0, -1, ;, "Errore generico");

    /* Aggiorna il contatore di messaggi sul canale */
    iRet = JADA_MsgSetCounter(msgOut, sokSelf->iCounter++);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    /* Blocca il socket */
    iRet = pthread_mutex_lock(&sokSelf->socketMutex);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    /* Scrivi il messaggio */
    iResult = JADA_TcpSendMessageToSocket(sokSelf->iSocket, sokSelf->szOutBuffer, sokSelf->iOutLength, msgOut);

    /* Sblocca il socket */
    iRet = pthread_mutex_unlock(&sokSelf->socketMutex);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "Errore generico");

    return(iResult);
}

JADA_Message JADA_TcpReceiveMessage(char *szFile, int iLine, JADA_Socket *sokSelf)
{
    JADA_Message            myMessage;

    /* Controlla se il socket e' valido */
    CORE_ReturnValIfFail(sokSelf->iSocket > 0, NULL, ;, "Errore generico");

    /* Leggi il messaggio dal socket */
    myMessage = JADA_TcpReceiveMessageFromSocket(szFile, iLine, sokSelf->iSocket, sokSelf->szInBuffer, sokSelf->iInLength);
    CORE_ReturnValIfFail(myMessage != NULL, NULL, ;, "errore in fase di lettura del messaggio");

    return(myMessage);
}

int JADA_TcpCreateSocket(JADA_Socket *sokSelf, int iSocket)
{
    int     iRet;

    memset(sokSelf, 0, sizeof(*sokSelf));

    sokSelf->iInLength = 32768;
    sokSelf->szInBuffer = JADA_Calloc(__FILE__, __LINE__, 1, sokSelf->iInLength);
    CORE_ReturnValIfFail(sokSelf->szInBuffer != NULL, -1, ;, "generic error");
    sokSelf->iOutLength = 32768;
    sokSelf->szOutBuffer = JADA_Calloc(__FILE__, __LINE__, 1, sokSelf->iOutLength);
    CORE_ReturnValIfFail(sokSelf->szOutBuffer != NULL, -1, ;, "generic error");

    iRet = pthread_mutex_init(&sokSelf->socketMutex, NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "generic error");
    iRet = pthread_mutex_init(&sokSelf->queueMutex, NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "generic error");
    iRet = pthread_cond_init(&sokSelf->queueCondVar, NULL);
    CORE_ReturnValIfFail(iRet == 0, -1, ;, "generic error");

    sokSelf->lstOutCallMsgs = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Message), NULL);
    CORE_ReturnValIfFail(sokSelf->lstOutCallMsgs != NULL, -1, ;, "generic error");
    sokSelf->lstOutVncMsgs = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Message), NULL);
    CORE_ReturnValIfFail(sokSelf->lstOutVncMsgs != NULL, -1, ;, "generic error");
    sokSelf->lstOutFtpMsgs = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Message), NULL);
    CORE_ReturnValIfFail(sokSelf->lstOutFtpMsgs != NULL, -1, ;, "generic error");
    sokSelf->lstOutSupportMsgs = JADA_LstNew(__FILE__, __LINE__, sizeof(JADA_Message), NULL);
    CORE_ReturnValIfFail(sokSelf->lstOutSupportMsgs != NULL, -1, ;, "generic error");

    sokSelf->iSocket = iSocket;

    return(0);
}

static int TcpDestroyList(char *szFile, int iLine, CORE_List lstSelf)
{
    int             iRet;
    JADA_Message    msgTmp;
    CORE_Currency   curM;

    iRet = JADA_LstGetFirst(lstSelf, &msgTmp, sizeof(msgTmp), &curM);
    while (iRet == 0) {
        iRet = JADA_MsgDestroy(szFile, iLine, msgTmp);
        iRet = JADA_LstGetNext(lstSelf, &msgTmp, sizeof(msgTmp), &curM);
    }
    iRet = JADA_LstDestroy(szFile, iLine, lstSelf);

    return(0);
}

int JADA_TcpDestroySocket(JADA_Socket *sokSelf)
{
    int             iRet;

    if (sokSelf->iSocket > 0) {
        closesocket(sokSelf->iSocket);
    }

    JADA_Free(__FILE__, __LINE__, sokSelf->szInBuffer, sokSelf->iInLength);
    JADA_Free(__FILE__, __LINE__, sokSelf->szOutBuffer, sokSelf->iOutLength);

    iRet = TcpDestroyList(__FILE__, __LINE__, sokSelf->lstOutCallMsgs);
    iRet = TcpDestroyList(__FILE__, __LINE__, sokSelf->lstOutVncMsgs);
    iRet = TcpDestroyList(__FILE__, __LINE__, sokSelf->lstOutFtpMsgs);
    iRet = TcpDestroyList(__FILE__, __LINE__, sokSelf->lstOutSupportMsgs);

    iRet = pthread_mutex_destroy(&sokSelf->socketMutex);
    iRet = pthread_mutex_destroy(&sokSelf->queueMutex);
    iRet = pthread_cond_destroy(&sokSelf->queueCondVar);

    memset(sokSelf, 0, sizeof(*sokSelf));

    return(0);
}

int JADA_TcpIsSocketOpened(JADA_Socket *sokSelf)
{
    if (sokSelf->iSocket > 0) {
        return(0);
    }
    return(-1);
}
