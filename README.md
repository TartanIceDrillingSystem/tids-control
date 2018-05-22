# tids-control
Software for autonomous martian ice extraction

## Pin configuation (must execute before run)
    config-pin P9.14 pwm
    config-pin P9.16 pwm
    config-pin P9.19 i2c
    config-pin P9.20 i2c

## Changes to /boot/uEnv.txt
    disable_uboot_overlay_video=1
    disable_uboot_overlay_audio=1
