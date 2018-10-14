#!/usr/bin/python
#coding:utf-8
from getFeature import *
import tensorflow as tf 
def add_layer(inputs,in_size,out_size,activation_function=None):
    Weights = tf.Variable(tf.random_normal([in_size,out_size]))
    biases  = tf.Variable(tf.zeros([1,out_size])+0.1)
    Wx_plus_b = tf.matmul(inputs,Weights)+biases
    if activation_function is None:
        outputs = Wx_plus_b
    else:
        outputs = activation_function(Wx_plus_b)
    return outputs

if __name__ == '__main__':
    print 'run loadNet.py'
    INPUT_LAYER = 7
    MIN_LAYER = 6
    LAST_LAYER = 5
    OUT_LAYER = 4
    outString = ['capsule','circle','ellipse','triangle']
    IMG_DIR = '/home/dzqiu/qt-workspace/Pill_Final/python/segmentation/'  
    Net_DIR = '/home/dzqiu/qt-workspace/Pill_Final/python/trainNet2/All1W.ckpt'
    (img_list,x_data,y_data)=getAllFeature(IMG_DIR)
    sname = img_list
    count=0
    for img_dir_name in img_list:
        [img_dir,img_name]=os.path.split(img_dir_name)
        [name,extension]=os.path.splitext(img_name)
        sname[count]=name
        count=count+1
    print sname
    print('Get Feature Matrix :'+str(np.shape(x_data)))
    print('Get Labels Matrix :'+str(np.shape(y_data)))
    xs = tf.placeholder(tf.float32,[None,INPUT_LAYER])    
    ys = tf.placeholder(tf.float32,[None,OUT_LAYER])  

    # l1 = add_layer(xs,INPUT_LAYER,MIN_LAYER,activation_function=tf.nn.relu) 
    # predition = add_layer(l1,MIN_LAYER,OUT_LAYER,activation_function=None)
    l1 = add_layer(xs,INPUT_LAYER,MIN_LAYER,activation_function=tf.nn.relu) 
    l2 = add_layer(l1,MIN_LAYER,LAST_LAYER,activation_function=tf.nn.relu)
    predition = add_layer(l2,LAST_LAYER,OUT_LAYER,activation_function=None)


    cross_entropy = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=predition,labels=ys))
    #train_step =tf.train.GradientDescentOptimizer(0.5).minimize(cross_entropy)
    train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)  
    correct_prediction = tf.equal(tf.argmax(predition,1), tf.argmax(ys,1))
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

    # print x_data
    saver = tf.train.Saver()
    with tf.Session() as sess:
        saver.restore(sess,Net_DIR)
        # train_accuracy = sess.run(accuracy,feed_dict={xs:x_data,ys:y_data})
        # error = sess.run(cross_entropy,feed_dict={xs:x_data,ys:y_data})
        result = sess.run(predition,feed_dict={xs:x_data})
        # print result
        result = np.argmax(result,axis=1)
        # print result
        fh=open(str(IMG_DIR)+"result.txt",'w')
        for i in range(np.size(result)):
            
            print("%s is %s"%(img_list[i],outString[result[i]]))
            fh.write("%s\n%d\n"%(sname[i],result[i]))
            # print(x_data[i])
        fh.close()
     

       # print ("training accuracy %g,cross_entropy %g"%(train_accuracy,error))
    
