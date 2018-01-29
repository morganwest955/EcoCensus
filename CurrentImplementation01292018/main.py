import models
import numpy as np
import tensorflow as tf

import dataset
import time
from datetime import timedelta
import math
import random
import numpy as np

# Adding Seed so that random initialization is consistent
from numpy.random import seed

seed(1)
from tensorflow import set_random_seed

set_random_seed(2)

def load_data():
    # Prepare input data
    classes = ['miconia', 'negative']
    num_classes = len(classes)

    # 20% of the data will automatically be used for validation
    validation_size = 0.2
    img_size = 128
    train_path = '/Users/bound_to_love/Downloads/training_data'

    # We shall load all the training and validation images and labels into memory using openCV and use that during training
    data = dataset.read_train_sets(train_path, img_size, classes, validation_size=validation_size)

    return (data.train.images, data.train.labels, data.valid.images, data.valid.labels)


flags = tf.app.flags
FLAGS = flags.FLAGS
flags.DEFINE_float('learning_rate', 0.01, 'Learning rate')
flags.DEFINE_integer('batch_size', 25, 'Batch size')

X_train, Y_train, X_test, Y_test = load_data()

batch_size = 128

X = tf.placeholder("float", [batch_size, 32, 32, 3])
Y = tf.placeholder("float", [batch_size, 2])
learning_rate = tf.placeholder("float", [])

# ResNet Models
#net = models.resnet(X, 20)
# net = models.resnet(X, 32)
# net = models.resnet(X, 44)
net = models.resnet(X, 56)

cross_entropy = -tf.reduce_sum(Y * tf.log(net))
opt = tf.train.MomentumOptimizer(learning_rate, 0.9)
train_op = opt.minimize(cross_entropy)

sess = tf.Session()
sess.run(tf.initialize_all_variables())

correct_prediction = tf.equal(tf.argmax(net, 1), tf.argmax(Y, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))

saver = tf.train.Saver()
checkpoint = tf.train.latest_checkpoint(".")
if checkpoint:
    print "Restoring from checkpoint", checkpoint
    saver.restore(sess, checkpoint)
else:
    print "Couldn't find checkpoint to restore from. Starting over."

for j in range(2):
    for i in range(0, len(data.train.labels), batch_size):
        feed_dict = {
            X: X_train[i:i + batch_size],
            Y: Y_train[i:i + batch_size],
            learning_rate: 0.001}
        sess.run([train_op], feed_dict=feed_dict)
        if i % 512 == 0:
            print "training on image #%d" % i
            saver.save(sess, 'progress', global_step=i)

for i in range(0, len(data.valid.labels), batch_size):
    if i + batch_size < len(data.valid.labels):
        acc = sess.run([accuracy], feed_dict={
            X: X_test[i:i + batch_size],
            Y: Y_test[i:i + batch_size]
        })
        accuracy_summary = tf.scalar_summary("accuracy", accuracy)
        print acc

sess.close()