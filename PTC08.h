#ifndef PTC08_H_
#define PTC08_H_

#include <stdint.h>
#include <string.h>
#include "uart.h"
#include "timer.h"

#define HIGH_BYTE(B) 	    (B >> 8) & 0xFF
#define LOW_BYTE(B)		    B & 0xFF

#define FIRST_BYTE(B)       (B >> 24) & 0xFF
#define SECOND_BYTE(B)      (B >> 16) & 0xFF
#define THIRD_BYTE(B)       (B >> 8) & 0xFF
#define FOURTH_BYTE(B)       B & 0xFF

#define MIN(B, A)           B < A ? B : A


#define DEFAULT_SERIAL_NUMBER           0x00

#define VC0706_RECEIVE			        0x56
#define VC0706_RETURN			        0x76

#define PTC08_RESPONSE_TIMEOUT          1000000000

#define GEN_VERSION 			0x11 //Get Firmware version information
#define SET_SERIAL_NUMBER 		0x21 //Set serial number
#define SET_PORT 				0x24 //Set port
#define SYSTEM_RESET 			0x26 //System reset
#define READ_DATA 				0x30 //Read data register
#define WRITE_DATA 				0x31 //Write data register
#define READ_FBUF 				0x32 //Read buffer register
#define WRITE_FBUF 				0x33 //Write buffer register
#define GET_FBUF_LEN 			0x34 //Get image lengths in frame buffer
#define SET_FBUF_LEN 			0x35 //Set image lengths in frame buffer
#define FBUF_CTRL 				0x36 //Control frame buffer register
#define COMM_MOTION_CTRL 		0x37 //Motion detect on or off in communication interface
#define COMM_MOTION_STATUS 		0x38 //Get motion monitoring status in communication interface
#define COMM_MOTION_DETECTED 	0x39 //Motion has been detected by communication interface
#define MIRROR_CTRL 			0x3A //Mirror control
#define MIRROR_STATUS 			0x3B //Mirror status
#define COLOR_CTRL 				0x3C //Control color
#define COLOR_STATUS 			0x3D //Color status
#define POWER_SAVE_CTRL 		0x3E //Power mode control
#define POWER_SAVE_STATUS 		0x3F //Power save mode or not
#define AE_CTRL 				0x40 //Control AE
#define AE_STATUS 				0x41 //AE status
#define MOTION_CTRL 			0x42 //Motion control
#define MOTION_STATUS 			0x43 //Get motion status
#define TV_OUT_CTRL 			0x44 //TV output on or off control
#define OSD_ADD_CHAR 			0x45 //Add characters to OSD channels
#define DOWNSIZE_CTRL 			0x54 //Downsize Control
#define DOWNSIZE_STATUS 		0x55 //Downsize status
#define GET_FLASH_SIZE 			0x60 //Get SPI flash size
#define ERASE_FLASH_SECTOR 		0x61 //Erase one block of the flash
#define ERASE_FLASH_ALL 		0x62 //Erase the whole flash
#define READ_LOGO 				0x70 //Read and show logo
#define SET_BITMAP 				0x71 //Bitmap operation
#define BATCH_WRITE 			0x80 //Write mass data at a time

/*Image Size definitions*/
#define IMAGE_640x480                       0x00
#define IMAGE_320x240                       0x11
#define IMAGE_160x120                       0x22

/*Status Codes*/
#define EXECUTION_OK						0x00
#define	COMMAND_NOT_RECEIVED				0x01
#define DATA_LENGHT_ERROR					0x02
#define DATA_FORMAT_ERROR					0x03
#define COMMAND_CANNOT_EXECUTE_NOW			0x04
#define COMMAND_RECEIVED_WRONG_EXECUTION	0x05

/*Device Types*/
#define CHIP_REGISTER                       0x01
#define SENSOR_REGISTER                     0x02
#define CCIR656_REGISTER                    0x03
#define I2C_EEPROM                          0x04
#define SPI_EEPROM                          0x05
#define SPI_FLASH                           0x06

/*Frame Buffer Control Flags*/
#define STOP_CURRENT_FRAME                  0x00
#define STOP_NEXT_FRAME                     0x01
#define RESUME_FRAME                        0x02
#define STEP_FRAME                          0x03

/*Motion Detection Control Flags*/
#define STOP_MOTION_MONITORING              0x00
#define START_MOTION_MONITORING             0x01

/*Power Save Control Command Types*/
#define POWER_SAVE_CONTROL_MODE             0x00
#define POWER_SAVE_ATTR_CONFIG              0x01

