#include <Arduino.h>

#include "AR488_Store_Tek_4924.h"



/***** AR488_Store_Tek_4924.cpp, ver. 0.05.82, 27/06/2022 *****/
/*
 * Tektronix 4924 Tape Storage functions implementation
 */

#ifdef EN_STORAGE



extern GPIBbus gpibBus;

// extern ArduinoOutStream cout;

extern void printHex(char *buffr, int dsize);

//CharStream charStream(line_buffer_size);
//ArduinoOutStream gpibout(charStream);


/***** Tektronix file types *****/
alphaIndex tekFileTypes [] = {
  { 'A', "ASCII" },
  { 'B', "BINARY" },
  { 'N', "NEW" },
  { 'L', "LAST" } 
};


/***** Tektronix file usages *****/
alphaIndex tekFileUsages [] = {
  { 'P', "PROG" },
  { 'D', "DATA" },
  { 'L', "LOG" },
  { 'T', "TEXT" },
};


/***** Index of accepted storage commands *****/
SDstorage::storeCmdRec SDstorage::storeCmdHidx [] = { 
  { 0x60, &SDstorage::stgc_0x60_h },  // STATUS
  { 0x61, &SDstorage::stgc_0x61_h },  // SAVE
  { 0x62, &SDstorage::stgc_0x62_h },  // CLOSE
  { 0x63, &SDstorage::stgc_0x63_h },  // OPEN
  { 0x64, &SDstorage::stgc_0x64_h },  // OLD/APPEND
  { 0x66, &SDstorage::stgc_0x66_h },  // TYPE
  { 0x67, &SDstorage::stgc_0x67_h },  // KILL
  { 0x69, &SDstorage::stgc_0x69_h },  // HEADER/DIRECTORY/CD
  { 0x6A, &SDstorage::stgc_0x6A_h },  // COPY
  { 0x6C, &SDstorage::stgc_0x6C_h },  // PRINT
  { 0x6D, &SDstorage::stgc_0x6D_h },  // INPUT
  { 0x6E, &SDstorage::stgc_0x6E_h },  // READ
  { 0x6F, &SDstorage::stgc_0x6F_h },  // WRITE
  { 0x71, &SDstorage::stgc_0x71_h },  // BSAVE/BOLD
  { 0x73, &SDstorage::stgc_0x73_h },  // LIST/TLIST
  { 0x7B, &SDstorage::stgc_0x7B_h },  // FIND
  { 0x7C, &SDstorage::stgc_0x7C_h },  // MARK
//  { 0x7D, &SDstorage::stgc_0x7D_h },  // SECRET
  { 0x7E, &SDstorage::stgc_0x7E_h }   // ERROR
};


/**********************************************/
/***** Tek file header handling functions *****/
/*****vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*****/


/***** Constructor *****/
TekFileInfo::TekFileInfo(){
  clear();
}


/***** Clear the file information record *****/
void TekFileInfo::clear(){
  fnum = 0;
  ftype = '\0';
  fusage = '\0';
  memset(fdesc, '\0', (file_header_size-30)+1);
//  fsecret = false;
//  frecords = 0;
  fsize = 0;
}


/***** Return file number value *****/
uint8_t TekFileInfo::getFnumVal(){
  return fnum;
}


/***** Return file number string*****/
void TekFileInfo::getFnumStr(char * numstr){
  sprintf(numstr, "%-3d", fnum);  // Left justified 3-digit number
}


/***** Return file type character *****/
char TekFileInfo::getFtype(){
  return ftype;
}


/***** Return file usage character *****/
char TekFileInfo::getFusage(){
  return fusage;
}


/***** Return file type string *****/
void TekFileInfo::getFtypeStr(char * typestr){
  uint8_t i = 0;
  while (tekFileTypes[i].idx) {
    if (tekFileTypes[i].idx == ftype) {
      strncpy(typestr, tekFileTypes[i].desc, strlen(tekFileTypes[i].desc));
      break;
    }
    i++;
  }
}


/***** Return file usage string *****/
void TekFileInfo::getFusageStr(char * usagestr){
  uint8_t i = 0;
  if (fusage) {
    // Fusage set to find the keyword
    while (tekFileUsages[i].idx) {
      if (tekFileUsages[i].idx == fusage) {
        strncpy(usagestr, tekFileUsages[i].desc, strlen(tekFileUsages[i].desc));
        break;
      }
      i++;
    }
  }else{
    // Fusage unset so return spaces
    memset(usagestr,' ',8);
  }
}


/***** Return the file description *****/
void TekFileInfo::getFdescStr(char * description){
  uint8_t dl = (file_header_size-30);
  uint8_t scnt = 0;
  uint8_t i = 0;

  // Is description contain only spaces (dl = scnt) ?  
  for (i=0; i<dl; i++) {
    if (fdesc[i]==0x20) {
      scnt++;
    }else if (fdesc[i]==0) {
      break;
    }
  }

  if ( (fdesc[0]==0) || (scnt==i) ) {
    // NULL or full of spaces
    memset(description, '-', dl); // Blank string returns '----------------'
  }else{
    // Valid data
    strncpy(description, fdesc, dl);
  }
}


/***** Return string containing assigned number of records *****/
/*
void TekFileInfo::getFrecords(char * recordstr){
  sprintf(recordstr, "%8d", frecords);
}
*/

/***** Return string contaning the file size *****/
void TekFileInfo::getFsize(char * sizestr){
//  sprintf(sizestr, "%6d", fsize); // (Right justification)
  sprintf(sizestr, "%ld", fsize);  // (Left [no] justification)
}


/***** Return a file name from the stored file information *****/
void TekFileInfo::getFilename(char * filename){
  char * filenameptr = filename;
  memset(filename, '\0', file_header_size);
  // File number
  getFnumStr(filenameptr);
  filenameptr = filenameptr + 7;
  // File type
  getFtypeStr(filenameptr);
  filenameptr = filenameptr + 8;
  // File usage
  getFusageStr(filenameptr);
  filenameptr = filenameptr + 5;
  // File description
  getFdescStr(filenameptr);
  filenameptr = filenameptr + ((file_header_size-30)+1);  // +1 for the space
  *filenameptr = ' ';
  filenameptr++;
  // File size
  getFsize(filenameptr);
  // Up to char pos (file_header_size - 7) replace NULL with space
  for (uint8_t i=0; i<(file_header_size-7); i++) {
    if (filename[i] == 0x00) filename[i] = 0x20;
  }
}


/***** Return a Tek header from the stored file information *****/
void TekFileInfo::getTekHeader(char * header){
  char * headerptr = header;
  size_t records;
  memset(header, '\0', 44);
  header[0] = 0x20;
  headerptr++;
  getFnumStr(headerptr);
  header[6] = 0x20;
  headerptr = headerptr + 6;
  getFtypeStr(headerptr);
  headerptr = headerptr + 8;
  getFusageStr(headerptr);
  headerptr = headerptr + 10;
  headerptr = headerptr + 8;
  records = fsizeToRecords(fsize);
  sprintf(headerptr, "%8d", records);
  header[43] = 0x0D;
  header[44] = 0x13;
}


/***** Set file info from filename string *****/
void TekFileInfo::setFromFilename(char * filename){
//  char * fptr = filename;
  char sfname[file_header_size];
  uint8_t plen;
  uint8_t depos;
  char ch;
  uint8_t dcnt = 0;
  char fsizestr[8];
  char * param;

  memset(sfname, '\0', file_header_size);
  memset(fsizestr, '\0', 8);
  strncpy(sfname, filename, strlen(filename));
  param = strtok(sfname, " ");
  fnum = (uint8_t)atoi(param);   // fnum takes values 0 - 255
  
  param = strtok(NULL, " ");
  setFtype(param[0]);
  
  param = strtok(NULL, " ");
  setFusage(param[0]);
  
  param = strtok(NULL, "\0");
  plen = strlen(param);
  depos = plen;

  // Determine the position of the final space
  while ( (ch != ' ') && (depos > 0) ) {
    depos--;
    ch = param[depos];
  }

  // Extract description
  memset(fdesc, '\0', (max_fdesc_length + 1) );
  for (uint8_t i=0; i<depos; i++) {
    ch = param[i];
    if ( (ch != '[') && (ch != ']') ) {
      fdesc[dcnt] = ch;
      dcnt++;
    }
    if (dcnt == max_fdesc_length) break; // Reached max desc length - prevent overrun
  }

  // Extract file size string and convert to number
  dcnt = 0;
  if ( (plen-depos)>7 ){  // Prevent overrun
    fsize = 0;
  }else{
    for (uint8_t i=depos; i<plen; i++){
      fsizestr[dcnt] = param[i];
      dcnt++;
    }
    fsize = atol(fsizestr);
  }

}


