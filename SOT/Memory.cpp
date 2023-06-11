#include <windows.h>
#include <string>
#include <TlHelp32.h>
#include <iostream>
#include "offsets.h"
#include <processthreadsapi.h>
#pragma once
class Memory
{
public:

    int ProcessID = NULL; // not very important for later use

    HANDLE hProc = NULL; // Important for ReadProcessMemory()
    uintptr_t ExebaseAddress = NULL; // BaseAddress from the main exe


    Memory(LPCWSTR GameName) {
        // Gain access to the process in 3 simple steps:
        // 1: Get the process id by looping thru all processes and comparing the name om the .exe file
        // 2: Get the Module base address by looping thru all modules and comparing the name of the .exe / .dll file
        // 3: Open the process which will return a HANDLE called hProc and u can use that as an argument in ReadProccesMemory()


        //Process ID
        ProcessID = FindProcId(GameName);
        if (ProcessID == NULL) { ExitProcess(101); } // check if PID is invalid
        std::wcout << (const wchar_t*)GameName << L"'s ProcessId: " << ProcessID << "\n";

        // Module base Adress
        // Add more GetBaseAddr funtions if dll's base addresses are needed
        ExebaseAddress = getBaseAddr(ProcessID, GameName);
        if (ExebaseAddress == NULL) { ExitProcess(102); }// check if Base Adress is invalid
        std::cout << std::hex << "Module base adress: 0x" << ExebaseAddress << "\n" << std::dec;

        // Open the process
        hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, ProcessID);
        if (hProc == NULL) { ExitProcess(103); }

        return;
    }
    ~Memory() {
        //Deleting a memory class
        std::cout << "Shutting down memory class" << "\n";
        if (hProc) { CloseHandle(hProc); }; // Release the HANDLE hProc

        return;
    }


    void MemoryLoop() {
        //check if process still exists
        DWORD exitCode = 0;
        GetExitCodeProcess(hProc, &exitCode);
        if (exitCode != STILL_ACTIVE) {
            ExitProcess(104);
        }
        
    }
private:
    
    static DWORD FindProcId(LPCWSTR procName) {
        
        // init vars
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(PROCESSENTRY32);
        DWORD ProcID = 0;

        //Create a snapshot of all processes currently running
        HANDLE procSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS , NULL);
        // loop thru the processes untill you find the one named "game.exe"
        for (bool b = Process32First(procSnap, &procEntry); b; b = Process32Next(procSnap, &procEntry))
        {
            //string compare process exe name with procName
            if (wcscmp(procEntry.szExeFile, procName) == 0)
            {
                ProcID = procEntry.th32ProcessID;
                break;
            }

        }
        //procid still 0 means nothing found
        if (ProcID == 0) {
            std::cout << "\n" << "Couldn't find the game's process Id :( " << "\n";
        }
        
        CloseHandle(procSnap);
        return ProcID;
    }

    uintptr_t getBaseAddr(DWORD procID, LPCWSTR modname) {
        //init vars
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(MODULEENTRY32);
        uintptr_t modBaseAddr = 0;


        // creating a snapshot of all modules
        HANDLE procSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32 , procID);
        // loop thru modules
        for (bool b = Module32First(procSnap, &modEntry); b; b = Module32Next(procSnap, &modEntry)) {
            //if module found
            if (wcscmp(modEntry.szModule, modname) == 0) {
                //store module base addr in var
                modBaseAddr = (uintptr_t)modEntry.modBaseAddr;

            }
        }
        CloseHandle(procSnap);
        return modBaseAddr;
    }
};

