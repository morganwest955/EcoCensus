import tensorflow as tf
import numpy as np
from PIL import Image
import os,glob,cv2
#import sys,argparse


rootdir ='/Users/bound_to_love/Downloads/Miconia_Partitions'

directory = os.path.dirname('/Users/bound_to_love/Downloads/Predictions_v2/')
directory1 = os.path.dirname('/Users/bound_to_love/Downloads/Predictions_v2/Miconia/')
directory2 = os.path.dirname('/Users/bound_to_love/Downloads/Predictions_v2/Negative/')

if not os.path.exists(directory):
    os.makedirs(directory)
    print "Directory made"

if not os.path.exists(directory1):
    os.makedirs(directory1)
    print "Directory 1 made"

if not os.path.exists(directory2):
    os.makedirs(directory2)
    print "Directory 2 made"

image_size=128
num_channels=3
images = []

## Let us restore the saved model
sess = tf.Session()
# Step-1: Recreate the network graph. At this step only graph is created.
saver = tf.train.import_meta_graph('miconia-model.meta')
# Step-2: Now let's load the weights saved using the restore method.
saver.restore(sess, tf.train.latest_checkpoint('./'))

# Accessing the default graph which we have restored
graph = tf.get_default_graph()

# Now, let's get hold of the op that we can be processed to get the output.
# In the original network y_pred is the tensor that is the prediction of the network
y_pred = graph.get_tensor_by_name("y_pred:0")

## Let's feed the images to the input placeholders
x = graph.get_tensor_by_name("x:0")
y_true = graph.get_tensor_by_name("y_true:0")
y_test_images = np.zeros((1, 2))

files = os.listdir(rootdir)
for file in files[1:]:
    image = cv2.imread(rootdir+"/"+file)
    # Reading the image using OpenCV
    # Resizing the image to our desired size and preprocessing will be done exactly as done during training
    image = cv2.resize(image, (image_size, image_size),0,0, cv2.INTER_LINEAR)
    images.append(image)
    images = np.array(images, dtype=np.uint8)
    images = images.astype('float32')
    images = np.multiply(images, 1.0/255.0)
    #The input to the network is of shape [None image_size image_size num_channels]. Hence we reshape.
    x_batch = images.reshape(1, image_size,image_size,num_channels)

    ### Creating the feed_dict that is required to be fed to calculate y_pred
    feed_dict_testing = {x: x_batch, y_true: y_test_images}
    result=sess.run(y_pred, feed_dict=feed_dict_testing)
    # result is of this format [probabiliy_of_miconia probability_of_other]
    miconia = result.item((0,0))
    other = result.item((0,1))
    img = Image.open(rootdir+"/"+file)
    if miconia > other and miconia > .75:
        img.save(directory1+"/" + str(miconia)+ "_" + file)
    else:
        img.save(directory2+"/" + str(other) + "_" + file)

    images = []