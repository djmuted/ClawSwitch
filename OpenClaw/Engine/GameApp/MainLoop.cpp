#include "BaseGameApp.h"
#include "MainLoop.h"
#include <fstream>
#include <iostream>
#include <switch.h>

alignas(16) u8 __nx_exception_stack[0x1000];
u64 __nx_exception_stack_size = sizeof(__nx_exception_stack);

void __libnx_exception_handler(ThreadExceptionDump *ctx)
{
    int i;
    FILE *f = fopen("exception_dump", "w");
    if(f==NULL)return;

    fprintf(f, "error_desc: 0x%x\n", ctx->error_desc);//You can also parse this with ThreadExceptionDesc.
    //This assumes AArch64, however you can also use threadExceptionIsAArch64().
    for(i=0; i<29; i++)fprintf(f, "[X%d]: 0x%lx\n", i, ctx->cpu_gprs[i].x);
    fprintf(f, "fp: 0x%lx\n", ctx->fp.x);
    fprintf(f, "lr: 0x%lx\n", ctx->lr.x);
    fprintf(f, "sp: 0x%lx\n", ctx->sp.x);
    fprintf(f, "pc: 0x%lx\n", ctx->pc.x);

    //You could print fpu_gprs if you want.

    fprintf(f, "pstate: 0x%x\n", ctx->pstate);
    fprintf(f, "afsr0: 0x%x\n", ctx->afsr0);
    fprintf(f, "afsr1: 0x%x\n", ctx->afsr1);
    fprintf(f, "esr: 0x%x\n", ctx->esr);

    fprintf(f, "far: 0x%lx\n", ctx->far.x);

    fclose(f);
}

std::ofstream logout("/switch/claw/log.txt");
void myLogFn(void* userdata, int category, SDL_LogPriority priority,
const char* message)
{
logout<<message<<std::endl;
}
int RunGameEngine(int argc, char** argv)
{
	SDL_LogSetOutputFunction(&myLogFn, NULL);
    if (SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH) != 0)
    {
        LOG_WARNING("Failed to set high priority class to this process");
    }

std::string userDirectory = "/switch/claw/";

	//userDirectory = "";
    LOG("Looking for: " + userDirectory + "config.xml");
    LOG("Looking for: " + userDirectory + "config.xml");

    LOG("Expecting config.xml in path: " + userDirectory + "config.xml");

    // Load options
    if (!g_pApp->LoadGameOptions(std::string(userDirectory + "config.xml").c_str()))
    {
        LOG_ERROR("Could not load game options. Exiting.");
        return -1;
    }

    g_pApp->GetGameConfig()->userDirectory = userDirectory;

    std::string savesFilePath = g_pApp->GetGameConfig()->userDirectory + g_pApp->GetGameConfig()->savesFile;
    LOG("Loaded with:\n\tConfig File: " + userDirectory + "config.xml" + "\n\tSaves File: " + savesFilePath);


    // Initialize game instance
    if (!g_pApp->Initialize(argc, argv))
    {
        LOG_ERROR("Failed to initialize. Exiting.");
        return -1;
    }

    // Run the game
    return g_pApp->Run();
}
