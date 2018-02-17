#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  int status;
  if(argint(0,&status) < 0)
     return -1;
  exit(status);
  return 0;  // not reached
}

int
sys_wait(void)
{
  int *status;
  argptr(0, (char**) &status, sizeof(int *));
  return wait(status);
}

int
sys_waitpid(void)
{
  int pid, options;
  int *status;
  argint(0,&pid);
  argint(2,&options);
  argptr(1,(char**) &status, sizeof(int*));
  return waitpid(pid, status, options);
}

int 
sys_setprioritypid(void)
{
  int pid, priority;
  argint(0,&pid);
  argint(1,&priority);
  return setprioritypid(pid, priority); 
}

int 
sys_setpriority(void)
{
  int priority;
  argint(0,&priority);
  return setpriority(priority); 
}

int
sys_getpriority(void)
{
   int pid;
   argint(0,&pid);
   return getpriority(pid);
}

int 
sys_timeinfo(void)
{
  int pid;
  argint(0,&pid);
  return timeinfo(pid); 
}

int
sys_up_time(void)
{
    return up_time();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
