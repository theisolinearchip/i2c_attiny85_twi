# I2C libraries for attiny85-like AVR micros 

A pair of __I2C libraries__ for __AVR micros__ with __USI two-wire mode__ capabilities (developed and tested on __attiny85__'s, but portable - in theory! - to similar devices with the proper pin changes) + a bunch of examples both with __primary__ and __secondary__ devices.

The goal here is to use them as the main entry point when dealing with __I2C__ in future projects (and improve them over time, of course!), so here's a fully functional micro-example of a __primary device__ reading from a __secondary device__ and writing into a third __secondary one__ (bonus: connection with a a __ssd1306 oled screen__!).

More info on [my hackaday.io project page](https://hackaday.io/project/183814-i2c-libraries-for-attiny85-like-avr-micros).

![I2C attiny85 TWI](http://albertgonzalez.coffee/projects/i2c_attiny85_twi/img/2_700.jpg?)

## I2C primary / secondary

The _i2c/_ folder contains both the primary and secondary libs that allows sending and reading bytes.

For __primary devices__ the approach is the regular one: start the transaction, send the address, send something or read whatever and finish the transaction.

The __secondary lib__ works with interrupts as a simple state machine: each overflow on __USISIF__ or __USIOIF__ (flags from internal status on TWI mode) triggers an interrupt that performs the read or write operation according to the protocol logic.

## Examples

Both _primary/_ and _secondary/_ folders contains a couple of simple examples that shows the whole process:

1. The __primary device__ reads info from the first __secondary one__: a "button device" that puts that button value into a buffer that is being sent when asked (classic "read from secondary" operation).
2. Based on the given value, the __primary one__ changes it's own status led and
3. also sends to a second __secondary device__ (one with three different leds) the specific command to turn on or off those leds ("write to secondary" operation).

The example, even being silly, covers the basics of sending, reading (single and multiple bytes) and acting both as a primary and secondary devices. There's also another connection to a __ssd1306 oled screen__ that prompts a picture at the beginning using the same libs (another proof of concept without any "real application" :_ D).

I tried to comment everything I could on the libs itself and the examples, since I wanted to know exactly why something needs to be done (and sometimes it's kinda hard and that's when the _trial and error_ appears! :D) so I think it's pretty self-explanatory.

(buuuut I'm not an expert on this field, so maybe there're things that can be done easily or some mistakes on my side!)

## Wiring

[Here](http://albertgonzalez.coffee/projects/i2c_attiny85_twi/schematic.pdf)'s a schematic for the wiring used on the example code (or check the _schematic.pdf_ on the main folder). Also keep in mind the following:

- Remember to use __10k__ (or similar) resistors to pull-up the __SDA__ and __SCL__ lines (some devices - like the __NES Mini Controller__! - seems to use it's own internal resistors, so there's no need to add anything if you're just working with one of those).
- The libs are set with the __attiny85__ pins in mind: __SDA__ and __SCL__ on __PB0__ and __PB2__. Needless to say, this needs to be changed for other micros if required.

## TODO, pending, work in progress, etc.

- Test on different micros (working on __attiny85__'s only, at least for now)
- There's no arbitration implemented for __primary devices__
- Testing, testing, testing (and bugfixing)
- More learning: _why is this exactly happening at that moment_, etc.

## Why?

While working with some __NES Mini Controllers__ I started tinkering with the __I2C protocol__ in order to connect them into some __attiny85__'s micros I usually use on my projects ([I managed to do something interesting here!](https://github.com/theisolinearchip/nesmini_usb_adapter)).

First I used a __pure software implementation approach__ and made a bit-banging small driver to act as a __primary device__ and being able to control multiple I2C devices I had (those controllers, some __ssd1306 oled screens__ and so on); but then I started reading about the "native" AVR approach: the Two-Wire mode.

After more reading, poking and trying lots of external references (there're lots of I2C implementations out there, some of them worked, others didn't, another ones are more Arduino-intended, maybe a bunch of them are made on C++...) I wrote this pair of C basic libraries that allows me to manage the __I2C protocol__ on my attiny85's both from __primary__ and __secondary__ sides.

## References

With no particular order, here's a bunch of links and documentation I found useful when writing this. Some of the code provided there is wrong or broken, but I used it as a reference to start fixing and debugging my own approach (other parts are more similar, like the state machine on the __secondary side__ that can also be found [here](https://github.com/CalcProgrammer1/Stepper-Motor-Controller/blob/master/UnipolarStepperDriver/usi_i2c_slave.c).

- [attiny85 datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf), check the two-wire mode section with the registers list!
- [Unipolar Stepper Driver](https://github.com/CalcProgrammer1/Stepper-Motor-Controller/blob/master/UnipolarStepperDriver), I found the state machine approach for the _secondary devices_ here.
- [AVR TWI: Two Wire Interface](https://rvzvolta.wordpress.com/2021/04/27/i2c-attiny85-maestro-y-esclavo-avr-twi-two-wire-interface/) (in spanish only), the first I2C-with-TWI approach I found. The examples doesn't work pretty well, but I used them as a starting point to dig in the documentation.
- [attiny85 I2C protocol tutorial](https://www.gadgetronicx.com/attiny85-i2c-protocol-tutorial/), again, the code on this one doesn't seem to work, but the registers explanation is pretty good.
- [Setting up an ATtiny45 as I2C master - SDA stuck low](https://electronics.stackexchange.com/questions/58944/setting-up-an-attiny45-as-i2c-master-sda-stuck-low), remember to release your SDA and SCL lines!
