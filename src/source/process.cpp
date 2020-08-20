
#include "process.h"
#include <sys/stat.h>
#include <fcntl.h>
process *process::process_instance = nullptr;
//移动环境变量
void process::init_set_proctitle()
{
    if (_envmem != nullptr)
        return;

    _envmem = new char[_env_need_mem];
    memset(_envmem, 0, _env_need_mem);

    char* ptmp = _envmem;
    for(int i = 0;environ[i];i++)
    {
        size_t size = strlen(environ[i])+1;
        strncpy(ptmp,environ[i],size);
        environ[i] = ptmp;
        ptmp += size;
    }

    return;
}

void process::set_proctitle(const std::string &title)
{
    if (_envmem != nullptr)
        return;

    size_t esy = _argv_need_mem + _env_need_mem;
    if(title.length() >= esy)
        return; //title超出 argc 与 环境变量的总长

    //置后续的命令行参数为空，表示只有argv[]中只有一个元素了，这是好习惯；防止后续argv被滥用
    _argv[1] = NULL;
     
    char * ptmp = _argv[0];
    //设置标题
    strncpy(ptmp,title.c_str(),title.length());
    ptmp += title.length();
    size_t cha = esy - title.length();
    memset(ptmp,0,cha);
    return;
}

//创建守护进程
//return fail -1, 子进程 0 ，父进程 1
int process::daemon()
{
    switch (fork())
    {
    case -1: //failed
        return -1;
    case 0: //child
        break;
    default: // father
        return 1;
    }

    _parent_pid = _pid;
    _pid = getpid();

    if(setsid()== -1)
    {
        //todo log
       return -1; 
    }

    umask(0);

    int fd = open("/dev/null",O_RDWR);
    if(fd == -1)
    {
        //todo log
        return -1;
    }
    if(dup2(fd,STDIN_FILENO)== -1)
    {
        //todo logo 
        return -1;
    }
    if(dup2(fd,STDOUT_FILENO)==-1)
    {
        //todo log 
        return -1;
    }
    if(fd > STDERR_FILENO)
    {
        if(close(fd)==-1)
        {
            //rodo log
            return -1;
        }
    }
    return 0;
}