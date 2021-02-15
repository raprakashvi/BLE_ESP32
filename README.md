# BLE_ESP32

### ESP32 : 

  #### Fake Heartrate Sensor [File: server_fake_heartrate]
  - Uses heart rate measurement UUID and updates the display number by 1 every second. 
  - Good to learn about characteristics and descriptors and general BLE stuff
  
  #### Fake Accelerometer [File: server_mpu6050_heartrate_fake]
  - Pushes X axis acceleration data using heartrate measurement UUID
  - Combines MPU6050 code with BLE server
  
  #### Accelerometer_MPU6050 [File: MPU6050_temp.ino]
  - Uses custom characteristics to send X axis acceleration data to client 
  - Sends sensor body position and sensor location 
  - Ok to use directly with minor modifications
  
### Arduino : 
 #### MPU6050 [File: mpu6050_arduino]
  - MPU6050 code for accessing all the sensors
  
  #### Sensor Data Plot [File: Real_Time_Plot_From_Arduino_Serial]
  - Plots real time graph from MPU6050 data
  
 
 [Markdown Cheatsheet](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)
 
 //markdown test
 [MPU6050](./raprakashvi/BLE_ESP32/mpu6050_arduino]
