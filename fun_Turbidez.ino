String SensorTurbidez(){
  
  float ntu;
  float Tensao;
  float sensorValue = analogRead(33);// read the input on analog pin 0:
  Tensao = ((sensorValue/2435.71)*5.0) ; // Convert the analog reading (which goes from 0 - 1023) to a Tensão (0 - 5V):
  Serial.println("Tensão:");
  Serial.println(Tensao);
  if(Tensao<2.5){
     ntu=3000;
  }else  if(Tensao>4.2){
    ntu=0;
  }else{ 
    ntu = (-1120.4*(Tensao*Tensao) +5742.3*Tensao -4353.8);}//-1120.4*sqrt(Tensao) +5742.3*Tensao -4353.8);}
    
  Serial.println("DataPin:"); // print out the value you read: printf("Int printf(\"GeeksForGeeks\\n \") = %d\n", val);
  Serial.println(sensorValue);

  Serial.println("Turbidez:");
  Serial.println(ntu);
  delay(1000);
  
  return String("{'ntu':'"+String(ntu)+"','Tensao':'"+String(Tensao)+"','sensorValue':'"+String(sensorValue)+"'}");
  
  }
