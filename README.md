# mqtt
Creating to test mqtt protocol

#To install and test mosquitto in linux
sudo apt update
sudo apt install mosquitto mosquitto-clients
 
From terminal 1: mosquitto_sub -h localhost -t test/topic
From terminal 2 : mosquitto_pub -h localhost -t test/topic -m "Hello MQTT!"
