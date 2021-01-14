//Kareem Abdelaty
//300075331

#include "scheduler.h"
#include "common.h"
#include <queue>
#include <iostream>

// this is the function you should edit
//
// runs Round-Robin scheduling simulator
// input:
//   quantum = time slice
//   max_seq_len = maximum length of the reported executing sequence
//   processes[] = list of process with populated IDs, arrival_times, and bursts
// output:
//   seq[] - will contain the execution sequence but trimmed to max_seq_len size
//         - idle CPU will be denoted by -1
//         - other entries will be from processes[].id
//         - sequence will be compressed, i.e. no repeated consecutive numbers
//   processes[]
//         - adjust finish_time and start_time for each process
//         - do not adjust other fields
//
void simulate_rr(
    int64_t quantum, 
    int64_t max_seq_len,
    std::vector<Process> & processes,
    std::vector<int> & seq
) {
	int64_t time = 0;
	int64_t index = 0;
	int64_t time_jump = 1;
	int64_t next=0;
	int64_t	lowest_finish =0;
	int64_t qtimer = quantum;
	int64_t n =0;
	int inserted = 0;
	int64_t num_proccesses = (int64_t)processes.size();
	int64_t minimum;
	std::queue<Process> q;
	bool start_recorded = false;
    int current_proccess_id = -1;
	int previous_proccess_id = -2;
	int64_t remaining_time[num_proccesses];
	for(int i=0 ;i<num_proccesses; i++){
		processes[i].start_time = -1;
		remaining_time[i] = -1;
	}
	seq.clear();
	while(1){
		if((index == num_proccesses)&&(q.empty())){
			break;
		}
		while(current_proccess_id != -1){
				if(current_proccess_id != previous_proccess_id){
					seq.push_back(current_proccess_id);
					inserted++;
					previous_proccess_id = current_proccess_id;
				}
				if(processes[current_proccess_id].start_time == -1){
					processes[current_proccess_id].start_time =time;
					remaining_time[current_proccess_id] = processes[current_proccess_id].burst;
				}
				if((!q.empty())&& start_recorded&&inserted>=max_seq_len){
					if(index<num_proccesses){
						minimum =  std::min(next, lowest_finish);
					}
					else{
						minimum = lowest_finish;
					}
					if(minimum%(quantum *((int64_t) q.size())) == 0){
						n = (minimum/(quantum *((int64_t) q.size()))) -1;
					}
					else{
						n = minimum/(quantum *((int64_t) q.size()));
					}
					time_jump = n*quantum*((int64_t) q.size());
					time +=time_jump;
					std::queue<Process> q2 =q;
					int current_proccess;
					while(!q2.empty()){
						current_proccess = (q2.front()).id;
						q2.pop();
						remaining_time[current_proccess] -= (n*quantum);
					}
					}
				
				else if(index<num_proccesses&&q.size()==1){
					if(remaining_time[current_proccess_id]<=next){
						time_jump = remaining_time[current_proccess_id];
						remaining_time[current_proccess_id] -= time_jump;
						time +=time_jump;
						qtimer =0;
					}else{
						time_jump = next;
						remaining_time[current_proccess_id] -= time_jump;
						time +=time_jump;
						 if(next % quantum==0){
							 qtimer = 0;
						 }else{
							 int64_t temp = next/quantum;
							 temp++;
							 int64_t temp2 = quantum*temp;
							 qtimer = temp2-next;
						 }
						 
					}
				}else if(q.size()==1&& index >= num_proccesses){
					time_jump = remaining_time[current_proccess_id];
					remaining_time[current_proccess_id] -= time_jump;
					time += time_jump;
				}else{
					minimum = std::min(remaining_time[current_proccess_id], qtimer);
					if(index < num_proccesses){
						int64_t temp = std::min(next, qtimer);
						minimum = std::min(temp,remaining_time[current_proccess_id] );
					}
					qtimer -= minimum;
					time_jump = minimum;
					time += time_jump;
					remaining_time[current_proccess_id] -= minimum;
					}
				if(qtimer==0||remaining_time[current_proccess_id]==0){
					q.pop();
					if(remaining_time[current_proccess_id]==0){
						processes[current_proccess_id].finish_time = time;
					}else{
						q.push(processes[current_proccess_id]);
					}
					qtimer = quantum;
			}
			current_proccess_id = -1;
			}
		
		for(int i=index ;i<num_proccesses; i++){
			if(processes[i].arrival_time == time ){
				q.push(processes[i]);
				index = i+1;
			}
		}
		std::queue<Process> q2 =q;
		start_recorded = true;
		while(!q2.empty()){
			int current_proccess = (q2.front()).id;
			q2.pop();
			if(processes[current_proccess].start_time == -1){
				start_recorded =false;
			}
		}
		lowest_finish = 9223372036854775807;
		for(int i=0 ;i<num_proccesses; i++){
			if(remaining_time[i] > 0){
				if(remaining_time[i] < lowest_finish){
					if(remaining_time[i] <= quantum){
							start_recorded = false;
						}
					lowest_finish = remaining_time[i];
				}
			}
		}
		if(index < num_proccesses){
			next  = processes[index].arrival_time - time;
		}
		if(index<num_proccesses){
			if((next<=(quantum *((int64_t) q.size())))||(lowest_finish<=(quantum *((int64_t) q.size())))){
				start_recorded = false;
			}
			
		}else{
			if(lowest_finish<=(quantum *((int64_t) q.size()))){
				start_recorded = false;
			}

		}
		if(current_proccess_id == -1){
			if(!q.empty()){
				current_proccess_id = (q.front()).id;
			}
			else if (index != num_proccesses){
				seq.push_back(-1);
				inserted++;
				if(index < num_proccesses){
					time_jump = next;
					time += time_jump;	
				}	
			}
			
		}
	}
	if(seq.size() >((unsigned long int) max_seq_len)){
		seq.resize(max_seq_len);
	}
	
    
}