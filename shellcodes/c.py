import os
import r2pipe

os.system('cd .. && ./gradlew app:assembleDebug')

bin = '../app/build/intermediates/cmake/debug/obj/arm64-v8a/shellcode'
if not os.path.exists(bin):
    print(bin, 'file not found')
    exit(1)

r = r2pipe.open(bin)
r.cmd('aaaa')
r.cmd('s main')
l = r.cmdj('pifj')
for i in l['ops']:
    print(hex(i['offset']), '\t', i['bytes'], '\t', i['disasm'])
