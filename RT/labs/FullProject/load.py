



def load_navigation(fname, satellite, ts):

    data = dict()
    data['time'] = dict()
    time_keys = ['year', 'month', 'day', 'hour', 'minute', 'second']
    add_keys = ['a_0', 'a_1', 'a_2']
    keys = ["C_RS", "delta_n", "M_0", "C_UC", "e_0", "C_US", "sqrt_A", "t_oe",
    "C_IC", "OMEGA_0", "C_IS", "I_0", "C_RC", "omega", "OMEGADOT", "IDOT", "L2",
     "GPSWeek", "L2PFLAG", "PRECISION_OF_SAT", "ISPR", "T_GD"];


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

                out = data_lines[i][23:]#[23
                width = 19
                for j, key in enumerate(add_keys):
                    data[key] = float(out[width * (j): width * (j + 1)])


                out = "".join(map(lambda x: x[4:].strip('\n'), data_lines[i+1:i+7]))
                # if satellite == 'G21':
                #     print(data_lines[i][23:])
                #     print(out)
                width = 19
                for j, key in enumerate(keys):
                    data[key] = float(out[width * (j + 1): width * (j + 2)])

                time_split = ts.split(" ")
                for j, key in enumerate(time_keys):
                    data['time'][key] = int(time_split[j])

                data['time']['t_oe'] = data['t_oe']
                data['t_oe_mod'] = data['t_oe'] % 86400
                #if data['t_oe_mod'] >= data['t_oe']
                data['eps'] = 1e-9

                return data

    raise KeyError("Don't find this format in file")


def load_observation(fname, ts):

    def is_float(element):
        try:
            float(element)
            return True
        except ValueError:
            return False

    data = dict()
    data['sats'] = {}
    data['position'] = {}

    with open(fname) as file:

        lines = file.readlines()

        for i, line in enumerate(lines):
            if line.strip().endswith("APPROX POSITION XYZ"):
                spl = line.rstrip("APPROX POSITION XYZ").split()
                data['position']['X'] = spl[0]
                data['position']['Y'] = spl[1]
                data['position']['Z'] = spl[2]
                break


        types_of_observation = []
        for i, line in enumerate(lines):
            if line.strip().endswith("# / TYPES OF OBSERV"):
                types_of_observation = line.rstrip("# / TYPES OF OBSERV").split()[1:-1]
                break


        start_line_idx = 0
        for i, line in enumerate(lines):
            if line.endswith("END OF HEADER"):
                start_line_idx = i
                break

        format = " " + ts

        ts_line_idx = start_line_idx
        data_lines = lines[start_line_idx:]
        for i, line in enumerate(data_lines):
            if line.startswith(format):
                ts_line_idx = i
                break

        n_sats = 0
        for i, line in enumerate(data_lines[ts_line_idx: ts_line_idx + 3]):
            significant_line = line[30:].strip()
            if i == 0:
                n_sats = int(significant_line[:2])
                significant_line = significant_line[2:]

            for i in range(0, len(significant_line), 3):
                data['sats'][significant_line[i: i + 3]] = {}

        #print(data['sats'])
        for i, sat in enumerate(data['sats']):

            pos = ts_line_idx + 3 + i * 2

            #print(sat)
            #print(line)
            line = data_lines[pos] + "  " + data_lines[pos + 1]
            for j, obs in enumerate(types_of_observation):

                num = line[j * 16: 16 * (j + 1)][:-1]
                data['sats'][sat][obs] = float(num) if is_float(num) else None

    return data
