#if !defined(REGISTER_H)
#define REGISTER_H

void registermodule(const char *name, void (*init)(void), void (*stopandclean)(void));
void play(char idx);
void stop(char idx);

#endif // REGISTER_H
