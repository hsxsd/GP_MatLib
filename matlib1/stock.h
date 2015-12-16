typedef unsigned char byte;

#pragma pack(1) 
// ��������ͷ
struct RecvDataHeader
{
	DWORD CheckSum;
	byte EncodeMode;
	char tmp[5];
	WORD msgid;
	WORD Size;
	WORD DePackSize;
};
//��������ͷ
struct SendDataHeader
{
	byte CheckSum;//0x0c
	char tmp[4];
	WORD size1,size2;//size1=size2
};
// ͨ������ʷ��������
struct tdx_his
{
	long int date;
	long int open,high,low,close;
	float amount;
	int vol;
	int res;
};
// ��˾����ԭʼ����
struct TDX_STOCKINFO // ��ʼ������ 29�ֽ�
{
	char code[6];//����
	WORD rate;// ʵʱ�̿��еĳɽ�����ȥ�ĳ�����1��=n�ɣ�
	char name[8];//����
	WORD w1,w2;
	BYTE PriceMag;//С����λ��
	float YClose;//����
	WORD w3,w4;
};
struct CaiWu
{
	byte Mark;
	char code[6];
	float LTG;//��ͨ������
	WORD t1,t2;
	DWORD day1,day2;
	float zl[30];
};

// ��ʱͼ����
struct tdx_min
{
	WORD min;//ʱ��
	float close; //�ɽ���
	DWORD volume;//�ɽ���
};

// �ֱ���ʷ�ɽ�����
struct tdx_dealinfo
{
	int time;// h *60 + Min
	float close,volume;
	WORD sellorbuy;//Sell=1 buy=0
	WORD dealcount;//����
};
// ��˾���Ϸ�������
struct tdx_infostyle
{
	char name[8];
	char code[6];
	int start,len;
};
// ��K������
struct tdx_dayinfo
{
	DWORD day,time;
	float open,high,low,close;
	float amount;
	DWORD volume;
	WORD upCount,downcount;//ָ��ʱ����
};


//�ڻ���������
struct tdx_qhinfo
{
	byte t1,t2,t3,t4,t5;//t2=�г���t3=С����λ��
	char code[9];
	char name[17];
	char code1[11];
	char res[22];
};
//�ڻ�����
struct qh_dayinfo
{
	DWORD day; //  20100920��ʽ
	float open,high,low,close;//���� ��� ��� ����
	DWORD chicang;//�ֲ�
	DWORD volume; //�ɽ�
	float js;//����
};

struct mat_dayinfo
{
	int day; //����  20100920��ʽ
	int time;//ʱ��
	int open,high,low,close;//���� ��� ��� ����
	int chicang;//�ֲ�
	int  volume; //�ɽ�
	int js;//����
};

//�ڻ�����
struct qh_min
{
	WORD min;
	float close;
	float arg;//����
	int volume;//�ɽ���
	DWORD amount;
};
//�ڻ��ֱ�
struct qh_deal
{
	WORD min;//h *60 + Min
	DWORD close;//�۸�*1000
	DWORD volume;//����
	int DealCount;//����
	WORD dealtype;
};
//�ڻ�����
struct mat_pk 
{
	int dealcount;
	int last,open,high,low,close;//last���
	int b,s;//b=����
	int volume;//����
	int tradeQTY;
	int  res2;
	int nei,wai,res5;
	int chicang;//�ֲ�
	int buy1,buy2,buy3,buy4,buy5;
	int buyQTY1,buyQTY2,buyQTY3,buyQTY4,buyQTY5;
	int sell1,sell2,sell3,sell4,sell5;
	int sellQTY1,sellQTY2,sellQTY3,sellQTY4,sellQTY5;
	int js;//����
	int jj;//����
	int jj1;
};

struct qh_pk1
{
	byte marketmode;
	char code[7];
	WORD res1;
	DWORD dealcount;
	float last,open,high,low,close;//last���
	DWORD b,s;//b=����
	DWORD volume;//����
	DWORD tradeQTY;
	DWORD  res2;
	DWORD nei,wai,res5;
	DWORD chicang;//�ֲ�
	float buy1,buy2,buy3,buy4,buy5;
	DWORD buyQTY1,buyQTY2,buyQTY3,buyQTY4,buyQTY5;
	float sell1,sell2,sell3,sell4,sell5;
	DWORD sellQTY1,sellQTY2,sellQTY3,sellQTY4,sellQTY5;

	WORD  res6;
	float js;//����
	DWORD res7;
	float jj;//����
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

extern "C" __declspec(dllexport) int link();//���ӷ�����
extern "C" __declspec(dllexport) int linkServer(char *server,int port);//����ָ����������server=ip��ַ port=�˿�
extern "C" __declspec(dllexport) int savecodelist();
extern "C" __declspec(dllexport) int linkclose();//�ر�����
extern "C" __declspec(dllexport) int getlist(double *list,int count);
extern "C" __declspec(dllexport) int gethistory(double *out, int mark,char *code,int count,int zq);//��ȡ��ʷ����,mark=�г�(0=���ڣ�1=�Ϻ��� code=���� count=�������Ӻ���ǰ���㣬��� 800�� zq=����
extern "C" __declspec(dllexport) int stockpkbase(double *out,int mark,char *code);//��ȡʵʱ����,mark=�г� code=���� 
extern "C" __declspec(dllexport) int stockcaiwu(double *out,int mark,char *code);//��ȡ��������,mark=�г� code=����
extern "C" __declspec(dllexport) int stockquan(double *out,int mark,char *code);//��ȡȨϢ����,mark=�г� code=����
extern "C" __declspec(dllexport) int checklink();
extern "C" __declspec(dllexport) int getiopv(int mark,char *code,double *out);
extern "C" __declspec(dllexport) int getinfo(int mark,char *code,int type,char *out);



