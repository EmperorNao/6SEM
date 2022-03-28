
from os import sys
import numpy as np
from datetime import datetime


def get_times(filename):

	times = []
	with open(filename) as file:
		for line in file:
			obj = " ".join(line.split(" ")[0:2])
			times.append( datetime.strptime(obj[:-3] + "000" + obj[-3:], "%Y-%m-%d %H:%M:%S,%f"))

	return times




if __name__ == "__main__":

	if len(sys.argv) < 3:
		print("Don't get filenames")

	sender = sys.argv[1]
	reciever = sys.argv[2]

	sender_time = np.array(get_times(sender))
	reciever_time = np.array(get_times(reciever))

	delay = reciever_time[0] - sender_time[0]
	print(f"Delay is {delay.total_seconds()} s")

	print(reciever_time)
	#reciever_time -= delay
	print(reciever_time)

	diff = [ abs(r_time - s_time).total_seconds() * 1000 for r_time, s_time in zip(reciever_time, sender_time) ]
	print(f"Mean time is {np.mean(diff) - len(diff) * delay.total_seconds() / 1000} ms")