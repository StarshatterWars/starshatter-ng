/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo

*/

#define NOMINMAX

#include <algorithm>

#include <windows.h>
#include <imagehlp.h>

#include "Utils.h"
#include <cinttypes>   // PRIuPTR / PRIxPTR if you want them
#include <cstdint>     // uintptr_t

// +--------------------------------------------------------------------+

class ExceptionHandler
{
public:
    ExceptionHandler();
    ~ExceptionHandler();

private:
    static LPTOP_LEVEL_EXCEPTION_FILTER old_filter;

    static LONG WINAPI   ExceptionFilter(EXCEPTION_POINTERS* info);

    static void          PrintReport(EXCEPTION_POINTERS* info);

    // Helper functions
    static const char*   GetExceptionString(DWORD dwCode);
    static BOOL          GetLogicalAddress(VOID* addr, char* module, int len,
    DWORD& section, DWORD& offset);

    static BOOL          InitImageHelp();
    static void          ImageStackTrace(CONTEXT* context);
    static void          IntelStackTrace(CONTEXT* context);


    // Make typedefs for some IMAGEHLP.DLL functions so that we can use them
    // with GetProcAddress
    typedef BOOL (__stdcall * SYMINITIALIZEPROC)(HANDLE, LPSTR, BOOL);
    typedef BOOL (__stdcall * SYMCLEANUPPROC)(HANDLE);

    typedef LPVOID (__stdcall *SYMFUNCTIONTABLEACCESSPROC)(HANDLE, DWORD);
    typedef DWORD (__stdcall *SYMGETMODULEBASEPROC)(HANDLE, DWORD);
    typedef BOOL (__stdcall *SYMGETSYMFROMADDRPROC)(HANDLE, DWORD, PDWORD, PIMAGEHLP_SYMBOL);

    typedef BOOL (__stdcall * STACKWALKPROC)(DWORD,
    HANDLE,
    HANDLE,
    LPSTACKFRAME,
    LPVOID,
    PREAD_PROCESS_MEMORY_ROUTINE,
    PFUNCTION_TABLE_ACCESS_ROUTINE,
    PGET_MODULE_BASE_ROUTINE,
    PTRANSLATE_ADDRESS_ROUTINE);

    static SYMINITIALIZEPROC            SymInitialize;
    static SYMCLEANUPPROC               SymCleanup;
    static STACKWALKPROC                StackTrace;
    static SYMFUNCTIONTABLEACCESSPROC   SymFunctionTableAccess;
    static SYMGETMODULEBASEPROC         SymGetModuleBase;
    static SYMGETSYMFROMADDRPROC        SymGetSymFromAddr;
};

// +--------------------------------------------------------------------+

LPTOP_LEVEL_EXCEPTION_FILTER ExceptionHandler::old_filter = 0;

ExceptionHandler::SYMINITIALIZEPROC ExceptionHandler::SymInitialize = 0;
ExceptionHandler::SYMCLEANUPPROC    ExceptionHandler::SymCleanup = 0;
ExceptionHandler::STACKWALKPROC     ExceptionHandler::StackTrace = 0;

ExceptionHandler::SYMFUNCTIONTABLEACCESSPROC
ExceptionHandler::SymFunctionTableAccess = 0;

ExceptionHandler::SYMGETMODULEBASEPROC
ExceptionHandler::SymGetModuleBase = 0;

ExceptionHandler::SYMGETSYMFROMADDRPROC
ExceptionHandler::SymGetSymFromAddr = 0;

ExceptionHandler global_exception_handler;


// +--------------------------------------------------------------------+

ExceptionHandler::ExceptionHandler()
{
    old_filter = SetUnhandledExceptionFilter(ExceptionFilter);
}

ExceptionHandler::~ExceptionHandler()
{
    SetUnhandledExceptionFilter(old_filter);
}

// +--------------------------------------------------------------------+

static bool in_filter = false;

void ExceptionHandler::IntelStackTrace(CONTEXT* context)
{
#if defined(_M_IX86)
    Print("\nStack Trace (Intel x86):\n");
    Print("Address   Frame     Logical addr  Module\n");

    DWORD  pc = context->Eip;
    DWORD* pFrame = (DWORD*)context->Ebp;

    while (pFrame)
    {
        char  mod_name[256] = { 0 };
        DWORD section = 0, offset = 0;

        GetLogicalAddress((void*)(uintptr_t)pc, mod_name, 256, section, offset);

        Print("%08X  %08X  %04X:%08X %s\n",
            (unsigned)pc, (unsigned)(uintptr_t)pFrame,
            (unsigned)section, (unsigned)offset, mod_name);

        DWORD* pPrevFrame = pFrame;

        // next frame:
        pc = pFrame[1];
        pFrame = (DWORD*)pFrame[0];

        // basic validation:
        if (((uintptr_t)pFrame & 3u) != 0) break;
        if (pFrame <= pPrevFrame) break;

        // This API is deprecated; keep it only if you insist on legacy behavior.
        // Prefer a guarded read (see note below).
        if (IsBadWritePtr(pFrame, sizeof(void*) * 2)) break;
    }
#else
    Print("\nStack Trace (Intel): unavailable on x64. Using ImageHelp stack trace.\n");
#endif
}
// +--------------------------------------------------------------------+

