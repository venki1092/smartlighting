VirtualSensorController: Json_Parser.c client_shared.c mosquitto_pub.c SampleCurl.cpp
	g++ -o VirtualSensorController.out Json_Parser.c client_shared.c mosquitto_pub.c SampleCurl.cpp -lcurl -lmosquitto -fpermissive

