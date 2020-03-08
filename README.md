# Project 5

# Places that require your implementaion
- For timer-interrupt-induced context switch to work
    - [scheduler](https://github.com/sklaw/enee447project5_hw_template_Shuangqi_sessions/blob/master/threads.c#L109-L153): you need to implement `THREAD_INIT` and `THREAD_RUN`.
- For svc-interrupt-induced context switch to work
    - [scheduler](https://github.com/sklaw/enee447project5_hw_template_Shuangqi_sessions/blob/master/threads.c#L109-L153): you need to implement `THREAD_INIT`, `THREAD_RUN` and `THREAD_SLEEP` case.
    - [case SYSCALL_RD_WORD in trap_handler](https://github.com/sklaw/enee447project5_hw_template_Shuangqi_sessions/blob/master/threads.c#L157-L179)
    - [case SYSCALL_WR_WORD in trap_handler](https://github.com/sklaw/enee447project5_hw_template_Shuangqi_sessions/blob/master/trap_handlers.c#L62-L74)
    - [do_dev_word](https://github.com/sklaw/enee447project5_hw_template_Shuangqi_sessions/blob/master/io.c#L276-L309)
    - [wake_thread](https://github.com/sklaw/enee447project5_hw_template_Shuangqi_sessions/blob/master/threads.c#L157-L179)

## How is the first thread started
![](https://github.com/sklaw/enee447project5_hw_template_Shuangqi_sessions/blob/master/images_used_by_README/how_the_first_thread_is_started.jpg)

## Context switch triggered by timer interrupt
![](https://github.com/sklaw/enee447project5_hw_template_Shuangqi_sessions/blob/master/images_used_by_README/context_swtich_triggered_by_timer_interrupt.jpg)

## Context swtich triggered by SVC call
![](https://github.com/sklaw/enee447project5_hw_template_Shuangqi_sessions/blob/master/images_used_by_README/context_switch_triggered_by_svc_call.jpg)
