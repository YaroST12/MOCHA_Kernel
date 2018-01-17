KERNEL_DIR=$PWD
export KBUILD_BUILD_HOST="Sup-Mint"
export KBUILD_BUILD_USER="Yaro330"
export ARCH=arm
make O=out f1xy_defconfig
make O=out -j16 ARCH=arm CROSS_COMPILE=/home/$USER/build/mocha/gcc-prebuilts/bin/arm-linaro-linux-androideabi- zImage
cp $KERNEL_DIR/out/arch/arm/boot/zImage $KERNEL_DIR/build/zImage
