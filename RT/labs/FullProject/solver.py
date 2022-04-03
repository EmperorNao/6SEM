import numpy as np
from numpy.linalg import norm as L2
from scipy.optimize import fsolve


c = 299792458


class Func:

    def __init__(self, S, X_sv, t):
        assert X_sv.shape[0] >= 4

        self.S = S
        self.X_sv = X_sv
        self.t = t


    def __call__(self, x):

        # delta_t is variable
        #return L2(x - self.X_sv * self.t) + self.delta_t * c - self.S
        return (
        L2(np.array([x[0], x[1], x[2]]) - self.X_sv[0] * self.t[0], 2) + x[3] * c - self.S[0],
        L2(np.array([x[0], x[1], x[2]]) - self.X_sv[1] * self.t[1], 2) + x[3] * c - self.S[1],
        L2(np.array([x[0], x[1], x[2]]) - self.X_sv[2] * self.t[2], 2) + x[3] * c - self.S[2],
        L2(np.array([x[0], x[1], x[2]]) - self.X_sv[3] * self.t[3], 2) + x[3] * c - self.S[3]
        )


def solve_navigation(S, X_sv, t):

    # shape
    return fsolve(Func(S, X_sv, t), np.zeros(4), maxfev = 100)
