typedef unsigned char byte;

#pragma pack(1) 
// 接收数据头
struct RecvDataHeader
{
	DWORD CheckSum;
	byte EncodeMode;
	char tmp[5];
	WORD msgid;
	WORD Size;
	WORD DePackSize;
};
//发送数据头
struct SendDataHeader
{
	byte CheckSum;//0x0c
	char tmp[4];
	WORD size1,size2;//size1=size2
};
// 通达信历史日线数据
struct tdx_his
{
	long int date;
	long int open,high,low,close;
	float amount;
	int vol;
	int res;
};
// 公司资料原始数据
struct TDX_STOCKINFO // 初始化数据 29字节
{
	char code[6];//代码
	WORD rate;// 实时盘口中的成交量除去的除数？1手=n股？
	char name[8];//名称
	WORD w1,w2;
	BYTE PriceMag;//小数点位数
	float YClose;//昨收
	WORD w3,w4;
};
struct CaiWu
{
	byte Mark;
	char code[6];
	float LTG;//流通股数量
	WORD t1,t2;
	DWORD day1,day2;
	float zl[30];
};

// 分时图数据
struct tdx_min
{
	WORD min;//时间
	float close; //成交价
	DWORD volume;//成交量
};

// 分笔历史成交数据
struct tdx_dealinfo
{
	int time;// h *60 + Min
	float close,volume;
	WORD sellorbuy;//Sell=1 buy=0
	WORD dealcount;//笔数
};
// 公司资料分类数据
struct tdx_infostyle
{
	char name[8];
	char code[6];
	int start,len;
};
// 日K线数据
struct tdx_dayinfo
{
	DWORD day,time;
	float open,high,low,close;
	float amount;
	DWORD volume;
	WORD upCount,downcount;//指数时有用
};


//期货基本资料
struct tdx_qhinfo
{
	byte t1,t2,t3,t4,t5;//t2=市场　t3=小数点位数
	char code[9];
	char name[17];
	char code1[11];
	char res[22];
};
//期货日线
struct qh_dayinfo
{
	DWORD day; //  20100920格式
	float open,high,low,close;//开盘 最高 最低 收盘
	DWORD chicang;//持仓
	DWORD volume; //成交
	float js;//结算
};

struct mat_dayinfo
{
	int day; //日期  20100920格式
	int time;//时间
	int open,high,low,close;//开盘 最高 最低 收盘
	int chicang;//持仓
	int  volume; //成交
	int js;//结算
};

//期货分钟
struct qh_min
{
	WORD min;
	float close;
	float arg;//均价
	int volume;//成交量
	DWORD amount;
};
//期货分笔
struct qh_deal
{
	WORD min;//h *60 + Min
	DWORD close;//价格*1000
	DWORD volume;//现量
	int DealCount;//增仓
	WORD dealtype;
};
//期货报价
struct mat_pk 
{
	int dealcount;
	int last,open,high,low,close;//last昨结
	int b,s;//b=开仓
	int volume;//总量
	int tradeQTY;
	int  res2;
	int nei,wai,res5;
	int chicang;//持仓
	int buy1,buy2,buy3,buy4,buy5;
	int buyQTY1,buyQTY2,buyQTY3,buyQTY4,buyQTY5;
	int sell1,sell2,sell3,sell4,sell5;
	int sellQTY1,sellQTY2,sellQTY3,sellQTY4,sellQTY5;
	int js;//结算
	int jj;//均价
	int jj1;
};

struct qh_pk1
{
	byte marketmode;
	char code[7];
	WORD res1;
	DWORD dealcount;
	float last,open,high,low,close;//last昨结
	DWORD b,s;//b=开仓
	DWORD volume;//总量
	DWORD tradeQTY;
	DWORD  res2;
	DWORD nei,wai,res5;
	DWORD chicang;//持仓
	float buy1,buy2,buy3,buy4,buy5;
	DWORD buyQTY1,buyQTY2,buyQTY3,buyQTY4,buyQTY5;
	float sell1,sell2,sell3,sell4,sell5;
	DWORD sellQTY1,sellQTY2,sellQTY3,sellQTY4,sellQTY5;

	WORD  res6;
	float js;//结算
	DWORD res7;
	float jj;//均价
	float jj1;
	DWORD res8,res9;
	DWORD res10[8];
	byte  cc[92];
};

struct mc 
{
	byte mark;
	char code[9];
};

#pragma pack() 

extern "C" __declspec(dllexport) int link();//连接服务器
extern "C" __declspec(dllexport) int linkServer(char *server,int port);//连接指定服务器，server=ip地址 port=端口
extern "C" __declspec(dllexport) int savecodelist();
extern "C" __declspec(dllexport) int linkclose();//关闭连接
extern "C" __declspec(dllexport) int getlist(double *list,int count);
extern "C" __declspec(dllexport) int gethistory(double *out, int mark,char *code,int count,int zq);//读取历史数据,mark=市场(0=深圳，1=上海） code=代码 count=数量（从后往前计算，最大 800） zq=周期
extern "C" __declspec(dllexport) int stockpkbase(double *out,int mark,char *code);//读取实时报价,mark=市场 code=代码 
extern "C" __declspec(dllexport) int stockcaiwu(double *out,int mark,char *code);//读取财务数据,mark=市场 code=代码
extern "C" __declspec(dllexport) int stockquan(double *out,int mark,char *code);//读取权息数据,mark=市场 code=代码
extern "C" __declspec(dllexport) int checklink();
extern "C" __declspec(dllexport) int getiopv(int mark,char *code,double *out);
extern "C" __declspec(dllexport) int getinfo(int mark,char *code,int type,char *out);



