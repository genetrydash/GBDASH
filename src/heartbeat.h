#if !defined(HEARTBEAT_H)
#define HEARTBEAT_H

void irq_handler(void);
void setbeatcallback(void (*callback)(void));
void settickrate(unsigned int rate);
void setbpm(int bpm);
void initirq(void);
void closeirq(void);
void settickcallback(void (*callback)(void));

#endif // HEARTBEAT_H
