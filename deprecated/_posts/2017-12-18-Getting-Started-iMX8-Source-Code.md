---
layout: post
title: "Getting Started with i.MX8 Source Code"
author: "Diego Dorta"
categories: tutorial
tags: [ imx8, source, code, uboot, kernel, rootfs ]
---

This post provides a step-by-step guide to users interested to build *U-Boot* and *Linux Kernel* for i.MX8 SoC boards manually, just using their source codes.
The approaching ones are `i.MX8QM LPDDR4 ARM2`, and `i.MX8MQ EVK Board`. The tutorial also demonstrate how to integrate them with a *rootfs* generated by the `Yocto Project`.
The *Kernel*, and *rootfs* procedure are the same for all boards, however *U-Boot* requires a different approach for each *SoC*, all detailed at *U-Boot* step.
To create a full image, choose a *U-Boot* step, then follow the *Kernel* and *rootfs* steps:

*Some steps may not work completely due the constant maintenance.*

# Installing Toolchain

A `cross compiler` is required to compile the source codes. This guide was tested with both
cross compiler packages `Yocto` or `Linux`:

#### Toolchain provide by Yocto

Get the download [`here`](http://shlinux12.ap.freescale.net/Yocto_SDK/4.9.x-1.0.0-GCC6.2/aarch64/). Then, change the permission to execute, you may need `root privileges`:

```bash
$ chmod +x fsl-imx-internal-xwayland-glibc-x86_64-fsl-image-gui-aarch64-toolchain-4.9.51-mx8-beta.sh
```
Install and apply `yes` for all:

```bash
$ ./fsl-imx-internal-xwayland-glibc-x86_64-fsl-image-gui-aarch64-toolchain-4.9.51-mx8-beta.sh 
```

Export the `toolchain`:

```bash
$ source /opt/fsl-imx-internal-xwayland/4.9.51-mx8-beta/environment-setup-aarch64-poky-linux
```

#### Toolchain provide by Linux

Install the Ubuntu toolchain `gcc-aarch64-linux-gnu` GNU C compiler for the `arm64` architecture:

```bash
$ apt-get install gcc-aarch64-linux-gnu
```
Export the environment variables:

```bash
$ export ARCH=arm64
$ export CROSS_COMPILE=/usr/bin/aarch64-linux-gnu-
```
## U-Boot 

### U-Boot for i.MX8QM LPDDR4 ARM2

#### Mkimage Step

Create a folder for organize the files:

```bash
$ mkdir mx8 && cd mx8/
```
Download of mkimage:

```bash
$ git clone https://bitbucket.sw.nxp.com/scm/imx/imx-mkimage.git
$ cd imx-mkimage/
$ git checkout imx_4.9.51_imx8_beta1
```
```NOTE
Commit ID: e131af1031fff50106777c2338c41bf58c0b86a1
```
After downloading, follow the next steps.

#### U-Boot Step 

Download the *U-Boot* from:

```bash
$ git clone https://bitbucket.sw.nxp.com/scm/imx/uboot-imx.git
$ cd uboot-imx/
$ git checkout imx_v2017.03_4.9.51_imx8_beta1
```
```NOTE
Commit ID: c8e6eb2179512ee87c8faa08bbe826ead40ab702
```
Compile the *U-Boot*:

```bash
$ unset LDFLAGS
$ make -j8 CC="$CC" imx8qm_lpddr4_arm2_defconfig
$ make -j8 CC="$CC"
```
The `u-boot.bin` file will be created. Copy it to mkimage `iMX8QM folder`:

```bash
$ cp u-boot.bin ../imx-mkimage/iMX8QM/
```

```IMPORTANT
Use *unset LDFLAGS* if the Yocto toolchain is used.
```

#### ATF (Arm Trust Firmware) Step

Get the download of `Arm Trust Firmware`:

```bash
$ git clone https://bitbucket.sw.nxp.com/scm/imx/arm-trusted-firmware.git
```
Compile the `ATF` by doing:

```bash
$ make PLAT=imx8qm bl31
```
This builds `bl31.bin` file on: *build/imx8qm/release/bl31.bin*.

Copy the `bl31.bin` inside mkimage `iMX8QM folder`:

```bash
$ cp build/imx8mq/release/bl31.bin ../imx-mkimage/iMX8QM/
```
#### SC Firmware Step

*This section needs improvements. Currently it is not possible to compile the firmware the pre-built firmware is used instead.*

Get the firmware from downloading the [`NXP Alpha release`](https://www.nxp.com/webapp/Download?colCode=L4.9.11_IMX8QM_SOURCE_Alpha&appType=license&Parent_nodeId=1454108179417712493862&Parent_pageType=product).
Copy the `imx-sc-firmware-0.1.bin` to mx8 folder, then:

```bash
$ chmod +x imx-sc-firmware-0.1.bin
$ ./imx-sc-firmware-0.1.bin
```
Copy the `firmware` into mkimage iMX8QM folder:

```bash
$ cp imx-sc-firmware-0.1/mx8qm-scfw-tcm.bin ../imx-mkimage/iMX8QM/
```
Then, rename the firmware to 'scfw_tcm.bin':

```bash
$ cd imx-mkimage/iMX8QM/
$ mv mx8qm-scfw-tcm.bin scfw_tcm.bin
```
#### Gathering Step

Check the files: `bl31.bin`, `scfm_tcm.bin`, `u-boot.bin`:

```bash
$ cd imx-mkimage/
$ tree iMX8QM/
├── bl31.bin
├── expand_c_define.sh
├── head.hash
├── imx8qm_dcd_1.6GHz.cfg
├── imx8qm_dcd_800MHz.cfg
├── imx8qm_dcd.cfg
├── lib
│
├── ddrc_mem_map.h
│
└── ddr_phy_mem_map.h
├── scfw_tcm.bin
├── soc.mak
├── u-boot-atf.bin
├── u-boot.bin
└── u-boot-hash.bin
1 directory, 14 files
```
*Open a new terminal to generate flash.bin file.*

```bash
$ cd imx-mkimage/
$ make clean
$ make SOC=iMX8QM flash
```
#### Flashing Step

Typical flash cmd: 

```bash
$ dd if=iMX8QM/flash.bin of=/dev/<your device> bs=1k seek=33 status=progress && sync
```

### U-Boot for i.MX8MQ EVK Board

#### Mkimage Step

Create a folder for organize the files:

```bash
$ mkdir mx8 && cd mx8/
```
Download of mkimage:

```bash
$ git clone https://bitbucket.sw.nxp.com/scm/imx/imx-mkimage.git
$ cd imx-mkimage/
$ git checkout imx_4.9.51_imx8_beta1
```
```NOTE
Commit ID: e131af1031fff50106777c2338c41bf58c0b86a1
```
After downloading, follow the next steps.

#### U-Boot Step

Download the U-Boot from:

```bash
$ git clone https://bitbucket.sw.nxp.com/scm/imx/uboot-imx.git
$ cd uboot-imx/
$ git checkout imx_v2017.03_4.9.51_imx8_beta1
```
```NOTE
Commit ID: c8e6eb2179512ee87c8faa08bbe826ead40ab702
```
To compile the *U-Boot*:

```bash
$ unset LDFLAGS
$ make -j8 imx8mq_evk_defconfig
$ make -j8
```
Copy the `u-boot.bin`, `u-boot-spl.bin`, `u-boot-nodtb.bin`, `fsl-imx8mq-evk.dtb` to the mkimage iMX8QM folder:

```bash
$ cp spl/u-boot-spl.bin ../imx-mkimage/iMX8M/
$ cp u-boot.bin ../imx-mkimage/iMX8M/
$ cp u-boot-nodtb.bin ../imx-mkimage/iMX8M/
$ cp arch/arm/dts/fsl-imx8mq-evk.dtb ../imx-mkimage/iMX8M/
```

```NOTE
Use *unset LDFLAGS* if the Yocto toolchain is used.
```
#### ATF (Arm Trust Firmware) Step

Download the ATF from:

```bash
$ git clone https://bitbucket.sw.nxp.com/scm/imx/arm-trusted-firmware.git
$ cd arm-trusted-firmware/
$ git checkout imx_4.9.51_imx8_beta1
```
```NOTE
Commit ID: a4388010f06ceb8fa9a33391bff47dabd027dbb1
```
Compile the ATF by running:

```bash
$ make PLAT=imx8mq bl31
```
This builds `bl31.bin` file on: *build/imx8mq/release/bl31.bin*.

Copy the `bl31.bin` to the mkimage iMX8M folder:

```bash
$ cp build/imx8mq/release/bl31.bin ../imx-mkimage/iMX8M/
```

#### SC Firmware Step

*This section needs improvements. Currently it is not possible to compile the firmware the pre-built firmware is used instead.*

Download the file [`imx-4.9.51-mx8qm_beta1-external-mirror_2017-11-29.tar.gz`](http://yb2.am.freescale.net/build-output/Linux_IMX_MX8_BETA/79/build_log/imx_rc_archive/external-mirror/). Then, follow the steps:

```bash
$ tar -xf imx-4.9.51-mx8qm_beta1-external-mirror_2017-11-29.tar.gz
$ chmod +x firmware-imx-7.1.bin
$ ./firmware-imx-7.1.bin
```
Copy the files to the mkimage iMX8M folder:

```bash
$ cd firmware-imx-7.1.bin/
$ cp firmware/hdmi/cadence/hdmi_imx8m.bin ../imx-mkimage/iMX8M/
$ cp firmware/ddr/synopsys/lpddr4_pmu_train_* ../imx-mkimage/iMX8M/
```
#### Gathering Step

Check the files: `bl31.bin`, `fsl-imx8mq-evk.dtb`, `hdmi_imx8m.bin`, `u-boot.bin`, `u-boot-nodtb.bin`, `u-boot-spl.bin`, `lpddr4_pmu_train_dmem.bin`, `lpddr4_pmu_train_imem.bin`:

```bash
$ cd imx-mkimage/
$ tree iMX8M/
├── bl31.bin
├── fsl-imx8mq-evk.dtb
├── hdmi_imx8m.bin
├── lib
│
└── ddr_memory_map.h
├── lpddr4_pmu_train_dmem.bin
├── lpddr4_pmu_train_imem.bin
├── mkimage_imx8.c
├── README
├── soc.mak
├── u-boot.bin
├── u-boot-nodtb.bin
└── u-boot-spl.bin
1 directory, 12 files
```
*Open a new terminal to generate flash.bin file.*

```bash
$ cd imx-mkimage/
$ make clean
$ make SOC=iMX8M flash_spl_uboot
```
#### Flashing Step

Typical flash cmd: 

```bash
$ dd if=iMX8M/flash.bin of=/dev/<your device> bs=1k seek=33 status=progress && sync
```
## Kernel

Download `linux-imx` from:

```bash
$ git clone https://bitbucket.sw.nxp.com/scm/imx/linux-imx.git
$ cd linux-imx/
$ git checkout imx_4.9.51_imx8_beta1
```
**Export** one of the toolchain `Yocto` or `Linux`. Then:

### Compiling

Compile the *Kernel*:

```bash
$ make ARCH=arm64 defconfig
$ make -j8 ARCH=arm64
```
Copy the Image file and dtb into your SD card:

```bash
$ cp arch/arm64/boot/Image /media/${USER}/Boot
$ cp arch/arm64/boot/dts/freescale/<dtb_file>.dtb /media/${USER}/Boot
```
## Root File System

A quick alternative to deploy a `rootfs` is to use a `pre-built` system, download it from [here](http://yb2.am.freescale.net/build-output/Linux_IMX_MX8_BETA/79/fsl-imx-xwayland/).

Uncompress the file:

```bash
$ bunzip2 <image>.rootfs.tar.bz2
$ tar -xf <image>.rootfs.tar -C /media/${USER}/rootfs
```

## Building RFS with Yocto

To get the BSP you need to have repo utility installed (*only need to do this once*):

```bash
$ mkdir ~/bin
$ curl http://commondatastorage.googleapis.com/git-repo-downloads/repo > ~/bin/repo
$ chmod a+x ~/bin/repo
$ PATH=${PATH}:~/bin
```
### i.MX8QM LPDDR4 ARM2

Download the `Yocto Project Environment` into your directory:

```bash
$ mkdir fsl-arm-yocto-bsp
$ cd fsl-arm-yocto-bsp
$ repo init -u https://source.codeaurora.org/external/imx/imx-manifest -b imx-linux-morty -m imx-4.9.51-8qm_beta1.xml
$ repo sync
```
The configuration for i.MX8QM LPDDR4 ARM2 is detailed below:

```bash
$ MACHINE=imx8qmlpddr4arm2 DISTRO=fsl-imx-wayland source ./fsl-setup-release.sh -b bld-wayland
```
Just run the following command for creating the image (this may take a while):

```bash
$ bitbake core-image-minimal
```
After this step is completed, flash the image into the SD card:

```bash
$ cd tmp/deploy/images/imx8qmlpddr4arm2/
$ dd if=core-image-minimal-imx8qmlpddr4arm2-20171212175528.rootfs.sdcard of=/dev/<your_device> status=progress && sync
```
*Other boards will need to wait for a few months.*

Download PDF file from [here]({{site.url}}{{site.baseurl}}/assets/pdf/Getting_Started_Source_Code_iMX8_v0.3.pdf).