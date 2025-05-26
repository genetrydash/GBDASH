// music_registry.c
extern void register1(void);
// Add extern declarations here as you add more music modules

typedef void (*RegisterFunc)(void);

RegisterFunc music_registers[] = {
    register1,
    // Add more register functions here
    0
};

void register_all_music(void) {
    int i;
    for (i = 0; music_registers[i] != 0; i++) {
        music_registers[i]();
    }
}
