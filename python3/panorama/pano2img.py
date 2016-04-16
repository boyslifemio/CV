import cv2,numpy,sys

from pano_sub import Image

if __name__=="__main__":
    image_names = sys.argv
    if len(image_names)==1 or len(image_names)==2:
        print("usage: python3 pano2img.py image1 image2")
        exit()

    images = []
    for i in image_names[1:]:
        images.append(Image(i))

    for i in images:
        i.show()

    matcher = cv2.BFMatcher(cv2.NORM_L2,False)
    matches = matcher.knnMatch(images[0].des,images[1].des,2)
    goodmatches = []
    trainkeys = []
    querykeys = []

    for i in matches:
        print(i[0].distance)
        if i[0].distance < 500:
            if i[0].distance/i[1].distance < 0.9:
                print("good")
                goodmatches.append(i[0])
                querykeys.append([images[0].kp[i[0].queryIdx].pt[0],images[0].kp[i[0].queryIdx].pt[1]])
                trainkeys.append([images[1].kp[i[0].trainIdx].pt[0],images[1].kp[i[0].trainIdx].pt[1]])

    print(trainkeys)

    H, status = cv2.findHomography(numpy.array(trainkeys),numpy.array(querykeys),cv2.RANSAC)

    dx=0
    dy=0
    count=0
    for i,v in enumerate(status):
        if v==1:
            dx += querykeys[i][0] - trainkeys[i][0]
            dy += querykeys[i][1] - trainkeys[i][1]
            count += 1

    height,width = images[0].image.shape[:2]
    dx = abs(int(round(dx/count)))
    dy = abs(int(round(dy/count)))

    print("-----Homography-----")
    panorama = cv2.warpPerspective(images[1].image,H,(int(width + dx),int(height + dy)))

    #panorama = panorama+images[0].image

     for i in range(width):
         for j in range(height):
             panorama[j,i] = images[0].image[j,i]

    cv2.imshow("panorama",panorama)
    cv2.waitKey(0)
