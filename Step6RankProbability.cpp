
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
#define PERNUM 8000
#define POINUM 3000000
using namespace std;

int min_home_count = 10; //threshold for min home stays
int min_work_count = 6;
double second_home_ratio = 0.5;
double departure_night_ratio = 0.1;
int night_departure_count;
int night_begin_time = 88; 
double min_home_work_dist = 0.4;
int personal_limit = 0;

double min_lon = -71.48;
double min_lat = 42.0620;
double max_lon = -71.48+0.5333;
double max_lat = 42.0620+0.5333;
double min_lon_big = -71.48;
double min_lat_big = 42.0620;
double max_lon_big = -71.48+0.5333;
double max_lat_big = 42.0620+0.5333;

double selected_home_latlon[PERNUM][2]={0};
double selected_other_latlon[POINUM][2]={0};
double other_begin_end_index[PERNUM][2]={0};
double other_visit_count[POINUM]={0};
int other_count=0;
int accept_count=0;

vector<pair<double, int> > dist_vec;
vector<pair<double, int> > count_vec;
double home_other_rank_sum[POINUM]={0};
double home_other_rank_sum_weighted[POINUM]={0};
int home_other_rank_count={0};

int time_min = 1266624000 + 18000;// Feb 20, 2010
int time_max = 1269907200 + 18000;// March 30th; 2010
int day_begin = time_min / 86400;
int day_end = time_max / 86400;
int t_day;//the day under calculation



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

struct sort_pred {
    bool operator()(const std::pair<double,int> &left, const std::pair<double,int> &right) {
        return left.first < right.first;
    }
};


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


int main(int argc, char** argv){
	srand((unsigned)time(NULL));
	double min_stay_dur = 300;
	
	string file_name1 = "SampleStayRegion.txt";
	string file_name3 = "RankProbability.txt";

	//the input file 
	ifstream fid_in;
	fid_in.open(file_name1.c_str(), ifstream::in);
	//the output file
	FILE * fout_id2;
	fout_id2 = fopen(file_name3.c_str(), "w");

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
	int total_count[10000] = { 0 };
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
			total_count[i] = 0;
		}
	
		counter++;

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
				
				total_count[num1_int - 1]++;
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
		

		if (!home_valid_sign ){
			continue;
		}
		

		
		if (place_lat[home]>min_lat && place_lat[home]<max_lat && place_lon[home]>min_lon && place_lon[home]<max_lon){
			selected_home_latlon[accept_count][0]=place_lat[home];
			selected_home_latlon[accept_count][1]=place_lon[home];
			other_begin_end_index[accept_count][0]=other_count;
			++accept_count;
			for (int i = 0; i<place_count; i++){
				if (place_lat[i]>min_lat_big && place_lat[i]<max_lat_big && place_lon[i]>min_lon_big && place_lon[i]<max_lon_big){
					if (i==home){
						
					}
					else{
						selected_other_latlon[other_count][0]=place_lat[i];
						selected_other_latlon[other_count][1]=place_lon[i];
						other_visit_count[other_count]=total_count[i];
						
						++other_count;
					}
				}
			}
		}
		if (accept_count==PERNUM){
			break;
		}

		lons.clear();
		lats.clear();
		times.clear();
		durations.clear();
		locs.clear();
	}
	
	for (int i=0;i<PERNUM-1;i++){
		other_begin_end_index[i][1]=other_begin_end_index[i+1][0]-1;
	}
	other_begin_end_index[PERNUM][1]=other_count-1;

	
	cout<<"calculating home other distance"<<endl;
	double dist_sum;
	//distance between each home point and other location
	for (int i=0;i<PERNUM;i++){
		if (i%100==0){
			cout<<i<<endl;
		}
		dist_vec.clear();
		count_vec.clear();
		dist_sum=0;
		for (int j=0;j<other_count;j++){
			double dist_ =distance(selected_home_latlon[i][0], selected_home_latlon[i][1], selected_other_latlon[j][0], selected_other_latlon[j][1], 'K');	
			dist_sum+=dist_;
			dist_vec.push_back(make_pair(dist_,j));
			count_vec.push_back(make_pair(dist_,other_visit_count[j]));
		}
		sort(dist_vec.begin(),dist_vec.end(),sort_pred());
		sort(count_vec.begin(),count_vec.end(),sort_pred());
		int person_begin=other_begin_end_index[i][0];
		int person_end=other_begin_end_index[i][1];
		for (int j=0;j<other_count;j++){
			if (dist_vec[j].second>=person_begin&&dist_vec[j].second<=person_end){
				home_other_rank_sum[j]++;
				home_other_rank_sum_weighted[j]+=count_vec[j].second;
				home_other_rank_count++;
			}
		}
	}
	
	//output result

	for (int i=0;i<other_count;i++){
		fprintf(fout_id2, "%f ", home_other_rank_sum_weighted[i]);
	}
	fprintf(fout_id2, "\n");
	
	return 0;
}


