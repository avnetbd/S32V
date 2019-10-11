## Introduction of AVB Video Listener Application ##
Please note that this application needs Linux BSP to be patched. Provided Linux BSP is already patched.

#### HW requirements ####
 * Supported boards:
   * S32V234-EVB 700-29288 REV X2
   * NXP BlueBox Mini
 * 4 x h264 camera TODO: @Daniel provide type and link where to buy
 * 4 x jpeg camera TODO: @Daniel provide type and link where to buy

#### SW requirements ####
Following kernel drivers are needed to run Video Listener:
 * csi.ko
 * fdma.ko
 * h264dcd.ko
 * jpegdcd.ko
 * oal_cma.ko
 * seq.ko
 * sm_drv.ko

#### Demo description ####
Video Listener demo application can be found in s32v234sdk/demos/avb. There are three variants:
 * avb_isp_jpeg_4stream
   * Playback of four mjpeg AVB video streams
   * Supported camera resolution is 1280x800 and it was tested with frame rate 25 fps
 * avb_isp_h264_4stream
   * Playback of four h.264 AVB video streams with NAL format and I-frames only
   * Supported camera resolution is 1280x960 and frame rate 10 to 30 fps
 * avb_isp_h264_1stream
   * Playback of four h.264 AVB video streams with NAL format and I-frames only
   * Supported camera resolution is 1280x960 and frame rate 10 to 30 fps
   * This variant is affected by known issue, image is damaged by frequent glitches and the stream is often
     restarted (VSDK-1608).
The variants share most of the code. Shared code can be found in s32v234sdk/demos/avb/common.

Folder [s32v234sdk/demos/avb/extern/firmware](extern/firmware) contains Video Listener streaming firmware and its header files which
must not be modified. There are 3 variants of VideoListener firmware binaries:
 * jpeg/theA5App.bin
   * Shall be used with avb_isp_jpeg_4stream demo
   * Tested with camera: NXP, MPC5606E
 * h264prep/theA5App.bin
   * Shall be used with both h264 demos
   * Tested with camera: First Sensor Mobility GmbH, VK_DC3K-1-ETH-190-IRC-CON-HSU-S
   * This firmware preprocesses the First Sensor camera streams to make it compatible with hardware decoder on s32v234.
 * h264/theA5App.bin
   * Shall be used with both h264 demos
   * It was not tested
   * This firmware does not preprocess the streams. It supports generic h264 camera with NAL format.

The firmware is being loaded by sm_drv.ko and it must be in /lib/firmware while sm_drv.ko is being inserted.

Configure the camera to tag the stream packets with VLAN tags.
VLAN priority used for video streams must not be used for anything else in the s32v234.
Here is how camera VLANs shall be configured for prebuilt Linux environment and default configuration:
 * VLAN priority 2
 * VLAN IDs for cameras connected to EVB via AUTO_ENET ports:
   * AUTO_ENET_P0 port: VLAN ID 110
   * AUTO_ENET_P1 port: VLAN ID 111
   * AUTO_ENET_P4 port: VLAN ID 112
   * AUTO_ENET_P5 port: VLAN ID 113
 * VLAN IDs for cameras connected to EVB via the Gigabit Ethernet port:
   * VLAN ID 100
 * VLAN IDs for cameras connected to BlueBox via AUTO ETH ports:
   * AUTO ETH P1 port: VLAN ID 110
   * AUTO ETH P2 port: VLAN ID 111
   * AUTO ETH P3 port: VLAN ID 112
   * AUTO ETH P4 port: VLAN ID 113

Configure the Stream ID of the cameras:
 * Each camera must have its own AVB stream ID
 * The stream ID of the camera must match option in vid_list.cfg: score set strm_stream_id STREAM_INDEX STREAM_ID

Application is using same build system as all VSDK demos, but in build target directory (build-v234ce-gnu-linux-*)
you can find additional file vid_list.cfg, which contains run-time configuration needed by the application.
It shall be put to rootfs/etc/ or it must be in current working directory while starting the application.

## How do I get set up ##
##### Building application commands #####
variables: VSDK_DIR, VARIANT

