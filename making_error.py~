from PIL import Image
import numpy as np
import random
import sys
import struct
from matplotlib import pyplot as plt
label=[]
dataset=[]	
err = []
back = []
def show(im):
	test = im
	test=np.array(test,np.uint8)
	test = test.reshape(3,1024)
	test = test.T
	test = test.reshape(32,32,3)

	plt.figure()
	plt.imshow(test)
	plt.show()

def addError(ori):
#	index of inside area
	inside=[]
	err=ori[:]
	for j in range(16):	
		inside = inside+range(264+j*32,264+j*32+16)+range(264+1024+j*32,264+1024+j*32+16)+range(264+2048+j*32,264+2048+j*32+16)
#	index of outside area
	outside = range(0,3072,1)
	outside = list(set(outside).difference(set(inside)))

#	random error
	err_area = outside
	ran_bit=random.sample(xrange(len(err_area)*8),len(err_area)*8/1000);
	
	for i in ran_bit:
		err[err_area[i/8]] = err[err_area[i/8]] ^ (2**(i%8))
	return err

def main():
#	get all images in format of [r*1024,g*1024,b*1024]
#	store them in dataset[0~9999]
	f=open("test_batch.bin","rb")
	data = f.read()
	for i in range(len(data)/3073):
		label.append(struct.unpack("B",data[i*3073])[0])
		datatmp = data[i*3073+1:i*3073+3073]
		dataset.append(list(struct.unpack("B"*(len(datatmp)),datatmp[:])))
	f.close()	
#	add error
	for k in range(len(dataset)):
		err.append(addError(dataset[k]))

#	show image 	
	show(err[int(sys.argv[1])])
	
	
#	save back
	out = open("testout.bin","wb")
	for j in range(len(dataset)):
		back=[label[j]]+ list(err[j])
		back = np.array(back,np.uint8)
		out.write(back)
	out.close()

if __name__ == '__main__':
	main()
