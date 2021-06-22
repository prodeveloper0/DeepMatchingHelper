from ctypes import pointer
from .native_types import dm_params
from .deepmatching import DeepMatching


class DeepMatchingBuilder:
    def __init__(self, binder):
        self.binder = binder
        self.downscale_factor = 1
        self.neighbor_radius = 0
        self.max_patch_size = 999
        self.verbose = False
        self.nthread = 1

    def create(self):
        params = dm_params()
        self.binder.dm_alloc_params(pointer(params))

        params.downscale_factor = self.downscale_factor
        params.neighbor_radius = self.neighbor_radius
        params.max_patch_size = self.max_patch_size
        params.verbose = 1 if self.verbose else 0
        params.nthread = self.nthread
        self.binder.dm_update_params(pointer(params))

        return DeepMatching(self.binder, params)

    def set_downscale_factor(self, v):
        self.downscale_factor = v
        return self

    def set_neighbor_radius(self, v):
        self.neighbor_radius = v
        return self

    def set_max_patch_size(self, v):
        self.max_patch_size = v
        return self

    def set_verbose(self, v=True):
        self.verbose = True if v else False
        return self

    def set_nthread(self, v):
        self.nthread = v
        return self