/***** Set the file number *****/
bool TekFileInfo::setFnumber(uint8_t filenum ){
  if (filenum < files_per_directory) {
    fnum = filenum;
    return true;
  }
  return false;
}


/***** Set the file type *****/
void TekFileInfo::setFtype(char typechar){
  uint8_t i = 0;
  ftype = '\0';
  // typechar must match known types
  while (tekFileTypes[i].idx) {
    if (tekFileTypes[i].idx == typechar) {
      ftype = typechar;
      break;
    }
    i++;
  }
  // For a NEW file clear the usage field
  if (ftype == 'N') fusage = '\0';
}


/***** Set the file usage *****/
void TekFileInfo::setFusage(char usagechar){
  uint8_t i = 0;
  fusage = '\0';
  // usagechar must match known usages
  while (tekFileUsages[i].idx) {
    if (tekFileUsages[i].idx == usagechar) {
      fusage = usagechar;
      break;
    }
    i++;
  }
}


/***** Set the file description *****/
void TekFileInfo::setFdesc(const char * description) {
  memset( fdesc, '\0', ((file_header_size-30)+1) );
  strncpy(fdesc, description, (file_header_size-30));
}


/***** Set the file size *****/
void TekFileInfo::setFsize(size_t filesize){
  fsize = filesize;
//  setFrecords(fsizeToRecords(fsize));
}


/***** Convert filesize to record count *****/
size_t TekFileInfo::fsizeToRecords(size_t fsize){
  size_t frecords = (fsize/256) + 1;
//  frecords = (fsize/256) + 1;
  return frecords;
}


/*****^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*****/
/***** Tek file header handling functions *****/
/**********************************************/




/*****************************/
/***** Utility functions *****/
/*****vvvvvvvvvvvvvvvvvvv*****/


/***** Convert 4 character hex string to 16-bit unsigned integer *****/
uint16_t SDstorage::hexToDataHeader(char * hexstr) {
  char asciistr[5] = {0};
  // Limit to 4 characters
  for (uint8_t i=0; i<4; i++) {
    asciistr[i] = hexstr[i];
  }
  // Return 16 bit value
  return (uint16_t)strtoul(asciistr, NULL, 16);
}


/***** Get the current directory *****/
void SDstorage::getDirectory(char * path) {
  strncpy(path, directory, 13);
}



/***** Set the current directory *****/
void SDstorage::setDirectory(char * path) {
  uint8_t j = 1;
  // Set directory path
  directory[0] = '/';
  for (uint8_t i=0; i<strlen(path); i++) {
    // Ignore slashes and line terminators (CR or LF)
    if ((path[i] != 0x2F) && (path[i] != 0x5C) && (path[i] != 0x0A) && (path[i] != 0x0D)) {
      // Add character to directory name
      if (j < 11) directory[j] = path[i]; // Copy max 10 characters
      j++;
    }
  }
  // Add terminating slash and NULL
  directory[j] = '/';
  directory[j+1] = '\0';

  // Reset list index to file 0
  listIdx = 0;

  // Close any previous file that was open
  closeFile();
}


/***** Display content of current file on terminal *****/
void SDstorage::viewCurrentFile(Stream &output){
  char linebuf[line_buffer_size];
  char path[full_path_size] = {0};
  strcpy(path, directory);
  strcat(path, f_name);
  ifstream sdin(path);
  while (sdin.getline(linebuf, line_buffer_size, '\r')) {
    output.println(linebuf);
  }
  sdin.close();
}


/***** Set the current file parameters *****/
bool SDstorage::findFile(uint8_t fnum){
  File findfile;
  char path[full_path_size] = {0};
  char fname[file_header_size];

  if (searchForFile(fnum, findfile)) { // Returns filehandle if found

    // Get filename from file handle and store in f_name
    findfile.getName(fname, file_header_size);
    strncpy(f_name, fname, file_header_size);
    findfile.close();

    // all BINARY files are in HEX format
    if ((f_name[7] == 'B') && (f_name[15] == 'P')) {
      f_type = 'B';    // BINARY PROGRAM file
    } else if ((f_name[7] == 'B') && (f_name[15] == 'D')) {
      f_type = 'H';    // BINARY DATA file ** to read - parse the data_type
    } else if (f_name[25] == 'S') {  // f_name[25] is location of file type SECRET ASCII PROGRAM
      f_type = 'S';    // SECRET ASCII PROGRAM file
    } else if (f_name[15] == 'P') {  // f_name[15] is location of file type (PROG, DATA,...)
      f_type = 'P';    // ASCII PROGRAM file
    } else if (f_name[7] == 'N') {  // f_name[7] is location of file type (ASCII,BINARY,NEW, or LAST)
      f_type = 'N';    // ASCII LAST file
    } else if (f_name[7] == 'L') {  // f_name[7] is location of file type (ASCII,BINARY,LAST)
      f_type = 'L';    // ASCII LAST file
    } else {
      f_type = 'D';    // ASCII DATA file, also allows other types like TEXT and LOG to be treated as DATA
    }

    if (f_type) {
      strncpy(path, directory, strlen(directory));
      strncat(path, f_name, strlen(f_name));
      sdinout.open(path, O_RDWR); // Open file for reading and writing
      listIdx = atoi(fname);

      return true;

#ifdef DEBUG_STORE_VERBOSE
    }else{
      // Type undetemined
      DB_PRINT(F("Unknown type!"),"");
      DB_PRINT(F("done."),"");
#endif
    }
  }
  return false;
}


/***** Strip any trailing CR or LF characters *****/
void SDstorage::stripLineEnd(char * buf, uint8_t buflen){
  // Run from end of the array backwards
  for (uint8_t i=(buflen-1); i>0; i--) {
    // Replace CR and LF with NULL
    if ( (buf[i]==0x0A) || (buf[i]==0x0D) ) buf[i] = 0x00;
    // Exit on first non-NULL (and by implication non-CR and non-LF) character 
    if (buf[i] != 0x00) i=1;
  }
}


/***** List files in directory to serial *****/
void SDstorage::listFiles(Stream &output){

  char fname[file_header_size+1] = {0};
//  bool found = false;
  File fileObj;

  for (uint8_t fnum = 1; fnum < files_per_directory; fnum++) {
    if (searchForFile(fnum, fileObj)) {
      // Retrieve file name
      fileObj.getName(fname, file_header_size);
      fileObj.close();  // Done with file handle
      output.println(fname);
    }
    if (fname[7] == 'L') return;
  }
}


