/*210222 : Added 3 axis acceleration
* Sending raw sensor data as char
* Serial displaying data in m/s2


/*Sends Acceleration data from an ESP32 and MPU6050 to a BLE Client
 *These libraries are developed by Neil Kolban for ESP32 to be used in Arduino IDE
 *Acceleration data received in Hexadecimal due to being in Unknown Characteristic
 * 02-14-2020
 * Ravi Prakash
 * 
 */

// Include these to create a server, utilize the BLE function
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h> 
#include <BLECharacteristic.h>
#include<Wire.h> 

const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
float acc_f[3] = {1, 20, 300 }; // 3 axis of acceleration X,Y,Z for serial monitor

int16_t acc[3] = {1, 20, 300 }; // 3 axis of acceleration X,Y,Z
byte senPos[1] = {1}; // 1 for "chest", 2 for "wrist" 
int i=0;

bool _BLEClientConnected = false;

#define accXService BLEUUID((uint16_t)0x1600) // Defining custom service. Will be displayed as "Unknown Service"

BLECharacteristic accXCharacteristic(BLEUUID((uint16_t)0x1A00), BLECharacteristic::PROPERTY_NOTIFY ); //Acceleration X characteristic def
BLECharacteristic locnameCharacteristic(BLEUUID((uint16_t)0x2AB5), BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ); //Location Name characteristic def
BLECharacteristic sensorPositionCharacteristic(BLEUUID((uint16_t)0x2A38), BLECharacteristic::PROPERTY_READ); // Sensor Position characteristic def

BLEDescriptor accXDescriptor(BLEUUID((uint16_t)0x2901)); // Acceleration X descriptor def
BLEDescriptor locnameDescriptor(BLEUUID((uint16_t)0x2901)); // Location Name descriptor def
BLEDescriptor sensorPositionDescriptor(BLEUUID((uint16_t)0x2901));// Sensor descriptor def

//Connected-Disconnected status update function
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      _BLEClientConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      _BLEClientConnected = false;
    }
};

//Initializing BLE function
void InitBLE() {
  BLEDevice::init("rp_1"); // Decide device name, here "rp_1"
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pacc = pServer->createService(accXService); // pacc is the pointer for Server created with accXService

  pacc->addCharacteristic(&accXCharacteristic); // adding characteristic to the Service
  accXDescriptor.setValue("Between 0 to 1000 Hz"); // adding value to the descriptor for display
  accXCharacteristic.addDescriptor(&accXDescriptor); //joining the descriptor "accXDescriptior" with the "accXCharacteristic"
  accXCharacteristic.addDescriptor(new BLE2902()); //Add if using Notify
  //BLE2902 allows us to stop receiving data from the BLE device thus conserving energy. Basically prevents constant "notify"

  pacc->addCharacteristic(&sensorPositionCharacteristic); 
  sensorPositionDescriptor.setValue("Gives sensor position"); // User defined value to be displayed
  sensorPositionCharacteristic.addDescriptor(&sensorPositionDescriptor); 

  pacc->addCharacteristic(&locnameCharacteristic);
  locnameDescriptor.setValue("Location Name"); // User defined value to be displayed
  locnameCharacteristic.addDescriptor(&locnameDescriptor); 

  pServer->getAdvertising()->addServiceUUID(accXService); // Adding the Service for advertising

  pacc->start(); //starting service
  // Start advertising
  pServer->getAdvertising()->start();
}


void setup() {
  Serial.begin(115200); //baud rate
  Serial.println("Starting now");
  InitBLE(); // Initializing BLE 
  
 

  //MPU6050 I2C configuration
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);


}


void loop() {

  //MPU6050 sending data to the microcontroller
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  //Using only x axis data for now

  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)

  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)


  
  acc_f[0] = AcX*5.9814*0.0001;
  acc_f[1] = AcY*5.9814*0.0001;
  acc_f[2] = AcZ*5.9814*0.0001;

  
  //Serial monitor output
  Serial.print(i++);
  Serial.print("-");
  Serial.print(acc_f[0],5);
  Serial.print("-");
  Serial.print(acc_f[1],5);
  Serial.print("-");
  Serial.println(acc_f[2],5);

  acc[0] = AcX;
  acc[1] = AcY;
  acc[2] = AcZ;

  // Serial.println((char*)acc);
  accXCharacteristic.setValue((char*) acc); // sending value to the characteristic. can only send one character
  //3 denotes the no of bytes. Here 3 because of 3 byte array
  accXCharacteristic.notify();

  locnameCharacteristic.setValue("Durham,NC");
  locnameCharacteristic.notify();

 sensorPositionCharacteristic.setValue(senPos, 1); 
 
  delay(10); //sampling rate 100 Hz
}
