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
        return [ L2(np.array([x[0], x[1], x[2]]) - self.X_sv[i], 2) ** 0.5 + x[3] * c - self.S[i] for i in range(len(self.X_sv)) ]


def solve_navigation(S, X_sv, t):

    # shape
    # return fsolve(Func(S, X_sv, t), np.zeros(X_sv.shape[0]), maxfev = 1000)

    #print(f"S.shape = {S.shape}, X_sv = {X_sv.shape}")

    position = np.zeros((4, 1))
    position = np.expand_dims(np.array([4097216.5539, 4429119.1897, -2065771.1988, 0]), -1)

    #sy = np.array(sy_list)

    eps = 1e-6
    norm = 1000000000000

    iter = 0
    while norm > eps:
        #print(f"Iter {iter + 1}")

        r = np.empty((S.shape[0], 1))
        der = np.empty((S.shape[0], 4))

        for i in range(der.shape[0]):
            vec = np.array(X_sv[i])
            #print(f"position = {position}, vec = {vec}")
            diff_x = position[0][0] - vec[0]
            diff_y = position[1][0] - vec[1]
            diff_z = position[2][0] - vec[2]
            #print(f"{diff_x}, {diff_y}, {diff_z},")
            range_ = (diff_x ** 2 + diff_y ** 2 + diff_z ** 2) **  0.5
            der[i][0] = diff_x / range_
            der[i][1] = diff_y / range_
            der[i][2] = diff_z / range_
            der[i][3] = c
            r[i][0] = range_ + position[3][0] * c - S[i]#sy_list[i]

        #print(der.shape)
        #print(der)
        print(r, '\n')
        der_t = der.transpose()
        matrix = np.dot(der_t, der)

        #print(matrix)

        #print(matrix)

        matrix = np.linalg.inv(matrix)
        dpos = np.dot(np.dot(matrix, der_t), r)
        norm = np.linalg.norm(dpos)
        position -= dpos
        iter += 1
        #print(f"POS = {position}")

    #print(f"Solved by {iter} iterations")

    answer = [p[0] for p in position]

    return answer
