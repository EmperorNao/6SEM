from load import load_file
from localisation import localise

if __name__ == '__main__':

    fname = r'.\ABPO00MDG_R_20200010000_01D_GN.rnx'
    satellite = 'G14'
    ts = '2020 01 01 04 00 00'

    try:
        data = load_file(fname, satellite, ts)
        localise(data)

        print(f"SAT № {satellite}")
        print(f"For YEAR = {data['time']['year']}, MONTH = {data['time']['month']}, DAY = {data['time']['day']}, HOUR = {data['time']['hour']}, MINUTE = {data['time']['minute']}, SECOND = {data['time']['second']}")
        print(f"X = {data['X_SVK']}, Y = {data['Y_SVK']}, Z = {data['Z_SVK']}\n"
              f"X_speed = {data['grad_X_SVK']}, Y_speed = {data['grad_Y_SVK']}, Z_speed = {data['grad_Z_SVK']}")

    except BaseException as e:
        print(f"Error = {e}")
