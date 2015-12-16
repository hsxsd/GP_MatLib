clear;
clc;
hfile = [matlabroot '\work\stock.h'];
loadlibrary('stock', hfile);
%libfunctionsview stock
%libfunctions stock -full;
%查看可调用函数
%libfunctions('stock');

fprintf('连接服务器...\n');
lk=calllib('stock','link');

if lk==-6 
fprintf('服务器地址失效,请联系QQ6523766.\n');
unloadlibrary stock;
return;
end
if lk>0
fprintf('最后日期:%d\n',lk);
else
fprintf('连接失败，请检查网络:%d.\n',lk);
unloadlibrary stock;
return;
end

%取股票报价
code='IF1505';
mark=47; %0x27 股指
mk='深圳';
if mark==47 
   mk='股指';
end
rt=calllib('stock','stockinfo',mark,code);
if rt==1 
    A=[];
    for m = 0:37
        k=calllib('stock','getstockindex',m);
        A=[A,k];
    end
    fprintf('%s 股票代码:%s\n',mk,code);
    fprintf('昨收价:%f\n',A(2)/1000);
    fprintf('开盘价:%f\n',A(3)/1000);
    fprintf('最高价:%f\n',A(4)/1000);
    fprintf('最低价:%f\n',A(5)/1000);
    fprintf('最新价:%f\n',A(6)/1000);
    fprintf('外盘:%d\n',A(12));
    fprintf('内盘:%d\n',A(13));
    fprintf('成交量:%d\n',A(9));
    fprintf('现量:%d\n',A(10));
    fprintf('持仓:%d\n',A(15));
    fprintf('开仓:%d\n',A(7));
    fprintf('仓差:%d\n',A(7)-A(9));
    fprintf('平仓:%d\n',A(7)-(A(7)-A(9))*2);
    fprintf('结算价:%f\n',A(36)/1000);
    fprintf('均价:%f\n',A(37)/1000);
    fprintf('==============================\n');
    fprintf('卖五:%f  卖五量:%d\n',A(30)/1000,A(35));
    fprintf('卖四:%f  卖四量:%d\n',A(29)/1000,A(34));
    fprintf('卖三:%f  卖三量:%d\n',A(28)/1000,A(33));
    fprintf('卖二:%f  卖二量:%d\n',A(27)/1000,A(32));
    fprintf('卖一:%f  卖一量:%d\n',A(26)/1000,A(31));
    fprintf('==============================\n');
    fprintf('买一:%f  买一量:%d\n',A(16)/1000,A(21));
    fprintf('买二:%f  买二量:%d\n',A(17)/1000,A(22));
    fprintf('买三:%f  买三量:%d\n',A(18)/1000,A(23));
    fprintf('买四:%f  买四量:%d\n',A(19)/1000,A(24));
    fprintf('买五:%f  买五量:%d\n',A(20)/1000,A(25));
    fprintf('==============================\n');
   
else
    fprintf('取%s报价失败! [%d]\n',code,rt);
end

%取历史数据
rt=calllib('stock','getstockhistory',mark,code,4);
if rt>0 
   fprintf('成功取得%s %s日线历史数据 长度: %d\n',mk,code,rt);
   B=cell(rt,10);
    for m = 0:rt-1
        for n=0:8
            k=calllib('stock','getstockdayindex',m,n);
            B{m+1,n+1}=k;
        end
    end
    for m=1:rt
       fprintf('日期:%d 时间:%d 开盘:%f 最高:%f 最低:%f 收盘:%f 持仓量:%ld 成交量:%d 结算价:%d\n',B{m,1},B{m,2},B{m,3}/1000,B{m,4}/1000,B{m,5}/1000,B{m,6}/1000,B{m,7},B{m,8},B{m,9});
    end
    %fprintf('%d:%d =%d\n',m,n,k);
else
    fprintf('取%s日线历史数据失败! [%d]\n',code,rt);
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
fprintf('关闭连接:%d\n',rt);
return;