Note that build-v234ce-gnu-linux-d (debug) can be used instead of build-v234ce-gnu-linux-o (optimized)
``` bash
cd $VSDK_DIR/demos/avb/$VARIANT/build-v234ce-gnu-linux-o/
make allsub
```

##### Installing application commands #####
variables: VSDK_DIR, VARIANT, STREAM_TYPE, TARGET_ROOTFS, DEMO_DIR
``` bash
mkdir $TARGET_ROOTFS/$DEMO_DIR
cp $VSDK_DIR/demos/avb/$VARIANT/build-v234ce-gnu-linux-o/vid_list.cfg $TARGET_ROOTFS/etc/
cp $VSDK_DIR/demos/avb/$VARIANT/build-v234ce-gnu-linux-o/*.elf $TARGET_ROOTFS/$DEMO_DIR
cp $VSDK_DIR/demos/avb/extern/firmware/$STREAM_TYPE/theA5App.bin $TARGET_ROOTFS/lib/firmware/
```


##### Running application commands (on target board with provided Linux environment) #####
variables: VARIANT, DEMO_DIR
``` bash
# Configure Ethernet interface with ifconfig command
ifconfig eth0 allmulti
ifconfig eth0 up
# Alternatively, configure Ethernet interface with ip command
ip link set dev eth0 allmulticast on
ip link set dev eth0 up
# Load firmware
insmod <PATH TO>sm_drv.ko # should be in /vsdk/demos or /s32v234 directory
# Run the demo
cd /$DEMO_DIR
./$VARIANT.elf
```

## Details for custom integration of Video Listener ##

#### Patches in s32v234_sdk/os folder ####
Linux patches must be applied and Linux must be rebuilt before build of any kernel modules. Supported version is BSP16.
 * 0001-s32v234-dts-Add-VSDK-specific-configuration.patch
   * adds support for VSDK hardware, adds 1 MB of reserved memory
   * shall be applied on Linux version BSP16
   * no modifications shall be done on this patch
 * 0002-fec-add-VSDK-specific-configuration.patch   
   * modifies fec driver for Ethernet adapter
   * shall be applied on Linux version BSP16
   * no modifications shall be done on this patch
 * 0001-secure-monitor-enable-Secure-Monitor-support.patch
   * enables Secure Monitor support and changes h264 decoder clock (workaround for issue VSDK-1608 - Green stripes in decoded image in avb h264 demo)
   * shall be applied on u-boot version BSP16
   * no modifications shall be done on this patch
 * 0001-vlan-add-vlan-for-VSDK-specific-configuration.patch
   * provides default VLAN configuration for the AVB streams
   * shall be applied on sja1105x switch repository version BSP16
   * this patch (or patched files) can be modified to customize VLAN configuration

#### Secure monitor driver ####
Source code of **sm_drv** is available at: https://source.codeaurora.org/external/autobsps32/extra/sm_drv/
It can be build with command:
``` bash
make KERNELDIR=<PATH TO LINUX> ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-
```
#### VLAN and Ethernet ports configuration ####
 * VLAN priority in video stream must match configuration in file vid_list.cfg, option: score vlan_filt_add
 * VLAN ID in video stream must match Ethernet switch configuration. It may be different for each camera.
   * For each camera:
     * Choose a VLAN ID and Ethernet port to connect the camera to.
     * On the switch, add a VLAN domain with chosen VLAN ID and add the camera port and CPU port. Make sure the CPU port
       is tagged.
 * Default VLAN configuration is available in form of patch: bsp16-sja1105x-vlan2_keep_tag.patch
 * There are two chained SJA1105Q switches on the EVB, sw0 and sw1:
   * Port 0 of sw1 is connected to port 3 of sw0
   * Ports 1 to 4 of sw0 are connected to AUTO_ENET_P0 to AUTO_ENET_P3 connectors
   * Port 0 of sw0 is connected to S32V234 CPU
   * Ports 1 and 2 of sw1 are connected to AUTO_ENET_P4 and AUTO_ENET_P5 connectors
   * Port 4 of sw1 is connected to gigabit Ethernet connector

If needed, Video Listener AVB firmware sources are available as separate product:
S32V23 AVB Video Listener, version RTM-1.1.0
