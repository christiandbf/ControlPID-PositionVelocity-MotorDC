//Include Device Specific Header From Sketch>>Import Library (In This Case LinxChipkitMax32.h)
//Also Include Desired LINX Listener From Sketch>>Import Library (In This Case LinxSerialListener.h)
#include <LinxArduinoUno.h>
#include <LinxSerialListener.h>

//Custom library for encoder
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
 
//Create A Pointer To The LINX Device Object We Instantiate In Setup()
LinxArduinoUno* LinxDevice;

//Initializing encoder
Encoder encoder(2, 3);

//Forward declaration of functions to invoke
int getPulsesEncoder();
int resetPulsesEncoder();
int getPulsesEncoderAndTimeStamp();

//Initialize LINX Device And Listener
void setup()
{
  //Instantiate The LINX Device
  LinxDevice = new LinxArduinoUno();
  
  //The LINXT Listener Is Pre Instantiated, Call Start And Pass A Pointer To The LINX Device And The UART Channel To Listen On
  LinxSerialConnection.Start(LinxDevice, 0);  

  //Attach command to a LINX Listener
  LinxSerialConnection.AttachCustomCommand(0, getPulsesEncoder);
  LinxSerialConnection.AttachCustomCommand(1, resetPulsesEncoder);
  LinxSerialConnection.AttachCustomCommand(2, getPulsesEncoderAndTimeStamp);
}

void loop()
{
  //Listen For New Packets From LabVIEW
  LinxSerialConnection.CheckForCommands();
  
  //Your Code Here, But It will Slow Down The Connection With LabVIEW
}

//Function to execute when custom command 0 is invoked
int getPulsesEncoder(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
  long pulses = encoder.read();
  memcpy(response, &pulses, sizeof(pulses));
  *numResponseBytes = sizeof(pulses);
  return 0;
}

//Function to execute when custom command 1 is invoked
int resetPulsesEncoder(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response) {
  encoder.write(0);
  return 0;
}

//Function to execute when custom command 2 is invoked
int getPulsesEncoderAndTimeStamp(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
  long pulses = encoder.read();
  unsigned long timeStamp = micros();
  long dataToSend[] = {pulses, timeStamp};
  memcpy(response, &dataToSend, sizeof(dataToSend));
  *numResponseBytes = sizeof(dataToSend);
  return 0;
}