void
ExceptionHandler::PrintReport(EXCEPTION_POINTERS* info)
{
    EXCEPTION_RECORD* record = info->ExceptionRecord;
    CONTEXT* context = info->ContextRecord;
    DWORD             code = record->ExceptionCode;

    Print("\n*********************************************\n");
    Print("FATAL EXCEPTION:\n");

#if defined(_M_X64) || defined(_WIN64)
    Print("\nRegisters (x64):\n");
    Print("RAX: %016llx\n", (unsigned long long)context->Rax);
    Print("RBX: %016llx\n", (unsigned long long)context->Rbx);
    Print("RCX: %016llx\n", (unsigned long long)context->Rcx);
    Print("RDX: %016llx\n", (unsigned long long)context->Rdx);
    Print("RSI: %016llx\n", (unsigned long long)context->Rsi);
    Print("RDI: %016llx\n", (unsigned long long)context->Rdi);
    Print("RBP: %016llx\n", (unsigned long long)context->Rbp);
    Print("RSP: %016llx\n", (unsigned long long)context->Rsp);
    Print("R8 : %016llx\n", (unsigned long long)context->R8);
    Print("R9 : %016llx\n", (unsigned long long)context->R9);
    Print("R10: %016llx\n", (unsigned long long)context->R10);
    Print("R11: %016llx\n", (unsigned long long)context->R11);
    Print("R12: %016llx\n", (unsigned long long)context->R12);
    Print("R13: %016llx\n", (unsigned long long)context->R13);
    Print("R14: %016llx\n", (unsigned long long)context->R14);
    Print("R15: %016llx\n", (unsigned long long)context->R15);
    Print("\n");
    Print("RIP: %016llx\n", (unsigned long long)context->Rip);
    Print("EFlags: %08x\n", (unsigned int)context->EFlags);
    Print("\n");

#else
    Print("\nRegisters (x86):\n");
    Print("EAX:    %08x\n", (unsigned int)context->Eax);
    Print("EBX:    %08x\n", (unsigned int)context->Ebx);
    Print("ECX:    %08x\n", (unsigned int)context->Ecx);
    Print("EDX:    %08x\n", (unsigned int)context->Edx);
    Print("EDI:    %08x\n", (unsigned int)context->Edi);
    Print("ESI:    %08x\n", (unsigned int)context->Esi);
    Print("EBP:    %08x\n", (unsigned int)context->Ebp);
    Print("\n");
    Print("CS:EIP: %04x:%08x\n", (unsigned int)context->SegCs, (unsigned int)context->Eip);
    Print("SS:ESP: %04x:%08x\n", (unsigned int)context->SegSs, (unsigned int)context->Esp);
    Print("DS:     %04x\n", (unsigned int)context->SegDs);
    Print("ES:     %04x\n", (unsigned int)context->SegEs);
    Print("FS:     %04x\n", (unsigned int)context->SegFs);
    Print("GS:     %04x\n", (unsigned int)context->SegGs);
    Print("Flags:  %08x\n", (unsigned int)context->EFlags);
    Print("\n");
#endif

    Print("Exception Code:  %08x %s\n", (unsigned int)code, GetExceptionString(code));

    // Pointer-safe printing:
    Print("Exception Addr:  %p\n", record->ExceptionAddress);

    if (code == EXCEPTION_ACCESS_VIOLATION && record->NumberParameters >= 2) {
        const uintptr_t addr = (uintptr_t)record->ExceptionInformation[1];
        if (record->ExceptionInformation[0]) {
            Print("                 Program attempted to WRITE to address %p\n", (void*)addr);
        }
        else {
            Print("                 Program attempted to READ from address %p\n", (void*)addr);
        }
    }

    if (InitImageHelp()) {
        ImageStackTrace(context);
        SymCleanup(GetCurrentProcess());
    }
    else {
        IntelStackTrace(context);
    }

    Print("\n*********************************************\nPROGRAM TERMINATED.\n");
}
// +--------------------------------------------------------------------+

