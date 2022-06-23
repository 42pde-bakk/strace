def parse_syscalls():
    enum_converts = {
        'T_INT': 'INT',
        'T_ADDR': 'POINTER',
        'T_HEX': 'INT',
        'T_LINT': 'LONG_INT',
        'T_NONE': 'NONE',
        'T_ULINT': 'UNSIGNED_LONG_INT',
        'T_STR': 'STRING',
        'T_E_PTRACE_REQUEST': 'E_PTRACE_REQUEST',
        'T_UNKNOWN': 'INT',
        'T_UINT': 'UNSIGNED_INT',
        'T_CADDR_T': 'POINTER',
        'T_LLINT': 'LONG_LONG_INT',
        'T_ULLINT': 'UNSIGNED_LONG_LONG_INT',
        'T_BUFF': 'VOID_POINTER'
    }
    # https://raw.githubusercontent.com/vsteffen/ft_strace/master/includes/syscall_tables/syscall64.h
    lines = open('syscalls.txt', 'r').read().splitlines()
    with open('srcs/syscall_table.c', 'w') as f:
        f.write('#include "syscalls.h"\n\n')
        f.write('const t_syscall syscalls[] = {\n')
        for line in lines:
            nb, item = line.split(' = ')
            nb = int(nb.strip('[').strip(']'))
            print(nb)
            item = item.replace('{', '').replace('}', '').replace(',', '')
            for k, v in enum_converts.items():
                item = item.replace(k, v)
            arr = item.split()
            name = arr[0]
            registers = arr[1:-2]
            return_value = arr[-2]
            print(name)
            print(registers)
            registers_str = ', '.join(registers)
            f.write(f'\t\t[{nb}] = {{')
            f.write(f' .nb64 = {nb}, ')
            f.write(f' .return_value = {return_value}, ')
            f.write(f' .name = {name}, ')
            f.write(f' .registers = {{ {registers_str} }}, ')
            stripped_name = name.strip('"')
            if stripped_name in ['mmap', 'ptrace']:
                f.write(f' .handler = &{stripped_name}_handler')
            else:
                f.write(f'.handler = &print_syscall')
            f.write('},\n')
        f.write('};\n')


if __name__ == '__main__':
    parse_syscalls()