/***** Read binary data/program from file *****/
uint8_t SDstorage::binaryRead() {
  int16_t c;
  uint8_t err = 0;
  uint32_t filesize;
  uint32_t padding = 0;

  // Actual file size on disk
  filesize = sdinout.fileSize();
  // Calculate padding required to fill up to the next 256-byte block
  padding = (256 - (filesize % 256));
  
  
  while (sdinout.available()) {

    // Read a byte
    c = sdinout.read();

#ifdef DEBUG_STORE_BINARYREAD
    if (!err) {
      DB_HEX_PRINT(c);
    }
#endif

/*
    if (sdinout.peek() < 0) {  // Look ahead for EOF
      // Reached EOF - send last byte and 0xFF with EOI
//      err = gpibBus.writeByte(c, SEND_DATA_ONLY);
//      err = gpibBus.writeByte(0xFF, SEND_WITH_EOI);

      err = gpibBus.writeByte(c, SEND_WITH_EOI);

      return(0);

    }else{
      // Send byte to the GPIB bus
      err = gpibBus.writeByte(c, SEND_DATA_ONLY);
    }
*/

      // Send byte to the GPIB bus
      err = gpibBus.writeByte(c, SEND_DATA_ONLY);


    // Exit on ATN or receiver request to stop (NDAC HIGH)
    if (err) {
#ifdef DEBUG_STORE_BINARYREAD
      if (err == 1) DB_PRINT(F("\r\nIFC detected!"),"");
      if (err == 2) DB_PRINT(F("\r\nATN detected!"),"");
      if (err == 10) DB_PRINT(F("\r\nTimeout!"),"");
#endif
      // Rewind file read by a character (current character has already been read)
      sdinout.seekCur(-1);
      break;
    }
  }

  if (padding > 0) {
    err = gpibBus.writeByte(0xFF, SEND_DATA_ONLY);    // Signal end of data
    padding--;
    for (uint32_t p=0; p<padding; p++) {
      err = gpibBus.writeByte(0x20, SEND_DATA_ONLY);  // Pad with 0x20 to 256-byte boundary
      if (err) break;      
    }
  }

#ifdef DEBUG_STORE_BINARYREAD
  DB_PRINT(F("Err: "), err);
#endif

  return err;
}


/***** COPY binary data from file *****/
uint8_t SDstorage::binaryCopy() {
  int16_t c;
  uint8_t err = 0;
  int copycnt = 0;
  
  while (sdinout.available()) {

    // Read a byte
    c = sdinout.read();

//    cnt++;


#ifdef DEBUG_STORE_BINARYCOPY
    if (!err) {
      DB_HEX_PRINT(c);
    }
#endif

    if ( c==(copyterm&0xFF)) {
      // Terminator reached
      err = gpibBus.writeByte(c, SEND_WITH_EOI);
//      err = gpibBus.writeByte(0xFF, SEND_WITH_EOI);
      return 0;
    } else if (c==0x0D) {
      // CR reached
      err = gpibBus.writeByte(c, SEND_WITH_EOI);
      return 0;
    }else if (c<0) {
      // EOF reached
      err = gpibBus.writeByte(0xFF, SEND_WITH_EOI);
      return 0;
    }else if (copycnt == (copylen-1)) {
      // Reached copylen bytes
      err = gpibBus.writeByte(c, SEND_WITH_EOI);
      return 0;      
    }else if (sdinout.peek() < 0) {  // Look ahead for EOF
      // Reached EOF - send last byte and 0xFF with EOI
      err = gpibBus.writeByte(c, SEND_WITH_EOI);
//      err = gpibBus.writeByte(0xFF, SEND_WITH_EOI);
      return 0;

/*
#ifdef DEBUG_STORE_BINARYCOPY
      DB_PRINT(F("<EOF"),);
#endif
*/

    }else{
      // Send byte to the GPIB bus
      err = gpibBus.writeByte(c, SEND_DATA_ONLY);
      copycnt++;
    }

    // Exit on ATN or receiver request to stop (NDAC HIGH)
    if (err) {
#ifdef DEBUG_STORE_BINARYCOPY
      if (err == 1) DB_PRINT(F("\r\nreceiver requested stop!"),"");
      if (err == 2) DB_PRINT(F("\r\nATN detected."),"");
#endif
      // Set lines to listen ?
      // Rewind file read by a character (current character has already been read)
      sdinout.seekCur(-1);
      break;
    }
  }

  return err;
}


/***** Write binary data/program to file *****/
bool SDstorage::binaryWrite(){
  return gpibBus.receiveToFile(sdinout, true, false, 0);  
}


/*****^^^^^^^^^^^^^^^^^^^*****/
/***** Utility functions *****/
/*****************************/




/**************************************/
/***** SD Card handling functions *****/
/*****vvvvvvvvvvvvvvvvvvvvvvvvvvvv*****/

/***** Contructor - initialise the SD Card object *****/
SDstorage::SDstorage(){
  // Set up CS pin
  pinMode(SDCARD_CS_PIN, OUTPUT);
  digitalWrite(SDCARD_CS_PIN, HIGH);
//  if (!sd.cardBegin(SD_CONFIG)) {
  // Initialise SD card object
  if (!sd.begin(SD_CONFIG)) {
    errorCode = 7;
#ifdef DEBUG_STORE
    DB_PRINT(F("SD card initialisation failed!"),"");
#endif
  }
}


/***** Search for file by number function *****/
/*
 * filenum 1-files_per_directory: search for the file number
 * filenum = 0: search for the last file
 * returns true if the file is found
 */
bool SDstorage::searchForFile(uint8_t filenum, File& fileObj){

  File dirObj;
  char fname[file_header_size];

#ifdef DEBUG_STORE_FILESEARCH
  DB_PRINT(F("Searching: "),directory);
#endif

  // filenum is always 0 or more (uint8_t), but must be no larger than files_per_directory
  if (filenum <= files_per_directory){

    if (!dirObj.open(directory, O_RDONLY)) {
      errorCode = 3;
#ifdef DEBUG_STORE_FILESEARCH
      DB_PRINT(F("Failed to open directory!"),"");
#endif
      return false;
    }

    // Scan through each file in the directory
    while(fileObj.openNext(&dirObj, O_RDWR)) {

      // Skip directories, hidden files, and null files
      if (!fileObj.isSubDir() && !fileObj.isHidden()) {

        // Retrieve file name
        fileObj.getName(fname, file_header_size);
        // Extract file number
        int num = atoi(fname);
        // Check file number against search parameter
        if (filenum == num) {
#ifdef DEBUG_STORE_FILESEARCH
          DB_PRINT(F("found file "), filenum);
          DB_PRINT(F("done."),"");
#endif
//          fileObj.close();
          dirObj.close();     
          return true;
        }
      }    
      fileObj.close(); 
    }

    dirObj.close();
    errorCode = 2;

#ifdef DEBUG_STORE_FILESEARCH
    DB_PRINT(F("file not found!"),"");
#endif
  }


#ifdef DEBUG_STORE_FILESEARCH
  DB_PRINT(F("done."),"");
#endif

  return false;
}


/***** Get the number of the last file *****/
uint8_t SDstorage::getLastFile(File& fileObj) {
  File dirObj;
  char fname[file_header_size];

#ifdef DEBUG_STORE_GETLAST
  DB_PRINT(F("Searching: "), directory);
#endif

  if (!dirObj.open(directory, O_RDONLY)) {
    errorCode = 3;
#ifdef DEBUG_STORE_GETLAST
    DB_PRINT(F("failed to open directory!"),"");
#endif
    return 0;
  }

  // Scan through the files in the directory
  while(fileObj.openNext(&dirObj, O_RDONLY)) {

    // Skip directories, hidden files, and null files
    if (!fileObj.isSubDir() && !fileObj.isHidden()) {
      // Retrieve file name
      fileObj.getName(fname, file_header_size);
      // Extract file type
      if (fname[7] == 'L') {
        // Extract file number
        int num = atoi(fname);
        fileObj.close();
        dirObj.close();
#ifdef DEBUG_STORE_GETLAST
        DB_PRINT(F("LAST is file number: "), num);
        DB_PRINT(F("done."),"");
#endif
        return num;
      }
    }
    fileObj.close();
  }
  dirObj.close();
  errorCode = 2;

#ifdef DEBUG_STORE_GETLAST
  DB_PRINT(F("file not found!"),"");
  DB_PRINT(F("done."),"");
#endif

  return 0;
}