const char*
ExceptionHandler::GetExceptionString(DWORD code)
{
#define EXCEPTION( x ) case EXCEPTION_##x: return #x;

    switch (code) {
        EXCEPTION( ACCESS_VIOLATION )
        EXCEPTION( DATATYPE_MISALIGNMENT )
        EXCEPTION( BREAKPOINT )
        EXCEPTION( SINGLE_STEP )
        EXCEPTION( ARRAY_BOUNDS_EXCEEDED )
        EXCEPTION( FLT_DENORMAL_OPERAND )
        EXCEPTION( FLT_DIVIDE_BY_ZERO )
        EXCEPTION( FLT_INEXACT_RESULT )
        EXCEPTION( FLT_INVALID_OPERATION )
        EXCEPTION( FLT_OVERFLOW )
        EXCEPTION( FLT_STACK_CHECK )
        EXCEPTION( FLT_UNDERFLOW )
        EXCEPTION( INT_DIVIDE_BY_ZERO )
        EXCEPTION( INT_OVERFLOW )
        EXCEPTION( PRIV_INSTRUCTION )
        EXCEPTION( IN_PAGE_ERROR )
        EXCEPTION( ILLEGAL_INSTRUCTION )
        EXCEPTION( NONCONTINUABLE_EXCEPTION )
        EXCEPTION( STACK_OVERFLOW )
        EXCEPTION( INVALID_DISPOSITION )
        EXCEPTION( GUARD_PAGE )
        EXCEPTION( INVALID_HANDLE )
    }

    static char buffer[512] = { 0 };

    FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE,
    GetModuleHandle("NTDLL.DLL"),
    code, 0, buffer, sizeof(buffer), 0 );

    return buffer;
}

// +--------------------------------------------------------------------+

BOOL
ExceptionHandler::GetLogicalAddress(void* addr, char* mod_name, int len, DWORD& section, DWORD& offset)
{
    MEMORY_BASIC_INFORMATION mbi;

    if (!VirtualQuery(addr, &mbi, sizeof(mbi)))
    return FALSE;

    DWORD hMod = (DWORD)mbi.AllocationBase;

    if (!GetModuleFileName((HMODULE)hMod, mod_name, len))
    return FALSE;

    PIMAGE_DOS_HEADER       pDosHdr  = (PIMAGE_DOS_HEADER) hMod;
    PIMAGE_NT_HEADERS       pNtHdr   = (PIMAGE_NT_HEADERS)(hMod + pDosHdr->e_lfanew);
    PIMAGE_SECTION_HEADER   pSection = IMAGE_FIRST_SECTION( pNtHdr );

    DWORD rva = (DWORD)addr - hMod; // RVA is offset from module load address

    // Iterate through the section table, looking for the one that encompasses
    // the linear address.
    for (unsigned i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++, pSection++ ) {
        DWORD sectionStart = pSection->VirtualAddress;
        DWORD sectionEnd = sectionStart
        + std::max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);

        // Is the address in this section???
        if ((rva >= sectionStart) && (rva <= sectionEnd)) {
            // Yes, address is in the section.  Calculate section and offset,
            // and store in the "section" & "offset" params, which were
            // passed by reference.
            section = i+1;
            offset = rva - sectionStart;
            return TRUE;
        }
    }

    return FALSE;   // Should never get here!
}

// +--------------------------------------------------------------------+

BOOL
ExceptionHandler::InitImageHelp()
{
    Print("\n");

    HMODULE h = LoadLibrary("IMAGEHLP.DLL");
    if (!h) {
        Print("--- could not load IMAGEHLP.DLL (%08x) ---\n", GetLastError());
        return FALSE;
    }

    SymInitialize = (SYMINITIALIZEPROC) GetProcAddress(h, "SymInitialize");
    if (!SymInitialize) {
        Print("--- could not find SymInitialize ---\n");
        return FALSE;
    }

    SymCleanup = (SYMCLEANUPPROC) GetProcAddress(h, "SymCleanup");
    if (!SymCleanup) {
        Print("--- could not find SymCleanup ---\n");
        return FALSE;
    }

    StackTrace = (STACKWALKPROC) GetProcAddress(h, "StackWalk");
    if (!StackTrace) {
        Print("--- could not find StackWalk ---\n");
        return FALSE;
    }

    SymFunctionTableAccess = (SYMFUNCTIONTABLEACCESSPROC)
    GetProcAddress(h, "SymFunctionTableAccess");

    if (!SymFunctionTableAccess) {
        Print("--- could not find SymFunctionTableAccess ---\n");
        return FALSE;
    }

    SymGetModuleBase = (SYMGETMODULEBASEPROC) GetProcAddress(h, "SymGetModuleBase");
    if (!SymGetModuleBase) {
        Print("--- could not find SymGetModuleBase ---\n");
        return FALSE;
    }

    SymGetSymFromAddr = (SYMGETSYMFROMADDRPROC) GetProcAddress(h, "SymGetSymFromAddr");
    if (!SymGetSymFromAddr) {
        Print("--- could not find SymGetSymFromAddr ---\n");
        return FALSE;
    }

    if (!SymInitialize(GetCurrentProcess(), 0, TRUE)) {
        Print("--- could not Initialize IMAGEHLP.DLL (%08x) ---\n", GetLastError());
        return FALSE;
    }

    Print("Loaded IMAGEHLP.DLL\n");
    return TRUE;
}

