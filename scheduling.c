//
// Created by Aleksandar Kostic on 2019-10-28 for Western University's 3305 class.
// This program implements a simulation of the FCFS, SJF, and RR CPU scheduling algorithms
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    char *inputFile = "cpu_scheduling_input_file.txt";
    FILE *infile;
    FILE *outFile;

    char *line = NULL;
    size_t length = 0;
    ssize_t read;

    int queueNumber = 1;

    // Opening the files & seeing if there were any errors opening them
    infile = fopen(inputFile, "r");

    if (infile == NULL) {

        perror("Error opening the file");
        exit(1);

    }

    outFile = fopen("cpu_scheduling_output_file.txt", "w");

    if (outFile == NULL) {

        perror("Error opening the file");
        exit(1);

    }

    while ((read = getline(&line, &length, infile)) != -1) { // Continue reading until EOF

        //
        // SECTION 1: FCFS CPU Scheduling Algorithm
        //
        char *arrayOfProcesses[read-1];
        char *arrayOfBurstTimes[read-1];
        char *timeQuantum;
        int waitTime = 0;
        float cumAvgWaitTime = 0;
        int numProcesses = 0;

        fprintf(outFile, "Ready Queue %d Applying FCFS Scheduling\n", queueNumber);
        fprintf(outFile, "\n");

        // Used for parsing the line that was read (representing the queue)
        // First four times the pointer is moved as these have the queue number and quantum time
        char *pointer = strtok(line, " ");
        pointer = strtok(NULL, " ");
        pointer = strtok(NULL, " ");
        pointer = strtok(NULL, " ");
        timeQuantum = pointer;
        int timeQuantumInt = atoi(timeQuantum);
        pointer = strtok(NULL, " ");

        // This section deals with adding the processes to the new arrays for the process names & burst times
        // Furthermore, this prints the order of selection by the CPU.
        fprintf(outFile, "Order of selection by CPU:\n");
        while (pointer != NULL) {

            arrayOfProcesses[numProcesses] = pointer; // Obtains & stores the process number and its associated burst times
            fprintf(outFile, "%s ", pointer);

            pointer = strtok(NULL, " ");
            arrayOfBurstTimes[numProcesses] = pointer;

            pointer = strtok(NULL, " ");
            numProcesses++; // this is used to determine the number of processes & also for indexing

        }
        fprintf(outFile, "\n");
        fprintf(outFile, "\n");

        // This section deals with printing the waiting times for each process
        fprintf(outFile, "Individual waiting times for each process:\n");
        for (int i = 0; i < numProcesses; i++) {

            fprintf(outFile, "%s = %d\n", arrayOfProcesses[i], waitTime);
            waitTime = waitTime + atoi(arrayOfBurstTimes[i]);

            // Used to calculate cumulative waiting time for all processes
            if (i != numProcesses-1) {

                cumAvgWaitTime = cumAvgWaitTime + waitTime;

            }

        }
        fprintf(outFile, "\n");

        // This section deals with determining the average waiting time
        fprintf(outFile, "Average waiting time = %.1f\n", cumAvgWaitTime/numProcesses);
        fprintf(outFile, "\n");


        //
        // SECTION 2: SJF CPU Scheduling Algorithm
        //

        // This section creates two new arrays for the shifting of processes based on their burst times
        char *arrayOfProcessesSJF[read-1];
        char *arrayOfBurstTimesSJF[read-1];

        for (int i = 0; i < numProcesses; i++) { // Create copies of the arrays

            arrayOfProcessesSJF[i] = arrayOfProcesses[i];
            arrayOfBurstTimesSJF[i] = arrayOfBurstTimes[i];

        }

        // This section deals with sorting both the newly created arrays (shortest -> longest burst times)
        for (int i = 0; i < numProcesses; i++) {

            for (int j = i+1; j < numProcesses; j++) {

                if (atoi(arrayOfBurstTimesSJF[i]) > atoi(arrayOfBurstTimesSJF[j])) {

                    // Switches order in the burst times array
                    char *temp1 = arrayOfBurstTimesSJF[i];
                    arrayOfBurstTimesSJF[i] = arrayOfBurstTimesSJF[j];
                    arrayOfBurstTimesSJF[j] = temp1;

                    // Switches order in the processes array
                    char *temp2 = arrayOfProcessesSJF[i];
                    arrayOfProcessesSJF[i] = arrayOfProcessesSJF[j];
                    arrayOfProcessesSJF[j] = temp2;

                }

            }

        }

        fprintf(outFile, "Ready Queue %d Applying SJF Scheduling\n", queueNumber);
        fprintf(outFile, "\n");

        // This section deals with adding the processes to the new arrays for the process names & burst times
        // Furthermore, this prints the order of selection by the CPU.
        fprintf(outFile, "Order of selection by CPU:\n");
        for (int i = 0; i < numProcesses; i++) {

            fprintf(outFile, "%s ", arrayOfProcessesSJF[i]);

        }
        fprintf(outFile, "\n");
        fprintf(outFile, "\n");

        // This section deals with printing the waiting times for each process
        int waitTimeSJF = 0;
        float cumAvgWaitTimeSJF = 0;

        fprintf(outFile, "Individual waiting times for each process:\n");
        for (int i = 0; i < numProcesses; i++) {

            fprintf(outFile, "%s = %d\n", arrayOfProcessesSJF[i], waitTimeSJF);
            waitTimeSJF = waitTimeSJF + atoi(arrayOfBurstTimesSJF[i]);

            // Used to calculate cumulative waiting time for all processes
            if (i != numProcesses-1) {

                cumAvgWaitTimeSJF = cumAvgWaitTimeSJF + waitTimeSJF;

            }

        }
        fprintf(outFile, "\n");

        // This section deals with determining the average waiting time
        fprintf(outFile, "Average waiting time = %.1f\n", cumAvgWaitTimeSJF/numProcesses);
        fprintf(outFile, "\n");

        //
        // SECTION 3: RR CPU Scheduling Algorithm
        //

        fprintf(outFile, "Ready Queue %d Applying RR Scheduling\n", queueNumber);
        fprintf(outFile, "\n");

        // This section creates a new array to indicate how much burst time remains for each process
        int arrayOfBurstTimesRemaining[read-1];
        int arrayOfCurrentRunTimes[read-1];
        int currentTimeRunning = 0;
        char CPUsequence[500] = "";

        for (int i = 0; i < numProcesses; i++) {

            arrayOfBurstTimesRemaining[i] = atoi(arrayOfBurstTimes[i]);
            arrayOfCurrentRunTimes[i] = 0;

        }

        // Infinite loop; allows for the continual execution of the processes in the queue
        for (;;) {

            int flagToStop = 1; // This flag only remains in the 1 state if there is no more to process

            for (int i = 0; i < numProcesses; i++) {

                // When it first beings being processed, note down the time it began
                if (arrayOfBurstTimesRemaining[i] == atoi(arrayOfBurstTimes[i])) {

                    arrayOfCurrentRunTimes[i] = currentTimeRunning;

                }

                // This condition is only met when the time remaining to process is more than the time quantum
                if (arrayOfBurstTimesRemaining[i] > timeQuantumInt) {

                    flagToStop = 0; // If there is more to process, change the flag to indicate this

                    currentTimeRunning = currentTimeRunning + timeQuantumInt;
                    arrayOfBurstTimesRemaining[i] = arrayOfBurstTimesRemaining[i] - timeQuantumInt;

                    strcat(CPUsequence, arrayOfProcesses[i]); // Write to the string to get process list
                    strcat(CPUsequence, " ");

                }

                else if (arrayOfBurstTimesRemaining[i] <= 0) {

                    // Do nothing; this was left like this on purpose

                }

                // This is the last time the process is to be run as the time quantum is greater than remaining burst time
                else {

                    currentTimeRunning = currentTimeRunning + arrayOfBurstTimesRemaining[i];
                    arrayOfBurstTimesRemaining[i] = 0; // Set remaining time to 0 as process has finished
                    arrayOfCurrentRunTimes[i] = currentTimeRunning - arrayOfCurrentRunTimes[i];

                    strcat(CPUsequence, arrayOfProcesses[i]); // Write to the string to get process list
                    strcat(CPUsequence, " ");

                }

            }

            if (flagToStop == 1) { // Terminates the while loop when all processes are finished

                break;

            }

        }

        // This section deals with adding the processes to the new arrays for the process names & burst times
        // Furthermore, this prints the order of selection by the CPU.
        fprintf(outFile, "Order of selection by CPU:\n");
        fprintf(outFile, "%s", CPUsequence);
        fprintf(outFile, "\n");

        // This section prints the turnaround times for each process
        fprintf(outFile, "\n");
        fprintf(outFile, "Turnaround times for each process\n");
        for (int i = 0; i < numProcesses; i++) {

            fprintf(outFile, "%s = %d\n", arrayOfProcesses[i], arrayOfCurrentRunTimes[i]);

        }
        fprintf(outFile, "\n");


        // All algorithms completed for this queue; process next queue
        queueNumber++;

    }

    fclose(infile);
    fclose(outFile);

}

