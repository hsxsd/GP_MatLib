
/*
private void button23_Click(object sender, EventArgs e)
{
byte[] a ={ 0x0C, 0x0B, 0x04, 0x0A, 0x00, 0x01, 0x14, 0x00, 0x14, 0x00, 0x3D, 0x05, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x14, 0x00, 0x01, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
a[12] = 0xb;//$0:=上A $1=上B  $2:=深A $3:=深B $4=上债  $5=深债 $6=所有A股  $7=所有B股 $8=沪深债券  $9=沪深基金  $A=所有 $B:=指数  $C:=中小 $E:=创业
a[14] = 6;
a[20] = 1;// 0=不排序 1=升序 2=降序
/*

名称'1
最新'6
现手'b
买价'19
卖价'1A
买量'17
卖量'18
成交量'09
涨跌'0C
持仓量'11
开盘'3
最高'4
最低'5
昨结'5A
幅度%'0E

排序类型
2:=昨收
3:=今开
4:=最高
5:=最低
6:=现价
7:=买入
8:=卖出
9:=总量
10:=总额
11:=现量
12:=涨跌
14:=涨幅
15:=振幅
16=均价
17:=市盈率
19:=内盘
20:=外盘
0x15=内外比
0x17=买量一
0x18=卖量一
48=每笔均量
36:=换手率
37:=流通股
0x2e=涨速
0x23=量比
0x25=流通股本
0x26=流通市值
0x27=AB股票总市值
0x2d=强弱度
0x2f=活跃度
0x30=每笔均量
0x31=每笔换手
0x19=开盘涨幅
0x1A=最高涨幅
0x1B=最低涨幅
0x1C=均涨幅
0x1D=实体涨幅
0x1E=回头波
0x1F=攻击波
0x59=结算价
0x6b=仓差


short start = 0;//开始
short count = 50;//数量
byte[] sb1 = BitConverter.GetBytes(start);
a[16] = sb1[0];
a[17] = sb1[1];
sb1 = BitConverter.GetBytes(count);
a[18] = sb1[0];
a[19] = sb1[1];
if (Command(a, a.Length))
{
// SaveRecvData();
RecvBr.ReadInt16();
int n = RecvBr.ReadInt16();
string s1;
List1.Items.Clear();
int t = 0;
int i = 4;
int dd;
byte[] code = new byte[6];
for (int j = 0; j < n; j++)
{
byte m = RecvBuffer[i];
Array.Copy(RecvBuffer, i + 1, code, 0, 6);
i = i + 7;
dd = TDXGetInt16(RecvBuffer, i, ref i);

double prize = TDXDecode(RecvBuffer, i, ref i) / 100.0;
double last = prize + (((double)TDXDecode(RecvBuffer, i, ref i)) / 100.0);
double open = prize + (((double)TDXDecode(RecvBuffer, i, ref i)) / 100);
double high = prize + (((double)TDXDecode(RecvBuffer, i, ref i)) / 100);
double low = prize + (((double)TDXDecode(RecvBuffer, i, ref i)) / 100);
int Time = TDXDecode(RecvBuffer, i, ref i);
dd = TDXDecode(RecvBuffer, i, ref i);
int volume = TDXDecode(RecvBuffer, i, ref i);
int tradeQTY = TDXDecode(RecvBuffer, i, ref i);// '现量;
double amount = TDXGetDouble(RecvBuffer, i, ref i);
int b = TDXDecode(RecvBuffer, i, ref i);
int s = TDXDecode(RecvBuffer, i, ref i);

dd = TDXDecode(RecvBuffer, i, ref i);
dd = TDXDecode(RecvBuffer, i, ref i);

double buy1 = prize + (((double)TDXDecode(RecvBuffer, i, ref i)) / 100);
double sell1 = prize + (((double)TDXDecode(RecvBuffer, i, ref i)) / 100);
int buyQTY1 = TDXDecode(RecvBuffer, i, ref  i);
int sellQTY1 = TDXDecode(RecvBuffer, i, ref  i);

double buy2 = prize + (((double)TDXDecode(RecvBuffer, i, ref i)) / 100);
double sell2 = prize + (((double)TDXDecode(RecvBuffer, i, ref i)) / 100);
int buyQTY2 = TDXDecode(RecvBuffer, i, ref  i);
int sellQTY2 = TDXDecode(RecvBuffer, i, ref  i);

double buy3 = prize + (((double)TDXDecode(RecvBuffer, i, ref i)) / 100);
double sell3 = prize + (((double)TDXDecode(RecvBuffer, i, ref i)) / 100);
int buyQTY3 = TDXDecode(RecvBuffer, i, ref  i);
int sellQTY3 = TDXDecode(RecvBuffer, i, ref  i);
double buy4 = prize + (((double)TDXDecode(RecvBuffer, i, ref i)) / 100);
double sell4 = prize + (((double)TDXDecode(RecvBuffer, i, ref i)) / 100);
int buyQTY4 = TDXDecode(RecvBuffer, i, ref  i);
int sellQTY4 = TDXDecode(RecvBuffer, i, ref  i);
double buy5 = prize + (((double)TDXDecode(RecvBuffer, i, ref i)) / 100);
double sell5 = prize + (((double)TDXDecode(RecvBuffer, i, ref i)) / 100);
int buyQTY5 = TDXDecode(RecvBuffer, i, ref  i);
int sellQTY5 = TDXDecode(RecvBuffer, i, ref  i);
dd = TDXDecode(RecvBuffer, i, ref t);
dd = TDXDecode(RecvBuffer, t, ref t);
dd = TDXDecode(RecvBuffer, t, ref t);
i = i + 3;
dd = TDXDecode(RecvBuffer, i, ref i);
dd = TDXDecode(RecvBuffer, i, ref i);
dd = TDXDecode(RecvBuffer, i, ref i);
double speed = TDXGetInt16(RecvBuffer, i, ref i) / 100;
TDXGetInt16(RecvBuffer, i, ref i);
List1.Items.Add("===============================");
s1 = " 时间:" + Time.ToString() + " 代码:" + System.Text.Encoding.GetEncoding("GB2312").GetString(code);
List1.Items.Add(s1);
s1 = "──────────────";
List1.Items.Add(s1); ;
s1 = " 卖五:" + String.Format("{0:F4}", sell5) + "  " + String.Format("{0:d2}", sellQTY5);
List1.Items.Add(s1); ;
s1 = " 卖四:" + String.Format("{0:F4}", sell4) + "  " + String.Format("{0:d2}", sellQTY4);
List1.Items.Add(s1); ;
s1 = " 卖三:" + String.Format("{0:F4}", sell3) + "  " + String.Format("{0:d2}", sellQTY3);
List1.Items.Add(s1); ;
s1 = " 卖二:" + String.Format("{0:F4}", sell2) + "  " + String.Format("{0:d2}", sellQTY2);
List1.Items.Add(s1); ;
s1 = " 卖一:" + String.Format("{0:F4}", sell1) + "  " + String.Format("{0:d2}", sellQTY1);
List1.Items.Add(s1); ;
s1 = "──────────────";
List1.Items.Add(s1); ;
s1 = " 买一:" + String.Format("{0:F4}", buy1) + "  " + String.Format("{0:d2}", buyQTY1);
List1.Items.Add(s1); ;
s1 = " 买二:" + String.Format("{0:F4}", buy2) + "  " + String.Format("{0:d2}", buyQTY2);
List1.Items.Add(s1); ;
s1 = " 买三:" + String.Format("{0:F4}", buy3) + "  " + String.Format("{0:d2}", buyQTY3);
List1.Items.Add(s1); ;
s1 = " 买四:" + String.Format("{0:F4}", buy4) + "  " + String.Format("{0:d2}", buyQTY4);
List1.Items.Add(s1); ;
s1 = " 买五:" + String.Format("{0:F4}", buy5) + "  " + String.Format("{0:d2}", buyQTY5);
List1.Items.Add(s1); ;
s1 = "──────────────";
List1.Items.Add(s1); ;

s1 = " 昨收:" + String.Format("{0:F4}", last);
List1.Items.Add(s1); ;
s1 = " 今开:" + String.Format("{0:F4}", open);
List1.Items.Add(s1); ;
s1 = " 最高:" + String.Format("{0:F4}", high);
List1.Items.Add(s1); ;
s1 = " 最低:" + String.Format("{0:F4}", low);
List1.Items.Add(s1); ;
s1 = " 收盘:" + String.Format("{0:F4}", prize);
List1.Items.Add(s1); ;
; s1 = " 成交量:" + String.Format("{0:F4}", volume);
List1.Items.Add(s1); ;
s1 = " 成交额:" + String.Format("{0:F4}", amount);
List1.Items.Add(s1); ;
s1 = " 现量:" + String.Format("{0:F4}", tradeQTY);
List1.Items.Add(s1); ;
s1 = " 买盘:" + String.Format("{0:F4}", b);
List1.Items.Add(s1); ;
s1 = " 卖盘:" + String.Format("{0:F4}", s);
List1.Items.Add(s1);
}

}

}
*/