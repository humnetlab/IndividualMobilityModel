
#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <stdio.h> 
#include <stdlib.h>
#include <time.h> 
#pragma warning(disable:4996)
#define pi 3.14159265358979323846
#define CELLNUM 256
#define PERNUM 10000
#define N 9 //number of levels
#define QNUM 15
using namespace std;

string dur_thres = "300";
double min_stay_dur = 300;
double min_lon = -71.48;
double min_lat = 42.0620;
double cell_size = 0.0020832;
int time_min = 1266624000 + 18000;// Feb 20, 2010
int time_max = 1269907200 + 18000;// March 30th; 2010

double max_lon = min_lon + 0.53331 - 0.001;
double max_lat = min_lat + 0.53331 - 0.001;
int point_count[CELLNUM][CELLNUM][3][N] = {0}; //count home, work, and other points in the grid
int xid;
int yid;
int id;
double moment_sum[3][N][QNUM] = {0}; //activity type, level, moment q=-2:0.5:3
int moment_count[3][N][QNUM] = {0};
double q[QNUM] = {-2,-1.5,-1,-0.5,0,0.5,1,1.5,2,2.5,3,3.5,4,4.5,5};
int accept_count=0;

int min_home_count = 10; //threshold for min home stays
int min_work_count = 6;
double second_home_ratio = 0.5;
double departure_night_ratio = 0.1;
int night_departure_count;
int night_begin_time = 88; 
double min_home_work_dist = 0.4;
int personal_limit = 0;

int day_begin = time_min / 86400;
int day_end = time_max / 86400;
int t_day;//the day under calculation
int BIG_NEGATIVE_NUM=-999;

//store the records of each person 
vector<double> lons;
vector<double> lats;
vector<int> locs;
vector<int> times;
vector<int> durations;
//store the records of each person in one day
vector<double> one_day_lons;
vector<double> one_day_lats;
vector<int> one_day_times;
vector<int> one_day_durations;
vector<int> one_day_loc_id;
double one_day_filled_hours[24];//if in that hour there is at least one record the slot value is 1

int personal_count;
int one_day_count;
int partial_sum;
int rand_num;
int trip_time;
int start_activity_index;
int end_activity_index;
int next_activity;
int daily_min_activity;
int next_activity_id;
int this_activity_id;
int this_activity;
int activity_start_day;
int activity_end_day;
int current_rec_day;
int activity_start_slot;
int activity_dur;
int temp_hour;
int min_slot;
int max_slot;
int min_dur;
int max_dur;
int o_tract;
int d_tract;
int counter1 = 0;
int counter2 = 0;

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

double small_num;
double RandomSmallNumber(){
	return small_num;
}

