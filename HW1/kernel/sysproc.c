#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_set_affinity_mask(void)
{
  int mask;
  struct proc *p = myproc();
  argint(0, &mask);
  p->affinity_mask = mask;
  p->effective_affinity_mask = mask;
  return 0;
}

uint64
sys_exit(void)
{
  int n;
  char exitMsg[32];
  char buffer[32];
  uint64 stringAddr;
  int errorArgString;
  argint(0, &n);
  argaddr(1, &stringAddr);
  errorArgString = argstr(1, buffer, 32);
  if (stringAddr==0)
  {
    strncpy(exitMsg, "No exit message",strlen("No exit message"));
  }
  else if (errorArgString < 0)
  {
    strncpy(exitMsg, "Coudn't read exit massage",32);
  }
  else
  {
    strncpy(exitMsg, buffer,32);
  }
  strncpy(myproc()->exit_msg, exitMsg,strlen(exitMsg));
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 exitMsg;
  uint64 p;
  argaddr(0, &p);
  argaddr(1, &exitMsg);
  return wait(p,exitMsg);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//return the size of the running process’ memory in bytes. 
uint64
sys_memsize(void)
{
  return myproc()->sz;
}
