################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/include" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/mqtt_client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/platform/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink_extlib/provisioninglib" -g --gcc --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

button_if.obj: C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/button_if.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/include" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/mqtt_client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/platform/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink_extlib/provisioninglib" -g --gcc --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="button_if.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

gpio_if.obj: C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/gpio_if.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/include" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/mqtt_client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/platform/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink_extlib/provisioninglib" -g --gcc --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="gpio_if.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/include" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/mqtt_client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/platform/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink_extlib/provisioninglib" -g --gcc --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

network_common.obj: C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/network_common.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/include" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/mqtt_client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/platform/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink_extlib/provisioninglib" -g --gcc --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="network_common.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

network_if.obj: C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/network_if.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/include" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/mqtt_client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/platform/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink_extlib/provisioninglib" -g --gcc --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="network_if.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_ccs.obj: C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/startup_ccs.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/include" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/mqtt_client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/platform/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink_extlib/provisioninglib" -g --gcc --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="startup_ccs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

timer_if.obj: C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/timer_if.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/include" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/mqtt_client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/platform/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink_extlib/provisioninglib" -g --gcc --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="timer_if.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

uart_if.obj: C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/uart_if.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_5.2.9/include" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/mqtt_client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/client/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/common/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/platform/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/netapps/mqtt/include/" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink_extlib/provisioninglib" -g --gcc --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="uart_if.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


