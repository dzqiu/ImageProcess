#!/usr/bin/python
#coding:utf-8
from getFeature import *
import tensorflow as tf 
import matplotlib.pyplot as plt 
from mpl_toolkits.mplot3d import Axes3D

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
    print 'run trainNN.py'

    INPUT_LAYER = 7
    MIN_LAYER = 6
    LAST_LAYER = 5
    OUT_LAYER = 4

    IMG_DIR 		= '/home/dzqiu/qt-workspace/Pill_Final/python/data/contours/incomplete/'
    Validation_IMG 	= '/home/dzqiu/qt-workspace/Pill_Final/python/validation/contours/incomplete/'
    Net_DIR 		= '/home/dzqiu/qt-workspace/Pill_Final/python/TrainNet2/All1W.ckpt'
    #IMG_DIR = '/home/dzqiu/Documents/image/video/samples/mysample/contours/incomplete/'
    (img_list,x_data,y_data)=getAllFeature(IMG_DIR)
    ( _,vx,vy)=getAllFeature(Validation_IMG)

    print 'Done'
    print('Get Feature Matrix :'+str(np.shape(x_data)))
    print('Get Labels Matrix :'+str(np.shape(y_data)))
    print('Get Validation Matrix :'+str(np.shape(vx)))
    print('Get Validation Matrix :'+str(np.shape(vy)))

    xs = tf.placeholder(tf.float32,[None,INPUT_LAYER])    
    ys = tf.placeholder(tf.float32,[None,OUT_LAYER])    
    # l1 = add_layer(xs,INPUT_LAYER,MIN_LAYER,activation_function=tf.nn.relu) 
    # predition = add_layer(l1,MIN_LAYER,OUT_LAYER,activation_function=None)
    l1 = add_layer(xs,INPUT_LAYER,MIN_LAYER,activation_function=tf.nn.relu) 
    l2 = add_layer(l1,MIN_LAYER,LAST_LAYER,activation_function=tf.nn.relu)
    l3 = add_layer(l2,LAST_LAYER,3,activation_function=tf.nn.relu)
    predition = add_layer(l3,3,OUT_LAYER,activation_function=None)

    cross_entropy = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=predition,labels=ys))
    #train_step =tf.train.GradientDescentOptimizer(0.5).minimize(cross_entropy)
    train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)  

    correct_prediction = tf.equal(tf.argmax(predition,1), tf.argmax(ys,1))
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

    fig_x = np.linspace(0,30000,301)
    fig_error = np.linspace(0,30000,301)
    fig_accuracy = np.linspace(0,30000,301)
    
    ax = plt.subplot(111)  
    saver = tf.train.Saver()
    with tf.Session() as sess:
        init = tf.initialize_all_variables()
        sess.run(init)
        for i in range(15001):
            sess.run(train_step,feed_dict={xs:x_data,ys:y_data})
            if i % 50  == 0:
                train_accuracy = 1-sess.run(accuracy,feed_dict={xs:x_data,ys:y_data})
                error = sess.run(cross_entropy,feed_dict={xs:x_data,ys:y_data})
                print ("step %d, training accuracy %g,cross_entropy %g"%(i, train_accuracy,error))
                fig_error[i/50] = error
                fig_accuracy[i/50] = train_accuracy 
        error  = 1-sess.run(accuracy,feed_dict={xs:vx,ys:vy})
        save_path = saver.save(sess,Net_DIR)
        # l2_out= sess.run(l2,feed_dict={xs:vx,ys:vy})
        # fig = plt.figure()
        # ax = fig.add_subplot(111, projection='3d')
        # for tax in l2_out:
        #     ax.scatter(tax[0], tax[1], tax[2], c='r')
        print ("Validation error:%g"%(error))
        print ("Save to path:",save_path)
    plt.plot(fig_x,fig_error,color='red',label=r'$TrainDataSetError$')  
    plt.plot(fig_x,fig_accuracy,color='blue',label=r'$AccuarcyError$')  
    plt.legend(loc='upper left',frameon=False)  
    plt.show()
    
