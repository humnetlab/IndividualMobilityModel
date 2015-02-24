% generate simulated home and other locations using 
% parameters from step 2_1

%define the grid
min_lon = -71.48;
min_lat = 42.0620;
n=8; %number of local slopes
cell_size =  0.5333/2^n;
PDs=PD.*PS.*(1-PD).*(1-PS).*rho+PD.*PS;
PD0=PD-PDs;
P0s=PS-PDs;
p00=1-PDs-P0s-PD0;
SigmaD(5:8)=0;%pure beta cascade
SigmaS(7:8)=0;%pure beta cascade
SigmaD_square=SigmaD.^2;
SigmaS_square=SigmaS.^2;
total_home=10000; %total number of home points
total_other=150000; %total number of other points
side_num=2^n;
grid_num=side_num^2;
density_d=ones(side_num);
density_s=ones(side_num);
for i=1:n
    i
    level_side_num=2^i;
    %for the sigma_ normal distribution
    mu=[-0.5*SigmaD_square(i),-0.5*SigmaS_square(i)];
    sigma = [SigmaD(i)^2,rho(i)*SigmaD(i)*SigmaS(i);rho(i)*SigmaD(i)*SigmaS(i),SigmaS(i)^2];
    %log(wd_i) and log(ws_i),normal distribution
    rdnum = mvnrnd(mu,sigma,level_side_num^2);
    %wd_i and ws_i
    lognm_num=exp(rdnum);
    %for the beta cascade
    rdBetaArr=rand(level_side_num^2,1);
    
    for j=1:side_num
        for k=1:side_num
            xid=ceil(j/side_num*level_side_num);
            yid=ceil(k/side_num*level_side_num);
            density_d(j,k)=density_d(j,k)*lognm_num((xid-1)*level_side_num+yid,1);
            density_s(j,k)=density_s(j,k)*lognm_num((xid-1)*level_side_num+yid,2);
            %deal with the beta cascade
            rdBeta=rdBetaArr((xid-1)*level_side_num+yid);
            if rdBeta<PDs(i)
                density_d(j,k)=density_d(j,k)*1/PD(i);
                density_s(j,k)=density_d(j,k)*1/PS(i);
            elseif rdBeta<PDs(i)+PD0(i)
                density_s(j,k)=0;
                density_d(j,k)=density_d(j,k)*1/PD(i);
            elseif rdBeta<PDs(i)+PD0(i)+P0s(i)
                density_s(j,k)=density_d(j,k)*1/PS(i);
                density_d(j,k)=0;
            else
                density_d(j,k)=0;
                density_s(j,k)=0;
            end
        end
    end
end
density_d=density_d/sum(sum(density_d));%sum to 1
density_s=density_s/sum(sum(density_s));%sum to 1
points_d=round(total_home*density_d);
points_s=round(total_other*density_s);

%output the lon lat of generated data
fid = fopen('SimuPointHome.txt','w');
for i=1:side_num
    for j=1:side_num
        if points_d(i,j)>0
            for k=1:points_d(i,j)
                lon=min_lon+i*cell_size+0.5*rand()*cell_size;
                lat=min_lat+j*cell_size+0.5*rand()*cell_size;
                fprintf(fid,'%f %f\n',lat,lon);
            end
        end
    end
end
fclose(fid);
fid = fopen('SimuPointOther.txt','w');
for i=1:side_num
    for j=1:side_num
        if points_s(i,j)>0
            for k=1:points_s(i,j)
                lon=min_lon+i*cell_size+0.5*rand()*cell_size;
                lat=min_lat+j*cell_size+0.5*rand()*cell_size;
                fprintf(fid,'%f %f\n',lat,lon);
            end
        end
    end
end
fclose(fid);
