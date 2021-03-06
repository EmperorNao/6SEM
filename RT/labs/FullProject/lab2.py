from load import load_observation, load_navigation
from localisation import localise
from solver import solve_navigation
import traceback
import numpy as np
from math import sqrt, sin



def transform_data(s: str) :
    splited = s.split()
    return '20{0} {1} {2} {3} {4} {5}'.format(
    splited[0],
    splited[1].rjust(2, '0'),
    splited[2].rjust(2, '0'),
    splited[3].rjust(2, '0'),
    splited[4].rjust(2, '0'),
    #splited[5].rjust(20, '0')[:2],
    '00'
    )


def realtime(s):
    spl = s.split()
    return int(spl[3]) * 3600 + int(spl[4]) * 60 + int(spl[5])


def correction(data):

    return data['a_0'] + data['t_k'] * (data['a_1'] + data['t_k'] * data['a_2']) - 4.442807633 * 1e-10 * data['e_0'] * data['sqrt_A'] * sin(data['E_k']) - data['T_GD']
    #return 0


c = 299792458


if __name__ == '__main__':

    ofname = r'.\abpo0010.20o'
    nfname = r'.\ABPO00MDG_R_20200010000_01D_GN.rnx'
    #ts = '20  1  1 21  0  0.0000000'
    ts = '20  1  1 20  0  0.0000000'

    transformeds = transform_data(ts)

    S = []
    X_sv = []
    t = []

    try:

        nsats = 0
        local_data = load_observation(ofname, ts)

        # for sat, values in local_data['sats'].items():
        #     print(f"{sat}: {values['C1'] if 'C1' in values else '-'}")


        print(f"Real position = {local_data['position']}")

        for sat, obs in local_data['sats'].items():
            if sat.startswith('G') and obs['C1'] is not None:

                #print(sat)
                try:
                    data = load_navigation(nfname, sat, transformeds)

                    #print(f"SAT = {sat}")
                    #print(data)
                    #print()

                    # print(sat)
                    # print(data)

                    timedelta = -obs['C1'] / c
                    # print(f"t_delta = {timedelta}")
                    localise(data, timedelta)

                    X_sv.append(np.array([data['X_SVK'], data['Y_SVK'], data['Z_SVK']]))
                    # print(f"{sat}: X = {data['X_SVK']}, Y = {data['Y_SVK']}, Z = {data['Z_SVK']}\n")
                    # print(f"{data['a_0'], data['a_1'], data['a_2']}")
                    print(sat)
                    print(np.array([data['X_SVK'], data['Y_SVK'], data['Z_SVK']]))

                    nsats += 1
                    t.append(1)
                    #realtime(transformeds)-obs['C1'] / c)
                    #print(f"C1 = {obs['C1']}")
                    #print(obs['C1'])
                    #print(-correction(data) * c)
                    #print(sat, data['T_GD'])
                    S.append(obs['C1'] - correction(data) * c)
                    #S.append(obs['C1'])
                    #print(f" NSATS = {nsats}")

                except BaseException:
                    print(f"Don't find {sat} in navigation")

        # print(f"S = {S}, t = {t}")
        #
        # print()
        # print(X_sv)
        # print()
        pred = solve_navigation(np.array(S), np.array(X_sv), t)
        print(f"Predicted position = {pred[0], pred[1], pred[2], pred[3]}")


    except BaseException as e:
        print(traceback.format_exc())
