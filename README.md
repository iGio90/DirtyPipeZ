# DirtyPipeZ

This repo contains my researches on Dirtypipe and Android.

This is not a reliable solution, check out [polygraphene implementation](https://github.com/polygraphene/DirtyPipe-Android/) if you are looking for a ready-to-use "tool".

----

I started this research in the early-mid of march, I tried all my best to avoid re-abusing the bug again from init
but at some point polygraphene pushed his opensource implementation - at that point, i had finished my init takeover but i failed figuring some way to dont abuse the bug again from init - so i adapted his/her stage2 to work on my chain.

back in the days polygraphene trigger was from adb, not sure if it's still same, but he/she is free to rip my method if he/she want to create an app that does 1 click.

----

The shellcodes directory has the result of those days spent trying to not abusing the bug again. There is a dlopen shellcode and a cp shellcode which i will likely re-use in the future.
