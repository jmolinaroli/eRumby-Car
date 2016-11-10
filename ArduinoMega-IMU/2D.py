"""
ldr.py
Display analog data from Arduino using Python (matplotlib)
Author: Mahesh Venkitachalam
Website: electronut.in
"""

import sys, serial, argparse
import numpy as np
from time import sleep
from collections import deque

import matplotlib.pyplot as plt 
import matplotlib.animation as animation

    
# plot class
class AnalogPlot:
  # constr
  def __init__(self, strPort, maxLen):
      # open serial port
      self.ser = serial.Serial(strPort, 9600)

      self.ax = deque([0.0]*maxLen)
      self.ay = deque([0.0]*maxLen)
      self.az = deque([0.0]*maxLen)
      self.maxLen = maxLen

  # add to buffer
  def addToBuf(self, buf, val):
      if len(buf) < self.maxLen:
          buf.append(val)
      else:
          buf.pop()
          buf.appendleft(val)

  # add data
  def add(self, data):
      assert(len(data) == 3)
      self.addToBuf(self.ax, data[0])
      self.addToBuf(self.ay, data[1])
      self.addToBuf(self.az, data[2])

  # update plot
  def update(self, frameNum, a0, a1, a2):
      try:
          line = self.ser.readline()
          data = [float(val) for val in line.split()]
          # print data
          if(len(data) == 3):
              self.add(data)
              a0.set_data(range(self.maxLen), self.ax)
              a1.set_data(range(self.maxLen), self.ay)
              a2.set_data(range(self.maxLen), self.az)
      except KeyboardInterrupt:
          print('exiting')
      
      return a0, 

  # clean up
  def close(self):
      # close serial
      self.ser.flush()
      self.ser.close()    

# main() function
def main():


  # create parser
  ##parser = argparse.ArgumentParser(description="LDR serial")
  # add expected arguments
  ##parser.add_argument('--port', dest='port', required=True)

  # parse args
  ##args = parser.parse_args()
  
  strPort = '/dev/ttyACM0'
  #strPort = args.port

  print('reading from serial port %s...' % strPort)

  # plot parameters
  analogPlot = AnalogPlot(strPort, 100)

  print('plotting data...')

  # set up animation
  fig = plt.figure()

  
  ax = plt.axes(xlim=(0, 100), ylim=(-200, 200))
  
  a0, = ax.plot([], [])
  a1, = ax.plot([], [])
  a2, = ax.plot([], [])
  anim = animation.FuncAnimation(fig, analogPlot.update, 
                                 fargs=(a0, a1, a2), 
                                 interval=50)
  plt.xlabel('Time')
  plt.ylabel('Roll-Pitch-Yaw')
  plt.title('IMU signals')
  plt.text(30, 180, r'$Blue-Roll, Green-Pitch, Red-Yaw$')
  plt.grid(True)
  # show plot
  plt.show()
  
  # clean up
  analogPlot.close()

  print('exiting.')
  

# call main
if __name__ == '__main__':
  main()
