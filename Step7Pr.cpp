
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>    // std::random_shuffle
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <math.h>
#include <stdio.h> 
#include <stdlib.h>
#include <time.h> 
#pragma warning(disable:4996)
#define pi 3.14159265358979323846
#define PERNUM 20000
#define POINUM 3000000
using namespace std;

double deg2rad(double deg) {
	return (deg * pi / 180);
}

double rad2deg(double rad) {
	return (rad * 180 / pi);
}

double distance(double lat1, double lon1, double lat2, double lon2, char unit) {
	double theta, dist;
	theta = lon1 - lon2;
	dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
	dist = acos(dist);
	dist = rad2deg(dist);
	dist = dist * 60 * 1.1515;
	switch (unit) {
	case 'M':
		break;
	case 'K':
		dist = dist * 1.609344;
		break;
	case 'N':
		dist = dist * 0.8684;
		break;
	}
	return (dist);
}

struct sort_pred {
    bool operator()(const std::pair<double,int> &left, const std::pair<double,int> &right) {
        return left.first < right.first;
    }
};

double min_dist=0.6;
double dist_vec[POINUM]={0};
double big_cell_sizes[8] = {0.26665, 0.13333, 0.066664, 0.033332, 0.016666, 0.008333,  0.0042, 0.0021};
int in_cell_count[8][2] = {0}; //count each trip
double min_lon = -71.48;
double min_lat = 42.0620;

vector<pair<double, int> > dist_pair;
int xid1;
int yid1;
int xid2;
int yid2;

double selected_home_latlon[PERNUM][2]={0};
double selected_other_latlon[POINUM][2]={0};
double rank_select_pdf[POINUM]={0};
double pdf_sum=0;
double rank_select_cdf[POINUM]={0};
int per_count=0;
int other_count=0;

int main(int argc, char** argv){
	srand(time(0));
	string file_name1 = argv[1];//"SimuPointHome.txt";
	string file_name2 = argv[2];//"SimuPointOther.txt";
	string file_name3 = argv[3];//"PrSimu.txt";
	string file_name4 = argv[4];//"PISimu.txt";
	//the input file 
	ifstream fid_in1;
	fid_in1.open(file_name1.c_str(), ifstream::in);
	ifstream fid_in2;
	fid_in2.open(file_name2.c_str(), ifstream::in);
	//output simulated r
	FILE * fout_id1;
	fout_id1 = fopen(file_name3.c_str(), "w");
	FILE * fout_id2;
	fout_id2 = fopen(file_name4.c_str(), "w");
	
	
	string tline;
	while (getline(fid_in1, tline)){
		stringstream parse_line(tline);
		parse_line >> selected_home_latlon[per_count][0] >> selected_home_latlon[per_count][1];
		per_count++;
	}
	while (getline(fid_in2, tline)){
		stringstream parse_line(tline);
		parse_line >> selected_other_latlon[other_count][0] >> selected_other_latlon[other_count][1];
		other_count++;
	}
	
	//probability to choose each location
	for (int i=0;i<other_count;i++){
		rank_select_pdf[i]=pow(i+1,-0.92);
		pdf_sum+=rank_select_pdf[i];
	}
	
	rank_select_pdf[0]/=pdf_sum;
	for (int i=1;i<other_count;i++){
		rank_select_pdf[i]/=pdf_sum;
	}
	
	double max_num=RAND_MAX;
	double rd_num;
	for (int i=0;i<per_count;i+=10){
		if (i%100==0){
			cout<<i<<endl;
		}
		dist_pair.clear();
		for (int j=0;j<other_count;j++){
			dist_vec[j]=distance(selected_home_latlon[i][0], selected_home_latlon[i][1], selected_other_latlon[j][0], selected_other_latlon[j][1], 'K');		
			dist_pair.push_back(make_pair(dist_vec[j],j));
		}
		sort(dist_vec,dist_vec+other_count);
		sort(dist_pair.begin(), dist_pair.end(), sort_pred());
		
		
		//not choosing opportunities closer than certain limit
		int begin_j;
		for (int j=1;j<other_count;j++){
			if (dist_vec[j]>min_dist){
				begin_j=j;
				break;
			}
		}
		pdf_sum=0;
		for (int j=begin_j;j<other_count;j++){
			pdf_sum+=rank_select_pdf[j-begin_j];
		}
		for (int j=0;j<other_count;j++){
			if (j<begin_j){
				rank_select_cdf[j]=0;
			}
			else{
				rank_select_cdf[j]=rank_select_cdf[j-1]+rank_select_pdf[j-begin_j]/pdf_sum;
			}
		}

		
		//select 10 other trips
		for (int j=0;j<10;j++){
			rd_num=rand()/max_num;
			
			for (int k=0;k<other_count;k++){
				if (rd_num<rank_select_cdf[k]){
					fprintf(fout_id1,"%f %d\n",dist_vec[k],k);
					
					//if the place is within the home cell
					for (int l=0;l<8;l++){
						xid1 = (int)((selected_home_latlon[i][1] - min_lon)/big_cell_sizes[l]);
						yid1 = (int)((selected_home_latlon[i][0] - min_lat)/big_cell_sizes[l]);
						
						xid2 = (int)((selected_other_latlon[dist_pair[k].second][1] - min_lon)/big_cell_sizes[l]);
						yid2 = (int)((selected_other_latlon[dist_pair[k].second][0] - min_lat)/big_cell_sizes[l]);
						
						if (xid1==xid2 && yid1==yid2){
							in_cell_count[l][0]++;
						}
						else{
							in_cell_count[l][1]++;
						}
					}
					
					break;
				}
			}
			
		}
	}
	
	for (int i=0;i<8;i++){
		fprintf(fout_id2,"%d %d\n",in_cell_count[i][0],in_cell_count[i][1]);
	}
	
	return 0;
}
