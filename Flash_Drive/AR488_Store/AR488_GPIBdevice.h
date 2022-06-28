#ifndef AR488_GPIBdevice_H
#define AR488_GPIBdevice_H

//#include <SD.h>
#include "AR488_Config.h"
#include "AR488_Layouts.h"
#include "AR488_SerialPorts.h"

#ifdef EN_STORAGE
  #ifdef EN_TEK_4924
    #include "AR488_Store_Tek_4924.h"
  #endif
#endif

/***** AR488_GPIBbus.cpp, ver. 0.05.83, 28/06/2022 *****/


/*********************************************/
/***** GPIB COMMAND & STATUS DEFINITIONS *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv *****/

#define GPIB_CFG_SIZE 83

// Universal Multiline commands (apply to all devices)

#define GC_LLO 0x11
#define GC_DCL 0x14
#define GC_PPU 0x15
#define GC_SPE 0x18
#define GC_SPD 0x19
#define GC_UNL 0x3F
#define GC_TAD 0x40
#define GC_PPE 0x60
#define GC_PPD 0x70
#define GC_UNT 0x5F
// Address commands
#define GC_LAD 0x20
// Addressed commands
#define GC_GTL 0x01
#define GC_SDC 0x04
#define GC_PPC 0x05
#define GC_GET 0x08


/***** GPIB control states *****/
// Controller mode
#define CINI 0x01 // Controller idle state
#define CIDS 0x02 // Controller idle state
#define CCMS 0x03 // Controller command state
#define CTAS 0x04 // Controller talker active state
#define CLAS 0x05 // Controller listner active state
// Listner/device mode
#define DINI 0x06 // Device initialise state
#define DIDS 0x07 // Device idle state
#define DLAS 0x08 // Device listener active (listening/receiving)
#define DTAS 0x09 // Device talker active (sending) state

/***** ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** GPIB COMMAND & STATUS DEFINITIONS *****/
/*********************************************/


/****************************************/
/***** GPIB CLASS OBJECT DEFINITION *****/
/***** vvvvvvvvvvvvvvvvvvvvvvvvvvvv *****/

class GPIBbus {

  public:

    union GPIBconf{
      struct{
        bool eot_en;      // Enable/disable append EOT char to string received from GPIB bus before sending to USB
        bool eoi;         // Assert EOI on last data char written to GPIB - 0-disable, 1-enable
        uint8_t cmode;    // Controller/device mode (0=unset, 1=device, 2=controller)
        uint8_t caddr;    // This interface address
        uint8_t paddr;    // Primary address to use when addressing device
        uint8_t saddr;    // Secondary address to use when addressing device
        uint8_t eos;      // EOS (end of send to GPIB) characters [0=CRLF, 1=CR, 2=LF, 3=None]
        uint8_t stat;     // Status byte to return in response to a serial poll
        uint8_t amode;    // Auto mode setting (0=off; 1=Prologix; 2=onquery; 3=continuous);
        uint16_t rtmo;    // Read timout (read_tmo_ms) in milliseconds - 0-3000 - value depends on instrument
        char eot_ch;      // EOT character to append to USB output when EOI signal detected
        char vstr[48];    // Custom version string
//        uint16_t tmbus;   // Delay to allow the bus control/data lines to settle (1-30,000 microseconds)
        uint8_t eor;      // EOR (end of receive from GPIB instrument) characters [0=CRLF, 1=CR, 2=LF, 3=None, 4=LFCR, 5=ETX, 6=CRLF+ETX, 7=SPACE]
        char sname[16];   // Interface short name
        uint32_t serial;  // Serial number
        uint8_t idn;      // Send ID in response to *idn? 0=disable, 1=send name; 2=send name+serial
      };
    uint8_t db[GPIB_CFG_SIZE];
    };

    union GPIBconf cfg;
    
    bool txBreak;  // Signal to break the GPIB transmission
    uint8_t cstate;


    GPIBbus();

    void begin();
    void stop();

    bool isAsserted(uint8_t gpibsig);
    void setControls(uint8_t state);
    void sendStatus();
    void sendEOI();
    void setStatus(uint8_t statusByte);
    bool receiveData(Stream& dataStream, bool detectEoi, bool detectEndByte, uint8_t endByte);
    bool receiveToFile(File& outputFile, bool detectEoi, bool detectEndByte, uint8_t endByte);
    void sendData(char *databuffer, size_t dsize, bool lastChunk);
    bool sendRawData(char *databuffer, size_t dsize);
    void clearDataBus();
    uint8_t readByte(uint8_t *db, bool readWithEoi, bool *eoi);  
    uint8_t writeByte(uint8_t db, bool isLastByte);
    void setControlVal(uint8_t value, uint8_t mask, uint8_t mode);
    void setDataVal(uint8_t);

//    void setDeviceAddressedState(uint8_t stat);
    bool isDeviceAddressedToListen();
    bool isDeviceAddressedToTalk();
    bool isDeviceInIdleState();

#ifdef EN_STORAGE
//    bool receiveData(ofstream& outputFile, bool detectEoi, bool detectEndByte, uint8_t endByte);
    uint8_t receiveParams(bool detectEoi, char * receiveBuffer, uint8_t bufSize);
//    void sendData(ifstream& fileStream);
#endif

    void signalBreak();

  private:

//    bool dataContinuity;
    uint8_t deviceAddressedState;

    boolean waitOnPinState(uint8_t state, uint8_t pin, const uint16_t interval);  
    bool isTerminatorDetected(uint8_t bytes[3], uint8_t eorSequence);
    void setSrqSig();
    void clrSrqSig();
    bool atnStatus = false;

};




/***** ^^^^^^^^^^^^^^^^^^^^^^^^^^^^ *****/
/***** GPIB CLASS OBJECT DEFINITION *****/
/****************************************/


#endif // AR488_GPIBdevice_H