/*Power Control Items */
/*For Command Type 0*/
#define GPIO                                0x00
#define UART                                0x01
#define STOP_POWER_SAVE                     0x00
#define START_POWER_SAVE                    0x01
/*For Command Type 1*/
#define STOP_FBUF                           0x00
#define STOP_JPG                            0x01
#define NO_RELATIVITY                       0x00
#define RELATIVITY                          0x02

/*Frame Buffer Types*/
#define CURRENT_FRAME                       0x00
#define NEXT_FRAME                          0x01

/*Control Modes Read Buffer*/
#define MCU_MODE                            0x0A
#define DMA_MODE                            0x0F

int8_t PTC08_FirmwareVersion(uint8_t serialNumber);
int8_t PTC08_SetSerialNumber(uint8_t serialNumber, uint8_t newSerialNumber);
int8_t PTC08_SetPort(uint8_t serialNumber, uint8_t dataLenght, uint8_t interfaceType, uint16_t configData);
uint8_t *PTC08_Reset(uint8_t SerialNumber);
bool PTC08_ReadData(uint8_t SerialNumber, uint8_t DeviceType, uint8_t DataNumToRead, uint32_t ConfigInfo, uint8_t *data);
int8_t PTC08_WriteData(uint8_t serialNumber, uint8_t dataLenght, uint8_t deviceType, uint8_t dataNumToWrite, uint32_t configInfo, uint8_t *data);
bool PTC08_ReadFBuff(uint8_t SerialNumber, uint8_t fBufType, uint8_t ControlMode, uint16_t StartingAddress, uint16_t DataLenght, uint16_t Delay, uint8_t *data);
int8_t PTC08_WriteFBuff(uint8_t serialNumber, uint8_t controlMode, uint32_t startingAddress, uint32_t dataLenghts, uint16_t delay);
uint32_t PTC08_GetFBuffLen(uint8_t SerialNumber, uint8_t fBuffType);
int8_t PTC08_SetFBuffLen(uint8_t serialNumber, uint32_t dataLenghts);
bool PTC08_FBuffCtrl(uint8_t serialNumber, uint8_t controlFlag);
int8_t PTC08_CommMotionCtrl(uint8_t serialNumber, uint8_t controlFlag);
int8_t PTC08_CommMotionStatus(uint8_t serialNumber);
int8_t PTC08_CommMotionDetected(void);
int8_t PTC08_MirrorCtrl(uint8_t serialNumber,uint8_t controlMode, uint8_t mirrorMode);
int8_t PTC08_MirrorStatus(uint8_t serialNumber);
int8_t PTC08_ColorCtrl(uint8_t serialNumber, uint8_t controlMode, uint8_t showMode);
int8_t PTC08_ColorStatus(uint8_t serialNumber);
int8_t PTC08_PowerSaveCtrl(uint8_t serialNumber, uint8_t commandType, uint32_t controlItem);
int8_t PTC08_PowerSaveStatus(uint8_t serialNumber, uint8_t commandType);
int8_t PTC08_AECtrl(uint8_t serialNumber, uint8_t AEAttribute, uint8_t controlMode, uint8_t controlItem);
int8_t PTC08_AEStatus(uint8_t serialNumber, uint8_t AEAttribute);
int8_t PTC08_MotionCtrl(uint8_t serialNumber, uint8_t dataLenghts, uint8_t motionAttribute, uint8_t controlItem);
int8_t PTC08_MotionStatus(uint8_t serialNumber, uint8_t motionAttribute);
int8_t PTC08_TVOutCtrl(uint8_t serialNumber, uint8_t controlItem);
int8_t PTC08_OSDAddChar(uint8_t serialNumber, uint8_t dataLenght, uint8_t characterNumber, uint8_t startingAddress, uint8_t *characters);
int8_t PTC08_DownsizeSize(uint8_t serialNumber, uint8_t controlItem);
int8_t PTC08_DownsizeStatus(uint8_t serialNumber);
int8_t PTC08_GetFlashSize(uint8_t serialNumber);
int8_t PTC08_EraseFlashSector(uint32_t startingAddress);
int8_t PTC08_EraseFlashAll(uint8_t serialNumber);
int8_t PTC08_ReadLogo(uint8_t serialNumber, uint16_t logoLenghts, uint32_t startingAddress);
int8_t PTC08_SetBitmap(uint8_t serialNumber, uint8_t dataLenghts, uint8_t controlType, uint16_t bitmapLenghts, uint32_t startingAddress);



#endif /* PTC08_H_ */
