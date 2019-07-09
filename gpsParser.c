#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char** argv){
    FILE *sp;  //pointer to stream
    sp = popen("/bin/ls /dev/", "r");  //gets output stream from running ls
    char ttyTest[] = "ttyUSB";  //string that will be tested below
    char devices[100];  //temporary array for storing devices in /dev/
    char *testedPointer;  //temporary pointer to the memory address in sp that contains the string "ttyUSB"
    int devCounter = 0;  //counter for number of USB ttys in /dev/
    char usbDevices[9][10];  //array of USB tty device names
    while (fgets(devices, sizeof(devices), sp) != NULL) { //reads characters from stream sp and iterates through each device with while{}
        testedPointer = strstr(devices, ttyTest);  //tests each line if it contains the string "ttyUSB"
        if (testedPointer != NULL){  //if it contains "ttyUSB"
            printf("DEBUG: Got USB tty device %d\n", devCounter + 1);  //Debugging output
            strcpy(usbDevices[devCounter], devices);  //copies ttyUSB devices found into array usbDevices
            devCounter++;  //increments counter of amount of USB devices
        }
    }
    char ttyNumChar;  //byte that stores the CHARACTER representation of the USB device number, ie for ttyUSB1, this variable has the value 49, or 0b00110001 ("1" in ascii)
    if (devCounter == 0){
        printf("DEBUG: No USB ttys attached\n");  //case if no ttyUSB devices are attached
        return -1;
    }
    else if(devCounter == 1){
        printf("DEBUG: One USB tty attached, defaulting to %s", usbDevices[0]);  //defaults to only ttyUSB available if only one exists
        ttyNumChar = usbDevices[0][6];  //selects 7th character of ttyUSBn, storing n as ttyNumChar as a CHARACTER BYTE
        //printf("%c", usbDevices[0][6]);
    }
    else{
        printf("DEBUG: More than one USB tty attached, select a device from below: (cat device beforehand to make sure you select the GPS module\n");  
        for(int i = 0; i < 9; i++){  //iterates through all 9 possible ttyUSB devices
            testedPointer = strstr(usbDevices[i], ttyTest);
            if (testedPointer != NULL){
                printf("Device [%d]: %s", i+1, usbDevices[i]);  //prints ttyUSB devices available for user to later pick
            }
        }
        int userInput;  //userinput for device number selection, follows number scheme presented by else{} above
        scanf("%d", &userInput);  //scans in single INTEGER for selection of USB device number
        ttyNumChar = usbDevices[userInput - 1][6];  //selects ttyNumChar using aforementioned else{} numbering scheme, and stores CHARACTER representation of n in ttyUSBn 
    }
    printf("DEBUG: You selected (char) ttyUSB%c\n", ttyNumChar);  //output for clarity
    char tty[13] = "/dev/ttyUSB";  //another string to test against
    tty[11] = ttyNumChar;  //makes n in /dev/ttyUSBn equal to the previously selected value
    printf("DEBUG: You selected (char full) %s\n", tty);
    int fileDescriptor, serialReadLine;
    char readBuffer[255];  //buffer for reading in serial data
    fileDescriptor = open(tty, O_RDWR | O_NOCTTY );  //opens socket to serial port
    if (fileDescriptor < 0) { //checks for a valid file descriptor
        perror(tty); 
        exit(-1); 
    }
    char *gpggaPointer;  //pointer to readBuffer
    while (1) {  //reads serial continuously until a $GPGGA packet is sent
        serialReadLine = read(fileDescriptor, readBuffer, 255);  //read serial
        readBuffer[serialReadLine] = 0;
        gpggaPointer = strstr(readBuffer, "$GPGGA");  //compare readBuffer with $GPGGA string
        if (gpggaPointer != NULL){  //if valid $GPGGA packet is in buffer, then stop while loop
            break;
        }
        
    }
    char gpggaData[51];  //array for gpgga data to be stored in
    strcpy(gpggaData, gpggaPointer);  //copy string from pointer to new array (new array not actually used for anything apart from printing gpgga data)
    printf("GPGGA Data:\n%s", gpggaData);
    char degreesNSChar[] = "  ";  //next lines setup character arrays for different GPS data parts
    char minutesNSChar[] = "        ";   
    char NS[] = " ";
    char degreesEWChar[] = "   ";
    char minutesEWChar[] = "        ";
    char EW[] = " ";
    strncpy(degreesNSChar, gpggaPointer+17, 2);  //next lines copy different GPS data into arrays above
    strncpy(minutesNSChar, gpggaPointer+19, 8);
    strncpy(NS, gpggaPointer+28, 1);
    strncpy(degreesEWChar, gpggaPointer+30, 3);
    strncpy(minutesEWChar, gpggaPointer+33, 8);
    strncpy(EW, gpggaPointer+42, 1);
    double degreesNS, minutesNS, degreesEW, minutesEW;  //floats for a numerical representation of above data
    degreesNS = atof(degreesNSChar);  //convert strings above to long floats
    minutesNS = atof(minutesNSChar);
    degreesEW = atof(degreesEWChar);
    minutesEW = atof(minutesEWChar);
    double decimalRepNS = degreesNS + (minutesNS / 60.0f);  //calculate decimal representation of degrees from degrees and minutes above (there are 60 minutes per degree)
    double decimalRepEW = degreesEW + (minutesEW / 60.0f);
    printf("Position is %lf%s, %lf%s\n", decimalRepNS, NS, decimalRepEW, EW);  //display position
    return(0);  //done



}