%slope plot for home location
figure()
moments = flipud(MomentCalculation(1:9,:));
x=1:9;
for i=5:11
    plot(x,log2(moments(:,i)./moments(1,i)),'-o','linewidth',2,'markerfacecolor','b','markersize',3);
    hold on;
end
set(gcf, 'Position', [100 100 300 300])
xlim([1 9])
set(gca,'FontName','Times New Roman','FontSize',10)
xlabel('N','FontName','Times New Roman','FontSize',10)
ylabel('Log(E(D^q))','FontName','Times New Roman','FontSize',10)
set(gca,'XTick',[1 2 3 4 5 6 7 8 9])
title('Home location moment plot')

%slope plot for other location
figure()
moments = flipud(MomentCalculation(19:27,:));
x=1:9;
for i=5:11
    plot(x,log2(moments(:,i)./moments(1,i)),'-o','linewidth',2,'markerfacecolor','b','markersize',3);
    hold on;
end
set(gcf, 'Position', [100 100 300 300])
xlim([1 9])
set(gca,'FontName','Times New Roman','FontSize',10)
xlabel('N','FontName','Times New Roman','FontSize',10)
ylabel('Log(E(D^q))','FontName','Times New Roman','FontSize',10)
set(gca,'XTick',[1 2 3 4 5 6 7 8 9])
title('Other location moment plot')