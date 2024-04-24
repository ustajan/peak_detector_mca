#!/usr/bin/env python3
# Authors:  Areg Danagoulian
# email: aregjan@mit.edu
# License: Attribution 4.0 International (CC BY 4.0)
# The code is very simple.  It takes what the serial connection gives it, and spews it either onto the screen or into the file (if specified). Simple.

import serial
import time
import argparse
import sys
import numpy

parser = argparse.ArgumentParser()
parser.add_argument('port', help='specify the serial port (e.g. /dev/ttyUSB0 on linux, COM14 on windows)')
parser.add_argument('-b', '--baudrate', type=int, default=115200,
					help='set the baudrate of the serial port')
parser.add_argument('-o', '--output', help='specify the output file. stdout will be used if none is given.')
parser.add_argument('-p', '--data-points',
					help='quit after the specified amount of detections. use 0 or a negative number to run indefinitely.', type=int)
args = parser.parse_args()

outfile = None

if args.output:
	outfile = open(args.output, "w")
else:
	outfile = sys.stdout

count = 0

if args.rate:
	integration_count=int(args.rate)
	times=numpy.zeros(integration_count)

beg = time.time()
time_elapsed=0
last_time=0
with serial.Serial(args.port, args.baudrate, timeout=10) as ser:
	while True:
		s = ser.readline().decode() #get the actual time

		outfile.write(s)
		outfile.flush()

		count += 1

		if count == args.data_points:
			quit()
