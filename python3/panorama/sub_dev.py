import cv2
import numpy as np

class Image:
    def __init__(self,name,img):
        def calculate(self):
            detector = cv2.AKAZE_create()
            keypoints, descriptors = detector.detectAndCompute(self.image,None)
            return keypoints, descriptors

        self.name = name
        self.image = img
        self.kp,self.des = calculate(self)

    def show(self):
        cv2.imshow(self.name, self.image)
        cv2.waitKey(0)

    def resizeMat(self, div):
        height,width = (self.image).shape[0:2]
        d = [0,0]
        if div[0][0] < 0:
            d[0]=div[0][0]
            for i in range(-1 * int(div[0][0])):
                self.image = np.insert(self.image,0,[0,0,0],axis = 1)
        if div[0][1] > width:
            for i in range(int(div[0][1]) - width + 3):
                self.image = np.insert(self.image,-1,[0,0,0],axis = 1)
        if div[1][0] < 0:
            d[1]=div[1][0]
            for i in range(-1 * int(div[1][0])):
                self.image = np.insert(self.image, 0, np.array((0, 0, 0)), 0)
        if div[1][1] > height:
            for i in range(int(div[1][1]) - height + 3):
                self.image = np.insert(self.image, -1, np.array((0, 0, 0)), 0)
        print(d)
        return d


def resize_image(img):
    img = cv2.resize(img,(int(img.shape[1]*600/3024),int(img.shape[0]*800/4032)))
    return img

def calcDst4(H, size, targetSize):
    x = []
    y = []
    x.append(((H[0][0]*0 + H[0][1]*0 + H[0][2])/(H[2][0]*0 + H[2][1]*0 + H[2][2])))
    y.append(((H[1][0]*0 + H[1][1]*0 + H[1][2])/(H[2][0]*0 + H[2][1]*0 + H[2][2])))
    x.append(((H[0][0]*0 + H[0][1]*size[1] + H[0][2])/(H[2][0]*0 + H[2][1]*size[1] + H[2][2])))
    y.append(((H[1][0]*0 + H[1][1]*size[1] + H[1][2])/(H[2][0]*0 + H[2][1]*size[1] + H[2][2])))
    x.append(((H[0][0]*size[0] + H[0][1]*0 + H[0][2])/(H[2][0]*size[0] + H[2][1]*0 + H[2][2])))
    y.append(((H[1][0]*size[0] + H[1][1]*0 + H[1][2])/(H[2][0]*size[0] + H[2][1]*0 + H[2][2])))
    x.append(((H[0][0]*size[0] + H[0][1]*size[1] + H[0][2])/(H[2][0]*size[0] + H[2][1]*size[1] + H[2][2])))
    y.append(((H[1][0]*size[0] + H[1][1]*size[1] + H[1][2])/(H[2][0]*size[0] + H[2][1]*size[1] + H[2][2])))

    min_x = min(x)
    min_y = min(y)
    max_x = max(x)
    max_y = max(y)
    div = ((min_x, max_x),(min_y, max_y))
    print(div)
    return div

def make_panorama(original1,original2):
    matcher = cv2.BFMatcher(cv2.NORM_L2,False)
    matches = matcher.knnMatch(original1.des,original2.des,2)
    goodmatches = []
    trainkeys = []
    querykeys = []
    maskArray = []

    for i in matches:
        print(i[0].distance,end='')
        if i[0].distance < 900:
            if i[0].distance/i[1].distance < 0.8:
                print('-->good match------------------>')
                goodmatches.append(i[0])
                querykeys.append((original1.kp[i[0].queryIdx].pt[0],original1.kp[i[0].queryIdx].pt[1]))
                trainkeys.append((original2.kp[i[0].trainIdx].pt[0],original2.kp[i[0].trainIdx].pt[1]))
            else:
                print('-->not good match')
        else:
            print('-->not good match')

    print("-----Calculating Homography-----")
    H, status = cv2.findHomography(np.array(trainkeys),np.array(querykeys),cv2.RANSAC)
    print('-----finished to calculate-----')
    div = calcDst4(H,original2.image.shape,original1.image.shape)
    d = original1.resizeMat(div)
    print(original1.image.shape)
    T_xy = [[1,0,-d[0]],[0,1,-d[1]],[0,0,1]]
    panorama = cv2.warpPerspective(original2.image,np.dot(T_xy,H),(original1.image.shape[1],original1.image.shape[0]))

    for i in range(original1.image.shape[1]):
      for j in range(original1.image.shape[0]):
        if original1.image[j,i].all():
          if panorama[j,i].all():
            panorama[j,i] = original1.image[j,i]/2+panorama[j,i]/2
          else:
            panorama[j,i] = original1.image[j,i]
    #cv2.imwrite("mask",mask)
    print("--next--")
    return panorama
