# Vexriscv-briey-FPGA
程序下载流程：

1.将移植好的briey_SOC烧录进高云FPGA,将赛灵思JTAG_SMT2下载器的JTAG接口连接到对应IO口

mcu_TDO-->F14

mcu_TCK -->C17

mcu_TDI  -->C18

mcu_TMS-->G14

2.Eclipse 编译和调试

使用Eclipse完成对VexRiscvSocSoftware中uart例程的编译，生成相应的elf文件

注意修改链接脚本

VexRiscvSocSoftware/projects/briey/libs/linker.ld

3.在unbuntu系统中openocd_riscv文件夹下运行指令

sudo src/openocd -f tcl/interface/ftdi/digilent_jtag_smt2.cfg  -c "set BRIEY_CPU0_YAML /home/liujinshuo/FPGA_compare/VexRiscv/cpu0.yaml" -f tcl/target/briey.cfg

完成PC与CPU的连接

运行前可以使用lsusb指令查看JTAG_SMT2是否连接到虚拟机。

4.在VexRiscvSocSoftware的上级目录运行GDB调试指令 

/opt/Riscv-toolchain/bin/riscv64-unknown-elf-gdb VexRiscvSocSoftware/projects/briey/uart/build/uart.elf  -ex "target extended-remote localhost:3333"

monitor reset halt

load

continue 

5.printf函数实现

将VexRiscvSocSoftware/libs文件夹下的uart.h替换为本文件加下的uart.h即可。

调用myprintf函数打印输出。
