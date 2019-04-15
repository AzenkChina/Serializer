--lua script language

local input = require "input"
local power = require "power"

--设置功率源参数
print("Setting power source...")

power.set(__Item.Voltage, __Phase.A, 231.54)
power.set(__Item.Voltage, __Phase.B, 228.21)
power.set(__Item.Voltage, __Phase.C, 230.33)

power.set(__Item.Current, __Phase.A, 1.02)
power.set(__Item.Current, __Phase.B, 2.21)
power.set(__Item.Current, __Phase.C, 1.33)
power.set(__Item.Current, __Phase.N, 0)

power.set(__Item.Angle, __Phase.A, 30)
power.set(__Item.Angle, __Phase.B, 15)
power.set(__Item.Angle, __Phase.C, 45)

power.set(__Item.Const, __Quadrant.Active, 6400)
power.set(__Item.Const, __Quadrant.Reactive, 6400)

power.set(__Item.Inverse, __Voltage.Normal)

power.set(__Item.Mode, __Mode.Four)

print("Setting power source done.")




--等待3秒
print("sleep 3 second.")
os.execute("sleep 3")



--启动功率源
print("Source starting...")

power.start()

--等待30秒
print("sleep 30 second.")
os.execute("sleep 30")

--按键测试
print("press DOWN key.")
input.keyboard(__KeyID.DOWN, __KeyOption.PRESSED)
os.execute("sleep 3")

print("press DOWN key.")
input.keyboard(__KeyID.DOWN, __KeyOption.PRESSED)
os.execute("sleep 3")

print("press DOWN key.")
input.keyboard(__KeyID.DOWN, __KeyOption.PRESSED)
os.execute("sleep 3")

print("press DOWN key.")
input.keyboard(__KeyID.DOWN, __KeyOption.PRESSED)
os.execute("sleep 3")

print("press DOWN key.")
input.keyboard(__KeyID.DOWN, __KeyOption.PRESSED)
os.execute("sleep 3")

print("press DOWN key.")
input.keyboard(__KeyID.DOWN, __KeyOption.PRESSED)
os.execute("sleep 3")


print("press UP key.")
input.keyboard(__KeyID.UP, __KeyOption.PRESSED)
os.execute("sleep 3")

print("press UP key.")
input.keyboard(__KeyID.UP, __KeyOption.PRESSED)
os.execute("sleep 3")

print("press UP key.")
input.keyboard(__KeyID.UP, __KeyOption.PRESSED)
os.execute("sleep 3")

print("press UP key.")
input.keyboard(__KeyID.UP, __KeyOption.PRESSED)
os.execute("sleep 3")

print("press UP key.")
input.keyboard(__KeyID.UP, __KeyOption.PRESSED)
os.execute("sleep 3")

print("press UP key.")
input.keyboard(__KeyID.UP, __KeyOption.PRESSED)
os.execute("sleep 3")

print("press UP key.")
input.keyboard(__KeyID.UP, __KeyOption.PRESSED)
os.execute("sleep 3")



print("press 0 key.")
input.keyboard(__KeyID.N0, __KeyOption.PRESSED)
os.execute("sleep 5")

print("release 0 key.")
input.keyboard(__KeyID.N0, __KeyOption.RELEASED)
os.execute("sleep 5")

print("long press 0 key.")
input.keyboard(__KeyID.N0, __KeyOption.LONG_PRESSED)
os.execute("sleep 5")

print("long release 0 key.")
input.keyboard(__KeyID.N0, __KeyOption.LONG_RELEASED)
os.execute("sleep 5")


--关闭功率源
power.stop()

print("Source stopped.")
