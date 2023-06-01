#include <windows.h>
#include <string>
#include <TlHelp32.h>
#include <iostream>
#include "offsets.h"
#pragma once
class Memory
{
public:
    int ProcessID = NULL;
    HANDLE hProc = NULL;
    uintptr_t baseAddress = NULL;
    bool terminateProcess = false;
    LPCWSTR GameName = L"game.exe"; // .exe name of the application/game e.g. (csgo.exe)



    Memory() {
        //Process ID
        ProcessID = FindProcId(GameName);
        if (ProcessID == 0) { terminateProcess = true; return; } // check if PID is invalid

        // Module base Adress
        baseAddress = getBaseAddr(ProcessID, GameName);
        if (baseAddress == 0) { terminateProcess = true; return;}// check if Base Adress is invalid

        // Open the process
        hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, ProcessID);
        return;
    }
    ~Memory() {
        //Deleting a memory class
        std::cout << "Shutting down memory class" << "\n";
        if (hProc) { CloseHandle(hProc); };//wierd crash
        return;
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
                std::cout << "ProcessId: " << ProcID << "\n";
                

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
                std::cout << std::hex << "Module base adress: 0x" << modBaseAddr << "\n" << std::dec;

            }
        }
        CloseHandle(procSnap);
        return modBaseAddr;
    }
};

