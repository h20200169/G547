# Driver for I2C protocol based ADXL345.
# Prerequisites for the project.
1. Basic understanding of the I2C protocol.
2. A fair understanding if the I2C subsystem in the kernel.
3. Working of the ADXL345 accelerometer sensor.

# Summary
The following project implements a driver communicating with the ADXL345 over the I2C protocol, reads its data register, and returns its raw values to the user through a number of IOCTL calls implemented. Driver also facilitates the configuration of the range of the sensor reading using RANGE_SENSOR ioctl call. The host used is the Raspberry Pi 4b 2gb model.

# I2C subsystem
The programming interface of the I2C subsystem revolves around two drivers
1. I2C adapter driver
    Adapater basically represents the I2C bus. Each bus works using a algorithm. The algorithm depending upon the pins used from the board can be board's internal algorithm or bit banging algorithm if the pins are GPIO pins with alternate functionality. An Algorithm performs the actual reading and writing of I2C messages to the hardware –
this may involve bit banging GPIO lines or writing to an I2C controller chip.
2. I2C client driver
    This the driver that actually implements the read write amog other functionalities for the client (slave or sensor). In this project the device has been instatiated explicilty in the sense that the kernel is not aware of the client device.
   
# Detecting the I2C device address
  Execute i2cdetect -y -adapter number   ->   For e.g. i2cdetect -y -1
 
# Reading all the registers
  Execute i2cdump -y -1 0xaddress
 
# Reading individual registers
  Execute i2cget -y -1 0xaddress 0xregister_address
 
# Writing To registers
  Execute i2cset -y -1 0xaddress 0xregister_address 0xdata
 
# Setup and the schematic.

1. Schematic:
![pi-and-adxl345_bb](https://user-images.githubusercontent.com/91187838/145357740-37398451-7328-43cf-84fe-7c67c9a6c738.png)

2. Setup:
![setup](https://user-images.githubusercontent.com/91187838/145377385-9f25f36f-8d52-49bd-8dc5-9a8e98d3cebf.jpg)


# ADXL345
![adxl](https://user-images.githubusercontent.com/91187838/145358122-29aeb078-308e-4756-be4e-f8dad8ddb12b.jpg)

The ADXL345 is a MEMS producing output proportional to the acceleration that it is subjected to. It is able to measure so along 3 axes (x, y, z). It has 6 8-bit data registers, 2 for each axis, in effect 16 bits allocated to each axis. But not all the bits are used for the data interpretation. Depending upon the configuration of the DATA_FORMAT_REGISTER the number of bits actually associated with the data change. The Enable register also needs to be configured to get the sensor up and running. For further reference, refer the ADXL345 datasheet [here](https://www.alldatasheet.com/view.jsp?Searchword=Adxl345%20datasheet&gclid=CjwKCAiA78aNBhAlEiwA7B76p7tAyBbKxHDv3296kI4oela5oAmEqQoyKUQxXYYmcrXFbaS1P5yjVRoCPIIQAvD_BwE).


# Connections
| ADXL Pin | RaspberryPi Pin |
  | --- | --- |
  | VCC | Pin 1 (3.3 V)|
  | Ground | Pin 39 |
  | SCL | Pin 3 |
  | SDA | Pin 5|
 
# Driver functionality
The driver implemented has ioctl call for
1. Turning on the device
2. Reading individual axis data.
3. Setting the range to be +/-2g, +/-4g, +/-8g and +/- 16g.

# IOCTL calls implimented:
1.IOCTL_PWR_CTL:
ADXL345 sensor has POWER_CTL register at address location 0x2D(45D).
Register 0x2D—POWER_CTL (Read/Write)
![image](https://user-images.githubusercontent.com/91187838/145367019-5cd9ed45-9493-424c-a630-c74afd8c6630.png)

 

So this ioctl call can be used to control power given to the sensor

2.ACC_X_READ:
ADXL345 provides data of 3 cordinated X,Y and Z.
So this ioctl call is used to get data from X axis.
DATAX0 is the register at location 0x32 (50D)which has lower byte of data and DATAX1 (location 0x33 has higher byte of data from X axis)
This sensor has 10 bit resolution . So out of 16 bits ,10 bits are configured to get data.
Also data provided by sensor is signed 10 bit in 2's compliment form. So in this ioctl call , if data is negative then 2's compliment of binary data will be taken and result will be displayed with negative sign.
 
 3.ACC_Y_READ:
ADXL345 provides data of 3 cordinated X,Y and Z.
So this ioctl call is used to get data from Y axis.
DATAY0 is the register at location 0x34 (52D)which has lower byte of data and DATAY1 (location 0x35 has higher byte of data from Y axis)
This sensor has 10 bit resolution . So out of 16 bits ,10 bits are configured to get data.
Also data provided by sensor is signed 10 bit in 2's compliment form. So in this ioctl call , if data is negative then 2's compliment of binary data will be taken and result will be displayed with negative sign.

 3.ACC_Z_READ:
ADXL345 provides data of 3 cordinated X,Y and Z.
So this ioctl call is used to get data from Z axis.
DATAZ0 is the register at location 0x36 (54D)which has lower byte of data and DATAZ1 (location 0x37 has higher byte of data from Z axis)
This sensor has 10 bit resolution . So out of 16 bits ,10 bits are configured to get data.
Also data provided by sensor is signed 10 bit in 2's compliment form. So in this ioctl call , if data is negative then 2's compliment of binary data will be taken and result will be displayed with negative sign.

4.RANGE_SETTING:
DATA_FORMAT is read writable register at location 0x31.

![image](https://user-images.githubusercontent.com/91187838/145367162-aa5a3680-5fd7-49f3-97b9-48eb09773917.png)



 | D1 | D0 | g Range |
 |---|---|---|
| 0 | 0 | ±2g |
| 0 | 1 | ±4g |
| 1 | 0 | ±8g |
| 1 | 1 | ±16 g |

So ioctl call is provided to select range(±2 g,±4 g,±8 g,±16 g).
As per requirement range of results can be selected.

# Interpreting the data
  The data given out by the sensor is in 2's compliment form. If the range is +/- 2g and resolution is 10 bit, then LSB will represent 4g/1024 (mulitpying factor) as an absolute value. Bringing the 2's compliment data back into decimal form and multiplying it by the factor will give us the value in Gs. The multiplying factor will change depending upon the range and the resolution.
 
# Steps
1. Navigate to the directory conatining the driver and associated Makefile.
2. Execute the make command.
3. Insert the module -> sudo insmod main_1.ko
4. Grant the permission for the dev file -> sudo chmod 777 /dev/adxl_file
5. Execute the user file -> ./iocltusr
