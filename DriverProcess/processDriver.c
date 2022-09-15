#include <ntddk.h>


#define UniqueProcessId 0x440   // win 10  eprocess  structure offset
#define ActiveProcessLinks 0x448 // win 10  eprocess structure  offset
#define ImageFileName 0x5a8 // win 10  eprocess structure offset
// these offsets can be different which  depend on  the version of your windows system .

VOID DriverUnload(PDRIVER_OBJECT  DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);
    DbgPrintEx(0,0,"[%ws] Unload Successful \n", __FUNCTIONW__);
}

NTSTATUS Get_All_Process()
{
    UINT64 process_pid = 0;
    PUCHAR process_name = NULL;
    PLIST_ENTRY process_list = NULL;
    PEPROCESS process_first = NULL;
    PEPROCESS process_address = PsGetCurrentProcess();// get current Eprocess
    
    process_list = (PLIST_ENTRY)((UINT64)process_address + ActiveProcessLinks);//ActiveProcessLinks
    //process_list is a doubly linked list¡£
    //You can start traversing at any starting node
    process_first = (PEPROCESS)((UINT64)(process_list->Blink) - ActiveProcessLinks);

    if (!process_address)
    {
        DbgPrintEx(0,0,"[ERROR]: NOT ....\n");
        return STATUS_SEVERITY_ERROR;
    }
    while (process_address)
    {
        process_pid = *(UINT64*)((UINT64)process_address + UniqueProcessId);
        process_name = (PUCHAR)((UINT64)process_address + ImageFileName);
        DbgPrintEx(0,0,"pid = %ld  name = %s \n", process_pid, process_name);
        process_list = process_list->Flink;//jsut go forward 
        process_address = (PEPROCESS)((UINT64)(process_list)-ActiveProcessLinks);
        if (process_first == process_address)//back to the starting point
        {
            DbgPrintEx(0,0,"END!......\n");
            break;
        }
    }
    return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT  DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);
    DbgPrintEx(0,0,"[%ws] [OK] \n", __FUNCTIONW__);
    Get_All_Process();
    DriverObject->DriverUnload = DriverUnload;
    return STATUS_SUCCESS;
}