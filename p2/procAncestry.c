#include <asm/current.h>
#include <asm/errno.h>
//#include <asm-generic/uaccess.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/syscalls.h>

unsigned long **sys_call_table;

struct ancestry {
    pid_t ancestors[10];
    pid_t children[100];
    pid_t siblings[100];
};

asmlinkage long (*ref_sys_cs3013_syscall1)(void);
asmlinkage long (*ref_sys_cs3013_syscall2)(void);

asmlinkage long new_sys_cs3013_syscall1(void) {
  printk(KERN_INFO "\"'Hello world?!' More like 'Goodbye, world!' EXTERMINATE!\" -- Dalek");
  return 0;
}

static void readChildren(pid_t list[100], struct list_head *head) {
  struct list_head *position;
  int i = 0;
  list_for_each(position, head) {
    struct task_struct *task = list_entry(position, struct task_struct, children);
    list[i++] = task->pid;
  }
}

static void readSiblings(pid_t list[100], struct list_head *head) {
  struct list_head *position;
  int i = 0;
  list_for_each(position, head) {
    struct task_struct *task = list_entry(position, struct task_struct, sibling);
    list[i++] = task->pid;
  }
}

asmlinkage long procAncestry(unsigned short *target_pid, struct ancestry *response) {
    int result = 0;
    unsigned short t_pid;
    struct ancestry data;
    struct task_struct *task_data;
    struct list_head *children_list;
    struct list_head *siblings_list;

    int i;
    for(i = 0; i < 10; i ++) {
        data.ancestors[i] = 0;
    }
    for(i = 0; i < 100; i++) {
        data.children[i] = 0;
        data.siblings[i] = 0;
    }

    if(copy_from_user(&t_pid, target_pid, sizeof(unsigned short))) {
      return EFAULT;
    }
    task_data = current;
    children_list = &current->children;
    siblings_list = &current->sibling;

    readChildren(data.children, children_list);
    readSiblings(data.siblings, siblings_list);

    data.children[0] = 12345;
    if(copy_to_user(response, &data, sizeof(struct ancestry))) {
      return EFAULT;
    }
    return result;
}

static unsigned long **find_sys_call_table(void) {
  unsigned long int offset = PAGE_OFFSET;
  unsigned long **sct;
  
  while (offset < ULLONG_MAX) {
    sct = (unsigned long **)offset;

    if (sct[__NR_close] == (unsigned long *) sys_close) {
      printk(KERN_INFO "Interceptor: Found syscall table at address: 0x%02lX",
	     (unsigned long) sct);
      return sct;
    }
    
    offset += sizeof(void *);
  }
  
  return NULL;
}

static void disable_page_protection(void) {
  /*
    Control Register 0 (cr0) governs how the CPU operates.

    Bit #16, if set, prevents the CPU from writing to memory marked as
    read only. Well, our system call table meets that description.
    But, we can simply turn off this bit in cr0 to allow us to make
    changes. We read in the current value of the register (32 or 64
    bits wide), and AND that with a value where all bits are 0 except
    the 16th bit (using a negation operation), causing the write_cr0
    value to have the 16th bit cleared (with all other bits staying
    the same. We will thus be able to write to the protected memory.

    It's good to be the kernel!
  */
  write_cr0 (read_cr0 () & (~ 0x10000));
}

static void enable_page_protection(void) {
  /*
   See the above description for cr0. Here, we use an OR to set the 
   16th bit to re-enable write protection on the CPU.
  */
  write_cr0 (read_cr0 () | 0x10000);
}

static int __init interceptor_start(void) {
  /* Find the system call table */
  if(!(sys_call_table = find_sys_call_table())) {
    /* Well, that didn't work. 
       Cancel the module loading step. */
    return -1;
  }
  
  /* Store a copy of all the existing functions */
  ref_sys_cs3013_syscall1 = (void *)sys_call_table[__NR_cs3013_syscall1];
  ref_sys_cs3013_syscall2 = (void *)sys_call_table[__NR_cs3013_syscall2];

  /* Replace the existing system calls */
  disable_page_protection();

  sys_call_table[__NR_cs3013_syscall1] = (unsigned long *)new_sys_cs3013_syscall1;
  sys_call_table[__NR_cs3013_syscall2] = (unsigned long *)procAncestry;

  enable_page_protection();
  
  /* And indicate the load was successful */
  printk(KERN_INFO "Loaded interceptor!");

  return 0;
}

static void __exit interceptor_end(void) {
  /* If we don't know what the syscall table is, don't bother. */
  if(!sys_call_table)
    return;
  
  /* Revert all system calls to what they were before we began. */
  disable_page_protection();

  sys_call_table[__NR_cs3013_syscall1] = (unsigned long *)ref_sys_cs3013_syscall1;
  sys_call_table[__NR_cs3013_syscall2] = (unsigned long *)ref_sys_cs3013_syscall2;
  
  enable_page_protection();

  printk(KERN_INFO "Unloaded interceptor!");
}

MODULE_LICENSE("GPL");
module_init(interceptor_start);
module_exit(interceptor_end);