/***** Rename a file *****/
bool SDstorage::renameFile(File& fileObj, char ftype, char fusage) {

  TekFileInfo fileinfo;
  char fname[file_header_size];
  File dirObj;
  
  if (dirObj.open(directory, O_RDONLY)) {
    if (f_type == 'N') { // Rename new files only
      // Make sure file is marked as 'ftype' and 'fusage' and with appropriate length
      fileObj.getName(fname, file_header_size);
      fileinfo.setFromFilename(fname);
      // Set file parameters
      fileinfo.setFtype(ftype);
      fileinfo.setFusage(fusage);
      fileinfo.setFsize(fileObj.fileSize());
      // Read back new filename
      fileinfo.getFilename(fname);
      // Rename the file
      if (sdinout.rename(&dirObj, fname)) {
#ifdef DEBUG_STORE_RENAME
        DB_PRINT(F("filename updated."),"");
#endif
        return true;
      }else{
#ifdef DEBUG_STORE_RENAME
        DB_PRINT(F("failed to update filename!"),"");           
#endif
        return false;
      }
#ifdef DEBUG_STORE_RENAME
    }else{
      errorCode = 2;
      DB_PRINT(F("incorrect file type!"),"");
#endif
    }
  }else{
    errorCode = 3;
#ifdef DEBUG_STORE_RENAME
    DB_PRINT(F("failed to open directory!"),"");
#endif      
  }
  return false;
}


/***** Make a file NEW *****/
bool SDstorage::makeNewFile(File& fileObj, uint16_t filelength) {

  TekFileInfo fileinfo;
  char fname[file_header_size];
  char ftype;
  char path[full_path_size] = {0};

  fileObj.getName(fname, file_header_size);
  fileinfo.setFromFilename(fname);
  ftype = fileinfo.getFtype();

  if (ftype != 'L') {   // Cannot kill the LAST file!

    // Get the existing full path
    strncpy(path, directory, strlen(directory));
    strncat(path, fname, strlen(fname));
    fileObj.close();
          
    // Delete the file content
    if (sd.remove(path)) {
      // Set file parameters
      fileinfo.setFtype('N');
      fileinfo.setFdesc("");
      fileinfo.setFsize(filelength);
      // Read back new filename
      fileinfo.getFilename(fname);

      // Compose the new full path
      memset(path, '\0', full_path_size);
      strncpy(path, directory, strlen(directory));
      strncat(path, fname, strlen(fname));

      // Create a new NEW file with same number
      if (fileObj.open(path,(O_RDWR | O_CREAT | O_AT_END))) { // Create new file if it doesn't already exist
#ifdef DEBUG_STORE_MAKENEW
        DB_PRINT(F("file set to NEW."),"");
#endif
        fileObj.close(); // Finished with file
        return true;
      }else{
#ifdef DEBUG_STORE_MAKENEW
        DB_PRINT(F("failed make NEW!"),"");           
#endif
      }
#ifdef DEBUG_STORE_MAKENEW
    }else{
      DB_PRINT(F("failed to clear existing file!"),"");
#endif
    }
  }
  return false;
}


void SDstorage::closeFile(){
  // Close file handle
  sdinout.close();
  // Clear f_name and f_type
  memset(f_name, '\0', file_header_size);
  f_type = '\0';
}


/*****^^^^^^^^^^^^^^^^^^^^^^^^^^^^*****/
/***** SD Card handling functions *****/
/**************************************/




/**************************************/
/***** Command handling functions *****/
/*****vvvvvvvvvvvvvvvvvvvvvvvvvvvv*****/


/***** Command handler interface *****/
void SDstorage::storeExecCmd(uint8_t cmd) {
  uint8_t i = 0;
  while (storeCmdHidx[i].cmdByte) {
    if (storeCmdHidx[i].cmdByte == cmd) {
#ifdef DEBUG_STORE_EXEC
      DB_PRINT(F("Executing GPIB secondary command: "),"");
      DB_HEX_PRINT(cmd);
#endif
      // Call handler
      (this->*storeCmdHidx[i].handler)();
      return;
    }
    i++;
  }
#ifdef DEBUG_STORE_EXEC
  DB_PRINT(F("Secondary command."),"");
  DB_HEX_PRINT(cmd);
  DB_RAW_PRINTLN(F("not found!"));
#endif
}


/***** COMMAND HANDLERS *****/

/***** STATUS command *****/
void SDstorage::stgc_0x60_h(){
  char statstr[5] = {0};
#ifdef DEBUG_STORE_STATUS
  DB_PRINT(F("sending status byte..."),"");
#endif
  itoa(gpibBus.cfg.stat, statstr, 10);
  // Send info to GPIB bus
  gpibBus.sendData(statstr, strlen(statstr), SEND_WITH_EOI);
#ifdef DEBUG_STORE_STATUS
  DB_PRINT(F("done."),"");
#endif  
}


/***** SAVE command *****/
void SDstorage::stgc_0x61_h(){

  TekFileInfo fileinfo;
  uint8_t r = 0;
  
#ifdef DEBUG_STORE_SAVE
  DB_PRINT(F("started SAVE handler..."),"");
#endif

  if ((f_type == 'N') || (f_type == 'P')) {  // OR 'P' ?
    // Read data into the file, re-writing its contents
    sdinout.rewind();
    r = gpibBus.receiveToFile(sdinout, true, false, 0);
    if (r == 0) {
      // End the file here
      sdinout.truncate();
      // Make sure file is makrked as ASCII PROG, with appropriate length and rename
      renameFile(sdinout, 'A', 'P');
#ifdef DEBUG_STORE_SAVE
    }else{
      DB_PRINT(F("receive failed!"),"");
#endif
    }
#ifdef DEBUG_STORE_SAVE
  }else{
    errorCode = 2;
    DB_PRINT(F("incorrect file type!"),"");
#endif
  }

#ifdef DEBUG_STORE_SAVE
    DB_PRINT(F("done."),"");
#endif  
}


/***** CLOSE command *****/
void SDstorage::stgc_0x62_h(){
  uint8_t r = 0;
  char pbuffer[12];
#ifdef DEBUG_STORE_CLOSE
  DB_PRINT(F("started CLOSE handler..."),"");
  DB_PRINT(F("closing: "), f_name);
#endif

  // Read optional file number with EOI detection
  r = gpibBus.receiveParams(true, pbuffer, 12);   // Limit to 12 characters
  r=r;  // Get rid of compiler warning

  // Close file handle
  closeFile();

#ifdef DEBUG_STORE_CLOSE
  DB_PRINT(F("done."),"");
#endif
}


/***** OPEN command *****/
void SDstorage::stgc_0x63_h(){
  
}


/***** OLD/APPEND command (tek_OLD) *****/
void SDstorage::stgc_0x64_h() {
  char path[full_path_size] = {0};
  char linebuffer[line_buffer_size];

  // line_buffer_size = 72 char line max in Tek plus CR plus NULL

#ifdef DEBUG_STORE_APPEND
  DB_PRINT(F("started OLD/APPEND handler..."),"");
#endif

  if (f_type == 'P') {

    strncpy(path, directory, strlen(directory));
    strncat(path, f_name, strlen(f_name));

#ifdef DEBUG_STORE_APPEND
    DB_PRINT(F("reading: "), path);
#endif

    ifstream sdin(path);

    while (sdin.getline(linebuffer, line_buffer_size, '\r')) {
      // getline() discards CR so add it back on
      strncat(linebuffer, "\r\0", 2);
    
      if (sdin.peek() == EOF) {
        // Last line was read so send data with EOI on last character
        gpibBus.sendData(linebuffer, strlen(linebuffer), SEND_WITH_EOI);
      }else{
        // Send line of data to the GPIB bus
        gpibBus.sendData(linebuffer, strlen(linebuffer), SEND_DATA_ONLY);
      }
    }

    // Close the file
    closeFile();
    
  }else{
    errorCode = 6;
    gpibBus.writeByte(0xFF, SEND_WITH_EOI);  // Send FF and signal EOI
#ifdef DEBUG_STORE_APPEND
    DB_PRINT(F("incorrect file type!"),"");
#endif
  }

#ifdef DEBUG_STORE_APPEND
    DB_PRINT(F("done."),"");
#endif

}


