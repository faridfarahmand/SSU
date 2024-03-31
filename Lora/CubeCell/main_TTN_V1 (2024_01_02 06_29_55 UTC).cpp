#include "LoRaRelated.h"

Adafruit_BMP280 bmp;
AHT10 myAHT20(AHT10_ADDRESS_0X38, AHT20_SENSOR);

/* Prepares the payload of the frame and sensors*/
static void prepareTxFrame( uint8_t port )
{
Wire.begin();

	pinMode(Vext, OUTPUT);
 	digitalWrite(Vext, LOW);//set vext to high

while (myAHT20.begin() != true) {
Serial.println(F("AHT20 not connected or fail to load calibration coefficient")); //(F()) save string to flash & keeps dynamic memory free
delay(5000);
}
Serial.println(F("AHT20 OK"));
 
if (!bmp.begin()) {
Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
while (1);
}
 
/* Default settings from datasheet. */
bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, /* Operating Mode. */
Adafruit_BMP280::SAMPLING_X2, /* Temp. oversampling */
Adafruit_BMP280::SAMPLING_X16, /* Pressure oversampling */
Adafruit_BMP280::FILTER_X16, /* Filtering. */
Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */


//voltage = analogRead(ADC);
//Serial.print("Sound: ");
//Serial.println(voltage);

Serial.print("Temperature: ");
Serial.print(myAHT20.readTemperature());
Serial.println(" *C");
 
Serial.print("Humidity: ");
Serial.print(myAHT20.readHumidity());
Serial.println(" RH");
 
Serial.print("Pressure: ");
Serial.print( bmp.readPressure());
Serial.println(" hPa");
Serial.println(" ");
 
 // convert 
 int nTemp = myAHT20.readTemperature();
 int nHum = myAHT20.readHumidity();
 int nPres = bmp.readPressure()/100;
  	
	
    appData[0] = (nTemp >>  0) & 0xFF; 
	appData[1] = (nHum >>  0) & 0xFF; 
	appData[2] = (nPres >>  8) & 0xFF; 
	appData[3] = (nPres >>  0) & 0xFF; 
	
	
	//appData[16] = (uint8_t)(batteryVoltage>>8);
	//appData[17] = (uint8_t)batteryVoltage;
	
Wire.end();
digitalWrite(Vext, HIGH);

}


void setup() 
{
  Serial.begin(115200);

  Wire.begin();

	pinMode(Vext, OUTPUT);
 	digitalWrite(Vext, LOW);//set vext to high
  

  #if(AT_SUPPORT)
	  enableAt();
  #endif
	deviceState = DEVICE_STATE_INIT;
	LoRaWAN.ifskipjoin();
}

void loop()
{
	  Serial.println(millis());
	  delay(5000);
	switch( deviceState )
	{
		case DEVICE_STATE_INIT:
		{
#if(LORAWAN_DEVEUI_AUTO)
			LoRaWAN.generateDeveuiByChipID();
#endif
#if(AT_SUPPORT)
			getDevParam();
#endif
			printDevParam();
			LoRaWAN.init(loraWanClass,loraWanRegion);
			deviceState = DEVICE_STATE_JOIN;
			break;
		}
		case DEVICE_STATE_JOIN:
		{
			LoRaWAN.join();
			break;
		}
		case DEVICE_STATE_SEND:
		{
			prepareTxFrame( appPort );
			LoRaWAN.send();
			deviceState = DEVICE_STATE_CYCLE;
			break;
		}
		case DEVICE_STATE_CYCLE:
		{
			// Schedule next packet transmission
			txDutyCycleTime = appTxDutyCycle + randr( 0, APP_TX_DUTYCYCLE_RND );
			LoRaWAN.cycle(txDutyCycleTime);
			deviceState = DEVICE_STATE_SLEEP;
			break;
		}
		case DEVICE_STATE_SLEEP:
		{
			LoRaWAN.sleep();
			break;
		}
		default:
		{
			deviceState = DEVICE_STATE_INIT;
			break;
		}
	}
}