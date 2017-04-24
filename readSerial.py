import serial
import os
import smtplib

ser = serial.Serial('COM3', 9600)
while (1):
	
	s = ser.readline().rstrip()

	print(s)
	if s == b'Shut down':
	
		os.system("shutdown -s")
		
		server = smtplib.SMTP('smtp.gmail.com', 587)
		server.starttls()
		server.login('Email#', 'Password#')
		server.sendmail('Email#', 'Number#', 'Message#')
		break
	
	
print("Shutting down system...")