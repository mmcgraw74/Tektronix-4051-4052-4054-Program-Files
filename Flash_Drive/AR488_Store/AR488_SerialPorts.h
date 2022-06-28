#ifndef AR488_SERIALPORTS_H
#define AR488_SERIALPORTS_H

#include <Arduino.h>
#include <DEVNULL.h>
#include "AR488_Config.h"

/***** AR488_SerialPort.cpp, ver. 0.05.83, 28/06/2022 *****/
/*
 * Serial Port definition
 */


// SoftwareSerial library required?
#if defined(AR_SERIAL_SWPORT) || defined(DB_SERIAL_SWPORT)
  #include <SoftwareSerial.h>
#endif



#ifdef DATAPORT_ENABLE

  extern Stream& dataPort;
  void startDataPort();

  #define DATAPORT_START() startDataPort()
  #define DATA_RAW_PRINT(str) dataPort.print(str)
  #define DATA_RAW_PRINTLN(str) dataPort.println(str)

#else

  extern Stream& dataPort;

  #define DATAPORT_START()
  #define DATA_RAW_PRINT(str)
  #define DATA_RAW_PRINTLN(str)

#endif  // DATAPORT_ENABLE



#ifdef DEBUG_ENABLE

  extern Stream& debugPort;
  void startDebugPort();

  template<typename T1, typename T2>
  void debugPrint(const char * function, const char * filestr, int line, T1 msg1, T2 msg2){
    const char * filename = (strrchr(filestr, '/') ? strrchr(filestr, '/') + 1 : filestr);
    dataPort.print(filename);
    dataPort.print(':');
    dataPort.print(line);
    dataPort.print(" (");
    dataPort.print(function);
    dataPort.print(") > ");
    dataPort.print(msg1);
    dataPort.println(msg2);
  }

  void printHex(uint8_t byteval);
  void printHexBuf(char * buf, size_t bsize);

  #define DEBUG_START() startDebugPort()
  #define DB_PRINT(msg1,msg2) debugPrint(__FUNCTION__, __FILE__, __LINE__,(msg1),(msg2))
  #define DB_RAW_PRINT(msg) debugPort.print(msg)
  #define DB_RAW_PRINTLN(msg) debugPort.println(msg)
  #define DB_HEX_PRINT(byteval) printHex(byteval)
  #define DB_HEXB_PRINT(buf, bsize) printJexBuf(buf, bsize)


#else

  #define DEBUG_START()
  #define DB_PRINT(msg1,msg2)
  #define DB_RAW_PRINT(msg)
  #define DB_RAW_PRINTLN(msg)
  #define DB_HEX_PRINT(byteval)
  #define DB_HEXB_PRINT(buf, bsize)

#endif  // DEBUG_ENABLE



#endif  // AR488_SERIALPORTS_H
