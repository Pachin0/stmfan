Havent pushed the code yet! Stop looking. 

# STM32 fancontroller

Hello, this is a basic STM32 based pwm controller for fans of a linux pc. I chose a STM32 due to being cheap and very powerful, but most importantly it is programmable with nothing but free software tools. On the market there does not seem to be PWM controllers that expose a hwmon interface for linux, and so I decided to make my own. This fancontroller uses timers provided in the STM32 to make a standard 21Khz signal that is PWM from 0 to 255. The controller communicates through serial comm and at this moment it is very basic. This is a work in progress. 


## Things to do
 - Implement COBS encoding in our serial comm protocol to packetize the data we exchange. Packets will allow us to control more fans and even use the controller to report various temperatures that could be hooked up to analog read pins.
 - Implement a proper hwmon interface driver. Look for the most elegant way of implementing the driver. It can be kernel driver, it can be userspace, but it has to report to hwmon at the end of the day. Possible implementation is trying to write a kernel driver that probes some way for a magic number that the stm32 can push out from serial comm. This is likely dirty. A mechanism for exposing a hwmon from userspace is likely the best option, more elegant and does not imply littering the kernel with our code.
 - clean up the code so we can programatically handle for fans. Abstracting away the process of setting the timers, alternate functions, etc. To a function that takes a list of pins that support PWM. Then exposes those fans to the driver.
 - Anti ramp up setting. This is to fix loud fans ramping up to 100% unnecesarilly during boot. When the driver is not paired to the computer, such as the times during boot. It can control the fans to some set value that the user gives it.
 - Temperature sensor readings. The stm is perfectly able to analog read pins and send them over. Thing is to do this in a programmatical fashion and make it elegant in the code.
 - Probably rewrite the computer side in haskell. I like haskell, and the task of reporting temperatures and handling the fandriver should be done by a functional program. Functional programs written in haskell have a lot of safety, and we need safety for a fan controller. 

# Why do I do this
I encountered a problem when I bought 12 delta server fans for my pc and realized that the PWM outputs are not usable. The motherboard has 4 pin fan headers but they do not expose a way for them to be controller in linux (don't expose a hwmon interface). My motherboard being so old meant that this functionality was likely not possible to implement. Looking for solutions I found no one. No one sells commertial fancontrollers that report to linux a hwmon interface. Annoyed I looked at how to do my own. I figured pc motherboards have internal USB and often serial ports. Any of those could be used to connect to the fancontroller hardware. Also this is useful for retrofitting server fans, they are sometimes overpowered and loud so a controller with hwmon interface would be a lifesaver. This is also a nice university project to have around. Low level embedded programming is a valuable skill. 
