from ctypes import *
from .native_types import dm_image, dm_corres


class DeepMatching:
    def __init__(self, binder, params):
        self.binder = binder
        self.params = params

    def __del__(self):
        self.binder.dm_free_params(pointer(self.params))

    def make_dm_image_unsafe(self, cv_img):
        dmi = dm_image()
        h, w = cv_img.shape
        dmi.width = w
        dmi.stride = w
        dmi.height = h
        dmi.data = cv_img.ctypes.data_as(POINTER(c_float))
        return dmi

    @staticmethod
    def convert_image(img):
        import cv2
        if img.ndim == 2:
            return img.astype('float32')
        return cv2.cvtColor(img, cv2.COLOR_BGR2GRAY).astype('float32')

    def compute(self, im1, im2):
        # Convert image to 32FC1 image
        float_im1 = self.convert_image(im1.copy())
        float_im2 = self.convert_image(im2.copy())

        # Convert numpy array to dm_image
        dm_im1 = self.make_dm_image_unsafe(float_im1)
        dm_im2 = self.make_dm_image_unsafe(float_im2)

        # Variables to pass as a pointer
        p_dm_params = pointer(self.params)
        p_dm_im1 = pointer(dm_im1)
        p_dm_im2 = pointer(dm_im2)

        # Variables to store results
        corres = pointer(dm_corres())
        count = c_int()

        # Compute matches
        if self.binder.dm_compute_unsafe(p_dm_params, p_dm_im1, p_dm_im2, pointer(corres), pointer(count)) == 0:
            return None

        # Convert result to Python tuple
        def __corres_converter(corr):
            x0, y0 = int(corr.x0), int(corr.y0)
            x1, y1 = int(corr.x1), int(corr.y1)
            score = corr.score
            return ((x0, y0), (x1, y1)), score
        matches = [__corres_converter(corres[i]) for i in range(count.value)]

        # Free result variable allocated from dm_compute_unsafe
        self.binder.dm_free(corres)
        return matches
