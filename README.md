# LNCVM

List of Loconet CVs on Arduino platform. With minimum using of RAM memory.

Features:
- default CV values and master list saves in programm memory (don't use RAM):
- CV master-record:
    * CV address;
    * default value;
    * text;
- support EEPROM to store CV values;
- support two kind of CV-groups:
    * systems CV;
    * users CV;
- support special system CVs:
   * reset CVs values to default;
   * reset Arduino; 
   
How to use: you can found an example in Examples folder.

Requires: 
- EEPROMex (https://playground.arduino.cc/Code/EEPROMex);
- NMRA Loconet library (https://github.com/mrrwa/LocoNet);

Warning: class does not check uniqly LN CV address. The user should control it by yourself.
