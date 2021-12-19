import cv2

i = cv2.imread('motion_blur_checker.ppm')
cv2.imwrite('motion_blur_checker.jpg', i)
