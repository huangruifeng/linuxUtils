#pragma once
#include<cstdlib>
#include<cstring>
#include<string>
#include<unistd.h>
class process{
//移动环境变量
void init_set_proctitle();

void set_proctitle(const std::string& title);

int daemon();

~process()
{
    if (_envmem != nullptr)
        free(_envmem);
}

static process *get_instance(int argc, char *const *argv)
{
    if (process_instance == nullptr)
    {
        process_instance = new process(argc,argv);
    }
    return process_instance;
}

private:
    static process *process_instance;

    class Release
    {
    public:
        ~Release()
        {
            if (process::process_instance != nullptr)
            {
                delete process::process_instance;
                process::process_instance = nullptr;
            }
        }
    };
    process(const process&) =delete;
    process& operator= (const process&) =delete;

    process(int argc, char *const *argv) : _argc(argc), _argv((char **)argv),_envmem(nullptr)
    {
        _parent_pid = getppid();
        _pid = getpid();
        
        _argv_need_mem = 0;
        int i = 0;
        for(i =0 ;i<argc;i++)
        {
            _argv_need_mem += strlen(argv[i])+1;//+1 预留'\0'
        }
        _env_need_mem = 0;
        for(i = 0;environ[i];i++)
        {
            _env_need_mem += strlen(environ[i]) + 1;
        }
    }
    char* _envmem;
    char ** _argv;
    int _argc;
    size_t _argv_need_mem;
    size_t _env_need_mem;

    pid_t _parent_pid;
    pid_t _pid;
};
