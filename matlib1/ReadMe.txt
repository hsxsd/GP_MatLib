
/*
private void button23_Click(object sender, EventArgs e)
{
byte[] a ={ 0x0C, 0x0B, 0x04, 0x0A, 0x00, 0x01, 0x14, 0x00, 0x14, 0x00, 0x3D, 0x05, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x14, 0x00, 0x01, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
a[12] = 0xb;//$0:=��A $1=��B  $2:=��A $3:=��B $4=��ծ  $5=��ծ $6=����A��  $7=����B�� $8=����ծȯ  $9=�������  $A=���� $B:=ָ��  $C:=��С $E:=��ҵ
a[14] = 6;
a[20] = 1;// 0=������ 1=���� 2=����
/*

����'1
����'6
����'b
���'19
����'1A
����'17
����'18
�ɽ���'09
�ǵ�'0C
�ֲ���'11
����'3
���'4
���'5
���'5A
����%'0E

��������
2:=����
3:=��
4:=���
5:=���
6:=�ּ�
7:=����
8:=����
9:=����
10:=�ܶ�
11:=����
12:=�ǵ�
14:=�Ƿ�
15:=���
16=����
17:=��ӯ��
19:=����
20:=����
0x15=�����
0x17=����һ
0x18=����һ
48=ÿ�ʾ���
36:=������
37:=��ͨ��
0x2e=����
0x23=����
0x25=��ͨ�ɱ�
0x26=��ͨ��ֵ
0x27=AB��Ʊ����ֵ
0x2d=ǿ����
0x2f=��Ծ��
0x30=ÿ�ʾ���
0x31=ÿ�ʻ���
0x19=�����Ƿ�
0x1A=����Ƿ�
0x1B=����Ƿ�
0x1C=���Ƿ�
0x1D=ʵ���Ƿ�
0x1E=��ͷ��
0x1F=������
0x59=�����
0x6b=�ֲ�


short start = 0;//��ʼ
short count = 50;//����
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
int tradeQTY = TDXDecode(RecvBuffer, i, ref i);// '����;
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
s1 = " ʱ��:" + Time.ToString() + " ����:" + System.Text.Encoding.GetEncoding("GB2312").GetString(code);
List1.Items.Add(s1);
s1 = "����������������������������";
List1.Items.Add(s1); ;
s1 = " ����:" + String.Format("{0:F4}", sell5) + "  " + String.Format("{0:d2}", sellQTY5);
List1.Items.Add(s1); ;
s1 = " ����:" + String.Format("{0:F4}", sell4) + "  " + String.Format("{0:d2}", sellQTY4);
List1.Items.Add(s1); ;
s1 = " ����:" + String.Format("{0:F4}", sell3) + "  " + String.Format("{0:d2}", sellQTY3);
List1.Items.Add(s1); ;
s1 = " ����:" + String.Format("{0:F4}", sell2) + "  " + String.Format("{0:d2}", sellQTY2);
List1.Items.Add(s1); ;
s1 = " ��һ:" + String.Format("{0:F4}", sell1) + "  " + String.Format("{0:d2}", sellQTY1);
List1.Items.Add(s1); ;
s1 = "����������������������������";
List1.Items.Add(s1); ;
s1 = " ��һ:" + String.Format("{0:F4}", buy1) + "  " + String.Format("{0:d2}", buyQTY1);
List1.Items.Add(s1); ;
s1 = " ���:" + String.Format("{0:F4}", buy2) + "  " + String.Format("{0:d2}", buyQTY2);
List1.Items.Add(s1); ;
s1 = " ����:" + String.Format("{0:F4}", buy3) + "  " + String.Format("{0:d2}", buyQTY3);
List1.Items.Add(s1); ;
s1 = " ����:" + String.Format("{0:F4}", buy4) + "  " + String.Format("{0:d2}", buyQTY4);
List1.Items.Add(s1); ;
s1 = " ����:" + String.Format("{0:F4}", buy5) + "  " + String.Format("{0:d2}", buyQTY5);
List1.Items.Add(s1); ;
s1 = "����������������������������";
List1.Items.Add(s1); ;

s1 = " ����:" + String.Format("{0:F4}", last);
List1.Items.Add(s1); ;
s1 = " ��:" + String.Format("{0:F4}", open);
List1.Items.Add(s1); ;
s1 = " ���:" + String.Format("{0:F4}", high);
List1.Items.Add(s1); ;
s1 = " ���:" + String.Format("{0:F4}", low);
List1.Items.Add(s1); ;
s1 = " ����:" + String.Format("{0:F4}", prize);
List1.Items.Add(s1); ;
; s1 = " �ɽ���:" + String.Format("{0:F4}", volume);
List1.Items.Add(s1); ;
s1 = " �ɽ���:" + String.Format("{0:F4}", amount);
List1.Items.Add(s1); ;
s1 = " ����:" + String.Format("{0:F4}", tradeQTY);
List1.Items.Add(s1); ;
s1 = " ����:" + String.Format("{0:F4}", b);
List1.Items.Add(s1); ;
s1 = " ����:" + String.Format("{0:F4}", s);
List1.Items.Add(s1);
}

}

}
*/