## Example how move-mount-no-propagate can be used:

Required kernel patch for MOVE_MOUNT_NO_PROPAGATE:
https://github.com/Snorch/linux/commit/84886f588527b062993ec3e9760c879163852518

### Term 1

```
unshare -m --propagation private
mkdir shared-{1..10}
mkdir shared-1/mnt
mount -t tmpfs tmpfs shared-1
mount --make-shared shared-1
for i in shared-{2..10}; do mount --bind shared-1 $i; done
echo $$
#8436
```

### Term 2

```
unshare -m --propagation private
mkdir shared private slave
mount -t tmpfs tmpfs private
mount --bind private shared
mount --make-shared shared
mount --bind shared slave
mount --make-slave slave

./move-mount-no-propagate private /proc/8436/ns/mnt /move-mount-no-propagate/shared-1/mnt
#Mounting private into mount namespace of /proc/8436/ns/mnt at /move-mount-no-propagate/shared-1/mnt
#Done
./move-mount-no-propagate shared /proc/8436/ns/mnt /move-mount-no-propagate/shared-2/mnt
#Mounting shared into mount namespace of /proc/8436/ns/mnt at /move-mount-no-propagate/shared-2/mnt
#Done
./move-mount-no-propagate slave /proc/8436/ns/mnt /move-mount-no-propagate/shared-3/mnt
#Mounting slave into mount namespace of /proc/8436/ns/mnt at /move-mount-no-propagate/shared-3/mnt
#Done

cat /proc/self/mountinfo | grep move-mount-no-propagate
#1313 1301 0:69 / /move-mount-no-propagate/private rw,relatime - tmpfs tmpfs rw
#1314 1301 0:69 / /move-mount-no-propagate/shared rw,relatime shared:505 - tmpfs tmpfs rw
#1315 1301 0:69 / /move-mount-no-propagate/slave rw,relatime master:505 - tmpfs tmpfs rw
```

### Term 1

```
cat /proc/self/mountinfo | grep move-mount-no-propagate
#1303 1262 0:68 / /move-mount-no-propagate/shared-1 rw,relatime shared:504 - tmpfs tmpfs rw
#1304 1262 0:68 / /move-mount-no-propagate/shared-2 rw,relatime shared:504 - tmpfs tmpfs rw
#1305 1262 0:68 / /move-mount-no-propagate/shared-3 rw,relatime shared:504 - tmpfs tmpfs rw
#1306 1262 0:68 / /move-mount-no-propagate/shared-4 rw,relatime shared:504 - tmpfs tmpfs rw
#1307 1262 0:68 / /move-mount-no-propagate/shared-5 rw,relatime shared:504 - tmpfs tmpfs rw
#1308 1262 0:68 / /move-mount-no-propagate/shared-6 rw,relatime shared:504 - tmpfs tmpfs rw
#1309 1262 0:68 / /move-mount-no-propagate/shared-7 rw,relatime shared:504 - tmpfs tmpfs rw
#1310 1262 0:68 / /move-mount-no-propagate/shared-8 rw,relatime shared:504 - tmpfs tmpfs rw
#1311 1262 0:68 / /move-mount-no-propagate/shared-9 rw,relatime shared:504 - tmpfs tmpfs rw
#1312 1262 0:68 / /move-mount-no-propagate/shared-10 rw,relatime shared:504 - tmpfs tmpfs rw
#1316 1303 0:69 / /move-mount-no-propagate/shared-1/mnt rw,relatime - tmpfs tmpfs rw
#1317 1304 0:69 / /move-mount-no-propagate/shared-2/mnt rw,relatime shared:505 - tmpfs tmpfs rw
#1318 1305 0:69 / /move-mount-no-propagate/shared-3/mnt rw,relatime master:505 - tmpfs tmpfs rw
```
