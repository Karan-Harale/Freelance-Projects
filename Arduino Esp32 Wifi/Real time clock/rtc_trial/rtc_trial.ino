#include <SPI.h>
#include <SD.h>                                                          // for the SD card
#include <RTClib.h>                                                     // for the RTC

Sd2Card card; SdVolume volume; SdFile root;

const int chipSelect = 10;                                            // define select pin for SD card module
File myFile;                                                         // Create a file to store the data
RTC_DS1307 rtc;                                                      // Instance of the class for RTC
 
void setup() { 
   Serial.begin(9600);                                              // Init Serial port & sd card
  SD.begin(10);

  Serial.println("Initializing SD card...");                                       // we'll use the initialization code from the utility libraries
                                                                                   // since we're just testing if the card is working!
   if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    while (1);
  }
  else 
  {
    Serial.println("Wiring is correct and a card is present.");
  }


  Serial.println();
  Serial.print("Card type:         ");
  
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }
 
                                                // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    while (1);
  }
 
                                             // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("Volume type is:    FAT");
  Serial.println(volume.fatType(), DEC);
 
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
  Serial.print("Volume size (Kb):  ");
  Serial.println(volumesize);
  Serial.print("Volume size (Mb):  ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Gb):  ");
  Serial.println((float)volumesize / 1024.0);
  Serial.println();
  Serial.println("Files found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
 
  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
  }





  
  void loop() {                                                  // Open file
  myFile=SD.open("Data.txt", FILE_WRITE);
                                                // If the file opened ok, write to it
  if (myFile) {
    Serial.println("File opened ok");
  
        // Save time on SD card
    DateTime now = rtc.now();
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print(',');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.println(now.minute(), DEC);
    
    // Print time on Serial monitor
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.println(now.minute(), DEC); 
 

  }
}
 
