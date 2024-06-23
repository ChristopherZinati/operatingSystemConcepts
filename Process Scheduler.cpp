#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

class Process {
public:
    int pid;
    int arr_time;
    int burst_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;

    Process(int id, int arr, int burst) : pid(id), arr_time(arr), 
        completion_time(0), waiting_time(0), turnaround_time(0){}
};

class SchedulingAlgorithms{
public:
    void fcfs (vector<Process>& processes){
        sort(processes.begin(), processes.end(), [](Process& a, Process& b) { 
            return a.arr_time < b.arr_time;
            });
        int curr_time = 0;
        for (auto& process : processes){
            if (curr_time < process.arr_time){
                curr_time = process.arr_time;
            }
            process.completion_time = curr_time + process.burst_time;
            process.turnaround_time = process.completion_time-process.arr_time;
            process.waiting_time = process.turnaround_time-process.burst_time;
            curr_time += process.burst_time;          
        }
    }
    void sjn (vector<Process>& processes){
        sort(processes.begin(), processes.end(), [](Process& a, Process& b) { 
            return a.arr_time < b.arr_time;
            });
        int curr_time = 0;
        vector<Process> ready;

        while(!processes.empty() || !ready.empty()){
            while(!processes.empty() && processes.front().arr_time < curr_time){
                ready.push_back(processes.front());
                processes.erase(processes.begin());
            }
            if (!ready.empty()){
                sort(ready.begin(), ready.end(), [](Process& a, Process& b) {
                    return a.burst_time < b.burst_time;
                });
                Process process = ready.front();
                ready.erase(ready.begin());

                process.completion_time = curr_time + process.burst_time;
                process.turnaround_time = process.completion_time - process.arr_time;
                process.waiting_time = process.turnaround_time - process.burst_time;
                curr_time += process.burst_time;
            }
            else {
               curr_time = processes.front().arr_time; 
            }
        }
    }
};