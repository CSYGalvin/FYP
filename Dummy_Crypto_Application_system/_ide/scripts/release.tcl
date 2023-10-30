# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: C:\Users\Galnerd\FYP\Dummy_Crypto_Project\workspace\Dummy_Crypto_Application_system\_ide\scripts\release.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source C:\Users\Galnerd\FYP\Dummy_Crypto_Project\workspace\Dummy_Crypto_Application_system\_ide\scripts\release.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent JTAG-SMT1 210203B70A1DA" && level==0 && jtag_device_ctx=="jsn-JTAG-SMT1-210203B70A1DA-23727093-0"}
fpga -file C:/Users/Galnerd/FYP/Dummy_Crypto_Project/workspace/Dummy_Crypto_Application/_ide/bitstream/design_1_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw C:/Users/Galnerd/FYP/Dummy_Crypto_Project/workspace/Dummy_Crypto_Platform/export/Dummy_Crypto_Platform/hw/design_1_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source C:/Users/Galnerd/FYP/Dummy_Crypto_Project/workspace/Dummy_Crypto_Application/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow C:/Users/Galnerd/FYP/Dummy_Crypto_Project/workspace/Dummy_Crypto_Application/Release/Dummy_Crypto_Application.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "*A9*#0"}
con
