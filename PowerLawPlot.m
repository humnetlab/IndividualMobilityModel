%loglog plot
function PowerLawPlot(m_data,marker,color,min_cut,max_cut,unit_bins)
if nargin < 6
   unit_bins=10;
end
if nargin < 5
    max_cut = 9999999;
end
if nargin <4
    min_cut =0.0000001;
end
index=find(m_data>min_cut&m_data<max_cut);
m_data=m_data(index);
max_data=max(m_data);
min_data=min(m_data);
max_order=ceil(log10(max_data))
min_order=floor(log10(min_data))
order_range=max_order-min_order;
Nbins=zeros(1,order_range*unit_bins+1);

[m,n]=size(m_data);
min_order_data=10^min_order;

for ii=1:m
    Nbins(1,floor(log10(m_data(ii)/min_order_data)*unit_bins)+1)=Nbins(1,floor(log10(m_data(ii)/min_order_data)*unit_bins)+1)+1;
end
Nbins_sum=sum(Nbins');
Nbins=Nbins./Nbins_sum;
%Nbins
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

loglog(x(2:end),Nbins(2:end),[color,marker],'MarkerEdgeColor',color,'MarkerSize',4);
set(gca,'FontName','Times New Roman','FontSize',10)
xlabel('x','FontName','Times New Roman','FontSize',10)
ylabel('P(x)','FontName','Times New Roman','FontSize',10)
%xlim([10^min_order,10^max_order])

