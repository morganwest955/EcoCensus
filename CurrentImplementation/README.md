# EcoCensus
The following is contained in EcoCensus-master

image_partitioning.py was written and run by Rebekah Loving for the initial image analysis of the Miconia images with k-means to aid in the creation of training files necessary for the convolutional neural network we will be building with TensorFlow and then implementing as a standalone. In order to run the image_partitioning.py OpenCV must be installed as well as PIL and numpy. Furthermore, the directories, which this file is working with will need to be modified. Moreover, we do not advise the running of these files as the computational time is quite long for the analysis and segmentation of the files. We provide some of the results in the Miconia_Kmeans and Miconia_Partitioned folders as well as the original images in the Miconia folder.

build_image_data.py and read_tfrecord_data.py are two open source Python files that we will be using in the implementation of our TensorFlow models.

Moreover, the EcoCensus Framework folder contains some of the initial work for the Java and Python EcoCensus framework implemented by Morgan Friend.
