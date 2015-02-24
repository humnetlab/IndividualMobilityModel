%input: moment calculation result  MomentCalculation.txt
%output: 
%home occupy probability of the beta cascade PD, 
%other occupy probability of the beta cascade PS, 
%home standard error of the lognormal cascade SigmaD, 
%other standard error of the lognormal cascade  SigmaS
%correlation coefficient rho

load MomentCalculation.txt%each column is a different moment, each row is a granularity
%moment calculation for home location
%estimate local values of VD and PD
moments = flipud(MomentCalculation(1:9,:));
pNum=15;
K0=zeros(1,8);%for different n
for i=1:8
    K0(i)=log2(moments(i+1,5))-log2(moments(i,5));
end
K2=zeros(1,8);
for i=1:8
    K2(i)=log2(moments(i+1,9))-log2(moments(i,9));
end
PD=2.^K0;
VD=K2+K0;
SigmaD=sqrt(VD*log(2));

%moment calculation for other location
%estimate local values of VS and PS
moments = flipud(MomentCalculation(19:27,:));
K0=zeros(1,8);%for different n
for i=1:8
    K0(i)=log2(moments(i+1,5))-log2(moments(i,5));
end
%q=2 is the 9 th
K2=zeros(1,8);
for i=1:8
    K2(i)=log2(moments(i+1,9))-log2(moments(i,9));
end
PS=2.^K0;
VS=K2+K0;
SigmaS=sqrt(VS*log(2));

%estimate correlation coefficient rho
%first 4, pure log normal, last 3, pure beta
%level 5 is a mix, so use the average value of level 4 and 6 to approximate
rho=zeros(1,8);
%from  MeanSDProd.txt
meanSDProd=load('MeanSDProd.txt');
meanSDProd=fliplr(meanSDProd);
for i=1:4
    %rho(i)=(meanSDProd(i+1)/meanSDProd(i)-1)/(SigmaS(i)*SigmaD(i));
    rho(i)=log(sqrt(meanSDProd(i+1)/meanSDProd(i)-1)+1)^2/(SigmaS(i)*SigmaD(i));
end
for i=6:8
    rho(i)=(meanSDProd(i+1)/meanSDProd(i)-1)/(sqrt((1-PD(i))/PD(i))*sqrt((1-PS(i))/PS(i)));
end
rho(5)=(rho(4)+rho(6))/2;



