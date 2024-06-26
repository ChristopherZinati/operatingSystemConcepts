#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <string>

class Process {
public:
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;

    Process(int id, int arrival, int burst)
        : pid(id), arrival_time(arrival), burst_time(burst), remaining_time(burst),
          completion_time(0), waiting_time(0), turnaround_time(0) {}
};

class SchedulingAlgorithms {
public:
    void fcfs_scheduling(std::vector<Process>& processes) {
        std::sort(processes.begin(), processes.end(), [](Process& a, Process& b) {
            return a.arrival_time < b.arrival_time;
        });

        int current_time = 0;
        gantt_chart.clear();
        timeline.clear();

        for (auto& process : processes) {
            if (current_time < process.arrival_time) {
                gantt_chart.push_back("Idle");
                timeline.push_back(current_time);
                current_time = process.arrival_time;
            }
            gantt_chart.push_back("P" + std::to_string(process.pid));
            timeline.push_back(current_time);
            process.completion_time = current_time + process.burst_time;
            process.turnaround_time = process.completion_time - process.arrival_time;
            process.waiting_time = process.turnaround_time - process.burst_time;
            current_time += process.burst_time;
        }
        timeline.push_back(current_time);
    }

    void sjn_scheduling(std::vector<Process>& processes) {
        std::sort(processes.begin(), processes.end(), [](Process& a, Process& b) {
            return a.arrival_time < b.arrival_time;
        });

        int current_time = 0;
        std::vector<Process*> ready_queue;
        std::vector<Process> completed_processes;

        gantt_chart.clear();
        timeline.clear();

        while (!processes.empty() || !ready_queue.empty()) {
            while (!processes.empty() && processes.front().arrival_time <= current_time) {
                ready_queue.push_back(&processes.front());
                processes.erase(processes.begin());
            }

            if (!ready_queue.empty()) {
                std::sort(ready_queue.begin(), ready_queue.end(), [](Process* a, Process* b) {
                    return a->burst_time < b->burst_time;
                });
                Process* process = ready_queue.front();
                ready_queue.erase(ready_queue.begin());

                if (current_time < process->arrival_time) {
                    gantt_chart.push_back("Idle");
                    timeline.push_back(current_time);
                    current_time = process->arrival_time;
                }

                gantt_chart.push_back("P" + std::to_string(process->pid));
                timeline.push_back(current_time);

                current_time += process->burst_time;
                process->completion_time = current_time;
                process->turnaround_time = process->completion_time - process->arrival_time;
                process->waiting_time = process->turnaround_time - process->burst_time;
                completed_processes.push_back(*process);
            } else if (!processes.empty()) {
                gantt_chart.push_back("Idle");
                timeline.push_back(current_time);
                current_time = processes.front().arrival_time;
            }
        }
        timeline.push_back(current_time);
        processes = completed_processes;
    }

    void rr_scheduling(std::vector<Process>& processes, int time_quantum) {
        std::queue<Process*> ready_queue;
        gantt_chart.clear();
        timeline.clear();

        int current_time = 0;
        int index = 0;

        while (index < processes.size() || !ready_queue.empty()) {
            while (index < processes.size() && processes[index].arrival_time <= current_time) {
                ready_queue.push(&processes[index]);
                index++;
            }
            if (!ready_queue.empty()) {
                Process* process = ready_queue.front();
                ready_queue.pop();

                if (process->remaining_time > time_quantum) {
                    gantt_chart.push_back("P" + std::to_string(process->pid));
                    timeline.push_back(current_time);
                    current_time += time_quantum;
                    process->remaining_time -= time_quantum;
                    while (index < processes.size() && processes[index].arrival_time <= current_time) {
                        ready_queue.push(&processes[index]);
                        index++;
                    }
                    ready_queue.push(process);
                } else {
                    gantt_chart.push_back("P" + std::to_string(process->pid));
                    timeline.push_back(current_time);
                    current_time += process->remaining_time;
                    process->completion_time = current_time;
                    process->turnaround_time = process->completion_time - process->arrival_time;
                    process->waiting_time = process->turnaround_time - process->burst_time;
                    process->remaining_time = 0;
                }
            } else if (index < processes.size()) {
                gantt_chart.push_back("Id"); //Id = Idle
                timeline.push_back(current_time);
                current_time = processes[index].arrival_time;
            }
        }
        timeline.push_back(current_time);
    }

    void print_gantt_chart() const {
        std::cout << "Gantt Chart:\n";

        for (const auto& slot : gantt_chart) {
            std::cout << "| " << slot << " ";
        }
        std::cout << "|\n";

        for (const auto& time : timeline) {
            std::cout << time << std::setw(5);
        }
        std::cout << "\n";
    }

private:
    std::vector<std::string> gantt_chart;
    std::vector<int> timeline;
};
int main() {
    int choice;
    std::cout << "Choose an option:\n";
    std::cout << "1. Enter custom processes\n";
    std::cout << "2. Use example processes\n";
    std::cin >> choice;

    std::vector<Process> processes;

    if (choice == 1) {
        int num_processes;
        std::cout << "Enter the number of processes: ";
        std::cin >> num_processes;

        for (int i = 0; i < num_processes; ++i) {
            int pid, arrival_time, burst_time;
            std::cout << "Enter Process ID, Arrival Time, and Burst Time for process " << (i + 1) << ": ";
            std::cin >> pid >> arrival_time >> burst_time;
            processes.emplace_back(pid, arrival_time, burst_time);
        }
    } else {
        processes = {
            Process(1, 0, 8),
            Process(2, 1, 4),
            Process(3, 2, 9),
            Process(4, 3, 5)
        };
    }

    int time_quantum;
    std::cout << "Enter time quantum for Round Robin scheduling: ";
    std::cin >> time_quantum;

    SchedulingAlgorithms scheduler;

    std::vector<Process> fcfs_processes = processes;
    scheduler.fcfs_scheduling(fcfs_processes);
    std::cout << "\nFCFS Scheduling:\n";
    for (const auto& process : fcfs_processes) {
        std::cout << "Process " << process.pid << ": Waiting Time = " << process.waiting_time
                  << ", Turnaround Time = " << process.turnaround_time << "\n";
    }
    scheduler.print_gantt_chart();

    std::vector<Process> sjn_processes = processes;
    scheduler.sjn_scheduling(sjn_processes);
    std::cout << "\nSJN Scheduling:\n";
    for (const auto& process : sjn_processes) {
        std::cout << "Process " << process.pid << ": Waiting Time = " << process.waiting_time
                  << ", Turnaround Time = " << process.turnaround_time << "\n";
    }
    scheduler.print_gantt_chart();

    std::vector<Process> rr_processes = processes;
    scheduler.rr_scheduling(rr_processes, time_quantum);
    std::cout << "\nRR Scheduling:\n";
    for (const auto& process : rr_processes) {
        std::cout << "Process " << process.pid << ": Waiting Time = " << process.waiting_time
                  << ", Turnaround Time = " << process.turnaround_time << "\n";
    }
    scheduler.print_gantt_chart();

    return 0;
}
