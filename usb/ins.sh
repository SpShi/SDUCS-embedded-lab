INS=insmod
RM=rmmod
KODIR=../kernel_module
OP=$INS

$OP $KODIR/softi2c.ko
$OP $KODIR/aht20.ko
$OP $KODIR/ch422g.ko
#$OP $KODIR/stk8ba.ko
$OP $KODIR/led.ko
$OP $KODIR/motor.ko
$OP $KODIR/rfid.ko
$OP $KODIR/key.ko
$OP $KODIR/beep.ko
$OP $KODIR/aip1944.ko
$OP $KODIR/irdrv.ko
