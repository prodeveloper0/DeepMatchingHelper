import cv2
import pydm


def main():
    matcher = pydm.bind(r'./dm_helper.so').create()

    img1 = cv2.imread('climb1.png', cv2.IMREAD_COLOR)
    img2 = cv2.imread('climb2.png', cv2.IMREAD_COLOR)
    result = matcher.compute(img1, img2)
    matched_result_image = pydm.draw_matches(img1, img2, result, thickness=1, point_only=True)

    cv2.namedWindow('Matched', cv2.WINDOW_KEEPRATIO)
    cv2.imshow('Matched', matched_result_image)
    cv2.waitKey()
    cv2.destroyWindow('Matched')


if __name__ == '__main__':
    main()

