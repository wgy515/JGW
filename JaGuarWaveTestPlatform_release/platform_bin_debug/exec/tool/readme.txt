1.将需要的xxx.img放到data目录下
2.连上adb，执行所要的命令，如下：

//清空misc分区(清除所有标记位)
adb shell dd if=/data/jw_clear.img  of=/dev/block/sda4
//写入misc分区(写入所有标记位:ffbm-01/root/diag口)
adb shell dd if=/data/misc_smt.img  of=/dev/block/sda4
//写入misc分区(只写入root/diag控制标记位)
adb shell dd if=/data/jw_flag.img  of=/dev/block/sda4