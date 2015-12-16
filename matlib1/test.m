clear;
clc;
hfile = [matlabroot '\work\stock.h'];
loadlibrary('stock', hfile);
%libfunctionsview stock
%libfunctions stock -full;
%�鿴�ɵ��ú���
%libfunctions('stock');

fprintf('���ӷ�����...\n');
lk=calllib('stock','link');

if lk==-6 
fprintf('��������ַʧЧ,����ϵQQ6523766.\n');
unloadlibrary stock;
return;
end
if lk>0
fprintf('�������:%d\n',lk);
else
fprintf('����ʧ�ܣ���������:%d.\n',lk);
unloadlibrary stock;
return;
end

%ȡ��Ʊ����
code='IF1505';
mark=47; %0x27 ��ָ
mk='����';
if mark==47 
   mk='��ָ';
end
rt=calllib('stock','stockinfo',mark,code);
if rt==1 
    A=[];
    for m = 0:37
        k=calllib('stock','getstockindex',m);
        A=[A,k];
    end
    fprintf('%s ��Ʊ����:%s\n',mk,code);
    fprintf('���ռ�:%f\n',A(2)/1000);
    fprintf('���̼�:%f\n',A(3)/1000);
    fprintf('��߼�:%f\n',A(4)/1000);
    fprintf('��ͼ�:%f\n',A(5)/1000);
    fprintf('���¼�:%f\n',A(6)/1000);
    fprintf('����:%d\n',A(12));
    fprintf('����:%d\n',A(13));
    fprintf('�ɽ���:%d\n',A(9));
    fprintf('����:%d\n',A(10));
    fprintf('�ֲ�:%d\n',A(15));
    fprintf('����:%d\n',A(7));
    fprintf('�ֲ�:%d\n',A(7)-A(9));
    fprintf('ƽ��:%d\n',A(7)-(A(7)-A(9))*2);
    fprintf('�����:%f\n',A(36)/1000);
    fprintf('����:%f\n',A(37)/1000);
    fprintf('==============================\n');
    fprintf('����:%f  ������:%d\n',A(30)/1000,A(35));
    fprintf('����:%f  ������:%d\n',A(29)/1000,A(34));
    fprintf('����:%f  ������:%d\n',A(28)/1000,A(33));
    fprintf('����:%f  ������:%d\n',A(27)/1000,A(32));
    fprintf('��һ:%f  ��һ��:%d\n',A(26)/1000,A(31));
    fprintf('==============================\n');
    fprintf('��һ:%f  ��һ��:%d\n',A(16)/1000,A(21));
    fprintf('���:%f  �����:%d\n',A(17)/1000,A(22));
    fprintf('����:%f  ������:%d\n',A(18)/1000,A(23));
    fprintf('����:%f  ������:%d\n',A(19)/1000,A(24));
    fprintf('����:%f  ������:%d\n',A(20)/1000,A(25));
    fprintf('==============================\n');
   
else
    fprintf('ȡ%s����ʧ��! [%d]\n',code,rt);
end

%ȡ��ʷ����
rt=calllib('stock','getstockhistory',mark,code,4);
if rt>0 
   fprintf('�ɹ�ȡ��%s %s������ʷ���� ����: %d\n',mk,code,rt);
   B=cell(rt,10);
    for m = 0:rt-1
        for n=0:8
            k=calllib('stock','getstockdayindex',m,n);
            B{m+1,n+1}=k;
        end
    end
    for m=1:rt
       fprintf('����:%d ʱ��:%d ����:%f ���:%f ���:%f ����:%f �ֲ���:%ld �ɽ���:%d �����:%d\n',B{m,1},B{m,2},B{m,3}/1000,B{m,4}/1000,B{m,5}/1000,B{m,6}/1000,B{m,7},B{m,8},B{m,9});
    end
    %fprintf('%d:%d =%d\n',m,n,k);
else
    fprintf('ȡ%s������ʷ����ʧ��! [%d]\n',code,rt);
end;

p11=1:300;
ap1=p11;
c_3=libpointer('int32Ptr',ap1);
rt=calllib('stock','getlist',c_3,20);
for m = 1:19
    fprintf('%d:%d\n',m+1,p11(m));
end;

rt=calllib('stock','close');
unloadlibrary stock;
fprintf('�ر�����:%d\n',rt);
return;


