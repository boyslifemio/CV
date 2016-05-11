import cv2
import numpy
import sys

from pano_sub import Image,WideImage
from pano_sub import make_panorama

if __name__=="__main__":
    image_names = sys.argv
    if len(image_names) == 1:
        print("usage: python3 panorama.py image1 image2 image3 ..")
        exit()

    if len(image_names) == 2:
        print("usage: ２枚以上画像のパスを渡してください")
        exit()

    images = []
    panorama = []
    for i in image_names[1:]:
        images.append(Image(i))

    #for i in images:
    #   i.show()


    panorama.append(WideImage(images[0].image))

    for i in range(0,len(images)-1):
      panorama.append(WideImage(make_panorama(panorama[i],images[i+1])))

    cv2.imwrite("panorama.png",panorama[-1].image)

    #for i in range(0,len(panorama)):
    #   cv2.imshow("result"+str(i),panorama[i].image)

    print("FIN")
    cv2.waitKey(0)
