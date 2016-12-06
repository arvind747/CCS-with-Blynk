################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
BlynkHandlers.obj: ../BlynkHandlers.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/json/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" -g --define=cc3200 --define=__SL__ --define=ccs --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="BlynkHandlers.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

blynk.obj: ../blynk.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/json/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" -g --define=cc3200 --define=__SL__ --define=ccs --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="blynk.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

blynkDependency.obj: ../blynkDependency.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/json/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" -g --define=cc3200 --define=__SL__ --define=ccs --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="blynkDependency.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

gpio_if.obj: ../gpio_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/json/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" -g --define=cc3200 --define=__SL__ --define=ccs --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="gpio_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/json/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" -g --define=cc3200 --define=__SL__ --define=ccs --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pinmux.obj: ../pinmux.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/json/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" -g --define=cc3200 --define=__SL__ --define=ccs --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="pinmux.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common/startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/json/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" -g --define=cc3200 --define=__SL__ --define=ccs --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

timer_if.obj: ../timer_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/json/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" -g --define=cc3200 --define=__SL__ --define=ccs --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="timer_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart_if.obj: C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common/uart_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/netapps/json/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/driverlib/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/inc/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.1.0/cc3200-sdk/simplelink/include" -g --define=cc3200 --define=__SL__ --define=ccs --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="uart_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


