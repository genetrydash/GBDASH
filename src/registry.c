// music_registry.c
#define REGISTER_MODULE(x) extern void register##x(void);
REGISTER_MODULE(1)
REGISTER_MODULE(2)
REGISTER_MODULE(3)

typedef void (*RegisterFunc)(void);

RegisterFunc music_registers[] = {
    register1,
    register2,
    register3,
    // Add more register functions here
    0
};

void register_all_music(void) {
    int i;
    for (i = 0; music_registers[i] != 0; i++) {
        music_registers[i]();
    }
}