int main(){
	srand((unsigned)time(NULL));
	small_num=0.1;
	string file_name1 = "SampleStayRegion.txt";
	string file_name2 = "MomentCalculation.txt";
	string file_name3 = "MeanSDProd.txt";
	string file_name4 = "HomeLocation.txt";
	string file_name5 = "WorkLocation.txt";
	string file_name6 = "OtherLocation.txt";
	
	//the input file 
	ifstream fid_in;
	fid_in.open(file_name1.c_str(), ifstream::in);
	//the output file
	FILE * fout_id;
	fout_id = fopen(file_name2.c_str(), "w");
	FILE * fout_id2;
	fout_id2 = fopen(file_name3.c_str(), "w");
	FILE * fout_id3;
	fout_id3 = fopen(file_name4.c_str(), "w");
	FILE * fout_id4;
	fout_id4 = fopen(file_name5.c_str(), "w");
	FILE * fout_id5;
	fout_id5 = fopen(file_name6.c_str(), "w");
	
	int SSlot[96][2]={0};
	string tline;
	double num1;
	double num2;
	double num3;
	double num4;
	double num5;
	int num1_int;
	int num2_int;
	int num3_int;
	int counter = 0;
	int per_count;
	double hour_bos;
	int place_count;
	int home_count[10000] = { 0 };
	int work_count[10000] = { 0 };
	double place_lon[10000] = { 0 };
	double place_lat[10000] = { 0 };
	int home;
	int work;
	int home_num;
	int second_home_num;
	int work_num;
	double home_work_dist;
	double work_product;
	int week_day;
	int home_valid_sign;
	int work_valid_sign;
	int person_id;

	while (getline(fid_in, tline)){
		stringstream parse_line(tline);
		parse_line >> num1_int >> num2_int >> num3_int >> person_id;
		per_count = num2_int;
		place_count = num3_int;
		for (int i = 0; i<10000; i++){
			home_count[i] = 0;
			work_count[i] = 0;
		}
		counter++;

		if (per_count>personal_limit&&per_count<10000 && place_count>2){

			//read in data and identify home and work
			for (int i = 0; i<per_count; i++){
				getline(fid_in, tline);
				stringstream parse_line(tline);
				parse_line >> num1_int >> num2_int >> num3_int >> num4 >> num5;
				if (num3_int>min_stay_dur){
					lons.push_back(num4);
					lats.push_back(num5);
					times.push_back(num2_int);
					durations.push_back(num3_int);
					locs.push_back(num1_int - 1); //location start with 0

					hour_bos = ((num2_int % 86400) / 3600.0) - 5;
					if (hour_bos<0){
						hour_bos += 24;
					}
					//1970/1/1 is a Thursday
					//From 0 to 6, Monday is 0.
					week_day = (int)(((num2_int - 18000) % 604800) / 86400.0) + 3;
					if (week_day>6){
						week_day -= 7;
					}
					if ((hour_bos<8 || hour_bos>19) && week_day<5 || week_day>5){
						home_count[num1_int - 1]++;
					}
					if (hour_bos>8 && hour_bos<19 && week_day<5){
						work_count[num1_int - 1]++;
					}
					place_lon[num1_int - 1] = num4;
					place_lat[num1_int - 1] = num5;
				}
			}
			home_num = -1;
			work_num = -1;
			second_home_num = -1;
			home = -1;
			work = -1;
			work_product = -1;
			home_work_dist = -1;
			home_valid_sign = 0;
			work_valid_sign = 0;
			for (int i = 0; i<place_count; i++){
				if (home_count[i]>home_num){
					second_home_num = home_num;
					home_num = home_count[i];
					home = i;
					if (home_num >= min_home_count){
						home_valid_sign = 1;
					}
				}
			}
			
			//only one significant home location
			if (second_home_num > home_num * second_home_ratio){
				home_valid_sign = 0;
			}
			
			//original criteria
			if (home_valid_sign == 1){
				
				//if there are too many night departures
				night_departure_count = 0;
				for (int i = 0; i < per_count; i++){
					if (locs[i]!=home){
						int eslot = ((((int)(times[i] + durations[i])) - 18000) % 86400) / 900;
						int sday = (((int)times[i]) - 18000) / 86400;
						int eday = (((int)(times[i] + durations[i])) - 18000) / 86400;
						if (eslot > night_begin_time || sday != eday){
							night_departure_count++;
						}
					}
				}
				if (night_departure_count > per_count * departure_night_ratio){
					home_valid_sign = 0;
					continue;
				}
			
				counter1++;
				for (int i = 0; i<place_count; i++){
					if (i != home && work_count[i] * distance(place_lat[home], place_lon[home], place_lat[i], place_lon[i], 'K')>work_product){
						work_product = work_count[i] * distance(place_lat[home], place_lon[home], place_lat[i], place_lon[i], 'K');
						work_num = work_count[i];
						home_work_dist = distance(place_lat[home], place_lon[home], place_lat[i], place_lon[i], 'K');
						work = i;
						if (work_num >= min_work_count && home_work_dist>min_home_work_dist){
							work_valid_sign = 1;
						}
						else{
							work_valid_sign = 0;
						}
					}
				}
			}
			if (!work_valid_sign){
				work = -1;
			}
			else{
				counter2++;
			}
			
			if (!home_valid_sign){
				continue;
			}
			
			if (place_lat[home]>min_lat && place_lat[home]<max_lat && place_lon[home]>min_lon && place_lon[home]<max_lon){
				++accept_count;
				for (int i = 0; i<place_count; i++){
					if (place_lon[i]<min_lon || place_lon[i]>max_lon || place_lat[i]<min_lat || place_lat[i]>max_lat){
						continue;
					}
					else{
						xid = (int)((place_lon[i] - min_lon)/cell_size);
						yid = (int)((place_lat[i] - min_lat)/cell_size);
						if (xid > CELLNUM - 1 || yid > CELLNUM - 1){
							continue;
						}
						if (i == home){
							point_count[xid][yid][0][0]++;
							fprintf(fout_id3,"%f %f\n", place_lat[i], place_lon[i]);
						}
						else if (i == work){
							point_count[xid][yid][1][0]++;
							fprintf(fout_id4,"%f %f\n", place_lat[i], place_lon[i]);
						}
						else{
							point_count[xid][yid][2][0]++;
							fprintf(fout_id5,"%f %f\n", place_lat[i], place_lon[i]);
						}
					}
				}
			
			}
			if (accept_count==PERNUM){
				break;
			}
		}
		else{
			//skip the data
			for (int i = 0; i<per_count; i++){
				getline(fid_in, tline);
			}
		}
		lons.clear();
		lats.clear();
		times.clear();
		durations.clear();
		locs.clear();
	}
	
	//aggregate to bigger levels
	//calculate the log(omega) value for each cell
	//n=0 is the smallest granularity
	for (int n=1; n<N; n++){
		int level_cell_num = CELLNUM / pow(2,n);
		for (int i=0;i<level_cell_num;i++){
			for (int j=0;j<level_cell_num;j++){
				for (int k=0;k<3;k++){
					point_count[i][j][k][n] = point_count[i*2][j*2][k][n-1] + point_count[i*2+1][j*2][k][n-1] + point_count[i*2][j*2+1][k][n-1] + point_count[i*2+1][j*2+1][k][n-1];
				}
				
			}
		}
	}
		
	//for each level, calculate moments
	for (int n=0; n<N; n++){
		int level_cell_num = CELLNUM / pow(2.0,n);
		//measure rho
		double sd_prod_sum=0;
		int s;
		int d;
		for (int i=0;i<level_cell_num;i++){
			for (int j=0;j<level_cell_num;j++){
				s=16.0*point_count[i][j][0][n]/pow(4.0,n);
				d=16.0*point_count[i][j][2][n]/pow(4.0,n);
				sd_prod_sum+=s*d;
			}
		}
		fprintf(fout_id2, "%f ", sd_prod_sum/(level_cell_num*level_cell_num));
		
		for (int k=0;k<3;k++){
			for (int i=0;i<level_cell_num;i++){
				for (int j=0;j<level_cell_num;j++){
					for (int qindex=0; qindex<QNUM; qindex++){
						if (point_count[i][j][k][n]>0){
							moment_sum[k][n][qindex] += pow(16.0*point_count[i][j][k][n]/pow(4.0,n),q[qindex]); //multiply 16 because the grid size is 250m
						}
						//count even empty 
						moment_count[k][n][qindex]++;
					}
				}
			}
		}
	}
	for (int k=0;k<3;k++){
		for (int n=0; n<N; n++){
			for (int qindex=0; qindex<QNUM; qindex++){
				fprintf(fout_id, "%f ", moment_sum[k][n][qindex]/moment_count[k][n][qindex]);
			}
			fprintf(fout_id, "\n");
		}
	}
	return 0;
}




