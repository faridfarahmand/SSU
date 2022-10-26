void connectionDetails()
{
  // Details of the connection
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
  Serial.print("Channel:\t");
  Serial.println(WiFi.channel());          
  Serial.print("RSSI:\t");
  Serial.println(WiFi.RSSI());   
  Serial.print("DNS IP:\t");
  Serial.println(WiFi.dnsIP(1));            // get the DNS IP Address    
  Serial.print("Gateway IP:\t");
  Serial.println(WiFi.gatewayIP()); 
  Serial.println("---------------------");    
}