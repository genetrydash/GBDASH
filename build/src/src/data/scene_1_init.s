.module scene_1_init

.include "vm.i"
.include "data/game_globals.i"

.globl _fade_frames_per_step

.area _CODE_255


___bank_scene_1_init = 255
.globl ___bank_scene_1_init

_scene_1_init::
        VM_LOCK

GBVM$scene_1_init$autofade$5b91a2b7_124a_495d_a0cb_c96c1550e98f$scene$5b91a2b7_124a_495d_a0cb_c96c1550e98f$script = .
.globl GBVM$scene_1_init$autofade$5b91a2b7_124a_495d_a0cb_c96c1550e98f$scene$5b91a2b7_124a_495d_a0cb_c96c1550e98f$script
        ; Wait 1 Frames
        VM_IDLE

        ; Fade In
        VM_SET_CONST_INT8       _fade_frames_per_step, 1
        VM_FADE_IN              1

        ; Stop Script
        VM_STOP
