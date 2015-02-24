%analytical result of the number of trips in a day
rho=0.6;
gamma=0.21;
load PT.txt %144 slots, 
parameters = load('DTparameters.txt');
K=48; %number of time slots
m=size(parameters,1);
locnum=20; %max number of daily locations
n1_arr=[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20];
n2_arr=[1, 6, 11, 16, 21, 26, 31, 36, 41, 46, 51, 56, 61, 66, 71, 76, 81, 86, 91, 96, 101];
ptmat=zeros(1,K);
slotratio=3;
pn=zeros(m,locnum);
for j=1:K
    ptmat(j)=PT(slotratio*j-2)+PT(slotratio*j-1)+PT(slotratio*j);
end
%initial state
xi=zeros(1,2*K-2);
xi(1)=1;
for p=1:m
    if mod(p,100)==0
        p
    end
    pt=ptmat(:).*parameters(p,3)*1;
    n1=n1_arr(parameters(p,1)+1);
    n2=n2_arr(parameters(p,2)+1);
    lambda=zeros(2*K-2,2*K-2,K);
    %time step
    for i=1:K
        p1=n1*pt(i);
        if p1>1
            p1=1;
        end
        p2=n2*pt(i)/3;
        if p2>1
            p2=1;
        end
        %the first row
        lambda(1,1,i)=1-pt(i);
        lambda(1,3,i)=pt(i);
        %the middle rows
        for j=1:K-3
            pnew=rho*j^(-gamma) / (0.6*(1-rho*j^(-gamma)) + rho*j^(-gamma)); 
           
            lambda(2*j,2*j,i)=1-pt(i);
            lambda(2*j,2*j+1,i)=pt(i) * (1-pnew);
            lambda(2*j,2*j+3,i)=pt(i)*pnew;
            
            
            lambda(2*j+1,2*j,i)=p1*(1-p2);
            lambda(2*j+1,2*j+1,i)=1-p1 + p1*p2*(1-pnew);
            lambda(2*j+1,2*j+3,i)=p1*p2*pnew;
        end
        %the last three rows
        pnew=1-0.6*(1-rho*(K-2)^(-gamma));
        lambda(2*K-4,2*K-4,i)=1;
        lambda(2*K-3,2*K-4,i)=p1*(1-p2);
        lambda(2*K-3,2*K-3,i)=1-p1+p1*p2*(1-pnew);
        lambda(2*K-3,2*K-2,i)=p1*p2*pnew;
        lambda(2*K-2,2*K-2,i)=1;
    end
    
    %lambda product
    lambdaprod=lambda(:,:,1);
    for i=2:K
        lambdaprod=lambdaprod*lambda(:,:,i);
    end
    %number of daily visited location N
    for N=1:locnum
        U=zeros(2*K-2,1);
        if N==1
            U(1)=1;
        else
            U(2*N-2)=1;
            U(2*N-1)=1;
        end
        pn(p,N)=xi*lambdaprod*U;
    end
end
pn=sum(pn)./sum(sum(pn));
save pn pn

%analytical result of the number of trips in a day
m=size(parameters,1);
pn=zeros(m,locnum);
%initial state
xi=zeros(1,2*K-2);
xi(1)=1;
for p=1:m
    if mod(p,100)==0
        p
    end
    pt=ptmat(:).*parameters(p,3)*1;
    n1=1;
    n2=n2_arr(parameters(p,2)+1);
    lambda=zeros(2*K-2,2*K-2,K);
    %time step
    for i=1:K
        p1=n1*pt(i);
        if p1>1
            p1=1;
        end
        p2=n2*pt(i)/3;
        if p2>1
            p2=1;
        end
        %the first row
        lambda(1,1,i)=1-pt(i);
        lambda(1,3,i)=pt(i);
        %the middle rows
        for j=1:K-3
           % pnew=1-0.65*(1-rho*j^(-gamma));
            pnew=rho*j^(-gamma) / (0.6*(1-rho*j^(-gamma)) + rho*j^(-gamma)); 
           
            lambda(2*j,2*j,i)=1-pt(i);
            lambda(2*j,2*j+1,i)=pt(i) * (1-pnew);
            lambda(2*j,2*j+3,i)=pt(i)*pnew;
            
            
            lambda(2*j+1,2*j,i)=p1*(1-p2);
            lambda(2*j+1,2*j+1,i)=1-p1 + p1*p2*(1-pnew);
            lambda(2*j+1,2*j+3,i)=p1*p2*pnew;
        end
        %the last three rows
        pnew=1-0.6*(1-rho*(K-2)^(-gamma));
        lambda(2*K-4,2*K-4,i)=1;
        lambda(2*K-3,2*K-4,i)=p1*(1-p2);
        lambda(2*K-3,2*K-3,i)=1-p1+p1*p2*(1-pnew);
        lambda(2*K-3,2*K-2,i)=p1*p2*pnew;
        lambda(2*K-2,2*K-2,i)=1;
    end
    
    %lambda product
    lambdaprod=lambda(:,:,1);
    for i=2:K
        lambdaprod=lambdaprod*lambda(:,:,i);
    end
    %number of daily visited location N
    for N=1:locnum
        U=zeros(2*K-2,1);
        if N==1
            U(1)=1;
        else
            U(2*N-2)=1;
            U(2*N-1)=1;
        end
        pn(p,N)=xi*lambdaprod*U;
    end
end
pn_non1=sum(pn)./sum(sum(pn));
save pn_non1 pn_non1



