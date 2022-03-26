
#### This Heat Bed is Segmented heatbed with 9 individually controlled segments.
What makes it special?
#### 1. Energy saving:
Each bed segment can be discretely controlled with its own heater circuit, you no longer have to heat up the entire bed for smaller prints.

#### 2. Heating evenly:
there are 3*3 separate zones each one have a thermistor that not only heating separately but also more even in heating than the normal print bed that have only one thermistor.

 ![bed](https://github.com/markniu/PandaSegmentHeatBed/raw/main/doc/pandabed0.jpg)

#### Hardware:
Bed Size:235*268**2mm

Print Size:220*220mm

Compatible with Creality ender3 serials.

Material: PCB FR4

Power input:24V

Power connector:XT60

Signal connector:I2C

#### Software:
communication protocol: Gcode,Sending heating Gcode M140 or M190,and then with G1 or G0 sending X and Y positon
for example: 
```c
M190 S60  
G1 X100 Y10 
``` 
it will heating the second Zone where the position (100,10) is.

#### Main board request: 
any board which has 2 free GPIO and run Marlin firmware. configure the 2 GPIO as software I2C port to communicate with Panda BED with Gcode.

The folder ``` library```  is the I2C library allows to control PandaBed which is a I2C Segmented heat bed for 3D printer.

The folder ``` Test_Panda_bed```  is a simple arduino platfromio project test example.

Marlin firmware source code:https://github.com/markniu/PandaZHU just enbale ``` #define PANDA_BED 1 ``` in the Configureation.h




### [Support me](https://www.pandapi3d.com/product-page/panda-segmented-heat-bed)
### [Support facebook](https://www.facebook.com/groups/380795976169477)
