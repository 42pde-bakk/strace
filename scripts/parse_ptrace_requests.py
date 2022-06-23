def parse_ptrace_requests():
    lines = open('/usr/include/sys/ptrace.h', 'r').read().splitlines()
    with open('srcs/ptrace_request_handler.c', 'w') as f:
        f.write('#include <sys/ptrace.h>\n')
        f.write('#include <stdio.h>\n\n')
        f.write('static void	enum__ptrace_request_handler(const unsigned long long int regval) {\n')
        f.write('\tswitch (regval) {\n')
        for line in lines:
            if '=' not in line:
                continue
            name, value = line.strip().split(' = ')
            f.write(f'\t\tcase {name}:\n')
            f.write(f'\t\t\tfprintf(stderr, "%s", "{name}");\n')
            f.write('\t\t\tbreak;\n')
        f.write('\t\tdefault:\n')
        f.write('\t\t\tfprintf(stderr, "%llu", regval);\n')
        f.write('\t\t\tbreak;\n')
        f.write('\t}\n')
        f.write('}\n')


if __name__ == '__main__':
    parse_ptrace_requests()
