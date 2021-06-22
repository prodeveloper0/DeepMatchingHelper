from .utils import *


def bind(libname):
    from .native_binder import Binder
    from .deepmatching_builder import DeepMatchingBuilder
    return DeepMatchingBuilder(Binder(libname))