/***** TYPE command *****/
 /*
 * Extract type of the next DATA on tape 
 * 0 : Empty File (NEW or not open)
 * 1 : End-of-file character
 * 2 : Numeric or Character string DATA/ASCII Format
 * 3 : Numeric data/Binary Format
 * 4 : Character String/Binary Format
 * Note: this is not about the file type but the data in the file
 * 
 * Empty might be indicated by file size = 0
 * Otherwise perhaps need to scan ahead using seek() ?
 */
void SDstorage::stgc_0x66_h() {
  int dtype = 0;
  int dsize = 0;
  int peekb[2] = {0};
  char numstr[6] = {'\0'};

/* ........... */
  
#ifdef DEBUG_STORE_TYPE
  DB_PRINT(F("started TYPE handler..."),"");
#endif

  if ( (sdinout.isOpen()) && (f_type == 'H')) {

#ifdef DEBUG_STORE_TYPE
    DB_PRINT(F("getting type for:"),"");
    DB_PRINT(directory, f_name);
#endif

    peekb[0] = sdinout.peek();
    sdinout.seekCur(1);
    peekb[1] = sdinout.peek();
    sdinout.seekCur(-1);
    
    if (peekb[0]<0){
      dtype = 1;
    }else{
      dtype = (peekb[0]&0xE0)>>5;
      dtype  = dtype + 2; // code 1=> type 3, code 2=> type 4;
    }
    if ( (dtype>2) && (dtype<5) ) dsize = ((peekb[0]&0x1F)<<8) | peekb[1];
//    if ( (dtype>2) && (dtype<5) ) dsize = peekb[1];

    sprintf(numstr, "%d\x0D" ,dtype);
    gpibBus.sendData(numstr, strlen(numstr), SEND_DATA_ONLY);
    memset(numstr, '\0', 6);
    sprintf(numstr, "%d\x0D" ,dsize);
    gpibBus.sendData(numstr, strlen(numstr), SEND_WITH_EOI);

#ifdef DEBUG_STORE_TYPE
    DB_PRINT(F("type BINARY"),"");
#endif

  }else if ( (sdinout.isOpen()) && (f_type == 'D') ) {
    sprintf(numstr, "%d\x0D" ,2);
    gpibBus.sendData(numstr, 2, SEND_DATA_ONLY);
    memset(numstr, '\0', 6);
    sprintf(numstr, "%d\x0D" ,0);
    gpibBus.sendData(numstr, 2, SEND_WITH_EOI);
#ifdef DEBUG_STORE_TYPE
    DB_PRINT(F("type ASCII"),"");
#endif
  }else{
    sprintf(numstr, "%d\x0D" ,0);
    gpibBus.sendData(numstr, 2, SEND_DATA_ONLY);
    gpibBus.sendData(numstr, 2, SEND_WITH_EOI);
#ifdef DEBUG_STORE_TYPE
    DB_PRINT(F("NEW or not open"),"");
#endif
  }

#ifdef DEBUG_STORE_TYPE
  DB_PRINT(F("done."),"");
#endif    
}


/***** KILL command *****/
void SDstorage::stgc_0x67_h(){

  TekFileInfo fileinfo;
  char kbuffer[12] = {0};
  uint8_t r = 0;
  uint8_t fnum = 0;
  File fileObj;

#ifdef DEBUG_STORE_KILL
  DB_PRINT(F("started KILL handler..."),"");
#endif
  
  // Read the directory name data
  r = gpibBus.receiveParams(false, kbuffer, 12);   // Limit to 12 characters

  if (r > 1) {  // Blank buffer contains CR

    fnum = (uint8_t)atoi(kbuffer);

#ifdef DEBUG_STORE_KILL
    DB_PRINT(F("received parameters: "), kbuffer);
    DB_PRINT(F("File to kill: "), fnum);
#endif

    // Find the file to be wiped out
    if (searchForFile(fnum, fileObj)) {

      // Create empty new file
      makeNewFile(fileObj, 0);
    }else{
      errorCode = 2;
#ifdef DEBUG_STORE_KILL
      DB_PRINT(F("file not found!"),"");
#endif
    }

#ifdef DEBUG_STORE_KILL
  }else{
    DB_PRINT(F("receive params failed!"),"");
#endif
  }

#ifdef DEBUG_STORE_KILL
  DB_PRINT(F("done."),"");
#endif
}


/***** DIRECTORY / HEADER / CD command *****/
void SDstorage::stgc_0x69_h() {
/*
 * This function retrieves or sets the current directory name.
 * There is no CD command on the 4051 or 4924 so the
 * CD command is run using PRINT@5,9:
 */
  // limit the emulator dir name to single level "/" plus 8 characters trailing "/" plus NULL
  char dnbuffer[13] = {0};
//  uint8_t j = 1;
  uint8_t r = 0;

  // If addressed to listen, write the file
  if (gpibBus.isDeviceAddressedToListen()){
  
#ifdef DEBUG_STORE_DIR
    DB_PRINT(F("started CD/DIR handler..."),"");
#endif
  
    // Read the directory name data
    r = gpibBus.receiveParams(false, dnbuffer, 12);   // Limit to 12 characters

//  printHex(dnbuffer, strlen(dnbuffer));

    if (r > 1) {  // Blank buffer contains just CR

#ifdef DEBUG_STORE_DIR
      DB_PRINT(F("received directory name: "), dnbuffer);
//      DB_HEXB_PRINT(dnbuffer, 12);
#endif

      // Set directory (closes any open files, sets file index to 0)
      setDirectory(dnbuffer);

#ifdef DEBUG_STORE_DIR
      DB_PRINT(F("set directory name: "), directory);
//      DB_HEXB_PRINT(directory, 12);
#endif

    }
  }

  // If addressed to talk, send the current directory name
  if (gpibBus.isDeviceAddressedToTalk()){
    gpibBus.sendData(directory, strlen(directory), SEND_WITH_EOI);
  }
  
#ifdef DEBUG_STORE_DIR
  DB_PRINT(F("done."),"");  
#endif
}


/***** COPY command *****/
void SDstorage::stgc_0x6A_h() {

// Note: Reads BINARY files up to 0x80 character

#ifdef DEBUG_STORE_COPY
  DB_PRINT(F("started COPY handler..."),"");
#endif

//  if (f_type == 'H') {


#ifdef DEBUG_STORE_COPY
    uint8_t err = 0;
    DB_PRINT(F("reading: "),(String(directory)+f_name));
    err = binaryCopy();
#else
//    binaryRead(0x80);
    binaryCopy();   
#endif


#ifdef DEBUG_STORE_COPY
    if (err) DB_PRINT(F("error reading file!"),"");
#endif

/*
  }else{
#ifdef DEBUG_STORE_COPY
    DB_PRINT(F("incorrect file type!"),"");
#endif
  }
*/

  gpibBus.setControls(DLAS);  // Send complete. Drop back to listen and await next command.

#ifdef DEBUG_STORE_COPY
  DB_PRINT(F("done."),"");
#endif

}


/***** PRINT command *****/
void SDstorage::stgc_0x6C_h() {
#ifdef DEBUG_STORE_COPY
  DB_PRINT(F("started PRINT handler..."),"");
#endif
  if ( (f_type == 'N') || f_type == 'D') {
    gpibBus.receiveToFile(sdinout, true, false, 0);
    // Make sure file is makrked as BINARY DATA, with appropriate length and rename
    if (f_type == 'N') renameFile(sdinout, 'A', 'D');   
#ifdef DEBUG_STORE_COPY
  }else{
    DB_PRINT(F("incorrect file type!"),"");
#endif
  }
#ifdef DEBUG_STORE_COPY
    DB_PRINT(F("done."),"");
#endif
}


/***** INPUT command *****/
/*
 * Reads text files and returns the next item (line at present)
 * Note: EOF character = FF
 */

 /*
  * Working on 4051 
  * Working on 4052/4054 but requires debug to be disabled
  */

