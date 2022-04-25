
from math import sqrt, sin, cos, atan2


def time_to_GPS(time) -> int:

    return int(time['t_oe'] // 86400) * 86400 + time['hour'] * 3600 + time['minute'] * 60 + time['second']


def localise(args, timedelta=0):

    # preparations
    args['m'] = 3.986005 * 1e14
    args['omega_3'] = 7.2921151467 * 1e-5
    args['A'] = args['sqrt_A'] ** 2

    # 4
    args['t'] = time_to_GPS(args['time'])

    args['t_k'] = args['t'] - args['t_oe'] + timedelta
    #args['t_k'] = args['t'] - args['t_oe_mod']

    if args['t_k'] > 302400:
        args['t_k'] = args['t_k'] - 604800
    elif args['t_k'] < -302400:
        args['t_k'] = args['t_k'] + 604800

    # 5
    args['n'] = sqrt(args['m']) / (args['sqrt_A'] * args['A']) + args['delta_n']

    # 6
    args['M_k'] = args['M_0'] + args['n'] * args['t_k']

    # 7
    args['E_k_n'] = args['M_k']
    while True:

        args['E_k'] = args['E_k_n']
        args['E_k_n'] = args['E_k'] + (args['M_k'] - args['E_k'] + args['e_0'] * sin(args['E_k'])) / (1 - args['e_0'] * cos(args['E_k']))
        if abs(args['E_k_n'] - args['E_k']) < args['eps']:
            break

    # 8
    args['grad_E_k'] = args['n'] / (1 - args['e_0'] * cos(args['E_k']))

    # 9
    args['theta_k'] = atan2(sqrt(1 - args['e_0'] ** 2) * sin(args['E_k']), cos(args['E_k']) - args['e_0'])

    # 10
    args['PHI_k'] = args['theta_k'] + args['omega']

    # 11
    args['grad_PHI_k'] = sqrt(1 - args['e_0'] ** 2) * args['grad_E_k'] / (1 - args['e_0'] * cos(args['E_k']))

    # 12
    args['U_k'] = args['PHI_k'] + args['C_UC'] * cos(2 * args['PHI_k']) + args['C_US'] * sin(2 * args['PHI_k'])

    # 13
    args['grad_U_k'] = args['grad_PHI_k'] * (1 + 2 * (args['C_UC'] * cos(2 * args['PHI_k']) - args['C_US'] * sin(2 * args['PHI_k'])))

    # 14
    args['r_k'] = args['A'] * (1 - args['e_0'] * cos(args['E_k'])) + args['C_RC'] * cos(2 * args['PHI_k']) + args['C_RS'] * sin(2 * args['PHI_k'])

    # 15
    args['grad_r_k'] = args['A'] * args['e_0'] * args['grad_E_k'] * sin(args['E_k']) + 2 * args['grad_PHI_k'] * (args['C_RC'] * cos(2 * args['PHI_k']) + args['C_RS'] * sin(2 * args['PHI_k']))

    # 16
    args['I_k'] = args['I_0'] + args['C_IC'] * cos(2 * args['PHI_k']) + args['C_IS'] * sin(2 * args['PHI_k']) + args['IDOT'] * args['t_k']

    # 17
    args['grad_I_k'] = args['IDOT'] + 2 * args['grad_PHI_k'] * (args['C_IC'] * cos(2 * args['PHI_k']) + args['C_IS'] * sin(2 * args['PHI_k']))

    # 18
    args['X_PHI_k'] = args['r_k'] * cos(args['U_k'])
    args['Y_PHI_k'] = args['r_k'] * sin(args['U_k'])

    # 19
    args['grad_X_PHI_k'] = args['grad_r_k'] * cos(args['U_k']) - args['Y_PHI_k'] * args['grad_U_k']
    args['grad_Y_PHI_k'] = args['grad_r_k'] * sin(args['U_k']) - args['X_PHI_k'] * args['grad_U_k']

    # 20
    args['OMEGA_k'] = args['OMEGA_0'] + (args['OMEGADOT'] - args['omega_3']) * args['t_k'] - args['omega_3'] * args['t_oe']

    # 21
    args['grad_OMEGA_k'] = args['OMEGADOT'] - args['omega_3']

    # 22
    args['X_SVK'] = args['X_PHI_k'] * cos(args['OMEGA_k']) - args['Y_PHI_k'] * cos(args['I_k']) * sin(args['OMEGA_k'])
    args['Y_SVK'] = args['X_PHI_k'] * sin(args['OMEGA_k']) + args['Y_PHI_k'] * cos(args['I_k']) * cos(args['OMEGA_k'])
    args['Z_SVK'] = args['Y_PHI_k'] * sin(args['I_k'])

    # 23
    args['grad_X_SVK'] = -args['grad_OMEGA_k'] * args['Y_SVK'] + args['grad_X_PHI_k'] * cos(args['OMEGA_k']) - (args['grad_Y_PHI_k'] * cos(args['I_k']) - args['Y_PHI_k'] * args['grad_I_k'] * sin(args['I_k'])) * sin(args['OMEGA_k'])
    args['grad_Y_SVK'] = args['grad_OMEGA_k'] * args['X_SVK'] + args['grad_X_PHI_k'] * sin(args['OMEGA_k']) + (args['grad_Y_PHI_k'] * cos(args['I_k']) - args['Y_PHI_k'] * args['grad_I_k'] * sin(args['I_k'])) * cos(args['OMEGA_k'])
    args['grad_Z_SVK'] = args['Y_PHI_k'] * args['grad_I_k'] * sin(args['I_k']) + args['grad_Y_PHI_k'] * sin(args['I_k'])
