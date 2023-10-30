# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: C:\Users\Galnerd\FYP\Dummy_Crypto_Project\workspace\Dummy_Crypto_Application_system\_ide\scripts\ila.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source C:\Users\Galnerd\FYP\Dummy_Crypto_Project\workspace\Dummy_Crypto_Application_system\_ide\scripts\ila.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw C:/Users/Galnerd/FYP/Dummy_Crypto_Project/workspace/Dummy_Crypto_Platform/export/Dummy_Crypto_Platform/hw/design_1_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source C:/Users/Galnerd/FYP/Dummy_Crypto_Project/workspace/Dummy_Crypto_Application/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow C:/Users/Galnerd/FYP/Dummy_Crypto_Project/workspace/Dummy_Crypto_Application/Debug/Dummy_Crypto_Application.elf
configparams force-mem-access 0
bpadd -addr &main
