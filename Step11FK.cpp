
#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <stdlib.h> 
#include <map>
#define pi 3.14159265358979323846
#define EPSILON 0.0000001
using namespace std;
int loc_vec[100000]={0};
int id_map[100000]={0};
int main(int argc, char** argv){
	//only users with locations between these numbers are used
	int threshold_min=atoi(argv[1]);
	int threshold_max=atoi(argv[2]);
	string thres_min_str=argv[1];
	string thres_max_str=argv[2];
	ifstream fid_in;
	fid_in.open("ExtractedStays.txt",ifstream::in);
	FILE * fout_id2;	
	string file_name0="FK"+ thres_min_str + "To" + thres_max_str + ".txt";
	fout_id2=fopen(file_name0.c_str(), "w");
	string tline;
	int num1;
	int num2;
	int num3;
	int num4;
	int num5;
	int temp_number;
	int num_person_count=0;
	int num_place=0;
	int personal_vector_size=0;
	int personal_record_num=0;
	vector<double> all_frequency_count;
	vector<int> personal_freuency_count;
	int person_id=0;
	
	
	
	for (int i=0;i<10000;i++){
		all_frequency_count.push_back(0);
	}
	
	while (getline(fid_in,tline)){
		stringstream parse_line(tline);
		parse_line>>num1>>num2>>num3>>num4>>num5;
		if (num1==person_id){
			loc_vec[personal_record_num]=num5;
			personal_record_num++;
		}
		else{
			//re-id and count the number of places;
			for (int i=0;i<100000;i++){
				id_map[i]=-1;
			}
			num_place=0;
			for (int i=0;i<personal_record_num;i++){
				
				if (id_map[loc_vec[i]]==-1){
					id_map[loc_vec[i]]=++num_place;
				}		

				
			}
			
			//deal with the old one first
			if (num_place>=threshold_min&&num_place<=threshold_max){
				num_person_count++;
				personal_freuency_count.clear();
				for (int i=0;i<num_place+1;i++){
					personal_freuency_count.push_back(0);
				}
				for (int i=0;i<personal_record_num;i++){
					personal_freuency_count[id_map[loc_vec[i]]]++; 
				}
				personal_vector_size=personal_freuency_count.size();
				//do the bubble sort
				for (int j=0;j<personal_vector_size-1;j++){
					for (int k=j+1;k<personal_vector_size;k++){
						if (personal_freuency_count[j]<personal_freuency_count[k]){
							temp_number=personal_freuency_count[k];
							personal_freuency_count[k]=personal_freuency_count[j];
							personal_freuency_count[j]=temp_number;
						}
					}
				}
				//update the overall
				for (int j=0;j<personal_vector_size;j++){
					if (personal_freuency_count[j]>0){
						all_frequency_count[j]+=(personal_freuency_count[j]+0.0001)/personal_record_num;
					}
				}
			}
			
			//start the new one
			num_place=0;			
			personal_record_num=0;
			person_id=num1;
			//the first record
			loc_vec[personal_record_num]=num5;
			personal_record_num++;
		}
	}
	//output the result
	for (int i=0;i<10000;i++){
		fprintf(fout_id2,"%11.6f\n",all_frequency_count[i]/num_person_count);
	}
}