void SDstorage::stgc_0x6D_h() {

  int16_t c;
  uint8_t err = false;

  // line_buffer_size = 72 char line max in Tek plus CR plus NULL

#ifdef DEBUG_STORE_INPUT
  DB_PRINT(F("started INPUT handler..."),"");
#endif

  if ((f_type == 'D') || (f_type == 'P')) {


#ifdef DEBUG_STORE_INPUT
    DB_PRINT(F("reading: "),(String(directory)+f_name));
#endif

    while (sdinout.available()) {

      // Read a byte
      c = sdinout.read();
    
      if (c == EOF) {  // Reached EOF
        // Send EOI + 0xFF to indicate EOF reached
        err = gpibBus.writeByte(0xFF, SEND_WITH_EOI);

/*** IN LOOP - MAY IMPACT TIMING ***
#ifdef DEBUG_STORE_INPUT
        DB_PRINT(F("\nEOF reached!"),"");
#endif
*/
      }else{
        // Send byte to the GPIB bus
        err = gpibBus.writeByte((uint8_t)c, SEND_DATA_ONLY);

/*** IN LOOP - MAY IMPACT TIMING ***
#ifdef DEBUG_STORE_INPUT
        if (!err) DB_RAW_PRINT((char)c);
#endif
*/
      }

      // Exit on ATN or receiver request stop
      if (err) {
//        if (err == 1) DB_PRINT(F("\r\nreceiver requested stop!"),"");
        if (err == 2) {
          gpibBus.setControls(DLAS);
/*** IN LOOP - MAY IMPACT TIMING ***
#ifdef DEBUG_STORE_INPUT      
          DB_PRINT(F("\r\nATN detected."),"");
#endif
*/
        }

/*** IN LOOP - MAY IMPACT TIMING ***
#ifdef DEBUG_STORE_INPUT        
        if (err == 3) DB_PRINT(F("\r\nwait timeout."),"");
#endif
*/
        // Rewind file read by a character (current character has already been read)
        sdinout.seekCur(-1);
        break;
      }

    }

  }else{
    errorCode = 2;
    // Send EOI + 0xFF to indicate EOF (no data)
    err = gpibBus.writeByte(0xFF, SEND_WITH_EOI);
#ifdef DEBUG_STORE_INPUT
    DB_PRINT(F("incorrect file type!"),"");
#endif
  }

#ifdef DEBUG_STORE_INPUT
    DB_PRINT(F("done."),"");
#endif

}



/***** READ command (tek_READ_one) *****/
void SDstorage::stgc_0x6E_h() {

  int16_t c;
  int16_t header[2];
  uint16_t dlen = 0;
  uint8_t err = 0;
  uint16_t dtype = 0;

  if (f_type == 'H') {

#ifdef DEBUG_STORE_READ
    DB_PRINT(F("reading: "),(String(directory)+f_name));
#endif

    // READ loop

    while (!err) {  // Drop out on error

      // Get header bytes
      header[0] = sdinout.read();
      header[1] = sdinout.read();

      if ( (header[0]==-1) || (header[1]==-1) ) {

        // Reached EOF - send last byte and 0xFF with EOI
        err = gpibBus.writeByte(0xFF, SEND_WITH_EOI);
        return;
      }

      // Get data length and type
      dtype = (header[0] & 0xE0);
      dlen = (header[0] & 0x1F) << 8;
      dlen = dlen + (header[1] & 0xFF);
      if (dtype & 0x40) dlen++;   // Text data appends one additional byte

#ifdef DEBUG_STORE_READ
      DB_PRINT(F("\nData length: "),dlen);
#endif

      // Send header
      err = gpibBus.writeByte((uint8_t)header[0], SEND_DATA_ONLY);
      if (err) {
        sdinout.seekCur(-2);  // Move back to begining of header
        break;
      }
      err = gpibBus.writeByte((uint8_t)header[1], SEND_DATA_ONLY);
      if (err) {
        sdinout.seekCur(-2);
        break;
      }

      // Send data
      if (!err) {
        for (uint16_t i=0; i<(dlen); i++){
          c = sdinout.read();
          err = gpibBus.writeByte(c, SEND_DATA_ONLY);
          if (err) break;
/*     
#ifdef DEBUG_STORE_READ
          DB_HEX_PRINT(c)
#endif
*/
        }
      }

    } // !err

#ifdef DEBUG_STORE_READ
    if (err) {
      DB_PRINT(F("error: "), err);
    }
#endif
    
  }else{
#ifdef DEBUG_STORE_READ
    DB_PRINT(F("incorrect file type!"),"");
    err = gpibBus.writeByte(0xFF, SEND_WITH_EOI);
#endif
  }  

#ifdef DEBUG_STORE_READ
  DB_PRINT(F("done."),"");
#endif

}



/***** WRITE command *****/

void SDstorage::stgc_0x6F_h() {
#ifdef DEBUG_STORE_WRITE
  DB_PRINT(F("started WRITE handler..."),"");
#endif
  if ( (f_type == 'N') || (f_type == 'H') ) {
    gpibBus.receiveToFile(sdinout, true, false, 0);
   // Make sure file is makrked as BINARY DATA, with appropriate length and rename
   if (f_type == 'N') renameFile(sdinout, 'B', 'D');   
#ifdef DEBUG_STORE_WRITE
  }else{
    DB_PRINT(F("incorrect file type!"),"");
#endif
  }
#ifdef DEBUG_STORE_WRITE
    DB_PRINT(F("done."),"");
#endif
}


/*  
 * WRITE alt
 * 
void SDstorage::stgc_0x6F_h() {
  uint16_t dlen = 2;
  uint16_t hval = 0;
  uint8_t hmode = 0;
  uint16_t cnt = 0;
  uint8_t db;
  uint8_t r = 0;
  bool readWithEoi = false;
  bool eoiDetected = false;
#ifdef DEBUG_STORE_WRITE
  debugStream.println(F("stgc_0x6F_h: started WRITE handler..."));
#endif
  if ( (f_type == 'N') || (f_type == 'H') ) {
    while (cnt < dlen) {
      r = gpibBus.readByte(&db, readWithEoi, &eoiDetected);
      if (r>0) break;
      if (cnt == 0) {
        hmode = db & 0xE0;
        hval = db & 0x1F;
      }
      if (cnt == 1) {
        dlen = (hval>>8) + db + 2;
        if (hmode&0x40) dlen++;
      }

debugStream.print(F("\nCnt: "));
debugStream.print(cnt);
debugStream.print(F("  Hmode: "));
debugStream.print(hmode);
debugStream.print(F("  Dlen: "));
debugStream.println(dlen);

      sdinout.write(db);
#ifdef DEBUG_STORE_WRITE
      char x[4] = {0};
      sprintf(x,"%02X ",db);    
      debugStream.print(x);
#endif   
      cnt++;
    }
*/
/* 
  }
    r = gpibBus.readByte(&db, readWithEoi, &eoiDetected);
    if (r==0) dlen = hb1 = (db & 0x1F) << 8;
    r = gpibBus.readByte(&db, readWithEoi, &eoiDetected);
    if (r==0) {
      dlen = dlen + db;
      for (uint16_t i=0; i<dlen; i++) {
        r = gpibBus.readByte(&db, readWithEoi, &eoiDetected);
        if (r>0) break;
        sdinout.write(db);

#ifdef DEBUG_STORE_WRITE
      char x[4] = {0};
      sprintf(x,"%02X ",db);    
      debugStream.print(x);
#endif

    }
*/
/*
    if (r==0) {
      // Make sure file is makrked as BINARY DATA, with appropriate length and rename
      if (f_type == 'N') renameFile(sdinout, 'B', 'D');
#ifdef DEBUG_STORE_WRITE
    }else{
      debugStream.print("   Error: ");
      debugStream.println(r);
#endif
    }
#ifdef DEBUG_STORE_WRITE
  }else{
    debugStream.println(F("stgc_0x6F_h: incorrect file type!"));
#endif
  }
#ifdef DEBUG_STORE_WRITE
    debugStream.println(F("stgc_0x6F_h: done."));
#endif
}
*/


/***** BSAVE / BOLD command *****/
/*
 * 4051 binary program loader
 * CALL "BSAVE",5
 * CALL "BOLD",5
 */
