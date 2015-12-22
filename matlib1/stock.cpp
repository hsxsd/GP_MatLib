// matlib1.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "string.h"
#include "stock.h"

#include "winsock2.h"
#pragma comment (lib, "wsock32.lib")  

#include "unzip.h"
#pragma comment (lib, "zlib.lib")  

#ifdef _MANAGED
#pragma managed(push, off)
#endif

//F:\Program Files\MATLAB\R2007b\work\Stock.DLL

SOCKET sck=-1;
int szcount=0,shcount=0;
struct RecvDataHeader hd;
int datetime=20000101;
unsigned char recvbuffer[5*1024*1024];//接收缓冲
unsigned char debuffer[10*1024*1024];//解压后缓冲
static struct tdx_infostyle info[16];

// bool busying=false;
// //创建线程代码：  
// DWORD   dwThreadId;    
// HANDLE   hThread   =   NULL;  
FILE *flog = NULL;

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call,LPVOID lpReserved )
{

	if (ul_reason_for_call == DLL_PROCESS_ATTACH)//加载
	{
		//hThread=NULL;
		sck=-1;
		flog = fopen("gplog.txt", "w");
	}

	if (ul_reason_for_call == DLL_PROCESS_DETACH) //卸载
	{
// 		if (hThread!=NULL)
// 		{
// 			CloseHandle(hThread);  
// 			hThread=NULL;
// 		}

		if (sck!=-1)
		{
			closesocket(sck);   
			sck=-1;
		}   

		if (flog != NULL)
		{
			fclose(flog);
		}

		//busying=false;
		WSACleanup();  
	} 

	return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif


//保存数据
void SaveRecvData()
{
	char sn[100];
	sprintf(sn,"c:\\MT[%.04x].dat",hd.msgid);
	//CString s1;
	//s1.Format("c:\\tdx[%.04x].dat",hd.msgid);
	HFILE  f=_lcreat(LPCSTR(sn),0);
	_hwrite(f,(char *)&debuffer,hd.DePackSize);
	_lclose(f);
}


//接收数据
bool RecvData(void)
{ 
	fprintf(flog, "开始RecvData\n");
	if (sck == -1)
	{
		fprintf(flog, "错误：Socket为-1\n退出RecvData\n");
		fflush(flog);
		return false;
	}


	bool ret=false;
	int hlen = sizeof(RecvDataHeader);
	fprintf(flog, "开始接收消息头\n");
	int len=recv(sck,(char *)&hd,hlen,0);// recvbuffer,hlen);
	if (len!=hlen)
	{
		fprintf(flog, "错误：接收消息头长度错误len(%d) != hlen(%d)\n退出RecvData\n", len, hlen);
		if (len == SOCKET_ERROR)
		{
			fprintf(flog, "recv错误：返回值=%d, 错误码=%d\n", len, WSAGetLastError());
		}
		linkclose();
		fflush(flog);
		return false;   
	}
	if (hd.CheckSum != 7654321)
	{
		fprintf(flog, "错误：接收消息头checksum错误\n退出RecvData\n");
		fflush(flog);
		return false;
	}
	int elen=hd.Size;
	int fcur = 0;

	fprintf(flog, "开始接收消息体\n");
	while(fcur<elen)
	{
		int len=recv(sck,(char *)recvbuffer+fcur,hlen,0);// sck->Receive(recvbuffer+fcur, elen - fcur);
		if (len <0)
		{
			fprintf(flog, "recv错误：返回值%d，错误码%d\n退出RecvData\n", len, WSAGetLastError());
			linkclose();
			fflush(flog);
			return false;
		}
		else
			fcur = fcur +len;
	}
	if (fcur != elen)
	{
		fprintf(flog, "错误：接收消息体长度错误\n退出RecvData\n");
		fflush(flog);
		return false;
	}
	if ((hd.EncodeMode & 0x10)==0x10)	
	{
		unsigned int long delen=hd.DePackSize;
		int  err=uncompress(debuffer,&delen,recvbuffer,hd.Size);
		if (err != 0)//解压错误
		{
			fflush(flog);
			return false;
		}
	}
	else
		CopyMemory(debuffer,recvbuffer,hd.Size);
	fflush(flog);
	return true;
}

extern "C" __declspec(dllexport) int  linkclose()
{
	int i=1;
	if (sck!=-1)
	{
		closesocket(sck);
		sck=-1;
	}   

// 	if (hThread!=NULL)
// 	{
// 		TerminateThread(hThread, 0);
// 		CloseHandle(hThread);  
// 		hThread=NULL;
// 		i=2;
// 	}
// 	busying=false;
	WSACleanup();  
	fflush(flog);
	return i;
}

extern "C" __declspec(dllexport) int  checklink()
{

	return 0;
}

// 线程函数  
// DWORD   __stdcall   ThreadFun(void   *)  
// {  
// 	fprintf(flog, "ThreadFun开始\n");
// 	busying=false;
// 	while(true)
// 	{
// 		fprintf(flog, "ThreadFun循环开始\n");
// 		if ((sck!=-1) && (busying==false))
// 		{
// 			busying=true;
// 			unsigned char bb1[]="\x0C\x0D\x00\x2A\x00\x01\x3B\x00\x3B\x00\x26\x05\x05\x00\x01\x39"
// 				"\x39\x39\x39\x39\x39\x5D\x4A\x02\x00\x00\x33\x39\x39\x30\x30\x31"
// 				"\x25\x4A\x02\x00\x00\x33\x39\x39\x30\x30\x35\x10\x4A\x02\x00\x00"
// 				"\x33\x39\x39\x33\x30\x30\x68\x4A\x02\x00\x00\x33\x39\x39\x30\x30"
// 				"\x36\x10\x4A\x02\x00";
// 			fprintf(flog, "ThreadFun循环发送心跳包\n");
// 			if (send(sck, (char *)bb1, sizeof(bb1) - 1, 0) != SOCKET_ERROR)
// 			{
// 				fprintf(flog, "ThreadFun循环接收心跳包结果\n");
// 				RecvData();
// 			}
// 			busying=false;
// 		}
// 		Sleep(4000);
// 		fprintf(flog, "ThreadFun循环结束\n");
//		fflush(flog);
// 	} 
// 	hThread=NULL;
// 	fprintf(flog, "ThreadFun结束\n");
// 	fflush(flog);
//	return   0;
// }   
// 
// bool CreateTimer()
// {
// 	hThread   =   CreateThread(NULL,0,ThreadFun,NULL,0,&dwThreadId);  
// 	if   (hThread   ==   NULL)    
// 		return false;
// 	Sleep(1000);//等待线程创建好了  
// 	return true;
// }
// 
// bool DeleteTimer()
// { 
// 	//PostThreadMessage(dwThreadId,WM_USER+1,0,0);//给线程发消息准备退出线程 
// 	if (hThread!=NULL)
// 	{
// 		TerminateThread(hThread, 0);
// 		CloseHandle(hThread);  
// 		hThread=NULL;
// 	}
// 
// 	return true;
// }

//v 解包成年月日时分
void TDXGetDate(DWORD v,int &yy,int &mm,int &dd,int &hhh,int &mmm)
{
	yy=2012;
	mm=1;
	dd=1;
	hhh=9;
	mmm=30;
	if (v>21000000)
	{
		yy=2004+((v & 0xF800) >> 11);
		int d1=v & 0x7FF;
		mm=d1 /100;
		dd=d1%100;
		int d2=v >>16;
		hhh=d2/60;
		mmm=d2%60;
	}
	else
	{
		yy = v/10000;
		mm = (v - yy * 10000) /100;
		dd =v%100;
		hhh=9;
		mmm=30;
	}
}

//解包数据
int TDXDecode(char *buf,int start,int &next)
{
	unsigned int num;
	long int num3, num2, num4, num5, num6, num7, num8;
	byte cc;
	num = 0;
	num3 = 0;
	num2 = 0;
	while (num2 < 0x20)
	{
		cc = buf[start + num2];
		num4 =cc;
		num5 = (num4 & 0x80) /0x80;
		if (num2 == 0) 
		{
			num3 = 1 - (((num4 & 0x40) / 0x40) * 2);
			num6 = num4 & 0x3F;
			num = num + num6;
		}
		else if (num2 == 1) 
		{
			num7 = (num4 & 0x7F) *(2 << (num2*6-1));//  power(2, num2 * 6));
			num = num + num7;
		}
		else
		{
			num8 = (num4 & 0x7F) *(2<<(num2*7-2));// Power(2, (num2 * 7) - 1);
			num = num + num8;
		}
		if (num5 == 0) 
		{
			num = num * num3;
			break;
		}
		num2++;
	}
	next=start+num2+1;
	return num; 
}
//读取16位数据
short int TDXGetInt16(char *buf,int start,int &next)
{
	short int *Num=(short int *)&buf[start];
	next = start + 2;
	return *Num;
}
//读取32位数据
long  int TDXGetInt32(char *buf,int start,int &next)
{
	long int *Num=(long int *)&buf[start];
	next = start + 4;
	return *Num;
}
//读取浮点数据float
float TDXGetDouble(char *buf,int start,int &next)
{
	float *d1=(float*)&buf[start];
	next = start + 4;
	return *d1;
}

extern "C" __declspec(dllexport) int linkServer(char *server,int port)
{
	WORD wVersionRequested;   
	WSADATA wsaData;   
	wVersionRequested = MAKEWORD( 2, 2 );   
	int err = WSAStartup( wVersionRequested, &wsaData );   
	if ( err != 0 ) 
	{   
		//cout<<"加载链接库失败"<<endl;   
		return -1;   
	}   
	if ( LOBYTE( wsaData.wVersion ) != 2 ||  HIBYTE( wsaData.wVersion ) != 2 ) 
	{   
		WSACleanup();   
		//cout<<"加载链接库版本错误"<<endl;   
		return -2;    
	}   
	sck=socket(AF_INET,SOCK_STREAM,IPPROTO_IP);//创建客户套接字   
	if (sck== INVALID_SOCKET)
	{
		WSACleanup();  	  
		return -3;
	}
	//服务器地址   
	SOCKADDR_IN addrOFserver;   
	addrOFserver.sin_addr.S_un.S_addr=inet_addr(server);   
	addrOFserver.sin_family=AF_INET;   
	addrOFserver.sin_port=htons(port);   

	connect(sck,(SOCKADDR*)&addrOFserver,sizeof(SOCKADDR)); 

	int timeout = 10000;
	setsockopt(sck, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	setsockopt(sck, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));

	unsigned char bb[]={0x0C,0x02,0x18,0x93,0x00,0x01,0x03,0x00,0x03,0x00,0x0D,0x00,0x01};

	fprintf(flog, "linkServer发送数据0x0C,0x02,0x18,0x93,0x00,0x01,0x03,0x00,0x03,0x00,0x0D,0x00,0x01\n");
	fflush(flog);
	if (send(sck,(char*)bb,sizeof(bb),0)== SOCKET_ERROR)
	{
		linkclose();
		fprintf(flog, "linkServer发送数据失败\n");
		fflush(flog);
		return -5;
	}
	fprintf(flog, "linkServer发送数据成功\n");
	fflush(flog);
	if (RecvData()==1)
	{
		int *k=(int *)&debuffer[42];
		datetime=*k;
	}
// 	if (datetime>0)
// 		CreateTimer();  
	return datetime;   
}

extern "C" __declspec(dllexport) int link()
{  
	return linkServer("218.18.103.38",7709);
}

extern "C" __declspec(dllexport) int gethistory(double *out,int mark,char *code,int start, int count,int zq)
{
	if (sck==NULL)
		return -1;
	if (strlen(code)!=6)
		return -2;

	bool zs=false;
	int CodeNum=atoi(code);

	if ((mark == 0) && (CodeNum >= 390000) && (CodeNum < 400000))
		zs = true;
	if ((mark == 1) && (CodeNum >= 930000) && (CodeNum < 999999))
		zs = true;
	if ((mark == 1) && (CodeNum >= 000001) && (CodeNum < 001000))
		zs = true;

	if ((mark==1) && (CodeNum >= 880200))
		zs=true;
// 	while(busying)
// 	{
// 
// 	}
// 	busying=true;
	unsigned char bb1[]="\x0C\x01\x08\x64\x01\x01\x12\x00\x12\x00\x29\x05\xFF\x00\x31\x32\x33\x34\x35\x36\xFE\x00\x01\x00\xF1\xF2\xE1\xE2";
	bb1[12]=mark;//市场0深圳 1上海
	CopyMemory(&bb1[14],code,6);//股票代码
	bb1[20]=zq;//周期
	WORD *start1=(WORD *)&bb1[24];
	*start1=start;//开始值
	WORD *count1=(WORD *)&bb1[26];
	*count1=count;//开始值

	fprintf(flog, "getHistory发送数据\n");
	fflush(flog);
	if (send(sck,(char *)bb1,sizeof(bb1)-1,0)== SOCKET_ERROR)
	{
		linkclose();
		// 		busying=false;
		fprintf(flog, "getHistory发送数据失败\n");
		fflush(flog);
		return -6;  
	}
	fprintf(flog, "getHistory发送成功\n");
	fflush(flog);
	int DataLen=-5;
	if (RecvData()==1)
	{
		//SaveRecvData();
		int len=hd.DePackSize;

		WORD *n=(WORD *)&debuffer[0];
		if (*n<1)
		{   
// 			busying=false;
			return DataLen;
		}
		DataLen=*n;
		int i=2;
		float js=1000.0;
		int yy,mm,dd,hhh,mmm;
		int num5, num6, num7, num8, num9;
		char *buf=(char *)&debuffer;
		num9=0;
		int t=0;
		for (int j=0;j<*n;j++)
		{
			TDXGetDate(TDXGetInt32(buf,i,i),yy,mm,dd,hhh,mmm);
			out[t++]=yy*10000+mm*100+dd;
			out[t++]=hhh*100+mmm;
			num5 = TDXDecode(buf, i, i);
			out[t++]=(num9+num5)/js;// open 
			num6 = TDXDecode(buf, i, i);
			out[t++] = (num9+num5 + num6)/js;//close 
			num7 = TDXDecode(buf, i, i);
			out[t++] = (num9+num5 + num7)/js;//high ;
			num8 = TDXDecode(buf, i, i);
			out[t++] = (num9+num5 + num8)/js;//low ;
			out[t++] =TDXDecode(buf, i, i); //volume
			out[t++] =TDXGetDouble(buf, i, i); //amount
			num9 = num9+num5 + num6;
			if (zs==true)
			{
				out[t++]=TDXGetInt16(buf,i,i);
				out[t++]=TDXGetInt16(buf,i,i);
			}
			else
			{
				out[t++]=0;
				out[t++]=0;
			}
			if (i>len) 
				break;
		}
	}
// 	busying=false;
	return DataLen;
}

extern "C" __declspec(dllexport) int stockpklist(int count,int mark[],char *code[], double *out)
{
	int ret=-1;
	if (sck==-1)
		return -1;
// 	while(busying)
// 	{
// 
// 	}
// 	busying=true;
	unsigned char bb[800];
	unsigned char bb2[]={0x0C,0x01,0x20,0x63,0x00,0x02,0x13,0x00,0x13,0x00,0x3E,0x05,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00};
	CopyMemory(bb,bb2,sizeof(bb2));
	int i=sizeof(bb2);
	for (int t=0;t<count;t++)
	{
		if (strlen(code[t])==6)
		{
			bb[i++]=mark[t];
			CopyMemory(&bb[i],code[t],6);      
			i+=6;
		}
	}
	bb[20]=(i-22)/7;//数量
	int len=i-10;
	WORD *size1=(WORD *)&bb[6];
	*size1=len;
	WORD *size2=(WORD *)&bb[8];
	*size2 = len;
	fprintf(flog, "stkpklist发送数据\n");
	fflush(flog);
	if (send(sck,(char *)bb,i,0)== SOCKET_ERROR)
	{
		linkclose();
		// 		busying=false;
		fprintf(flog, "stkpklist发送数据失败\n");
		fflush(flog);
		return -3;
	}
	fprintf(flog, "stkpklist发送数据成功\n");
	fflush(flog);
	if (RecvData()==1)
	{
		WORD *n=(WORD *)&debuffer[2];
		if (*n<1)
		{
// 			busying=false;
			return -4;
		}
		ret=*n;
		char *buf=(char *)&debuffer[4];
		int i=0;
		int t=0;
		for (int j=0;j<*n;j++)
		{
			byte m=buf[i];
			char code[8];
			CopyMemory(code,buf+i+1,6);
			code[6]=0;
			float dd=100.0;
			int codeint=atoi(code);
			i+=9;
			float prize=TDXDecode(buf, i, i) / dd;
			out[t++]=m;
			out[t++]=codeint;
			out[t++]= prize;
			//float last =
			out[t++]=prize + (TDXDecode(buf, i, i) / dd);
			//float open
			out[t++]= prize + (TDXDecode(buf, i, i) / dd);
			//float high 
			out[t++]= prize + (TDXDecode(buf, i, i) / dd);
			//float low
			out[t++]= prize + (TDXDecode(buf, i, i) / dd);
			//int Time 
			out[t++]= TDXDecode(buf, i, i);
			TDXDecode(buf, i, i);
			//int volume
			out[t++]=TDXDecode(buf, i, i);
			//int tradeQTY 
			out[t++]= TDXDecode(buf, i, i);//现量
			//float amount
			out[t++]= TDXGetDouble(buf, i, i)/10000;
			//int b 
			out[t++]= TDXDecode(buf, i, i);
			//int s 
			out[t++]= TDXDecode(buf, i, i);

			int d1 = TDXDecode(buf, i, i);
			int d2 = TDXDecode(buf, i, i);  
			out[t++]=d2;
			//float buy1 
			out[t++]= prize + (((TDXDecode(buf, i, i)) / dd));
			//float sell1
			out[t++]= prize + (((TDXDecode(buf, i, i)) / dd));
			//int buyQTY1 = 
			out[t++]=TDXDecode(buf, i, i);
			//int sellQTY1 
			out[t++]= TDXDecode(buf, i, i);
			//float buy2 
			out[t++]= prize + (((TDXDecode(buf, i, i)) / dd));
			//float sell2 
			out[t++]= prize + (((TDXDecode(buf, i, i)) / dd));
			//int buyQTY2 
			out[t++]= TDXDecode(buf, i, i);
			//int sellQTY2 
			out[t++]= TDXDecode(buf, i, i);
			//float buy3 
			out[t++]= prize + (((TDXDecode(buf, i, i)) / dd));
			//float sell3 
			out[t++]= prize + (((TDXDecode(buf, i, i)) / dd));
			//int buyQTY3 
			out[t++]= TDXDecode(buf, i, i);
			//int sellQTY3 
			out[t++]= TDXDecode(buf, i, i);
			//float buy4 
			out[t++]= prize + (((TDXDecode(buf, i, i)) / dd));
			//float sell4 
			out[t++]= prize + (((TDXDecode(buf, i, i)) / dd));
			//int buyQTY4 
			out[t++]= TDXDecode(buf, i, i);
			//int sellQTY4 
			out[t++]= TDXDecode(buf, i, i);
			//float buy5 
			out[t++]= prize + (((TDXDecode(buf, i, i)) / dd));
			//float sell5
			out[t++]= prize + (((TDXDecode(buf, i, i)) / dd));
			//int buyQTY5 
			out[t++]= TDXDecode(buf, i, i);
			//int sellQTY5 
			out[t++]= TDXDecode(buf, i, i);
			i = i + 3;
			int d3 = TDXDecode(buf, i, i);
			int d4 = TDXDecode(buf, i, i);  
			int d5 = TDXDecode(buf, i, i);
			int t1=TDXGetInt16(buf,i,i);
			//float speed
			out[t++]=(float)(t1/100.0);
			TDXGetInt16(buf,i,i);
		}

	}
// 	busying=false;
	return ret;
}
//取基金净值曲线数据
extern "C" __declspec(dllexport) int getiopv(int mark,char *code,double *out)
{
	int ret=-1;
	if (sck==-1)
		return -1;
// 	while(busying)
// 	{
// 
// 	}
// 	busying=true;

	unsigned char a[] ={ 0x0C, 0x26, 0x08, 0x00, 0x01, 0x01, 0x0E, 0x00, 0x0E, 0x00, 0x2E, 0x05, 0x00, 0x00, 0x31, 0x35, 0x39, 0x39, 0x32, 0x36, 0x00, 0x00, 0x00, 0x00 };
	a[12] =mark;
	CopyMemory(&a[14], code, 6);//股票代码
	fprintf(flog, "getiopv发送数据\n");
	fflush(flog);
	if (send(sck,(char *)a,sizeof(a),0)== SOCKET_ERROR)
	{
		linkclose();
		// 		busying=false;
		fprintf(flog, "getiopv发送数据失败\n");
		fflush(flog);
		return -3;
	}
	fprintf(flog, "getiopv发送数据成功\n");
	fflush(flog);
	if (RecvData()==1)
	{
		WORD *n=(WORD *)&debuffer[0];
		if (*n<1)
		{ 
// 			busying=false;
			return -4;
		}
		char *buf=(char *)&debuffer[0];
		ret=*n;
		int i = 4;
		int first=0,vol,iocp1=0,k=0;
		for (int t = 0; t <*n; t++)
		{
			first = first + TDXDecode(buf, i,  i);
			int d2 = TDXDecode(buf, i,  i);
			vol = TDXDecode(buf, i,  i);
			iocp1=iocp1+ TDXDecode(buf, i,  i);
			double v1 = (double)first / 1000;
			out[k++]=v1;
			double d4 = (double)iocp1 / 100000;
			out[k++]=d4;
			out[k++]=vol;
		}
	}
// 	busying=false;
	return ret;
}

extern "C" __declspec(dllexport) int stockpkbase(double *out,int mark,char *code)
{
	if (sck==-1)
		return -1;

// 	while(busying)
// 	{
// 
// 	}
// 	busying=true;
	unsigned char bb1[]="\x0C\x01\x20\x63\x00\x02\x13\x00\x13\x00\x3E\x05\x05\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x31\x32\x33\x34\x35\x36";
	bb1[22]=mark;//市场0深圳 1上海
	CopyMemory(&bb1[23], code, 6);//股票代码
	fprintf(flog, "stockpkbase发送数据\n");
	fflush(flog);
	if (send(sck,(char *)bb1,sizeof(bb1)-1,0)== SOCKET_ERROR)
	{
		linkclose();
		// 		busying=false;
		fprintf(flog, "stockpkbase发送数据失败\n");
		fflush(flog);
		return -3;
	}
	fprintf(flog, "stockpkbase发送数据成功\n");
	fflush(flog);
	if (RecvData()==1)
	{
		WORD *n=(WORD *)&debuffer[2];
		if (*n<1)
		{
// 			busying=false;
			return -4;
		}
		char *buf=(char *)&debuffer[4];
		int i=9;  

		float dd=100.0;
		if (code[0] == '5' &&
			code[1] == '1' &&
			(code[2] == '0' || code[2] == '1' || code[2] == '2' || code[2] == '3'))
		{
			dd = 1000.0;
		}

		float prize=TDXDecode(buf, i, i) / dd;
		out[0]= prize;
		//float last =
		out[1]=prize + (TDXDecode(buf, i, i) / dd);
		//float open
		out[2]= prize + (TDXDecode(buf, i, i) / dd);
		//float high 
		out[3]= prize + (TDXDecode(buf, i, i) / dd);
		//float low
		out[4]= prize + (TDXDecode(buf, i, i) / dd);
		//int Time 
		out[5]= TDXDecode(buf, i, i);
		TDXDecode(buf, i, i);
		//int volume
		out[6]=TDXDecode(buf, i, i);
		//int tradeQTY 
		out[7]= TDXDecode(buf, i, i);//现量
		//float amount
		out[8]= TDXGetDouble(buf, i, i)/10000;
		//int b 
		out[9]= TDXDecode(buf, i, i);
		//int s 
		out[10]= TDXDecode(buf, i, i);

		int d1 = TDXDecode(buf, i, i);
		int d2 = TDXDecode(buf, i, i);  

		//float buy1 
		out[11]= prize + (((TDXDecode(buf, i, i)) / dd));
		//float sell1
		out[12]= prize + (((TDXDecode(buf, i, i)) / dd));
		//int buyQTY1 = 
		out[13]=TDXDecode(buf, i, i);
		//int sellQTY1 
		out[14]= TDXDecode(buf, i, i);
		//float buy2 
		out[15]= prize + (((TDXDecode(buf, i, i)) / dd));
		//float sell2 
		out[16]= prize + (((TDXDecode(buf, i, i)) / dd));
		//int buyQTY2 
		out[17]= TDXDecode(buf, i, i);
		//int sellQTY2 
		out[18]= TDXDecode(buf, i, i);
		//float buy3 
		out[19]= prize + (((TDXDecode(buf, i, i)) / dd));
		//float sell3 
		out[20]= prize + (((TDXDecode(buf, i, i)) / dd));
		//int buyQTY3 
		out[21]= TDXDecode(buf, i, i);
		//int sellQTY3 
		out[22]= TDXDecode(buf, i, i);
		//float buy4 
		out[23]= prize + (((TDXDecode(buf, i, i)) / dd));
		//float sell4 
		out[24]= prize + (((TDXDecode(buf, i, i)) / dd));
		//int buyQTY4 
		out[25]= TDXDecode(buf, i, i);
		//int sellQTY4 
		out[26]= TDXDecode(buf, i, i);
		//float buy5 
		out[27]= prize + (((TDXDecode(buf, i, i)) / dd));
		//float sell5
		out[28]= prize + (((TDXDecode(buf, i, i)) / dd));
		//int buyQTY5 
		out[29]= TDXDecode(buf, i, i);
		//int sellQTY5 
		out[30]= TDXDecode(buf, i, i);
		i = i + 3;
		int d3 = TDXDecode(buf, i, i);
		int d4 = TDXDecode(buf, i, i);  
		int d5 = TDXDecode(buf, i, i);
		int t=TDXGetInt16(buf,i,i);
		//float speed
		out[31]=(float)(t/100.0);
		out[32]=d2;
		TDXGetInt16(buf,i,i);
// 		busying=false;
		return 1;
	}
// 	busying=false;
	return -5;
}


extern "C" __declspec(dllexport) int getlist(double *list,int count)
{
	for (int i=0;i<count;i++)
		list[i]=i*100;

	return count;

}

extern "C" __declspec(dllexport) int getstruct(struct qh_pk1 *value,int mark,char *code)
{
	value->marketmode=47;

	return 1;
}
extern "C" __declspec(dllexport) int savecodelist()
{
	char sPath[255];

	int k=GetModuleFileName(NULL,sPath,255); 
	sPath[k]=0;

	char sPath1[255];

	GetCurrentDirectory(255,sPath1);

	char *filename=strcat(sPath1,"\\codelist.csv");
// 
// 	while(busying)
// 	{
// 
// 	}
// 	busying=true;
	unsigned char bb[]="\x0C\x0C\x18\x6C\x00\x01\x08\x00\x08\x00\x4E\x04\xFF\x00\x01\x02\x03\x04";//取得股票数量
	bb[12]=0;//0深圳 1上海
	int *dt=(int *)&bb[14];
	*dt = datetime;
	fprintf(flog, "savecodelist发送数据\n");
	fflush(flog);
	if (send(sck,(char *)bb,sizeof(bb)-1,0)== SOCKET_ERROR)
	{
		linkclose();
		// 		busying=false;
		fprintf(flog, "savecodelist发送数据失败\n");
		fflush(flog);
		return -3;
	}
	fprintf(flog, "savecodelist发送数据成功\n");
	fflush(flog);
	Sleep(20);
	if (RecvData()==1)
	{
		WORD *StockLen=(WORD *)&debuffer[0];
		int szlen=*StockLen;
		char str[400];

		HFILE  f=_lcreat(LPCSTR(filename),0);
		if (f==HFILE_ERROR)
		{
// 			busying=false;
			return -10;
		}
		int k=0;
		int count=0;
		while(count<szlen)
		{
			unsigned char bb11[]="\x0C\x01\x18\x64\x01\x01\x06\x00\x06\x00\x50\x04\xFF\x00\xF2\xF3";
			bb11[12]=0;
			WORD *len=(WORD *)&bb11[14];
			*len=count;
			if (send(sck,(char *)bb11,sizeof(bb11)-1,0)== SOCKET_ERROR)
			{
				linkclose();
// 			busying=false;
				return -3;
			}  
			Sleep(20);
			if (RecvData()==1)
			{

				WORD *n=(WORD *)&debuffer[0];
				int sss=sizeof(struct TDX_STOCKINFO);
				for (int j=0;j<*n;j++)
				{

					struct TDX_STOCKINFO *sf=(struct TDX_STOCKINFO *)&debuffer[2+j*sizeof(struct TDX_STOCKINFO)];
					char code[10];
					CopyMemory(code,sf->code,6);
					code[6]=0;
					char name[10];
					CopyMemory(name,sf->name,8);
					name[8]=0;

					sprintf(str,"%d,%d,%s,%s,%.2f,%d,%d,%d,%d\r\n", j,0,count+j,code,name,sf->YClose,sf->w1,sf->w2,sf->w3,sf->w4);
					_hwrite(f,(char *)&str,(long)strlen(str));

				}    
				count+=*n;
			}
			else
				break;
		}
		_lclose(f);
// 		busying=false;
		return szlen;
	}
// 	busying=false;
	return -6;
}


extern "C" __declspec(dllexport) int stockcaiwu(double *out,int mark,char *code)
{
	if (sck==-1)
		return -1;
// 	while(busying)
// 	{
// 
// 	}
// 	busying=true;
	unsigned char bb1[]="\x0C\x1F\x18\x76\x00\x01\x0B\x00\x0B\x00\x10\x00\xDD\x00\xFF\x31\x32\x33\x34\x35\x36";
	bb1[12]=1;//数量
	bb1[14]=mark;//市场0深圳 1上海
	CopyMemory(&bb1[15],code,6);//股票代码
	fprintf(flog, "stockcaiwu发送数据\n");
	fflush(flog);
	if (send(sck,(char *)bb1,sizeof(bb1)-1,0)== SOCKET_ERROR)
	{
		linkclose();
		// 	busying=false;
		fprintf(flog, "stockcaiwu发送数据失败\n");
		fflush(flog);
		return -3;
	}
	fprintf(flog, "stockcaiwu发送数据成功\n");
	fflush(flog);
	if (RecvData()==1)
	{
		WORD *n=(WORD *)&debuffer[0];
		if (*n==0)
		{
// 			busying=false;
			return -5;
		}

		struct CaiWu *cw=(struct CaiWu *)&debuffer[2];
		out[0]=cw->LTG;
		out[1]=cw->day1;
		out[2]=cw->day2;
		for (int t=0;t<30;t++)
			out[3+t]=cw->zl[t];
// 		busying=false;
		return 1;
	}
// 	busying=false;
	return -4;
}


extern "C" __declspec(dllexport) int stockquan(double *out,int mark,char *code)
{
	if (sck==-1)
		return -1;
// 	while(busying)
// 	{
// 
// 	}
// 	busying=true;
	unsigned char bb1[]="\x0C\x1F\x18\x76\x00\x01\x0B\x00\x0B\x00\x0F\x00\xDD\x00\xFF\x31\x32\x33\x34\x35\x36";
	bb1[12]=1;//数量
	bb1[14]=mark;//市场0深圳 1上海
	CopyMemory(&bb1[15],code,6);//股票代码
	fprintf(flog, "stockquan发送数据\n");
	fflush(flog);
	if (send(sck,(char *)bb1,sizeof(bb1)-1,0)== SOCKET_ERROR)
	{
		linkclose();
		// 	    busying=false;
		fprintf(flog, "stockquan发送数据失败\n");
		fflush(flog);
		return -3;
	}
	fprintf(flog, "stockquan发送数据成功\n");
	fflush(flog);
	int datalen=-4;
	if (RecvData()==1)
	{  
		WORD *n=(WORD *)&debuffer[0];
		if (*n==0)
		{
// 			busying=false;
			return -5;
		}
		int i=2;
		int t3=0;
		int tt=0;
		for (int j=0;j<*n;j++)
		{  
			byte m=debuffer[i];
			i += 7;
			WORD *k=(WORD *)&debuffer[i];
			i+=2;   
			for (int t=0;t<*k;t++)
			{
				m= debuffer[i];   
				int *date=(int *)&debuffer[i+8];//  TDXGetInt32((char*)debuffer, num5, num5);
				byte  t1 = debuffer[i + 12];
				if (t1 == 1)
				{
					float *d1 =(float *)&debuffer[i+13];// TDXGetDouble((char*)debuffer, i + 13, num5);
					float *d2= (float *)&debuffer[i+13+4];//TDXGetDouble((char*)debuffer, i + 13 + 4, num5);
					float *d3= (float *)&debuffer[i+13+8];//TDXGetDouble((char*)debuffer, i + 13 + 4, num5);
					float *d4= (float *)&debuffer[i+13+12];//TDXGetDouble((char*)debuffer, i + 13 + 4, num5);
					out[tt++]= *date;
					out[tt++]= *d1;
					out[tt++] =*d2;
					out[tt++] =*d3; 
					out[tt++] =*d4; 
					//s4.Format(_T("%d Date:%d  送现金:%5.2f 配股价:%5.2f 送股数:%5.2f 配股比例:%5.2f\r\n"),t3,*date,*d1, *d2, *d3, *d4);
					//s3+=s4;
					t3+=1;
				}
				i= i + 29;
			}
		}
		datalen=t3;
// 	    busying=false;
		return datalen;
	}
//     busying=false;
	return -4;
}

char* mystrcpy(char* des,const char* source)
{
	if ((des==NULL) || (source==NULL))
		return NULL;
	char* r=des;
	while((*des++ = *source++)!='\0');
	return r;
}

extern "C" __declspec(dllexport) int getinfo(int mark,char *code,int type1,char *out)
{
	if (sck==-1)
		return -1;
// 	while(busying)
// 	{
// 
// 	}
// 	busying=true;
	unsigned char bb1[]="\x0C\x0F\x10\x9B\x00\x01\x0E\x00\x0E\x00\xCF\x02\x00\xFF\x31\x32\x33\x34\x35\x36\x00\x00\x00\x00";
	bb1[13]=mark;//市场0深圳 1上海
	CopyMemory(&bb1[14],code,6);//股票代码
	fprintf(flog, "getinfo发送数据\n");
	fflush(flog);
	if (send(sck,(char *)bb1,sizeof(bb1)-1,0)== SOCKET_ERROR)
	{
		linkclose();
		// 		busying=false;
		fprintf(flog, "getinfo发送数据失败\n");
		fflush(flog);
		return -3;
	}
	fprintf(flog, "getinfo发送数据成功\n");
	fflush(flog);
	int ret=-1;
	if (RecvData()==1)
	{
		//SaveRecvData();
		WORD *n=(WORD *)&debuffer[0];
		if (*n<1)
		{
// 			busying=false;
			return -2;
		}
		ret=*n;
		int i=2;
		for (int j=0;j<*n;j++)
		{
			//char name[10];
			//CopyMemory(name,&debuffer[i],8);
			//name[8]=0;
			i=i+64;
			//char code[10];
			//CopyMemory(code,&debuffer[i],6);
			//code[6]=0;
			i=i+80;
			int *start=(int *)&debuffer[i];
			int *len=(int *)&debuffer[i+4];
			//CopyMemory(info[j].code,code,6);
			info[j].start=*start;
			info[j].len=*len;
			i=i+8;
		}
		if (type1<*n)
		{
			unsigned char bb2[]="\x0C\x07\x10\x9C\x00\x01\x68\x00\x68\x00\xD0\x02\x00\x00\x30\x30"
				"\x30\x30\x31\x30\x00\x00\x30\x30\x30\x30\x31\x30\x2E\x74\x78\x74"
				"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
				"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
				"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
				"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
				"\x00\x00\x00\x00\x00\x00\x12\x50\x05\x00\x80\x30\x00\x00\x00\x00"
				"\x00\x00";
			bb2[13]=mark;//市场0深圳 1上海
			CopyMemory(&bb2[14],code,6);//股票代码
			bb2[20]=type1;//资料分类 -序号0~15
			CopyMemory(&bb2[22],code,6);//股票代码
			int t=info[type1].start;
			int nn=0;
			out[0]=0;
			while(nn<info[type1].len)
			{
				int *start=(int *)&bb2[102];
				*start=info[type1].start+nn;
				int *len=(int *)&bb2[106];
				*len=info[type1].len-nn;
				if (send(sck,(char *)bb2,sizeof(bb2)-1,0)== SOCKET_ERROR)
				{
					linkclose();
// 					busying=false;
					return -3;
				}  
				if (RecvData()==1)
				{
					SaveRecvData();
					WORD *n=(WORD *)&debuffer[10];
					if (*n>0)
					{
				     mystrcpy(out+nn,(char*)&debuffer[12]);
					 //strcpy(out,&debuffer[12]);
					 //strcat_s(out+nn,1024*1024,&debuffer[12]);
					 //CopyMemory(&debuffer[12],out+nn,*n);
					}
					nn+=*n;
					ret=1;
				}
				else
				{
					ret=-7;
				//	break;
				}
			}
		}
		else
			ret=-6;
	}

// 	busying=false;
	return ret;
}
