%P(k)
figure()
real = load('RankProbability.txt');
m = max(size(real));
real = [1:m;real];
simu = load('PrSimu.txt');
unitBin=10;
PowerLawPlot(simu(:,2),'s-','g',0,999999,unitBin);
hold on
PowerLawPlotWeighted(real(:,1:end)','o-','b',0,999999,unitBin);
xlim([10^1 10^5]);
ylim([10^-6 10^-3])
set(gcf, 'Position', [100 100 300 300])
set(gca,'XTick',[10^-1,10^0,10^1,10^2,10^3,10^4,10^5])
xlabel('k','FontName','Times New Roman','FontSize',10)
ylabel('P(k)','FontName','Times New Roman','FontSize',10)
set(gca,'ticklength',3*get(gca,'ticklength'))
hh =legend('Data','Simu','location','southwest')
legend('boxoff')

%P(r)
figure()
PrData=load('PrData.txt');
PrSimu=load('PrSimu.txt');
PrData=PrData(:,1);
PrSimu=PrSimu(:,1);
unitBin=12;
PowerLawPlot(PrSimu,'s-','g',0.7,100,unitBin);
hold on
PowerLawPlot(PrData,'o-','b',0.7,100,unitBin);
set(gcf, 'Position', [100 100 300 300])
xlim([10^-0.2 10^2])
ylim([10^-5 10^0.2])
set(gca,'XTick',[10^-1,10^0,10^1,10^2,10^3])
set(gca,'YTick',[10^-7,10^-6,10^-5,10^-4,10^-3,10^-2,10^-1,10^0,10^1,10^2,10^3])
set(gca,'FontName','Times New Roman','FontSize',10)
xlabel('r, km','FontName','Times New Roman','FontSize',10)
ylabel('P(r)','FontName','Times New Roman','FontSize',10)
set(gca,'ticklength',3*get(gca,'ticklength'))
legend('Simu','Data','location','southwest')
legend('boxoff')

%PI
figure()
sigma_wd=SigmaD;
sigma_ws=SigmaS;
rho_ln=rho; 
s0=180000;
alpha=0.86; %the slope of rank choosing probability
beta=(1-alpha)/(s0^(1-alpha)-1);
%the escaping probability at each tile
pd=zeros(1,4);
for tile =1:4
    sigma_s2=0;
    sigma_d2=0;
    for i=1:tile
        sigma_s2=sigma_s2+sigma_ws(i)^2;
        sigma_d2=sigma_d2+sigma_wd(i)^2;
    end
    sigma_s=sqrt(sigma_s2);
    sigma_d=sqrt(sigma_d2);
    rho_level=0;
    for i=1:tile
        rho_level=rho_level+rho_ln(i)*sigma_ws(i)*sigma_wd(i);
    end
    rho_level=rho_level/(sigma_s*sigma_d);
    ms=log(s0*4^(-tile))-0.5*sigma_s2;
    %mean and variance of ln(Sn,trip)
    mu=ms+rho_level*sigma_s*sigma_d;
    sigma=sigma_s;
    
    gamma1=(1-alpha+beta)/(1-alpha);
    gamma2=beta/(1-alpha)/sqrt(2*3.1415926)/sigma;
    x3=gamma2*sqrt(3.1415926/2)*exp(0.5*(alpha-1)*((alpha-1)*sigma*sigma-2*mu))*( erf(((alpha-1)*sigma*sigma-mu+log(s0))/(1.414*sigma)) - erf(((alpha-1)*sigma*sigma-mu+0)/(1.414*sigma)));
    x2=gamma1*(0.5+0.5*erf((log(s0)-mu)/(1.414*sigma))) - gamma1*(0.5+0.5*erf((0-mu)/(1.414*sigma))); %lognormal cdf
    x1=x2+x3;
    x11=exp(-mu*mu / (2*sigma^2) ) * ( beta* exp((mu-(alpha-1)*sigma^2)^2/(2*sigma^2)) *erf( ((alpha-1)*sigma^2-mu+log(s0)) / (1.414*sigma)) - (alpha-beta-1) * exp(mu^2/(2*sigma^2)) * erf((mu-log(s0))/(1.414*sigma)) );
    x12=exp(-mu*mu / (2*sigma^2) ) * ( beta* exp((mu-(alpha-1)*sigma^2)^2/(2*sigma^2)) *erf( ((alpha-1)*sigma^2-mu+log(1)) / (1.414*sigma)) - (alpha-beta-1) * exp(mu^2/(2*sigma^2)) * erf((mu-log(1))/(1.414*sigma)) );
    pd(tile)=(x11-x12)/(2*(alpha-1));
end

data=load('PI.txt');
simu=load('PISimu.txt');
data_pd = data(:,2)'./sum(data');
simu_pd = simu(:,2)'./sum(simu');

plot([0:4],[0 pd(1:4)],'g^-');
hold on;
plot([0:8],[0 simu_pd(1:8)],'rs-')
plot([0:8],[0 data_pd(1:8)],'o-');
set(gcf, 'Position', [100 100 300 300])
set(gca,'FontName','Times New Roman','FontSize',10)
xlabel('tile level, i','FontName','Times New Roman','FontSize',10)
ylabel('P>(i)','FontName','Times New Roman','FontSize',10)
legend('Analytic','Simulation','Data','location','southeast')
legend('boxoff')
xlim([0 8])
set(gca,'XTick',[0,1,2,3,4,5,6,7,8])
