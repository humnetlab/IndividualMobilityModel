%P(N)
load pn
load pn_non1
dailyActivityReal = load('DailyNReal.txt');
[number_count2,number2]=hist(dailyActivityReal(:,1),unique(dailyActivityReal(:,1)));
dailyActivityModel = load('SimuLocInterpolated.txt');
index = find(dailyActivityModel(:,1)>0);
dailyActivityModel=dailyActivityModel(index,:);
dailyActivityModel(:,4)=floor(dailyActivityModel(:,4));
user_id=unique(dailyActivityModel(:,1));
daily_activity_count=zeros(100,1);
m_user=size(user_id,1);
for i=1:m_user
    i
    index=find(dailyActivityModel(:,1)==user_id(i));
    user_data=dailyActivityModel(index,:);
    day_id=unique(user_data(:,4));
    m_day=size(day_id,1);
    for j=1:m_day;
        index=find(user_data(:,4)==day_id(j));
        daily_activity_num=size(unique(user_data(index,5)),1);
        daily_activity_count(daily_activity_num)=daily_activity_count(daily_activity_num)+1;
    end
end

semilogy(2:20,pn(1:19)/sum(pn(1:19)),'g^-','markersize',5)
hold on
semilogy(2:20,daily_activity_count(2:20)/sum(daily_activity_count(2:20)),'rs-','markersize',5)
semilogy(number2(2:20),number_count2(2:20)/sum(number_count2(2:20)),'bo-','markersize',5)
semilogy(2:20,pn_non1(2:20)/sum(pn_non1(2:20)),'kv-','markersize',5)
set(gcf, 'Position', [100 100 300 300])
set(gca,'FontName','Times New Roman','FontSize',10)
xlabel('N','FontName','Times New Roman','FontSize',10)
ylabel('P(N)','FontName','Times New Roman','FontSize',10)
xlim([2 15])
ylim([10^-7 10^0])
set(gca,'YTick',[10^-7,10^-6,10^-5,10^-4,10^-3,10^-2,10^-1,1])
set(gca,'XTick',[0 5 10 15 20])
legend('Analytic','Simulation','Data','Analytic-No N1','location','southwest')
set(gca,'ticklength',3*get(gca,'ticklength'))
legend('boxoff')


%P(dt)
figure()
dtReal = load('DTReal.txt');
dtSimu = load('DTSimu.txt');
PowerLawPlot(dtSimu(:,1),'s','r',600,160000,12)
hold on
PowerLawPlot(dtReal(:,1),'o','b',600,160000,12)
set(gca,'FontName','Times New Roman','FontSize',10)
xlabel('dt, s','FontName','Times New Roman','FontSize',10)
ylabel('P(dt)','FontName','Times New Roman','FontSize',10)
set(gcf, 'Position', [100 100 300 300])
legend('Simu','Data','Location','southwest')
legend('boxoff')
ylim([10^-10 10^-3])
xlim([400 10^5.5])
set(gca,'XTick',[10^2,10^3,10^4,10^5,10^6])
set(gca,'YTick',[10^-10 10^-9, 10^-8, 10^-7,10^-6,10^-5,10^-4,10^-3,10^-2,10^-1,10^0,10^1,10^2,10^3])
set(gca,'ticklength',3*get(gca,'ticklength'))


%beta1 beta2
parameters = load('DTparameters.txt');
[number_count,number]=hist(parameters(:,1),unique(parameters(:,1)));
[number_count2,number2]=hist(parameters(:,2),unique(parameters(:,2)));
figure()
bar(number+1,number_count./sum(number_count),0.7)
xlim([0 21])
set(gcf, 'Position', [100 100 240 240])
ylim([0 0.15])
set(gca,'YTick',[0 0.05 0.10 0.15])
set(gca,'FontName','Times New Roman','FontSize',10)
xlabel('beta1','FontName','Times New Roman','FontSize',10)
ylabel('Percentage','FontName','Times New Roman','FontSize',10)

figure()
bar(number2.*5+1, number_count2./sum(number_count2),0.7)
xlim([-0.5 102])
set(gcf, 'Position', [100 100 240 240])
ylim([0 0.15])
set(gca,'YTick',[0 0.05 0.10 0.15])
set(gca,'FontName','Times New Roman','FontSize',10)
xlabel('beta2','FontName','Times New Roman','FontSize',10)
ylabel('Percentage','FontName','Times New Roman','FontSize',10)


%f(k)
figure()
data=load('FK20To30.txt');
simu=load('FKSimu20To30.txt');
loglog(data(1:20),'bo','linewidth',2)
hold on
loglog(simu(1:20),'rs','linewidth',2)
set(gcf, 'Position', [100 100 300 300])
set(gca,'FontName','Times New Roman','FontSize',10)
xlabel('Rank, k','FontName','Times New Roman','FontSize',10)
ylabel('f(k)','FontName','Times New Roman','FontSize',10)
xlim([0 20])
legend('data','simu','Location','southwest');
set(gca,'ticklength',3*get(gca,'ticklength'))
legend('boxoff')