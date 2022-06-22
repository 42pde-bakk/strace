def parse_errno_table():
    # https://www.thegeekstuff.com/2010/10/linux-error-codes/
    lines = open('/Users/pde-bakk/CLionProjects/strace/errno_table.txt', 'r').read().splitlines()
    with open('/Users/pde-bakk/CLionProjects/strace/srcs/errno_table.c', 'w') as f:
        f.write('#include "strace.h"\n\n')
        f.write('t_errno errnoTable[] = {\n')
        for line in lines:
            print(line)
            print(line.split('\t'))
            nb, code, description = line.split('\t')
            f.write(f'\t\t[{nb}] = {{ .nb = {nb}, .code = "{code}", .description = "{description}" }},\n')
        f.write('};\n')


if __name__ == '__main__':
    parse_errno_table()


