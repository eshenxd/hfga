/**************************************
      功能：获取本机IP
	  时间：2014年9月12日
	  作者：Ethan
**************************************/
#include "GetLocalIPAddress.h"



string IP::getIP()
{
	if(WSAStartup(MAKEWORD(1,1),&wsaData)!=0) 
	{ 
		return NULL; 
	}  

	ipAddress = fetchIPAddress(); 
	WSACleanup();  
	return ipAddress; 
}

string IP::fetchIPAddress()
{
	char host_name[225]; 
	if(gethostname(host_name,sizeof (host_name))==SOCKET_ERROR) 
	{ 
		cout<< "Error "<<WSAGetLastError()<<" when getting local host name."<<endl;  
	} 

	struct hostent *phe=gethostbyname(host_name); 
	if(phe==0) 
	{ 
		cout<< "Yow!Bad host lookup.";  
	} 

	struct in_addr addr; 
	memcpy(&addr,phe->h_addr_list[0], sizeof(struct in_addr));  
	return inet_ntoa(addr); 
}
