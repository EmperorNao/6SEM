import numpy as np
from numpy.linalg import norm as L2
from scipy.optimize import fsolve


c = 299 792 458


class Func:

    def __init__(S, X_sv, S_, t):

        self.S = S
        self.X_sv = X_sv
        self.S_ = S_
        self.t = t
    def __call__(x):

        # delta_t is variable
        return L2(x - self.X_sv * self.t) + self.delta_t * c - self.S


def solve_navigation(S, X, X_sv, S_, t, delta_t):

    # shape 
    return fsolve(Func(S, X_sv, S_, t, delta_t), np.zeros_like(t))
