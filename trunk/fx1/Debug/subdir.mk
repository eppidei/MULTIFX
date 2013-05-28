################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MULTIFX_FX_class.c \
../MULTIFX_FX_library.c \
../MULTIFX_FX_library_utils.c \
../MULTIFX_MENU_class.c \
../MULTIFX_OSCILLATOR_class.c \
../MULTIFX_oss_conf.c \
../MULTIFX_oss_std_cfg.c \
../MULTIFX_oss_utils.c \
../MULTIFX_threads.c \
../main.c \
../ncurses_UI_class.c 

OBJS += \
./MULTIFX_FX_class.o \
./MULTIFX_FX_library.o \
./MULTIFX_FX_library_utils.o \
./MULTIFX_MENU_class.o \
./MULTIFX_OSCILLATOR_class.o \
./MULTIFX_oss_conf.o \
./MULTIFX_oss_std_cfg.o \
./MULTIFX_oss_utils.o \
./MULTIFX_threads.o \
./main.o \
./ncurses_UI_class.o 

C_DEPS += \
./MULTIFX_FX_class.d \
./MULTIFX_FX_library.d \
./MULTIFX_FX_library_utils.d \
./MULTIFX_MENU_class.d \
./MULTIFX_OSCILLATOR_class.d \
./MULTIFX_oss_conf.d \
./MULTIFX_oss_std_cfg.d \
./MULTIFX_oss_utils.d \
./MULTIFX_threads.d \
./main.d \
./ncurses_UI_class.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/leonardo/MULTIFX_wcpy/trunk/fx1 -I/home/leonardo/Programmi/OSS/oss-v4.2-build2007-src-gpl/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


