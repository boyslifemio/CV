import cv2, numpy

class Image:
    def __init__(self,name):
        def resize_image(name):
            image = cv2.imread(name, cv2.IMREAD_COLOR)
            image = cv2.resize(image,(int(image.shape[1]/6),int(image.shape[0]/6)))
            return image 

        def calculate(self):
            detector = cv2.AKAZE_create()
            keypoints, descriptors = detector.detectAndCompute(self.image,None)
            return keypoints, descriptors 


        self.name = name
        self.image = resize_image(self.name)
        self.kp,self.des = calculate(self)
        
    
    def show(self):
        cv2.imshow(self.name, self.image)
