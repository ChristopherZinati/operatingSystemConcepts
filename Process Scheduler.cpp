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
};