import cv2, numpy

class Image:
    def __init__(self,name):
        def make_cambus(img):
            cambus=numpy.zeros((img.shape[0]*2,img.shape[1]*2,img.shape[2]),numpy.uint8)
            for i in range(img.shape[1]):     
                for j in range(img.shape[0]):
                  cambus[j+int(img.shape[0]/2*800/4032),i+int(img.shape[1]/2*600/3024),:] = img[j,i,:]
            return cambus

        def resize_image(name):
            image = cv2.imread(name, cv2.IMREAD_COLOR)
            image = cv2.resize(image,(int(image.shape[1]*600/3024),int(image.shape[0]*800/4032)))
            return image 

        def calculate(self):
            detector = cv2.AKAZE_create()
            keypoints, descriptors = detector.detectAndCompute(self.image,None)
            return keypoints, descriptors 

        self.name = name
        self.image = make_cambus(resize_image(self.name))
        self.kp,self.des = calculate(self)
    
    def show(self):
        cv2.imshow(self.name, self.image)

class WideImage():
    def __init__(self,img): 
        def calculate(self):
            detector = cv2.AKAZE_create()
            keypoints, descriptors = detector.detectAndCompute(self.image,None)
            return keypoints, descriptors 
        
        self.name = 'panorama-temp'
        self.image = img
        self.kp,self.des = calculate(self)

    def show(self):
        cv2.imshow(self.name, self.image)

def make_panorama(original1,original2): 
    matcher = cv2.BFMatcher(cv2.NORM_L2,False)
    matches = matcher.knnMatch(original1.des,original2.des,2)
    goodmatches = []
    trainkeys = []
    querykeys = []

    for i in matches:
        print(i[0].distance)
        if i[0].distance < 500:
            if i[0].distance/i[1].distance < 0.9:
                print("good")
                goodmatches.append(i[0])
                querykeys.append((original1.kp[i[0].queryIdx].pt[0],original1.kp[i[0].queryIdx].pt[1]))
                trainkeys.append((original2.kp[i[0].trainIdx].pt[0],original2.kp[i[0].trainIdx].pt[1]))

    H, status = cv2.findHomography(numpy.array(trainkeys),numpy.array(querykeys),cv2.RANSAC)

    dx=0
    dy=0
    count=0
    for i,v in enumerate(status):
        if v==1:
            dx += querykeys[i][0] - trainkeys[i][0]
            dy += querykeys[i][1] - trainkeys[i][1]
            count += 1

    height,width = original1.image.shape[:2]
    dx = abs(int(round(dx/count)))
    dy = abs(int(round(dy/count)))

    print("-----Calculating Homography-----")
    panorama = cv2.warpPerspective(original2.image,H,(int(width+dx),int(height+dy)))

    for i in range(int(width/2)):
      for j in range(int(height/2)):
        if original1.image[j+int(height/4),int(i+width/4)].any():
          panorama[j+int(height/4),int(i+width/4)] = original1.image[j+int(height/4),int(i+width/4)]

    return panorama

