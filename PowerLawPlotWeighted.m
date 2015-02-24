%loglog plot
function [x_ nbins_]=PowerLawPlotWeighted(m_data,marker,color,min_cut,max_cut,unit_bins)
if nargin < 6
   unit_bins=10;
end
if nargin < 5
    max_cut = 9999999;
end
if nargin <4
    min_cut =0.0000001;
end
index=find(m_data(:,1)>min_cut&m_data(:,1)<max_cut);
m_data=m_data(index,:);
unit_bins=10;
max_data=max(m_data(:,1));
min_data=min(m_data(:,1));
max_order=ceil(log10(max_data))
min_order=floor(log10(min_data))
order_range=max_order-min_order;
Nbins=zeros(1,order_range*unit_bins+1);

[m,n]=size(m_data);
min_order_data=10^min_order;

for ii=1:m
    Nbins(1,floor(log10(m_data(ii,1)/min_order_data)*unit_bins)+1)=Nbins(1,floor(log10(m_data(ii,1)/min_order_data)*unit_bins)+1)+m_data(ii,2);
end
Nbins_sum=sum(Nbins');
Nbins=Nbins./Nbins_sum;
x=[];
for i=1:size(Nbins,2)
    x(i)=10^(min_order+i/unit_bins);
    if i>1
        Nbins(i)=Nbins(i)/(x(i)-x(i-1));
    else
        Nbins(i)=Nbins(i)/(x(i)-0);
    end
end
index=find(Nbins>0);
polyfit(log10(x(index(17:end))),log10(Nbins(index(17:end))),1)
x_=log10(x(index));
nbins_=log10(Nbins(index));
loglog(x(1:end),Nbins(1:end),[color,marker],'MarkerEdgeColor',color,'MarkerSize',5);
set(gca,'FontName','Times New Roman','FontSize',10)
xlabel('x','FontName','Times New Roman','FontSize',10)
ylabel('P(x)','FontName','Times New Roman','FontSize',10)

