import cv2
import numpy as np
import unicodedata
import copy

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

    def resizeMat2(self, div):
        height, width = self.image.shape[0:2]
        d = [0, 0, width, height]
        if div[0][0] < 0:
            d[0] = div[0][0]
        if div[0][1] > width:
            d[2] = div[0][1]
        if div[1][0] < 0:
            d[1] = div[1][0]
        if div[1][1] > height:
            d[3] = div[1][1]
        T = np.array([[1, 0, -d[0]], [0, 1, -d[1]], [0, 0, 1]])
        print(d)
        self.image = cv2.warpPerspective(self.image, T, (int(-d[0] + d[2]), int(-d[1] + d[3])))
        return d


def resize_image(img):
    img = cv2.resize(img,(int(img.shape[1]*600/3024),int(img.shape[0]*800/4032)))
    for i in img:
        for j in i:
            if not j.all():
                j[0] += 1
    return img

def calcDst4(H, size):
    x = []
    y = []
    x.append(((H[0][0]*0 + H[0][1]*0 + H[0][2])/(H[2][0]*0 + H[2][1]*0 + H[2][2])))
    y.append(((H[1][0]*0 + H[1][1]*0 + H[1][2])/(H[2][0]*0 + H[2][1]*0 + H[2][2])))
    x.append(((H[0][0]*0 + H[0][1]*size[0] + H[0][2])/(H[2][0]*0 + H[2][1]*size[0] + H[2][2])))
    y.append(((H[1][0]*0 + H[1][1]*size[0] + H[1][2])/(H[2][0]*0 + H[2][1]*size[0] + H[2][2])))
    x.append(((H[0][0]*size[1] + H[0][1]*0 + H[0][2])/(H[2][0]*size[1] + H[2][1]*0 + H[2][2])))
    y.append(((H[1][0]*size[1] + H[1][1]*0 + H[1][2])/(H[2][0]*size[1] + H[2][1]*0 + H[2][2])))
    x.append(((H[0][0]*size[1] + H[0][1]*size[0] + H[0][2])/(H[2][0]*size[1] + H[2][1]*size[0] + H[2][2])))
    y.append(((H[1][0]*size[1] + H[1][1]*size[0] + H[1][2])/(H[2][0]*size[1] + H[2][1]*size[0] + H[2][2])))

    min_x = min(x)
    min_y = min(y)
    max_x = max(x)
    max_y = max(y)
    div = ((min_x, max_x),(min_y, max_y))
    print(div)
    return div

def SquareCheck(Mat, i, j):
    flag = 0
    for col in range(-2,3):
        if Mat[i+col,j-2].any() and Mat[i+col,j-1].any() and Mat[i+col,j].any() and Mat[i+col,j+1].any() and Mat[i+col,j+2].any():
            flag+=1
    if flag == 5:
        return True
    else:
        return False

def Write(target, src):
    print('-----------Writing------------')
    mask = np.ndarray((target.shape[0], target.shape[1]), dtype=np.uint8)
    for i in range(src.image.shape[0]):
        for j in range(src.image.shape[1]):
            if i < 3 or i > src.image.shape[0]-3 or j < 3 or j > src.image.shape[1]-3:
                if src.image[i,j].all():
                    if target[i,j].all():
                        target[i,j] = src.image[i,j]/2+target[i,j]/2
                        mask[i,j] = 255
                    else:
                        target[i,j] = src.image[i,j]
            else:
                if SquareCheck(src.image, i, j):
                    if SquareCheck(target, i, j):
                        target[i,j] = src.image[i,j]/2+target[i,j]/2
                        mask[i,j] = 255
                    else:
                        target[i,j] = src.image[i,j]
    print('-----------Wrote------------')
    return target, mask


def temp():
    for i in range(original1.image.shape[1]):
        for j in range(original1.image.shape[0]):
            if original1.image[j,i].all():
                if panorama[j,i].all():
                    panorama[j,i] = original1.image[j,i]/2+panorama[j,i]/2
            else:
                panorama[j,i] = original1.image[j,i]


def make_panorama(original1,original2):
    matcher = cv2.BFMatcher(cv2.NORM_L2,False)
    matches = matcher.knnMatch(original1.des,original2.des,2)
    goodmatches = []
    trainkeys = []
    querykeys = []
    maskArray = []

    for i in matches:
        if i[0].distance < 500:
            if i[0].distance/i[1].distance < 0.8:
                print("\U0001F37A", end=' ')
                goodmatches.append(i[0])
                querykeys.append((original1.kp[i[0].queryIdx].pt[0],original1.kp[i[0].queryIdx].pt[1]))
                trainkeys.append((original2.kp[i[0].trainIdx].pt[0],original2.kp[i[0].trainIdx].pt[1]))
            else:
                pass
                #print("----------------->\U0001F4A9")
        else:
            pass
            #print("----------------->\U0001F4A9")

    print("-----Calculating Homography-----")
    H, status = cv2.findHomography(np.array(trainkeys),np.array(querykeys),cv2.RANSAC, 5.0)
    print('-----finished to calculate-----')
    div = calcDst4(H, original2.image.shape)
    d = original1.resizeMat(div)
    print(original1.image.shape)
    T_xy = [[1,0,-d[0]],[0,1,-d[1]],[0,0,1]]
    panorama = cv2.warpPerspective(original2.image,np.dot(T_xy,H),(original1.image.shape[1],original1.image.shape[0]))
    temp = copy.deepcopy(panorama)
    panorama, mask = Write(panorama,original1)
    output = cv2.seamlessClone(original1.image, temp, mask, (int(temp.shape[1]/2),int(temp.shape[0]/2)), cv2.NORMAL_CLONE)
    cv2.imshow('panorama',panorama)
    cv2.imshow('output',output)
    cv2.imshow('mask',mask)
    cv2.waitKey(0)
    print("--next--")
    return panorama
