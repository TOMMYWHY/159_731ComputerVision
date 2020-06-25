
# Assignment 3:
## Recognising gestures from images, 159.731, S1 2020
   
###   Family Name: JIANG , Given Name: TAO , Student ID:20004769
 
```console
   ./gesture train                  : extract trainset images features save to file and build a ANN model
   ./gesture test                   : extract testset images features save to file and test the ANN model
   ./gesture                        : use web-camera to  predict
   ./gesture ./images/test/0_A.jpg  : processes a single static image
```
 
 >  the model save as ./res/gesture.xml
 
 > the trainset features save as ./res/gesture_train.data
 
 >  the testset features save as ./res/gesture_test.data
 
 
### ANN model:
  > 3 hidden fully connected layers with 100 Neurons
  
  > epochs:500
  
  > learning rate:0.0001
  
