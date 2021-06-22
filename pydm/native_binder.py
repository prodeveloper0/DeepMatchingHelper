from ctypes import *
from .native_types import dm_corres, dm_image, dm_params


class Binder:
    def __init__(self, libname):
        self.libdm = CDLL(libname, RTLD_GLOBAL)

        # dm_alloc
        dm_alloc = self.libdm.dm_alloc
        dm_alloc.argtypes = [c_size_t]
        dm_alloc.restype = c_void_p
        self.dm_alloc = dm_alloc

        # dm_free
        dm_free = self.libdm.dm_free
        dm_free.argtypes = [c_void_p]
        self.dm_free = dm_free

        # dm_alloc_params
        dm_alloc_params = self.libdm.dm_alloc_params
        dm_alloc_params.argtypes = [POINTER(dm_params)]
        dm_alloc_params.restype = c_int
        self.dm_alloc_params = dm_alloc_params

        # dm_update_params
        dm_update_params = self.libdm.dm_update_params
        dm_update_params.argtypes = [POINTER(dm_params)]
        dm_update_params.restype = c_int
        self.dm_update_params = dm_update_params

        # dm_free_params
        dm_free_params = self.libdm.dm_free_params
        dm_free_params.argtypes = [POINTER(dm_params)]
        self.dm_free_params = dm_free_params

        # dm_copy_params
        dm_copy_params = self.libdm.dm_copy_params
        dm_copy_params.argtypes = [POINTER(dm_params), POINTER(dm_params)]
        dm_copy_params.restype = c_int
        self.dm_copy_params = dm_copy_params

        # dm_move_params
        dm_move_params = self.libdm.dm_move_params
        dm_move_params.argtypes = [POINTER(dm_params), POINTER(dm_params)]
        dm_move_params.restype = c_int
        self.dm_move_params = dm_move_params

        # dm_compute_unsafe
        dm_compute_unsafe = self.libdm.dm_compute_unsafe
        dm_compute_unsafe.argtypes = [POINTER(dm_params), POINTER(dm_image), POINTER(dm_image),
                                      POINTER(POINTER(dm_corres)), POINTER(c_int)]
        dm_compute_unsafe.restype = c_int
        self.dm_compute_unsafe = dm_compute_unsafe

        # dm_compute
        dm_compute = self.libdm.dm_compute
        dm_compute.argtypes = [POINTER(dm_params), POINTER(dm_image), POINTER(dm_image),
                               POINTER(POINTER(dm_corres)), POINTER(c_int)]
        dm_compute.restype = c_int
        self.dm_compute = dm_compute
