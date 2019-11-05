## PES Project Four Readme

Student Name: Kyle Bryan

##### Running Code, General description

The program is based on the tmp102 temperature sensor.  It runs through 2 different
state machines(state and table based) that each do the same thing.  The program
reads in temperature data and then averages it with the past temperature data. If
the temperature drops below 0C then the program will go to an alert state where
the LED is turned to blue, but will then continue to run as normal.  It
also detects whether the tmp102 has been disconnected and will turn the LED red and
exit the main state machine, waiting for a reset to start the program again.

This code is written to be run on either the FRDM board using MCUExpresso. There
are three targets within the makefile that can be generated.  Make sure you are
using the makefile that is in the /Debug folder as that is how the files are
referenced from.  The three targets are as follows:

- fb_status: runs program and prints out the temperature reading/average
- fb_debug: runs program and has a fairly verbose printout of what the program

- fb_test: runs a brief uCUnit test to show that initializing the hardware has
been done correctly

When implementing the makefile, make sure you are pointing to the debug folder
as all the files are referenced under the assumption that is where the makefile
is.  

e.g:
*Running in MCUExpresso*  
"make -r -f makefile -j8 fb_status"  
"make -r -f makefile -j8 fb_debug"  
"make -r -f makefile -j8 fb_test"  

The external I/O used on the FRDM board are as follows and need to be set up via
the pinout GUI provided in MCUExpresso.

I2C_Clock: Port B0  
I2C_Data: Port B1  
tmp102 Alert: Port A4  


##### Repository Contents
###### Source Code
*main.c*, *led_control.h*, *led_control.c*, *logger.h*,*logger.c*,
*bit.c/h*, *i2c.c/h*, *tmp102.c/h*, *statemachine.c/h*, *makefile*
, *testSuite.c/h*

The main.c is where the main program is run.  It calls the statemachine fucntions
which run the bulk of the program.  The i2c file is mainly used for read/write
via i2c which the tmp102 leverages to do tmp102 specific i2c calls.  The bit file
contains a test to see if the tmp102 is responding properly or not.  The logger
is used to write program info to the console, while the LED-control is used to
control the LED showing status of the program.

There is also the TestSuite.c/h files which are leveraged to run the uCUnit test
when building the fb_test target. This requires both the System and uCUnit.h
files to run properly(provided from ucUnit.org).

There was some code taken from online sources and those are called out in the
files they are use in(i2c and statemachine).

###### Extra Project Documents
*output examples.pdf*, *ReadTemperature.png/pdf*, *writealertlow.png/pdf*

These documents are in the /.bin folder and are other(non-source code) project4
deliverables.  They include a screenshot of the outputs in MCUExpresso, containing
some examples of the UART printouts to show code is working properly.  The .png
and .pdf files are screen captures of read/writes from/to the temperature sensor
via I2C.


#### Observations for Project:
This project was easily the most time intensive of the projects so far as there
were a lot of different parts to focus on.  I2C, GPIO, state machines, logger updates,
ucunittest, interrupts.  As a newer software/embedded person each aspect took a fair amount
of time to learn to implement. Though not the most difficult the expectations of
the logger and the different builds was a little confusing.   
It was cool to see how we can control the I2C lines and the response received from
a peripheral device.  
