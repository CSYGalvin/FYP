# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct C:\Users\Galnerd\FYP\Dummy_Crypto_Project\workspace\Dummy_Crypto_Platform\platform.tcl
# 
# OR launch xsct and run below command.
# source C:\Users\Galnerd\FYP\Dummy_Crypto_Project\workspace\Dummy_Crypto_Platform\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {Dummy_Crypto_Platform}\
-hw {C:\Users\Galnerd\FYP\Dummy_Crypto_Project\design_1_wrapper.xsa}\
-proc {ps7_cortexa9_0} -os {standalone} -out {C:/Users/Galnerd/FYP/Dummy_Crypto_Project/workspace}

platform write
platform generate -domains 
platform active {Dummy_Crypto_Platform}
platform generate
platform config -updatehw {C:/Users/Galnerd/FYP/Dummy_Crypto_Project/design_1_wrapper.xsa}
platform generate -domains 
platform config -updatehw {C:/Users/Galnerd/FYP/Dummy_Crypto_Project/design_1_wrapper.xsa}
platform generate -domains 
platform active {Dummy_Crypto_Platform}
platform config -updatehw {C:/Users/Galnerd/FYP/Dummy_Crypto_Project/design_1_wrapper.xsa}
platform generate -domains 
platform config -updatehw {C:/Users/Galnerd/FYP/Dummy_Crypto_Project/design_1_wrapper.xsa}
platform config -updatehw {C:/Users/Galnerd/FYP/Dummy_Crypto_Project/design_1_wrapper.xsa}
platform generate -domains 
platform active {Dummy_Crypto_Platform}
platform config -updatehw {C:/Users/Galnerd/FYP/Dummy_Crypto_Project/design_1_wrapper.xsa}
platform generate -domains 
platform active {Dummy_Crypto_Platform}
platform config -updatehw {C:/Users/Galnerd/FYP/Dummy_Crypto_Project/design_1_wrapper.xsa}
platform generate -domains 
platform config -updatehw {C:/Users/Galnerd/FYP/Dummy_Crypto_Project/design_1_wrapper.xsa}
platform generate -domains 
platform config -updatehw {C:/Users/Galnerd/FYP/Dummy_Crypto_Project/design_1_wrapper.xsa}
platform generate -domains 
platform config -updatehw {C:/Users/Galnerd/FYP/Dummy_Crypto_Project/design_1_wrapper.xsa}
platform generate -domains 
platform config -updatehw {C:/Users/Galnerd/FYP/Dummy_Crypto_Project/design_1_wrapper.xsa}
platform generate -domains 
platform config -updatehw {C:/Users/Galnerd/FYP/Dummy_Crypto_Project/design_1_wrapper.xsa}
platform generate -domains 
