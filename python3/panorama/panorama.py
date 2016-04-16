import cv2
import numpy
import sys

from pano_sub import Image

if __name__=="__main__":
    image_names = sys.argv
    if len(image_names) == 1:
        print("usage: python3 panorama.py image1 image2 image3 ..")
        exit()

    if len(image_names) == 2:
        print("usage: ２枚以上画像のパスを渡してください")
        exit()

    images = []
    for i in image_names[1:]:
        images.append(Image(i))

    for i in images:
        print(i.des)
        i.show()

    goodmatches = []
    tempmatches = []
    matcher = cv2.BFMatcher(cv2.NORM_L2,False)

    for i in range(0,len(images)-1):
        goodmatches.append([])
        for j in range(i+1,len(images)):
            matches = matcher.knnMatch(images[i].des,images[j].des,k = 2)
            tempmatches = []
            for k in matches:
                if(k[0].distance < 500):
                    print(k[0].distance)
                    if(k[0].distance/k[1].distance<0.8):
                        tempmatches.append(k[0])
            goodmatches[i].append(tempmatches)

    trainkeys = []
    querykeys = []
    tempmatches = []
    sortedgoodmatches = []
    l = 0

    # for i in goodmatches:
    #     sortedgoodmatches.append([])
    #     for j in i:
    #         better = sorted(j, key=lambda x: x.distance)
    #         #print("distance")
    #         for k in better:
    #             print(k.distance)
    #         tempmatches.append(better)
    #         sortedgoodmatches[l].append(tempmatches)
    #     l = l + 1


    temp1 = []
    temp2 = []

    for i in range(0, len(images) - 1):
        querykeys.append([])
        trainkeys.append([])
        l = i + 1
        for j in goodmatches[i]:
            temp1 = []
            temp2 = []
            for k in j:
                print("-------------")
                print(k.queryIdx)
                print(len(images[i].kp))
                temp1.append([images[i].kp[k.queryIdx].pt[0],images[i].kp[k.queryIdx].pt[1]])
                temp2.append([images[l].kp[k.trainIdx].pt[0],images[l].kp[k.trainIdx].pt[1]])
            querykeys[i].append(temp1)
            trainkeys[i].append(temp2)
            l = l + 1

    for i in trainkeys:
        for j in i:
            print("---keys---")
            print(len(j))

    for i in goodmatches:
        for j in i:
            print("---matches---")
            print(len(j))

    H = []
    for i in range(0, len(images)-1):
        H.append([])
        for j in range(0, len(images)-1-i):
            print(i,j)
            h_matrix, mask = cv2.findHomography(numpy.array(trainkeys[i][j]),numpy.array(querykeys[i][j]),cv2.RANSAC)
            H[i].append(h_matrix)


    print("--------Homography--------")
    print(H[0][1])
    print(H[1][0]*H[0][0])

    panorama = cv2.warpPerspective(images[1].image,H[0][0],(int(images[0].image.shape[1]*1),int(images[0].image.shape[0]*1)))
    #panorama = cv2.warpPerspective(images[2].image,rot,(int(images[0].image.shape[1]*1),int(images[0].image.shape[0]*1)))

    cv2.imwrite("panorama.png",panorama)
    cv2.imshow("result",panorama)

    print("FIN")
    cv2.waitKey(0)
