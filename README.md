# EcoCensus
The following is contained in EcoCensus-master

In order to use the current implementation of EcoCensus, the installation of TensorFlow is required. The CurrentImplementation07122017 is a folder containing the current python code for creating a convolutional neural network for the classification of invasive species, currently focusing on Miconia only. It contains five files, dataset.py, image_partition.py, predict.py, predictions.py, and train.py. All paths must be respecified by the user to work with whatever directories and training data that the user desires. The file dataset.py loads in the images and prepares them for training the convolutional neural network. The file image_partition.py partitions images into a hundred equal partitions and resizes the partitions. The file predict.py can be called, after the model is trained with train.py and the dataset that was loaded in, with the command line argument of a .JPG file. The predictions.py file specifies a folder of images to classify and creates folders with the classified images. The train.py file trains the convolutional neural network using the training dataset it is provided with a hold out set and validation, tracking the loss. 

The following are instructions for running the current implementation of EcoCensus:

  1. Update the paths in all of the files to point to the correct datasets and directories for placing models and predictions. 
  2. Run python train.py to train the convolutional neural network. 
  3. Run python predict.py image.JPG or python predictions.py to classify images.
  
The files coordFinder.py and imageReader.py were implemented by Morgan West. While currently non-functional, these files contain the bulk of what will be necessary to convert the classified partitions of images into lists of coordinates and labeled maps.

image_partitioning.py was written and run by Rebekah Loving for the initial image analysis of the Miconia images with k-means to aid in the creation of training files necessary for the convolutional neural network we will be building with TensorFlow and then implementing as a standalone. In order to run the image_partitioning.py OpenCV must be installed as well as PIL and numpy. Furthermore, the directories, which this file is working with will need to be modified. Moreover, we do not advise the running of these files as the computational time is quite long for the analysis and segmentation of the files. We provide some of the results in the Miconia_Kmeans and Miconia_Partitioned folders as well as the original images in the Miconia folder.

build_image_data.py and read_tfrecord_data.py are two open source Python files that we will be using in the implementation of our TensorFlow models.

Moreover, the EcoCensus Framework folder contains some of the initial work for the Java and Python EcoCensus framework implemented by Morgan Friend.
