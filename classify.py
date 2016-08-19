from PIL import Image
import numpy as np
import sys
import cifar10_2eval as cf
from matplotlib import pyplot as plt


filename=sys.argv[1]
im = Image.open("./stickers/"+filename)
print 'image mode:',im.mode,' ,image size:',im.size
if im.mode!='RGB' :
	im=im.convert('RGB')
if im.size!=(32,32):
	im = im.resize((32,32),Image.BILINEAR)
print 'CONVERTED ','image mode:',im.mode,' ,image size:',im.size


im = (np.array(im))
print im
plt.figure()
plt.imshow(im)
plt.show()


r = im[:,:,0].flatten()
g = im[:,:,1].flatten()
b = im[:,:,2].flatten()
label = [1]

outname=filename[0:-4]+'.bin'
out = np.array(list(label) + list(r) + list(g) + list(b),np.uint8)
out.tofile("./test_inputdata/"+outname)

cf.evaluate(outname)
