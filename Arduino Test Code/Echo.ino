//Created by Manolis Lykos, Maria Argyriou, Giannhs Vardaksis 20/11/2017

String data; //string that stores the incoming message

void setup()	//runs only once
{
  Serial.begin(9600); //set baud rate which is the default baud rate of hc 06
}

void loop()
{
  while(Serial.available())//while there is data available on the serial monitor
  {
    data+=char(Serial.read());//read data and store them
  }
  if(!Serial.available())	//if there are no more data. this ensures that after the user stops writing, no data are lost
  {
    if(data!="") //if data is available
    {
      Serial.println(data); //show the data
      data=""; //clear the data
    }
  }
  delay(5000); //delay
}
