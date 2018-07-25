#include <windows.h>
#include <stdio.h>
#define buffsize 10000
int main(int argc, char* argv[])
{
  HANDLE hComm;
  char ComPortName[] = "COM1";       // Name of the Serial port(May Change) to be opened,
  char TempChar;
  char SerialBuffer[buffsize];               // Buffer Containing Rxed Data
  int i = 0;
  BOOL Status;                           // Status of the various operations
  DWORD dwEventMask;                     // Event mask to trigger
  DWORD NoBytesRead;                     // Bytes read by ReadFile()
  DCB dcbSerialParams = { 0 };       // Initializing DCB structure
  COMMTIMEOUTS timeouts = { 0 };
  FILE *fp;


  hComm = CreateFile(argv[1],                //port name
                     GENERIC_READ | GENERIC_WRITE,       //Read/Write
                     0,                             // No Sharing
                     NULL,                          // No Security
                     OPEN_EXISTING,      // Open existing port only
                     0,             // Non Overlapped I/O
                     NULL);         // Null for Comm Device

  if (hComm == INVALID_HANDLE_VALUE)
    printf("Error in opening serial port\n");
  else{
    printf("opening serial port successful\n");

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    Status = GetCommState(hComm, &dcbSerialParams);
    dcbSerialParams.BaudRate = atoi(argv[2]);
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;
    SetCommState(hComm, &dcbSerialParams);

    timeouts.ReadIntervalTimeout         = 500;
    timeouts.ReadTotalTimeoutConstant    = 500;
    timeouts.ReadTotalTimeoutMultiplier  = 20;
    if (SetCommTimeouts(hComm, &timeouts) == FALSE)
      printf("Error! in Setting Time Outs/n");
    else
      printf("Setting Serial Port Timeouts Successfull\n");
    Status = SetCommMask(hComm, EV_RXCHAR);

    while(1) {
      Status = WaitCommEvent(hComm, &dwEventMask, NULL);
      i=0;
      SerialBuffer[0] = '\0';
      do
      {
        ReadFile( hComm,
                  &TempChar,
                  sizeof(TempChar),
                  &NoBytesRead,
                  NULL);
        SerialBuffer[i] = TempChar;
        i++;
        SerialBuffer[i] = '\0';
        TempChar = '\0';
      }
      while ((NoBytesRead > 0) || (i > buffsize - 3));
      if(SerialBuffer[1])                   //bugfix rx carattere vuoto
      {
        printf("%s\n", SerialBuffer);
        if(argc > 3)
        {
          fp=fopen(argv[3], "a");
          if(fp == NULL)
            exit(-1);
          fprintf(fp, "%s\n", SerialBuffer);
          fclose(fp);
          if(argc > 4)
          {
            if(CopyFile(argv[3], argv[4], FALSE))
              printf("copia file ok\n");
          }
        }
      }
    }

    CloseHandle(hComm);
  }
  return 0;
}
