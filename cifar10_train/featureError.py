
"""A simple script for inspect checkpoint files."""
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import sys

import tensorflow as tf

FLAGS = tf.app.flags.FLAGS
tf.app.flags.DEFINE_string("file_name", "model.ckpt-99999", "Checkpoint filename")
tf.app.flags.DEFINE_string("tensor_name", "conv1/weights", "Name of the tensor to inspect")


with tf.Session() as sess:
	saver = tf.train.Saver()
	saver.restore(sess,FLAG.file_name)
	print("restored!!!!!!")


