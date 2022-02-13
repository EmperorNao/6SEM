from load import load_file
from localisation import localise

if __name__ == '__main__':

    fname = r'P:\D\6SEM\РеальноеВремя\labs\lab1\ABPO00MDG_R_20200010000_01D_GN.rnx'
    satellite = 'G10'
    ts = '2020 01 01 00 00 00'

    data = load_file(fname, satellite, ts)
    localise(data)

    print(data['X_SVK'])
    print(data['Y_SVK'])
    print(data['Z_SVK'])

    print(data['grad_X_SVK'])
    print(data['grad_Y_SVK'])
    print(data['grad_Z_SVK'])
