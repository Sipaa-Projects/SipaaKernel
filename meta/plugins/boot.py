from ast import arg
import os
import logging
from cutekit import args, builder, const, cmds, shell

logger = logging.getLogger(__name__)
arch = "x86_64"
diskImage = ""

def kvmAvailable() -> bool:
    if os.path.exists("/dev/kvm") and os.access("/dev/kvm", os.R_OK):
        return True
    return False

def installLimine(bootDir: str) -> None:
    XORRISO = [
        "xorriso", "-as", "mkisofs", "-b", "limine-bios-cd.bin", "-no-emul-boot",
        "-boot-load-size", "4", "-boot-info-table", "--efi-boot", "limine-uefi-cd.bin", "-efi-boot-part", 
        "--efi-boot-image", "--protective-msdos-label", bootDir, "-o", diskImage
    ]

    files = [
        "limine-bios.sys",
        "limine-bios-cd.bin",
        "limine-uefi-cd.bin",
        "limine.c",
        "limine-bios-hdd.h"
    ]

    for file in files:
        limine = shell.wget(
            f"https://raw.githubusercontent.com/limine-bootloader/limine/v5.x-branch-binary/{file}"
        )

        if file not in ["limine.c", "limine-bios-hdd.h"]:
            shell.cp(limine, os.path.join(bootDir, file))
        else:
            shell.cp(limine, os.path.join(const.CACHE_DIR, file))

    shell.exec(*XORRISO)
    shell.exec(*["cc", "-o", os.path.join(const.CACHE_DIR, "limine"), os.path.join(const.CACHE_DIR, "limine.c")])
    shell.exec(*[os.path.join(const.CACHE_DIR, "limine"), "bios-install", diskImage])

def buildIso(args: args.Args) -> None:
    if not os.path.exists(os.path.join(".", "output")):
        os.mkdir(os.path.join(".", "output"))

    imageDir = shell.mkdir(os.path.join(const.PROJECT_CK_DIR, "images", "sk"))
    bootDir = shell.mkdir(os.path.join(imageDir, "boot"))
    global arch

    kernel = builder.build('sk-core', "kernel-" + str(arch))

    shell.cp(kernel.outfile(), os.path.join(bootDir, "kernel.elf"))
    shell.cp(os.path.join(const.META_DIR, "config", "limine.cfg"), bootDir)

    installLimine(imageDir)

def bootCmd(args: args.Args) -> None:
    global arch
    global diskImage
    arch = args.consumeOpt("arch")
    if arch == False:
        arch = "x86_64"
    diskImage = os.path.join(os.path.join(".", "output"), "sk-" + str(arch) + ".iso")
    buildIso(args)

    qemu_cmdline = "qemu-system-" + str(arch)

    print("qemu cmdline : " + qemu_cmdline)

    qemu = [
        qemu_cmdline,
        "-m", "4G",
        "-smp", "4",
        "-serial", "mon:stdio",
        "-cdrom", diskImage,
        "-bios", "meta/efi/efi-" + str(arch) + ".fd"
    ]

    if arch == "aarch64":
        qemu += [
            "-cpu", "cortex-a72",
            "-device", "ramfb",
            "-machine", "virt",
        ]
    else:
        qemu += [
            "-machine", "q35",
        ]
    if kvmAvailable() and str(arch) == "x86_64":
        qemu += ["-enable-kvm", "-cpu", "host"]

    shell.exec(*qemu)

cmds.append(cmds.Cmd('bi', 'build-iso', 'Build the ISO file', buildIso))
cmds.append(cmds.Cmd('B', 'boot', 'Boot the kernel', bootCmd))