void SDstorage::stgc_0x71_h() {

  char fname[file_header_size];
  TekFileInfo fileinfo;
  uint8_t r = 0;
  uint8_t err = 0;

#ifdef DEBUG_STORE_BINARYIO
  DB_PRINT(F("started BSAVE/BOLD handler..."),"");
#endif

  // If addressed to listen, write (BSAVE) the file
  if (gpibBus.isDeviceAddressedToListen()){
    sdinout.getName(fname, file_header_size);
    fileinfo.setFromFilename(fname);
    if ((fileinfo.getFtype()=='N')||((fileinfo.getFtype()=='B')&&(fileinfo.getFusage()=='P')) ){
#ifdef DEBUG_STORE_BINARYIO
      DB_PRINT(F("writing file..."),"");
#endif
      r = binaryWrite();
      if (r==0) { // No errors
        // End the file here
        sdinout.truncate();
        if (fileinfo.getFtype() == 'N') {
          renameFile(sdinout, 'B', 'P');
        }
      }
    }else{
      err = 5;
      errorCode = 6;
    }
  }

  // If addressed to talk read (BOLD) the file  
  if (gpibBus.isDeviceAddressedToTalk()){
    sdinout.getName(fname, file_header_size);
    fileinfo.setFromFilename(fname);
    if ((fileinfo.getFtype()=='B')&&(fileinfo.getFusage()=='P')){
#ifdef DEBUG_STORE_BINARYIO
      DB_PRINT(F("reading file..."),"");
#endif
      err = binaryRead();
    }else{
      err = 5;
      errorCode = 6;
      gpibBus.writeByte(0xFF, SEND_WITH_EOI);  // Send FF and signal EOI
    }
  }

#ifdef DEBUG_STORE_BINARYIO
  if (err==5) DB_PRINT(F("incorrect file type!"),"");
  DB_PRINT(F("done."),"");
#endif

  r = err;  // Gets rid of compiler warning

}


/***** LIST / TLIST command *****/
/*
 * Retrieve or rename the next file name in the directory
 * INPUT@5,19:A$ retrieves thenext file name into A$ 
 * PRINT@5,19:A$ renames the current file
 */
void SDstorage::stgc_0x73_h(){
/*
  char fname[file_header_size+1] = {0};
  bool found = false;
  File fileObj;
  TekFileInfo tekfile;
*/
  char readbuffer[line_buffer_size] = {0};  // Buffer must be bigger than file length
  char fname[file_header_size] = {0};
  uint8_t r = 0;
  File dirObj;

#ifdef DEBUG_STORE_TLIST
  DB_PRINT(F("started TLIST handler..."),"");
#endif

  // If addressed to listen, rename the file
  if (gpibBus.isDeviceAddressedToListen()){
    // Open the current directory
#ifdef DEBUG_STORE_TLIST
    DB_PRINT(F("Opening "), directory);
#endif
    if (dirObj.open(directory, O_RDONLY)) {
#ifdef DEBUG_STORE_TLIST
      DB_PRINT(F("Opened "),directory);
#endif
      // Read the file name  from GPIB and rename the file
      r = gpibBus.receiveParams(false, readbuffer, line_buffer_size);   // Limit to file_header_size characters including terminating null
//      DB_PRINT(F("R: "), r);
//      DB_PRINT(F("Filename: "), fname);
//      DB_PRINT(F("Len fname: "), strlen(fname));
      
      // If we have received a filename then rename the file
      if (r>0) {
        // Copy max [file_header_size] characters to fname
        strncpy(fname, readbuffer, file_header_size); 
        // Remove any trailing CR/LF
        stripLineEnd(fname, strlen(fname));
#ifdef DEBUG_STORE_TLIST
        DB_PRINT(F("Current filename: "), f_name);
//        DB_HEXB_PRINT(fname, strlen(fname));
        DB_PRINT(F("Rename to: "), fname);
#endif
        if (sdinout.rename(&dirObj, fname)) {
          strncpy(f_name, fname, file_header_size);
#ifdef DEBUG_STORE_TLIST
          DB_PRINT(F("rename succeeded."),"");
#endif
        }else{
          errorCode = 4;
#ifdef DEBUG_STORE_TLIST
          DB_PRINT(F("rename failed!"),"");
#endif
        }        
      }
    }
  }else{
    // If its not a NULL string then return the file name
    if (f_name[0]>0) {
      gpibBus.sendData(f_name, file_header_size, SEND_WITH_EOI);
    }else{
      gpibBus.writeByte(0xFF,SEND_WITH_EOI);
    }
  }

#ifdef DEBUG_STORE_TLIST
  DB_PRINT(F("done."),"");  
#endif

}


/***** FIND command *****/
/*
 * FINDs and OPENs file (num)
 * Returns: string result= "A" for ASCII, "H" for HEX (Binary or Secret file), "N" for Not Found
 * FIND iterates through each file in a directory until filenumber matches num
 * since SdFat file index is not sequential with Tek 4050 filenames 
 */
void SDstorage::stgc_0x7B_h(){
  char receiveBuffer[83] = {0};   // *0 chars + CR/LF + NULL
  uint8_t paramLen = 0;
  uint8_t num = 0;
  File findfile;
//  bool found = false;

#ifdef DEBUG_STORE_FIND
  DB_PRINT(F("started FIND handler..."),"");
#endif

  // Close currently open work file (clears handle, f_name and f_type)
  if (sdinout.isOpen()) closeFile();

  // Read file number parameter from GPIB
  paramLen = gpibBus.receiveParams(false, receiveBuffer, 83);

  // If we have received parameter data
  if (paramLen > 1) {   // Blank buffer contains only CR

#ifdef DEBUG_STORE_FIND
    DB_PRINT(F("received parameter: "), receiveBuffer);
#endif

    num = atoi(receiveBuffer);

#ifdef DEBUG_STORE_FIND

    bool found = findFile(num);

    if (found) {
      DB_PRINT(F("found: "), f_name);
      DB_PRINT(F("type:  "), f_type);
    }else{
      errorCode = 2;
      DB_PRINT(F("file "), (String(num)+" not found!"));
    }
#else
    findFile(num);
#endif

#ifdef DEBUG_STORE_FIND
  }else{
    DB_PRINT(F("no search criteria!"),"");
#endif
  }

#ifdef DEBUG_STORE_FIND
  DB_PRINT(F("done."),"");
#endif  
}


