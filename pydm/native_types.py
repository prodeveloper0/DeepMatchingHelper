from ctypes import Structure, c_int, c_float, c_void_p, POINTER


class dm_corres(Structure):
    _fields_ = [('x0', c_float),
                ('y0', c_float),
                ('x1', c_float),
                ('y1', c_float),
                ('maxima', c_float),
                ('score', c_float)]


class dm_image(Structure):
    _fields_ = [('width', c_int),
                ('height', c_int),
                ('stride', c_int),
                ('data', POINTER(c_float))]


class dm_params(Structure):
    _fields_ = [('inner_ptr', c_void_p),
                ('downscale_factor', c_int),
                ('neighbor_radius', c_int),
                ('max_patch_size', c_int),
                ('verbose', c_int),
                ('nthread', c_int)]
