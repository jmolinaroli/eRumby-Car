import sys    
import os
import serial
import time

s=serial.Serial("/dev/ttyACM0",19200,timeout=2)

s.flushInput()
s.flushOutput()
nome_prova='test1.txt'

file_txt=open(nome_prova,"w")

data_ora = time.asctime( time.localtime(time.time()) )

#intestazione_documento

file_txt.write("#PROVA :  " )
file_txt.write(nome_prova)
file_txt.write("\n#")
file_txt.write(data_ora)
file_txt.write("\n#")

file_txt.write("[deg]\t[deg]\t[deg]\t[?]\t[?]\t[?]\t[?]\t[?]\t[?]\n")
file_txt.write("#roll\tpitch\tyaw\tacc_x\tacc_y\tacc_z\tgyro_x\tgyro_y\tgyro_z\n")

a='a'

#invio valori a parte ATmega
s.write(a)
while True:
	if s.inWaiting()>0: # controlla se sono disponibili i dati nel buffer		
		print("//////  entrato  /////")
      		#lettura dei valori valori inviati dalla parte ATmega

		IMU =s.readline()


           	#stampa dati sul terminale 

		print (IMU)
		#scrittura valori nel file di testo
		
		file_txt.write(IMU)		

file_txt.close()	
s.close()
