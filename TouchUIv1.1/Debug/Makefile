################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../src/ \
../src/QTouch/


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../main.c \
../src/QTouch/BitBangSPI_Master.c \
../src/QTouch/init_mcu_atmega88.c \
../src/QTouch/TWI_Master.c \
../src/QTouch/eeprom_access.c \
../src/QTouch/QDebug.c \
../src/QTouch/QDebugTransport.c \
../touch.c


PREPROCESSING_SRCS +=  \
../src/QTouch/qt_asm_tiny_mega.S


ASM_SRCS += 


OBJS +=  \
main.o \
src/QTouch/BitBangSPI_Master.o \
src/QTouch/init_mcu_atmega88.o \
src/QTouch/TWI_Master.o \
src/QTouch/eeprom_access.o \
src/QTouch/QDebug.o \
src/QTouch/QDebugTransport.o \
src/QTouch/qt_asm_tiny_mega.o \
touch.o

OBJS_AS_ARGS +=  \
main.o \
src/QTouch/BitBangSPI_Master.o \
src/QTouch/init_mcu_atmega88.o \
src/QTouch/TWI_Master.o \
src/QTouch/eeprom_access.o \
src/QTouch/QDebug.o \
src/QTouch/QDebugTransport.o \
src/QTouch/qt_asm_tiny_mega.o \
touch.o

C_DEPS +=  \
main.d \
src/QTouch/BitBangSPI_Master.d \
src/QTouch/init_mcu_atmega88.d \
src/QTouch/TWI_Master.d \
src/QTouch/eeprom_access.d \
src/QTouch/QDebug.d \
src/QTouch/QDebugTransport.d \
src/QTouch/qt_asm_tiny_mega.d \
touch.d

C_DEPS_AS_ARGS +=  \
main.d \
src/QTouch/BitBangSPI_Master.d \
src/QTouch/init_mcu_atmega88.d \
src/QTouch/TWI_Master.d \
src/QTouch/eeprom_access.d \
src/QTouch/QDebug.d \
src/QTouch/QDebugTransport.d \
src/QTouch/qt_asm_tiny_mega.d \
touch.d

OUTPUT_FILE_PATH +=TouchUIv1.1.elf

OUTPUT_FILE_PATH_AS_ARGS +=TouchUIv1.1.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler

















./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"../src/QTouch" -I".."  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega88pa -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/QTouch/%.o: ../src/QTouch/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"../src/QTouch" -I".."  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega88pa -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler
src/QTouch/qt_asm_tiny_mega.o: ../src/QTouch/qt_asm_tiny_mega.S
	@echo Building file: $<
	@echo Invoking: AVR32/GNU Assembler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -Wa,-gdwarf2 -x assembler-with-cpp -c -mmcu=atmega88pa -I "../src/QTouch" -I ".."  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -Wa,-g   -o "$@" "$<" 
	@echo Finished building: $<
	


src/QTouch/%.o: ../src/QTouch/%.S
	@echo Building file: $<
	@echo Invoking: AVR32/GNU Assembler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -Wa,-gdwarf2 -x assembler-with-cpp -c -mmcu=atmega88pa -I "../src/QTouch" -I ".."  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -Wa,-g   -o "$@" "$<" 
	@echo Finished building: $<
	




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="TouchUIv1.1.map" -Wl,--start-group -Wl,-lavr4g1-4qt-k-1rs -Wl,-lm  -Wl,--end-group -Wl,-L"../src/QTouch"  -Wl,--gc-sections -mmcu=atmega88pa  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "TouchUIv1.1.elf" "TouchUIv1.1.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "TouchUIv1.1.elf" "TouchUIv1.1.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "TouchUIv1.1.elf" > "TouchUIv1.1.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "TouchUIv1.1.elf" "TouchUIv1.1.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-size.exe" "TouchUIv1.1.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "TouchUIv1.1.elf" "TouchUIv1.1.a" "TouchUIv1.1.hex" "TouchUIv1.1.lss" "TouchUIv1.1.eep" "TouchUIv1.1.map" "TouchUIv1.1.srec" "TouchUIv1.1.usersignatures"
	