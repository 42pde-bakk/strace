import json


def parse_syscalls():
    # https://raw.githubusercontent.com/vsteffen/ft_strace/master/includes/syscall_tables/syscall64.h
    lines = open('syscalls.txt', 'r').read().splitlines()
    with open('/Users/pde-bakk/CLionProjects/strace/srcs/syscall_table.c', 'w') as f:
        f.write('#include "syscalls.h"\n\n')
        f.write('t_syscall syscalls[] = {\n')
        for line in lines:
            nb, item = line.split(' = ')
            nb = nb.strip('[').strip(']')
            print(nb)
            item = item.replace('{', '').replace('}', '').replace(',', '')
            arr = item.split()
            name = arr[0]
            registers = arr[1:-2]
            return_value = arr[-2]
            handler = arr[-1]
            print(name)
            print(registers)
            print(handler)
            registers_str = ', '.join(registers)
            f.write(f'\t\t[{nb}] = {{ .nb64 = {nb}, .return_value = {return_value}, .name = {name}, {registers_str} }},\n')
            # item = item.replace('T_INT', 'INT').replace('T_NONE', 'NONE')
            # nb, code, description = line.split('\t')
            # f.write(f'\t\t[{nb}] = {{ .nb = {nb}, .code = "{code}", .description = "{description}" }},\n')
        f.write('};\n')


if __name__ == '__main__':
    parse_syscalls()


