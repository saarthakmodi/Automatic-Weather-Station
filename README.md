# Automatic-Weather-Station
PS-1 Project, Indian Meteorological Department, Pune
<h2> Wind Vane </h2>
<strong> Sensor manufactured by the department itself</strong>

Non-RS232 sensor, operates on TTL and can therefore be directly interfaced to the Arduino board via Analog Pins.

				N(180)


		W(90)				E(270)
	

				(0)S(360)


Check resistance of sensor first using multimeter before connecting to ensure resistance scheme is as above. Otherwise make appropriate changes in the code.

Analog values are received by connecting the sensor in a voltage divider circuit with a reference resistance.
Only two pins present can be connected in any order.

<h2> Temperature and Humidity Probe </h2>
<strong>Sensor Name: RHT 175 </strong>	

Also operates on TTL Logic, but Analog values obtained need to be scaled down to obtain meaningful data.

Connections according to colour.
> BLUE ---> TEMPERATURE ----> A1

> PINK ----> HUMIDITY ----> A2

> WHITE ----> 5V

> THICK BLACK , BROWN, VIOLET ----> GND
