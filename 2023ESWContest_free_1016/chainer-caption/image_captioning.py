# The original source code is created by Satoshi Tsutsui and downloadable at the GitHub below.
# https://github.com/apple2373/chainer-caption
#
# Based on the code, I modified and added some code to capture an image from the camera module.

import cv2
import RPi.GPIO as GPIO
import serial
import time
import sys, os, subprocess, json
import json
import chainer
import argparse
import numpy as np
import math

from chainer import cuda
import chainer.functions as F
from chainer import cuda, Function, FunctionSet, gradient_check, Variable, optimizers
from chainer import serializers

sys.path.append('./code')
from CaptionGenerator import CaptionGenerator



PIN = 20

GPIO.setmode(GPIO.BCM)
GPIO.setup(PIN,GPIO.IN)

cap = cv2.VideoCapture(0)


ser = serial.Serial('/dev/ttyACM0', 9600, timeout = 1)
ser.reset_input_buffer()



devnull = open('os.devnull', 'w')

#Parse arguments
parser = argparse.ArgumentParser()
parser.add_argument("-g", "--gpu",default=-1, type=int, help=u"GPU ID.CPU is -1")
parser.add_argument('--beam',default=3, type=int,help='beam size in beam search')
parser.add_argument('--depth',default=50, type=int,help='depth limit in beam search')
parser.add_argument('--lang',default="<sos>", type=str,help='special word to indicate the langauge or just <sos>')
args = parser.parse_args()

caption_generator=CaptionGenerator(
    rnn_model_place='./data/caption_en_model40.model',
    cnn_model_place='./data/ResNet50.model',
    dictonary_place='./data/MSCOCO/mscoco_caption_train2014_processed_dic.json',
    beamsize=args.beam,
    depth_limit=args.depth,
    gpu_id=args.gpu,
    first_word= args.lang,
    )


if __name__ == '__main__':

    while True:


        if GPIO.input(PIN) == 1 :
            ret, frame = cap.read()
            cv2.imwrite('image.jpg', frame)
            captions = caption_generator.generate('image.jpg')
            word = " ".join(captions[0]["sentence"][1:-1])
            word = word + " "
            
            ser.write(word.encode('ascii'))
            print(word)
            
   
        time.sleep(0.05)
