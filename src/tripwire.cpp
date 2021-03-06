//tripwire.cpp
//Run a command when a given process ID terminates
//Author: Mark Fitzgibbon

//gcc -o tripwire.exe tripwire.cpp -lpsapi -lstdc++
#include <windows.h>
#include <string>
#include <psapi.h>
#include <stdio.h>

int POLL_FREQ = 1000;

bool isProcessIDActive(DWORD processID){
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if(EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)){
		cProcesses = cbNeeded/sizeof(DWORD);
		for(i = 0; i < cProcesses; i++){
			if(aProcesses[i] != 0){
				if(aProcesses[i] == processID){
					return TRUE;
				}
			}
		}	
	}
	return FALSE;
}


int main(int argc, char* argv[]){
	DWORD pidU = -1;
	std::string command= "";
	if (argc > 1){
		pidU = atoi(argv[1]);	
		int index = 2;
		while(index < argc){
			command += argv[index];
			command += " ";
			index++;
		}
	}
	bool trigger = isProcessIDActive(pidU);
	if(trigger){
		printf("Arming tripwire to Process ID: %d\n", pidU);
		while(trigger){
			trigger = isProcessIDActive(pidU);
			Sleep(POLL_FREQ);
		}
		printf("PID %d ended\n", pidU);
		system(command.c_str());
	}
	else{
		printf("PID %d not found in tasklist\n", pidU);
	}
	return 0;
}