/***** MARK command *****/
void SDstorage::stgc_0x7C_h(){
  char fname[file_header_size] = {0};
  char mpbuffer[13] = {0};
  uint8_t numfiles = 0;
  uint8_t lastfilenum = 0;
  uint8_t curfilenum = 0;
  uint16_t flen = 0;
  uint8_t i = 0;
  File dirObj;
  File markFile;
  TekFileInfo fileinfo;

#ifdef DEBUG_STORE_MARK
  DB_PRINT(F("started MARK handler..."),"");
#endif

  // Read the requested number of files
  gpibBus.receiveParams(false, mpbuffer, 12);   // Limit to 12 characters
  numfiles = atoi(mpbuffer);
  // read the requested file size
  gpibBus.receiveParams(false, mpbuffer, 12);   // Limit to 12 characters
  flen = atoi(mpbuffer);

#ifdef DEBUG_STORE_MARK
  DB_PRINT(F("Number of files requested: "), numfiles);
  DB_PRINT(F("File length (bytes): "), flen);
#endif

  // Get the number of the current file
  sdinout.getName(fname, file_header_size);
  curfilenum = atoi(fname);

#ifdef DEBUG_STORE_MARK
  DB_PRINT(F("current file: "), curfilenum);
#endif

  // Get the number of the LAST file
  lastfilenum = getLastFile(markFile);

#ifdef DEBUG_STORE_MARK
  DB_PRINT(F("LAST file: "), lastfilenum);
#endif

  if (dirObj.open(directory, O_RDONLY)) {

    // Adding 'numfiles' cannot exceed files_per_directory - 1 (i.e. excluding LAST) so reduce 'numfiles' accordingly if required
    if (numfiles > (files_per_directory - (curfilenum + 1))) numfiles = files_per_directory - (curfilenum + 1);

    if (numfiles > 0) {

#ifdef DEBUG_STORE_MARK
      DB_PRINT(F("files to MARK: "), numfiles);
#endif

      // Delete everything from the current file to the last file
      for (i=curfilenum; i<=lastfilenum; i++) {
        if (searchForFile(i, markFile)){
#ifdef DEBUG_STORE_MARK
          bool r = false;
          r = markFile.remove();
#else
          markFile.remove();
#endif
          markFile.close();
#ifdef DEBUG_STORE_MARK
          if (!r) {
            DB_PRINT(F("failed to delete file "), i);
          }
#endif
#ifdef DEBUG_STORE_MARK
        }else{
          DB_PRINT(F("Unable to find file: "), i);
#endif
        }
      }

      // Generate 'numfiles' number of NEW files starting at the current file position
      for (i=curfilenum; i<curfilenum+numfiles; i++) {
        
        // Generate a new file name
        fileinfo.clear();
        fileinfo.setFnumber(i);
        fileinfo.setFtype('N');
        fileinfo.setFsize(flen);
        fileinfo.getFilename(fname);

        // Create a NEW file with the generated name
        if (markFile.open(&dirObj,fname,(O_RDWR | O_CREAT | O_AT_END))) { // Create new file if it doesn't already exist
          markFile.close();
#ifdef DEBUG_STORE_MARK
          DB_PRINT(F("created: "),(String(directory)+fname));
#endif
        }else{
#ifdef DEBUG_STORE_MARK
          DB_PRINT(F("Failed to create: "), fname);
#endif
        }
      }

      // Generate the LAST file name
      fileinfo.clear();
      fileinfo.setFnumber(i);
      fileinfo.setFtype('L');
      fileinfo.setFsize(0);
      fileinfo.getFilename(fname);

      // Create a LAST file with the generated name
      if (markFile.open(&dirObj,fname,(O_RDWR | O_CREAT | O_AT_END))) { // Create new file if it doesn't already exist
        markFile.close();
#ifdef DEBUG_STORE_MARK
        DB_PRINT(F("created LAST file"),"");
#endif
      }else{
#ifdef DEBUG_STORE_MARK
        DB_PRINT(F("Failed to create LAST file!"),"");
#endif
      }
      dirObj.close();
    }else{
      errorCode = 2;
#ifdef DEBUG_STORE_MARK
      DB_PRINT(F("LAST not found!"),"");
#endif
    }  

  }else{
    errorCode = 3;
#ifdef DEBUG_STORE_MARK
    DB_PRINT(F("failed to open directory: "), directory);
#endif
  }

#ifdef DEBUG_STORE_MARK
  DB_PRINT(F("done."),"");
#endif
}


/***** SECRET command *****/
/*
void SDstorage::stgc_0x7D_h(){
  
}
*/

/***** ERROR command *****/
/*
 *  1 - Domain error / invalid argument
 *  2 - File not found
 *  3 - Mag tape format error
 *  4 - Illegal access
 *  5 - File not open
 *  6 - Read error
 *  7 - No cartridge inserted
 *  8 - Over read (illegal tape length)
 *  9 - Write protected
 * 10 - Read after write error
 * 11 - End of medium
 * 12 - End of file
 */
void SDstorage::stgc_0x7E_h(){
  char errstr[5] = {0};
#ifdef DEBUG_STORE_ERROR
  DB_PRINT(F("sending error code "), errorCode);
#endif
//  charStream.flush();
//  gpibout << errorCode;
  itoa(errorCode, errstr, 10);
  // Send info to GPIB bus
//  gpibBus.sendRawData(charStream.toCharArray(), charStream.length());
//  gpibBus.sendRawData(errstr, strlen(errstr));
  gpibBus.sendData(errstr, strlen(errstr), SEND_WITH_EOI);
#ifdef DEBUG_STORE_ERROR
  DB_PRINT(F("done."),"");
#endif
}



/*****^^^^^^^^^^^^^^^^^^^^^^^^^^^^*****/
/***** Command handling functions *****/
/**************************************/




/***** TLIST command *****/
/*
  uint8_t number = 1;
  uint8_t index = 0;
  bool lastfile = false;

  SdFile dirFile;
  SdFile file;

#ifdef DEBUG_STORE_COMMANDS
  debugStream.println(F("stgc_0x7C_h: started TLIST handler..."));
#endif

  // match the specific Tek4924 tape file number from f_name
//  for (int number = 1; number < nMax; number++) { // find each file in # sequence
//  for (uint8_t number = 1; number < nMax; number++) { // find each file in # sequence
  while ((!lastfile) && (number < nMax)) {

    //cout << "Directory: " << directory << '\r\n';
    f_name[0] = 0;  // clear f_name, otherwise f_name=last filename
    file.close();  //  close any file that could be open (from previous FIND for example)

    if (!dirFile.open(directory, O_RDONLY)) {
//      sd.errorHalt("Open directory failed - possible invalid directory name");
      errorCode = 1;
#ifdef DEBUG_STORE_COMMANDS
      debugStream.print(F("stgc_0x7C_h: open directory '"));
      debugStream.print(directory);
      debugStream.println(F("' failed - name may be invalid"));

//      debugStream << F("This is a ") << number << F(" test!");
#endif
      return;
    }
    file.rewind();

//    for (int index = 0; index < nMax; index++) { //SdFat file index number
//    for (uint8_t index = 0; index < nMax; index++) { //SdFat file index number

    while ((file.openNext(&dirFile, O_RDONLY)) && (index < nMax)) {

      // while (n < nMax ) {
//      file.openNext(&dirFile, O_RDONLY);
      // Skip directories, hidden files, and null files
      if (!file.isSubDir() && !file.isHidden()) {

        file.getName(f_name, 46);

//        int filenumber = atoi(f_name);
        uint8_t filenumber = atoi(f_name);

        if (filenumber == number) {
          // print the entire file 'header' with leading space, and CR + DC3 delimiters
          // Note \x019 = end of medium
//          cout << F(" ") << f_name << '\r' << '\x019';

          // Stream info to buffer 
          charStream.flush();
//          gpibout.flush();
//          charStream << F(" ") << f_name << '\r' << '\x019';
//charStream.print("Test");
        
          gpibout << F(" ") << f_name << '\r' << '\x019';
          
          // Send info to GPIB bus
          gpibBus.sendRawData(charStream.toCharArray(), charStream.length());
//          gpibBus.sendRawData(gpibout.toCharArray(), gpibout.length());
        
        }
        
        if ((f_name[7] == 'L') && (filenumber == number)) {
          // then this is the LAST file - end of TLIST
//          file.close();  // end of iteration close **this** file
//          f_name[0] = 0;  // clear f_name, otherwise f_name=last filename
//          f_type = 'O';    // set file type to "O" for NOT OPEN
//          dirFile.close();  // end of iteration through all files, close directory

#ifdef DEBUG_STORE_COMMANDS
          debugStream.println(F("stgc_0x7C_h: reached last file."));  
#endif    
          lastfile = true;
          break;
//          return;

        } else if (file.isDir()) {
          // Indicate a directory.
//          cout << '/' << endl;

          // Stream info to buffer
          charStream.flush();
//          gpibout.flush();
          gpibout << '/' << endl;
          // Send info to GPIB bus
          gpibBus.sendRawData(charStream.toCharArray(), charStream.length());
//          gpibBus.sendRawData(gpibout.toCharArray(), gpibout.length());

        }

      }
      index++;
      file.close();  // end of iteration close **this** file
    }

    number++;
    f_name[0] = 0;  // clear f_name, otherwise f_name=last filename
    f_type = 'O';    // set file type to "O" for NOT OPEN
    dirFile.close();  // end of iteration through all files, close directory
  } 

  // Send EOI to end transmission
  gpibBus.sendEOI();

#ifdef DEBUG_STORE_COMMANDS
  debugStream.println(F("stgc_0x7C_h: end TLIST handler."));  
#endif
*/
/***** TLIST command *****/



#endif // EN_STORAGE
