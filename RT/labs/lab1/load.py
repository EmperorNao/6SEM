

def load_file(fname, satellite, ts):

    data = dict()
    data['time'] = dict()
    time_keys = ['year', 'month', 'day', 'hour', 'minute', 'second']
    keys = ["C_RS", "delta_n", "M_0", "C_UC", "e_0", "C_US", "sqrt_A", "t_oe", "C_IC", "OMEGA_0", "C_IS", "I_0", "C_RC", "omega", "OMEGADOT", "IDOT", "L2", "GPSWeek"];


    with open(fname) as file:

        lines = file.readlines()

        start_line_idx = 0
        for i, line in enumerate(lines[start_line_idx:]):
            if "END OF HEADER" in line:
                start_line_idx = i
                break

        format = satellite + " " + ts

        data_lines = lines[start_line_idx:]
        for i, line in enumerate(data_lines):
            if format in line:

                out = "".join(map(lambda x: x[4:].strip('\n'), data_lines[i+1:i+6]))
                width = 19
                for i, key in enumerate(keys):
                    data[key] = float(out[width * (i + 1): width * (i + 2)])

                time_split = ts.split(" ")
                for i, key in enumerate(time_keys):
                    data['time'][key] = int(time_split[i])

                data['time']['t_oe'] = data['t_oe']
                data['t_oe_mod'] = data['t_oe'] % 86400
                #if data['t_oe_mod'] >= data['t_oe']
                data['eps'] = 1e-9

                return data

    raise KeyError("Don't find this format in file")
