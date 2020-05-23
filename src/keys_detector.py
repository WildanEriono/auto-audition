class KeysDetector:
    LEFT = 0
    UP = 1
    RIGHT = 2
    DOWN = 3

    def __init__(self):
        pass

    def detect(self, gray) -> list:
        thres_img = self.threshold_gray(gray)
        cnts = self.find_contours(thres_img)
        (cnts, boundingBoxes) = self.sort_contours(cnts)

        keys = []
        for i in range(len(boundingBoxes)):
            key_roi = self.get_key_roi(thres_img, boundingBoxes[i])
            direction = self.get_direction(key_roi)
            keys.append(direction)

        return keys

    def threshold_gray(self, gray):
        _, thres = cv2.threshold(
            gray, AuditionCtrl.KEYS_THRESHOLD, 255, cv2.THRESH_BINARY
        )
        return thres

    def find_contours(self, thres_img):
        cnts = cv2.findContours(
            thres.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
        )
        cnts = imutils.grab_contours(cnts)
        return cnts

    def sort_contours(self, cnts):
        boundingBoxes = [cv2.boundingRect(c) for c in cnts]
        (cnts, boundingBoxes) = zip(
            *sorted(zip(cnts, boundingBoxes), key=lambda b: b[1][0])
        )

        return (cnts, boundingBoxes)

    def get_key_roi(self, thres_img, boxes):
        res = thres_img[
            boxes[1] : (boxes[1] + boxes[3]), boxes[0] : (boxes[0] + boxes[2])
        ]
        return res

    def get_direction(self, roi):
        h, w = roi.shape
        reg0 = roi[:, 0 : w // 3]
        reg1 = roi[2 * h // 3 : h, :]
        reg2 = roi[:, 2 * w // 3 : w]
        reg3 = roi[0 : h // 3, :]

        rate0 = cv2.countNonZero(reg0)
        rate1 = cv2.countNonZero(reg1)
        rate2 = cv2.countNonZero(reg2)
        rate3 = cv2.countNonZero(reg3)

        arr = np.array((rate0, rate1, rate2, rate3))
        direction = np.argmax(arr)
        return direction